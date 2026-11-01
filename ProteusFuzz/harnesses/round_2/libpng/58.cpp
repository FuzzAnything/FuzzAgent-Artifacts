// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_sCAL_fixed at pngset.c:694:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_get_y_offset_inches_fixed at pngget.c:403:1 in png.h
// png_get_pixel_aspect_ratio_fixed at pngget.c:213:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_oFFs at pngset.c:465:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
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
#include <cstddef>

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (info_ptr) {
        png_destroy_info_struct(png_ptr, &info_ptr);
    }
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for libpng to read from if needed
    const char* dummy_file = "./dummy_file";
    FILE* fp = fopen(dummy_file, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        cleanup_png(png_ptr, nullptr);
        return 0;
    }

    // Use setjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Attempt to open the dummy file for reading
    fp = fopen(dummy_file, "rb");
    if (!fp) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Read the PNG file to populate info_ptr
    // We skip reading the actual image data to focus on metadata APIs
    png_read_info(png_ptr, info_ptr);
    fclose(fp);

    // Fuzzing logic for target API functions

    // 1. png_set_oFFs
    // Extract fuzzing data for offset values and unit type
    png_int_32 offset_x = 0;
    png_int_32 offset_y = 0;
    int unit_type = 0;

    if (Size >= 9) {
        std::memcpy(&offset_x, Data, sizeof(png_int_32));
        std::memcpy(&offset_y, Data + sizeof(png_int_32), sizeof(png_int_32));
        std::memcpy(&unit_type, Data + 2 * sizeof(png_int_32), sizeof(int));
    }

    // Call png_set_oFFs
    png_set_oFFs(png_ptr, info_ptr, offset_x, offset_y, unit_type);

    // 2. png_get_oFFs
    png_int_32 get_offset_x = 0;
    png_int_32 get_offset_y = 0;
    int get_unit_type = 0;
    png_get_oFFs(png_ptr, info_ptr, &get_offset_x, &get_offset_y, &get_unit_type);

    // 3. png_set_sCAL_fixed
    // Extract width and height from data
    png_fixed_point width = 0;
    png_fixed_point height = 0;
    int scale_unit = 0;

    if (Size >= 17) {
        std::memcpy(&width, Data + 3 * sizeof(png_int_32), sizeof(png_fixed_point));
        std::memcpy(&height, Data + 3 * sizeof(png_int_32) + sizeof(png_fixed_point), sizeof(png_fixed_point));
        std::memcpy(&scale_unit, Data + 3 * sizeof(png_int_32) + 2 * sizeof(png_fixed_point), sizeof(int));
    }

    png_set_sCAL_fixed(png_ptr, info_ptr, scale_unit, width, height);

    // 4. png_get_x_offset_inches_fixed
    png_fixed_point x_offset_inches = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    (void)x_offset_inches; // Suppress unused variable warning

    // 5. png_get_y_offset_inches_fixed
    png_fixed_point y_offset_inches = png_get_y_offset_inches_fixed(png_ptr, info_ptr);
    (void)y_offset_inches;

    // 6. png_get_pixel_aspect_ratio_fixed
    png_fixed_point aspect_ratio = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
    (void)aspect_ratio;

    // 7. png_get_pHYs_dpi (related API from description)
    png_uint_32 res_x = 0;
    png_uint_32 res_y = 0;
    int phys_unit_type = 0;
    png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &phys_unit_type);

    // 8. png_get_x_offset_pixels (related API from description)
    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);
    (void)x_offset_pixels;

    // Cleanup
    cleanup_png(png_ptr, info_ptr);

    return 0;
}