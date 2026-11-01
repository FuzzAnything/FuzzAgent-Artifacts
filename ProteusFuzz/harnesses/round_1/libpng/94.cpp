// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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

static void fuzz_png_get_hIST(const uint8_t *Data, size_t Size) {
    if (Size < 8) return;

    std::vector<uint8_t> buffer(Data, Data + Size);
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(buffer.data(), 1, buffer.size(), fp);
    fclose(fp);

    fp = fopen("./dummy_file", "rb");
    if (!fp) return;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    png_uint_16 *hist = NULL;
    png_uint_32 ret = png_get_hIST(png_ptr, info_ptr, &hist);
    (void)ret;
    (void)hist;

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
}

static void fuzz_png_get_tIME(const uint8_t *Data, size_t Size) {
    if (Size < 8) return;

    std::vector<uint8_t> buffer(Data, Data + Size);
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(buffer.data(), 1, buffer.size(), fp);
    fclose(fp);

    fp = fopen("./dummy_file", "rb");
    if (!fp) return;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    png_timep mod_time = NULL;
    png_uint_32 ret = png_get_tIME(png_ptr, info_ptr, &mod_time);
    (void)ret;
    (void)mod_time;

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
}

static void fuzz_png_get_gAMA_fixed(const uint8_t *Data, size_t Size) {
    if (Size < 8) return;

    std::vector<uint8_t> buffer(Data, Data + Size);
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(buffer.data(), 1, buffer.size(), fp);
    fclose(fp);

    fp = fopen("./dummy_file", "rb");
    if (!fp) return;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    png_fixed_point int_file_gamma = 0;
    png_uint_32 ret = png_get_gAMA_fixed(png_ptr, info_ptr, &int_file_gamma);
    (void)ret;
    (void)int_file_gamma;

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
}

static void fuzz_png_get_image_width(const uint8_t *Data, size_t Size) {
    if (Size < 8) return;

    std::vector<uint8_t> buffer(Data, Data + Size);
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(buffer.data(), 1, buffer.size(), fp);
    fclose(fp);

    fp = fopen("./dummy_file", "rb");
    if (!fp) return;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    (void)width;

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
}

static void fuzz_png_get_x_pixels_per_meter(const uint8_t *Data, size_t Size) {
    if (Size < 8) return;

    std::vector<uint8_t> buffer(Data, Data + Size);
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(buffer.data(), 1, buffer.size(), fp);
    fclose(fp);

    fp = fopen("./dummy_file", "rb");
    if (!fp) return;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)x_ppm;

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
}

static void fuzz_png_get_uint_16(const uint8_t *Data, size_t Size) {
    if (Size < 2) return;
    png_uint_16 val = png_get_uint_16(Data);
    (void)val;
}

extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz each target function
    fuzz_png_get_hIST(Data, Size);
    fuzz_png_get_tIME(Data, Size);
    fuzz_png_get_gAMA_fixed(Data, Size);
    fuzz_png_get_image_width(Data, Size);
    fuzz_png_get_x_pixels_per_meter(Data, Size);
    fuzz_png_get_uint_16(Data, Size);

    return 0;
}