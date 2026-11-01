// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_sBIT at pngget.c:1140:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_set_shift at pngtrans.c:83:1 in png.h
// png_set_sBIT at pngset.c:804:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
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
#include <fstream>

static void safe_png_cleanup(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *dummy_file = "./dummy_file";
    std::ofstream out(dummy_file, std::ios::binary);
    if (!out) {
        return 0;
    }
    out.write(reinterpret_cast<const char*>(Data), Size);
    out.close();

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        safe_png_cleanup(png_ptr, info_ptr);
        return 0;
    }

    FILE *fp = fopen(dummy_file, "rb");
    if (!fp) {
        safe_png_cleanup(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    png_read_info(png_ptr, info_ptr);

    // 1. png_get_sBIT
    png_color_8 sig_bit;
    memset(&sig_bit, 0, sizeof(sig_bit));
    png_color_8p sig_bit_ptr = &sig_bit;
    png_uint_32 sbit_result = png_get_sBIT(png_ptr, info_ptr, &sig_bit_ptr);
    (void)sbit_result;

    // 2. png_get_bit_depth
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    (void)bit_depth;

    // 3. png_set_shift
    png_color_8 shift_bits;
    shift_bits.red = (png_byte)(Data[0] % 16);
    shift_bits.green = (png_byte)(Data[1 % Size] % 16);
    shift_bits.blue = (png_byte)(Data[2 % Size] % 16);
    shift_bits.gray = (png_byte)(Data[3 % Size] % 16);
    shift_bits.alpha = (png_byte)(Data[4 % Size] % 16);
    
    png_set_shift(png_ptr, &shift_bits);

    // 4. png_set_sBIT
    png_color_8 set_sbit;
    set_sbit.red = (png_byte)(Data[5 % Size] % 16);
    set_sbit.green = (png_byte)(Data[6 % Size] % 16);
    set_sbit.blue = (png_byte)(Data[7 % Size] % 16);
    set_sbit.gray = (png_byte)(Data[8 % Size] % 16);
    set_sbit.alpha = (png_byte)(Data[9 % Size] % 16);

    png_set_sBIT(png_ptr, info_ptr, &set_sbit);

    // 5. png_get_channels
    png_byte channels = png_get_channels(png_ptr, info_ptr);
    (void)channels;

    // 6. png_read_png
    int transforms = (Data[10 % Size] % 1024);
    png_read_png(png_ptr, info_ptr, transforms, nullptr);

    fclose(fp);
    safe_png_cleanup(png_ptr, info_ptr);

    return 0;
}