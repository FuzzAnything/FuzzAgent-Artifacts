// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_compression_method at pngwrite.c:1253:1 in png.h
// png_set_quantize at pngrtran.c:472:1 in png.h
// png_set_gamma_fixed at pngrtran.c:859:1 in png.h
// png_set_gamma at pngrtran.c:901:1 in png.h
// png_set_flush at pngwrite.c:966:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_check_for_invalid_index at pngset.c:2024:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
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
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>

static jmp_buf jump_buffer;
static volatile sig_atomic_t jump_occurred = 0;

static void dummy_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Trigger the jump instead of aborting
    jump_occurred = 1;
    longjmp(jump_buffer, 1);
}

static void dummy_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_structp read_ptr = NULL;
    png_infop read_info_ptr = NULL;
    
    int method = 0;
    int full_quantize = 0;
    int nrows = 0;
    int allowed = 0;
    double screen_gamma = 0.0;
    double file_gamma = 0.0;
    png_fixed_point screen_gamma_fixed = 0;
    png_fixed_point file_gamma_fixed = 0;
    
    // Extract parameters from input data safely
    method = Data[0];
    if (Size > 1) full_quantize = Data[1];
    if (Size > 2) nrows = Data[2];
    if (Size > 3) allowed = Data[3];
    
    // Check size before reading doubles (8 bytes each)
    if (Size > 15) { // Need 4 + 8 + 8 = 20 bytes total for both doubles, but let's be safe
        // Actually, indices: 4, 5, 6, 7 for first double (8 bytes)
        // 12, 13, 14, 15, 16, 17, 18, 19 for second double (8 bytes)
        // So we need Size > 19 to read both safely.
        // Let's adjust logic to read what is available.
        
        if (Size >= 12) {
            memcpy(&screen_gamma, Data + 4, sizeof(double));
        }
        if (Size >= 20) {
            memcpy(&file_gamma, Data + 12, sizeof(double));
        }
    }
    
    // Check size before reading fixed points (4 bytes each)
    if (Size >= 20) {
        if (Size >= 20) {
            memcpy(&screen_gamma_fixed, Data + 16, sizeof(png_fixed_point));
        }
        if (Size >= 24) {
            memcpy(&file_gamma_fixed, Data + 20, sizeof(png_fixed_point));
        }
    }

    // Create palette and histogram based on input size
    png_color *palette = NULL;
    png_uint_16 *histogram = NULL;
    int num_palette = 0;
    int maximum_colors = 0;

    // Ensure we have enough data before reading palette params
    if (Size > 25) {
        num_palette = Data[24];
        maximum_colors = Data[25];
        // Ensure reasonable bounds to avoid huge allocations
        if (num_palette > 256) num_palette = 256;
        if (maximum_colors > 256) maximum_colors = 256;
        
        if (num_palette > 0) {
            palette = (png_color *)malloc(num_palette * sizeof(png_color));
            if (palette) {
                size_t data_offset = 26;
                size_t data_len = Size - data_offset;
                if (data_len > 0) {
                    for (int i = 0; i < num_palette; i++) {
                        size_t idx = i * 3;
                        if (idx < data_len) palette[i].red = Data[data_offset + idx];
                        else palette[i].red = 0;
                        
                        if (idx + 1 < data_len) palette[i].green = Data[data_offset + idx + 1];
                        else palette[i].green = 0;
                        
                        if (idx + 2 < data_len) palette[i].blue = Data[data_offset + idx + 2];
                        else palette[i].blue = 0;
                    }
                } else {
                    for (int i = 0; i < num_palette; i++) {
                        palette[i].red = 0;
                        palette[i].green = 0;
                        palette[i].blue = 0;
                    }
                }
            }
        }
        if (num_palette > 0) {
            histogram = (png_uint_16 *)malloc(num_palette * sizeof(png_uint_16));
            if (histogram) {
                size_t data_offset = 26;
                size_t data_len = Size - data_offset;
                if (data_len > 0) {
                    for (int i = 0; i < num_palette; i++) {
                        if (i < data_len) histogram[i] = Data[data_offset + i];
                        else histogram[i] = 0;
                    }
                } else {
                    for (int i = 0; i < num_palette; i++) {
                        histogram[i] = 0;
                    }
                }
            }
        }
    }

    // Initialize libpng structures
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        free(palette);
        free(histogram);
        return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        free(palette);
        free(histogram);
        return 0;
    }

    // Set custom error handler to prevent abort
    png_set_error_fn(png_ptr, NULL, dummy_error_handler, dummy_warning_handler);

    // Use setjmp to catch errors from png_error calls
    if (setjmp(jump_buffer) == 0) {
        // Test png_set_compression_method
        png_set_compression_method(png_ptr, method);

        // Test png_set_quantize
        png_set_quantize(png_ptr, palette, num_palette, maximum_colors, histogram, full_quantize);

        // Test png_set_gamma_fixed
        // This might trigger an error if gamma is invalid (e.g., 0 or negative)
        // Our handler will catch it via longjmp
        png_set_gamma_fixed(png_ptr, screen_gamma_fixed, file_gamma_fixed);

        // Test png_set_gamma
        png_set_gamma(png_ptr, screen_gamma, file_gamma);

        // Test png_set_flush
        png_set_flush(png_ptr, nrows);

        // Create read struct for png_set_check_for_invalid_index
        read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (read_ptr) {
            read_info_ptr = png_create_info_struct(read_ptr);
            if (read_info_ptr) {
                png_set_error_fn(read_ptr, NULL, dummy_error_handler, dummy_warning_handler);
                png_set_check_for_invalid_index(read_ptr, allowed);
                png_destroy_read_struct(&read_ptr, &read_info_ptr, NULL);
            } else {
                png_destroy_read_struct(&read_ptr, NULL, NULL);
            }
        }

        // Test png_error explicitly if we want to trigger it directly
        // We do this inside the setjmp block so it gets caught
        const char *error_msg = "Fuzzing error";
        if (Size > 30) {
            size_t msg_len = Size - 30;
            if (msg_len > 100) msg_len = 100;
            char *msg_buf = (char *)malloc(msg_len + 1);
            if (msg_buf) {
                memcpy(msg_buf, Data + 30, msg_len);
                msg_buf[msg_len] = '\0';
                error_msg = msg_buf;
                // This will call our dummy_error_handler which does longjmp
                png_error(png_ptr, error_msg);
                free(msg_buf);
            }
        } else {
            png_error(png_ptr, error_msg);
        }
    } else {
        // Error was caught via longjmp
        // Cleanup happens below
    }

    // Cleanup
    if (read_ptr) {
        // If read_ptr was created but not destroyed inside the block (due to jump), destroy it
        // Note: If jump happened during read_ptr creation, read_ptr might be valid but info might not be fully set up.
        // We check if read_info_ptr is valid.
        if (read_info_ptr) {
            png_destroy_read_struct(&read_ptr, &read_info_ptr, NULL);
        } else {
            png_destroy_read_struct(&read_ptr, NULL, NULL);
        }
    }

    png_destroy_write_struct(&png_ptr, &info_ptr);
    free(palette);
    free(histogram);

    return 0;
}