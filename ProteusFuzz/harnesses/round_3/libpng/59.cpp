// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_next_frame_blend_op at pngget.c:1499:1 in png.h
// png_get_eXIf_1 at pngget.c:892:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_next_frame_dispose_op at pngget.c:1489:1 in png.h
// png_get_first_frame_is_hidden at pngget.c:1509:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
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
#include <setjmp.h>
#include <stdio.h>

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void dummy_write_data_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void *dummy_malloc_fn(png_structp png_ptr, png_size_t size) {
    (void)png_ptr;
    return malloc(size);
}

static void dummy_free_fn(png_structp png_ptr, void *ptr) {
    (void)png_ptr;
    free(ptr);
}

static void dummy_read_data_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input data to a dummy file to simulate a PNG stream
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Reopen for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;

    // Initialize png_struct and png_info
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return 0;
    }

    // Set error handlers
    png_set_error_fn(png_ptr, NULL, dummy_error_fn, dummy_warning_fn);

    // Initialize IO callback
    png_init_io(png_ptr, fp);

    // Use setjmp to catch libpng errors
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }

    // Attempt to read the PNG header
    png_read_info(png_ptr, info_ptr);

    // Fuzz target API functions
    png_byte dispose_op = png_get_next_frame_dispose_op(png_ptr, info_ptr);
    (void)dispose_op;

    png_byte first_frame_hidden = png_get_first_frame_is_hidden(png_ptr, info_ptr);
    (void)first_frame_hidden;

    png_byte compression_type = png_get_compression_type(png_ptr, info_ptr);
    (void)compression_type;

    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    (void)color_type;

    png_byte blend_op = png_get_next_frame_blend_op(png_ptr, info_ptr);
    (void)blend_op;

    png_uint_32 num_exif = 0;
    png_byte *exif = NULL;
    png_uint_32 exif_result = png_get_eXIf_1(png_ptr, info_ptr, &num_exif, &exif);
    (void)exif_result;
    if (exif && num_exif > 0) {
        // Free exif data if allocated by libpng (though typically it's internal)
        // Do not free here as libpng manages it internally
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    return 0;
}