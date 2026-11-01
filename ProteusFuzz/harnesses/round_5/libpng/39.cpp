// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_sCAL_s at pngset.c:598:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_set_sCAL_fixed at pngset.c:694:1 in png.h
// png_set_pCAL at pngset.c:482:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
// Fuzz driver for libpng API functions
#include <png.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Suppress compiler warnings for unused parameters in fuzzing context
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"

static void* safe_malloc(size_t size) {
    if (size == 0 || size > 1024 * 1024) return NULL;
    void* ptr = malloc(size);
    if (ptr) memset(ptr, 0, size);
    return ptr;
}

static void safe_free(void* ptr) {
    if (ptr) free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Setup error handling to prevent crashes during fuzzing
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Parse input data to drive fuzzing
    int unit = (Data[0] % 3) + 1; // 1 or 2 (valid units)
    int srgb_intent = Data[1] % 4;
    png_fixed_point width_fixed = (png_fixed_point)(Data[2] * 100000 + (Data[3] << 8));
    png_fixed_point height_fixed = (png_fixed_point)(Data[4] * 100000 + (Data[5] << 8));
    png_fixed_point gamma_fixed = (png_fixed_point)(Data[6] * 100000 + (Data[7] << 8));
    
    // Create string buffers for sCAL_s
    char width_str[32], height_str[32];
    snprintf(width_str, sizeof(width_str), "%d.%d", Data[8], Data[9]);
    snprintf(height_str, sizeof(height_str), "%d.%d", Data[10 % Size], Data[11 % Size]);

    // 1. Test png_set_sCAL_s
    png_set_sCAL_s(png_ptr, info_ptr, unit, width_str, height_str);

    // 2. Test png_set_sRGB_gAMA_and_cHRM
    png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, srgb_intent);

    // 3. Test png_set_sCAL_fixed
    png_set_sCAL_fixed(png_ptr, info_ptr, unit, width_fixed, height_fixed);

    // 4. Test png_set_pCAL
    // Prepare parameters for pCAL
    const char* purpose = "test";
    png_int_32 X0 = (png_int_32)Data[12 % Size];
    png_int_32 X1 = (png_int_32)Data[13 % Size];
    int type = Data[14 % Size] % 4; // 0-3
    int nparams = Data[15 % Size] % 10; // 0-9
    const char* units = "m";
    
    // Allocate params array
    char **params = (char **)safe_malloc(nparams * sizeof(char *));
    if (params) {
        for (int i = 0; i < nparams; i++) {
            params[i] = (char *)safe_malloc(16);
            if (params[i]) {
                snprintf(params[i], 16, "%d", Data[(16 + i) % Size]);
            }
        }
        png_set_pCAL(png_ptr, info_ptr, purpose, X0, X1, type, nparams, units, params);
        
        // Cleanup params
        for (int i = 0; i < nparams; i++) {
            safe_free(params[i]);
        }
        safe_free(params);
    }

    // 5. Test png_set_gAMA_fixed
    png_set_gAMA_fixed(png_ptr, info_ptr, gamma_fixed);

    // 6. Test png_destroy_info_struct
    // Correct usage: pass the pointer value (info_ptr), not the address of the pointer variable.
    // This ensures the internal allocations (like sCAL strings) are freed via the free_me flags.
    png_destroy_info_struct(png_ptr, &info_ptr);

    // Cleanup png_ptr
    // Note: info_ptr is already NULL after png_destroy_info_struct, so passing NULL is safe.
    png_destroy_read_struct(&png_ptr, NULL, NULL);

    return 0;
}

#pragma GCC diagnostic pop