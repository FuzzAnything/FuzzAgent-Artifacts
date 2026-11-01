// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_sCAL_s at pngset.c:598:1 in png.h
// png_set_pCAL at pngset.c:482:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    } else if (info_ptr) {
        png_destroy_info_struct(nullptr, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a dummy file if needed (though these APIs don't strictly require reading a file)
    write_dummy_file(Data, Size);

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Use setjmp for error handling as required by libpng
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Helper to extract integers from fuzz data
    auto get_int = [&](size_t &idx) -> int {
        if (idx + sizeof(int) > Size) return 0;
        int val;
        memcpy(&val, &Data[idx], sizeof(int));
        idx += sizeof(int);
        return val;
    };

    auto get_fixed = [&](size_t &idx) -> png_fixed_point {
        if (idx + sizeof(png_fixed_point) > Size) return 0;
        png_fixed_point val;
        memcpy(&val, &Data[idx], sizeof(png_fixed_point));
        idx += sizeof(png_fixed_point);
        return val;
    };

    auto get_cstr = [&](size_t &idx) -> const char * {
        if (idx >= Size) return nullptr;
        size_t len = 0;
        while (idx + len < Size && Data[idx + len] != 0) len++;
        if (idx + len >= Size) return nullptr;
        // Ensure we don't allocate too much
        if (len > 1024) return nullptr;
        char *str = (char *)malloc(len + 1);
        if (!str) return nullptr;
        memcpy(str, &Data[idx], len);
        str[len] = 0;
        idx += len + 1;
        return str;
    };

    size_t idx = 0;

    // 1. png_set_sRGB_gAMA_and_cHRM
    if (idx < Size) {
        int srgb_intent = Data[idx] % 4; // 0 to 3
        idx++;
        png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, srgb_intent);
    }

    // 2. png_set_sCAL_s
    if (idx < Size) {
        int unit = Data[idx] % 3; // 0, 1, 2
        idx++;
        const char *swidth = get_cstr(idx);
        const char *sheight = get_cstr(idx);
        if (swidth && sheight) {
            png_set_sCAL_s(png_ptr, info_ptr, unit, swidth, sheight);
            free((void *)swidth);
            free((void *)sheight);
        }
    }

    // 3. png_set_pCAL
    if (idx < Size) {
        const char *purpose = get_cstr(idx);
        if (!purpose) {
            if (idx < Size) idx++; // Skip one byte to avoid infinite loop if purpose failed
        } else {
            png_int_32 X0 = get_int(idx);
            png_int_32 X1 = get_int(idx);
            if (idx < Size) {
                int type = Data[idx] % 4; // 0-3
                idx++;
                if (idx < Size) {
                    int nparams = Data[idx] % 256;
                    idx++;
                    const char *units = get_cstr(idx);
                    
                    char **params = nullptr;
                    if (nparams > 0) {
                        params = (char **)malloc(nparams * sizeof(char *));
                        if (params) {
                            for (int i = 0; i < nparams; i++) {
                                params[i] = (char *)get_cstr(idx);
                            }
                        }
                    }

                    if (units && (params || nparams == 0)) {
                        png_set_pCAL(png_ptr, info_ptr, purpose, X0, X1, type, nparams, units, params);
                    }

                    if (params) {
                        for (int i = 0; i < nparams; i++) {
                            if (params[i]) free(params[i]);
                        }
                        free(params);
                    }
                    if (units) free((void *)units);
                }
            }
            free((void *)purpose);
        }
    }

    // 4. png_set_invalid
    if (idx < Size) {
        int mask = get_int(idx);
        png_set_invalid(png_ptr, info_ptr, mask);
    }

    // 5. png_set_cHRM_XYZ_fixed
    if (idx + 9 * sizeof(png_fixed_point) <= Size) {
        png_fixed_point int_red_X = get_fixed(idx);
        png_fixed_point int_red_Y = get_fixed(idx);
        png_fixed_point int_red_Z = get_fixed(idx);
        png_fixed_point int_green_X = get_fixed(idx);
        png_fixed_point int_green_Y = get_fixed(idx);
        png_fixed_point int_green_Z = get_fixed(idx);
        png_fixed_point int_blue_X = get_fixed(idx);
        png_fixed_point int_blue_Y = get_fixed(idx);
        png_fixed_point int_blue_Z = get_fixed(idx);
        png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, int_red_X, int_red_Y, int_red_Z,
                               int_green_X, int_green_Y, int_green_Z,
                               int_blue_X, int_blue_Y, int_blue_Z);
    }

    // 6. png_set_gAMA_fixed
    if (idx < Size) {
        png_fixed_point int_file_gamma = get_fixed(idx);
        png_set_gAMA_fixed(png_ptr, info_ptr, int_file_gamma);
    }

    cleanup_png(png_ptr, info_ptr);
    return 0;
}