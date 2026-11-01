// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_read_update_info at pngread.c:260:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_bgr at pngtrans.c:20:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_set_palette_to_rgb at pngrtran.c:945:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_bgr at pngtrans.c:20:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_palette_to_rgb at pngrtran.c:945:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
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

static void write_dummy_png(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    if (data && size > 0) {
        fwrite(data, 1, size, f);
    } else {
        // Write a minimal valid PNG header if no data provided
        const uint8_t png_sig[] = {137, 80, 78, 71, 13, 10, 26, 10};
        fwrite(png_sig, 1, 8, f);
        // Minimal IHDR chunk (13 bytes data + 4 length + 4 CRC = 21 bytes)
        // Length: 13
        uint8_t ihdr[] = {0, 0, 0, 13, 115, 72, 68, 82, 0, 0, 0, 1, 0, 0, 0, 1, 8, 2, 0, 0, 0};
        fwrite(ihdr, 1, 21, f);
        // Minimal IEND chunk (4 length + 4 CRC = 8 bytes)
        uint8_t iend[] = {0, 0, 0, 0, 73, 69, 78, 68, 0, 0, 0, 0};
        fwrite(iend, 1, 12, f);
    }
    fclose(f);
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    write_dummy_png(Data, Size);

    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Test png_set_bgr early as per requirements
    png_set_bgr(png_ptr);

    // Test png_set_expand
    png_set_expand(png_ptr);

    // Read header and info
    png_read_info(png_ptr, info_ptr);

    // Test png_set_palette_to_rgb
    png_set_palette_to_rgb(png_ptr);

    // Test png_get_rgb_to_gray_status
    png_byte status = png_get_rgb_to_gray_status(png_ptr);
    (void)status; // Use variable to avoid unused warning

    // Prepare for reading rows
    png_uint_32 width, height;
    int bit_depth, color_type;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

    if (width == 0 || height == 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }

    // Calculate row size
    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    if (row_bytes == 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }

    std::vector<png_byte> row(row_bytes);
    std::vector<png_byte> display_row(row_bytes);

    // Start reading
    png_read_update_info(png_ptr, info_ptr);

    // Fuzz png_read_row with various iterations
    for (png_uint_32 y = 0; y < height; ++y) {
        // Occasionally skip rows to explore different states
        if (Data[y % Size] % 3 == 0) {
            continue;
        }
        png_read_row(png_ptr, row.data(), display_row.data());
    }

    // Test png_read_image
    std::vector<png_byte*> image(height);
    for (png_uint_32 y = 0; y < height; ++y) {
        image[y] = new png_byte[row_bytes];
    }

    // Reset file pointer to re-read for png_read_image test
    fclose(fp);
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        png_init_io(png_ptr, fp);
        png_read_info(png_ptr, info_ptr);
        png_set_bgr(png_ptr);
        png_set_expand(png_ptr);
        png_set_palette_to_rgb(png_ptr);
        png_read_update_info(png_ptr, info_ptr);
        png_read_image(png_ptr, image.data());
        fclose(fp);
    }

    // Cleanup
    for (png_uint_32 y = 0; y < height; ++y) {
        delete[] image[y];
    }
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}