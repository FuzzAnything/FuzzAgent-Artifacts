// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_eXIf_1 at pngget.c:892:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_next_frame_dispose_op at pngget.c:1489:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_next_frame_blend_op at pngget.c:1499:1 in png.h
// png_get_first_frame_is_hidden at pngget.c:1509:1 in png.h
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
#include <setjmp.h>

// Dummy error handler to prevent libpng from calling abort
static void dummy_error_handler(png_structp png_ptr, const char *error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

// Dummy warning handler
static void dummy_warning_handler(png_structp png_ptr, const char *warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file to hold the fuzzed data
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        remove(filename);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        remove(filename);
        return 0;
    }

    // Set up error handling to prevent crashes
    png_set_error_fn(png_ptr, NULL, dummy_error_handler, dummy_warning_handler);

    // Use setjmp/longjmp to handle libpng errors gracefully
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        remove(filename);
        return 0;
    }

    // Initialize I/O
    fp = fopen(filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        remove(filename);
        return 0;
    }
    png_init_io(png_ptr, fp);

    // Read the PNG header and info chunks
    // We intentionally do not read the full image to focus on metadata APIs
    // and to allow malformed files to trigger error paths safely.
    png_read_info(png_ptr, info_ptr);

    // Close the file before calling get functions to avoid potential leaks
    // or state issues if libpng tries to read more.
    fclose(fp);

    // Call the target API functions to explore different states
    // 1. png_get_next_frame_dispose_op
    png_byte dispose_op = png_get_next_frame_dispose_op(png_ptr, info_ptr);
    (void)dispose_op;

    // 2. png_get_compression_type
    png_byte compression_type = png_get_compression_type(png_ptr, info_ptr);
    (void)compression_type;

    // 3. png_get_color_type
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    (void)color_type;

    // 4. png_get_next_frame_blend_op
    png_byte blend_op = png_get_next_frame_blend_op(png_ptr, info_ptr);
    (void)blend_op;

    // 5. png_get_first_frame_is_hidden
    png_byte first_frame_hidden = png_get_first_frame_is_hidden(png_ptr, info_ptr);
    (void)first_frame_hidden;

    // 6. png_get_eXIf_1
    png_uint_32 num_exif = 0;
    png_byte *exif = NULL;
    png_uint_32 exif_result = png_get_eXIf_1(png_ptr, info_ptr, &num_exif, &exif);
    (void)exif_result;
    (void)num_exif;
    (void)exif;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    remove(filename);

    return 0;
}