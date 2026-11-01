// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_add_alpha at pngtrans.c:204:1 in png.h
// png_set_user_transform_info at pngtrans.c:805:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_set_packswap at pngtrans.c:69:1 in png.h
// png_set_filler at pngtrans.c:119:1 in png.h
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
#include <fstream>

static void write_dummy_png(const uint8_t *data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) return;
    if (size > 0) {
        file.write(reinterpret_cast<const char*>(data), size);
    } else {
        // Write a minimal valid PNG header if input is empty to avoid immediate crash on open
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
        file.write(reinterpret_cast<const char*>(minimal_png), sizeof(minimal_png));
    }
    file.close();
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        write_dummy_png(nullptr, 0);
    } else {
        write_dummy_png(Data, Size);
    }

    std::ifstream file("./dummy_file", std::ios::binary);
    if (!file) {
        return 0;
    }
    file.close();

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    FILE* fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Read the IHDR chunk first
    png_read_info(png_ptr, info_ptr);

    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    
    // Test png_get_IHDR
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);

    // Test png_set_packswap (only effective for bit_depth < 8)
    png_set_packswap(png_ptr);

    // Test png_set_filler
    // Only valid for Gray or RGB images
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_RGB) {
        png_uint_32 filler = 0xFF;
        int flags = (Size > 0) ? (Data[0] & 0x03) : PNG_FILLER_AFTER;
        if (flags == 0) flags = PNG_FILLER_AFTER;
        png_set_filler(png_ptr, filler, flags);
    }

    // Test png_set_add_alpha
    // Only valid for Gray or RGB images
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_RGB) {
        png_uint_32 alpha_val = (Size > 1) ? (Data[1] & 0xFF) : 0xFF;
        int alpha_flags = (Size > 2) ? (Data[2] & 0x03) : PNG_FILLER_AFTER;
        if (alpha_flags == 0) alpha_flags = PNG_FILLER_AFTER;
        png_set_add_alpha(png_ptr, alpha_val, alpha_flags);
    }

    // Test png_set_user_transform_info
    // Cast const uint8_t* to void* is allowed directly
    void* user_ptr = const_cast<uint8_t*>(const_cast<uint8_t*>(Data));
    int user_depth = (Size > 3) ? (Data[3] % 17) : 8;
    int user_channels = (Size > 4) ? (Data[4] % 5) : 3;
    png_set_user_transform_info(png_ptr, user_ptr, user_depth, user_channels);

    // Prepare for reading image
    png_read_update_info(png_ptr, info_ptr);

    // Allocate row pointers
    png_bytep* row_pointers = nullptr;
    if (height > 0 && width > 0) {
        row_pointers = new png_bytep[height];
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = new png_byte[png_get_rowbytes(png_ptr, info_ptr)];
        }
    }

    // Test png_read_png
    // Using transforms = 0 to let previous settings take effect
    png_read_png(png_ptr, info_ptr, 0, nullptr);

    // Cleanup
    if (row_pointers) {
        for (png_uint_32 y = 0; y < height; y++) {
            delete[] row_pointers[y];
        }
        delete[] row_pointers;
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);

    return 0;
}