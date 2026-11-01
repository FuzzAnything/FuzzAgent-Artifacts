// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_sRGB at pngget.c:699:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_get_cLLI at pngget.c:808:1 in png.h
// png_get_cLLI_fixed at pngget.c:788:1 in png.h
// png_get_mDCV at pngget.c:860:1 in png.h
// png_get_mDCV_fixed at pngget.c:829:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_mDCV at pngset.c:297:1 in png.h
// png_set_mDCV_fixed at pngset.c:225:1 in png.h
// png_set_cHRM at pngset.c:99:1 in png.h
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include <new>

static void fuzz_helper_set_mDCV(png_structp png_ptr, png_infop info_ptr, const uint8_t *Data, size_t Size) {
    if (Size < 80) return;

    double white_x = 0.0, white_y = 0.0;
    double red_x = 0.0, red_y = 0.0;
    double green_x = 0.0, green_y = 0.0;
    double blue_x = 0.0, blue_y = 0.0;
    double max_lum = 0.0, min_lum = 0.0;

    // Parse doubles from input (simple 8-byte chunks)
    if (Size >= 8) { memcpy(&white_x, Data, 8); Data += 8; Size -= 8; }
    if (Size >= 8) { memcpy(&white_y, Data, 8); Data += 8; Size -= 8; }
    if (Size >= 8) { memcpy(&red_x, Data, 8); Data += 8; Size -= 8; }
    if (Size >= 8) { memcpy(&red_y, Data, 8); Data += 8; Size -= 8; }
    if (Size >= 8) { memcpy(&green_x, Data, 8); Data += 8; Size -= 8; }
    if (Size >= 8) { memcpy(&green_y, Data, 8); Data += 8; Size -= 8; }
    if (Size >= 8) { memcpy(&blue_x, Data, 8); Data += 8; Size -= 8; }
    if (Size >= 8) { memcpy(&blue_y, Data, 8); Data += 8; Size -= 8; }
    if (Size >= 8) { memcpy(&max_lum, Data, 8); Data += 8; Size -= 8; }
    if (Size >= 8) { memcpy(&min_lum, Data, 8); Data += 8; Size -= 8; }

    png_set_mDCV(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y, max_lum, min_lum);

    // Also test fixed version if enough data
    if (Size >= 40) {
        png_fixed_point fx_white_x = 0, fx_white_y = 0;
        png_fixed_point fx_red_x = 0, fx_red_y = 0;
        png_fixed_point fx_green_x = 0, fx_green_y = 0;
        png_fixed_point fx_blue_x = 0, fx_blue_y = 0;
        png_uint_32 max_lum_fixed = 0, min_lum_fixed = 0;

        if (Size >= 4) { memcpy(&fx_white_x, Data, 4); Data += 4; Size -= 4; }
        if (Size >= 4) { memcpy(&fx_white_y, Data, 4); Data += 4; Size -= 4; }
        if (Size >= 4) { memcpy(&fx_red_x, Data, 4); Data += 4; Size -= 4; }
        if (Size >= 4) { memcpy(&fx_red_y, Data, 4); Data += 4; Size -= 4; }
        if (Size >= 4) { memcpy(&fx_green_x, Data, 4); Data += 4; Size -= 4; }
        if (Size >= 4) { memcpy(&fx_green_y, Data, 4); Data += 4; Size -= 4; }
        if (Size >= 4) { memcpy(&fx_blue_x, Data, 4); Data += 4; Size -= 4; }
        if (Size >= 4) { memcpy(&fx_blue_y, Data, 4); Data += 4; Size -= 4; }
        if (Size >= 4) { memcpy(&max_lum_fixed, Data, 4); Data += 4; Size -= 4; }
        if (Size >= 4) { memcpy(&min_lum_fixed, Data, 4); Data += 4; Size -= 4; }

        png_set_mDCV_fixed(png_ptr, info_ptr, fx_white_x, fx_white_y, fx_red_x, fx_red_y, fx_green_x, fx_green_y, fx_blue_x, fx_blue_y, max_lum_fixed, min_lum_fixed);
    }
}

