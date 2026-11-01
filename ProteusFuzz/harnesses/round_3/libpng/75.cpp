// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_tRNS at pngset.c:1144:1 in png.h
// png_get_tRNS at pngget.c:1202:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_hIST at pngset.c:375:1 in png.h
// png_set_PLTE at pngset.c:739:1 in png.h
// png_get_bKGD at pngget.c:502:1 in png.h
// png_set_bKGD at pngset.c:24:1 in png.h
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
#include <vector>

static void setup_png_structs(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr) {
    *png_ptr_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (*png_ptr_ptr == nullptr) return;

    *info_ptr_ptr = png_create_info_struct(*png_ptr_ptr);
    if (*info_ptr_ptr == nullptr) {
        png_destroy_read_struct(png_ptr_ptr, nullptr, nullptr);
        *png_ptr_ptr = nullptr;
        return;
    }

    if (setjmp(png_jmpbuf(*png_ptr_ptr)) != 0) {
        png_destroy_read_struct(png_ptr_ptr, info_ptr_ptr, nullptr);
        *png_ptr_ptr = nullptr;
        *info_ptr_ptr = nullptr;
    }
}

static void cleanup_png_structs(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr) {
    if (*png_ptr_ptr != nullptr || *info_ptr_ptr != nullptr) {
        png_destroy_read_struct(png_ptr_ptr, info_ptr_ptr, nullptr);
        *png_ptr_ptr = nullptr;
        *info_ptr_ptr = nullptr;
    }
}

static void fuzz_png_set_hIST(const uint8_t *Data, size_t Size) {
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    setup_png_structs(&png_ptr, &info_ptr);
    if (png_ptr == nullptr || info_ptr == nullptr) return;

    if (Size >= 2) {
        png_uint_16 num_palette = (png_uint_16)((Data[0] << 8) | Data[1]);
        if (num_palette > 0 && Size >= 2 + num_palette * 2) {
            const png_uint_16 *hist = reinterpret_cast<const png_uint_16*>(Data + 2);
            png_set_hIST(png_ptr, info_ptr, hist);
        }
    }
    cleanup_png_structs(&png_ptr, &info_ptr);
}

static void fuzz_png_set_PLTE(const uint8_t *Data, size_t Size) {
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    setup_png_structs(&png_ptr, &info_ptr);
    if (png_ptr == nullptr || info_ptr == nullptr) return;

    if (Size >= 2) {
        int num_palette = (Data[0] << 8) | Data[1];
        if (num_palette > 0 && Size >= 2 + num_palette * 3) {
            const png_color *palette = reinterpret_cast<const png_color*>(Data + 2);
            png_set_PLTE(png_ptr, info_ptr, palette, num_palette);
        }
    }
    cleanup_png_structs(&png_ptr, &info_ptr);
}

static void fuzz_png_get_bKGD(const uint8_t *Data, size_t Size) {
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    setup_png_structs(&png_ptr, &info_ptr);
    if (png_ptr == nullptr || info_ptr == nullptr) return;

    png_color_16 background;
    png_color_16p background_ptr = &background;
    png_uint_32 result = png_get_bKGD(png_ptr, info_ptr, &background_ptr);
    (void)result;
    (void)Size;
    (void)Data;
    cleanup_png_structs(&png_ptr, &info_ptr);
}

static void fuzz_png_set_bKGD(const uint8_t *Data, size_t Size) {
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    setup_png_structs(&png_ptr, &info_ptr);
    if (png_ptr == nullptr || info_ptr == nullptr) return;

    if (Size >= sizeof(png_color_16)) {
        const png_color_16 *background = reinterpret_cast<const png_color_16*>(Data);
        png_set_bKGD(png_ptr, info_ptr, background);
    }
    cleanup_png_structs(&png_ptr, &info_ptr);
}

static void fuzz_png_set_tRNS(const uint8_t *Data, size_t Size) {
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    setup_png_structs(&png_ptr, &info_ptr);
    if (png_ptr == nullptr || info_ptr == nullptr) return;

    if (Size >= 1) {
        int num_trans = Data[0];
        const png_byte *trans_alpha = nullptr;
        png_color_16 trans_color = {0, 0, 0, 0, 0};
        png_const_color_16p trans_color_ptr = &trans_color;

        if (Size >= 1 + num_trans) {
            trans_alpha = Data + 1;
        }
        png_set_tRNS(png_ptr, info_ptr, trans_alpha, num_trans, trans_color_ptr);
    }
    cleanup_png_structs(&png_ptr, &info_ptr);
}

static void fuzz_png_get_tRNS(const uint8_t *Data, size_t Size) {
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    setup_png_structs(&png_ptr, &info_ptr);
    if (png_ptr == nullptr || info_ptr == nullptr) return;

    png_byte *trans_alpha = nullptr;
    int num_trans = 0;
    png_color_16 *trans_color = nullptr;
    png_uint_32 result = png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, &trans_color);
    (void)result;
    (void)Size;
    (void)Data;
    cleanup_png_structs(&png_ptr, &info_ptr);
}

extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    switch (Data[0] % 6) {
        case 0:
            fuzz_png_set_hIST(Data, Size);
            break;
        case 1:
            fuzz_png_set_PLTE(Data, Size);
            break;
        case 2:
            fuzz_png_get_bKGD(Data, Size);
            break;
        case 3:
            fuzz_png_set_bKGD(Data, Size);
            break;
        case 4:
            fuzz_png_set_tRNS(Data, Size);
            break;
        case 5:
            fuzz_png_get_tRNS(Data, Size);
            break;
    }
    return 0;
}