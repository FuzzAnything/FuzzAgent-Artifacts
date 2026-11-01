// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
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
#include <cstring>
#include <fstream>
#include <vector>

static void write_dummy_png(const uint8_t *data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
    file.close();
}

extern "C" int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input data to a dummy file to simulate a PNG file
    write_dummy_png(Data, Size);

    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Open the dummy file for reading
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, fp);

    // Attempt to read the PNG signature and header
    // This will fail if the data is not a valid PNG, which is expected for fuzzing
    png_read_info(png_ptr, info_ptr);

    // Close the file
    fclose(fp);

    // Fuzz target API functions

    // 1. png_get_gAMA_fixed
    png_fixed_point gamma_value;
    png_get_gAMA_fixed(png_ptr, info_ptr, &gamma_value);

    // 2. png_get_image_width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);

    // 3. png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);

    // 4. png_get_uint_16
    // Create a buffer for testing png_get_uint_16
    // Use the first two bytes of input if available, otherwise use dummy data
    png_byte buf[2];
    if (Size >= 2) {
        buf[0] = Data[0];
        buf[1] = Data[1];
    } else {
        buf[0] = 0;
        buf[1] = 0;
    }
    png_uint_16 val16 = png_get_uint_16(buf);

    // 5. png_get_hIST
    png_uint_16 *hist = nullptr;
    png_get_hIST(png_ptr, info_ptr, &hist);

    // 6. png_get_tIME
    // png_get_tIME expects a png_timep * (pointer to pointer), not a pointer to struct directly
    png_timep mod_time_ptr = nullptr;
    png_get_tIME(png_ptr, info_ptr, &mod_time_ptr);
    // If we needed to access the struct, we would do: png_time *mod_time = mod_time_ptr;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}