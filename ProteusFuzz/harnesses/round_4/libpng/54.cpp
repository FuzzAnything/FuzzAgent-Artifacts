// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
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

static void write_dummy_png_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    // Write a minimal PNG signature
    unsigned char png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(png_sig, 1, 8, f);
    // Write the fuzzing data as a raw IHDR chunk followed by IEND
    // We construct a minimal IHDR chunk manually to ensure libpng can read the header
    if (size >= 13) {
        // Write IHDR chunk length (13 bytes)
        uint32_t length = 13;
        fwrite(&length, 4, 1, f);
        // Write IHDR type
        const char *type = "IHDR";
        fwrite(type, 1, 4, f);
        // Write IHDR data (width, height, bit depth, color type, compression, filter, interlace)
        // Use first 13 bytes of input data as the chunk data
        fwrite(data, 1, 13, f);
        // Write CRC (dummy)
        uint32_t crc = 0;
        fwrite(&crc, 4, 1, f);
    }
    // Write IEND chunk
    uint32_t iend_len = 0;
    fwrite(&iend_len, 4, 1, f);
    const char *iend_type = "IEND";
    fwrite(iend_type, 1, 4, f);
    uint32_t iend_crc = 0;
    fwrite(&iend_crc, 4, 1, f);
    
    // Write the rest of the fuzzing data as additional chunks (raw)
    // This is not valid PNG but tests robustness
    if (size > 13) {
        fwrite(data + 13, 1, size - 13, f);
    }
    fclose(f);
}

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    }
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size < 13) {
        return 0;
    }

    const char *filename = "./dummy_file";
    write_dummy_png_file(filename, Data, Size);

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        return 0;
    }

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

    // Read the PNG header and info
    png_read_info(png_ptr, info_ptr);

    // Fuzz png_get_gAMA_fixed
    png_fixed_point file_gamma = 0;
    png_uint_32 gama_ret = png_get_gAMA_fixed(png_ptr, info_ptr, &file_gamma);
    (void)gama_ret;

    // Fuzz png_get_image_width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    (void)width;

    // Fuzz png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)x_ppm;

    // Fuzz png_get_uint_16 (using input data as buffer)
    if (Size >= 2) {
        png_uint_16 val16 = png_get_uint_16(Data);
        (void)val16;
    }

    // Fuzz png_get_hIST
    png_uint_16 *hist = NULL;
    png_uint_32 hist_ret = png_get_hIST(png_ptr, info_ptr, &hist);
    (void)hist_ret;

    // Fuzz png_get_tIME
    // png_get_tIME expects a pointer to a png_timep (which is png_time_struct*)
    // So we need a png_time_struct variable and pass its address as a png_timep*
    png_time_struct mod_time_struct;
    png_timep mod_time_ptr = &mod_time_struct;
    png_uint_32 time_ret = png_get_tIME(png_ptr, info_ptr, &mod_time_ptr);
    (void)time_ret;

    // Cleanup
    cleanup_png(png_ptr, info_ptr);
    fclose(fp);
    return 0;
}