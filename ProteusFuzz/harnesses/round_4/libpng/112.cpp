// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_shift at pngtrans.c:83:1 in png.h
// png_set_sBIT at pngset.c:804:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_sBIT at pngget.c:1140:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
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
#include <cstring>
#include <cstdio>

static void png_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void png_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void setup_png_structs(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr, const uint8_t* data, size_t size) {
    if (!png_ptr_ptr || !info_ptr_ptr) return;

    *png_ptr_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, png_error_handler, png_warning_handler);
    if (!*png_ptr_ptr) return;

    *info_ptr_ptr = png_create_info_struct(*png_ptr_ptr);
    if (!*info_ptr_ptr) {
        png_destroy_read_struct(png_ptr_ptr, NULL, NULL);
        *png_ptr_ptr = NULL;
        return;
    }

    if (setjmp(png_jmpbuf(*png_ptr_ptr)) != 0) {
        png_destroy_read_struct(png_ptr_ptr, info_ptr_ptr, NULL);
        *png_ptr_ptr = NULL;
        *info_ptr_ptr = NULL;
        return;
    }

    // Create a dummy file to read from
    FILE* fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    } else {
        png_destroy_read_struct(png_ptr_ptr, info_ptr_ptr, NULL);
        *png_ptr_ptr = NULL;
        *info_ptr_ptr = NULL;
        return;
    }

    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(png_ptr_ptr, info_ptr_ptr, NULL);
        *png_ptr_ptr = NULL;
        *info_ptr_ptr = NULL;
        return;
    }

    png_init_io(*png_ptr_ptr, fp);
    fclose(fp);
}

extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    setup_png_structs(&png_ptr, &info_ptr, Data, Size);

    if (!png_ptr || !info_ptr) {
        return 0;
    }

    // Read the IHDR chunk to initialize info
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_read_info(png_ptr, info_ptr);

    // Fuzz png_get_sBIT
    png_color_8 sig_bit;
    memset(&sig_bit, 0, sizeof(sig_bit));
    png_color_8p sig_bit_ptr = &sig_bit;
    png_uint_32 sbit_result = png_get_sBIT(png_ptr, info_ptr, &sig_bit_ptr);
    (void)sbit_result;

    // Fuzz png_get_bit_depth
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    (void)bit_depth;

    // Fuzz png_set_shift
    png_color_8 true_bits;
    true_bits.red = (png_byte)(Data[0] % 9);
    true_bits.green = (png_byte)(Data[1] % 9);
    true_bits.blue = (png_byte)(Data[2] % 9);
    true_bits.gray = (png_byte)(Data[3] % 9);
    png_set_shift(png_ptr, &true_bits);

    // Fuzz png_set_sBIT
    png_color_8 sbit_data;
    sbit_data.red = (png_byte)(Data[4] % 9);
    sbit_data.green = (png_byte)(Data[5] % 9);
    sbit_data.blue = (png_byte)(Data[6] % 9);
    sbit_data.gray = (png_byte)(Data[7] % 9);
    png_set_sBIT(png_ptr, info_ptr, &sbit_data);

    // Fuzz png_get_channels
    png_byte channels = png_get_channels(png_ptr, info_ptr);
    (void)channels;

    // Fuzz png_read_png
    // We use a simple transform flag derived from input data
    int transforms = PNG_TRANSFORM_IDENTITY;
    if (Size > 8) {
        if (Data[8] & 0x01) transforms |= PNG_TRANSFORM_BGR;
        if (Data[8] & 0x02) transforms |= PNG_TRANSFORM_SWAP_ALPHA;
        if (Data[8] & 0x04) transforms |= PNG_TRANSFORM_INVERT_ALPHA;
    }

    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_read_png(png_ptr, info_ptr, transforms, NULL);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    // Remove dummy file
    remove("./dummy_file");

    return 0;
}