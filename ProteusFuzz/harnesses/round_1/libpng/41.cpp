// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_cHRM_XYZ at pngset.c:115:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_XYZ at pngget.c:559:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_sRGB at pngget.c:699:1 in png.h
// png_get_cLLI at pngget.c:808:1 in png.h
// png_get_cLLI_fixed at pngget.c:788:1 in png.h
// png_get_mDCV at pngget.c:860:1 in png.h
// png_get_mDCV_fixed at pngget.c:829:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
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
#include <cstring>
#include <fstream>
#include <vector>
#include <cmath>

static void write_dummy_png(const uint8_t *data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) return;
    if (size > 0) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
    file.close();
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Write data to dummy file
    write_dummy_png(Data, Size);

    // Open file for reading
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // Fuzz png_set_mDCV (double version)
    double white_x = (Data[0] / 255.0) * 1.3;
    double white_y = (Data[1] / 255.0) * 1.3;
    double red_x = (Data[2] / 255.0) * 1.3;
    double red_y = (Data[3] / 255.0) * 1.3;
    double green_x = (Data[4] / 255.0) * 1.3;
    double green_y = (Data[5] / 255.0) * 1.3;
    double blue_x = (Data[6] / 255.0) * 1.3;
    double blue_y = (Data[7] / 255.0) * 1.3;
    double max_lum = (Data[8] / 255.0) * 10000.0;
    double min_lum = (Data[9] / 255.0) * 10000.0;

    png_set_mDCV(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y, max_lum, min_lum);

    // Fuzz png_set_mDCV_fixed
    png_fixed_point int_white_x = (png_fixed_point)(white_x * 100000);
    png_fixed_point int_white_y = (png_fixed_point)(white_y * 100000);
    png_fixed_point int_red_x = (png_fixed_point)(red_x * 100000);
    png_fixed_point int_red_y = (png_fixed_point)(red_y * 100000);
    png_fixed_point int_green_x = (png_fixed_point)(green_x * 100000);
    png_fixed_point int_green_y = (png_fixed_point)(green_y * 100000);
    png_fixed_point int_blue_x = (png_fixed_point)(blue_x * 100000);
    png_fixed_point int_blue_y = (png_fixed_point)(blue_y * 100000);
    png_uint_32 max_lum_scaled = (png_uint_32)(max_lum * 10000);
    png_uint_32 min_lum_scaled = (png_uint_32)(min_lum * 10000);

    png_set_mDCV_fixed(png_ptr, info_ptr, int_white_x, int_white_y, int_red_x, int_red_y, int_green_x, int_green_y, int_blue_x, int_blue_y, max_lum_scaled, min_lum_scaled);

    // Fuzz png_set_cHRM
    png_set_cHRM(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y);

    // Fuzz png_set_cHRM_fixed
    png_set_cHRM_fixed(png_ptr, info_ptr, int_white_x, int_white_y, int_red_x, int_red_y, int_green_x, int_green_y, int_blue_x, int_blue_y);

    // Fuzz png_set_cHRM_XYZ
    double red_X = (Data[10] / 255.0) * 1.0;
    double red_Y = (Data[11] / 255.0) * 1.0;
    double red_Z = (Data[12] / 255.0) * 1.0;
    double green_X = (Data[13] / 255.0) * 1.0;
    double green_Y = (Data[14] / 255.0) * 1.0;
    double green_Z = (Data[15] / 255.0) * 1.0;
    double blue_X = (Data[16] / 255.0) * 1.0;
    double blue_Y = (Data[17] / 255.0) * 1.0;
    double blue_Z = (Data[18] / 255.0) * 1.0;

    png_set_cHRM_XYZ(png_ptr, info_ptr, red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z);

    // Fuzz png_set_cHRM_XYZ_fixed
    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr,
        (png_fixed_point)(red_X * 100000), (png_fixed_point)(red_Y * 100000), (png_fixed_point)(red_Z * 100000),
        (png_fixed_point)(green_X * 100000), (png_fixed_point)(green_Y * 100000), (png_fixed_point)(green_Z * 100000),
        (png_fixed_point)(blue_X * 100000), (png_fixed_point)(blue_Y * 100000), (png_fixed_point)(blue_Z * 100000));

    // Fuzz png_get_cHRM
    double g_white_x, g_white_y, g_red_x, g_red_y, g_green_x, g_green_y, g_blue_x, g_blue_y;
    png_get_cHRM(png_ptr, info_ptr, &g_white_x, &g_white_y, &g_red_x, &g_red_y, &g_green_x, &g_green_y, &g_blue_x, &g_blue_y);

    // Fuzz png_get_cHRM_fixed
    png_fixed_point g_int_white_x, g_int_white_y, g_int_red_x, g_int_red_y, g_int_green_x, g_int_green_y, g_int_blue_x, g_int_blue_y;
    png_get_cHRM_fixed(png_ptr, info_ptr, &g_int_white_x, &g_int_white_y, &g_int_red_x, &g_int_red_y, &g_int_green_x, &g_int_green_y, &g_int_blue_x, &g_int_blue_y);

    // Fuzz png_get_cHRM_XYZ
    double g_red_X, g_red_Y, g_red_Z, g_green_X, g_green_Y, g_green_Z, g_blue_X, g_blue_Y, g_blue_Z;
    png_get_cHRM_XYZ(png_ptr, info_ptr, &g_red_X, &g_red_Y, &g_red_Z, &g_green_X, &g_green_Y, &g_green_Z, &g_blue_X, &g_blue_Y, &g_blue_Z);

    // Fuzz png_get_cHRM_XYZ_fixed
    png_fixed_point g_int_red_X, g_int_red_Y, g_int_red_Z, g_int_green_X, g_int_green_Y, g_int_green_Z, g_int_blue_X, g_int_blue_Y, g_int_blue_Z;
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &g_int_red_X, &g_int_red_Y, &g_int_red_Z, &g_int_green_X, &g_int_green_Y, &g_int_green_Z, &g_int_blue_X, &g_int_blue_Y, &g_int_blue_Z);

    // Fuzz png_get_sRGB
    int srgb_intent;
    png_get_sRGB(png_ptr, info_ptr, &srgb_intent);

    // Fuzz png_get_cLLI
    double max_content, max_frame_avg;
    png_get_cLLI(png_ptr, info_ptr, &max_content, &max_frame_avg);

    // Fuzz png_get_cLLI_fixed
    png_uint_32 max_content_fixed, max_frame_avg_fixed;
    png_get_cLLI_fixed(png_ptr, info_ptr, &max_content_fixed, &max_frame_avg_fixed);

    // Fuzz png_get_mDCV
    double m_white_x, m_white_y, m_red_x, m_red_y, m_green_x, m_green_y, m_blue_x, m_blue_y, m_max_lum, m_min_lum;
    png_get_mDCV(png_ptr, info_ptr, &m_white_x, &m_white_y, &m_red_x, &m_red_y, &m_green_x, &m_green_y, &m_blue_x, &m_blue_y, &m_max_lum, &m_min_lum);

    // Fuzz png_get_mDCV_fixed
    png_fixed_point m_int_white_x, m_int_white_y, m_int_red_x, m_int_red_y, m_int_green_x, m_int_green_y, m_int_blue_x, m_int_blue_y;
    png_uint_32 m_max_lum_scaled, m_min_lum_scaled;
    png_get_mDCV_fixed(png_ptr, info_ptr, &m_int_white_x, &m_int_white_y, &m_int_red_x, &m_int_red_y, &m_int_green_x, &m_int_green_y, &m_int_blue_x, &m_int_blue_y, &m_max_lum_scaled, &m_min_lum_scaled);

    // Fuzz png_get_oFFs
    png_int_32 offset_x, offset_y;
    int unit_type;
    png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, &unit_type);

    // Fuzz png_get_cICP
    png_byte colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag;
    png_get_cICP(png_ptr, info_ptr, &colour_primaries, &transfer_function, &matrix_coefficients, &video_full_range_flag);

    // Cleanup
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}