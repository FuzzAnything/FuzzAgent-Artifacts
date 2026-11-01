// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_alpha_mode at pngrtran.c:444:1 in png.h
// png_set_gamma at pngrtran.c:901:1 in png.h
// png_set_background at pngrtran.c:155:1 in png.h
// png_set_tRNS_to_alpha at pngrtran.c:969:1 in png.h
// png_set_scale_16 at pngrtran.c:171:1 in png.h
// png_set_flush at pngwrite.c:966:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_expand_16 at pngrtran.c:985:1 in png.h
// png_set_rgb_to_gray at pngrtran.c:1085:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include <vector>

static void write_dummy_png_file(const char* filename, const uint8_t* data, size_t size) {
    FILE* f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    } else if (info_ptr) {
        png_destroy_read_struct(nullptr, &info_ptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file with the fuzzed data
    const char* dummy_file = "./dummy_file";
    write_dummy_png_file(dummy_file, Data, Size);

    FILE* fp = fopen(dummy_file, "rb");
    if (!fp) return 0;

    // Initialize libpng read structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return 0;
    }

    // Use setjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        cleanup_png(png_ptr, info_ptr);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read header (IHDR)
    png_read_info(png_ptr, info_ptr);

    // Extract some values from input data to drive fuzzing logic
    int error_action = (Size > 0) ? (Data[0] % 2) : 0; // 0 or 1
    double red_coeff = (Size > 1) ? ((double)(Data[1] % 100) / 100.0) : 0.33;
    double green_coeff = (Size > 2) ? ((double)(Data[2] % 100) / 100.0) : 0.33;
    double screen_gamma = (Size > 3) ? ((double)(Data[3] % 200) / 100.0) : 2.2;
    double file_gamma = (Size > 4) ? ((double)(Data[4] % 200) / 100.0) : 1.0;
    double output_gamma = (Size > 5) ? ((double)(Data[5] % 200) / 100.0) : -1.0; // PNG_DEFAULT_sRGB
    int alpha_mode = (Size > 6) ? (Data[6] % 4) : 0; // PNG_ALPHA_PNG, etc.
    int bg_gamma_code = (Size > 7) ? (Data[7] % 4) : 0;
    int need_expand = (Size > 8) ? (Data[8] % 2) : 0;
    double bg_gamma = (Size > 9) ? ((double)(Data[9] % 200) / 100.0) : 2.2;

    // Prepare background color
    png_color_16 background_color = {0, 0, 0, 0};
    if (Size > 10) background_color.red = Data[10];
    if (Size > 11) background_color.green = Data[11];
    if (Size > 12) background_color.blue = Data[12];

    // 1. png_set_expand_16
    // Can be called after png_create_read_struct, before png_read_image.
    // It forces palette to RGB and expands tRNS.
    png_set_expand_16(png_ptr);

    // 2. png_set_rgb_to_gray
    // Requires png_read_info to be called first (done above).
    // Converts RGB to grayscale.
    png_set_rgb_to_gray(png_ptr, error_action, red_coeff, green_coeff);
    
    // Check status
    png_byte status = png_get_rgb_to_gray_status(png_ptr);
    (void)status; // Suppress unused warning

    // 3. png_set_alpha_mode
    // Configures alpha handling and gamma.
    png_set_alpha_mode(png_ptr, alpha_mode, output_gamma);

    // 4. png_set_gamma
    // Configures gamma correction.
    png_set_gamma(png_ptr, screen_gamma, file_gamma);

    // 5. png_set_background
    // Replaces transparent pixels with background color.
    png_set_background(png_ptr, &background_color, bg_gamma_code, need_expand, bg_gamma);

    // 6. png_set_tRNS_to_alpha
    // Converts tRNS chunk to full alpha channel.
    png_set_tRNS_to_alpha(png_ptr);

    // Additional exploration: png_set_scale_16
    png_set_scale_16(png_ptr);

    // Additional exploration: png_set_flush
    int nrows = (Size > 13) ? (Data[13] % 100) : 0;
    png_set_flush(png_ptr, nrows);

    // Attempt to read the image to trigger actual processing
    // We don't need to store the image, just trigger the logic
    png_bytep* row_pointers = nullptr;
    png_uint_32 height = 0;
    png_uint_32 width = 0;
    
    // Get dimensions safely using correct types
    if (png_get_IHDR(png_ptr, info_ptr, &width, &height, nullptr, nullptr, nullptr, nullptr, nullptr) != 0) {
        if (height > 0 && width > 0 && height < 1000 && width < 1000) {
            row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
            if (row_pointers) {
                for (png_uint_32 i = 0; i < height; ++i) {
                    row_pointers[i] = (png_bytep)malloc(png_get_rowbytes(png_ptr, info_ptr));
                }
                png_read_image(png_ptr, row_pointers);
                
                for (png_uint_32 i = 0; i < height; ++i) {
                    free(row_pointers[i]);
                }
                free(row_pointers);
            }
        }
    }

    // Cleanup
    png_read_end(png_ptr, nullptr);
    cleanup_png(png_ptr, info_ptr);
    fclose(fp);

    return 0;
}