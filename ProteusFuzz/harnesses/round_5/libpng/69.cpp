// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_sCAL at pngget.c:1049:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_sCAL_s at pngget.c:1067:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_mDCV_fixed at pngget.c:829:1 in png.h
// png_get_mDCV_fixed at pngget.c:829:1 in png.h
// png_get_cLLI at pngget.c:808:1 in png.h
// png_get_cLLI_fixed at pngget.c:788:1 in png.h
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
#include <stdlib.h>
#include <string.h>
#include <fuzzer/FuzzedDataProvider.h>

static void safe_png_destroy(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr && info_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    } else if (png_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
    } else if (info_ptr) {
        // If we only have info_ptr and no png_ptr, we cannot safely use png_destroy_read_struct
        // because it expects a valid png_struct. We must manually destroy the info struct.
        // However, png_destroy_info_struct requires a png_ptr (can be NULL though).
        png_destroy_info_struct(NULL, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    FuzzedDataProvider fdp(Data, Size);

    // Create a dummy file for reading
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        safe_png_destroy(png_ptr, info_ptr);
        return 0;
    }

    // Open the file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        safe_png_destroy(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Read the PNG file
    // We don't call png_read_png to avoid heavy processing, just read the header and chunks
    // to populate info_ptr for the target APIs.
    png_read_info(png_ptr, info_ptr);

    // --- Fuzzing Target APIs ---

    // 1. png_get_mDCV_fixed
    png_fixed_point int_white_x, int_white_y, int_red_x, int_red_y,
                    int_green_x, int_green_y, int_blue_x, int_blue_y;
    png_uint_32 mastering_display_maximum_luminance_scaled_by_10000,
                 mastering_display_minimum_luminance_scaled_by_10000;
    png_get_mDCV_fixed(png_ptr, info_ptr,
        &int_white_x, &int_white_y,
        &int_red_x, &int_red_y,
        &int_green_x, &int_green_y,
        &int_blue_x, &int_blue_y,
        &mastering_display_maximum_luminance_scaled_by_10000,
        &mastering_display_minimum_luminance_scaled_by_10000);

    // Test with null pointers (should be safe)
    png_get_mDCV_fixed(png_ptr, info_ptr, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    // 2. png_get_cLLI
    double maximum_content_light_level, maximum_frame_average_light_level;
    png_get_cLLI(png_ptr, info_ptr,
        &maximum_content_light_level,
        &maximum_frame_average_light_level);
    
    png_uint_32 clli_max, clli_avg;
    png_get_cLLI_fixed(png_ptr, info_ptr, &clli_max, &clli_avg);

    // 3. png_get_cHRM
    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    png_get_cHRM(png_ptr, info_ptr,
        &white_x, &white_y,
        &red_x, &red_y,
        &green_x, &green_y,
        &blue_x, &blue_y);

    png_fixed_point int_wx, int_wy, int_rx, int_ry, int_gx, int_gy, int_bx, int_by;
    png_get_cHRM_fixed(png_ptr, info_ptr,
        &int_wx, &int_wy,
        &int_rx, &int_ry,
        &int_gx, &int_gy,
        &int_bx, &int_by);

    // 4. png_get_sCAL
    int unit;
    double width, height;
    png_get_sCAL(png_ptr, info_ptr, &unit, &width, &height);
    
    png_fixed_point fwidth, fheight;
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &fwidth, &fheight);

    char *swidth = NULL, *sheight = NULL;
    png_get_sCAL_s(png_ptr, info_ptr, &unit, &swidth, &sheight);
    // Note: swidth and sheight are internal pointers, do not free them manually.

    // 5. png_get_IHDR
    png_uint_32 ihdr_width, ihdr_height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(png_ptr, info_ptr,
        &ihdr_width, &ihdr_height,
        &bit_depth, &color_type,
        &interlace_method, &compression_method, &filter_method);

    // Test with null pointers
    png_get_IHDR(png_ptr, info_ptr, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    // 6. png_get_oFFs (from the provided list in the prompt description, though not in the main numbered list, it's in the signature block)
    png_int_32 offset_x, offset_y;
    int unit_type;
    png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, &unit_type);

    // 7. png_get_cICP (from the provided list in the prompt description)
    png_byte colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag;
    png_get_cICP(png_ptr, info_ptr,
        &colour_primaries, &transfer_function,
        &matrix_coefficients, &video_full_range_flag);

    // Cleanup
    fclose(fp);
    safe_png_destroy(png_ptr, info_ptr);
    remove(filename);

    return 0;
}