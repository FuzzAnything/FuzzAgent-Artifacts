// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_eXIf_1 at pngget.c:892:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_pixels_per_meter at pngget.c:167:1 in png.h
// png_get_y_pixels_per_inch at pngget.c:369:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
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

static void read_png_data(const uint8_t *data, size_t size, png_structp *png_ptr, png_infop *info_ptr) {
    if (size < 8) return;

    // Check for PNG signature
    const uint8_t png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    if (memcmp(data, png_sig, 8) != 0) return;

    *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!*png_ptr) return;

    *info_ptr = png_create_info_struct(*png_ptr);
    if (!*info_ptr) {
        png_destroy_read_struct(png_ptr, nullptr, nullptr);
        return;
    }

    if (setjmp(png_jmpbuf(*png_ptr))) {
        png_destroy_read_struct(png_ptr, info_ptr, nullptr);
        *png_ptr = nullptr;
        *info_ptr = nullptr;
        return;
    }

    // Create a dummy file to read from
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        png_destroy_read_struct(png_ptr, info_ptr, nullptr);
        *png_ptr = nullptr;
        *info_ptr = nullptr;
        return;
    }
    fwrite(data, 1, size, fp);
    fclose(fp);

    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(png_ptr, info_ptr, nullptr);
        *png_ptr = nullptr;
        *info_ptr = nullptr;
        return;
    }

    png_init_io(*png_ptr, fp);

    // Read the IHDR and other chunks
    png_read_info(*png_ptr, *info_ptr);

    fclose(fp);
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    read_png_data(Data, Size, &png_ptr, &info_ptr);

    if (!png_ptr || !info_ptr) {
        if (png_ptr) png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Test png_get_oFFs
    png_int_32 offset_x, offset_y;
    int unit_type;
    png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, &unit_type);

    // Test png_get_pHYs_dpi
    png_uint_32 res_x, res_y;
    png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);

    // Test png_get_pixels_per_meter
    png_get_pixels_per_meter(png_ptr, info_ptr);

    // Test png_get_y_pixels_per_inch
    png_get_y_pixels_per_inch(png_ptr, info_ptr);

    // Test png_get_image_height
    png_get_image_height(png_ptr, info_ptr);

    // Test png_get_eXIf_1
    png_uint_32 num_exif;
    png_byte *exif;
    png_get_eXIf_1(png_ptr, info_ptr, &num_exif, &exif);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}