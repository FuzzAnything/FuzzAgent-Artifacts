// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_longjmp at pngerror.c:663:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_eXIf_1 at pngget.c:892:1 in png.h
// png_get_pixels_per_meter at pngget.c:167:1 in png.h
// png_get_next_frame_x_offset at pngget.c:1449:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <png.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <setjmp.h>
#include <cstdint>

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    png_longjmp(png_ptr, 1);
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    fp = fopen(filename, "rb");
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

    png_set_error_fn(png_ptr, NULL, dummy_error_fn, dummy_warning_fn);

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        remove(filename);
        return 0;
    }

    png_init_io(png_ptr, fp);

    png_read_info(png_ptr, info_ptr);

    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    (void)width;

    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    (void)height;

    png_uint_32 num_exif = 0;
    png_byte *exif = NULL;
    png_uint_32 exif_flag = png_get_eXIf_1(png_ptr, info_ptr, &num_exif, &exif);
    (void)exif_flag;

    png_uint_32 ppm = png_get_pixels_per_meter(png_ptr, info_ptr);
    (void)ppm;

    png_uint_32 x_offset = png_get_next_frame_x_offset(png_ptr, info_ptr);
    (void)x_offset;

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    remove(filename);

    return 0;
}