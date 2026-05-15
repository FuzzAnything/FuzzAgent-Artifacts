// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_user_limits at pngset.c:1937:1 in png.h
// png_get_user_width_max at pngget.c:1305:1 in png.h
// png_get_user_height_max at pngget.c:1311:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
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
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <setjmp.h>

#include "code/png.h"

static void dummy_png_error(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - just prevent default error handling
}

static void dummy_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing - just prevent default warning handling
}

extern "C" int LLVMFuzzerTestOneInput_192(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    // Write fuzzing data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open dummy file for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        return 0;
    }

    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                  dummy_png_error, dummy_png_warning);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return 0;
    }

    // Set up error handling with setjmp/longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during PNG processing
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
        remove("./dummy_file");
        return 0;
    }

    // Set up PNG reading from file
    png_init_io(png_ptr, fp);

    // Extract user limits from fuzzing data (first 8 bytes)
    png_uint_32 user_width_max = 0;
    png_uint_32 user_height_max = 0;
    if (Size >= 8) {
        // Use memcpy to avoid strict aliasing issues
        memcpy(&user_width_max, Data, sizeof(png_uint_32));
        memcpy(&user_height_max, Data + 4, sizeof(png_uint_32));
    }

    // Set user limits
    png_set_user_limits(png_ptr, user_width_max, user_height_max);

    // Get user limits back
    png_uint_32 retrieved_width_max = png_get_user_width_max(png_ptr);
    png_uint_32 retrieved_height_max = png_get_user_height_max(png_ptr);

    // Try to read PNG info - this may fail for invalid PNG data
    png_read_info(png_ptr, info_ptr);

    // Get image dimensions - only if png_read_info succeeded
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);

    // Remove dummy file
    remove("./dummy_file");

    return 0;
}