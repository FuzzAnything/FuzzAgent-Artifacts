#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <unistd.h>
#include <fcntl.h>

namespace harness_006 {

// Function to create a temporary file with given content
static int create_temp_file_with_content(const uint8_t* data, size_t size, char* temp_path) {
    static int temp_counter = 0;
    snprintf(temp_path, 256, "/tmp/libpng_fuzz_input_%d_%d.png", getpid(), temp_counter++);
    
    FILE* tmp = fopen(temp_path, "wb");
    if (!tmp) return 0;
    
    size_t written = 0;
    if (size > 0) {
        written = fwrite(data, 1, size, tmp);
    }
    fclose(tmp);
    
    return (written == size) ? 1 : 0;
}

// Function to create an output temporary file path
static void create_output_temp_path(char* temp_path) {
    static int output_counter = 0;
    snprintf(temp_path, 256, "/tmp/libpng_fuzz_output_%d_%d.png", getpid(), output_counter++);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 100) {
        return 0;  // Need minimum data for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Consume operation type from fuzzer input
    int operation = fdp.ConsumeIntegralInRange<int>(0, 5);
    
    // Consume format choice
    int format_choice = fdp.ConsumeIntegralInRange<int>(0, 15);
    
    // Consume conversion flag
    int convert_to_8bit = fdp.ConsumeBool() ? 1 : 0;
    
    // Consume row stride type
    int row_stride_type = fdp.ConsumeIntegralInRange<int>(-1, 1);
    
    // Determine image format based on consumed choice
    png_uint_32 format = 0;
    switch (format_choice % 8) {
        case 0: format = PNG_FORMAT_GRAY; break;
        case 1: format = PNG_FORMAT_GA; break;
        case 2: format = PNG_FORMAT_RGB; break;
        case 3: format = PNG_FORMAT_RGBA; break;
        case 4: format = PNG_FORMAT_LINEAR_Y; break;
        case 5: format = PNG_FORMAT_LINEAR_Y_ALPHA; break;
        case 6: format = PNG_FORMAT_LINEAR_RGB; break;
        case 7: format = PNG_FORMAT_LINEAR_RGB_ALPHA; break;
    }
    
    // Add some flags based on remaining fuzzer data
    if (fdp.ConsumeBool()) format |= PNG_FORMAT_FLAG_ALPHA;
    if (fdp.ConsumeBool()) format |= PNG_FORMAT_FLAG_COLORMAP;
    if (fdp.ConsumeBool()) format |= PNG_FORMAT_FLAG_LINEAR;
    
    // Extract PNG data from remaining input for file creation
    size_t png_data_size = fdp.ConsumeIntegralInRange<size_t>(8, fdp.remaining_bytes());
    std::vector<uint8_t> png_data = fdp.ConsumeBytes<uint8_t>(png_data_size);
    
    // Test different operations based on consumed value
    switch (operation) {
        case 0: {
            // Test: png_image_begin_read_from_file -> png_image_finish_read -> png_image_write_to_file
#ifdef PNG_STDIO_SUPPORTED
            char input_temp_path[256];
            if (create_temp_file_with_content(png_data.data(), png_data.size(), input_temp_path)) {
                png_image image;
                memset(&image, 0, sizeof(image));
                image.version = PNG_IMAGE_VERSION;
                image.opaque = NULL;
                
                // Begin reading from file
                if (png_image_begin_read_from_file(&image, input_temp_path) != 0) {
                    // Set format for output
                    image.format = format;
                    
                    // Calculate buffer size needed
                    png_uint_32 buffer_size = PNG_IMAGE_SIZE(image);
                    if (buffer_size > 0 && buffer_size < 10 * 1024 * 1024) {  // Sanity check
                        void* buffer = malloc(buffer_size);
                        if (buffer) {
                            // Calculate row stride
                            png_int_32 row_stride = 0;
                            if (row_stride_type == 0) {
                                // Default stride
                                row_stride = PNG_IMAGE_ROW_STRIDE(image);
                            } else if (row_stride_type > 0) {
                                // Positive stride (top-down)
                                row_stride = PNG_IMAGE_ROW_STRIDE(image);
                            } else {
                                // Negative stride (bottom-up)
                                row_stride = -PNG_IMAGE_ROW_STRIDE(image);
                            }
                            
                            // Set background color
                            png_color background;
                            background.red = fdp.ConsumeIntegralInRange<uint8_t>(0, 255);
                            background.green = fdp.ConsumeIntegralInRange<uint8_t>(0, 255);
                            background.blue = fdp.ConsumeIntegralInRange<uint8_t>(0, 255);
                            
                            // Finish reading into buffer
                            if (png_image_finish_read(&image, &background, buffer, row_stride, NULL) != 0) {
                                // Write to a temporary output file
                                char output_temp_path[256];
                                create_output_temp_path(output_temp_path);
                                
                                png_image_write_to_file(&image, output_temp_path, convert_to_8bit,
                                                        buffer, row_stride, NULL);
                                
                                // Clean up output file
                                unlink(output_temp_path);
                            }
                            free(buffer);
                        }
                    }
                    png_image_free(&image);
                }
                
                // Clean up input file
                unlink(input_temp_path);
            }
#endif
            break;
        }
        
        case 1: {
            // Test: Round-trip - read from file, write to memory, read from memory, write to file
#ifdef PNG_STDIO_SUPPORTED
            char input_temp_path[256];
            if (create_temp_file_with_content(png_data.data(), png_data.size(), input_temp_path)) {
                png_image read_image;
                memset(&read_image, 0, sizeof(read_image));
                read_image.version = PNG_IMAGE_VERSION;
                read_image.opaque = NULL;
                
                if (png_image_begin_read_from_file(&read_image, input_temp_path) != 0) {
                    read_image.format = format;
                    
                    png_uint_32 buffer_size = PNG_IMAGE_SIZE(read_image);
                    if (buffer_size > 0 && buffer_size < 5 * 1024 * 1024) {
                        void* buffer = malloc(buffer_size);
                        if (buffer) {
                            png_int_32 row_stride = PNG_IMAGE_ROW_STRIDE(read_image);
                            png_color background = {128, 128, 128};  // Gray background
                            
                            if (png_image_finish_read(&read_image, &background, buffer, row_stride, NULL) != 0) {
                                // Write to memory first
                                png_image write_image;
                                memset(&write_image, 0, sizeof(write_image));
                                write_image.version = PNG_IMAGE_VERSION;
                                write_image.opaque = NULL;
                                write_image.width = read_image.width;
                                write_image.height = read_image.height;
                                write_image.format = format;
                                
                                png_alloc_size_t memory_size = 0;
                                if (png_image_write_to_memory(&write_image, NULL, &memory_size,
                                                              convert_to_8bit, buffer, row_stride, NULL) != 0) {
                                    if (memory_size > 0 && memory_size < 10 * 1024 * 1024) {
                                        void* memory_buffer = malloc(memory_size);
                                        if (memory_buffer) {
                                            png_image_write_to_memory(&write_image, memory_buffer, &memory_size,
                                                                      convert_to_8bit, buffer, row_stride, NULL);
                                            
                                            // Now read from memory and write to file
                                            png_image mem_image;
                                            memset(&mem_image, 0, sizeof(mem_image));
                                            mem_image.version = PNG_IMAGE_VERSION;
                                            mem_image.opaque = NULL;
                                            
                                            if (png_image_begin_read_from_memory(&mem_image, memory_buffer, memory_size) != 0) {
                                                mem_image.format = format;
                                                
                                                png_uint_32 mem_buffer_size = PNG_IMAGE_SIZE(mem_image);
                                                if (mem_buffer_size > 0 && mem_buffer_size < 5 * 1024 * 1024) {
                                                    void* mem_buffer = malloc(mem_buffer_size);
                                                    if (mem_buffer) {
                                                        png_int_32 mem_row_stride = PNG_IMAGE_ROW_STRIDE(mem_image);
                                                        png_color mem_background = {200, 200, 200};
                                                        
                                                        if (png_image_finish_read(&mem_image, &mem_background, mem_buffer, mem_row_stride, NULL) != 0) {
                                                            char output_temp_path[256];
                                                            create_output_temp_path(output_temp_path);
                                                            
                                                            png_image_write_to_file(&mem_image, output_temp_path, convert_to_8bit,
                                                                                    mem_buffer, mem_row_stride, NULL);
                                                            
                                                            unlink(output_temp_path);
                                                        }
                                                        free(mem_buffer);
                                                    }
                                                }
                                                png_image_free(&mem_image);
                                            }
                                            free(memory_buffer);
                                        }
                                    }
                                }
                                png_image_free(&write_image);
                            }
                            free(buffer);
                        }
                    }
                    png_image_free(&read_image);
                }
                unlink(input_temp_path);
            }
#endif
            break;
        }
        
        case 2: {
            // Test: File read with stdio wrapper -> write to stdio
#ifdef PNG_STDIO_SUPPORTED
            char input_temp_path[256];
            if (create_temp_file_with_content(png_data.data(), png_data.size(), input_temp_path)) {
                FILE* input_file = fopen(input_temp_path, "rb");
                if (input_file) {
                    png_image image;
                    memset(&image, 0, sizeof(image));
                    image.version = PNG_IMAGE_VERSION;
                    image.opaque = NULL;
                    
                    if (png_image_begin_read_from_stdio(&image, input_file) != 0) {
                        image.format = format;
                        
                        png_uint_32 buffer_size = PNG_IMAGE_SIZE(image);
                        if (buffer_size > 0 && buffer_size < 10 * 1024 * 1024) {
                            void* buffer = malloc(buffer_size);
                            if (buffer) {
                                png_int_32 row_stride = PNG_IMAGE_ROW_STRIDE(image);
                                png_color background = {0, 0, 0};  // Black background
                                
                                if (png_image_finish_read(&image, &background, buffer, row_stride, NULL) != 0) {
                                    // Write to stdio
                                    char output_temp_path[256];
                                    create_output_temp_path(output_temp_path);
                                    
                                    FILE* output_file = fopen(output_temp_path, "wb");
                                    if (output_file) {
                                        png_image_write_to_stdio(&image, output_file, convert_to_8bit,
                                                                buffer, row_stride, NULL);
                                        fclose(output_file);
                                        unlink(output_temp_path);
                                    }
                                }
                                free(buffer);
                            }
                        }
                        png_image_free(&image);
                    }
                    fclose(input_file);
                }
                unlink(input_temp_path);
            }
#endif
            break;
        }
        
        case 3: {
            // Test: Multiple write methods from same read
#ifdef PNG_STDIO_SUPPORTED
            char input_temp_path[256];
            if (create_temp_file_with_content(png_data.data(), png_data.size(), input_temp_path)) {
                png_image image;
                memset(&image, 0, sizeof(image));
                image.version = PNG_IMAGE_VERSION;
                image.opaque = NULL;
                
                if (png_image_begin_read_from_file(&image, input_temp_path) != 0) {
                    image.format = format;
                    
                    png_uint_32 buffer_size = PNG_IMAGE_SIZE(image);
                    if (buffer_size > 0 && buffer_size < 10 * 1024 * 1024) {
                        void* buffer = malloc(buffer_size);
                        if (buffer) {
                            png_int_32 row_stride = PNG_IMAGE_ROW_STRIDE(image);
                            png_color background = {255, 255, 255};  // White background
                            
                            if (png_image_finish_read(&image, &background, buffer, row_stride, NULL) != 0) {
                                // Test 1: Write to file
                                char file_output_path[256];
                                create_output_temp_path(file_output_path);
                                png_image_write_to_file(&image, file_output_path, convert_to_8bit,
                                                       buffer, row_stride, NULL);
                                unlink(file_output_path);
                                
                                // Test 2: Write to memory
                                png_alloc_size_t mem_size = 0;
                                if (png_image_write_to_memory(&image, NULL, &mem_size,
                                                              convert_to_8bit, buffer, row_stride, NULL) != 0) {
                                    if (mem_size > 0 && mem_size < 10 * 1024 * 1024) {
                                        void* mem_buffer = malloc(mem_size);
                                        if (mem_buffer) {
                                            png_image_write_to_memory(&image, mem_buffer, &mem_size,
                                                                      convert_to_8bit, buffer, row_stride, NULL);
                                            free(mem_buffer);
                                        }
                                    }
                                }
                                
                                // Test 3: Write to stdio
                                char stdio_output_path[256];
                                create_output_temp_path(stdio_output_path);
                                FILE* stdio_file = fopen(stdio_output_path, "wb");
                                if (stdio_file) {
                                    png_image_write_to_stdio(&image, stdio_file, convert_to_8bit,
                                                            buffer, row_stride, NULL);
                                    fclose(stdio_file);
                                    unlink(stdio_output_path);
                                }
                            }
                            free(buffer);
                        }
                    }
                    png_image_free(&image);
                }
                unlink(input_temp_path);
            }
#endif
            break;
        }
        
        case 4: {
            // Test: Error handling - invalid file paths
#ifdef PNG_STDIO_SUPPORTED
            png_image image;
            memset(&image, 0, sizeof(image));
            image.version = PNG_IMAGE_VERSION;
            image.opaque = NULL;
            
            // Try reading from non-existent file
            png_image_begin_read_from_file(&image, "/tmp/non_existent_file_12345.png");
            png_image_free(&image);
            
            // Try writing to invalid file path
            memset(&image, 0, sizeof(image));
            image.version = PNG_IMAGE_VERSION;
            image.opaque = NULL;
            image.width = 100;
            image.height = 100;
            image.format = PNG_FORMAT_RGB;
            
            // Small buffer for testing
            uint8_t small_buffer[100 * 100 * 3];
            png_image_write_to_file(&image, "/invalid/path/to/file.png", 0,
                                   small_buffer, 100 * 3, NULL);
            png_image_free(&image);
#endif
            break;
        }
        
        case 5: {
            // Test: Mixed operations - read from memory, write to file, read back
#ifdef PNG_STDIO_SUPPORTED
            if (png_data_size >= 8) {
                png_image mem_image;
                memset(&mem_image, 0, sizeof(mem_image));
                mem_image.version = PNG_IMAGE_VERSION;
                mem_image.opaque = NULL;
                
                if (png_image_begin_read_from_memory(&mem_image, png_data.data(), png_data.size()) != 0) {
                    mem_image.format = format;
                    
                    png_uint_32 buffer_size = PNG_IMAGE_SIZE(mem_image);
                    if (buffer_size > 0 && buffer_size < 5 * 1024 * 1024) {
                        void* buffer = malloc(buffer_size);
                        if (buffer) {
                            png_int_32 row_stride = PNG_IMAGE_ROW_STRIDE(mem_image);
                            png_color background = {64, 64, 64};
                            
                            if (png_image_finish_read(&mem_image, &background, buffer, row_stride, NULL) != 0) {
                                // Write to file
                                char output_temp_path[256];
                                create_output_temp_path(output_temp_path);
                                
                                if (png_image_write_to_file(&mem_image, output_temp_path, convert_to_8bit,
                                                           buffer, row_stride, NULL) != 0) {
                                    // Read back from file
                                    png_image file_image;
                                    memset(&file_image, 0, sizeof(file_image));
                                    file_image.version = PNG_IMAGE_VERSION;
                                    file_image.opaque = NULL;
                                    
                                    if (png_image_begin_read_from_file(&file_image, output_temp_path) != 0) {
                                        file_image.format = format;
                                        
                                        png_uint_32 file_buffer_size = PNG_IMAGE_SIZE(file_image);
                                        if (file_buffer_size > 0 && file_buffer_size < 5 * 1024 * 1024) {
                                            void* file_buffer = malloc(file_buffer_size);
                                            if (file_buffer) {
                                                png_int_32 file_row_stride = PNG_IMAGE_ROW_STRIDE(file_image);
                                                png_color file_background = {192, 192, 192};
                                                
                                                png_image_finish_read(&file_image, &file_background, file_buffer, file_row_stride, NULL);
                                                free(file_buffer);
                                            }
                                        }
                                        png_image_free(&file_image);
                                    }
                                }
                                unlink(output_temp_path);
                            }
                            free(buffer);
                        }
                    }
                    png_image_free(&mem_image);
                }
            }
#endif
            break;
        }
    }

    return 0;
}

} // namespace harness_006
