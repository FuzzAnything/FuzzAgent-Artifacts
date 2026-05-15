#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "png.h"

namespace harness_003 {


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

// High-level PNG reading harness targeting:
// 1. png_read_png - single call PNG reading with transforms (223 undiscovered branches)
// 2. Simplified PNG image API: png_image_begin_read_from_memory, png_image_finish_read
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        // Need minimum data for PNG signature and configuration
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration from fuzzer input
    uint8_t api_choice = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0: png_read_png, 1: simplified API, 2: mixed
    
    // Consume PNG transform flags for png_read_png
    int transforms = 0;
    if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_IDENTITY;
    if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_STRIP_16;
    if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_STRIP_ALPHA;
    if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_PACKING;
    if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_PACKSWAP;
    if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_EXPAND;
    if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_INVERT_MONO;
    if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_SHIFT;
    if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_BGR;
    if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_SWAP_ALPHA;
    if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_SWAP_ENDIAN;
    if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_INVERT_ALPHA;
    if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_STRIP_FILLER;
    
    // Consume format configuration for simplified API
    uint8_t format_bits = fdp.ConsumeIntegral<uint8_t>();
    uint32_t format = PNG_FORMAT_RGBA;
    if ((format_bits & 0x01) != 0) format = PNG_FORMAT_RGB;
    if ((format_bits & 0x02) != 0) format = PNG_FORMAT_GRAY;
    if ((format_bits & 0x04) != 0) format = PNG_FORMAT_GA;
    if ((format_bits & 0x08) != 0) format = PNG_FORMAT_LINEAR_RGB;
    if ((format_bits & 0x10) != 0) format = PNG_FORMAT_LINEAR_RGB_ALPHA;
    if ((format_bits & 0x20) != 0) format = PNG_FORMAT_BGR;
    if ((format_bits & 0x40) != 0) format = PNG_FORMAT_BGRA;
    
    // Consume background color for simplified API
    png_color background;
    background.red = fdp.ConsumeIntegral<uint8_t>();
    background.green = fdp.ConsumeIntegral<uint8_t>();
    background.blue = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume remaining bytes for PNG data
    std::vector<uint8_t> png_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (png_data.size() < 8) {
        // PNG signature is 8 bytes
        return 0;
    }
    
    // Test png_read_png (high-level single-call PNG reading)
    if (api_choice == 0 || api_choice == 2) {
        png_structp png_ptr = NULL;
        png_infop info_ptr = NULL;
        png_infop end_info = NULL;
        
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, pngtest_error, pngtest_warning);
        if (png_ptr == NULL) {
            // Continue to simplified API test if png_read_png fails
            goto test_simplified_api;
        }
        
        // Set up error handling with setjmp
        if (setjmp(png_jmpbuf(png_ptr))) {
            // Error occurred during PNG processing
            if (png_ptr) {
                png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            }
            goto test_simplified_api;
        }
        
        info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == NULL) {
            png_destroy_read_struct(&png_ptr, NULL, NULL);
            goto test_simplified_api;
        }
        
        end_info = png_create_info_struct(png_ptr);
        if (end_info == NULL) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            goto test_simplified_api;
        }
        
        // Set up custom read function
        png_read_data read_data = {png_data.data(), png_data.size(), 0};
        png_set_read_fn(png_ptr, &read_data, png_read_from_buffer);
        png_set_sig_bytes(png_ptr, 0);
        
        // Use the high-level png_read_png API
        // This reads the entire PNG file in one call, applying transforms
        png_read_png(png_ptr, info_ptr, transforms, NULL);
        
        // Clean up
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    }
    
test_simplified_api:
    // Test simplified PNG image API (png_image_begin_read_from_memory, png_image_finish_read)
    if (api_choice == 1 || api_choice == 2) {
        // We need fresh PNG data for simplified API test
        // Use the original PNG data but ensure we have enough
        if (png_data.size() >= 8) {
            png_image image;
            
            // Initialize png_image structure as required by simplified API
            memset(&image, 0, sizeof(image));
            image.version = PNG_IMAGE_VERSION;
            image.format = format;  // Set the format from fuzzer input
            
            // Try to begin reading from memory
            if (png_image_begin_read_from_memory(&image, png_data.data(), png_data.size())) {
                // Successfully read PNG header
                
                // Calculate required buffer size
                png_alloc_size_t buffer_size = PNG_IMAGE_SIZE(image);
                
                if (buffer_size > 0 && buffer_size < 1024 * 1024) { // Limit to 1MB
                    // Allocate buffer for image data
                    void* buffer = malloc(buffer_size);
                    if (buffer != NULL) {
                        // Try to finish reading the image
                        // Use NULL for colormap as we're not using color-map formats
                        png_image_finish_read(&image, &background, buffer, 0, NULL);
                        
                        free(buffer);
                    }
                }
                
                // Free any resources allocated by libpng
                png_image_free(&image);
            }
        }
    }
    
    return 0;
}

} // namespace harness_003
