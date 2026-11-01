// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_sBIT at pngget.c:1140:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
// png_set_shift at pngtrans.c:83:1 in png.h
// png_set_sBIT at pngset.c:804:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
// png_set_sBIT at pngset.c:804:1 in png.h
// png_set_shift at pngtrans.c:83:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
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

static void cleanup_png(png_structp png_ptr, png_infop info_ptr, png_bytep *row_pointers, int height) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    } else if (info_ptr) {
        png_destroy_info_struct(NULL, &info_ptr);
    }
    
    if (row_pointers && height > 0) {
        for (int i = 0; i < height; ++i) {
            free(row_pointers[i]);
        }
        free(row_pointers);
    }
}

extern "C" int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    FuzzedDataProvider fdp(Data, Size);

    // Create a dummy file if needed (though we'll use memory buffers here)
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Use setjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr, NULL, 0);
        return 0;
    }

    // Try to read from the dummy file
    fp = fopen(dummy_file, "rb");
    if (fp) {
        png_init_io(png_ptr, fp);

        // Attempt to read the PNG header and info
        png_read_info(png_ptr, info_ptr);
        
        // Get image dimensions
        png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
        png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
        
        // Limit dimensions to prevent excessive memory usage
        if (width > 10000 || height > 10000) {
            fclose(fp);
            cleanup_png(png_ptr, info_ptr, NULL, 0);
            return 0;
        }

        // Allocate row pointers
        png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
        if (!row_pointers) {
            fclose(fp);
            cleanup_png(png_ptr, info_ptr, NULL, 0);
            return 0;
        }
        for (png_uint_32 y = 0; y < height; ++y) {
            row_pointers[y] = (png_bytep)malloc(png_get_rowbytes(png_ptr, info_ptr));
            if (!row_pointers[y]) {
                for (png_uint_32 i = 0; i < y; ++i) free(row_pointers[i]);
                free(row_pointers);
                fclose(fp);
                cleanup_png(png_ptr, info_ptr, NULL, 0);
                return 0;
            }
        }

        // Fuzz png_get_sBIT
        png_color_8 sig_bit;
        png_color_8p sig_bit_ptr = &sig_bit;
        png_uint_32 sbit_result = png_get_sBIT(png_ptr, info_ptr, &sig_bit_ptr);
        (void)sbit_result; // Suppress unused variable warning

        // Fuzz png_get_bit_depth
        png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        (void)bit_depth;

        // Fuzz png_get_channels
        png_byte channels = png_get_channels(png_ptr, info_ptr);
        (void)channels;

        // Fuzz png_set_shift
        png_color_8 true_bits;
        true_bits.red = fdp.ConsumeIntegralInRange<uint8_t>(0, 16);
        true_bits.green = fdp.ConsumeIntegralInRange<uint8_t>(0, 16);
        true_bits.blue = fdp.ConsumeIntegralInRange<uint8_t>(0, 16);
        true_bits.alpha = fdp.ConsumeIntegralInRange<uint8_t>(0, 16);
        png_set_shift(png_ptr, &true_bits);

        // Fuzz png_set_sBIT
        png_color_8 sig_bit_set;
        sig_bit_set.red = fdp.ConsumeIntegralInRange<uint8_t>(0, 16);
        sig_bit_set.green = fdp.ConsumeIntegralInRange<uint8_t>(0, 16);
        sig_bit_set.blue = fdp.ConsumeIntegralInRange<uint8_t>(0, 16);
        sig_bit_set.alpha = fdp.ConsumeIntegralInRange<uint8_t>(0, 16);
        png_set_sBIT(png_ptr, info_ptr, &sig_bit_set);

        // Fuzz png_read_png
        int transforms = fdp.ConsumeIntegralInRange<int>(0, 255);
        png_read_png(png_ptr, info_ptr, transforms, NULL);

        // Cleanup row pointers
        for (png_uint_32 y = 0; y < height; ++y) {
            free(row_pointers[y]);
        }
        free(row_pointers);
        fclose(fp);
    } else {
        // If file couldn't be opened, still test the setters/getters with minimal setup
        png_color_8 sig_bit_set;
        sig_bit_set.red = fdp.ConsumeIntegralInRange<uint8_t>(0, 16);
        sig_bit_set.green = fdp.ConsumeIntegralInRange<uint8_t>(0, 16);
        sig_bit_set.blue = fdp.ConsumeIntegralInRange<uint8_t>(0, 16);
        sig_bit_set.alpha = fdp.ConsumeIntegralInRange<uint8_t>(0, 16);
        png_set_sBIT(png_ptr, info_ptr, &sig_bit_set);

        png_color_8 true_bits;
        true_bits.red = fdp.ConsumeIntegralInRange<uint8_t>(0, 16);
        true_bits.green = fdp.ConsumeIntegralInRange<uint8_t>(0, 16);
        true_bits.blue = fdp.ConsumeIntegralInRange<uint8_t>(0, 16);
        true_bits.alpha = fdp.ConsumeIntegralInRange<uint8_t>(0, 16);
        png_set_shift(png_ptr, &true_bits);
    }

    cleanup_png(png_ptr, info_ptr, NULL, 0);
    return 0;
}