#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "png.h"

namespace harness_005 {


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

// Custom write function for writing to memory buffer
struct png_write_data {
    uint8_t* buffer;
    size_t size;
    size_t capacity;
    size_t offset;
};

static void png_write_to_buffer(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_write_data* wd = (png_write_data*)png_get_io_ptr(png_ptr);
    
    // Check if we need to expand buffer
    if (wd->offset + length > wd->capacity) {
        // For fuzzing, we don't actually need to store all data
        // Just simulate successful write
        wd->offset += length;
        return;
    }
    
    // Write data to buffer
    if (wd->buffer != NULL && wd->offset + length <= wd->capacity) {
        memcpy(wd->buffer + wd->offset, data, length);
    }
    wd->offset += length;
}

static void png_flush_buffer(png_structp png_ptr) {
    (void)png_ptr;
    // Nothing to do for memory buffer
}

// Harness targeting PNG writing transformation APIs with 80+ undiscovered branches
// Specifically targets pngwtran.c (0% coverage) and pngget.c (4.08% coverage)
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 100) {
        // Need enough data for configuration and image data
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume image parameters from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 512);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 512);
    int bit_depth = fdp.PickValueInArray({1, 2, 4, 8, 16});
    int color_type = fdp.PickValueInArray({
        PNG_COLOR_TYPE_GRAY,
        PNG_COLOR_TYPE_GRAY_ALPHA,
        PNG_COLOR_TYPE_PALETTE,
        PNG_COLOR_TYPE_RGB,
        PNG_COLOR_TYPE_RGB_ALPHA
    });
    int interlace_type = fdp.PickValueInArray({
        PNG_INTERLACE_NONE,
        PNG_INTERLACE_ADAM7
    });
    int compression_type = PNG_COMPRESSION_TYPE_BASE;
    int filter_type = PNG_FILTER_TYPE_BASE;
    
    // Consume configuration flags for transformation APIs
    uint8_t config_flags = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume remaining bytes for image data and other parameters
    std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (remaining_data.size() < 50) {
        return 0; // Need enough data for various API parameters
    }
    
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    
    // Create write structure
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                      NULL, pngtest_error, pngtest_warning);
    if (png_ptr == NULL) {
        return 0;
    }
    
    // Set up error handling with setjmp - MUST be done before any libpng calls that can longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during PNG processing
        if (png_ptr) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
        }
        return 0;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }
    
    // Set up custom write function
    png_write_data write_data;
    const size_t buffer_size = 64 * 1024; // 64KB buffer for fuzzing
    write_data.buffer = (uint8_t*)malloc(buffer_size);
    write_data.size = 0;
    write_data.capacity = buffer_size;
    write_data.offset = 0;
    
    png_set_write_fn(png_ptr, &write_data, png_write_to_buffer, png_flush_buffer);
    
    // Set image dimensions and color type
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);
    
    // ============================================================
    // TARGET: png_set_crc_action (32 undiscovered branches)
    // ============================================================
    int crit_action = fdp.ConsumeIntegralInRange<int>(0, 5);
    int ancil_action = fdp.ConsumeIntegralInRange<int>(0, 5);
    png_set_crc_action(png_ptr, crit_action, ancil_action);
    
    // ============================================================
    // TARGET: png_set_filter (64 undiscovered branches)
    // ============================================================
    int filter_method = 0; // Currently only 0 is valid
    int filters = 0;
    
    // Build filter mask from config flags
    if (config_flags & 0x01) filters |= PNG_FILTER_NONE;
    if (config_flags & 0x02) filters |= PNG_FILTER_SUB;
    if (config_flags & 0x04) filters |= PNG_FILTER_UP;
    if (config_flags & 0x08) filters |= PNG_FILTER_AVG;
    if (config_flags & 0x10) filters |= PNG_FILTER_PAETH;
    if (config_flags & 0x20) filters |= PNG_ALL_FILTERS;
    if (config_flags & 0x40) filters |= PNG_FAST_FILTERS;
    if (config_flags & 0x80) filters |= PNG_NO_FILTERS;
    
    png_set_filter(png_ptr, filter_method, filters);
    
    // ============================================================
    // TARGET: png_set_quantize (133 undiscovered branches) - READ API but included for completeness
    // Note: png_set_quantize is actually a read API, but we'll test it anyway
    // ============================================================
