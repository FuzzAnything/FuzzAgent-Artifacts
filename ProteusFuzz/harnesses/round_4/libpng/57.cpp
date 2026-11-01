// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_gAMA at pngset.c:365:1 in png.h
// png_set_cHRM at pngset.c:99:1 in png.h
// png_set_cHRM_XYZ at pngset.c:115:1 in png.h
// png_set_mDCV at pngset.c:297:1 in png.h
// png_set_sRGB at pngset.c:819:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_XYZ at pngget.c:559:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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
#include <cstring>
#include <cstdlib>

static void write_dummy_png(const uint8_t *data, size_t size, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    // Write minimal valid PNG signature
    const uint8_t png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(png_sig, 1, 8, f);
    
    // Write IHDR chunk (minimal)
    uint32_t width = 1, height = 1, bit_depth = 8, color_type = 2;
    uint32_t compression = 0, filter = 0, interlace = 0;
    uint32_t length = 13;
    
    fwrite(&length, 4, 1, f);
    fwrite("IHDR", 4, 1, f);
    fwrite(&width, 4, 1, f);
    fwrite(&height, 4, 1, f);
    fwrite(&bit_depth, 1, 1, f);
    fwrite(&color_type, 1, 1, f);
    fwrite(&compression, 1, 1, f);
    fwrite(&filter, 1, 1, f);
    fwrite(&interlace, 1, 1, f);
    
    // CRC (dummy)
    uint32_t crc = 0;
    fwrite(&crc, 4, 1, f);
    
    // Write IDAT chunk with provided data
    if (size > 0) {
        length = (uint32_t)size;
        fwrite(&length, 4, 1, f);
        fwrite("IDAT", 4, 1, f);
        fwrite(data, 1, size, f);
        fwrite(&crc, 4, 1, f);
    }
    
    // IEND chunk
    length = 0;
    fwrite(&length, 4, 1, f);
    fwrite("IEND", 4, 1, f);
    fwrite(&crc, 4, 1, f);
    
    fclose(f);
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Silent error handler for fuzzing
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Silent warning handler for fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create dummy PNG file
    const char *filename = "./dummy_file";
    write_dummy_png(Data, Size, filename);

    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Setup error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Open the dummy file
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Read header
    png_read_info(png_ptr, info_ptr);

    // Extract fuzzing data for parameters
    double gamma = 0.0;
    if (Size > 0) gamma = (double)Data[0] / 255.0;
    if (Size > 1) gamma += (double)Data[1] / 1000.0;

    double white_x = 0.0, white_y = 0.0;
    double red_x = 0.0, red_y = 0.0;
    double green_x = 0.0, green_y = 0.0;
    double blue_x = 0.0, blue_y = 0.0;

    if (Size > 2) white_x = (double)Data[2] / 255.0;
    if (Size > 3) white_y = (double)Data[3] / 255.0;
    if (Size > 4) red_x = (double)Data[4] / 255.0;
    if (Size > 5) red_y = (double)Data[5] / 255.0;
    if (Size > 6) green_x = (double)Data[6] / 255.0;
    if (Size > 7) green_y = (double)Data[7] / 255.0;
    if (Size > 8) blue_x = (double)Data[8] / 255.0;
    if (Size > 9) blue_y = (double)Data[9] / 255.0;

    double red_X = 0.0, red_Y = 0.0, red_Z = 0.0;
    double green_X = 0.0, green_Y = 0.0, green_Z = 0.0;
    double blue_X = 0.0, blue_Y = 0.0, blue_Z = 0.0;

    if (Size > 10) red_X = (double)Data[10] / 255.0;
    if (Size > 11) red_Y = (double)Data[11] / 255.0;
    if (Size > 12) red_Z = (double)Data[12] / 255.0;
    if (Size > 13) green_X = (double)Data[13] / 255.0;
    if (Size > 14) green_Y = (double)Data[14] / 255.0;
    if (Size > 15) green_Z = (double)Data[15] / 255.0;
    if (Size > 16) blue_X = (double)Data[16] / 255.0;
    if (Size > 17) blue_Y = (double)Data[17] / 255.0;
    if (Size > 18) blue_Z = (double)Data[18] / 255.0;

    double max_lum = 0.0, min_lum = 0.0;
    if (Size > 19) max_lum = (double)Data[19] * 1000.0;
    if (Size > 20) min_lum = (double)Data[20] * 100.0;

    int srgb_intent = 0;
    if (Size > 21) srgb_intent = Data[21] % 4;

    // Test png_set_gAMA
    png_set_gAMA(png_ptr, info_ptr, gamma);

    // Test png_set_cHRM
    png_set_cHRM(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y);

    // Test png_set_cHRM_XYZ
    png_set_cHRM_XYZ(png_ptr, info_ptr, red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z);

    // Test png_set_mDCV
    png_set_mDCV(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y, max_lum, min_lum);

    // Test png_set_sRGB
    png_set_sRGB(png_ptr, info_ptr, srgb_intent);

    // Test retrieval functions
    double get_white_x, get_white_y, get_red_x, get_red_y, get_green_x, get_green_y, get_blue_x, get_blue_y;
    png_get_cHRM(png_ptr, info_ptr, &get_white_x, &get_white_y, &get_red_x, &get_red_y, &get_green_x, &get_green_y, &get_blue_x, &get_blue_y);

    double get_red_X, get_red_Y, get_red_Z, get_green_X, get_green_Y, get_green_Z, get_blue_X, get_blue_Y, get_blue_Z;
    png_get_cHRM_XYZ(png_ptr, info_ptr, &get_red_X, &get_red_Y, &get_red_Z, &get_green_X, &get_green_Y, &get_green_Z, &get_blue_X, &get_blue_Y, &get_blue_Z);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    remove(filename);

    return 0;
}