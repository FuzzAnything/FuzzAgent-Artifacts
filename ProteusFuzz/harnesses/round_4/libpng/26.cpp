// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_sig_cmp at png.c:80:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_palette_to_rgb at pngrtran.c:945:1 in png.h
// png_set_scale_16 at pngrtran.c:171:1 in png.h
// png_set_packswap at pngtrans.c:69:1 in png.h
// png_set_invert_mono at pngtrans.c:249:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_start_read_image at pngread.c:291:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
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
#include <setjmp.h>

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
}

static bool write_dummy_png(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return false;

    // Write minimal valid PNG header
    uint8_t png_sig[] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(png_sig, 1, 8, f);

    // IHDR chunk (minimal)
    uint8_t ihdr[] = {
        0, 0, 0, 13, // length
        'I', 'H', 'D', 'R', // type
        0, 0, 0, 1, // width 1
        0, 0, 0, 1, // height 1
        8, 2, 1, 0, 0, 0, 0 // bit depth 8, color type 2 (RGB), etc.
    };
    fwrite(ihdr, 1, 13, f);
    // Dummy CRC
    uint32_t crc = 0;
    fwrite(&crc, 1, 4, f);

    // IDAT chunk (compressed data)
    // Write some raw data from input if available, else dummy
    size_t idat_len = (size > 0) ? size : 1;
    if (idat_len > 65535) idat_len = 65535;
    uint32_t len32 = (uint32_t)idat_len;
    fwrite(&len32, 1, 4, f);
    fwrite("IDAT", 1, 4, f);
    if (size > 0) {
        fwrite(data, 1, idat_len, f);
    } else {
        uint8_t dummy = 0;
        fwrite(&dummy, 1, 1, f);
    }
    // Dummy CRC
    fwrite(&crc, 1, 4, f);

    // IEND chunk
    uint32_t iend_len = 0;
    fwrite(&iend_len, 1, 4, f);
    fwrite("IEND", 1, 4, f);
    fwrite(&crc, 1, 4, f);

    fclose(f);
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write dummy PNG file
    if (!write_dummy_png(Data, Size)) {
        return 0;
    }

    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        cleanup_png(png_ptr, nullptr);
        fclose(fp);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Check PNG signature manually instead of using png_sig_read
    png_byte signature[8];
    if (fread(signature, 1, 8, fp) != 8) {
        cleanup_png(png_ptr, info_ptr);
        fclose(fp);
        return 0;
    }
    if (png_sig_cmp(signature, 0, 8) != 0) {
        cleanup_png(png_ptr, info_ptr);
        fclose(fp);
        return 0;
    }

    // Read info
    png_read_info(png_ptr, info_ptr);

    // Apply transformations in various orders to explore states
    // 1. png_set_palette_to_rgb
    png_set_palette_to_rgb(png_ptr);

    // 2. png_set_scale_16
    png_set_scale_16(png_ptr);

    // 3. png_set_packswap
    png_set_packswap(png_ptr);

    // 4. png_set_invert_mono
    png_set_invert_mono(png_ptr);

    // 5. png_set_expand
    png_set_expand(png_ptr);

    // Update info after transformations
    png_read_update_info(png_ptr, info_ptr);

    // 6. png_start_read_image
    png_start_read_image(png_ptr);

    // Attempt to read image rows (minimal)
    png_bytep row = (png_bytep)malloc(png_get_rowbytes(png_ptr, info_ptr));
    if (row) {
        png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
        for (png_uint_32 y = 0; y < height; ++y) {
            png_read_row(png_ptr, row, nullptr);
        }
        free(row);
    }

    // End read
    png_read_end(png_ptr, nullptr);

    cleanup_png(png_ptr, info_ptr);
    fclose(fp);

    return 0;
}