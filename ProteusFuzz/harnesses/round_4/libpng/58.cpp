// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_iCCP at pngget.c:718:1 in png.h
// png_get_tRNS at pngget.c:1202:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_eXIf_1 at pngget.c:892:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_get_sBIT at pngget.c:1140:1 in png.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    if (size > 0) {
        fwrite(data, 1, size, f);
    } else {
        // Write a minimal valid PNG header if input is empty to avoid immediate failure
        const uint8_t minimal_png[] = {
            0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A,
            0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52,
            0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
            0x08, 0x02, 0x00, 0x00, 0x00, 0x90, 0x77, 0x53,
            0xDE, 0x00, 0x00, 0x00, 0x0C, 0x49, 0x44, 0x41,
            0x54, 0x08, 0xD7, 0x63, 0xF8, 0xFF, 0xFF, 0x3F,
            0x00, 0x05, 0xFE, 0x02, 0xFE, 0xDC, 0xCC, 0x59,
            0xE7, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E,
            0x44, 0xAE, 0x42, 0x60, 0x82
        };
        fwrite(minimal_png, 1, sizeof(minimal_png), f);
    }
    fclose(f);
}

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    } else if (info_ptr) {
        // If png_ptr is null but info_ptr is not, we can't safely destroy
        // In a real scenario, this shouldn't happen if managed correctly.
        // We just leak to avoid crash or undefined behavior.
    }
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

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
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return 0;
    }

    // Set error handling to return instead of aborting
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the signature
    png_read_info(png_ptr, info_ptr);

    // --- Fuzz png_get_cICP ---
    png_byte colour_primaries = 0;
    png_byte transfer_function = 0;
    png_byte matrix_coefficients = 0;
    png_byte video_full_range_flag = 0;
    png_get_cICP(png_ptr, info_ptr, &colour_primaries, &transfer_function,
                 &matrix_coefficients, &video_full_range_flag);

    // --- Fuzz png_get_sBIT ---
    // Signature requires png_color_8p * (pointer to pointer)
    png_color_8 sig_bit;
    png_color_8p sig_bit_ptr = &sig_bit;
    png_get_sBIT(png_ptr, info_ptr, &sig_bit_ptr);

    // --- Fuzz png_get_iCCP ---
    char *name = nullptr;
    int compression_type = 0;
    png_byte *profile = nullptr;
    png_uint_32 proflen = 0;
    png_get_iCCP(png_ptr, info_ptr, &name, &compression_type, &profile, &proflen);
    // Note: profile data is owned by libpng, do not free

    // --- Fuzz png_get_tRNS ---
    // Signature requires png_color_16p * (pointer to pointer) for trans_color
    png_byte *trans_alpha = nullptr;
    int num_trans = 0;
    png_color_16 trans_color;
    png_color_16p trans_color_ptr = &trans_color;
    png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, &trans_color_ptr);

    // --- Fuzz png_get_color_type ---
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);

    // --- Fuzz png_get_eXIf_1 ---
    png_uint_32 num_exif = 0;
    png_byte *exif = nullptr;
    png_get_eXIf_1(png_ptr, info_ptr, &num_exif, &exif);

    // Cleanup
    cleanup_png(png_ptr, info_ptr);
    fclose(fp);

    return 0;
}