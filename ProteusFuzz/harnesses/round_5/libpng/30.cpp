// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_current_pass_number at pngtrans.c:859:1 in png.h
// png_get_current_row_number at pngtrans.c:847:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_read_rows at pngread.c:641:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_bytep *row_pointers = NULL;
    png_byte *row_buffer = NULL;
    png_byte *display_row_buffer = NULL;
    png_uint_32 width = 0, height = 0;
    int bit_depth = 0, color_type = 0;
    int interlace_type = 0;

    // --- Read Test ---
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;
    
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    png_set_error_fn(png_ptr, NULL, dummy_error_fn, dummy_warning_fn);

    if (fp = fopen("./dummy_file", "rb")) {
        png_init_io(png_ptr, fp);
        
        if (setjmp(png_jmpbuf(png_ptr)) == 0) {
            png_read_info(png_ptr, info_ptr);
            
            png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);
            
            // Limit dimensions to prevent excessive allocation or invalid states
            if (width > 1024 || height > 1024) {
                fclose(fp);
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                return 0;
            }

            if (width > 0 && height > 0) {
                size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
                row_buffer = (png_byte*)malloc(row_bytes);
                display_row_buffer = (png_byte*)malloc(row_bytes);
                if (row_buffer && display_row_buffer) {
                    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
                    if (row_pointers) {
                        for (png_uint_32 y = 0; y < height; y++) {
                            row_pointers[y] = row_buffer;
                        }
                    }
                }
            }

            if (setjmp(png_jmpbuf(png_ptr)) == 0) {
                png_read_update_info(png_ptr, info_ptr);

                // Test png_get_current_pass_number
                png_byte current_pass = png_get_current_pass_number(png_ptr);
                (void)current_pass;

                // Test png_get_current_row_number
                png_uint_32 current_row = png_get_current_row_number(png_ptr);
                (void)current_row;

                // Test png_read_row in a loop
                if (row_buffer && display_row_buffer && height > 0) {
                    for (png_uint_32 y = 0; y < height; y++) {
                        png_read_row(png_ptr, row_buffer, display_row_buffer);
                    }
                }

                // Test png_read_rows
                if (row_pointers && height > 0) {
                    png_read_rows(png_ptr, row_pointers, NULL, height);
                }

                // Test png_read_image
                if (row_pointers && height > 0) {
                    png_read_image(png_ptr, row_pointers);
                }
            }
        }
        fclose(fp);
    }

    if (row_pointers) free(row_pointers);
    if (row_buffer) free(row_buffer);
    if (display_row_buffer) free(display_row_buffer);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    // --- Write Test ---
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;
    
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    png_set_error_fn(png_ptr, NULL, dummy_error_fn, dummy_warning_fn);

    if (fp = fopen("./dummy_file", "wb")) {
        png_init_io(png_ptr, fp);
        
        // Safe dimension calculation
        width = (Size > 10) ? (Data[0] % 100) + 1 : 10;
        height = (Size > 11) ? (Data[1] % 100) + 1 : 10;
        
        // Ensure non-zero dimensions
        if (width == 0) width = 1;
        if (height == 0) height = 1;
        
        bit_depth = 8;
        color_type = PNG_COLOR_TYPE_RGB;
        
        png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
        
        if (setjmp(png_jmpbuf(png_ptr)) == 0) {
            png_write_info(png_ptr, info_ptr);

            size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
            row_buffer = (png_byte*)malloc(row_bytes);
            
            if (row_buffer) {
                // Prepare row pointers array for png_write_rows
                row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
                
                if (row_pointers) {
                    memset(row_buffer, 0, row_bytes);
                    for (size_t i = 0; i < row_bytes && i < Size; i++) {
                        row_buffer[i] = Data[i % Size];
                    }
                    
                    // Initialize all row pointers to point to the same buffer
                    for (png_uint_32 y = 0; y < height; y++) {
                        row_pointers[y] = row_buffer;
                    }

                    // Test png_write_rows with the correctly formed array of pointers
                    png_write_rows(png_ptr, row_pointers, height);
                }
                
                // Cleanup row_pointers allocated for write test
                if (row_pointers) free(row_pointers);
            }

            png_write_end(png_ptr, NULL);
        }
        fclose(fp);
    }

    if (row_buffer) free(row_buffer);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}