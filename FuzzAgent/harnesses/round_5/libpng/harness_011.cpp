#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "png.h"

namespace harness_011 {


// Simple error handling for fuzzing
static void pngtest_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void pngtest_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Custom read function for reading from memory buffer
struct png_read_data {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void png_read_from_buffer(png_structp png_ptr, png_bytep out, png_size_t length) {
    png_read_data* rd = (png_read_data*)png_get_io_ptr(png_ptr);
    if (rd->offset + length > rd->size) {
        png_error(png_ptr, "Read beyond buffer");
    }
    memcpy(out, rd->data + rd->offset, length);
    rd->offset += length;
}

// Harness targeting traditional PNG row-by-row reading operations
// Specifically targets: png_read_rows (62 undiscovered branches), png_read_row,
// png_read_image, png_read_update_info, png_read_start_row, png_read_end_row
// Focuses on low-level sequential reading patterns rather than high-level APIs
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) {
        // Need enough data for PNG signature and configuration
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume reading mode from fuzzer input
    // 0: png_read_rows with both row and display_row
    // 1: png_read_rows with row only  
    // 2: png_read_rows with display_row only
    // 3: png_read_row individual rows
    // 4: png_read_image (for comparison)
    // 5: Mixed mode with multiple API calls
    uint8_t reading_mode = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Consume interlace handling configuration
    uint8_t interlace_config = fdp.ConsumeIntegral<uint8_t>();
    bool use_interlace_handling = (interlace_config & 0x01) != 0;
    
    // Consume transformation configuration
    uint8_t transform_flags = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume row batch size for png_read_rows
    uint32_t max_batch_size = fdp.ConsumeIntegralInRange<uint32_t>(1, 100);
    
    // Consume remaining bytes for PNG data
    std::vector<uint8_t> png_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (png_data.size() < 8) {
        // PNG signature is 8 bytes minimum
        return 0;
    }
    
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_infop end_info = NULL;
    
    // Create read structure
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, pngtest_error, pngtest_warning);
    if (png_ptr == NULL) {
        return 0;
    }
    
