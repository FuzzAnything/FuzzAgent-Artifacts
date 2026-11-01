// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_y_pixels_per_inch at pngget.c:369:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_pixels_per_meter at pngget.c:167:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
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
#include <png.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void* safe_malloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) exit(1);
    return ptr;
}

static void safe_free(void* ptr) {
    if (ptr) free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a temporary file to simulate input
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Open the file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the PNG header and info
    // We do not call png_read_info here to allow fuzzing of malformed data
    // but we must call it to populate info_ptr for the target functions to work on valid data.
    // However, for fuzzing, we want to test both valid and invalid states.
    // We will try to read info, and if it fails, we still try the API calls to test error handling.
    png_set_sig_bytes(png_ptr, 0);

    // Try to read info; if it fails, the info_ptr might be in an inconsistent state
    // which is also a valid fuzzing scenario for these getters.
    png_read_info(png_ptr, info_ptr);

    // Variables for output parameters
    png_uint_32 ppm;
    png_int_32 offset_x, offset_y;
    int unit_type;
    int unit_s;
    char *swidth = NULL, *sheight = NULL;
    char *purpose = NULL;
    png_int_32 X0, X1;
    int type, nparams;
    char *units = NULL;
    char **params = NULL;
    png_uint_32 ppi;
    png_fixed_point width_fp, height_fp;

    // 1. png_get_pixels_per_meter
    ppm = png_get_pixels_per_meter(png_ptr, info_ptr);
    (void)ppm;

    // 2. png_get_oFFs
    png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, &unit_type);

    // 3. png_get_sCAL_s
    png_get_sCAL_s(png_ptr, info_ptr, &unit_s, &swidth, &sheight);
    if (swidth) safe_free(swidth);
    if (sheight) safe_free(sheight);

    // 4. png_get_pCAL
    png_get_pCAL(png_ptr, info_ptr, &purpose, &X0, &X1, &type, &nparams, &units, &params);
    if (purpose) safe_free(purpose);
    if (units) safe_free(units);
    if (params) {
        // pCAL params are an array of strings
        for (int i = 0; i < nparams; i++) {
            if (params[i]) safe_free(params[i]);
        }
        safe_free(params);
    }

    // 5. png_get_y_pixels_per_inch
    ppi = png_get_y_pixels_per_inch(png_ptr, info_ptr);
    (void)ppi;

    // 6. png_get_sCAL_fixed
    // Only call if fixed point is supported (usually always true in modern builds)
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit_s, &width_fp, &height_fp);

    // Cleanup
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    // Remove dummy file
    remove(filename);

    return 0;
}