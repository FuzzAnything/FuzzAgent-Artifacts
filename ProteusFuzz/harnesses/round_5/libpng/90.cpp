// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_image_write_to_file at pngwrite.c:2402:1 in png.h
// png_image_finish_read at pngread.c:4069:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_quantize at pngrtran.c:472:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_image_write_to_stdio at pngwrite.c:2353:1 in png.h
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_image_free at png.c:3989:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <png.h>

extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Extract parameters from fuzz input
    uint8_t mode = Data[0] % 6;
    uint8_t width = (Data[1] % 100) + 1;
    uint8_t height = (Data[2] % 100) + 1;
    uint8_t bit_depth = (Data[3] % 8) + 1;
    uint8_t color_type = Data[4] % 7;
    uint8_t interlace = Data[5] % 2;
    uint8_t compression = Data[6] % 6;
    uint8_t filter = Data[7] % 6;
    uint8_t chunk = Data[8] % 2;
    uint8_t convert_to_8bit = Data[9] % 2;

    // Initialize png_image structure
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.width = width;
    image.height = height;
    image.format = (png_uint_32)color_type;
    // Note: png_image does not have a 'bit_depth' member in recent versions.
    // Bit depth is often inferred from format or handled internally.
    // We set flags to include interlacing if needed.
    // PNG_IMAGE_FLAG_INTERLACED is not a standard macro in libpng for png_image.flags
    // The interlace method is usually set via the 'flags' member but the constant might differ or be implicit.
    // In modern libpng, the interlace is often handled by the 'flags' field but the specific constant
    // might be defined differently or not exposed directly for setting in this manner.
    // We will skip setting the interlace flag directly to avoid compilation errors,
    // as the core fuzzing logic relies on the buffer and image dimensions.
    if (interlace) {
        // Attempt to set interlace if the flag exists, otherwise skip.
        // Since PNG_IMAGE_FLAG_INTERLACED caused an error, we assume it's not available or named differently.
        // We rely on the library's default behavior or other mechanisms.
        // No-op for now to ensure compilation.
    }

    // Calculate row stride manually based on format and bit depth
    // This is a simplified calculation; actual calculation depends on format
    int channels = 1;
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_PALETTE) channels = 3;
    else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) channels = 2;
    else if (color_type == PNG_COLOR_TYPE_RGBA) channels = 4;
    
    png_int_32 row_stride = 0;
    
    if (bit_depth < 8) {
        // Packed pixels for grayscale
        if (color_type == PNG_COLOR_TYPE_GRAY) {
            row_stride = (width * bit_depth + 7) / 8;
        } else if (color_type == PNG_COLOR_TYPE_PALETTE) {
            // Palette is always 8-bit index per pixel
            row_stride = width;
        } else {
            // Fallback for other packed types
            row_stride = (width * bit_depth + 7) / 8;
        }
    } else {
        // Standard unpacked pixels
        int bytes_per_pixel = channels * (bit_depth / 8);
        row_stride = width * bytes_per_pixel;
    }

    // Ensure row_stride is positive and reasonable
    if (row_stride <= 0) row_stride = width;

    size_t buffer_size = (size_t)row_stride * height;
    if (buffer_size > 1024 * 1024) return 0; // Limit buffer size

    void *buffer = malloc(buffer_size);
    if (!buffer) return 0;
    memset(buffer, 0, buffer_size);

    // Fill buffer with fuzz data if available
    if (Size > 10) {
        size_t copy_size = (Size - 10 < buffer_size) ? (Size - 10) : buffer_size;
        memcpy(buffer, Data + 10, copy_size);
    }

    // Create dummy file for file-based operations
    const char *filename = "./dummy_file";
    FILE *file = fopen(filename, "wb");
    if (!file) {
        free(buffer);
        return 0;
    }

    // Allocate colormap if needed
    png_color *colormap = nullptr;
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        int num_palette = (Data[10] % 256) + 1;
        colormap = (png_color *)malloc(num_palette * sizeof(png_color));
        if (colormap) {
            for (int i = 0; i < num_palette; ++i) {
                colormap[i].red = Data[11 + (i % (Size - 11))];
                colormap[i].green = Data[12 + (i % (Size - 12))];
                colormap[i].blue = Data[13 + (i % (Size - 13))];
            }
        }
    }

    // Execute fuzzing based on mode
    switch (mode) {
        case 0: // png_image_write_to_file
            png_image_write_to_file(&image, filename, convert_to_8bit, buffer, row_stride, colormap);
            break;
        case 1: // png_image_finish_read (requires reading first, but we simulate)
            {
                png_const_colorp background = nullptr;
                png_image_finish_read(&image, background, buffer, row_stride, colormap);
            }
            break;
        case 2: // png_set_quantize (requires png_struct, simulated)
            {
                png_struct *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
                if (png_ptr) {
                    png_info *info_ptr = png_create_info_struct(png_ptr);
                    if (info_ptr) {
                        int num_palette = (Data[10] % 256) + 1;
                        int max_colors = (Data[11] % 256) + 1;
                        int full_quantize = Data[12] % 2;
                        png_color *palette = (png_color *)malloc(num_palette * sizeof(png_color));
                        if (palette) {
                            for (int i = 0; i < num_palette; ++i) {
                                palette[i].red = Data[13 + (i % (Size - 13))];
                                palette[i].green = Data[14 + (i % (Size - 14))];
                                palette[i].blue = Data[15 + (i % (Size - 15))];
                            }
                            png_uint_16 *histogram = (png_uint_16 *)malloc(num_palette * sizeof(png_uint_16));
                            if (histogram) {
                                for (int i = 0; i < num_palette; ++i) {
                                    histogram[i] = Data[16 + (i % (Size - 16))];
                                }
                                png_set_quantize(png_ptr, palette, num_palette, max_colors, histogram, full_quantize);
                                free(histogram);
                            }
                            free(palette);
                        }
                        png_destroy_info_struct(png_ptr, &info_ptr);
                    }
                    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
                }
            }
            break;
        case 3: // png_image_write_to_stdio
            png_image_write_to_stdio(&image, file, convert_to_8bit, buffer, row_stride, colormap);
            break;
        case 4: // png_image_write_to_memory
            {
                png_alloc_size_t memory_bytes = 0;
                void *memory = nullptr;
                // First call to get required size
                png_image_write_to_memory(&image, nullptr, &memory_bytes, convert_to_8bit, buffer, row_stride, colormap);
                if (memory_bytes > 0 && memory_bytes < 1024 * 1024) {
                    memory = malloc(memory_bytes);
                    if (memory) {
                        png_image_write_to_memory(&image, memory, &memory_bytes, convert_to_8bit, buffer, row_stride, colormap);
                        free(memory);
                    }
                }
            }
            break;
        case 5: // png_image_free
            png_image_free(&image);
            break;
    }

    // Cleanup
    fclose(file);
    free(buffer);
    if (colormap) free(colormap);
    return 0;
}