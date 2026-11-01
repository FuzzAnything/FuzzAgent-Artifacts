// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_hIST at pngget.c:911:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
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
#include <cstring>
#include <cstdio>
#include <cstdlib>

static void static_cleanup(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    } else if (info_ptr) {
        // If png_ptr is null but info_ptr is not, we can't safely destroy info_ptr
        // as it depends on png_ptr. In a real scenario, this shouldn't happen.
        // We just leak to avoid crash in this specific edge case.
    }
}

extern "C" int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file to simulate reading a PNG
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize libpng read structure
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
        static_cleanup(png_ptr, info_ptr);
        return 0;
    }

    // Open the dummy file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the PNG header and info
    // We use png_read_info which will trigger parsing of chunks
    // If the data is invalid, it will jump to the setjmp handler
    png_read_info(png_ptr, info_ptr);
    fclose(fp);

    // --- Fuzz Target API Calls ---

    // 1. png_get_gAMA_fixed
    png_fixed_point gama_value = 0;
    png_get_gAMA_fixed(png_ptr, info_ptr, &gama_value);
    // Also test with NULL pointer (should be safe)
    png_get_gAMA_fixed(png_ptr, info_ptr, nullptr);

    // 2. png_get_image_width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    // Test with NULL info_ptr (should return 0 safely)
    png_uint_32 width_null = png_get_image_width(png_ptr, nullptr);

    // 3. png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    // Test with NULL info_ptr
    png_uint_32 x_ppm_null = png_get_x_pixels_per_meter(png_ptr, nullptr);

    // 4. png_get_uint_16
    // We need a buffer of at least 2 bytes. If input is too small, skip or pad.
    if (Size >= 2) {
        png_uint_16 val = png_get_uint_16(Data);
        (void)val; // Suppress unused variable warning
    } else {
        // Create a small buffer to test the function safely
        png_byte buf[2] = {0, 0};
        if (Size > 0) buf[0] = Data[0];
        if (Size > 1) buf[1] = Data[1];
        png_uint_16 val = png_get_uint_16(buf);
        (void)val;
    }

    // 5. png_get_hIST
    png_uint_16 *hist = nullptr;
    png_uint_32 hist_result = png_get_hIST(png_ptr, info_ptr, &hist);
    // Test with NULL output pointer
    png_get_hIST(png_ptr, info_ptr, nullptr);

    // 6. png_get_tIME
    png_time mod_time;
    png_timep time_ptr = &mod_time;
    png_uint_32 time_result = png_get_tIME(png_ptr, info_ptr, &time_ptr);
    // Test with NULL output pointer
    png_get_tIME(png_ptr, info_ptr, nullptr);

    // Cleanup
    static_cleanup(png_ptr, info_ptr);

    // Remove dummy file
    remove(filename);

    return 0;
}