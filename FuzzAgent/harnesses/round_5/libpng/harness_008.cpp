#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <unistd.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "png.h"

namespace harness_008 {


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

// Harness specifically targeting PNG simplified writing API (pngwrite.c)
// Targeting 175+ undiscovered branches in simplified writing functions
// APIs: png_image_write_to_file, png_image_write_to_memory, png_image_write_to_stdio
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 100) {
        // Need enough data for configuration and image data
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume image configuration from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 256);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 256);
    
    // Choose image format from fuzzer input
    uint8_t format_bits = fdp.ConsumeIntegral<uint8_t>();
    png_uint_32 format = PNG_FORMAT_RGBA;
    
    // Map format bits to PNG formats
    if ((format_bits & 0x01) != 0) format = PNG_FORMAT_RGB;
    if ((format_bits & 0x02) != 0) format = PNG_FORMAT_GRAY;
    if ((format_bits & 0x04) != 0) format = PNG_FORMAT_GA;
    if ((format_bits & 0x08) != 0) format = PNG_FORMAT_LINEAR_RGB;
    if ((format_bits & 0x10) != 0) format = PNG_FORMAT_LINEAR_RGB_ALPHA;
    if ((format_bits & 0x20) != 0) format = PNG_FORMAT_BGR;
    if ((format_bits & 0x40) != 0) format = PNG_FORMAT_BGRA;
    
    // Choose API to test: 0 = write to memory, 1 = write to stdio, 2 = write to file, 3 = all
    uint8_t api_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Choose convert_to_8bit flag
    bool convert_to_8bit = fdp.ConsumeBool();
    
    // Choose whether to use colormap
    bool use_colormap = fdp.ConsumeBool();
    
    // Initialize png_image structure
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.width = width;
    image.height = height;
    image.format = format;
    image.flags = 0;
    
    // Calculate channels based on format
    int channels = 1; // Gray
    if (format == PNG_FORMAT_GA) channels = 2;
    else if (format == PNG_FORMAT_RGB || format == PNG_FORMAT_BGR || 
             format == PNG_FORMAT_LINEAR_RGB) channels = 3;
    else if (format == PNG_FORMAT_RGBA || format == PNG_FORMAT_BGRA || 
             format == PNG_FORMAT_LINEAR_RGB_ALPHA) channels = 4;
    
    // Calculate bytes per component based on format
    size_t bytes_per_component = 1; // Default for 8-bit formats
    if (format & PNG_FORMAT_FLAG_LINEAR) {
        bytes_per_component = 2; // 16-bit linear formats
    }
    
    // Calculate bytes per pixel (for non-colormapped formats)
    size_t bytes_per_pixel = channels * bytes_per_component;
    
    // Calculate row stride (can be 0 for default)
    png_int_32 row_stride = 0;
    if (fdp.ConsumeBool()) {
        // Use positive or negative stride
        row_stride = fdp.ConsumeIntegralInRange<int32_t>(-1024, 1024);
        if (row_stride == 0) row_stride = 1;
    }
    
    // Calculate buffer size needed for image data using correct component-based calculation
    // row_stride is in components, not bytes (critical fix for 16-bit linear formats)
    size_t row_stride_components = (row_stride != 0 ? abs(row_stride) : (width * channels));
    size_t buffer_size = bytes_per_component * height * row_stride_components;
    
    // Consume image buffer data from fuzzer input
    std::vector<uint8_t> buffer_data = fdp.ConsumeBytes<uint8_t>(std::min(buffer_size, fdp.remaining_bytes()));
    
    // If we don't have enough data, pad with zeros
    if (buffer_data.size() < buffer_size) {
        buffer_data.resize(buffer_size, 0);
    }
    
    // Prepare colormap if needed
    std::vector<uint8_t> colormap_data;
    const void* colormap = nullptr;
    
    if (use_colormap && (format & PNG_FORMAT_FLAG_COLORMAP)) {
        // For colormapped formats, we need a colormap
        image.colormap_entries = fdp.ConsumeIntegralInRange<png_uint_32>(1, 256);
        // PNG_IMAGE_COLORMAP_SIZE = PNG_IMAGE_SAMPLE_SIZE * colormap_entries
        // PNG_IMAGE_SAMPLE_SIZE = channels * bytes_per_component
        size_t colormap_size = image.colormap_entries * channels * bytes_per_component;
        colormap_data = fdp.ConsumeBytes<uint8_t>(std::min(colormap_size, fdp.remaining_bytes()));
        
        if (colormap_data.size() < colormap_size) {
            colormap_data.resize(colormap_size, 0);
        }
        colormap = colormap_data.data();
    } else {
        image.colormap_entries = 0;
    }

    
    // Test png_image_write_to_memory
    if (api_choice == 0 || api_choice == 3) {
        png_alloc_size_t memory_bytes = 0;
        
        // First call to get required memory size
        int result = png_image_write_to_memory(&image, nullptr, &memory_bytes, 
                                              convert_to_8bit ? 1 : 0,
                                              buffer_data.data(), row_stride, colormap);
        
        // If successful in calculating size, try actual write
        if (result != 0 && memory_bytes > 0 && memory_bytes < 1024 * 1024) { // Limit to 1MB
            std::vector<uint8_t> memory_buffer(memory_bytes, 0);
            png_alloc_size_t actual_bytes = memory_bytes;
            
            result = png_image_write_to_memory(&image, memory_buffer.data(), &actual_bytes,
                                              convert_to_8bit ? 1 : 0,
                                              buffer_data.data(), row_stride, colormap);
            
            // Check if write was successful or buffer too small
            if (result == 0 && actual_bytes > memory_bytes) {
                // Buffer was too small, try with larger buffer
                memory_buffer.resize(actual_bytes, 0);
                png_alloc_size_t new_bytes = actual_bytes;
                result = png_image_write_to_memory(&image, memory_buffer.data(), &new_bytes,
                                                  convert_to_8bit ? 1 : 0,
                                                  buffer_data.data(), row_stride, colormap);
            }
        }
    }
    
    // Test png_image_write_to_stdio
    if (api_choice == 1 || api_choice == 3) {
        // Create temporary file for stdio writing
        FILE* tmp_file = tmpfile();
        if (tmp_file != NULL) {
            int result = png_image_write_to_stdio(&image, tmp_file, 
                                                 convert_to_8bit ? 1 : 0,
                                                 buffer_data.data(), row_stride, colormap);
            fclose(tmp_file);
        }
    }
    
    // Test png_image_write_to_file
    if (api_choice == 2 || api_choice == 3) {
        // Create temporary filename
        char tmp_filename[] = "/tmp/png_fuzz_XXXXXX";
        int fd = mkstemp(tmp_filename);
        if (fd >= 0) {
            close(fd); // Close the file descriptor, we'll use the filename
            
            int result = png_image_write_to_file(&image, tmp_filename,
                                                convert_to_8bit ? 1 : 0,
                                                buffer_data.data(), row_stride, colormap);
            
            // Clean up temporary file
            unlink(tmp_filename);
        }
    }
    
    // Always free image resources
    png_image_free(&image);
    
    return 0;
}

} // namespace harness_008
