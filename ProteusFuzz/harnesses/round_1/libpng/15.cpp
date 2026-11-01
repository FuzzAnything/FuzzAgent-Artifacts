// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_sCAL_fixed at pngset.c:694:1 in png.h
// png_set_oFFs at pngset.c:465:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_get_pixel_aspect_ratio_fixed at pngget.c:213:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_get_y_offset_inches_fixed at pngget.c:403:1 in png.h
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
#include <cstddef>

static void* safe_malloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Use setjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Parse input data to generate test values
    int unit = Data[0] % 3; // 0, 1, or 2 for unit type
    png_fixed_point width_fixed = static_cast<png_fixed_point>(Data[1] * 256 + Data[2]);
    png_fixed_point height_fixed = static_cast<png_fixed_point>(Data[3] * 256 + Data[4]);
    png_int_32 offset_x = static_cast<png_int_32>(Data[5] * 256 + Data[6]);
    png_int_32 offset_y = static_cast<png_int_32>(Data[7] * 256 + Data[8]);

    // Test png_set_sCAL_fixed
    png_set_sCAL_fixed(png_ptr, info_ptr, unit, width_fixed, height_fixed);

    // Test png_set_oFFs
    png_set_oFFs(png_ptr, info_ptr, offset_x, offset_y, unit);

    // Test retrieval functions
    png_int_32 ret_offset_x, ret_offset_y;
    int ret_unit_type;
    png_get_oFFs(png_ptr, info_ptr, &ret_offset_x, &ret_offset_y, &ret_unit_type);

    png_fixed_point aspect_ratio = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
    (void)aspect_ratio; // Suppress unused variable warning

    png_fixed_point x_offset_inches = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    (void)x_offset_inches;

    png_fixed_point y_offset_inches = png_get_y_offset_inches_fixed(png_ptr, info_ptr);
    (void)y_offset_inches;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}