#ifdef PNG_READ_QUANTIZE_SUPPORTED
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        // Create a simple palette for testing
        png_color palette[256];
        int num_palette = fdp.ConsumeIntegralInRange<int>(1, 256);
        int maximum_colors = fdp.ConsumeIntegralInRange<int>(1, 256);
        
        // Fill palette with random colors
        for (int i = 0; i < num_palette && i < 256; i++) {
            palette[i].red = fdp.ConsumeIntegral<uint8_t>();
            palette[i].green = fdp.ConsumeIntegral<uint8_t>();
            palette[i].blue = fdp.ConsumeIntegral<uint8_t>();
        }
        
        // Create histogram if enough data
        png_uint_16 histogram[256];
        int full_quantize = fdp.ConsumeBool() ? 1 : 0;
        
        png_set_quantize(png_ptr, palette, num_palette, maximum_colors,
                        histogram, full_quantize);
    }
#endif
    
    // ============================================================
    // TARGET: png_set_iCCP (54 undiscovered branches)
    // ============================================================
#ifdef PNG_WRITE_iCCP_SUPPORTED
    if (remaining_data.size() > 20) {
        const char* name = "fuzzer_profile";
        int compression_type_icc = PNG_COMPRESSION_TYPE_BASE;
        
        // Use a portion of remaining data for ICC profile
        size_t icc_size = std::min<size_t>(remaining_data.size() / 2, 1024);
        std::vector<png_byte> icc_data(icc_size);
        
        // Copy data for ICC profile
        for (size_t i = 0; i < icc_size; i++) {
            icc_data[i] = remaining_data[i % remaining_data.size()];
        }
        
        png_set_iCCP(png_ptr, info_ptr, (png_const_charp)name,
                    compression_type_icc, icc_data.data(), icc_size);
    }
#endif
    
    // Write info to PNG
    png_write_info(png_ptr, info_ptr);
    
    // ============================================================
    // TARGET: png_write_rows (80 undiscovered branches)
    // Test different ways of writing rows
    // ============================================================
    if (height > 0 && width > 0) {
        // Calculate rowbytes
        png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
        if (rowbytes == 0) {
            rowbytes = (width * bit_depth * 
                       ((color_type == PNG_COLOR_TYPE_RGB) ? 3 : 
                        (color_type == PNG_COLOR_TYPE_RGBA) ? 4 :
                        (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) ? 2 : 1) + 7) / 8;
        }
        
        // Allocate row pointers
        png_bytep* row_pointers = (png_bytep*)png_malloc(png_ptr, height * sizeof(png_bytep));
        if (row_pointers != NULL) {
            for (png_uint_32 i = 0; i < height; i++) {
                row_pointers[i] = (png_bytep)png_malloc(png_ptr, rowbytes);
                if (row_pointers[i] != NULL) {
                    // Fill row with pattern data from fuzzer input
                    for (png_size_t j = 0; j < rowbytes; j++) {
                        row_pointers[i][j] = remaining_data[(i * rowbytes + j) % remaining_data.size()];
                    }
                }
            }
            
            // Test png_write_rows with different num_rows values
            png_uint_32 num_rows_to_write;
            
            // Strategy 1: Write all rows at once
            if (config_flags & 0x01) {
                num_rows_to_write = height;
                png_write_rows(png_ptr, row_pointers, num_rows_to_write);
            }
            // Strategy 2: Write rows in chunks
            else if (config_flags & 0x02) {
                num_rows_to_write = fdp.ConsumeIntegralInRange<png_uint_32>(1, height);
                png_uint_32 rows_written = 0;
                while (rows_written < height) {
                    png_uint_32 chunk_size = std::min(num_rows_to_write, height - rows_written);
                    png_write_rows(png_ptr, &row_pointers[rows_written], chunk_size);
                    rows_written += chunk_size;
                }
            }
            // Strategy 3: Write single rows with png_write_row
            else {
                for (png_uint_32 i = 0; i < height; i++) {
                    png_write_row(png_ptr, row_pointers[i]);
                }
            }
            
            // Free row pointers
            for (png_uint_32 i = 0; i < height; i++) {
                if (row_pointers[i] != NULL) {
                    png_free(png_ptr, row_pointers[i]);
                }
            }
            png_free(png_ptr, row_pointers);
        }
    }
    
    // Write end of PNG
    png_write_end(png_ptr, info_ptr);
    
    // Clean up
    if (write_data.buffer != NULL) {
        free(write_data.buffer);
    }
    
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}

} // namespace harness_005
