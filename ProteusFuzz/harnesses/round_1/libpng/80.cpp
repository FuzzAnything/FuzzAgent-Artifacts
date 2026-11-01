// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fuzzer/FuzzedDataProvider.h>

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    } else if (info_ptr) {
        // If png_ptr is null but info_ptr is not, we can't safely destroy info_ptr
        // as it relies on png_ptr. This is a fallback for malformed states.
        // In a real scenario, we ensure png_ptr is valid before creating info_ptr.
        free(info_ptr); 
    }
}

extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    FuzzedDataProvider data_provider(Data, Size);

    // Create a dummy file to read from
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
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

    // Use setjmp/longjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Open the dummy file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Read the PNG header and info
    // We use a small number of bytes to read to avoid heavy processing, 
    // focusing on the info retrieval functions.
    // We try to read the IHDR and potentially other chunks.
    // To ensure we have some data, we attempt to read the info.
    // Note: If the data is not a valid PNG, this will likely jump.
    png_read_info(png_ptr, info_ptr);
    fclose(fp);

    // --- Fuzzing Target API Functions ---

    // 1. png_get_hIST
    png_uint_16 *hist = NULL;
    png_uint_32 hist_result = png_get_hIST(png_ptr, info_ptr, &hist);
    (void)hist_result; // Suppress unused variable warning
    // hist is only valid if result is non-zero, but we don't need to use it for fuzzing logic here.

    // 2. png_get_tIME
    png_timep mod_time = NULL;
    png_uint_32 time_result = png_get_tIME(png_ptr, info_ptr, &mod_time);
    (void)time_result;

    // 3. png_get_gAMA_fixed
    png_fixed_point file_gamma = 0;
    png_uint_32 gamma_result = png_get_gAMA_fixed(png_ptr, info_ptr, &file_gamma);
    (void)gamma_result;

    // 4. png_get_image_width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    (void)width;

    // 5. png_get_x_pixels_per_meter
    png_uint_32 x_pmm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)x_pmm;

    // 6. png_get_uint_16
    // We need a buffer of at least 2 bytes.
    if (Size >= 2) {
        png_byte buf[2];
        buf[0] = Data[0];
        buf[1] = Data[1];
        png_uint_16 val = png_get_uint_16(buf);
        (void)val;
    }

    // Cleanup
    cleanup_png(png_ptr, info_ptr);
    remove(filename);

    return 0;
}