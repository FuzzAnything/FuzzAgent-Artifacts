// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_cLLI_fixed at pngget.c:788:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_cLLI_fixed at pngset.c:162:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
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
#include <cstdio>

static void
fuzzer_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Suppress errors during fuzzing to prevent aborts
}

static void
fuzzer_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Suppress warnings during fuzzing
}

static int
read_png_data_from_fuzz_input(png_structp png_ptr, png_bytep data, png_size_t length) {
    // This function is a placeholder. In a real scenario, we would read from the fuzz input.
    // However, libpng expects a file-based IO or a custom callback.
    // Since we are using a dummy file approach, we rely on standard fread.
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file to hold the fuzz data
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    if (fwrite(Data, 1, Size, fp) != Size) {
        fclose(fp);
        return 0;
    }
    fclose(fp);

    // Re-open for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        return 0;
    }

    // Create PNG read struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    // Set error handlers to suppress crashes
    png_set_error_fn(png_ptr, NULL, fuzzer_error_callback, fuzzer_warning_callback);

    // Create PNG info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, fp);

    // Use setjmp to catch errors during read
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // Fuzz png_get_gAMA_fixed
    png_fixed_point gAMA_value;
    png_uint_32 gAMA_result = png_get_gAMA_fixed(png_ptr, info_ptr, &gAMA_value);
    (void)gAMA_result;
    (void)gAMA_value;

    // Fuzz png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)x_ppm;

    // Fuzz png_get_sCAL_fixed
    int unit;
    png_fixed_point width, height;
    png_uint_32 scal_result = png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width, &height);
    (void)scal_result;
    (void)unit;
    (void)width;
    (void)height;

    // Fuzz png_get_cLLI_fixed
    png_uint_32 maxCLL, maxFALL;
    png_uint_32 cLLI_result = png_get_cLLI_fixed(png_ptr, info_ptr, &maxCLL, &maxFALL);
    (void)cLLI_result;
    (void)maxCLL;
    (void)maxFALL;

    // Fuzz png_set_cLLI_fixed with some values derived from input
    png_uint_32 set_maxCLL = (Size > 0) ? (png_uint_32)(Data[0] * 10000) : 0;
    png_uint_32 set_maxFALL = (Size > 1) ? (png_uint_32)(Data[1] * 10000) : 0;
    // Ensure values are within reasonable limits to avoid immediate errors if possible,
    // though the API handles errors via setjmp.
    if (set_maxCLL > 200000) set_maxCLL = 200000;
    if (set_maxFALL > 200000) set_maxFALL = 200000;
    
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred in set_cLLI_fixed, ignore and continue cleanup
    } else {
        png_set_cLLI_fixed(png_ptr, info_ptr, set_maxCLL, set_maxFALL);
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    
    // Remove dummy file
    remove(filename);

    return 0;
}