// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_cHRM at pngset.c:99:1 in png.h
// png_get_cLLI at pngget.c:808:1 in png.h
// png_get_mDCV at pngget.c:860:1 in png.h
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
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_sRGB at pngget.c:699:1 in png.h
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
#include <vector>

static void dummy_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void write_data_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    std::vector<uint8_t> png_data(8 + Size);
    png_data[0] = 0x89; png_data[1] = 0x50; png_data[2] = 0x4E; png_data[3] = 0x47;
    png_data[4] = 0x0D; png_data[5] = 0x0A; png_data[6] = 0x1A; png_data[7] = 0x0A;
    
    memcpy(png_data.data() + 8, Data, Size);

    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(png_data.data(), 1, png_data.size(), fp);
    fclose(fp);

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_callback, dummy_warning_callback);
    if (!png_ptr) {
        return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    fp = fopen(filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);
    fclose(fp);

    double white_x = (Size > 0 ? Data[0] : 0.0) / 255.0;
    double white_y = (Size > 1 ? Data[1] : 0.0) / 255.0;
    double red_x = (Size > 2 ? Data[2] : 0.0) / 255.0;
    double red_y = (Size > 3 ? Data[3] : 0.0) / 255.0;
    double green_x = (Size > 4 ? Data[4] : 0.0) / 255.0;
    double green_y = (Size > 5 ? Data[5] : 0.0) / 255.0;
    double blue_x = (Size > 6 ? Data[6] : 0.0) / 255.0;
    double blue_y = (Size > 7 ? Data[7] : 0.0) / 255.0;
    double max_lum = (Size > 8 ? Data[8] : 0.0) * 100.0;
    double min_lum = (Size > 9 ? Data[9] : 0.0) * 100.0;

#ifdef PNG_mDCV_SUPPORTED
    png_set_mDCV(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y, max_lum, min_lum);
#endif

    double gx_white_x, gx_white_y, gx_red_x, gx_red_y, gx_green_x, gx_green_y, gx_blue_x, gx_blue_y;
    png_get_cHRM(png_ptr, info_ptr, &gx_white_x, &gx_white_y, &gx_red_x, &gx_red_y, &gx_green_x, &gx_green_y, &gx_blue_x, &gx_blue_y);

    int srgb_intent = 0;
    png_get_sRGB(png_ptr, info_ptr, &srgb_intent);

    png_set_cHRM(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y);

    double max_content, max_frame_avg;
    png_get_cLLI(png_ptr, info_ptr, &max_content, &max_frame_avg);

    double get_white_x, get_white_y, get_red_x, get_red_y, get_green_x, get_green_y, get_blue_x, get_blue_y;
    double get_max_lum, get_min_lum;
    png_get_mDCV(png_ptr, info_ptr, &get_white_x, &get_white_y, &get_red_x, &get_red_y, &get_green_x, &get_green_y, &get_blue_x, &get_blue_y, &get_max_lum, &get_min_lum);

    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    remove(filename);

    return 0;
}