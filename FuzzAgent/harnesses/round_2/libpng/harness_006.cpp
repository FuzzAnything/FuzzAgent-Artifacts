#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

namespace harness_006 {

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

// Helper function to create a temporary file with PNG data
char* create_temp_png_file(const uint8_t* png_data, size_t png_size) {
    // Create a temporary file name
    char* temp_filename = strdup("/tmp/libpng_fuzz_XXXXXX");
    if (temp_filename == NULL) return NULL;
    
    int fd = mkstemp(temp_filename);
    if (fd < 0) {
        free(temp_filename);
        return NULL;
    }
    
    // Write PNG data to the file
    ssize_t written = write(fd, png_data, png_size);
    close(fd);
    
    if (written != (ssize_t)png_size) {
        unlink(temp_filename);
        free(temp_filename);
        return NULL;
    }
    
    return temp_filename;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for meaningful testing
    // We need at least some PNG header bytes and parameters
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input: first part for PNG file data, second part for parameters
    size_t png_data_size = fdp.ConsumeIntegralInRange<size_t>(50, size - 50);
    if (png_data_size < 8) return 0; // Need at least PNG signature
    
    std::vector<uint8_t> png_data = fdp.ConsumeBytes<uint8_t>(png_data_size);
    
    // Consume parameters for testing
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 3; // 0: file, 1: stdio, 2: memory
    uint8_t format_type = fdp.ConsumeIntegral<uint8_t>() % 5; // Different PNG formats
    bool test_read_rows = fdp.ConsumeBool();
    
    // Create temporary file with PNG data
    char* temp_filename = create_temp_png_file(png_data.data(), png_data.size());
    if (temp_filename == NULL) {
        return 0; // Failed to create temp file
    }
    
    // Initialize png_image structure
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    int result = 0;
    
    // Test different high-level reading APIs based on test_type
    switch (test_type) {
        case 0: { // Test png_image_begin_read_from_file
            result = png_image_begin_read_from_file(&image, temp_filename);
            break;
        }
        case 1: { // Test png_image_begin_read_from_stdio
            FILE* fp = fopen(temp_filename, "rb");
            if (fp != NULL) {
                result = png_image_begin_read_from_stdio(&image, fp);
                if (result != 0) {
                    // File is now owned by libpng, don't close it ourselves
                } else {
                    fclose(fp);
                }
            }
            break;
        }
        case 2: { // Test png_image_begin_read_from_memory
            // For memory-based reading, we need valid PNG data
            result = png_image_begin_read_from_memory(&image, png_data.data(), png_data.size());
            break;
        }
    }
    
    // If reading was successful, test additional operations
    if (result != 0) {
        // Set image format based on fuzzer input
        switch (format_type) {
            case 0: image.format = PNG_FORMAT_RGBA; break;
            case 1: image.format = PNG_FORMAT_RGB; break;
            case 2: image.format = PNG_FORMAT_GRAY; break;
            case 3: image.format = PNG_FORMAT_GA; break;
            case 4: image.format = PNG_FORMAT_AG; break;
            default: image.format = PNG_FORMAT_RGBA; break;
        }
        
        // Allocate buffer for image data
        size_t buffer_size = PNG_IMAGE_SIZE(image);
        if (buffer_size > 0 && buffer_size < 100 * 1024 * 1024) { // Limit to 100MB
            png_byte* buffer = (png_byte*)malloc(buffer_size);
            if (buffer != NULL) {
                // Target API: png_image_finish_read
                if (png_image_finish_read(&image, NULL, buffer, 0, NULL)) {
                    // Image successfully read into buffer
                    
                    // Target API: png_read_rows (if test_read_rows is true and we have PNG structures)
                    if (test_read_rows && image.opaque != NULL) {
                        // Get the PNG structures from the opaque field
                        // Note: This is accessing internal structures, so we need to be careful
                        // In real usage, png_read_rows is typically used with traditional API
                        // not simplified API. But we can still create traditional structures
                        // for testing png_read_rows specifically
                        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                                                     NULL,
                                                                     user_error_fn,
                                                                     user_warning_fn);
                        if (png_ptr != NULL) {
                            png_infop info_ptr = png_create_info_struct(png_ptr);
                            if (info_ptr != NULL) {
                                // Set up error handling
                                if (setjmp(png_jmpbuf(png_ptr))) {
                                    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                                } else {
                                    // Create a custom read function that reads from our buffer
                                    // For simplicity, we'll just test png_read_rows with dummy data
                                    png_byte* row_pointers[1];
                                    png_byte row_buffer[1024];
                                    row_pointers[0] = row_buffer;
                                    
                                    // Target API: png_read_rows
                                    png_read_rows(png_ptr, row_pointers, NULL, 1);
                                    
                                    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                                }
                            } else {
                                png_destroy_read_struct(&png_ptr, NULL, NULL);
                            }
                        }
                    }
                }
                free(buffer);
            }
        }
        
        // Target API: png_image_free (cleanup)
        png_image_free(&image);
    }
    
    // Clean up temporary file (if it still exists)
    if (test_type != 1 || result == 0) { // If stdio test failed or wasn't stdio test
        unlink(temp_filename);
    }
    
    free(temp_filename);
    
    return 0;
}

} // namespace harness_006