static void fuzz_helper_set_cHRM(png_structp png_ptr, png_infop info_ptr, const uint8_t *Data, size_t Size) {
    if (Size < 64) return;

    double white_x = 0.0, white_y = 0.0;
    double red_x = 0.0, red_y = 0.0;
    double green_x = 0.0, green_y = 0.0;
    double blue_x = 0.0, blue_y = 0.0;

    if (Size >= 8) { memcpy(&white_x, Data, 8); Data += 8; Size -= 8; }
    if (Size >= 8) { memcpy(&white_y, Data, 8); Data += 8; Size -= 8; }
    if (Size >= 8) { memcpy(&red_x, Data, 8); Data += 8; Size -= 8; }
    if (Size >= 8) { memcpy(&red_y, Data, 8); Data += 8; Size -= 8; }
    if (Size >= 8) { memcpy(&green_x, Data, 8); Data += 8; Size -= 8; }
    if (Size >= 8) { memcpy(&green_y, Data, 8); Data += 8; Size -= 8; }
    if (Size >= 8) { memcpy(&blue_x, Data, 8); Data += 8; Size -= 8; }
    if (Size >= 8) { memcpy(&blue_y, Data, 8); Data += 8; Size -= 8; }

    png_set_cHRM(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y);

    // Test fixed version
    if (Size >= 32) {
        png_fixed_point fx_white_x = 0, fx_white_y = 0;
        png_fixed_point fx_red_x = 0, fx_red_y = 0;
        png_fixed_point fx_green_x = 0, fx_green_y = 0;
        png_fixed_point fx_blue_x = 0, fx_blue_y = 0;

        if (Size >= 4) { memcpy(&fx_white_x, Data, 4); Data += 4; Size -= 4; }
        if (Size >= 4) { memcpy(&fx_white_y, Data, 4); Data += 4; Size -= 4; }
        if (Size >= 4) { memcpy(&fx_red_x, Data, 4); Data += 4; Size -= 4; }
        if (Size >= 4) { memcpy(&fx_red_y, Data, 4); Data += 4; Size -= 4; }
        if (Size >= 4) { memcpy(&fx_green_x, Data, 4); Data += 4; Size -= 4; }
        if (Size >= 4) { memcpy(&fx_green_y, Data, 4); Data += 4; Size -= 4; }
        if (Size >= 4) { memcpy(&fx_blue_x, Data, 4); Data += 4; Size -= 4; }
        if (Size >= 4) { memcpy(&fx_blue_y, Data, 4); Data += 4; Size -= 4; }

        png_set_cHRM_fixed(png_ptr, info_ptr, fx_white_x, fx_white_y, fx_red_x, fx_red_y, fx_green_x, fx_green_y, fx_blue_x, fx_blue_y);
    }
}

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling to prevent crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Create a dummy file to simulate reading
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Open the dummy file for reading
    f = fopen(dummy_file, "rb");
    if (!f) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, f);

    // Attempt to read the header and info
    png_read_info(png_ptr, info_ptr);

    // Fuzz png_set_mDCV
    fuzz_helper_set_mDCV(png_ptr, info_ptr, Data, Size);

    // Fuzz png_set_cHRM
    fuzz_helper_set_cHRM(png_ptr, info_ptr, Data, Size);

    // Fuzz png_get_cHRM
    double gx_white_x = 0, gx_white_y = 0;
    double gx_red_x = 0, gx_red_y = 0;
    double gx_green_x = 0, gx_green_y = 0;
    double gx_blue_x = 0, gx_blue_y = 0;
    png_get_cHRM(png_ptr, info_ptr, &gx_white_x, &gx_white_y, &gx_red_x, &gx_red_y, &gx_green_x, &gx_green_y, &gx_blue_x, &gx_blue_y);

    // Fuzz png_get_cHRM_fixed
    png_fixed_point fx_white_x = 0, fx_white_y = 0;
    png_fixed_point fx_red_x = 0, fx_red_y = 0;
    png_fixed_point fx_green_x = 0, fx_green_y = 0;
    png_fixed_point fx_blue_x = 0, fx_blue_y = 0;
    png_get_cHRM_fixed(png_ptr, info_ptr, &fx_white_x, &fx_white_y, &fx_red_x, &fx_red_y, &fx_green_x, &fx_green_y, &fx_blue_x, &fx_blue_y);

    // Fuzz png_get_sRGB
    int srgb_intent = 0;
    png_get_sRGB(png_ptr, info_ptr, &srgb_intent);

    // Fuzz png_get_cICP
    png_byte cp, tf, mc, vfr;
    png_get_cICP(png_ptr, info_ptr, &cp, &tf, &mc, &vfr);

    // Fuzz png_get_cLLI
    double max_content_light = 0, max_frame_avg_light = 0;
    png_get_cLLI(png_ptr, info_ptr, &max_content_light, &max_frame_avg_light);

    // Fuzz png_get_cLLI_fixed
    png_uint_32 max_content_fixed = 0, max_frame_avg_fixed = 0;
    png_get_cLLI_fixed(png_ptr, info_ptr, &max_content_fixed, &max_frame_avg_fixed);

    // Fuzz png_get_mDCV
    double m_white_x = 0, m_white_y = 0;
    double m_red_x = 0, m_red_y = 0;
    double m_green_x = 0, m_green_y = 0;
    double m_blue_x = 0, m_blue_y = 0;
    double m_max_lum = 0, m_min_lum = 0;
    png_get_mDCV(png_ptr, info_ptr, &m_white_x, &m_white_y, &m_red_x, &m_red_y, &m_green_x, &m_green_y, &m_blue_x, &m_blue_y, &m_max_lum, &m_min_lum);

    // Fuzz png_get_mDCV_fixed
    png_fixed_point mf_white_x = 0, mf_white_y = 0;
    png_fixed_point mf_red_x = 0, mf_red_y = 0;
    png_fixed_point mf_green_x = 0, mf_green_y = 0;
    png_fixed_point mf_blue_x = 0, mf_blue_y = 0;
    png_uint_32 mf_max_lum = 0, mf_min_lum = 0;
    png_get_mDCV_fixed(png_ptr, info_ptr, &mf_white_x, &mf_white_y, &mf_red_x, &mf_red_y, &mf_green_x, &mf_green_y, &mf_blue_x, &mf_blue_y, &mf_max_lum, &mf_min_lum);

    // Fuzz png_get_oFFs
    png_int_32 offset_x = 0, offset_y = 0;
    int unit_type = 0;
    png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, &unit_type);

    // Cleanup
    fclose(f);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    remove(dummy_file);

    return 0;
}