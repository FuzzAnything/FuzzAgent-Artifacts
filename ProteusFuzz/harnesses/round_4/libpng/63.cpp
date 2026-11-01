// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
// png_set_hIST at pngset.c:375:1 in png.h
// png_set_gAMA at pngset.c:365:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA at pngget.c:677:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
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
#include <stddef.h>
#include <cstring>
#include <cstdio>

static void write_dummy_png(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    if (size > 0) {
        fwrite(data, 1, size, f);
    } else {
        // Write a minimal valid PNG signature if no data provided
        unsigned char signature[] = {137, 80, 78, 71, 13, 10, 26, 10};
        fwrite(signature, 1, 8, f);
    }
    fclose(f);
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for reading
    write_dummy_png(Data, Size);

    // --- Test Reading Scenario ---
    png_structp png_read_ptr = NULL;
    png_infop info_read_ptr = NULL;
    FILE *fp_read = fopen("./dummy_file", "rb");

    if (fp_read) {
        // Initialize read structures
        png_read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_read_ptr) {
            fclose(fp_read);
            return 0;
        }

        info_read_ptr = png_create_info_struct(png_read_ptr);
        if (!info_read_ptr) {
            png_destroy_read_struct(&png_read_ptr, NULL, NULL);
            fclose(fp_read);
            return 0;
        }

        // Set error handling to prevent crashes during fuzzing
        if (setjmp(png_jmpbuf(png_read_ptr))) {
            png_destroy_read_struct(&png_read_ptr, &info_read_ptr, NULL);
            fclose(fp_read);
            return 0;
        }

        png_init_io(png_read_ptr, fp_read);

        // Attempt to read info (may fail on malformed input, handled by setjmp)
        png_read_info(png_read_ptr, info_read_ptr);

        // 1. png_get_gAMA
        double file_gamma;
        png_get_gAMA(png_read_ptr, info_read_ptr, &file_gamma);
        png_fixed_point int_file_gamma;
        png_get_gAMA_fixed(png_read_ptr, info_read_ptr, &int_file_gamma);

        // 2. png_get_hIST
        png_uint_16 *hist = NULL;
        png_get_hIST(png_read_ptr, info_read_ptr, &hist);

        // 6. png_set_invalid (Test with various masks)
        png_set_invalid(png_read_ptr, info_read_ptr, PNG_INFO_gAMA);
        png_set_invalid(png_read_ptr, info_read_ptr, PNG_INFO_hIST);
        png_set_invalid(png_read_ptr, info_read_ptr, 0);

        // Cleanup Read
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, NULL);
        fclose(fp_read);
    }

    // --- Test Writing Scenario ---
    png_structp png_write_ptr = NULL;
    png_infop info_write_ptr = NULL;
    FILE *fp_write = fopen("./dummy_file", "wb");

    if (fp_write) {
        // Initialize write structures
        png_write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_write_ptr) {
            fclose(fp_write);
            return 0;
        }

        info_write_ptr = png_create_info_struct(png_write_ptr);
        if (!info_write_ptr) {
            png_destroy_write_struct(&png_write_ptr, NULL);
            fclose(fp_write);
            return 0;
        }

        if (setjmp(png_jmpbuf(png_write_ptr))) {
            png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
            fclose(fp_write);
            return 0;
        }

        png_init_io(png_write_ptr, fp_write);

        // Set basic IHDR info required for writing
        png_set_IHDR(png_write_ptr, info_write_ptr, 1, 1, 8, PNG_COLOR_TYPE_PALETTE,
                     PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

        // 3. png_write_info_before_PLTE
        png_write_info_before_PLTE(png_write_ptr, info_write_ptr);

        // 4. png_set_hIST
        png_uint_16 dummy_hist[256];
        for (int i = 0; i < 256; ++i) {
            dummy_hist[i] = (png_uint_16)(i * 255);
        }
        png_set_hIST(png_write_ptr, info_write_ptr, dummy_hist);

        // 1. png_set_gAMA
        png_set_gAMA(png_write_ptr, info_write_ptr, 2.2);
        png_set_gAMA_fixed(png_write_ptr, info_write_ptr, 220000);

        // 6. png_set_invalid
        png_set_invalid(png_write_ptr, info_write_ptr, PNG_INFO_hIST);

        // Cleanup Write
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        fclose(fp_write);
    }

    // --- Test Destruction Logic ---
    png_structp png_test_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_test_ptr = png_create_info_struct(png_test_ptr);
    
    if (png_test_ptr && info_test_ptr) {
        // 5. png_destroy_info_struct
        png_destroy_info_struct(png_test_ptr, &info_test_ptr);
        // info_test_ptr should be NULL now
        
        // Cleanup remaining struct
        png_destroy_read_struct(&png_test_ptr, NULL, NULL);
    }

    return 0;
}