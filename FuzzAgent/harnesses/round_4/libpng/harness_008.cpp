#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <zlib.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

namespace harness_008 {

// Error handling callbacks for libpng
static void pngtest_error(png_structp png_ptr, png_const_charp msg) {
    (void)png_ptr;
    (void)msg;
    // Do nothing - just return
}

static void pngtest_warning(png_structp png_ptr, png_const_charp msg) {
    (void)png_ptr;
    (void)msg;
    // Do nothing - just return
}

// Custom read function for memory buffers
struct png_memory_buffer {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void pngtest_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_memory_buffer* buffer = (png_memory_buffer*)png_get_io_ptr(png_ptr);
    
    if (buffer->offset + length > buffer->size) {
        png_error(png_ptr, "Read beyond buffer");
        return;
    }
    
    memcpy(data, buffer->data + buffer->offset, length);
    buffer->offset += length;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for PNG operations
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create memory buffer for PNG operations
    png_memory_buffer read_buffer;
    read_buffer.data = data;
    read_buffer.size = size;
    read_buffer.offset = 0;
    
    // Step 1: Create PNG read structure - TARGET HELPER: png_create_read_struct
    png_structp png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        pngtest_error, 
        pngtest_warning);
    
    if (!png_ptr) {
        return 0;
    }
    
    // Step 2: Create info structure - TARGET HELPER: png_create_info_struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Step 3: Set up custom read functions - TARGET HELPER: png_set_read_fn
    png_set_read_fn(png_ptr, &read_buffer, pngtest_read_data);
    
    // Set error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Step 4: Configure CRC checking behavior - PRIMARY TARGET: png_set_crc_action
    // Consume CRC action parameters from fuzzer input
    int crit_action = fdp.ConsumeIntegralInRange<int>(PNG_CRC_DEFAULT, PNG_CRC_NO_CHANGE);
    int ancil_action = fdp.ConsumeIntegralInRange<int>(PNG_CRC_DEFAULT, PNG_CRC_NO_CHANGE);
    png_set_crc_action(png_ptr, crit_action, ancil_action);
    
    // Step 5: Set compression buffer size - PRIMARY TARGET: png_set_compression_buffer_size
    // Consume buffer size from fuzzer input (reasonable range: 1KB to 1MB)
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1024, 1048576);
    png_set_compression_buffer_size(png_ptr, buffer_size);
    
    // Try to read PNG info - this will fail if the data isn't valid PNG,
    // but that's OK for fuzzing as we're testing error paths too
    png_read_info(png_ptr, info_ptr);
    
    // Get image dimensions after reading info
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Only proceed if we have valid dimensions
    if (width > 0 && height > 0 && width < 4096 && height < 4096) {
        // Calculate row bytes for allocation
        png_size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        
        // Limit row size to prevent excessive memory usage
        if (row_bytes > 0 && row_bytes < 65536) {
            // Allocate row buffers for reading
            png_bytep row_pointers[height];
            for (png_uint_32 y = 0; y < height; y++) {
                row_pointers[y] = (png_bytep)png_malloc(png_ptr, row_bytes);
                if (!row_pointers[y]) {
                    // Clean up already allocated rows
                    for (png_uint_32 i = 0; i < y; i++) {
                        png_free(png_ptr, row_pointers[i]);
                    }
                    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
                    return 0;
                }
            }
            
            // Step 6: Read rows one by one - PRIMARY TARGET: png_read_rows
            // Read rows in chunks to test different num_rows values
            png_uint_32 rows_to_read = fdp.ConsumeIntegralInRange<png_uint_32>(1, height);
            png_uint_32 start_row = 0;
            
            while (start_row < height) {
                png_uint_32 current_chunk = (start_row + rows_to_read <= height) ? rows_to_read : height - start_row;
                
                // Prepare row pointers for the current chunk
                png_bytep* chunk_rows = &row_pointers[start_row];
                
                // Read the chunk of rows
                png_read_rows(png_ptr, chunk_rows, nullptr, current_chunk);
                
                start_row += current_chunk;
                
                // Consume new rows_to_read value for next iteration
                if (start_row < height) {
                    rows_to_read = fdp.ConsumeIntegralInRange<png_uint_32>(1, height - start_row);
                }
            }
            
            // Clean up row buffers
            for (png_uint_32 y = 0; y < height; y++) {
                png_free(png_ptr, row_pointers[y]);
            }
        }
        
        // Step 7: Test APNG frame reading if APNG is supported
        // Check if we have APNG animation control
        png_uint_32 num_frames = 0, num_plays = 0;
        if (png_get_acTL(png_ptr, info_ptr, &num_frames, &num_plays) && num_frames > 0) {
            // Try to read frame head - PRIMARY TARGET: png_read_frame_head
            // This requires APNG setup which we may not have, but we can still try
            // to call the function to exercise it
            #ifdef PNG_READ_APNG_SUPPORTED
            png_read_frame_head(png_ptr, info_ptr);
            #endif
        }
        
        // Step 8: Test RFC1123 buffer conversion for time metadata - PRIMARY TARGET: png_convert_to_rfc1123_buffer
        // Check if we have time metadata
        png_timep mod_time;
        if (png_get_tIME(png_ptr, info_ptr, &mod_time)) {
            char rfc1123_buffer[29];
            // Convert time to RFC1123 format - PRIMARY TARGET: png_convert_to_rfc1123_buffer
            int convert_result = png_convert_to_rfc1123_buffer(rfc1123_buffer, mod_time);
            (void)convert_result; // Result can be ignored for fuzzing
        }
        
        // Try to read the rest of the PNG (end chunk)
        png_read_end(png_ptr, info_ptr);
    }
    
    // Step 9: Clean up - TARGET HELPER: png_destroy_read_struct
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}

} // namespace harness_008
