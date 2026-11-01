// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_next_frame_x_offset at pngget.c:1449:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_num_frames at pngget.c:1379:1 in png.h
// png_get_next_frame_width at pngget.c:1429:1 in png.h
// png_get_next_frame_height at pngget.c:1439:1 in png.h
// png_get_next_frame_y_offset at pngget.c:1459:1 in png.h
// png_get_num_plays at pngget.c:1389:1 in png.h
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
#include <cstddef>
#include <cstdio>
#include <cstring>

static void dummy_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
    // Do nothing for dummy read
}

static void dummy_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do nothing for dummy error
}

static void dummy_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Do nothing for dummy warning
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file to read from
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Reopen for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        return 0;
    }

    // Allocate png_struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    // Set custom read and error functions
    png_set_read_fn(png_ptr, (png_voidp)fp, dummy_read_data);
    png_set_error_fn(png_ptr, NULL, dummy_error, dummy_warning);

    // Allocate png_info
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return 0;
    }

    // Use setjmp to handle libpng errors
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }

    // Try to read the PNG info
    png_read_info(png_ptr, info_ptr);

    // Call the target API functions
    png_uint_32 num_frames = png_get_num_frames(png_ptr, info_ptr);
    (void)num_frames;

    png_uint_32 next_frame_width = png_get_next_frame_width(png_ptr, info_ptr);
    (void)next_frame_width;

    png_uint_32 next_frame_height = png_get_next_frame_height(png_ptr, info_ptr);
    (void)next_frame_height;

    png_uint_32 next_frame_y_offset = png_get_next_frame_y_offset(png_ptr, info_ptr);
    (void)next_frame_y_offset;

    png_uint_32 num_plays = png_get_num_plays(png_ptr, info_ptr);
    (void)num_plays;

    png_uint_32 next_frame_x_offset = png_get_next_frame_x_offset(png_ptr, info_ptr);
    (void)next_frame_x_offset;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    return 0;
}