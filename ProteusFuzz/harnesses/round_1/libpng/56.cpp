// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_sRGB at pngget.c:699:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_sCAL_s at pngget.c:1067:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_data_freer at png.c:461:1 in png.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void setup_png_structs(png_structp *png_ptr, png_infop *info_ptr, const uint8_t *data, size_t size) {
    *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (*png_ptr == NULL) return;

    *info_ptr = png_create_info_struct(*png_ptr);
    if (*info_ptr == NULL) {
        png_destroy_read_struct(png_ptr, NULL, NULL);
        *png_ptr = NULL;
        return;
    }

    if (setjmp(png_jmpbuf(*png_ptr))) {
        png_destroy_read_struct(png_ptr, info_ptr, NULL);
        *png_ptr = NULL;
        *info_ptr = NULL;
        return;
    }

    // Create a dummy file to simulate reading
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }

    fp = fopen("./dummy_file", "rb");
    if (fp) {
        png_init_io(*png_ptr, fp);
        // Attempt to read the IHDR chunk to populate info_ptr
        // We catch any errors here to allow the fuzz driver to continue
        if (setjmp(png_jmpbuf(*png_ptr))) {
            fclose(fp);
            png_destroy_read_struct(png_ptr, info_ptr, NULL);
            *png_ptr = NULL;
            *info_ptr = NULL;
            return;
        }
        png_read_info(*png_ptr, *info_ptr);
        fclose(fp);
    } else {
        png_destroy_read_struct(png_ptr, info_ptr, NULL);
        *png_ptr = NULL;
        *info_ptr = NULL;
    }
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    setup_png_structs(&png_ptr, &info_ptr, Data, Size);

    if (png_ptr == NULL || info_ptr == NULL) {
        return 0;
    }

    // 1. Test png_data_freer
    // Try both PNG_DESTROY_WILL_FREE_DATA and PNG_USER_WILL_FREE_DATA
    // Mask can be various PNG_INFO_* flags
    png_data_freer(png_ptr, info_ptr, PNG_DESTROY_WILL_FREE_DATA, PNG_INFO_pCAL);
    png_data_freer(png_ptr, info_ptr, PNG_USER_WILL_FREE_DATA, PNG_INFO_sRGB);
    png_data_freer(png_ptr, info_ptr, PNG_DESTROY_WILL_FREE_DATA, PNG_INFO_sCAL);
    // Note: PNG_INFO_IHDR is not a valid mask for png_data_freer as IHDR data is not dynamically allocated in the same way.
    // Using PNG_INFO_tRNS or PNG_INFO_PLTE as alternatives for valid dynamic data masks if needed, 
    // but here we stick to the flags explicitly mentioned or known to be dynamic.
    png_data_freer(png_ptr, info_ptr, PNG_USER_WILL_FREE_DATA, PNG_INFO_tRNS);

    // 2. Test png_get_pCAL
    char *purpose = NULL;
    png_int_32 X0 = 0, X1 = 0;
    int type = 0, nparams = 0;
    char *units = NULL;
    char **params = NULL;
    png_get_pCAL(png_ptr, info_ptr, &purpose, &X0, &X1, &type, &nparams, &units, &params);
    // Cleanup if pCAL data was allocated by libpng and we set freer to destroy
    if (purpose) free(purpose);
    if (units) free(units);
    if (params) {
        for (int i = 0; i < nparams; i++) {
            if (params[i]) free(params[i]);
        }
        free(params);
    }

    // 3. Test png_get_sRGB
    int file_srgb_intent = 0;
    png_get_sRGB(png_ptr, info_ptr, &file_srgb_intent);

    // 4. Test png_get_IHDR
    png_uint_32 width = 0, height = 0;
    int bit_depth = 0, color_type = 0;
    int interlace_method = 0, compression_method = 0, filter_method = 0;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);

    // 5. Test png_get_sCAL_s
    int unit = 0;
    char *swidth = NULL, *sheight = NULL;
    png_get_sCAL_s(png_ptr, info_ptr, &unit, &swidth, &sheight);
    if (swidth) free(swidth);
    if (sheight) free(sheight);

    // 6. Test png_get_valid
    // Check validity of various chunks
    png_get_valid(png_ptr, info_ptr, PNG_INFO_pCAL);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_sRGB);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_sCAL);
    // PNG_INFO_IHDR is not a valid flag for png_get_valid in standard libpng versions (it's always present if read_info succeeded)
    // We use other valid flags.
    png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_PLTE);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_gAMA);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_cHRM);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}