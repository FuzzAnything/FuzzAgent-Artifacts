#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

namespace harness_007 {

// Temporary file for file-based APIs
static const char* temp_filename = "/tmp/fuzz_temp_007.png";

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful PNG image operations
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize png_image structure for reading
    png_image image_read;
    memset(&image_read, 0, sizeof(image_read));
    image_read.version = PNG_IMAGE_VERSION;
    image_read.opaque = NULL;
    
    // Initialize png_image structure for writing
    png_image image_write;
    memset(&image_write, 0, sizeof(image_write));
    image_write.version = PNG_IMAGE_VERSION;
    image_write.opaque = NULL;
    
    // Consume format from fuzzer input - target different formats to maximize coverage
    uint8_t format_choice = fdp.ConsumeIntegral<uint8_t>() % 12;
    png_uint_32 format = PNG_FORMAT_RGBA; // default
    
    switch (format_choice) {
        case 0: format = PNG_FORMAT_RGBA; break;
        case 1: format = PNG_FORMAT_RGB; break;
        case 2: format = PNG_FORMAT_GRAY; break;
        case 3: format = PNG_FORMAT_GA; break;
        case 4: format = PNG_FORMAT_AG; break;
        case 5: format = PNG_FORMAT_BGR; break;
        case 6: format = PNG_FORMAT_ARGB; break;
        case 7: format = PNG_FORMAT_ABGR; break;
        case 8: format = PNG_FORMAT_BGRA; break;
        case 9: format = PNG_FORMAT_LINEAR_RGB; break;
        case 10: format = PNG_FORMAT_LINEAR_RGB_ALPHA; break;
        case 11: format = PNG_FORMAT_LINEAR_Y; break;
    }
    
    // Consume flags
    uint8_t flags_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    png_uint_32 flags = 0;
    switch (flags_choice) {
        case 0: flags = 0; break;
        case 1: flags = PNG_IMAGE_FLAG_COLORSPACE_NOT_sRGB; break;
        // Other flag combinations could be tested
    }
    
    // Set format for reading (will be updated after reading)
    image_read.format = format;
    image_read.flags = flags;
    
    // Set format for writing (will potentially be updated from read image)
    image_write.format = format;
    image_write.flags = flags;
    
    // Consume PNG data from fuzzer input
    size_t png_data_size = fdp.ConsumeIntegralInRange<size_t>(50, size/2);
    std::vector<uint8_t> png_data = fdp.ConsumeBytes<uint8_t>(png_data_size);
    
    if (png_data.size() < 50) {
        png_image_free(&image_read);
        png_image_free(&image_write);
        return 0;
    }
    
    // Step 1: Call png_image_begin_read_from_memory with PNG buffer
    int begin_result = png_image_begin_read_from_memory(&image_read, 
                                                       png_data.data(), 
                                                       png_data.size());
    // Check result via warning_or_error field
    if (image_read.warning_or_error > 1) { // Error occurred
        png_image_free(&image_read);
        png_image_free(&image_write);
        return 0;
    }
    
    // Step 2: Call png_image_finish_read to get pixel data
    size_t buffer_size = PNG_IMAGE_SIZE(image_read);
    if (buffer_size == 0 || buffer_size > 1024 * 1024) { // Limit memory usage
        png_image_free(&image_read);
        png_image_free(&image_write);
        return 0;
    }
    
    std::vector<uint8_t> read_buffer(buffer_size);
    
    // Consume background color from fuzzer input
    png_color background;
    background.red = fdp.ConsumeIntegral<uint8_t>();
    background.green = fdp.ConsumeIntegral<uint8_t>();
    background.blue = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume row stride (can be positive or negative)
    png_int_32 row_stride = fdp.ConsumeIntegral<int32_t>();
    
    int finish_result = png_image_finish_read(&image_read,
                                             &background,
                                             read_buffer.data(),
                                             row_stride,
                                             NULL);
    
    if (image_read.warning_or_error > 1 || read_buffer.empty()) {
        png_image_free(&image_read);
        png_image_free(&image_write);
        return 0;
    }
    
    // Step 3: Optionally modify pixel data or create new png_image for writing
    // Update write image with read dimensions and format
    image_write.width = image_read.width;
    image_write.height = image_read.height;
    image_write.format = image_read.format;
    image_write.flags = image_read.flags;
    
    // Optionally modify some pixel data based on fuzzer input
    if (!read_buffer.empty() && fdp.ConsumeBool()) {
        size_t modify_count = fdp.ConsumeIntegralInRange<size_t>(0, read_buffer.size() / 4);
        for (size_t i = 0; i < modify_count && i < read_buffer.size(); i++) {
            size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, read_buffer.size() - 1);
            read_buffer[idx] = fdp.ConsumeIntegral<uint8_t>();
        }
    }
    
    // Step 4: Call png_image_write_to_file to write to temporary file
    // First clean up any existing temp file
    remove(temp_filename);
    
    int convert_to_8bit = fdp.ConsumeBool() ? 1 : 0;
    
    // Use 0 for row_stride (libpng will calculate) or use the same as read
    png_int_32 write_row_stride = fdp.ConsumeBool() ? 0 : row_stride;
    
    int write_file_result = png_image_write_to_file(&image_write,
                                                   temp_filename,
                                                   convert_to_8bit,
                                                   read_buffer.data(),
                                                   write_row_stride,
                                                   NULL);
    // Check if file was created
    FILE* check_file = fopen(temp_filename, "rb");
    if (check_file) {
        fclose(check_file);
        // Clean up temp file
        remove(temp_filename);
    }
    
    // Step 5: Call png_image_write_to_memory as alternative writing path
    // First get required memory size
    size_t memory_size = 0;
    int size_result = png_image_write_get_memory_size(image_write,
                                                     memory_size,
                                                     convert_to_8bit,
                                                     read_buffer.data(),
                                                     write_row_stride,
                                                     NULL);
    
    if (size_result && memory_size > 0 && memory_size < 10 * 1024 * 1024) {
        std::vector<uint8_t> write_buffer(memory_size);
        size_t actual_size = memory_size;
        
        int write_mem_result = png_image_write_to_memory(&image_write,
                                                        write_buffer.data(),
                                                        &actual_size,
                                                        convert_to_8bit,
                                                        read_buffer.data(),
                                                        write_row_stride,
                                                        NULL);
        (void)write_mem_result;
    }
    
    // Step 6: Also test png_image_write_to_stdio if supported
#ifdef PNG_SIMPLIFIED_WRITE_STDIO_SUPPORTED
    if (fdp.ConsumeBool()) {
        FILE* stdio_file = fopen("/dev/null", "wb");
        if (stdio_file) {
            int write_stdio_result = png_image_write_to_stdio(&image_write,
                                                             stdio_file,
                                                             convert_to_8bit,
                                                             read_buffer.data(),
                                                             write_row_stride,
                                                             NULL);
            (void)write_stdio_result;
            fclose(stdio_file);
        }
    }
#endif
    
    // Step 7: Call png_image_free to clean up
    png_image_free(&image_read);
    png_image_free(&image_write);
    
    return 0;
}

} // namespace harness_007
