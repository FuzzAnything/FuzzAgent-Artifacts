// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_sCAL_s at pngset.c:598:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_pCAL at pngset.c:482:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_set_pHYs at pngset.c:723:1 in png.h
// png_set_iCCP at pngset.c:859:1 in png.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize libpng structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    // Set up error handling to prevent crashes during fuzzing
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Parse input data for test parameters
    size_t offset = 0;
    auto read_uint32 = [&]() -> png_uint_32 {
        if (offset + 4 > Size) return 0;
        png_uint_32 val = 0;
        memcpy(&val, Data + offset, 4);
        offset += 4;
        return val;
    };
    auto read_int32 = [&]() -> png_int_32 {
        if (offset + 4 > Size) return 0;
        png_int_32 val = 0;
        memcpy(&val, Data + offset, 4);
        offset += 4;
        return val;
    };
    auto read_byte = [&]() -> png_byte {
        if (offset >= Size) return 0;
        return Data[offset++];
    };
    auto read_string = [&](size_t max_len) -> std::string {
        if (offset >= Size) return "";
        size_t len = 0;
        while (offset < Size && len < max_len && Data[offset] != 0) {
            offset++;
            len++;
        }
        std::string str(reinterpret_cast<const char*>(Data + offset - len), len);
        return str;
    };

    // 1. Test png_set_gAMA_fixed
    png_fixed_point gamma = static_cast<png_fixed_point>(read_int32());
    png_set_gAMA_fixed(png_ptr, info_ptr, gamma);

    // 2. Test png_set_pHYs
    png_uint_32 res_x = read_uint32();
    png_uint_32 res_y = read_uint32();
    int unit_type = read_byte() % 2; // 0 or 1
    png_set_pHYs(png_ptr, info_ptr, res_x, res_y, unit_type);

    // 3. Test png_set_iCCP
    std::string iccp_name = read_string(100);
    int compression_type = read_byte() % 2; // 0 or 1
    png_uint_32 proflen = read_uint32();
    if (proflen > 10000) proflen = 10000; // Limit profile length
    
    // Use a stack-allocated buffer or ensure vector is not leaked if we return early
    // Since we don't return early here, vector will be destroyed at scope exit.
    // However, to be safe against potential early returns in future modifications or 
    // if the function logic changes, we keep it local.
    std::vector<png_byte> profile(proflen);
    for (size_t i = 0; i < proflen && offset < Size; ++i) {
        profile[i] = read_byte();
    }
    png_set_iCCP(png_ptr, info_ptr, iccp_name.c_str(), compression_type, profile.data(), profile.size());

    // 4. Test png_set_sCAL_s
    int sc_unit = read_byte() % 3; // 0, 1, or 2
    std::string swidth = read_string(50);
    std::string sheight = read_string(50);
    // Ensure non-empty strings to avoid immediate errors
    if (swidth.empty()) swidth = "1.0";
    if (sheight.empty()) sheight = "1.0";
    png_set_sCAL_s(png_ptr, info_ptr, sc_unit, swidth.c_str(), sheight.c_str());

    // 5. Test png_set_cHRM_XYZ_fixed
    png_fixed_point red_X = static_cast<png_fixed_point>(read_int32());
    png_fixed_point red_Y = static_cast<png_fixed_point>(read_int32());
    png_fixed_point red_Z = static_cast<png_fixed_point>(read_int32());
    png_fixed_point green_X = static_cast<png_fixed_point>(read_int32());
    png_fixed_point green_Y = static_cast<png_fixed_point>(read_int32());
    png_fixed_point green_Z = static_cast<png_fixed_point>(read_int32());
    png_fixed_point blue_X = static_cast<png_fixed_point>(read_int32());
    png_fixed_point blue_Y = static_cast<png_fixed_point>(read_int32());
    png_fixed_point blue_Z = static_cast<png_fixed_point>(read_int32());
    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z);

    // 6. Test png_set_pCAL
    std::string purpose = read_string(100);
    png_int_32 X0 = read_int32();
    png_int_32 X1 = read_int32();
    int type = read_byte() % 4; // 0-3
    int nparams = read_byte() % 256;
    std::string units = read_string(50);
    
    // Store strings in a vector to keep them alive for the duration of the call
    std::vector<std::string> param_strings;
    param_strings.reserve(nparams);
    std::vector<char*> params(nparams);
    
    for (int i = 0; i < nparams; ++i) {
        std::string p = read_string(50);
        param_strings.push_back(std::move(p));
        params[i] = const_cast<char*>(param_strings[i].c_str());
    }
    
    png_set_pCAL(png_ptr, info_ptr, purpose.c_str(), X0, X1, type, nparams, units.c_str(), params.data());

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return 0;
}