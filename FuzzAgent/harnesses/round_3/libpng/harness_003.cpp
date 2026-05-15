#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_003 {

// Function to create a temporary file for testing file-based APIs
static FILE* create_temp_file(const uint8_t* data, size_t size) {
    FILE* tmp = tmpfile();
    if (tmp && size > 0) {
        fwrite(data, 1, size, tmp);
        rewind(tmp);
    }
    return tmp;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 100) {
        return 0;  // Need minimum data for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Consume parameters from fuzzer input
    int operation = fdp.ConsumeIntegralInRange<int>(0, 6);
    int format_choice = fdp.ConsumeIntegralInRange<int>(0, 15);
    int convert_to_8bit = fdp.ConsumeBool() ? 1 : 0;
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

    // Extract PNG data from remaining input
    size_t png_data_size = fdp.ConsumeIntegralInRange<size_t>(8, fdp.remaining_bytes());
    std::vector<uint8_t> png_data = fdp.ConsumeBytes<uint8_t>(png_data_size);

    // Test different operations based on consumed value
    switch (operation) {
        case 0: {
            // Test: png_image_begin_read_from_memory + png_image_finish_read + png_image_write_to_memory
            png_image image;
            memset(&image, 0, sizeof(image));
            image.version = PNG_IMAGE_VERSION;
            image.opaque = NULL;

            // Begin reading from memory
            if (png_image_begin_read_from_memory(&image, png_data.data(), png_data.size()) != 0) {
                // Set format for output
                image.format = format;

                // Calculate buffer size needed
                png_uint_32 buffer_size = PNG_IMAGE_SIZE(image);
                if (buffer_size > 0 && buffer_size < 10 * 1024 * 1024) {  // Sanity check
                    void* buffer = malloc(buffer_size);
                    if (buffer) {
                        // Calculate row stride (positive for top-down, negative for bottom-up)
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

                        // Finish reading into buffer
                        png_color background = {255, 255, 255};  // White background
                        if (png_image_finish_read(&image, &background, buffer, row_stride, NULL) != 0) {
                            // Now write the image back to memory
                            png_image write_image;
                            memset(&write_image, 0, sizeof(write_image));
                            write_image.version = PNG_IMAGE_VERSION;
                            write_image.opaque = NULL;
                            write_image.width = image.width;
                            write_image.height = image.height;
                            write_image.format = format;

                            // Calculate size needed for output
                            png_alloc_size_t output_size = 0;
                            if (png_image_write_to_memory(&write_image, NULL, &output_size,
                                                          convert_to_8bit, buffer, row_stride, NULL) != 0) {
                                if (output_size > 0 && output_size < 10 * 1024 * 1024) {
                                    void* output_buffer = malloc(output_size);
                                    if (output_buffer) {
                                        png_image_write_to_memory(&write_image, output_buffer, &output_size,
                                                                  convert_to_8bit, buffer, row_stride, NULL);
                                        free(output_buffer);
                                    }
                                }
                            }

                            png_image_free(&write_image);
                        }
                        free(buffer);
                    }
                }
                png_image_free(&image);
            }
            break;
        }

        case 1: {
            // Test: File-based read and write
#ifdef PNG_STDIO_SUPPORTED
            FILE* tmp_file = create_temp_file(png_data.data(), png_data.size());
            if (tmp_file) {
                png_image image;
                memset(&image, 0, sizeof(image));
                image.version = PNG_IMAGE_VERSION;
                image.opaque = NULL;

                if (png_image_begin_read_from_stdio(&image, tmp_file) != 0) {
                    image.format = format;

                    png_uint_32 buffer_size = PNG_IMAGE_SIZE(image);
                    if (buffer_size > 0 && buffer_size < 10 * 1024 * 1024) {
                        void* buffer = malloc(buffer_size);
                        if (buffer) {
                            png_int_32 row_stride = PNG_IMAGE_ROW_STRIDE(image);
                            png_color background = {200, 200, 200};  // Light gray background
                            if (png_image_finish_read(&image, &background, buffer, row_stride, NULL) != 0) {
                                // Write to a temporary file
                                const char* temp_filename = "/tmp/libpng_fuzz_test.png";
                                png_image_write_to_file(&image, temp_filename, convert_to_8bit,
                                                        buffer, row_stride, NULL);
                            }
                            free(buffer);
                        }
                    }
                    png_image_free(&image);
                }
                fclose(tmp_file);
            }
#endif
            break;
        }

        case 2: {
            // Test: Memory read with different buffer configurations
            png_image image;
            memset(&image, 0, sizeof(image));
            image.version = PNG_IMAGE_VERSION;
            image.opaque = NULL;

            if (png_image_begin_read_from_memory(&image, png_data.data(), png_data.size()) != 0) {
                image.format = format;

                // Try with various buffer alignments
                for (int alignment = 1; alignment <= 64; alignment *= 2) {
                    png_uint_32 buffer_size = PNG_IMAGE_SIZE(image);
                    if (buffer_size > 0) {
                        // Allocate with extra padding for alignment testing
                        void* buffer = malloc(buffer_size + alignment);
                        if (buffer) {
                            // Align buffer
                            void* aligned_buffer = (void*)(((uintptr_t)buffer + alignment - 1) & ~(alignment - 1));
                            
                            png_int_32 row_stride = PNG_IMAGE_ROW_STRIDE(image);
                            if (fdp.ConsumeBool()) {
                                // Test with negative stride (bottom-up)
                                row_stride = -row_stride;
                            }
                            
                            png_color background = {0, 0, 0};  // Black background
                            png_image_finish_read(&image, &background, aligned_buffer, row_stride, NULL);
                            free(buffer);
                        }
                    }
                }
                png_image_free(&image);
            }
            break;
        }

        case 3: {
            // Test: Error path - invalid memory input
            png_image image;
            memset(&image, 0, sizeof(image));
            image.version = PNG_IMAGE_VERSION;
            image.opaque = NULL;

            // Test with potentially invalid data
            const uint8_t* invalid_data = (const uint8_t*)"NOT_A_PNG";
            size_t invalid_size = 9;
            png_image_begin_read_from_memory(&image, invalid_data, invalid_size);
            png_image_free(&image);
            break;
        }

        case 4: {
            // Test: Partial/incomplete PNG data
            if (png_data_size > 8) {
                png_image image;
                memset(&image, 0, sizeof(image));
                image.version = PNG_IMAGE_VERSION;
                image.opaque = NULL;

                // Try with truncated data
                size_t truncated_size = png_data_size / 2;
                png_image_begin_read_from_memory(&image, png_data.data(), truncated_size);
                png_image_free(&image);
            }
            break;
        }

        case 5: {
            // Test: Multiple read/write cycles
            png_image image;
            memset(&image, 0, sizeof(image));
            image.version = PNG_IMAGE_VERSION;
            image.opaque = NULL;

            if (png_image_begin_read_from_memory(&image, png_data.data(), png_data.size()) != 0) {
                image.format = format;

                png_uint_32 buffer_size = PNG_IMAGE_SIZE(image);
                if (buffer_size > 0 && buffer_size < 5 * 1024 * 1024) {
                    void* buffer = malloc(buffer_size);
                    if (buffer) {
                        png_int_32 row_stride = PNG_IMAGE_ROW_STRIDE(image);
                        png_color background = {128, 128, 128};  // Gray background
                        
                        if (png_image_finish_read(&image, &background, buffer, row_stride, NULL) != 0) {
                            // Write multiple times with different parameters
                            for (int i = 0; i < 3; i++) {
                                png_image write_image;
                                memset(&write_image, 0, sizeof(write_image));
                                write_image.version = PNG_IMAGE_VERSION;
                                write_image.opaque = NULL;
                                write_image.width = image.width;
                                write_image.height = image.height;
                                write_image.format = format | (i * PNG_FORMAT_FLAG_ALPHA);

                                png_alloc_size_t output_size = 0;
                                if (png_image_write_to_memory(&write_image, NULL, &output_size,
                                                              i % 2, buffer, row_stride, NULL) != 0) {
                                    if (output_size > 0 && output_size < 5 * 1024 * 1024) {
                                        void* output_buffer = malloc(output_size);
                                        if (output_buffer) {
                                            png_image_write_to_memory(&write_image, output_buffer, &output_size,
                                                                      i % 2, buffer, row_stride, NULL);
                                            free(output_buffer);
                                        }
                                    }
                                }
                                png_image_free(&write_image);
                            }
                        }
                        free(buffer);
                    }
                }
                png_image_free(&image);
            }
            break;
        }

        case 6: {
            // Test: Edge cases with minimum/maximum values
            png_image image;
            memset(&image, 0, sizeof(image));
            image.version = PNG_IMAGE_VERSION;
            image.opaque = NULL;

            // Try with extremely small buffer
            if (png_data_size >= 8) {
                png_image_begin_read_from_memory(&image, png_data.data(), 8);
                png_image_free(&image);
            }
            break;
        }
    }

    return 0;
}

} // namespace harness_003
