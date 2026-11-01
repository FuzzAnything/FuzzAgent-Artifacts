// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_valid at pngget.c:19:1 in png.h
// png_set_mDCV at pngset.c:297:1 in png.h
// png_get_cLLI at pngget.c:808:1 in png.h
// png_set_cHRM at pngset.c:99:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_mDCV at pngget.c:860:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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

static void dummy_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Silently ignore errors to prevent fuzzer from aborting
}

static void dummy_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Silently ignore warnings
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file to simulate file I/O if needed, though we mostly use memory
    // libpng functions tested here operate on structs, not directly on files for these specific APIs
    // but we need valid png_struct and png_info initialized.
    
    // Initialize png_struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_handler, dummy_warning_handler);
    if (!png_ptr) {
        return 0;
    }

    // Initialize png_info
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling to prevent crashes during fuzzing
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // We do not need to read a real file for these specific getter/setter functions
    // as they manipulate the info_ptr struct directly.
    // However, to ensure the struct is in a "read" state if required by some internal logic,
    // we can pretend we are in the middle of processing, but strictly speaking,
    // png_set_* functions work on the struct regardless of read/write state usually,
    // and png_get_* check flags set by png_set_* or by reading.
    
    // Fuzzing Strategy:
    // 1. Randomly select which API to call based on input data.
    // 2. Generate random parameters from the input buffer.
    // 3. Call the API.
    // 4. Handle potential exceptions via setjmp/longjmp (already set up).

    uint8_t op = Data[0] % 6;

    // Helper to extract doubles from buffer safely
    auto get_double = [&](size_t idx) -> double {
        if (idx + 8 > Size) return 0.0;
        double val;
        memcpy(&val, &Data[idx], sizeof(double));
        return val;
    };

    // Helper to extract fixed_point (int) from buffer
    auto get_fixed = [&](size_t idx) -> png_fixed_point {
        if (idx + 4 > Size) return 0;
        int val;
        memcpy(&val, &Data[idx], sizeof(int));
        return (png_fixed_point)val;
    };

    // Helper to extract uint32
    auto get_uint32 = [&](size_t idx) -> png_uint_32 {
        if (idx + 4 > Size) return 0;
        uint32_t val;
        memcpy(&val, &Data[idx], sizeof(uint32_t));
        return val;
    };

    // Helper to extract int32
    auto get_int32 = [&](size_t idx) -> png_int_32 {
        if (idx + 4 > Size) return 0;
        int32_t val;
        memcpy(&val, &Data[idx], sizeof(int32_t));
        return val;
    };

    // Helper to extract byte
    auto get_byte = [&](size_t idx) -> png_byte {
        if (idx >= Size) return 0;
        return Data[idx];
    };

    switch (op) {
        case 0: {
            // png_get_valid
            // png_get_valid(png_const_structrp png_ptr, png_const_inforp info_ptr, png_uint_32 flag)
            png_uint_32 flag = get_uint32(1);
            png_uint_32 result = png_get_valid(png_ptr, info_ptr, flag);
            (void)result; // Use result to avoid unused variable warnings
            break;
        }
        case 1: {
            // png_set_mDCV (double version)
            // png_set_mDCV(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y, max_lum, min_lum)
            double white_x = get_double(1);
            double white_y = get_double(9);
            double red_x = get_double(17);
            double red_y = get_double(25);
            double green_x = get_double(33);
            double green_y = get_double(41);
            double blue_x = get_double(49);
            double blue_y = get_double(57);
            double max_lum = get_double(65);
            double min_lum = get_double(73);
            
            // Ensure we have enough data, otherwise skip or use defaults
            if (Size >= 81) {
                png_set_mDCV(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y, max_lum, min_lum);
            }
            break;
        }
        case 2: {
            // png_get_cLLI
            // png_get_cLLI(png_ptr, info_ptr, max_content, max_frame_avg)
            double max_content = 0.0;
            double max_frame_avg = 0.0;
            png_uint_32 result = png_get_cLLI(png_ptr, info_ptr, &max_content, &max_frame_avg);
            (void)result;
            break;
        }
        case 3: {
            // png_set_cHRM
            // png_set_cHRM(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y)
            double white_x = get_double(1);
            double white_y = get_double(9);
            double red_x = get_double(17);
            double red_y = get_double(25);
            double green_x = get_double(33);
            double green_y = get_double(41);
            double blue_x = get_double(49);
            double blue_y = get_double(57);

            if (Size >= 65) {
                png_set_cHRM(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y);
            }
            break;
        }
        case 4: {
            // png_get_cHRM
            // png_get_cHRM(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y)
            double white_x = 0.0, white_y = 0.0;
            double red_x = 0.0, red_y = 0.0;
            double green_x = 0.0, green_y = 0.0;
            double blue_x = 0.0, blue_y = 0.0;
            png_uint_32 result = png_get_cHRM(png_ptr, info_ptr, &white_x, &white_y, &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y);
            (void)result;
            break;
        }
        case 5: {
            // png_get_mDCV
            // png_get_mDCV(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y, max_lum, min_lum)
            double white_x = 0.0, white_y = 0.0;
            double red_x = 0.0, red_y = 0.0;
            double green_x = 0.0, green_y = 0.0;
            double blue_x = 0.0, blue_y = 0.0;
            double max_lum = 0.0, min_lum = 0.0;
            png_uint_32 result = png_get_mDCV(png_ptr, info_ptr, &white_x, &white_y, &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y, &max_lum, &min_lum);
            (void)result;
            break;
        }
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    return 0;
}