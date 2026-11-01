// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
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

extern "C" int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file to simulate reading a PNG
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Use setjmp for error handling as required by libpng
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Re-open file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the PNG header and info
    // We do not call png_read_png to avoid full image decoding, 
    // focusing on the info structure population.
    // However, to populate info_ptr with chunks, we must read at least the header.
    // If the data is not a valid PNG, this will trigger the setjmp handler.
    png_read_info(png_ptr, info_ptr);

    fclose(fp);

    // --- Fuzzing Target API Functions ---

    // 1. png_get_gAMA_fixed
    png_fixed_point int_file_gamma = 0;
    png_uint_32 gama_result = png_get_gAMA_fixed(png_ptr, info_ptr, &int_file_gamma);
    (void)gama_result; // Suppress unused variable warning

    // 2. png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)x_ppm;

    // 3. png_get_image_width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    (void)width;

    // 4. png_get_valid
    // Check various common flags
    png_uint_32 valid_gama = png_get_valid(png_ptr, info_ptr, PNG_INFO_gAMA);
    png_uint_32 valid_phys = png_get_valid(png_ptr, info_ptr, PNG_INFO_pHYs);
    png_uint_32 valid_hist = png_get_valid(png_ptr, info_ptr, PNG_INFO_hIST);
    png_uint_32 valid_trns = png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);
    (void)valid_gama; (void)valid_phys; (void)valid_hist; (void)valid_trns;

    // 5. png_get_uint_16
    // Use the fuzzing data buffer directly if size permits
    if (Size >= 2) {
        const png_byte *buf = Data;
        png_uint_16 val = png_get_uint_16(buf);
        (void)val;
    }

    // 6. png_get_hIST
    png_uint_16 *hist = NULL;
    png_uint_32 hist_result = png_get_hIST(png_ptr, info_ptr, &hist);
    (void)hist_result;
    (void)hist;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    // Remove dummy file
    remove(filename);

    return 0;
}