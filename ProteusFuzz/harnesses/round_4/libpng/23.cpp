// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_png at pngwrite.c:1376:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_sRGB at pngset.c:819:1 in png.h
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

static void write_dummy_png_file(const uint8_t *data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        if (size > 0) {
            file.write(reinterpret_cast<const char*>(data), size);
        }
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Create dummy file if needed for read operations (though we focus on write APIs)
    write_dummy_png_file(Data, Size);

    // Initialize libpng structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Set up error handling to prevent crashes during fuzzing
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Initialize a minimal image to allow png_write_png to work
    png_uint_32 width = 1, height = 1;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;
    
    // Correct usage: png_get_rowbytes requires info_ptr
    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    if (row_bytes == 0) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    png_bytep row = (png_bytep)png_malloc(png_ptr, row_bytes * height);
    if (!row) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    memset(row, 0, row_bytes * height);

    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Extract fuzzing data for parameters
    int param1 = Data[0];
    int param2 = Data[1];
    png_fixed_point fixed_val = static_cast<png_fixed_point>(param1 * 100000 + param2);
    
    // Fuzz png_set_gAMA_fixed
    png_set_gAMA_fixed(png_ptr, info_ptr, fixed_val);

    // Fuzz png_set_sRGB_gAMA_and_cHRM
    png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, param1 % 4);

    // Fuzz png_set_cHRM_fixed
    png_set_cHRM_fixed(png_ptr, info_ptr,
                       fixed_val, fixed_val,
                       fixed_val, fixed_val,
                       fixed_val, fixed_val,
                       fixed_val, fixed_val);

    // Fuzz png_set_cHRM_XYZ_fixed
    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr,
                           fixed_val, fixed_val, fixed_val,
                           fixed_val, fixed_val, fixed_val,
                           fixed_val, fixed_val, fixed_val);

    // Fuzz png_set_sRGB
    png_set_sRGB(png_ptr, info_ptr, param2 % 4);

    // Attempt to write the PNG (this will fail without a valid file handle, but exercises the logic)
    // We use a dummy file path; in a real scenario, we'd open a file for writing.
    // For fuzzing purposes, we just call the function to trigger internal logic.
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);

    // Cleanup
    png_free(png_ptr, row);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}