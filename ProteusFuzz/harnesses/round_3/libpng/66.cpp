// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_rows at pngset.c:1845:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
// png_get_sBIT at pngget.c:1140:1 in png.h
// png_set_sBIT at pngset.c:804:1 in png.h
// png_set_shift at pngtrans.c:83:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
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
#include <setjmp.h>

// Helper to create a dummy file and return a file pointer
static FILE* create_dummy_file(const uint8_t *data, size_t size) {
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) return nullptr;
    fwrite(data, 1, size, fp);
    fclose(fp);

    fp = fopen(filename, "rb");
    return fp;
}

static void cleanup_png(png_structp *png_ptr, png_infop *info_ptr) {
    if (*png_ptr) {
        png_destroy_read_struct(png_ptr, info_ptr, nullptr);
        *png_ptr = nullptr;
    }
    if (*info_ptr) {
        png_destroy_info_struct(*png_ptr, info_ptr);
        *info_ptr = nullptr;
    }
}

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    png_color_8 sig_bit = {0, 0, 0};
    png_color_8 true_bits = {8, 8, 8};
    png_byte channels = 0;
    png_uint_32 sbit_flag = 0;
    png_color_8p sig_bit_ptr = nullptr;

    // Create PNG struct and info
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling to prevent crashes
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        cleanup_png(&png_ptr, &info_ptr);
        return 0;
    }

    // Create dummy file and initialize I/O
    FILE *fp = create_dummy_file(Data, Size);
    if (!fp) {
        cleanup_png(&png_ptr, &info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Fuzz png_read_info
    png_read_info(png_ptr, info_ptr);

    // Fuzz png_get_channels
    channels = png_get_channels(png_ptr, info_ptr);

    // Fuzz png_get_sBIT
    // png_get_sBIT expects png_color_8p * (pointer to pointer)
    sig_bit_ptr = &sig_bit;
    sbit_flag = png_get_sBIT(png_ptr, info_ptr, &sig_bit_ptr);

    // Fuzz png_set_sBIT
    png_set_sBIT(png_ptr, info_ptr, &sig_bit);

    // Fuzz png_set_shift
    png_set_shift(png_ptr, &true_bits);

    // Fuzz png_read_png
    // Prepare row pointers for png_read_png
    png_bytep *row_pointers = nullptr;
    png_uint_32 height = 0;
    png_uint_32 width = 0;
    int color_type = 0;

    png_get_IHDR(png_ptr, info_ptr, &width, &height, nullptr, &color_type, nullptr, nullptr, nullptr);

    if (height > 0 && width > 0 && height * width < 10000000) { // Limit memory usage
        row_pointers = (png_bytep *)malloc(height * sizeof(png_bytep));
        if (row_pointers) {
            for (png_uint_32 i = 0; i < height; ++i) {
                row_pointers[i] = (png_byte *)malloc(width * 4); // Assume max 4 bytes per pixel
                if (!row_pointers[i]) {
                    for (png_uint_32 j = 0; j < i; ++j) {
                        free(row_pointers[j]);
                    }
                    free(row_pointers);
                    row_pointers = nullptr;
                    break;
                }
            }
        }

        if (row_pointers) {
            // Set row pointers in info_ptr
            // png_set_rows expects png_byte **, so we pass row_pointers directly
            png_set_rows(png_ptr, info_ptr, row_pointers);

            // Call png_read_png
            png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);

            // Cleanup row pointers
            for (png_uint_32 i = 0; i < height; ++i) {
                free(row_pointers[i]);
            }
            free(row_pointers);
        }
    }

    // Cleanup
    fclose(fp);
    remove("./dummy_file");
    cleanup_png(&png_ptr, &info_ptr);

    return 0;
}