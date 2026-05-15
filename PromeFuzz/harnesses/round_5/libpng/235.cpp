// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_sCAL_s at pngset.c:598:1 in png.h
// png_set_sCAL_fixed at pngset.c:694:1 in png.h
// png_get_sCAL at pngget.c:1049:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_sCAL_s at pngget.c:1067:1 in png.h
// png_get_pCAL at pngget.c:994:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "code/png.h"
#include <cstdint>
#include <cstring>
#include <cstdlib>

static void dummy_error_fn(png_structp, png_const_charp) {}
static void dummy_warning_fn(png_structp, png_const_charp) {}

static png_structp create_png_struct() {
    return png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
}

static png_infop create_info_struct(png_structp png_ptr) {
    return png_create_info_struct(png_ptr);
}

extern "C" int LLVMFuzzerTestOneInput_235(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create PNG structures
    png_structp png_ptr = create_png_struct();
    if (!png_ptr) return 0;
    
    png_infop info_ptr = create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp)nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, (png_infopp)nullptr);
        return 0;
    }

    // Extract parameters from fuzz data
    int unit = (Data[0] % 2) + 1; // Ensure 1 or 2
    
    // For string parameters
    size_t str_len = Size > 1 ? Size - 1 : 0;
    char* str_param = static_cast<char*>(malloc(str_len + 1));
    if (str_param) {
        if (str_len > 0) {
            memcpy(str_param, Data + 1, str_len);
        }
        str_param[str_len] = '\0';
    }

    // For fixed-point parameters
    png_fixed_point fixed_val = 0;
    if (Size >= 5) {
        fixed_val = static_cast<png_fixed_point>(Data[1] << 24 | Data[2] << 16 | Data[3] << 8 | Data[4]);
    }

    // Test png_set_sCAL_s
    if (str_param) {
        png_set_sCAL_s(png_ptr, info_ptr, unit, str_param, str_param);
    }

    // Test png_set_sCAL_fixed
    png_set_sCAL_fixed(png_ptr, info_ptr, unit, fixed_val, fixed_val);

    // Variables for get functions
    int retrieved_unit = 0;
    double dbl_width = 0.0, dbl_height = 0.0;
    png_fixed_point fixed_width = 0, fixed_height = 0;
    char* str_width = nullptr;
    char* str_height = nullptr;
    
    // Test png_get_sCAL (floating point)
    #if defined(PNG_FLOATING_ARITHMETIC_SUPPORTED) || defined(PNG_FLOATING_POINT_SUPPORTED)
    png_get_sCAL(png_ptr, info_ptr, &retrieved_unit, &dbl_width, &dbl_height);
    #endif

    // Test png_get_sCAL_fixed
    #if defined(PNG_FLOATING_ARITHMETIC_SUPPORTED) || defined(PNG_FLOATING_POINT_SUPPORTED)
    png_get_sCAL_fixed(png_ptr, info_ptr, &retrieved_unit, &fixed_width, &fixed_height);
    #endif

    // Test png_get_sCAL_s
    png_get_sCAL_s(png_ptr, info_ptr, &retrieved_unit, &str_width, &str_height);

    // Test png_get_pCAL
    char* purpose = nullptr;
    png_int_32 X0 = 0, X1 = 0;
    int type = 0, nparams = 0;
    char* units = nullptr;
    char** params = nullptr;
    png_get_pCAL(png_ptr, info_ptr, &purpose, &X0, &X1, &type, &nparams, &units, &params);

    // Cleanup
    if (str_param) free(str_param);
    png_destroy_write_struct(&png_ptr, (png_infopp)&info_ptr);
    
    return 0;
}