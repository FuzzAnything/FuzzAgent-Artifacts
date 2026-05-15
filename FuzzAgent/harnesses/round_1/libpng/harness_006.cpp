#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

namespace harness_006 {

// Callback functions for libpng error handling
static void png_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void png_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Temporary file for file-based APIs
static const char* temp_filename = "/tmp/fuzz_temp_png.png";

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful PNG file/stdio operations
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector: 0 for file, 1 for stdio
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Create PNG read structures as per guidance
    png_structp png_read_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING,
        NULL,
        png_error_callback,
        png_warning_callback);
    
    if (png_read_ptr == NULL) {
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_read_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_read_ptr, NULL, NULL);
        return 0;
    }
    
    // Initialize png_image structure
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.opaque = NULL;
    
    // Consume PNG data from fuzzer input for file/stdio
    std::vector<uint8_t> png_data = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(50, size/2));
    
    if (png_data.size() < 50) {
        png_destroy_read_struct(&png_read_ptr, &info_ptr, NULL);
        return 0;
    }
    
    int begin_result = 0;
    
    if (operation == 0) {
        // Test png_image_begin_read_from_file (Target API - 59 undiscovered branches)
        // Write PNG data to temporary file
        FILE* temp_file = fopen(temp_filename, "wb");
        if (temp_file == NULL) {
            png_destroy_read_struct(&png_read_ptr, &info_ptr, NULL);
            return 0;
        }
        
        fwrite(png_data.data(), 1, png_data.size(), temp_file);
        fclose(temp_file);
        
        // Now call the target API
        begin_result = png_image_begin_read_from_file(&image, temp_filename);
        
        // Clean up temporary file
        remove(temp_filename);
    } else {
        // Test png_image_begin_read_from_stdio (Target API - 57 undiscovered branches)
        // Create a temporary file in memory using fmemopen if available
        // Fallback to writing to a temporary file
        FILE* temp_file = fopen(temp_filename, "wb");
        if (temp_file == NULL) {
            png_destroy_read_struct(&png_read_ptr, &info_ptr, NULL);
            return 0;
        }
        
        fwrite(png_data.data(), 1, png_data.size(), temp_file);
        fclose(temp_file);
        
        // Reopen for reading
        temp_file = fopen(temp_filename, "rb");
        if (temp_file == NULL) {
            png_destroy_read_struct(&png_read_ptr, &info_ptr, NULL);
            return 0;
        }
        
        // Call the target API with stdio FILE*
        begin_result = png_image_begin_read_from_stdio(&image, temp_file);
        
        fclose(temp_file);
        remove(temp_filename);
    }
    
    // Check if begin_read succeeded (warning_or_error == 0 means success)
    if (image.warning_or_error == 0) {
        // Consume format for finish_read
        uint8_t format_choice = fdp.ConsumeIntegral<uint8_t>() % 8;
        switch (format_choice) {
            case 0: image.format = PNG_FORMAT_RGBA; break;
            case 1: image.format = PNG_FORMAT_RGB; break;
            case 2: image.format = PNG_FORMAT_GRAY; break;
            case 3: image.format = PNG_FORMAT_GA; break;
            case 4: image.format = PNG_FORMAT_AG; break;
            case 5: image.format = PNG_FORMAT_ABGR; break;
            case 6: image.format = PNG_FORMAT_ARGB; break;
            case 7: image.format = PNG_FORMAT_BGR; break;
        }
        
        // Calculate buffer size needed for image
        size_t row_stride = PNG_IMAGE_ROW_STRIDE(image);
        size_t buffer_size = PNG_IMAGE_BUFFER_SIZE(image, row_stride);
        
        if (buffer_size > 0 && buffer_size < 1024 * 1024 * 10) { // Limit to 10MB
            std::vector<uint8_t> buffer(buffer_size);
            
            // Consume background color if needed
            png_color background;
            bool use_background = fdp.ConsumeBool();
            if (use_background) {
                background.red = fdp.ConsumeIntegral<uint8_t>();
                background.green = fdp.ConsumeIntegral<uint8_t>();
                background.blue = fdp.ConsumeIntegral<uint8_t>();
            }
            
            // Allocate colormap if needed (for color-mapped formats)
            void* colormap = NULL;
            if (image.format & PNG_FORMAT_FLAG_COLORMAP) {
                size_t colormap_size = PNG_IMAGE_COLORMAP_SIZE(image);
                if (colormap_size > 0 && colormap_size < 1024 * 1024) {
                    colormap = malloc(colormap_size);
                }
            }
            
            // Test png_image_finish_read (Required Helper - 54 undiscovered branches)
            int finish_result = png_image_finish_read(&image,
                                                     use_background ? &background : NULL,
                                                     buffer.data(),
                                                     row_stride,
                                                     colormap);
            (void)finish_result;
            
            if (colormap != NULL) {
                free(colormap);
            }
        }
    }
    
    // Test png_image_free (Required Helper - cleanup)
    png_image_free(&image);
    
    // Clean up PNG structures as per guidance
    png_destroy_read_struct(&png_read_ptr, &info_ptr, NULL);
    
    return 0;
}

} // namespace harness_006
