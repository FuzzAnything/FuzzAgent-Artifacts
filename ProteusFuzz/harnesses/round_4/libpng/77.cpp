// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_swap at pngtrans.c:34:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_read_rows at pngread.c:641:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
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
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>

static void write_dummy_png_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    // Write a minimal valid PNG header if data is too small
    if (size < 8) {
        const uint8_t header[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
        fwrite(header, 1, 8, f);
        if (size > 0) fwrite(data, 1, size, f);
    } else {
        fwrite(data, 1, size, f);
    }
    fclose(f);
}

static void setup_png_structs(png_structpp png_ptr, png_infopp info_ptr, int mode) {
    *png_ptr = nullptr;
    *info_ptr = nullptr;
    
    // Use a hardcoded version string compatible with libpng
    const char *user_ver = "1.6.37";
    
    if (mode == 1) {
        *png_ptr = png_create_read_struct(user_ver, nullptr, nullptr, nullptr);
    } else {
        *png_ptr = png_create_write_struct(user_ver, nullptr, nullptr, nullptr);
    }
    
    if (!*png_ptr) return;
    
    *info_ptr = png_create_info_struct(*png_ptr);
    if (!*info_ptr) {
        png_destroy_read_struct(png_ptr, nullptr, nullptr);
        *png_ptr = nullptr;
        return;
    }
}

static void cleanup_png_structs(png_structpp png_ptr, png_infopp info_ptr, int mode) {
    if (mode == 1) {
        png_destroy_read_struct(png_ptr, info_ptr, nullptr);
    } else {
        png_destroy_write_struct(png_ptr, info_ptr);
    }
    *png_ptr = nullptr;
    *info_ptr = nullptr;
}

static int try_read_png(png_structpp png_ptr, png_infopp info_ptr, const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;
    
    if (setjmp(png_jmpbuf(*png_ptr))) {
        fclose(fp);
        return 0;
    }
    
    png_init_io(*png_ptr, fp);
    
    // Read minimal info
    png_read_info(*png_ptr, *info_ptr);
    
    png_uint_32 width = png_get_image_width(*png_ptr, *info_ptr);
    png_uint_32 height = png_get_image_height(*png_ptr, *info_ptr);
    int bit_depth = png_get_bit_depth(*png_ptr, *info_ptr);
    int color_type = png_get_color_type(*png_ptr, *info_ptr);
    
    if (width == 0 || height == 0 || width > 10000 || height > 10000) {
        fclose(fp);
        return 0;
    }
    
    png_byte **row_pointers = (png_byte **)malloc(height * sizeof(png_byte *));
    if (!row_pointers) {
        fclose(fp);
        return 0;
    }
    
    size_t row_bytes = png_get_rowbytes(*png_ptr, *info_ptr);
    if (row_bytes == 0) {
        free(row_pointers);
        fclose(fp);
        return 0;
    }
    
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte *)malloc(row_bytes);
        if (!row_pointers[y]) {
            for (png_uint_32 i = 0; i < y; i++) free(row_pointers[i]);
            free(row_pointers);
            fclose(fp);
            return 0;
        }
    }
    
    // Apply transformations
    png_set_swap(*png_ptr);
    png_set_swap_alpha(*png_ptr);
    
    png_read_update_info(*png_ptr, *info_ptr);
    
    // Test png_read_rows
    png_read_rows(*png_ptr, row_pointers, nullptr, height);
    
    // Test png_read_row individually (loop)
    for (png_uint_32 y = 0; y < height; y++) {
        png_read_row(*png_ptr, row_pointers[y], nullptr);
    }
    
    // Cleanup row pointers
    for (png_uint_32 y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);
    fclose(fp);
    return 1;
}

static int try_write_png(png_structpp png_ptr, png_infopp info_ptr, const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    
    if (setjmp(png_jmpbuf(*png_ptr))) {
        fclose(fp);
        return 0;
    }
    
    png_init_io(*png_ptr, fp);
    
    png_uint_32 width = 10;
    png_uint_32 height = 10;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;
    
    png_set_IHDR(*png_ptr, *info_ptr, width, height, bit_depth, color_type, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    
    png_set_swap(*png_ptr);
    png_set_swap_alpha(*png_ptr);
    
    png_write_info(*png_ptr, *info_ptr);
    
    size_t row_bytes = width * 3; // RGB
    png_byte **row_pointers = (png_byte **)malloc(height * sizeof(png_byte *));
    if (!row_pointers) {
        fclose(fp);
        return 0;
    }
    
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte *)malloc(row_bytes);
        if (!row_pointers[y]) {
            for (png_uint_32 i = 0; i < y; i++) free(row_pointers[i]);
            free(row_pointers);
            fclose(fp);
            return 0;
        }
        // Fill with fuzz data or pattern
        if (size > 0) {
            size_t copy_len = (size > row_bytes) ? row_bytes : size;
            memcpy(row_pointers[y], data, copy_len);
            if (copy_len < row_bytes) {
                memset(row_pointers[y] + copy_len, 0, row_bytes - copy_len);
            }
        } else {
            memset(row_pointers[y], 0, row_bytes);
        }
    }
    
    // Test png_write_rows
    png_write_rows(*png_ptr, row_pointers, height);
    
    // Test png_write_row individually
    for (png_uint_32 y = 0; y < height; y++) {
        png_write_row(*png_ptr, row_pointers[y]);
    }
    
    png_write_end(*png_ptr, nullptr);
    
    for (png_uint_32 y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);
    fclose(fp);
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Write dummy file
    write_dummy_png_file(Data, Size);
    
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    
    // Try reading
    setup_png_structs(&png_ptr, &info_ptr, 1);
    if (png_ptr && info_ptr) {
        try_read_png(&png_ptr, &info_ptr, Data, Size);
        cleanup_png_structs(&png_ptr, &info_ptr, 1);
    }
    
    // Try writing
    setup_png_structs(&png_ptr, &info_ptr, 2);
    if (png_ptr && info_ptr) {
        try_write_png(&png_ptr, &info_ptr, Data, Size);
        cleanup_png_structs(&png_ptr, &info_ptr, 2);
    }
    
    return 0;
}