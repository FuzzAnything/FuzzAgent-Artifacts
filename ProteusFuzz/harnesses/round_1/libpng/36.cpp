// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_read_rows at pngread.c:641:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
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

static void cleanup_png(png_structp png_ptr, png_infop info_ptr, FILE *fp, png_byte **row_bufs, int num_rows) {
    if (row_bufs) {
        for (int i = 0; i < num_rows; ++i) {
            free(row_bufs[i]);
        }
        free(row_bufs);
    }
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    }
    if (fp) {
        fclose(fp);
    }
}

static int try_write_fuzz(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        return 0;
    }
    
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr, fp, NULL, 0);
        return 0;
    }
    
    png_init_io(png_ptr, fp);
    
    // Create a minimal header
    png_set_IHDR(png_ptr, info_ptr, 10, 10, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);
    
    // Apply transformations
    png_set_swap(png_ptr);
    png_set_swap_alpha(png_ptr);
    
    // Prepare row data
    int w = 10;
    int bytes_per_pixel = 3; // RGB
    int row_bytes = w * bytes_per_pixel;
    int max_rows = (Size > 100) ? 10 : (Size / row_bytes);
    if (max_rows > 10) max_rows = 10;
    if (max_rows < 1) max_rows = 1;
    
    png_byte **row_bufs = (png_byte **)malloc(max_rows * sizeof(png_byte *));
    if (!row_bufs) {
        cleanup_png(png_ptr, info_ptr, fp, NULL, 0);
        return 0;
    }
    
    for (int i = 0; i < max_rows; ++i) {
        row_bufs[i] = (png_byte *)malloc(row_bytes);
        if (!row_bufs[i]) {
            for (int j = 0; j < i; ++j) free(row_bufs[j]);
            free(row_bufs);
            cleanup_png(png_ptr, info_ptr, fp, NULL, 0);
            return 0;
        }
        // Fill with fuzz data
        size_t avail = Size - (i * row_bytes);
        size_t to_copy = (avail > row_bytes) ? row_bytes : avail;
        if (to_copy > 0) {
            memcpy(row_bufs[i], Data + (i * row_bytes), to_copy);
            if (to_copy < row_bytes) {
                memset(row_bufs[i] + to_copy, 0, row_bytes - to_copy);
            }
        } else {
            memset(row_bufs[i], 0, row_bytes);
        }
    }
    
    // Call target API: png_write_rows
    png_write_rows(png_ptr, row_bufs, max_rows);
    
    png_write_end(png_ptr, info_ptr);
    
    cleanup_png(png_ptr, info_ptr, fp, row_bufs, max_rows);
    return 1;
}

static int try_read_fuzz(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0; // Need at least a valid PNG signature
    
    // Write a minimal valid PNG to dummy_file first
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    
    // Write PNG signature
    const uint8_t signature[] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(signature, 1, 8, fp);
    
    // Write IHDR chunk (minimal)
    uint32_t w_val = 10, h_val = 10;
    uint8_t bit_depth = 8, color_type = 2; // RGB
    uint8_t compression = 0, filter = 0, interlace = 0;
    
    uint32_t ihdr_len = 13;
    uint8_t ihdr_type[] = {'I', 'H', 'D', 'R'};
    uint8_t ihdr_data[13];
    
    // Encode width
    ihdr_data[0] = (w_val >> 24) & 0xFF;
    ihdr_data[1] = (w_val >> 16) & 0xFF;
    ihdr_data[2] = (w_val >> 8) & 0xFF;
    ihdr_data[3] = w_val & 0xFF;
    // Encode height
    ihdr_data[4] = (h_val >> 24) & 0xFF;
    ihdr_data[5] = (h_val >> 16) & 0xFF;
    ihdr_data[6] = (h_val >> 8) & 0xFF;
    ihdr_data[7] = h_val & 0xFF;
    ihdr_data[8] = bit_depth;
    ihdr_data[9] = color_type;
    ihdr_data[10] = compression;
    ihdr_data[11] = filter;
    ihdr_data[12] = interlace;
    
    fwrite(&ihdr_len, 4, 1, fp);
    fwrite(ihdr_type, 1, 4, fp);
    fwrite(ihdr_data, 1, 13, fp);
    
    // Calculate CRC (simplified: just zeros for fuzzing, libpng might skip or warn)
    uint32_t crc = 0;
    fwrite(&crc, 4, 1, fp);
    
    // Write IDAT chunk with fuzz data (compressed)
    // We'll just write raw data as if it were compressed (libpng will fail gracefully or handle)
    uint32_t idat_len = Size - 8; // Skip signature
    if (idat_len > 1000) idat_len = 1000;
    uint8_t idat_type[] = {'I', 'D', 'A', 'T'};
    
    fwrite(&idat_len, 4, 1, fp);
    fwrite(idat_type, 1, 4, fp);
    fwrite(Data + 8, 1, idat_len, fp);
    fwrite(&crc, 4, 1, fp); // Dummy CRC
    
    // Write IEND chunk
    uint32_t iend_len = 0;
    uint8_t iend_type[] = {'I', 'E', 'N', 'D'};
    fwrite(&iend_len, 4, 1, fp);
    fwrite(iend_type, 1, 4, fp);
    fwrite(&crc, 4, 1, fp);
    
    fclose(fp);
    
    // Now try to read
    fp = fopen("./dummy_file", "rb");
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
        cleanup_png(png_ptr, info_ptr, fp, NULL, 0);
        return 0;
    }
    
    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);
    
    // Apply transformations
    png_set_swap(png_ptr);
    png_set_swap_alpha(png_ptr);
    png_set_expand(png_ptr);
    
    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);
    int bytes_per_pixel = png_get_rowbytes(png_ptr, info_ptr) / width;
    int row_bytes = width * bytes_per_pixel;
    
    // Prepare buffers
    int max_rows = (height > 10) ? 10 : height;
    if (max_rows < 1) max_rows = 1;
    
    png_byte **row_bufs = (png_byte **)malloc(max_rows * sizeof(png_byte *));
    png_byte **display_bufs = (png_byte **)malloc(max_rows * sizeof(png_byte *));
    
    if (!row_bufs || !display_bufs) {
        if (row_bufs) free(row_bufs);
        if (display_bufs) free(display_bufs);
        cleanup_png(png_ptr, info_ptr, fp, NULL, 0);
        return 0;
    }
    
    for (int i = 0; i < max_rows; ++i) {
        row_bufs[i] = (png_byte *)malloc(row_bytes);
        display_bufs[i] = (png_byte *)malloc(row_bytes);
        if (!row_bufs[i] || !display_bufs[i]) {
            for (int j = 0; j < i; ++j) {
                free(row_bufs[j]);
                free(display_bufs[j]);
            }
            free(row_bufs);
            free(display_bufs);
            cleanup_png(png_ptr, info_ptr, fp, NULL, 0);
            return 0;
        }
    }
    
    // Call target API: png_read_rows
    png_read_rows(png_ptr, row_bufs, display_bufs, max_rows);
    
    // Also try png_read_row for a single row
    if (max_rows > 0) {
        png_read_row(png_ptr, row_bufs[0], display_bufs[0]);
    }
    
    png_read_end(png_ptr, info_ptr);
    
    // Cleanup
    for (int i = 0; i < max_rows; ++i) {
        free(row_bufs[i]);
        free(display_bufs[i]);
    }
    free(row_bufs);
    free(display_bufs);
    
    cleanup_png(png_ptr, info_ptr, fp, NULL, 0);
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Try both write and read fuzzing
    try_write_fuzz(Data, Size);
    try_read_fuzz(Data, Size);
    
    return 0;
}