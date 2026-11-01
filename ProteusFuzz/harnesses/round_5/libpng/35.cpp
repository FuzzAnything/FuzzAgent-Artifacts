// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_mDCV_fixed at pngset.c:225:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_pHYs at pngset.c:723:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
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

// Helper to create a minimal PNG file for reading operations
static bool create_dummy_png(const char* filename, const uint8_t* data, size_t size) {
    FILE* f = fopen(filename, "wb");
    if (!f) return false;

    // Minimal PNG signature
    const uint8_t signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(signature, 1, 8, f);

    // IHDR chunk (minimal valid header)
    uint32_t width = 1, height = 1, bit_depth = 8, color_type = 2, compression = 0, filter = 0, interlace = 0;
    uint32_t ihdr_len = 13;
    
    fwrite(&ihdr_len, 4, 1, f);
    fwrite("IHDR", 4, 1, f);
    fwrite(&width, 4, 1, f);
    fwrite(&height, 4, 1, f);
    fwrite(&bit_depth, 1, 1, f);
    fwrite(&color_type, 1, 1, f);
    fwrite(&compression, 1, 1, f);
    fwrite(&filter, 1, 1, f);
    fwrite(&interlace, 1, 1, f);
    
    // CRC for IHDR (dummy)
    uint32_t crc = 0;
    fwrite(&crc, 4, 1, f);

    // IDAT chunk (dummy compressed data)
    uint32_t idat_len = size > 0 ? size : 1;
    fwrite(&idat_len, 4, 1, f);
    fwrite("IDAT", 4, 1, f);
    if (size > 0) {
        fwrite(data, 1, size, f);
    } else {
        uint8_t dummy = 0;
        fwrite(&dummy, 1, 1, f);
    }
    fwrite(&crc, 4, 1, f);

    // IEND chunk
    uint32_t iend_len = 0;
    fwrite(&iend_len, 4, 1, f);
    fwrite("IEND", 4, 1, f);
    fwrite(&crc, 4, 1, f);

    fclose(f);
    return true;
}

// Custom error handlers to prevent aborts during fuzzing
static void fuzzer_error_handler(png_structp png_ptr, const char* error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do nothing, let the fuzzer continue
}

static void fuzzer_warning_handler(png_structp png_ptr, const char* warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Do nothing
}

// Wrapper to safely call png_set_cHRM_XYZ_fixed
static void safe_png_set_cHRM_XYZ_fixed(png_structp png_ptr, png_infop info_ptr,
    png_fixed_point int_red_X, png_fixed_point int_red_Y, png_fixed_point int_red_Z,
    png_fixed_point int_green_X, png_fixed_point int_green_Y, png_fixed_point int_green_Z,
    png_fixed_point int_blue_X, png_fixed_point int_blue_Y, png_fixed_point int_blue_Z) {
    if (!png_ptr || !info_ptr) return;
    
    // Validate inputs to avoid invalid cHRM XYZ errors
    // cHRM values must be positive and within reasonable ranges
    if (int_red_X <= 0 || int_red_Y <= 0 || int_red_Z <= 0) return;
    if (int_green_X <= 0 || int_green_Y <= 0 || int_green_Z <= 0) return;
    if (int_blue_X <= 0 || int_blue_Y <= 0 || int_blue_Z <= 0) return;
    
    // Check for reasonable ranges (typical chromaticity values are between 0 and 1)
    // PNG fixed point is scaled by 100000, so 1.0 = 100000
    if (int_red_X > 100000 || int_red_Y > 100000 || int_red_Z > 100000) return;
    if (int_green_X > 100000 || int_green_Y > 100000 || int_green_Z > 100000) return;
    if (int_blue_X > 100000 || int_blue_Y > 100000 || int_blue_Z > 100000) return;

    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr,
        int_red_X, int_red_Y, int_red_Z,
        int_green_X, int_green_Y, int_green_Z,
        int_blue_X, int_blue_Y, int_blue_Z);
}

