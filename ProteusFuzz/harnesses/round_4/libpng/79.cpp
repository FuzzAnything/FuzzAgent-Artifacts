// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_read_rows at pngread.c:641:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
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

static void write_dummy_png(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    
    // Write a minimal valid PNG header
    const uint8_t png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(png_sig, 1, 8, f);
    
    // Write IHDR chunk (minimal valid header)
    uint8_t ihdr[13] = {0, 0, 0, 1, 0, 0, 0, 1, 8, 2, 0, 0, 0}; // 1x1 RGB
    uint32_t len = 13;
    uint32_t crc = 0; // Simplified, real CRC calculation omitted for fuzzing
    
    fwrite(&len, 4, 1, f);
    fwrite("IHDR", 4, 1, f);
    fwrite(ihdr, 1, 13, f);
    fwrite(&crc, 4, 1, f);
    
    // Write IDAT chunk with fuzz data
    uint32_t data_len = size > 0 ? size : 1;
    fwrite(&data_len, 4, 1, f);
    fwrite("IDAT", 4, 1, f);
    if (data && size > 0) {
        fwrite(data, 1, size, f);
    } else {
        fwrite("\x00", 1, 1, f);
    }
    fwrite(&crc, 4, 1, f);
    
    // Write IEND chunk
    len = 0;
    fwrite(&len, 4, 1, f);
    fwrite("IEND", 4, 1, f);
    fwrite(&crc, 4, 1, f);
    
    fclose(f);
}

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    } else if (info_ptr) {
        png_destroy_info_struct(NULL, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Write dummy PNG file
    write_dummy_png(Data, Size);
    
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;
    
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return 0;
    }
    
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        fclose(fp);
        return 0;
    }
    
    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);
    
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    
    // Allocate row buffers
    png_byte **row_pointers = NULL;
    png_byte **display_pointers = NULL;
    png_byte **image_pointers = NULL;
    
    if (width > 0 && height > 0 && width < 1000 && height < 1000) {
        size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        if (row_bytes > 0 && row_bytes < 100000) {
            row_pointers = (png_byte **)malloc(height * sizeof(png_byte *));
            display_pointers = (png_byte **)malloc(height * sizeof(png_byte *));
            image_pointers = (png_byte **)malloc(height * sizeof(png_byte *));
            
            if (row_pointers && display_pointers && image_pointers) {
                for (png_uint_32 i = 0; i < height; i++) {
                    row_pointers[i] = (png_byte *)malloc(row_bytes);
                    display_pointers[i] = (png_byte *)malloc(row_bytes);
                    image_pointers[i] = (png_byte *)malloc(row_bytes);
                    if (!row_pointers[i] || !display_pointers[i] || !image_pointers[i]) {
                        break;
                    }
                    memset(row_pointers[i], 0, row_bytes);
                    memset(display_pointers[i], 0, row_bytes);
                    memset(image_pointers[i], 0, row_bytes);
                }
            }
        }
    }
    
    // Test png_set_swap and png_set_swap_alpha
    png_set_swap(png_ptr);
    png_set_swap_alpha(png_ptr);
    
    // Prepare for reading rows
    png_read_update_info(png_ptr, info_ptr);
    
    // Test png_read_row (single row)
    if (row_pointers && height > 0) {
        png_read_row(png_ptr, row_pointers[0], display_pointers[0]);
    }
    
    // Test png_read_rows (multiple rows)
    if (row_pointers && height > 1) {
        png_read_rows(png_ptr, row_pointers, display_pointers, height > 2 ? 2 : height);
    }
    
    // Test png_read_image (full image)
    if (image_pointers && height > 0) {
        png_read_image(png_ptr, image_pointers);
    }
    
    // Cleanup row buffers
    if (row_pointers) {
        for (png_uint_32 i = 0; i < height; i++) {
            if (row_pointers[i]) free(row_pointers[i]);
        }
        free(row_pointers);
    }
    if (display_pointers) {
        for (png_uint_32 i = 0; i < height; i++) {
            if (display_pointers[i]) free(display_pointers[i]);
        }
        free(display_pointers);
    }
    if (image_pointers) {
        for (png_uint_32 i = 0; i < height; i++) {
            if (image_pointers[i]) free(image_pointers[i]);
        }
        free(image_pointers);
    }
    
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    
    // Test write functions with a new context
    png_structp write_png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!write_png) return 0;
    
    png_infop write_info = png_create_info_struct(write_png);
    if (!write_info) {
        png_destroy_write_struct(&write_png, NULL);
        return 0;
    }
    
    if (setjmp(png_jmpbuf(write_png))) {
        cleanup_png(write_png, write_info);
        return 0;
    }
    
    // Create a dummy file for writing
    FILE *wfp = fopen("./dummy_file", "wb");
    if (!wfp) {
        cleanup_png(write_png, write_info);
        return 0;
    }
    
    png_init_io(write_png, wfp);
    
    // Set minimal image info for writing
    png_set_IHDR(write_png, write_info, 1, 1, 8, PNG_COLOR_TYPE_RGB, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(write_png, write_info);
    
    // Allocate row for writing
    png_byte *write_row = (png_byte *)malloc(3);
    if (write_row) {
        write_row[0] = 0; write_row[1] = 0; write_row[2] = 0;
        
        // Test png_write_rows
        png_byte **write_rows = &write_row;
        png_write_rows(write_png, write_rows, 1);
        
        free(write_row);
    }
    
    png_write_end(write_png, write_info);
    cleanup_png(write_png, write_info);
    fclose(wfp);
    
    return 0;
}