#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>

namespace harness_005 {

// Custom write function for libpng that writes to memory buffer
void user_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Handle zero-length writes according to libpng's API contract
    if (length == 0) {
        return;  // Nothing to write
    }
    
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) return;
    
    // Cast to vector pointer and append data
    std::vector<uint8_t>* buffer = static_cast<std::vector<uint8_t>*>(io_ptr);
    buffer->insert(buffer->end(), data, data + length);
}

// Flush function (required but can be empty for memory buffers)
void user_flush_data(png_structp png_ptr) {
    (void)png_ptr;
    // Nothing to flush for memory buffer
}

// Error handling functions
void user_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do nothing - just return
}

void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Do nothing - just return
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for write transformation testing
    // Required: width, height, bit_depth, color_type, quantization params, filter flags
    if (size < 50) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume image parameters from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    int bit_depth = fdp.ConsumeIntegralInRange<int>(1, 16);
    int color_type = fdp.ConsumeIntegralInRange<int>(0, 6);
    int interlace_type = fdp.ConsumeIntegralInRange<int>(0, 1);
    int compression_type = fdp.ConsumeIntegralInRange<int>(0, 1);
    int filter_type = fdp.ConsumeIntegralInRange<int>(0, 5);
    
    // Create PNG write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                  NULL, 
                                                  user_error_fn, 
                                                  user_warning_fn);
    if (png_ptr == NULL) {
        return 0;
    }
    
    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }
    
    // Set up error handling using setjmp/longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Set up output buffer for writing
    std::vector<uint8_t> output_buffer;
    png_set_write_fn(png_ptr, &output_buffer, user_write_data, user_flush_data);
    
    // Set image header information (Required Setup API)
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);
    
    // Target API 1: png_set_crc_action (32 undiscovered branches)
    // Configure CRC error handling behavior based on fuzzer input
    int crit_action = fdp.ConsumeIntegralInRange<int>(0, 5);
    int ancil_action = fdp.ConsumeIntegralInRange<int>(0, 5);
    png_set_crc_action(png_ptr, crit_action, ancil_action);
    
    // Target API 2: png_set_quantize (130 undiscovered branches)
    // Set up color quantization parameters for PNG writing
    if (fdp.remaining_bytes() >= sizeof(png_color) * 256 + sizeof(png_uint_16) * 256 + 4) {
        // Create palette for quantization
        int num_palette = fdp.ConsumeIntegralInRange<int>(1, 256);
        int maximum_colors = fdp.ConsumeIntegralInRange<int>(1, 256);
        int full_quantize = fdp.ConsumeIntegralInRange<int>(0, 1);
        
        // Allocate palette array
        std::vector<png_color> palette(num_palette);
        for (int i = 0; i < num_palette; i++) {
            if (fdp.remaining_bytes() >= 3) {
                palette[i].red = fdp.ConsumeIntegral<uint8_t>();
                palette[i].green = fdp.ConsumeIntegral<uint8_t>();
                palette[i].blue = fdp.ConsumeIntegral<uint8_t>();
            } else {
                palette[i].red = palette[i].green = palette[i].blue = 0;
            }
        }
        
        // Allocate histogram array (optional, can be NULL)
        std::vector<png_uint_16> histogram;
        if (fdp.ConsumeBool() && fdp.remaining_bytes() >= sizeof(png_uint_16) * 256) {
            histogram.resize(256);
            for (int i = 0; i < 256; i++) {
                histogram[i] = fdp.ConsumeIntegral<png_uint_16>();
            }
        }
        
        // Call png_set_quantize with the prepared parameters
        png_set_quantize(png_ptr, 
                        palette.data(), 
                        num_palette, 
                        maximum_colors,
                        histogram.empty() ? NULL : histogram.data(),
                        full_quantize);
    }
    
    // Target API 3: png_set_filter (61 undiscovered branches)
    // Configure compression filter settings for PNG writing
    int filter_method = 0;  // Currently only valid value is 0
    int filters = fdp.ConsumeIntegral<int>();
    
    // Mask filters to valid values
    filters &= (PNG_NO_FILTERS | PNG_FILTER_NONE | PNG_FILTER_SUB | 
                PNG_FILTER_UP | PNG_FILTER_AVG | PNG_FILTER_PAETH);
    png_set_filter(png_ptr, filter_method, filters);
    
    // Write PNG header information
    png_write_info(png_ptr, info_ptr);
    
    // Prepare image data for writing
    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    if (row_bytes == 0 || height == 0) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Check if we have enough fuzzer data for at least one row
    if (fdp.remaining_bytes() < row_bytes) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Allocate row pointers for image data
    png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)png_malloc(png_ptr, row_bytes);
        
        // Fill row with fuzzer data if available
        if (fdp.remaining_bytes() >= row_bytes) {
            std::vector<uint8_t> row_data = fdp.ConsumeBytes<uint8_t>(row_bytes);
            memcpy(row_pointers[y], row_data.data(), row_bytes);
        } else {
            // Fill with zeros if not enough data
            memset(row_pointers[y], 0, row_bytes);
        }
    }
    
    // Target API 4: png_write_rows (100 undiscovered branches)
    // Write image data rows with transformations applied
    png_uint_32 rows_to_write = height;
    
    // Consume how many rows to write at once (test different batch sizes)
    png_uint_32 batch_size = fdp.ConsumeIntegralInRange<png_uint_32>(1, height);
    if (batch_size == 0) batch_size = 1;
    
    png_uint_32 rows_written = 0;
    while (rows_written < rows_to_write) {
        png_uint_32 current_batch = (rows_to_write - rows_written < batch_size) ? 
                                    (rows_to_write - rows_written) : batch_size;
        
        // Prepare pointer array for the current batch
        png_bytepp batch_rows = row_pointers + rows_written;
        
        // Write the batch of rows
        png_write_rows(png_ptr, batch_rows, current_batch);
        
        rows_written += current_batch;
        // Consume a new batch size for variety
        if (fdp.remaining_bytes() >= sizeof(png_uint_32)) {
            png_uint_32 remaining_rows = rows_to_write - rows_written;
            if (remaining_rows > 0) {
                batch_size = fdp.ConsumeIntegralInRange<png_uint_32>(1, remaining_rows);
            }
        }
    }
    
    // Finish writing
    png_write_end(png_ptr, info_ptr);
    
    // Free allocated rows
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);
    
    // Clean up (Cleanup API)
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}

} // namespace harness_005