// Wrapper to safely call png_set_mDCV_fixed
static void safe_png_set_mDCV_fixed(png_structp png_ptr, png_infop info_ptr,
    png_fixed_point int_white_x, png_fixed_point int_white_y,
    png_fixed_point int_red_x, png_fixed_point int_red_y,
    png_fixed_point int_green_x, png_fixed_point int_green_y,
    png_fixed_point int_blue_x, png_fixed_point int_blue_y,
    png_uint_32 mastering_display_maximum_luminance_scaled_by_10000,
    png_uint_32 mastering_display_minimum_luminance_scaled_by_10000) {
    if (!png_ptr || !info_ptr) return;

    // Validate chromaticity values
    if (int_white_x <= 0 || int_white_y <= 0) return;
    if (int_red_x <= 0 || int_red_y <= 0) return;
    if (int_green_x <= 0 || int_green_y <= 0) return;
    if (int_blue_x <= 0 || int_blue_y <= 0) return;

    // Check for reasonable ranges
    if (int_white_x > 100000 || int_white_y > 100000) return;
    if (int_red_x > 100000 || int_red_y > 100000) return;
    if (int_green_x > 100000 || int_green_y > 100000) return;
    if (int_blue_x > 100000 || int_blue_y > 100000) return;

    // Validate luminance values (must be positive and MSB zero for 31-bit unsigned)
    if (mastering_display_maximum_luminance_scaled_by_10000 == 0) return;
    if (mastering_display_minimum_luminance_scaled_by_10000 == 0) return;
    if (mastering_display_maximum_luminance_scaled_by_10000 & 0x80000000) return;
    if (mastering_display_minimum_luminance_scaled_by_10000 & 0x80000000) return;

    png_set_mDCV_fixed(png_ptr, info_ptr,
        int_white_x, int_white_y,
        int_red_x, int_red_y,
        int_green_x, int_green_y,
        int_blue_x, int_blue_y,
        mastering_display_maximum_luminance_scaled_by_10000,
        mastering_display_minimum_luminance_scaled_by_10000);
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a dummy PNG file for read operations
    const char* dummy_file = "./dummy_file";
    if (!create_dummy_png(dummy_file, Data, Size)) {
        return 0;
    }

    // Initialize PNG read structure
    png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, fuzzer_error_handler, fuzzer_warning_handler);
    if (!read_ptr) return 0;

    png_infop read_info_ptr = png_create_info_struct(read_ptr);
    if (!read_info_ptr) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        return 0;
    }

    // Initialize PNG write structure
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, fuzzer_error_handler, fuzzer_warning_handler);
    if (!write_ptr) {
        png_destroy_read_struct(&read_ptr, &read_info_ptr, nullptr);
        return 0;
    }

    png_infop write_info_ptr = png_create_info_struct(write_ptr);
    if (!write_info_ptr) {
        png_destroy_write_struct(&write_ptr, nullptr);
        png_destroy_read_struct(&read_ptr, &read_info_ptr, nullptr);
        return 0;
    }

    // Set up read file
    FILE* read_fp = fopen(dummy_file, "rb");
    if (!read_fp) {
        png_destroy_write_struct(&write_ptr, &write_info_ptr);
        png_destroy_read_struct(&read_ptr, &read_info_ptr, nullptr);
        return 0;
    }

    png_init_io(read_ptr, read_fp);

    // Set up write file (dummy)
    FILE* write_fp = fopen(dummy_file, "wb");
    if (!write_fp) {
        fclose(read_fp);
        png_destroy_write_struct(&write_ptr, &write_info_ptr);
        png_destroy_read_struct(&read_ptr, &read_info_ptr, nullptr);
        return 0;
    }

    png_init_io(write_ptr, write_fp);

    // Extract values from input data for fuzzing
    png_fixed_point red_X = (Size > 0) ? (png_fixed_point)Data[0] : 0;
    png_fixed_point red_Y = (Size > 1) ? (png_fixed_point)Data[1] : 0;
    png_fixed_point red_Z = (Size > 2) ? (png_fixed_point)Data[2] : 0;
    png_fixed_point green_X = (Size > 3) ? (png_fixed_point)Data[3] : 0;
    png_fixed_point green_Y = (Size > 4) ? (png_fixed_point)Data[4] : 0;
    png_fixed_point green_Z = (Size > 5) ? (png_fixed_point)Data[5] : 0;
    png_fixed_point blue_X = (Size > 6) ? (png_fixed_point)Data[6] : 0;
    png_fixed_point blue_Y = (Size > 7) ? (png_fixed_point)Data[7] : 0;
    png_fixed_point blue_Z = (Size > 8) ? (png_fixed_point)Data[8] : 0;

    png_fixed_point white_x = (Size > 9) ? (png_fixed_point)Data[9] : 0;
    png_fixed_point white_y = (Size > 10) ? (png_fixed_point)Data[10] : 0;

    png_uint_32 max_lum = (Size > 11) ? (png_uint_32)Data[11] : 0;
    png_uint_32 min_lum = (Size > 12) ? (png_uint_32)Data[12] : 0;

    png_uint_32 res_x = (Size > 13) ? (png_uint_32)Data[13] : 0;
    png_uint_32 res_y = (Size > 14) ? (png_uint_32)Data[14] : 0;
    int unit_type = (Size > 15) ? (int)Data[15] : 0;

    // Fuzz png_set_cHRM_XYZ_fixed with safe wrapper
    safe_png_set_cHRM_XYZ_fixed(write_ptr, write_info_ptr,
        red_X, red_Y, red_Z,
        green_X, green_Y, green_Z,
        blue_X, blue_Y, blue_Z);

    // Fuzz png_set_mDCV_fixed with safe wrapper
    safe_png_set_mDCV_fixed(write_ptr, write_info_ptr,
        white_x, white_y,
        red_X, red_Y,
        green_X, green_Y,
        blue_X, blue_Y,
        max_lum, min_lum);

    // Fuzz png_set_pHYs
    png_set_pHYs(write_ptr, write_info_ptr, res_x, res_y, unit_type);

    // Fuzz png_get_cHRM_fixed
    png_fixed_point out_white_x, out_white_y, out_red_x, out_red_y, out_green_x, out_green_y, out_blue_x, out_blue_y;
    png_get_cHRM_fixed(read_ptr, read_info_ptr,
        &out_white_x, &out_white_y,
        &out_red_x, &out_red_y,
        &out_green_x, &out_green_y,
        &out_blue_x, &out_blue_y);

    // Fuzz png_get_cHRM_XYZ_fixed
    png_fixed_point out_red_X, out_red_Y, out_red_Z, out_green_X, out_green_Y, out_green_Z, out_blue_X, out_blue_Y, out_blue_Z;
    png_get_cHRM_XYZ_fixed(read_ptr, read_info_ptr,
        &out_red_X, &out_red_Y, &out_red_Z,
        &out_green_X, &out_green_Y, &out_green_Z,
        &out_blue_X, &out_blue_Y, &out_blue_Z);

    // Fuzz png_get_sCAL_fixed
    int out_unit;
    png_fixed_point out_width, out_height;
    png_get_sCAL_fixed(read_ptr, read_info_ptr, &out_unit, &out_width, &out_height);

    // Fuzz png_get_cICP (if supported)
    #ifdef PNG_cICP_SUPPORTED
    png_byte colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag;
    png_get_cICP(read_ptr, read_info_ptr,
        &colour_primaries, &transfer_function,
        &matrix_coefficients, &video_full_range_flag);
    #endif

    // Fuzz png_get_oFFs (if supported)
    #ifdef PNG_oFFs_SUPPORTED
    png_int_32 offset_x, offset_y;
    int unit;
    png_get_oFFs(read_ptr, read_info_ptr, &offset_x, &offset_y, &unit);
    #endif

    // Cleanup
    fclose(read_fp);
    fclose(write_fp);

    png_destroy_write_struct(&write_ptr, &write_info_ptr);
    png_destroy_read_struct(&read_ptr, &read_info_ptr, nullptr);

    return 0;
}