// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_IHDR at pngget.c:928:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_y_pixels_per_inch at pngget.c:369:1 in png.h
// png_get_sCAL_s at pngget.c:1067:1 in png.h
// png_get_pixels_per_meter at pngget.c:167:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
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
#include <png.h>
#include <cstdint>
#include <cstdio>
#include <cstring>

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    abort();
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a temporary file to hold the fuzzed PNG data
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set custom error handlers to prevent crashes
    png_set_error_fn(png_ptr, NULL, dummy_error_fn, dummy_warning_fn);

    // Use setjmp to handle errors gracefully during read
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Open the file for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the signature and IHDR
    png_read_info(png_ptr, info_ptr);

    // Target API 1: png_get_y_pixels_per_inch
    png_uint_32 y_ppi = png_get_y_pixels_per_inch(png_ptr, info_ptr);
    (void)y_ppi;

    // Target API 2: png_get_sCAL_s
    int unit_s = 0;
    char *swidth = NULL;
    char *sheight = NULL;
    png_uint_32 ret_scal_s = png_get_sCAL_s(png_ptr, info_ptr, &unit_s, &swidth, &sheight);
    if (ret_scal_s == PNG_INFO_sCAL) {
        if (swidth) free(swidth);
        if (sheight) free(sheight);
    }

    // Target API 3: png_get_pixels_per_meter
    png_uint_32 ppm = png_get_pixels_per_meter(png_ptr, info_ptr);
    (void)ppm;

    // Target API 4: png_get_sCAL_fixed
    int unit_f = 0;
    png_fixed_point width_f = 0;
    png_fixed_point height_f = 0;
    png_uint_32 ret_scal_fixed = png_get_sCAL_fixed(png_ptr, info_ptr, &unit_f, &width_f, &height_f);
    (void)ret_scal_fixed;

    // Target API 5: png_get_pCAL
    char *purpose = NULL;
    png_int_32 X0 = 0, X1 = 0;
    int type = 0, nparams = 0;
    char *units = NULL;
    char **params = NULL;
    png_uint_32 ret_pcal = png_get_pCAL(png_ptr, info_ptr, &purpose, &X0, &X1, &type, &nparams, &units, &params);
    if (ret_pcal == PNG_INFO_pCAL) {
        if (purpose) free(purpose);
        if (units) free(units);
        if (params) {
            for (int i = 0; i < nparams; i++) {
                if (params[i]) free(params[i]);
            }
            free(params);
        }
    }

    // Target API 6: png_get_IHDR
    png_uint_32 width = 0, height = 0;
    int bit_depth = 0, color_type = 0;
    int interlace_method = 0, compression_method = 0, filter_method = 0;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return 0;
}