    // Set up error handling with setjmp - MUST be done before any libpng calls that can longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during PNG processing
        if (png_ptr) {
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        }
        return 0;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    end_info = png_create_info_struct(png_ptr);
    if (end_info == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Set up custom read function to read from our buffer
    png_read_data read_data = {png_data.data(), png_data.size(), 0};
    png_set_read_fn(png_ptr, &read_data, png_read_from_buffer);
    
    // Tell libpng we haven't read any signature bytes yet
    png_set_sig_bytes(png_ptr, 0);
    
    // Read PNG info (this reads the signature and IHDR)
    png_read_info(png_ptr, info_ptr);
    
    // Get image dimensions
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Apply transformations based on fuzzer input
    if (transform_flags & 0x01) png_set_expand(png_ptr);
    if (transform_flags & 0x02) png_set_gray_to_rgb(png_ptr);
    if (transform_flags & 0x04) png_set_rgb_to_gray(png_ptr, 1, 0.299, 0.587);
    if (transform_flags & 0x08) png_set_strip_16(png_ptr);
    if (transform_flags & 0x10) png_set_strip_alpha(png_ptr);
    if (transform_flags & 0x20) png_set_bgr(png_ptr);
    if (transform_flags & 0x40) png_set_swap(png_ptr);
    if (transform_flags & 0x80) png_set_packing(png_ptr);
    
    // Update info after transformations
    png_read_update_info(png_ptr, info_ptr);
    
    // Get updated rowbytes after transformations
    png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    // Setup interlace handling if requested
    int num_passes = 1;
    if (use_interlace_handling) {
        num_passes = png_set_interlace_handling(png_ptr);
        // Call png_start_read_image to initialize for interlaced reading
        png_start_read_image(png_ptr);
    }
    
    // Allocate row buffers based on reading mode
    png_bytep* row_pointers = NULL;
    png_bytep* display_row_pointers = NULL;
    size_t rows_to_allocate = 0;
    
    if (height > 0 && height < 10000) { // Reasonable limit for fuzzing
        rows_to_allocate = height;
        
        // Allocate row pointers for image data
        row_pointers = (png_bytep*)png_malloc(png_ptr, rows_to_allocate * sizeof(png_bytep));
        if (row_pointers != NULL) {
            for (png_uint_32 i = 0; i < rows_to_allocate; i++) {
                row_pointers[i] = (png_bytep)png_malloc(png_ptr, rowbytes);
                if (row_pointers[i] != NULL) {
                    memset(row_pointers[i], 0, rowbytes);
                }
            }
        }
        
        // Allocate display row pointers for progressive display
        if (reading_mode == 0 || reading_mode == 2 || reading_mode == 5) {
            display_row_pointers = (png_bytep*)png_malloc(png_ptr, rows_to_allocate * sizeof(png_bytep));
            if (display_row_pointers != NULL) {
                for (png_uint_32 i = 0; i < rows_to_allocate; i++) {
                    display_row_pointers[i] = (png_bytep)png_malloc(png_ptr, rowbytes);
                    if (display_row_pointers[i] != NULL) {
                        memset(display_row_pointers[i], 0, rowbytes);
                    }
                }
            }
        }
    }
    
    // Main row-by-row reading logic based on reading mode
    switch (reading_mode) {
        case 0: // png_read_rows with both row and display_row
            if (row_pointers != NULL && display_row_pointers != NULL && height > 0) {
                // Read rows in batches
                for (png_uint_32 start_row = 0; start_row < height; ) {
                    uint32_t batch_size = std::min(max_batch_size, height - start_row);
                    if (batch_size == 0) break;
                    
                    // Prepare pointers for this batch
                    png_bytep* batch_rows = &row_pointers[start_row];
                    png_bytep* batch_display_rows = &display_row_pointers[start_row];
                    
                    png_read_rows(png_ptr, batch_rows, batch_display_rows, batch_size);
                    start_row += batch_size;
                }
            }
            break;
            
        case 1: // png_read_rows with row only
            if (row_pointers != NULL && height > 0) {
                // Read rows in batches
                for (png_uint_32 start_row = 0; start_row < height; ) {
                    uint32_t batch_size = std::min(max_batch_size, height - start_row);
                    if (batch_size == 0) break;
                    
                    png_bytep* batch_rows = &row_pointers[start_row];
                    png_read_rows(png_ptr, batch_rows, NULL, batch_size);
                    start_row += batch_size;
                }
            }
            break;
            
        case 2: // png_read_rows with display_row only
            if (display_row_pointers != NULL && height > 0) {
                // Read rows in batches
                for (png_uint_32 start_row = 0; start_row < height; ) {
                    uint32_t batch_size = std::min(max_batch_size, height - start_row);
                    if (batch_size == 0) break;
                    
                    png_bytep* batch_display_rows = &display_row_pointers[start_row];
                    png_read_rows(png_ptr, NULL, batch_display_rows, batch_size);
                    start_row += batch_size;
                }
            }
            break;
            
        case 3: // png_read_row individual rows
            if (row_pointers != NULL && height > 0) {
                // Read each row individually using png_read_row
                for (png_uint_32 row_num = 0; row_num < height; row_num++) {
                    png_read_row(png_ptr, row_pointers[row_num], NULL);
                }
                
                // Also test with display_row
                if (display_row_pointers != NULL) {
                    // Reset and read again with display_row
                    png_set_sig_bytes(png_ptr, 0);
                    png_read_data read_data2 = {png_data.data(), png_data.size(), 0};
                    png_set_read_fn(png_ptr, &read_data2, png_read_from_buffer);
                    png_read_info(png_ptr, info_ptr);
                    png_read_update_info(png_ptr, info_ptr);
                    
                    for (png_uint_32 row_num = 0; row_num < height; row_num++) {
                        png_read_row(png_ptr, row_pointers[row_num], display_row_pointers[row_num]);
                    }
                }
            }
            break;
            
        case 4: // png_read_image (for comparison with row-by-row reading)
            if (row_pointers != NULL && height > 0) {
                png_read_image(png_ptr, row_pointers);
            }
            break;
            
        case 5: // Mixed mode with multiple API calls
            if (row_pointers != NULL && height > 0) {
                // Test different API sequences
                if (height >= 3) {
                    // Read first row with png_read_row
                    png_read_row(png_ptr, row_pointers[0], NULL);
                    
                    // Read next 2 rows with png_read_rows
                    if (height >= 3) {
                        png_bytep* batch_rows = &row_pointers[1];
                        png_read_rows(png_ptr, batch_rows, NULL, 2);
                    }
                    
                    // Read remaining rows with png_read_image
                    if (height > 3) {
                        png_bytep* remaining_rows = &row_pointers[3];
                        // Note: png_read_image expects to read ALL rows, so we need to handle this differently
                        // For simplicity, just read the rest with png_read_rows
                        for (png_uint_32 start_row = 3; start_row < height; ) {
                            uint32_t batch_size = std::min(max_batch_size, height - start_row);
                            if (batch_size == 0) break;
                            
                            png_bytep* batch_rows = &row_pointers[start_row];
                            png_read_rows(png_ptr, batch_rows, NULL, batch_size);
                            start_row += batch_size;
                        }
                    }
                }
            }
            break;
    }
    
    // Read end of PNG file
    png_read_end(png_ptr, end_info);
    
    // Clean up allocated memory
    if (row_pointers != NULL) {
        for (png_uint_32 i = 0; i < rows_to_allocate; i++) {
            if (row_pointers[i] != NULL) {
                png_free(png_ptr, row_pointers[i]);
            }
        }
        png_free(png_ptr, row_pointers);
    }
    
    if (display_row_pointers != NULL) {
        for (png_uint_32 i = 0; i < rows_to_allocate; i++) {
            if (display_row_pointers[i] != NULL) {
                png_free(png_ptr, display_row_pointers[i]);
            }
        }
        png_free(png_ptr, display_row_pointers);
    }
    
    // Destroy PNG structures
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    
    return 0;
}

} // namespace harness_011
