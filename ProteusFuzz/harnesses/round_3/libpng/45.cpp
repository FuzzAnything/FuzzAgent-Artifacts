// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_benign_errors at pngset.c:1994:1 in png.h
// png_set_gamma_fixed at pngrtran.c:859:1 in png.h
// png_set_interlace_handling at pngtrans.c:98:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_set_check_for_invalid_index at pngset.c:2024:1 in png.h
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

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) return;
    if (Size > 0) {
        file.write(reinterpret_cast<const char*>(Data), Size);
    } else {
        // Write minimal valid PNG header if input is empty to avoid immediate failure
        // IHDR chunk: 13 bytes data + 4 bytes length + 4 bytes type + 4 bytes CRC
        // Minimal PNG signature
        const char signature[] = { 
            static_cast<char>(137), static_cast<char>(80), static_cast<char>(78), static_cast<char>(71),
            static_cast<char>(13), static_cast<char>(10), static_cast<char>(26), static_cast<char>(10) 
        };
        file.write(signature, 8);
    }
    file.close();
}

static uint8_t get_byte_safe(const uint8_t *Data, size_t Size, size_t index) {
    if (index < Size) {
        return Data[index];
    }
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input data to a temporary file
    write_dummy_file(Data, Size);

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Setup error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Open the dummy file
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Diverse fuzzing strategies for the target APIs

    // 1. png_set_rgb_to_gray_fixed
    // Fuzz error_action (0, 1, 2) and fixed point weights
    int error_action = get_byte_safe(Data, Size, 0) % 3;
    png_fixed_point red = (png_fixed_point)(get_byte_safe(Data, Size, 1) * 100000 + (get_byte_safe(Data, Size, 2) << 8));
    png_fixed_point green = (png_fixed_point)(get_byte_safe(Data, Size, 3) * 100000 + (get_byte_safe(Data, Size, 4) << 8));
    
    // Ensure weights are positive to avoid immediate app_error if possible, 
    // but fuzzing negative values is also valid to test error handling.
    if (get_byte_safe(Data, Size, 5) & 1) red = -red;
    if (get_byte_safe(Data, Size, 6) & 1) green = -green;

    png_set_rgb_to_gray_fixed(png_ptr, error_action, red, green);
    
    // Check status
    png_byte status = png_get_rgb_to_gray_status(png_ptr);
    (void)status; // Use status to avoid unused warning

    // 2. png_set_check_for_invalid_index
    // Fuzz allowed (0 or 1)
    int allowed = get_byte_safe(Data, Size, 7) % 2;
    png_set_check_for_invalid_index(png_ptr, allowed);

    // 3. png_set_benign_errors
    // Fuzz allowed (0 or 1)
    int benign = get_byte_safe(Data, Size, 8) % 2;
    png_set_benign_errors(png_ptr, benign);

    // 4. png_set_gamma_fixed
    // Fuzz screen_gamma and override_file_gamma
    png_fixed_point screen_gamma = (png_fixed_point)(get_byte_safe(Data, Size, 9) * 100000 + (get_byte_safe(Data, Size, 10) << 8));
    png_fixed_point override_file_gamma = (png_fixed_point)(get_byte_safe(Data, Size, 11) * 100000 + (get_byte_safe(Data, Size, 12) << 8));
    
    // Introduce negative or zero values to test validation
    if (get_byte_safe(Data, Size, 13) & 1) screen_gamma = -screen_gamma;
    if (get_byte_safe(Data, Size, 14) & 1) override_file_gamma = -override_file_gamma;
    if (get_byte_safe(Data, Size, 15) & 1) screen_gamma = 0;
    if (get_byte_safe(Data, Size, 16) & 1) override_file_gamma = 0;

    png_set_gamma_fixed(png_ptr, screen_gamma, override_file_gamma);

    // 5. png_set_interlace_handling
    // Returns number of passes
    int passes = png_set_interlace_handling(png_ptr);
    (void)passes;

    // 6. png_read_png
    // Fuzz transform flags
    int transforms = 0;
    if (Size > 20) {
        transforms = get_byte_safe(Data, Size, 17) | (get_byte_safe(Data, Size, 18) << 8) | (get_byte_safe(Data, Size, 19) << 16) | (get_byte_safe(Data, Size, 20) << 24);
    }
    
    // Attempt to read the PNG
    // Note: png_read_png handles the reading loop internally.
    // It may fail if the file is not a valid PNG or if transforms are invalid.
    png_read_png(png_ptr, info_ptr, transforms, NULL);

    // Cleanup
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}