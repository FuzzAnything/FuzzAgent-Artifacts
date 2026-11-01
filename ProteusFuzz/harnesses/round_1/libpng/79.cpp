// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_y_offset_inches at pngget.c:424:1 in png.h
// png_get_x_offset_inches at pngget.c:413:1 in png.h
// png_get_y_pixels_per_inch at pngget.c:369:1 in png.h
// png_get_pixels_per_inch at pngget.c:357:1 in png.h
// png_get_pixel_aspect_ratio at pngget.c:189:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
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
    if (!file) return;
    file.write(reinterpret_cast<const char*>(data), size);
    file.close();
}

extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input to a dummy file to simulate a PNG file
    write_dummy_png(Data, Size);

    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    // Create PNG info structure
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

    // Open the dummy file
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, fp);

    // Read PNG info (header)
    png_read_info(png_ptr, info_ptr);

    // Close file
    fclose(fp);

    // Call target API functions
    // 1. png_get_y_offset_inches
    float y_offset_inches = png_get_y_offset_inches(png_ptr, info_ptr);
    (void)y_offset_inches;

    // 2. png_get_x_offset_inches
    float x_offset_inches = png_get_x_offset_inches(png_ptr, info_ptr);
    (void)x_offset_inches;

    // 3. png_get_y_pixels_per_inch
    png_uint_32 y_ppi = png_get_y_pixels_per_inch(png_ptr, info_ptr);
    (void)y_ppi;

    // 4. png_get_pixels_per_inch
    png_uint_32 ppi = png_get_pixels_per_inch(png_ptr, info_ptr);
    (void)ppi;

    // 5. png_get_pixel_aspect_ratio
    float aspect_ratio = png_get_pixel_aspect_ratio(png_ptr, info_ptr);
    (void)aspect_ratio;

    // 6. png_get_pHYs_dpi (if supported)
    #ifdef PNG_pHYs_SUPPORTED
    png_uint_32 res_x, res_y;
    int unit_type;
    png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    (void)res_x;
    (void)res_y;
    (void)unit_type;
    #endif

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}