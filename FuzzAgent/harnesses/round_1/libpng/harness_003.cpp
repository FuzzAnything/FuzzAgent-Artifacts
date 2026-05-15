#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

namespace harness_003 {

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
static const char* temp_filename = "/tmp/fuzz_temp.png";

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful PNG operations
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector to test different code paths
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Test 1: Create read and write structs as per guidance
    {
        png_structp png_read_ptr = png_create_read_struct(
            PNG_LIBPNG_VER_STRING,
            NULL,
            png_error_callback,
            png_warning_callback);
        
        if (png_read_ptr == NULL) {
            return 0;
        }
        
        png_structp png_write_ptr = png_create_write_struct(
            PNG_LIBPNG_VER_STRING,
            NULL,
            png_error_callback,
            png_warning_callback);
        
        if (png_write_ptr == NULL) {
            png_destroy_read_struct(&png_read_ptr, NULL, NULL);
            return 0;
        }
        
        // Test png_image APIs as specified in guidance
        png_image image_read, image_write;
        
        // Initialize read image structure
        memset(&image_read, 0, sizeof(image_read));
        image_read.version = PNG_IMAGE_VERSION;
        
        // Initialize write image structure  
        memset(&image_write, 0, sizeof(image_write));
        image_write.version = PNG_IMAGE_VERSION;
        
        // Consume image parameters from fuzzer input
        image_write.width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1024);
        image_write.height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1024);
        
        // Consume format from limited set of valid formats
        uint8_t format_choice = fdp.ConsumeIntegral<uint8_t>() % 8;
        switch (format_choice) {
            case 0: image_write.format = PNG_FORMAT_RGBA; break;
            case 1: image_write.format = PNG_FORMAT_RGB; break;
            case 2: image_write.format = PNG_FORMAT_GRAY; break;
            case 3: image_write.format = PNG_FORMAT_GA; break;
            case 4: image_write.format = PNG_FORMAT_AG; break;
            case 5: image_write.format = PNG_FORMAT_ABGR; break;
            case 6: image_write.format = PNG_FORMAT_ARGB; break;
            case 7: image_write.format = PNG_FORMAT_BGR; break;
        }
        
        image_write.flags = 0;
        image_write.colormap_entries = 0;
        
        // Test png_image_begin_read_from_memory (Target API from guidance)
        std::vector<uint8_t> png_data = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(50, size/2));
        
        if (png_data.size() >= 50) {
            int begin_result = png_image_begin_read_from_memory(&image_read, 
                                                               png_data.data(), 
                                                               png_data.size());
            (void)begin_result; // Result checked by png_image.warning_or_error
            
            // If begin_read succeeded, test png_image_finish_read (Target API)
            if (image_read.warning_or_error == 0) {
                // Allocate buffer for image data
                size_t buffer_size = PNG_IMAGE_SIZE(image_read);
                if (buffer_size > 0 && buffer_size < 1024 * 1024) { // Limit memory
                    std::vector<uint8_t> read_buffer(buffer_size);
                    
                    // Consume background color from fuzzer input
                    png_color background;
                    background.red = fdp.ConsumeIntegral<uint8_t>();
                    background.green = fdp.ConsumeIntegral<uint8_t>();
                    background.blue = fdp.ConsumeIntegral<uint8_t>();
                    
                    // Test different row strides
                    png_int_32 row_stride = fdp.ConsumeIntegral<int32_t>();
                    
                    int finish_result = png_image_finish_read(&image_read,
                                                             &background,
                                                             read_buffer.data(),
                                                             row_stride,
                                                             NULL);
                    (void)finish_result;
                    
                    // If we have image data, test write APIs
                    if (image_read.warning_or_error == 0 && !read_buffer.empty()) {
                        // Update write image with read dimensions
                        image_write.width = image_read.width;
                        image_write.height = image_read.height;
                        image_write.format = image_read.format;
                        
                        // Test png_image_write_to_memory (Target API)
                        {
                            size_t memory_size = 0;
                            // First get required size
                            int size_result = png_image_write_get_memory_size(image_write,
                                                                              memory_size,
                                                                              0, // convert_to_8_bit
                                                                              read_buffer.data(),
                                                                              0, // row_stride
                                                                              NULL);
                            
                            if (size_result && memory_size > 0 && memory_size < 10 * 1024 * 1024) {
                                std::vector<uint8_t> write_buffer(memory_size);
                                size_t actual_size = memory_size;
                                
                                int write_result = png_image_write_to_memory(&image_write,
                                                                             write_buffer.data(),
                                                                             &actual_size,
                                                                             0, // convert_to_8_bit
                                                                             read_buffer.data(),
                                                                             0, // row_stride
                                                                             NULL);
                                (void)write_result;
                            }
                        }
                        
                        // Test png_image_write_to_file if we have a temp file (Target API)
                        if (operation == 0 || operation == 2) {
                            // Create a temporary file for testing
                            FILE* temp_file = fopen(temp_filename, "wb");
                            if (temp_file) {
                                int file_result = png_image_write_to_file(&image_write,
                                                                         temp_filename,
                                                                         0, // convert_to_8_bit
                                                                         read_buffer.data(),
                                                                         0, // row_stride
                                                                         NULL);
                                (void)file_result;
                                fclose(temp_file);
                                // Clean up temp file
                                remove(temp_filename);
                            }
                        }
                        
                        // Test png_image_write_to_stdio if we can open a file (Target API)
                        if (operation == 1 || operation == 3) {
                            FILE* stdio_file = fopen("/dev/null", "wb");
                            if (stdio_file) {
                                int stdio_result = png_image_write_to_stdio(&image_write,
                                                                            stdio_file,
                                                                            0, // convert_to_8_bit
                                                                            read_buffer.data(),
                                                                            0, // row_stride
                                                                            NULL);
                                (void)stdio_result;
                                fclose(stdio_file);
                            }
                        }
                    }
                }
            }
        }
        
        // Test png_image_free on both structures (Required Helper)
        png_image_free(&image_read);
        png_image_free(&image_write);
        
        // Clean up read/write structs as per guidance
        png_destroy_read_struct(&png_read_ptr, NULL, NULL);
        png_destroy_write_struct(&png_write_ptr, NULL);
    }
    
    // Test 2: Additional png_image API tests with synthetic image data
    {
        png_image synthetic_image;
        memset(&synthetic_image, 0, sizeof(synthetic_image));
        synthetic_image.version = PNG_IMAGE_VERSION;
        synthetic_image.width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 256);
        synthetic_image.height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 256);
        synthetic_image.format = PNG_FORMAT_RGBA;
        
        // Create synthetic image buffer
        size_t synth_buffer_size = synthetic_image.width * synthetic_image.height * 4;
        if (synth_buffer_size > 0 && synth_buffer_size < 1024 * 1024) {
            std::vector<uint8_t> synth_buffer(synth_buffer_size);
            
            // Fill with fuzzer data
            size_t fill_size = std::min(synth_buffer_size, fdp.remaining_bytes());
            if (fill_size > 0) {
                std::vector<uint8_t> fill_data = fdp.ConsumeBytes<uint8_t>(fill_size);
                if (fill_data.size() >= synth_buffer_size) {
                    memcpy(synth_buffer.data(), fill_data.data(), synth_buffer_size);
                } else {
                    // Pad with pattern
                    for (size_t i = 0; i < synth_buffer_size; i++) {
                        synth_buffer[i] = (i % 256);
                    }
                }
                
                // Test memory write with synthetic data
                size_t mem_size = 0;
                if (png_image_write_get_memory_size(synthetic_image, mem_size, 
                                                    0, synth_buffer.data(), 0, NULL)) {
                    if (mem_size > 0 && mem_size < 10 * 1024 * 1024) {
                        std::vector<uint8_t> output_buffer(mem_size);
                        size_t actual_size = mem_size;
                        png_image_write_to_memory(&synthetic_image, output_buffer.data(),
                                                 &actual_size, 0, synth_buffer.data(), 0, NULL);
                    }
                }
            }
        }
        
        png_image_free(&synthetic_image);
    }
    
    // Test 3: Edge cases with minimal or malformed PNG data
    if (fdp.remaining_bytes() > 0) {
        std::vector<uint8_t> edge_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        png_image edge_image;
        memset(&edge_image, 0, sizeof(edge_image));
        edge_image.version = PNG_IMAGE_VERSION;
        
        // Try to read with various data sizes including edge cases
        size_t test_sizes[] = {8, 16, 32, 64, 128, edge_data.size()};
        for (size_t test_size : test_sizes) {

            if (test_size <= edge_data.size()) {
                png_image_begin_read_from_memory(&edge_image, 
                                                edge_data.data(), 
                                                test_size);
                png_image_free(&edge_image);
                memset(&edge_image, 0, sizeof(edge_image));
                edge_image.version = PNG_IMAGE_VERSION;
            }
        }
        
        png_image_free(&edge_image);
    }
    
    return 0;
}

} // namespace harness_003
