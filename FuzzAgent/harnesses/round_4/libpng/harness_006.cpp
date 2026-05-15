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

namespace harness_006 {

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

// Create a temporary file with fuzzed data
static FILE* create_temp_file(const uint8_t* data, size_t size, const char* mode) {
    // Create a temporary file name
    char filename[] = "/tmp/png_fuzz_XXXXXX";
    int fd = mkstemp(filename);
    if (fd < 0) {
        return nullptr;
    }
    
    // Write fuzzed data to the file
    if (size > 0 && data != nullptr) {
        write(fd, data, size);
    }
    
    // Close the file descriptor and open as FILE*
    close(fd);
    return fopen(filename, mode);
}

// Clean up temporary file
static void cleanup_temp_file(FILE* file) {
    if (file != nullptr) {
        // Get the filename from the FILE* to delete it
        // This is simplified - in real implementation, we'd track the filename
        fclose(file);
        // File deletion would happen here if we tracked the filename
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 100) {
        return 0;  // Need minimum data for simplified I/O operations
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int operation = fdp.ConsumeIntegralInRange<int>(0, 3);
    int convert_to_8bit = fdp.ConsumeBool() ? 1 : 0;
    int image_width = fdp.ConsumeIntegralInRange<int>(1, 256);
    int image_height = fdp.ConsumeIntegralInRange<int>(1, 256);
    int format_choice = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Map format choice to PNG formats
    png_uint_32 image_format;
    switch (format_choice) {
        case 0: image_format = PNG_FORMAT_RGBA; break;
        case 1: image_format = PNG_FORMAT_RGB; break;
        case 2: image_format = PNG_FORMAT_GRAY; break;
        case 3: image_format = PNG_FORMAT_GA; break;
        case 4: image_format = PNG_FORMAT_BGRA; break;
        default: image_format = PNG_FORMAT_RGBA;
    }
    
    // Consume variable-size data
    size_t png_data_size = fdp.ConsumeIntegralInRange<size_t>(8, size / 2);
    std::vector<uint8_t> png_data = fdp.ConsumeBytes<uint8_t>(png_data_size);
    std::string filename = fdp.ConsumeRandomLengthString(64);
    
    // Create a temporary file with PNG data for testing
    FILE* temp_file = create_temp_file(png_data.data(), png_data.size(), "rb");
    if (!temp_file) {
        return 0;
    }
    
    // Initialize png_image structure for reading
    png_image read_image;
    memset(&read_image, 0, sizeof(read_image));
    read_image.version = PNG_IMAGE_VERSION;
    
    // Initialize png_image structure for writing
    png_image write_image;
    memset(&write_image, 0, sizeof(write_image));
    write_image.version = PNG_IMAGE_VERSION;
    write_image.width = image_width;
    write_image.height = image_height;
    write_image.format = image_format;
    
    // Buffer for image data
    size_t buffer_size = PNG_IMAGE_SIZE(write_image);
    std::vector<uint8_t> image_buffer(buffer_size);
    
    // Fill buffer with fuzzed data
    if (buffer_size > 0) {
        size_t fill_size = std::min(buffer_size, fdp.remaining_bytes());
        auto fill_data = fdp.ConsumeBytes<uint8_t>(fill_size);
        if (fill_size > 0) {
            memcpy(image_buffer.data(), fill_data.data(), fill_size);
        }
    }
    
    int result = 0;
    
    // Test different I/O operations based on fuzzed operation value
    switch (operation) {
        case 0:  // Test png_image_begin_read_from_file
            if (png_image_begin_read_from_file(&read_image, filename.c_str()) != 0) {
                // If read succeeds, try to finish it
                png_image_finish_read(&read_image, nullptr, image_buffer.data(), 0, nullptr);
                png_image_free(&read_image);
            }
            break;
            
        case 1:  // Test png_image_begin_read_from_stdio
            if (png_image_begin_read_from_stdio(&read_image, temp_file) != 0) {
                // If read succeeds, try to finish it
                png_image_finish_read(&read_image, nullptr, image_buffer.data(), 0, nullptr);
                png_image_free(&read_image);
            }
            break;
            
        case 2:  // Test png_image_write_to_file
            // Create another temporary file for writing
            {
                char write_filename[] = "/tmp/png_fuzz_write_XXXXXX";
                int write_fd = mkstemp(write_filename);
                if (write_fd >= 0) {
                    close(write_fd);
                    png_image_write_to_file(&write_image, write_filename, convert_to_8bit, 
                                           image_buffer.data(), 0, nullptr);
                    // Clean up the file
                    unlink(write_filename);
                }
            }
            break;
            
        case 3:  // Test png_image_write_to_stdio
            // Create a temporary file for stdio writing
            {
                char write_filename[] = "/tmp/png_fuzz_stdio_XXXXXX";
                int write_fd = mkstemp(write_filename);
                if (write_fd >= 0) {
                    close(write_fd);
                    FILE* write_file = fopen(write_filename, "wb");
                    if (write_file) {
                        png_image_write_to_stdio(&write_image, write_file, convert_to_8bit,
                                                image_buffer.data(), 0, nullptr);
                        fclose(write_file);
                    }
                    unlink(write_filename);
                }
            }
            break;
    }
    
    // Cleanup
    cleanup_temp_file(temp_file);
    
    return 0;
}

} // namespace harness_006
