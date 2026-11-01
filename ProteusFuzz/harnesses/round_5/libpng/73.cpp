// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_next_frame_x_offset at pngget.c:1449:1 in png.h
// png_get_next_frame_height at pngget.c:1439:1 in png.h
// png_get_next_frame_fcTL at pngget.c:1399:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_first_frame_is_hidden at pngset.c:1435:1 in png.h
// png_set_first_frame_is_hidden at pngset.c:1435:1 in png.h
// png_set_next_frame_fcTL at pngset.c:1357:1 in png.h
// png_write_frame_head at pngwrite.c:1501:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
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
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>

static void write_dummy_png(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    // Write a minimal PNG header
    unsigned char png_header[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
    fwrite(png_header, 1, 8, f);
    
    // Write IHDR chunk (minimal valid IHDR)
    uint32_t width = 1, height = 1;
    uint8_t bit_depth = 8, color_type = 2; // RGB
    uint8_t compression = 0, filter = 0, interlace = 0;
    
    uint32_t chunk_len = 13;
    uint8_t chunk_type[4] = {'I', 'H', 'D', 'R'};
    fwrite(&chunk_len, 4, 1, f);
    fwrite(chunk_type, 4, 1, f);
    fwrite(&width, 4, 1, f);
    fwrite(&height, 4, 1, f);
    fwrite(&bit_depth, 1, 1, f);
    fwrite(&color_type, 1, 1, f);
    fwrite(&compression, 1, 1, f);
    fwrite(&filter, 1, 1, f);
    fwrite(&interlace, 1, 1, f);
    
    // CRC (dummy)
    uint32_t crc = 0;
    fwrite(&crc, 4, 1, f);
    
    // Write acTL chunk for APNG
    uint32_t num_frames = 1;
    uint32_t num_plays = 0;
    chunk_len = 8;
    uint8_t actl_type[4] = {'a', 'c', 'T', 'L'};
    fwrite(&chunk_len, 4, 1, f);
    fwrite(actl_type, 4, 1, f);
    fwrite(&num_frames, 4, 1, f);
    fwrite(&num_plays, 4, 1, f);
    fwrite(&crc, 4, 1, f);
    
    // Write fcTL chunk
    uint32_t seq_num = 0;
    uint32_t f_width = 1, f_height = 1;
    uint32_t x_offset = 0, y_offset = 0;
    uint16_t delay_num = 10, delay_den = 100;
    uint8_t dispose_op = 0, blend_op = 0;
    chunk_len = 26;
    uint8_t fctl_type[4] = {'f', 'c', 'T', 'L'};
    fwrite(&chunk_len, 4, 1, f);
    fwrite(fctl_type, 4, 1, f);
    fwrite(&seq_num, 4, 1, f);
    fwrite(&f_width, 4, 1, f);
    fwrite(&f_height, 4, 1, f);
    fwrite(&x_offset, 4, 1, f);
    fwrite(&y_offset, 4, 1, f);
    fwrite(&delay_num, 2, 1, f);
    fwrite(&delay_den, 2, 1, f);
    fwrite(&dispose_op, 1, 1, f);
    fwrite(&blend_op, 1, 1, f);
    fwrite(&crc, 4, 1, f);
    
    // Write IDAT chunk (minimal)
    chunk_len = size > 0 ? size : 1;
    uint8_t idat_type[4] = {'I', 'D', 'A', 'T'};
    fwrite(&chunk_len, 4, 1, f);
    fwrite(idat_type, 4, 1, f);
    if (size > 0) {
        fwrite(data, 1, size, f);
    } else {
        uint8_t dummy = 0;
        fwrite(&dummy, 1, 1, f);
    }
    fwrite(&crc, 4, 1, f);
    
    // Write IEND chunk
    chunk_len = 0;
    uint8_t iend_type[4] = {'I', 'E', 'N', 'D'};
    fwrite(&chunk_len, 4, 1, f);
    fwrite(iend_type, 4, 1, f);
    fwrite(&crc, 4, 1, f);
    
    fclose(f);
}

extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    const char *filename = "./dummy_file";
    
    // Create a dummy PNG file
    write_dummy_png(filename, Data, Size);
    
    // Test reading
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);
    
    // Test png_get_next_frame_x_offset
    png_uint_32 x_offset = png_get_next_frame_x_offset(png_ptr, info_ptr);
    (void)x_offset;
    
    // Test png_get_next_frame_height
    png_uint_32 height = png_get_next_frame_height(png_ptr, info_ptr);
    (void)height;
    
    // Test png_get_next_frame_fcTL
    png_uint_32 w, h, xo, yo;
    png_uint_16 dn, dd;
    png_byte do_op, bo_op;
    png_get_next_frame_fcTL(png_ptr, info_ptr, &w, &h, &xo, &yo, &dn, &dd, &do_op, &bo_op);
    
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    // Test writing
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;
    
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }
    
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    fp = fopen(filename, "wb");
    if (!fp) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    png_init_io(png_ptr, fp);
    
    // Set basic image info
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    
    // Test png_set_first_frame_is_hidden
    png_set_first_frame_is_hidden(png_ptr, info_ptr, 1);
    png_set_first_frame_is_hidden(png_ptr, info_ptr, 0);
    
    // Test png_set_next_frame_fcTL
    png_set_next_frame_fcTL(png_ptr, info_ptr, 1, 1, 0, 0, 10, 100, 0, 0);
    
    // Test png_write_frame_head
    png_byte **row_pointers = NULL;
    png_write_frame_head(png_ptr, info_ptr, row_pointers, 1, 1, 0, 0, 10, 100, 0, 0);
    
    png_write_end(png_ptr, info_ptr);
    fclose(fp);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}