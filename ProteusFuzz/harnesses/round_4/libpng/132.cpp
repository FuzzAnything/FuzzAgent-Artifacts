// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
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

extern "C" int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Allocate buffers for save operations
    png_byte save_buf_4[4];
    png_byte save_buf_2[2];
    
    // Allocate buffer for get operations (reuse input if large enough)
    const png_byte *get_buf_4 = nullptr;
    
    // Initialize png_struct for png_get_uint_31 which requires it
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }
    
    // Create a minimal info struct to satisfy potential internal checks if needed
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Use setjmp to handle png_error called by png_get_uint_31
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Fuzz png_save_int_32
    png_int_32 int_val = 0;
    if (Size >= 4) {
        int_val = *(const int32_t*)(Data);
        png_save_int_32(save_buf_4, int_val);
    }

    // Fuzz png_save_uint_16
    unsigned int uint16_val = 0;
    if (Size >= 2) {
        uint16_val = *(const uint16_t*)(Data);
        png_save_uint_16(save_buf_2, uint16_val);
    }

    // Fuzz png_get_int_32
    if (Size >= 4) {
        get_buf_4 = Data;
        png_int_32 res_int = png_get_int_32(get_buf_4);
        (void)res_int; // Suppress unused variable warning
    }

    // Fuzz png_get_uint_31
    if (Size >= 4) {
        get_buf_4 = Data;
        // This function may call png_error if value > PNG_UINT_31_MAX
        // The setjmp above handles the longjmp
        png_uint_32 res_uint31 = png_get_uint_31(png_ptr, get_buf_4);
        (void)res_uint31;
    }

    // Fuzz png_get_uint_32
    if (Size >= 4) {
        get_buf_4 = Data;
        png_uint_32 res_uint32 = png_get_uint_32(get_buf_4);
        (void)res_uint32;
    }

    // Fuzz png_save_uint_32
    png_uint_32 uint32_val = 0;
    if (Size >= 4) {
        uint32_val = *(const uint32_t*)(Data);
        png_save_uint_32(save_buf_4, uint32_val);
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    return 0;
}