// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_cHRM at pngset.c:99:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
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
#include <cstring>
#include <cstdlib>

#include "code/png.h"

extern "C" int LLVMFuzzerTestOneInput_239(const uint8_t *Data, size_t Size) {
    if (Size < 13 * sizeof(png_fixed_point) + 4 * sizeof(double)) {
        return 0;
    }

    // Create dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Read PNG header
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);
    fclose(fp);

    // Extract values from fuzz data
    const png_fixed_point *fixed_data = reinterpret_cast<const png_fixed_point*>(Data);
    const double *double_data = reinterpret_cast<const double*>(Data + 13 * sizeof(png_fixed_point));

    // Test png_set_cHRM with floating point
    png_set_cHRM(png_ptr, info_ptr,
                 double_data[0], double_data[1],
                 double_data[2], double_data[3],
                 double_data[4], double_data[5],
                 double_data[6], double_data[7]);

    // Test png_set_cHRM_XYZ_fixed
    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr,
                           fixed_data[0], fixed_data[1], fixed_data[2],
                           fixed_data[3], fixed_data[4], fixed_data[5],
                           fixed_data[6], fixed_data[7], fixed_data[8]);

    // Test png_set_cHRM_fixed
    png_set_cHRM_fixed(png_ptr, info_ptr,
                       fixed_data[9], fixed_data[10],
                       fixed_data[0], fixed_data[1],
                       fixed_data[2], fixed_data[3],
                       fixed_data[4], fixed_data[5]);

    // Test png_set_gAMA_fixed
    png_set_gAMA_fixed(png_ptr, info_ptr, fixed_data[12]);

    // Test retrieval functions
    png_fixed_point retrieved_fixed[13];
    double retrieved_double[8];
    
    // Test png_get_cHRM_fixed
    png_get_cHRM_fixed(png_ptr, info_ptr,
                       &retrieved_fixed[0], &retrieved_fixed[1],
                       &retrieved_fixed[2], &retrieved_fixed[3],
                       &retrieved_fixed[4], &retrieved_fixed[5],
                       &retrieved_fixed[6], &retrieved_fixed[7]);

    // Test png_get_cHRM
    png_get_cHRM(png_ptr, info_ptr,
                 &retrieved_double[0], &retrieved_double[1],
                 &retrieved_double[2], &retrieved_double[3],
                 &retrieved_double[4], &retrieved_double[5],
                 &retrieved_double[6], &retrieved_double[7]);

    // Test png_get_gAMA_fixed
    png_fixed_point file_gamma;
    png_get_gAMA_fixed(png_ptr, info_ptr, &file_gamma);

    // Test png_get_cHRM_XYZ_fixed
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
                           &retrieved_fixed[8], &retrieved_fixed[9], &retrieved_fixed[10],
                           &retrieved_fixed[0], &retrieved_fixed[1], &retrieved_fixed[2],
                           &retrieved_fixed[3], &retrieved_fixed[4], &retrieved_fixed[5]);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}