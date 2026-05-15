#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_002 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need at least enough data for basic PNG operations
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize png_image structure
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    // First part of input: try to read as PNG from memory
    // Consume a portion for PNG data
    size_t png_data_size = fdp.ConsumeIntegralInRange<size_t>(8, size - 56);
    std::vector<uint8_t> png_data = fdp.ConsumeBytes<uint8_t>(png_data_size);
    
    if (png_data.size() < 8) {
        return 0;
    }
    
    // Try to begin reading PNG from memory
    int read_result = png_image_begin_read_from_memory(&image, 
                                                      png_data.data(), 
                                                      png_data.size());
    
    if (read_result == 0) {
        // Reading failed, free the image structure and return
        png_image_free(&image);
        return 0;
    }
    
    // Now we have a valid PNG header, select format based on fuzzed input
    // Available format flags from png.h
    png_uint_32 format = 0;
    
    // Consume format flags from input
    if (fdp.ConsumeBool()) format |= PNG_FORMAT_FLAG_COLOR;
    if (fdp.ConsumeBool()) format |= PNG_FORMAT_FLAG_ALPHA;
    if (fdp.ConsumeBool()) format |= PNG_FORMAT_FLAG_LINEAR;
    if (fdp.ConsumeBool()) format |= PNG_FORMAT_FLAG_BGR;
    if (fdp.ConsumeBool()) format |= PNG_FORMAT_FLAG_COLORMAP;
    
    // If no flags set, use a default format
    if (format == 0) {
        format = PNG_FORMAT_RGBA;  // Default to RGBA
    }
    
    image.format = format;
    
    // Consume flags for image processing
    if (fdp.ConsumeBool()) image.flags |= PNG_IMAGE_FLAG_COLORSPACE_NOT_sRGB;
    if (fdp.ConsumeBool()) image.flags |= PNG_IMAGE_FLAG_16BIT_sRGB;
    if (fdp.ConsumeBool()) image.flags |= PNG_IMAGE_FLAG_FAST;
    
    // Calculate buffer size needed for the image
    size_t buffer_size = PNG_IMAGE_SIZE(image);
    
    // Limit buffer size to prevent excessive memory usage
    if (buffer_size == 0 || buffer_size > 10 * 1024 * 1024) {  // 10MB limit
        png_image_free(&image);
        return 0;
    }
    
    // Allocate buffer for image data
    std::vector<uint8_t> buffer(buffer_size);
    
    // Set up background color if needed (for alpha removal)
    png_color background;
    png_color* background_ptr = nullptr;
    
    // Only need background if we're removing alpha from non-linear format
    if ((format & PNG_FORMAT_FLAG_ALPHA) == 0 && 
        (format & PNG_FORMAT_FLAG_LINEAR) == 0 &&
        (format & PNG_FORMAT_FLAG_COLORMAP) == 0) {
        background.red = fdp.ConsumeIntegral<uint8_t>();
        background.green = fdp.ConsumeIntegral<uint8_t>();
        background.blue = fdp.ConsumeIntegral<uint8_t>();
        background_ptr = &background;
    }
    
    // Consume row stride
    png_int_32 row_stride = 0;
    if (fdp.ConsumeBool()) {
        // Use a random stride, but ensure it's at least minimum
        row_stride = fdp.ConsumeIntegralInRange<png_int_32>(
            PNG_IMAGE_ROW_STRIDE(image), 
            PNG_IMAGE_ROW_STRIDE(image) * 2);
    }
    
    // Allocate colormap if needed
    std::vector<uint8_t> colormap;
    uint8_t* colormap_ptr = nullptr;
    
    if (format & PNG_FORMAT_FLAG_COLORMAP) {
        size_t colormap_size = PNG_IMAGE_COLORMAP_SIZE(image);
        if (colormap_size > 0 && colormap_size <= 256 * 4) {  // Reasonable limit
            colormap.resize(colormap_size);
            colormap_ptr = colormap.data();
            image.colormap_entries = colormap_size / 4;  // 4 bytes per entry
        }
    }
    
    // Finish reading the image
    int finish_result = png_image_finish_read(&image, 
                                             background_ptr,
                                             buffer.data(),
                                             row_stride,
                                             colormap_ptr);
    
    if (finish_result == 0) {
        // Finish read failed
        png_image_free(&image);
        return 0;
    }
    
    // Now test writing the image back to memory
    // Consume remaining input for write parameters
    bool convert_to_8bit = fdp.ConsumeBool();
    
    // Prepare for writing
    png_image write_image;
    memset(&write_image, 0, sizeof(write_image));
    write_image.version = PNG_IMAGE_VERSION;
    write_image.width = image.width;
    write_image.height = image.height;
    write_image.format = image.format;
    write_image.flags = image.flags;
    
    // Calculate maximum output size
    size_t max_output_size = PNG_IMAGE_SIZE(write_image) * 2 + 1024; // Extra for PNG overhead
    
    // Allocate output buffer
    std::vector<uint8_t> output_buffer(max_output_size);
    size_t output_size = max_output_size;
    
    // Write image to memory
    int write_result = png_image_write_to_memory(&write_image,
                                                output_buffer.data(),
                                                &output_size,
                                                convert_to_8bit,
                                                buffer.data(),
                                                row_stride,
                                                colormap_ptr);
    
    // Also test getting memory size without writing
    if (fdp.remaining_bytes() > 0) {
        size_t size_only = 0;
        png_image_write_get_memory_size(write_image, 
                                       size_only, 
                                       convert_to_8bit,
                                       buffer.data(),
                                       row_stride,
                                       colormap_ptr);
    }
    
    // Clean up
    png_image_free(&image);
    
    return 0;
}

} // namespace harness_002
