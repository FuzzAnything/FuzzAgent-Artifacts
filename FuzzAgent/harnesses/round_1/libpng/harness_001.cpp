#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_001 {

// Buffer structure for memory I/O writing
struct write_buffer {
    std::vector<uint8_t> data;
    size_t offset;
};

// Callback functions for libpng error handling (same as harness_000)
static void png_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void png_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Custom write function for writing to memory buffer
static void png_write_to_memory(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) {
        png_error(png_ptr, "No IO pointer");
        return;
    }
    
    write_buffer* buffer = (write_buffer*)io_ptr;
    
    // Append data to buffer
    size_t new_size = buffer->data.size() + length;
    buffer->data.resize(new_size);
    memcpy(buffer->data.data() + buffer->offset, data, length);
    buffer->offset += length;
}

// Custom flush function (required but does nothing for memory buffer)
static void png_flush_memory(png_structp png_ptr) {
    (void)png_ptr;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful PNG writing operations
    if (size < 50) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume image parameters from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1024);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1024);
    int bit_depth = fdp.PickValueInArray({1, 2, 4, 8, 16});
    int color_type = fdp.PickValueInArray({
        PNG_COLOR_TYPE_GRAY,
        PNG_COLOR_TYPE_GRAY_ALPHA,
        PNG_COLOR_TYPE_PALETTE,
        PNG_COLOR_TYPE_RGB,
        PNG_COLOR_TYPE_RGB_ALPHA
    });
    int interlace_type = fdp.ConsumeBool() ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE;
    int compression_type = PNG_COMPRESSION_TYPE_BASE;
    int filter_type = PNG_FILTER_TYPE_BASE;
    
    // Prepare write buffer for custom write function
    write_buffer buffer;
    buffer.offset = 0;
    
    // Create write structure
    png_structp png_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        NULL,
        png_error_callback,
        png_warning_callback);
    
    if (png_ptr == NULL) {
        return 0;
    }
    
    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }
    
    // Set up error handling via setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during PNG processing
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Initialize I/O with memory buffer
    png_set_write_fn(png_ptr, &buffer, png_write_to_memory, png_flush_memory);
    
    // Set image header (IHDR)
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);
    
    // Calculate row bytes
    png_size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    
    // Test Strategy 1: Use png_write_png (high-level API)
    {
        // Generate image data for png_write_png
        png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_bytep)png_malloc(png_ptr, row_bytes);
            // Fill with random data from fuzzer input
            std::vector<uint8_t> row_data = fdp.ConsumeBytes<uint8_t>(row_bytes);
            if (row_data.size() < row_bytes) {
                // Not enough data, fill with zeros
                memset(row_pointers[y], 0, row_bytes);
                if (!row_data.empty()) {
                    memcpy(row_pointers[y], row_data.data(), row_data.size());
                }
            } else {
                memcpy(row_pointers[y], row_data.data(), row_bytes);
            }
        }
        
        // Call png_write_png (combines write_info, write_image, write_end)
        png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
        
        // Free row pointers
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
        png_free(png_ptr, row_pointers);
    }
    
    // Reset buffer for next test
    buffer.data.clear();
    buffer.offset = 0;
    
    // Test Strategy 2: Step-by-step writing (alternative to png_write_png)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 100) {
        // Re-create structures for fresh test
        png_destroy_write_struct(&png_ptr, &info_ptr);
        
        png_ptr = png_create_write_struct(
            PNG_LIBPNG_VER_STRING,
            NULL,
            png_error_callback,
            png_warning_callback);
        
        if (png_ptr == NULL) {
            return 0;
        }
        
        info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == NULL) {
            png_destroy_write_struct(&png_ptr, NULL);
            return 0;
        }
        
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return 0;
        }
        
        png_set_write_fn(png_ptr, &buffer, png_write_to_memory, png_flush_memory);
        
        // Set image parameters again
        png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                     interlace_type, compression_type, filter_type);
        
        // Write header
        png_write_info(png_ptr, info_ptr);
        
        // Generate new image data
        png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_bytep)png_malloc(png_ptr, row_bytes);
            std::vector<uint8_t> row_data = fdp.ConsumeBytes<uint8_t>(row_bytes);
            if (row_data.size() < row_bytes) {
                memset(row_pointers[y], 0, row_bytes);
                if (!row_data.empty()) {
                    memcpy(row_pointers[y], row_data.data(), row_data.size());
                }
            } else {
                memcpy(row_pointers[y], row_data.data(), row_bytes);
            }
        }
        
        // Test both png_write_image and png_write_rows
        if (fdp.ConsumeBool()) {
            // Use png_write_image to write all rows at once
            png_write_image(png_ptr, row_pointers);
        } else {
            // Use png_write_rows for row-by-row writing
            png_write_rows(png_ptr, row_pointers, height);
        }
        
        // Write end and cleanup
        png_write_end(png_ptr, info_ptr);
        
        // Free row pointers
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
        png_free(png_ptr, row_pointers);
    }
    
    // Final cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}

} // namespace harness_001
