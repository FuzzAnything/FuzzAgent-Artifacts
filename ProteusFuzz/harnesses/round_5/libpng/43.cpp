// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_user_transform_info at pngtrans.c:805:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_start_read_image at pngread.c:291:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_interlace_handling at pngtrans.c:98:1 in png.h
// png_set_bgr at pngtrans.c:20:1 in png.h
// png_set_option at png.c:3740:1 in png.h
// png_set_option at png.c:3740:1 in png.h
// png_set_option at png.c:3740:1 in png.h
// png_set_option at png.c:3740:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>

static void write_dummy_png(const uint8_t *data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) return;
    if (size > 0) {
        file.write(reinterpret_cast<const char*>(data), size);
    } else {
        // Write a minimal valid PNG header if data is empty to ensure file exists
        const uint8_t header[] = {
            0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A,
            0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52,
            0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
            0x08, 0x02, 0x00, 0x00, 0x00, 0x90, 0x77, 0x53,
            0xDE, 0x00, 0x00, 0x00, 0x0C, 0x49, 0x44, 0x41,
            0x54, 0x08, 0xD7, 0x63, 0xF8, 0xFF, 0xFF, 0x3F,
            0x00, 0x05, 0xFE, 0x02, 0xFE, 0xDC, 0xCC, 0x59,
            0xE7, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E,
            0x44, 0xAE, 0x42, 0x60, 0x82
        };
        file.write(reinterpret_cast<const char*>(header), sizeof(header));
    }
    file.close();
}

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file for reading
    write_dummy_png(Data, Size);

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Use setjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Open the dummy file
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Fuzzing sequence based on API constraints

    // 1. png_set_interlace_handling: Must be called before png_read_update_info
    // We try calling it here. It returns number of passes.
    int passes = png_set_interlace_handling(png_ptr);
    (void)passes; // Suppress unused warning

    // 2. png_set_bgr: Sets BGR flag. Can be called after struct creation.
    png_set_bgr(png_ptr);

    // 3. png_set_option: Configures behavior flags.
    // Try a few valid and invalid options to explore states.
    int opt_result = png_set_option(png_ptr, 0, 1); // Valid even index
    (void)opt_result;
    opt_result = png_set_option(png_ptr, 1, 1); // Invalid odd index
    (void)opt_result;
    opt_result = png_set_option(png_ptr, 100, 0); // Out of range
    (void)opt_result;
    opt_result = png_set_option(nullptr, 0, 1); // Null pointer test
    (void)opt_result;

    // 4. png_set_user_transform_info: Sets custom transform params.
    // Must be called before png_start_read_image or png_read_update_info.
    // We call it here before reading info.
    void *user_ptr = const_cast<void*>(reinterpret_cast<const void*>(Data));
    int depth = (Size % 16); // 0-15
    int channels = (Size % 4) + 1; // 1-4
    png_set_user_transform_info(png_ptr, user_ptr, depth, channels);

    // 5. Read the header and info
    // We need to read the info to proceed to row processing
    png_read_info(png_ptr, info_ptr);

    // 6. png_set_rgb_to_gray: Convert RGB to grayscale.
    // Must be called after png_read_info but before row processing.
    // We use the fixed point version to avoid floating point issues in fuzzing.
    // Coefficients are derived from input data.
    png_fixed_point red_coeff = (png_fixed_point)((Data[0] * 100000));
    png_fixed_point green_coeff = (png_fixed_point)((Data[1] * 100000));
    int error_action = (Size % 2); // 0 or 1
    png_set_rgb_to_gray_fixed(png_ptr, error_action, red_coeff, green_coeff);

    // Check status
    png_byte status = png_get_rgb_to_gray_status(png_ptr);
    (void)status;

    // 7. png_start_read_image: Initialize row processing.
    // Must be called after png_read_update_info.
    // Note: png_read_update_info is typically called after transformations.
    // We call it now to ensure the state is updated before starting image read.
    png_read_update_info(png_ptr, info_ptr);
    png_start_read_image(png_ptr);

    // Attempt to read a single row to exercise the logic further
    // Allocate a row buffer
    png_bytep row = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
    if (row) {
        png_read_row(png_ptr, row, nullptr);
        png_free(png_ptr, row);
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);
    std::remove("./dummy_file");

    return 0;
}