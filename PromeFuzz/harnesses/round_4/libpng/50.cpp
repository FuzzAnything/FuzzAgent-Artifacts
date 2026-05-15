// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_invert_mono at pngtrans.c:249:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_set_packing at pngtrans.c:49:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_set_bgr at pngtrans.c:20:1 in png.h
// png_set_packswap at pngtrans.c:69:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "png.h"

static void dummy_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy read function - doesn't actually read anything
    memset(data, 0, length);
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing with warnings
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing with errors
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    // Write the fuzzing data to a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        remove("./dummy_file");
        return 0;
    }

    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        remove("./dummy_file");
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        remove("./dummy_file");
        return 0;
    }

    // Set up custom read function
    png_set_read_fn(png_ptr, nullptr, dummy_read_data);

    // Call target API functions based on input data
    // Use the first 6 bits of the first byte to decide which functions to call
    if (Size > 0) {
        uint8_t selector = Data[0];
        
        if (selector & 0x01) {
            png_set_invert_mono(png_ptr);
        }
        if (selector & 0x02) {
            png_set_expand(png_ptr);
        }
        if (selector & 0x04) {
            png_set_packing(png_ptr);
        }
        if (selector & 0x08) {
            png_set_swap_alpha(png_ptr);
        }
        if (selector & 0x10) {
            png_set_bgr(png_ptr);
        }
        if (selector & 0x20) {
            png_set_packswap(png_ptr);
        }
    }

    // Try to read the PNG info to trigger the transformations
    // This might fail, but that's OK for fuzzing
    png_read_info(png_ptr, info_ptr);

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    remove("./dummy_file");

    return 0;
}