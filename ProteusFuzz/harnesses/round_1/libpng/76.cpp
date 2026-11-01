// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
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
// png_read_info at pngread.c:102:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
// png_get_sBIT at pngget.c:1140:1 in png.h
// png_set_sBIT at pngset.c:804:1 in png.h
// png_set_shift at pngtrans.c:83:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
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

static void
fuzz_read_png(const uint8_t *data, size_t size) {
    if (size == 0) return;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return;
    }

    // Write input data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return;
    }
    fwrite(data, 1, size, fp);
    fclose(fp);

    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return;
    }

    png_init_io(png_ptr, fp);

    // Read header info
    png_read_info(png_ptr, info_ptr);

    // Fuzz png_get_bit_depth
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    (void)bit_depth;

    // Fuzz png_get_channels
    png_byte channels = png_get_channels(png_ptr, info_ptr);
    (void)channels;

    // Fuzz png_get_sBIT
    // The function expects a pointer to a pointer (png_color_8p *sig_bit)
    png_color_8 sig_bit;
    png_color_8p sig_bit_ptr = &sig_bit;
    png_uint_32 sbit_flag = png_get_sBIT(png_ptr, info_ptr, &sig_bit_ptr);
    (void)sbit_flag;

    // Fuzz png_set_sBIT
    png_color_8 fuzz_sbit = { 8, 8, 8, 8 };
    if (size > 0) fuzz_sbit.red = data[0] % 17;
    if (size > 1) fuzz_sbit.green = data[1] % 17;
    if (size > 2) fuzz_sbit.blue = data[2] % 17;
    if (size > 3) fuzz_sbit.alpha = data[3] % 17;
    
    png_set_sBIT(png_ptr, info_ptr, &fuzz_sbit);

    // Fuzz png_set_shift
    png_color_8 fuzz_shift = { 8, 8, 8, 8 };
    if (size > 0) fuzz_shift.red = data[0] % 17;
    if (size > 1) fuzz_shift.green = data[1] % 17;
    if (size > 2) fuzz_shift.blue = data[2] % 17;
    if (size > 3) fuzz_shift.alpha = data[3] % 17;
    
    png_set_shift(png_ptr, &fuzz_shift);

    // Fuzz png_read_png with various transforms
    int transforms = PNG_TRANSFORM_IDENTITY;
    if (size > 4) transforms = data[4] & 0xFF;
    
    // Rewind to allow png_read_png to re-read if necessary or continue
    // Note: png_read_png typically expects the file to be at the start of image data
    // after read_info, but rewinding ensures we test the full path if the function
    // attempts to re-parse headers or if the state was reset.
    rewind(fp);
    
    // Re-read info to ensure consistency if we rewound, though read_png might handle it.
    // To be safe for the fuzzer, we'll just call read_png.
    // If the file pointer is at the start, read_png might try to read IHDR again.
    // Standard usage: read_info -> (transforms) -> read_png.
    // Since we rewound, we should re-read info to match the file position.
    png_read_info(png_ptr, info_ptr);

    png_read_png(png_ptr, info_ptr, transforms, NULL);

    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_read_png(Data, Size);

    return 0;
}