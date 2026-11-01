// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_hIST at pngget.c:911:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
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
#include <cstddef>
#include <cstdio>

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    } else if (info_ptr) {
        // If png_ptr is null but info_ptr exists, we can't safely destroy it via libpng
        // as it expects a valid png_ptr. This scenario should ideally not happen in valid usage.
    }
}

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file to simulate a PNG file for the fuzzer
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Use setjmp for error handling as required by libpng
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

    // Attempt to read the PNG information
    // We do not call png_read_info here to allow fuzzing of invalid/malformed data
    // directly in the getter functions, but we need to ensure the structure is set up.
    // However, most getters require info to be read. 
    // To maximize state exploration, we try to read info, and if it fails, 
    // we still call the functions to test their NULL/error handling paths.
    
    png_uint_32 valid_flags = 0;
    
    // Try to read the info. If this fails, png_ptr and info_ptr might be in an inconsistent state,
    // which is exactly what we want to test for robustness.
    png_read_info(png_ptr, info_ptr);

    // 1. Test png_get_gAMA_fixed
    png_fixed_point gamma_value = 0;
    png_uint_32 gamma_ret = png_get_gAMA_fixed(png_ptr, info_ptr, &gamma_value);
    (void)gamma_ret;
    (void)gamma_value;

    // Test with NULL pointer
    png_get_gAMA_fixed(png_ptr, info_ptr, nullptr);

    // 2. Test png_get_valid
    // Check various flags
    valid_flags = png_get_valid(png_ptr, info_ptr, PNG_INFO_gAMA);
    valid_flags = png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);
    valid_flags = png_get_valid(png_ptr, info_ptr, PNG_INFO_hIST);
    valid_flags = png_get_valid(png_ptr, info_ptr, PNG_INFO_pHYs);
    valid_flags = png_get_valid(png_ptr, info_ptr, PNG_INFO_IDAT);
    (void)valid_flags;

    // 3. Test png_get_x_pixels_per_meter
    png_uint_32 xppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)xppm;

    // 4. Test png_get_hIST
    png_uint_16 *hist = nullptr;
    png_uint_32 hist_ret = png_get_hIST(png_ptr, info_ptr, &hist);
    (void)hist_ret;
    (void)hist;

    // 5. Test png_get_image_width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    (void)width;

    // 6. Test png_get_uint_16
    // Create a small buffer from the input data if available
    png_uint_16 val_16 = 0;
    if (Size >= 2) {
        val_16 = png_get_uint_16(Data);
    } else if (Size == 1) {
        // Edge case: only 1 byte, though png_get_uint_16 expects 2.
        // We pass the pointer anyway to see how it handles it (undefined behavior in real code, 
        // but good for fuzzing if the library has guards).
        // However, to be safe and avoid segfaults on strict sanitizers, we only call if size >= 2.
        // If we must test, we could construct a 2-byte buffer.
        png_byte buf[2];
        buf[0] = Data[0];
        buf[1] = 0; 
        val_16 = png_get_uint_16(buf);
    }
    (void)val_16;

    // Cleanup
    fclose(fp);
    cleanup_png(png_ptr, info_ptr);

    return 0;
}