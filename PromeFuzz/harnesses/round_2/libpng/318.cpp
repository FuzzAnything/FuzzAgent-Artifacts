// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_mDCV_fixed at pngset.c:225:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_cICP at pngset.c:136:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_get_cHRM_XYZ at pngget.c:559:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
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
#include <cstdlib>
#include <cstring>

#include "code/png.h"

static void dummy_error_fn(png_structp, png_const_charp) {}
static void dummy_warning_fn(png_structp, png_const_charp) {}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    return png_ptr;
}

static png_inforp create_info_struct(png_structp png_ptr) {
    if (!png_ptr) return nullptr;
    return png_create_info_struct(png_ptr);
}

extern "C" int LLVMFuzzerTestOneInput_318(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0; // Need minimum data for parameters

    // Create PNG structures
    png_structp png_ptr = create_png_struct();
    if (!png_ptr) return 0;
    
    png_inforp info_ptr = create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Create a png_infopp from info_ptr
        png_info* info_non_restrict = info_ptr;
        png_infopp info_ptr_ptr = &info_non_restrict;
        png_destroy_info_struct(png_ptr, info_ptr_ptr);
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Test png_set_cICP
    if (Size >= 4) {
        png_byte colour_primaries = Data[0];
        png_byte transfer_function = Data[1];
        png_byte matrix_coefficients = Data[2];
        png_byte video_full_range_flag = Data[3];
        
        png_set_cICP(png_ptr, info_ptr, colour_primaries, transfer_function,
                     matrix_coefficients, video_full_range_flag);
        
        // Test png_get_cICP
        png_byte out_cp, out_tf, out_mc, out_vfr;
        png_get_cICP(png_ptr, info_ptr, &out_cp, &out_tf, &out_mc, &out_vfr);
    }

    // Test png_set_cHRM_XYZ_fixed
    if (Size >= 36) { // 9 png_fixed_point * 4 bytes each
        const uint8_t* ptr = Data;
        png_fixed_point params[9];
        for (int i = 0; i < 9; i++) {
            if (Size >= (ptr - Data) + 4) {
                params[i] = *reinterpret_cast<const png_fixed_point*>(ptr);
                ptr += 4;
            }
        }
        
        png_set_cHRM_XYZ_fixed(png_ptr, info_ptr,
                               params[0], params[1], params[2],
                               params[3], params[4], params[5],
                               params[6], params[7], params[8]);
        
        // Test png_get_cHRM_XYZ (floating point version)
        double dparams[9];
        png_get_cHRM_XYZ(png_ptr, info_ptr,
                        &dparams[0], &dparams[1], &dparams[2],
                        &dparams[3], &dparams[4], &dparams[5],
                        &dparams[6], &dparams[7], &dparams[8]);
        
        // Test png_get_cHRM_XYZ_fixed
        png_fixed_point fparams[9];
        png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
                              &fparams[0], &fparams[1], &fparams[2],
                              &fparams[3], &fparams[4], &fparams[5],
                              &fparams[6], &fparams[7], &fparams[8]);
    }

    // Test png_set_mDCV_fixed
    if (Size >= 40) { // 8 png_fixed_point * 4 bytes + 2 png_uint_32 * 4 bytes
        const uint8_t* ptr = Data;
        png_fixed_point chroma_params[8];
        for (int i = 0; i < 8; i++) {
            if (Size >= (ptr - Data) + 4) {
                chroma_params[i] = *reinterpret_cast<const png_fixed_point*>(ptr);
                ptr += 4;
            }
        }
        
        png_uint_32 lum_values[2];
        for (int i = 0; i < 2; i++) {
            if (Size >= (ptr - Data) + 4) {
                lum_values[i] = *reinterpret_cast<const png_uint_32*>(ptr);
                ptr += 4;
            }
        }
        
        png_set_mDCV_fixed(png_ptr, info_ptr,
                          chroma_params[0], chroma_params[1],
                          chroma_params[2], chroma_params[3],
                          chroma_params[4], chroma_params[5],
                          chroma_params[6], chroma_params[7],
                          lum_values[0], lum_values[1]);
    }

    // Test png_destroy_info_struct
    // Create a png_infopp from info_ptr
    png_info* info_non_restrict = info_ptr;
    png_infopp info_ptr_ptr = &info_non_restrict;
    png_destroy_info_struct(png_ptr, info_ptr_ptr);
    
    // Clean up remaining structures
    png_destroy_write_struct(&png_ptr, nullptr);
    
    return 0;
}