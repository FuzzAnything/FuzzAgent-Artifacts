// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_background_fixed at pngrtran.c:125:1 in png.h
// png_set_alpha_mode_fixed at pngrtran.c:344:1 in png.h
// png_set_invert_alpha at pngtrans.c:236:1 in png.h
// png_set_alpha_mode_fixed at pngrtran.c:344:1 in png.h
// png_set_gamma_fixed at pngrtran.c:859:1 in png.h
// png_set_background_fixed at pngrtran.c:125:1 in png.h
// png_set_scale_16 at pngrtran.c:171:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <setjmp.h>

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    }
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    FuzzedDataProvider data_provider(Data, Size);

    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

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
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    fp = fopen(dummy_file, "rb");
    if (!fp) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);
    fclose(fp);

    png_color_16 background_color;
    background_color.index = data_provider.ConsumeIntegralInRange(0, 255);
    background_color.red = data_provider.ConsumeIntegralInRange(0, 65535);
    background_color.green = data_provider.ConsumeIntegralInRange(0, 65535);
    background_color.blue = data_provider.ConsumeIntegralInRange(0, 65535);
    background_color.gray = data_provider.ConsumeIntegralInRange(0, 65535);

    int background_gamma_code = data_provider.ConsumeIntegralInRange(0, 3);
    int need_expand = data_provider.ConsumeBool() ? 1 : 0;
    png_fixed_point background_gamma = data_provider.ConsumeIntegralInRange(10000, 300000);
    
    // Fuzz png_set_background_fixed
    png_set_background_fixed(png_ptr, &background_color, background_gamma_code, need_expand, background_gamma);

    // Fuzz png_set_alpha_mode_fixed
    int mode = data_provider.ConsumeIntegralInRange(0, 3);
    png_fixed_point output_gamma = data_provider.ConsumeIntegralInRange(10000, 300000);
    png_set_alpha_mode_fixed(png_ptr, mode, output_gamma);

    // Fuzz png_set_invert_alpha
    png_set_invert_alpha(png_ptr);

    // Fuzz png_set_alpha_mode_fixed again with different values (simulating png_set_alpha_mode logic)
    // Since ConsumeDoubleInRange is not available, we generate fixed point directly
    png_fixed_point fixed_output_gamma = data_provider.ConsumeIntegralInRange(10000, 300000);
    png_set_alpha_mode_fixed(png_ptr, mode, fixed_output_gamma);

    // Fuzz png_set_gamma_fixed
    png_fixed_point screen_gamma = data_provider.ConsumeIntegralInRange(10000, 300000);
    png_fixed_point override_file_gamma = data_provider.ConsumeIntegralInRange(10000, 300000);
    png_set_gamma_fixed(png_ptr, screen_gamma, override_file_gamma);

    // Fuzz png_set_background_fixed again (simulating png_set_background logic)
    png_fixed_point fixed_background_gamma = data_provider.ConsumeIntegralInRange(10000, 300000);
    png_set_background_fixed(png_ptr, &background_color, background_gamma_code, need_expand, fixed_background_gamma);

    // Fuzz png_set_scale_16
    png_set_scale_16(png_ptr);

    // Attempt to read the rest of the image
    png_read_end(png_ptr, NULL);

    cleanup_png(png_ptr, info_ptr);

    return 0;
}