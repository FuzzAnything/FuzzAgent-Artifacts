// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

static void write_dummy_png(const uint8_t *data, size_t size, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    // Write PNG signature
    const unsigned char png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(png_sig, 1, 8, f);
    
    // Write IHDR chunk (minimal valid header)
    // Length: 13 bytes
    unsigned char length[4] = {0, 0, 0, 13};
    fwrite(length, 1, 4, f);
    // Type: IHDR
    const char *type = "IHDR";
    fwrite(type, 1, 4, f);
    
    // Data: Width (4), Height (4), Bit depth (1), Color type (1), Compression (1), Filter (1), Interlace (1)
    unsigned char width[4] = {0, 0, 0, 1}; // 1 pixel width
    unsigned char height[4] = {0, 0, 0, 1}; // 1 pixel height
    unsigned char bit_depth = 8;
    unsigned char color_type = 2; // RGB
    unsigned char compression = 0;
    unsigned char filter = 0;
    unsigned char interlace = 0;
    
    fwrite(width, 1, 4, f);
    fwrite(height, 1, 4, f);
    fwrite(&bit_depth, 1, 1, f);
    fwrite(&color_type, 1, 1, f);
    fwrite(&compression, 1, 1, f);
    fwrite(&filter, 1, 1, f);
    fwrite(&interlace, 1, 1, f);
    
    // CRC (dummy)
    unsigned char crc[4] = {0, 0, 0, 0};
    fwrite(crc, 1, 4, f);
    
    // Write additional data from fuzz input as a custom chunk or IDAT
    if (size > 0) {
        // Write IDAT chunk
        unsigned char idat_len[4];
        if (size <= 0xFFFFFFFF) {
            idat_len[0] = (size >> 24) & 0xFF;
            idat_len[1] = (size >> 16) & 0xFF;
            idat_len[2] = (size >> 8) & 0xFF;
            idat_len[3] = size & 0xFF;
        } else {
            idat_len[0] = 0xFF;
            idat_len[1] = 0xFF;
            idat_len[2] = 0xFF;
            idat_len[3] = 0xFF;
        }
        fwrite(idat_len, 1, 4, f);
        const char *idat_type = "IDAT";
        fwrite(idat_type, 1, 4, f);
        fwrite(data, 1, size, f);
        // Dummy CRC
        fwrite(crc, 1, 4, f);
    }
    
    // Write IEND chunk
    unsigned char iend_len[4] = {0, 0, 0, 0};
    fwrite(iend_len, 1, 4, f);
    const char *iend_type = "IEND";
    fwrite(iend_type, 1, 4, f);
    fwrite(crc, 1, 4, f);
    
    fclose(f);
}

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    const char *filename = "./dummy_file";
    write_dummy_png(Data, Size, filename);
    
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }
    
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
    
    // Read the PNG header and info
    png_read_info(png_ptr, info_ptr);
    
    // Test png_get_image_width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    (void)width; // Use the value to avoid unused warning
    
    // Test png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)x_ppm;
    
    // Test png_get_hIST
    png_uint_16 *hist = NULL;
    png_uint_32 hist_result = png_get_hIST(png_ptr, info_ptr, &hist);
    if (hist_result && hist) {
        // hist is valid, but we don't need to do anything with it for fuzzing
    }
    
    // Test png_get_tIME
    png_timep mod_time = NULL;
    png_uint_32 time_result = png_get_tIME(png_ptr, info_ptr, &mod_time);
    if (time_result && mod_time) {
        // mod_time is valid
    }
    
    // Test png_get_gAMA_fixed
    png_fixed_point gamma = 0;
    png_uint_32 gamma_result = png_get_gAMA_fixed(png_ptr, info_ptr, &gamma);
    (void)gamma_result;
    
    // Test png_get_uint_16 with fuzz data
    if (Size >= 2) {
        png_uint_16 val = png_get_uint_16(Data);
        (void)val;
    }
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    remove(filename);
    
    return 0;
}