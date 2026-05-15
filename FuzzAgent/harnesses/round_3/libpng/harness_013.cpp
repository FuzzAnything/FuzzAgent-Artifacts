#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>

namespace harness_013 {

// Custom write callback structure for png_set_write_fn
struct png_write_memory_data {
    uint8_t* buffer;
    size_t size;
    size_t offset;
    size_t capacity;
};

// Write callback function for memory-based output
static void png_write_memory_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_write_memory_data* write_data = (png_write_memory_data*)png_get_io_ptr(png_ptr);
    
    // Resize buffer if needed
    if (write_data->offset + length > write_data->capacity) {
        size_t new_capacity = write_data->capacity * 2;
        if (new_capacity < write_data->offset + length) {
            new_capacity = write_data->offset + length;
        }
        uint8_t* new_buffer = (uint8_t*)realloc(write_data->buffer, new_capacity);
        if (!new_buffer) {
            png_error(png_ptr, "Memory allocation failed in write callback");
            return;
        }
        write_data->buffer = new_buffer;
        write_data->capacity = new_capacity;
    }
    
    memcpy(write_data->buffer + write_data->offset, data, length);
    write_data->offset += length;
    write_data->size = write_data->offset;
}

// Flush callback (required but does nothing for memory writing)
static void png_write_memory_flush(png_structp png_ptr) {
    // Nothing to flush for memory buffer
    (void)png_ptr;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for PNG testing
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume parameters from fuzzer input
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 7);
    int format_choice = fdp.ConsumeIntegralInRange<int>(0, 15);
    int convert_to_8bit = fdp.ConsumeBool() ? 1 : 0;
    int use_write_struct_2 = fdp.ConsumeBool() ? 1 : 0;
    
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
    
    // Extract PNG data from remaining input
    size_t png_data_size = fdp.ConsumeIntegralInRange<size_t>(8, fdp.remaining_bytes());
    std::vector<uint8_t> png_data = fdp.ConsumeBytes<uint8_t>(png_data_size);
    
    // STEP 1: Read PNG image from memory using simplified API
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.opaque = NULL;
    
    if (png_image_begin_read_from_memory(&image, png_data.data(), png_data.size()) == 0) {
        // Failed to read image, clean up and return
        png_image_free(&image);
        return 0;
    }
    
    // Set format for output based on fuzzer input
    image.format = format;
    
    // Calculate buffer size needed for image data
    png_uint_32 buffer_size = PNG_IMAGE_SIZE(image);
    if (buffer_size == 0 || buffer_size > 10 * 1024 * 1024) {
        // Size is zero or too large, clean up and return
        png_image_free(&image);
        return 0;
    }
    
    // Allocate buffer for image data
    void* image_buffer = malloc(buffer_size);
    if (!image_buffer) {
        png_image_free(&image);
        return 0;
    }
    
    // Calculate row stride
    png_int_32 row_stride = PNG_IMAGE_ROW_STRIDE(image);
    // Randomly choose positive (top-down) or negative (bottom-up) stride
    if (fdp.ConsumeBool()) {
        row_stride = -row_stride;
    }
    
    // STEP 2: Complete image reading
    png_color background = {200, 200, 200};  // Light gray background
    if (png_image_finish_read(&image, &background, image_buffer, row_stride, NULL) == 0) {
        // Failed to finish reading, clean up and return
        free(image_buffer);
        png_image_free(&image);
        return 0;
    }
    
    // Now we have a valid image in image_buffer, test different writing scenarios
    switch (test_scenario) {
        case 0: {
            // TEST: png_image_write_to_memory
            png_alloc_size_t memory_size = 0;
            
            // First, get the required memory size
            if (png_image_write_to_memory(&image, NULL, &memory_size,
                                          convert_to_8bit, image_buffer, row_stride, NULL) != 0) {
                if (memory_size > 0 && memory_size < 10 * 1024 * 1024) {
                    void* output_buffer = malloc(memory_size);
                    if (output_buffer) {
                        // Actually write to memory
                        png_image_write_to_memory(&image, output_buffer, &memory_size,
                                                  convert_to_8bit, image_buffer, row_stride, NULL);
                        free(output_buffer);
                    }
                }
            }
            break;
        }
        
        case 1: {
            // TEST: png_image_write_to_stdio
#ifdef PNG_SIMPLIFIED_WRITE_STDIO_SUPPORTED
            // Create a temporary file for stdio output
            FILE* tmp_file = tmpfile();
            if (tmp_file) {
                png_image_write_to_stdio(&image, tmp_file, convert_to_8bit,
                                         image_buffer, row_stride, NULL);
                fclose(tmp_file);
            }
#endif
            break;
        }
        
        case 2: {
            // TEST: png_image_write_to_file
#ifdef PNG_SIMPLIFIED_WRITE_STDIO_SUPPORTED
            const char* temp_filename = "/tmp/libpng_fuzz_write_test.png";
            png_image_write_to_file(&image, temp_filename, convert_to_8bit,
                                    image_buffer, row_stride, NULL);
#endif
            break;
        }
        
        case 3: {
            // TEST: Combined - write to all three targets
            // Write to memory
            png_alloc_size_t memory_size = 0;
            if (png_image_write_to_memory(&image, NULL, &memory_size,
                                          convert_to_8bit, image_buffer, row_stride, NULL) != 0) {
                if (memory_size > 0 && memory_size < 10 * 1024 * 1024) {
                    void* output_buffer = malloc(memory_size);
                    if (output_buffer) {
                        png_image_write_to_memory(&image, output_buffer, &memory_size,
                                                  convert_to_8bit, image_buffer, row_stride, NULL);
                        free(output_buffer);
                    }
                }
            }
            
#ifdef PNG_SIMPLIFIED_WRITE_STDIO_SUPPORTED
            // Write to stdio
            FILE* tmp_file = tmpfile();
            if (tmp_file) {
                png_image_write_to_stdio(&image, tmp_file, convert_to_8bit,
                                         image_buffer, row_stride, NULL);
                fclose(tmp_file);
            }
            
            // Write to file
            const char* temp_filename = "/tmp/libpng_fuzz_combined_test.png";
            png_image_write_to_file(&image, temp_filename, convert_to_8bit,
                                    image_buffer, row_stride, NULL);
#endif
            break;
        }
        
        case 4: {
            // TEST: Traditional write structure API (png_create_write_struct + png_set_write_fn)
            // STEP 3: Create write structure
            png_structp png_write_ptr = NULL;
            if (use_write_struct_2) {
                // Use png_create_write_struct_2
                png_write_ptr = png_create_write_struct_2(PNG_LIBPNG_VER_STRING,
                                                         NULL, NULL, NULL,
                                                         NULL, NULL, NULL);
            } else {
                // Use png_create_write_struct
                png_write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                       NULL, NULL, NULL);
            }
            
            if (!png_write_ptr) {
                break;  // Failed to create write structure
            }
            
            // Create info structure
            png_infop write_info_ptr = png_create_info_struct(png_write_ptr);
            if (!write_info_ptr) {
                png_destroy_write_struct(&png_write_ptr, NULL);
                break;
            }
            
            // Set up error handling
            if (setjmp(png_jmpbuf(png_write_ptr))) {
                png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
                break;
            }
            
            // STEP 4: Set up custom write callback for memory output
            png_write_memory_data write_data;
            write_data.buffer = NULL;
            write_data.size = 0;
            write_data.offset = 0;
            write_data.capacity = 1024;  // Initial capacity
            
            write_data.buffer = (uint8_t*)malloc(write_data.capacity);
            if (!write_data.buffer) {
                png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
                break;
            }
            
            png_set_write_fn(png_write_ptr, &write_data,
                            png_write_memory_callback, png_write_memory_flush);
            
            // Set image properties
            png_set_IHDR(png_write_ptr, write_info_ptr,
                        image.width, image.height,
                        8,  // bit depth
                        PNG_COLOR_TYPE_RGB,  // color type
                        PNG_INTERLACE_NONE,
                        PNG_COMPRESSION_TYPE_DEFAULT,
                        PNG_FILTER_TYPE_DEFAULT);
            
            // Write header
            png_write_info(png_write_ptr, write_info_ptr);
            
            // Convert our image buffer to row pointers for writing
            // This is simplified - in real usage we'd need proper conversion
            // based on the actual image format
            
            // Write end info
            png_write_end(png_write_ptr, write_info_ptr);
            
            // STEP 8: Clean up write structure
            png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
            
            // Free memory buffer
            free(write_data.buffer);
            break;
        }
        
        case 5: {
            // TEST: Error paths - invalid parameters for write APIs
            // Test png_image_write_to_memory with NULL image pointer
            png_alloc_size_t dummy_size = 0;
            png_image_write_to_memory(NULL, NULL, &dummy_size,
                                      convert_to_8bit, image_buffer, row_stride, NULL);
            
            // Test with invalid buffer
            png_image_write_to_memory(&image, NULL, NULL,
                                      convert_to_8bit, image_buffer, row_stride, NULL);
            
            // Test with zero-sized buffer
            void* zero_buffer = malloc(0);
            png_alloc_size_t zero_size = 0;
            if (zero_buffer) {
                png_image_write_to_memory(&image, zero_buffer, &zero_size,
                                          convert_to_8bit, image_buffer, row_stride, NULL);
                free(zero_buffer);
            }
            break;
        }
        
        case 6: {
            // TEST: Different convert_to_8bit values
            // Test with convert_to_8bit = 0 (16-bit linear if format is linear)
            png_alloc_size_t size1 = 0;
            if (png_image_write_to_memory(&image, NULL, &size1,
                                          0, image_buffer, row_stride, NULL) != 0) {
                if (size1 > 0 && size1 < 10 * 1024 * 1024) {
                    void* buf1 = malloc(size1);
                    if (buf1) {
                        png_image_write_to_memory(&image, buf1, &size1,
                                                  0, image_buffer, row_stride, NULL);
                        free(buf1);
                    }
                }
            }
            
            // Test with convert_to_8bit = 1 (8-bit sRGB)
            png_alloc_size_t size2 = 0;
            if (png_image_write_to_memory(&image, NULL, &size2,
                                          1, image_buffer, row_stride, NULL) != 0) {
                if (size2 > 0 && size2 < 10 * 1024 * 1024) {
                    void* buf2 = malloc(size2);
                    if (buf2) {
                        png_image_write_to_memory(&image, buf2, &size2,
                                                  1, image_buffer, row_stride, NULL);
                        free(buf2);
                    }
                }
            }
            break;
        }
        
        case 7: {
            // TEST: Different row stride values
            // Test with zero stride (let libpng calculate)
            png_alloc_size_t size_zero = 0;
            if (png_image_write_to_memory(&image, NULL, &size_zero,
                                          convert_to_8bit, image_buffer, 0, NULL) != 0) {
                if (size_zero > 0 && size_zero < 10 * 1024 * 1024) {
                    void* buf_zero = malloc(size_zero);
                    if (buf_zero) {
                        png_image_write_to_memory(&image, buf_zero, &size_zero,
                                                  convert_to_8bit, image_buffer, 0, NULL);
                        free(buf_zero);
                    }
                }
            }
            
            // Test with custom positive stride
            png_int_32 custom_stride = PNG_IMAGE_ROW_STRIDE(image) * 2;
            png_alloc_size_t size_custom = 0;
            if (png_image_write_to_memory(&image, NULL, &size_custom,
                                          convert_to_8bit, image_buffer, custom_stride, NULL) != 0) {
                if (size_custom > 0 && size_custom < 10 * 1024 * 1024) {
                    void* buf_custom = malloc(size_custom);
                    if (buf_custom) {
                        png_image_write_to_memory(&image, buf_custom, &size_custom,
                                                  convert_to_8bit, image_buffer, custom_stride, NULL);
                        free(buf_custom);
                    }
                }
            }
            break;
        }
    }
    
    // STEP 9: Free image resources
    free(image_buffer);
    png_image_free(&image);
    
    return 0;
}

} // namespace harness_013
