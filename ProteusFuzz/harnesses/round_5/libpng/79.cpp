// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_sCAL at pngset.c:664:1 in png.h
// png_get_sCAL_s at pngget.c:1067:1 in png.h
// png_get_y_pixels_per_inch at pngget.c:369:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_access_version_number at png.c:963:1 in png.h
// png_set_sCAL_s at pngset.c:598:1 in png.h
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
#include <cstdlib>
#include <cfloat>

static void safe_png_destroy(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // 1. Prepare environment
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling to prevent crashes during fuzzing
    if (setjmp(png_jmpbuf(png_ptr))) {
        safe_png_destroy(png_ptr, info_ptr);
        return 0;
    }

    // 2. Invoke Target Functions

    // Test png_access_version_number (Constant behavior, takes 0 arguments)
    png_uint_32 version = png_access_version_number();
    (void)version; // Use to suppress unused warning

    // Test png_set_sCAL_s
    // Extract strings from fuzz data safely
    const char *swidth = nullptr;
    const char *sheight = nullptr;
    int unit = 0;

    // Parse unit from first byte
    if (Data[0] < 3) {
        unit = Data[0]; // 0 or 1 or 2 (though 2 might be invalid depending on version, let libpng handle)
    } else {
        unit = 0;
    }

    // Find null-terminated strings or use substrings
    // We will construct valid strings to avoid immediate png_error on format
    // But we also want to test edge cases.
    // Let's create temporary buffers for valid strings to ensure we reach the logic
    char width_buf[20] = "1.0";
    char height_buf[20] = "1.0";

    if (Size > 20) {
        // Try to copy data into buffers, ensuring null termination
        size_t w_len = (Size > 10) ? 10 : Size;
        if (w_len > 18) w_len = 18;
        memcpy(width_buf, Data + 1, w_len);
        width_buf[w_len] = '\0';
        
        size_t h_len = (Size - 1 - w_len > 10) ? 10 : (Size - 1 - w_len);
        if (h_len > 18) h_len = 18;
        memcpy(height_buf, Data + 1 + w_len, h_len);
        height_buf[h_len] = '\0';

        // Ensure no leading minus for valid float test, but allow it to trigger validation
        // We will call with the raw data to test validation logic
        swidth = width_buf;
        sheight = height_buf;
    } else {
        // Default valid strings if data is too small
        swidth = "1.0";
        sheight = "1.0";
    }

    // Call png_set_sCAL_s
    // This might trigger png_error if strings are invalid, caught by setjmp
    png_set_sCAL_s(png_ptr, info_ptr, unit, swidth, sheight);

    // Test png_set_sCAL (double version)
    double d_width = 1.0;
    double d_height = 1.0;
    if (Size > 10) {
        // Interpret next 8 bytes as double if possible, or just use a derived value
        // For fuzzing, let's just use a simple mapping to avoid NaN/Inf issues unless intended
        unsigned long long val = 0;
        if (Size >= 18) {
            memcpy(&val, Data + 10, sizeof(val));
            d_width = (double)(val & 0xFFFFFFFF);
            d_height = (double)((val >> 32) & 0xFFFFFFFF);
        }
        // Clamp to reasonable range to avoid immediate warnings or overflows
        if (d_width < 0) d_width = 0;
        if (d_height < 0) d_height = 0;
    }
    
    int unit_double = (Data[0] % 2); // 0 or 1
    png_set_sCAL(png_ptr, info_ptr, unit_double, d_width, d_height);

    // Test png_get_sCAL_s
    int get_unit = 0;
    char *get_swidth = nullptr;
    char *get_sheight = nullptr;
    
    png_uint_32 result = png_get_sCAL_s(png_ptr, info_ptr, &get_unit, &get_swidth, &get_sheight);
    (void)result;
    (void)get_unit;
    (void)get_swidth;
    (void)get_sheight;

    // Test png_get_y_pixels_per_inch
    png_uint_32 y_ppi = png_get_y_pixels_per_inch(png_ptr, info_ptr);
    (void)y_ppi;

    // Test png_data_freer
    // Mask for sCAL data (PNG_INFO_sCAL)
    png_data_freer(png_ptr, info_ptr, PNG_DESTROY_WILL_FREE_DATA, PNG_INFO_sCAL);
    png_data_freer(png_ptr, info_ptr, PNG_USER_WILL_FREE_DATA, PNG_INFO_sCAL);

    // 3. Cleanup
    safe_png_destroy(png_ptr, info_ptr);

    return 0;
}