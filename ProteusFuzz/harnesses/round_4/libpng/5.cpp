// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_y_offset_inches at pngget.c:424:1 in png.h
// png_get_pixel_aspect_ratio at pngget.c:189:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_oFFs at pngset.c:465:1 in png.h
// png_get_y_offset_pixels at pngget.c:304:1 in png.h
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstddef>

static void setup_png_structs(png_structpp png_ptr, png_infopp info_ptr) {
    *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!*png_ptr) {
        *png_ptr = NULL;
        return;
    }
    *info_ptr = png_create_info_struct(*png_ptr);
    if (!*info_ptr) {
        png_destroy_read_struct(png_ptr, NULL, NULL);
        *png_ptr = NULL;
        *info_ptr = NULL;
    }
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    FuzzedDataProvider provider(Data, Size);

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_infopp info_ptr_ptr = &info_ptr;

    setup_png_structs(&png_ptr, &info_ptr);

    if (!png_ptr || !info_ptr) {
        return 0;
    }

    // Set a custom error handler to prevent libpng from aborting on invalid data
    // This allows the fuzzer to continue exploring even with bad inputs
    png_set_error_fn(png_ptr, NULL, NULL, NULL);

    // Generate valid IHDR parameters to avoid "Invalid IHDR data" error
    // Width and Height must be > 0
    uint32_t width = provider.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint32_t height = provider.ConsumeIntegralInRange<uint32_t>(1, 1024);
    
    // Bit depth must be valid for the chosen color type
    uint8_t bit_depth = provider.ConsumeIntegralInRange<uint8_t>(1, 16);
    int color_type = provider.ConsumeIntegralInRange<int>(0, 6);

    // Validate bit_depth based on color_type to ensure IHDR validity
    // PNG spec:
    // Color type 0 (Grayscale): 1, 2, 4, 8, 16
    // Color type 2 (RGB): 8, 16
    // Color type 3 (Palette): 1, 2, 4, 8
    // Color type 4 (Grayscale+Alpha): 8, 16
    // Color type 6 (RGBA): 8, 16
    bool valid_depth = false;
    switch (color_type) {
        case PNG_COLOR_TYPE_GRAY:
            if (bit_depth == 1 || bit_depth == 2 || bit_depth == 4 || bit_depth == 8 || bit_depth == 16) valid_depth = true;
            break;
        case PNG_COLOR_TYPE_RGB:
            if (bit_depth == 8 || bit_depth == 16) valid_depth = true;
            break;
        case PNG_COLOR_TYPE_PALETTE:
            if (bit_depth == 1 || bit_depth == 2 || bit_depth == 4 || bit_depth == 8) valid_depth = true;
            break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            if (bit_depth == 8 || bit_depth == 16) valid_depth = true;
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            if (bit_depth == 8 || bit_depth == 16) valid_depth = true;
            break;
        default:
            // Invalid color type, fallback to safe defaults
            color_type = PNG_COLOR_TYPE_RGB;
            bit_depth = 8;
            valid_depth = true;
            break;
    }

    // If the generated parameters are invalid, use safe defaults
    if (!valid_depth) {
        color_type = PNG_COLOR_TYPE_RGB;
        bit_depth = 8;
    }

    // Initialize image info with validated parameters
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);

    // Test png_set_oFFs
    int32_t offset_x = provider.ConsumeIntegralInRange<int32_t>(-1000, 1000);
    int32_t offset_y = provider.ConsumeIntegralInRange<int32_t>(-1000, 1000);
    int unit_type = provider.ConsumeIntegralInRange<int>(0, 2); // PNG_OFFSET_PIXEL, MICRON, etc.
    png_set_oFFs(png_ptr, info_ptr, offset_x, offset_y, unit_type);

    // Test png_get_y_offset_pixels
    png_int_32 y_offset_pixels = png_get_y_offset_pixels(png_ptr, info_ptr);
    (void)y_offset_pixels; // Suppress unused variable warning

    // Test png_get_y_offset_inches
    float y_offset_inches = png_get_y_offset_inches(png_ptr, info_ptr);
    (void)y_offset_inches;

    // Test png_get_pixel_aspect_ratio
    // Note: This requires pHYs chunk data. We haven't set it, so it should return 0.0 safely.
    float aspect_ratio = png_get_pixel_aspect_ratio(png_ptr, info_ptr);
    (void)aspect_ratio;

    // Test png_set_invalid
    // Fix: Use 32-bit unsigned integer range to avoid FuzzedDataProvider overflow/abort
    // The mask is an int, but we generate it as uint32_t then cast.
    // We limit the range to valid 32-bit values to ensure safety.
    uint32_t mask_val = provider.ConsumeIntegralInRange<uint32_t>(0, 0xFFFFFFFF);
    png_set_invalid(png_ptr, info_ptr, static_cast<int>(mask_val));

    // Test png_get_pHYs_dpi (requires pHYs data, which we haven't set, but function should handle gracefully)
    png_uint_32 res_x, res_y;
    int unit_type_out;
    png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type_out);

    // Cleanup
    png_destroy_info_struct(png_ptr, info_ptr_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}