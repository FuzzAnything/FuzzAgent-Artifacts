// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_bKGD at pngset.c:24:1 in png.h
// png_get_bKGD at pngget.c:502:1 in png.h
// png_get_tRNS at pngget.c:1202:1 in png.h
// png_set_hIST at pngset.c:375:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_PLTE at pngset.c:739:1 in png.h
// png_set_tRNS at pngset.c:1144:1 in png.h
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
    if (info_ptr) {
        png_destroy_info_struct(png_ptr, &info_ptr);
    }
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Initialize PNG structures
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        cleanup_png(png_ptr, nullptr);
        return 0;
    }

    // Set up error handling to prevent crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Prepare fuzzing data
    std::vector<uint8_t> fuzz_data(Data, Data + Size);
    size_t offset = 0;

    // Helper to read bytes safely
    auto read_bytes = [&](size_t count) -> std::vector<uint8_t> {
        if (offset + count > fuzz_data.size()) {
            return std::vector<uint8_t>(count, 0);
        }
        std::vector<uint8_t> chunk(fuzz_data.begin() + offset, fuzz_data.begin() + offset + count);
        offset += count;
        return chunk;
    };

    auto read_int = [&]() -> int {
        if (offset + 4 > fuzz_data.size()) return 0;
        int val = 0;
        std::memcpy(&val, fuzz_data.data() + offset, 4);
        offset += 4;
        return val;
    };

    // Fuzz png_set_PLTE
    {
        int num_palette = read_int() & 0xFFFF; // Limit to 65535
        if (num_palette > 256) num_palette = 256; // Reasonable limit for palette
        std::vector<png_color> palette(num_palette);
        for (int i = 0; i < num_palette; ++i) {
            std::vector<uint8_t> bytes = read_bytes(3);
            palette[i].red = bytes[0];
            palette[i].green = bytes[1];
            palette[i].blue = bytes[2];
        }
        png_set_PLTE(png_ptr, info_ptr, palette.data(), num_palette);
    }

    // Fuzz png_set_tRNS
    {
        int num_trans = read_int() & 0xFFFF;
        if (num_trans > 256) num_trans = 256;
        std::vector<png_byte> trans_alpha(num_trans);
        for (int i = 0; i < num_trans; ++i) {
            std::vector<uint8_t> bytes = read_bytes(1);
            trans_alpha[i] = bytes[0];
        }
        png_color_16 trans_color = {};
        std::vector<uint8_t> color_bytes = read_bytes(6);
        if (color_bytes.size() >= 6) {
            trans_color.red = static_cast<png_uint_16>(color_bytes[0]) | (static_cast<png_uint_16>(color_bytes[1]) << 8);
            trans_color.green = static_cast<png_uint_16>(color_bytes[2]) | (static_cast<png_uint_16>(color_bytes[3]) << 8);
            trans_color.blue = static_cast<png_uint_16>(color_bytes[4]) | (static_cast<png_uint_16>(color_bytes[5]) << 8);
        }
        png_set_tRNS(png_ptr, info_ptr, trans_alpha.data(), num_trans, &trans_color);
    }

    // Fuzz png_set_bKGD
    {
        png_color_16 background = {};
        std::vector<uint8_t> bg_bytes = read_bytes(6);
        if (bg_bytes.size() >= 6) {
            background.red = static_cast<png_uint_16>(bg_bytes[0]) | (static_cast<png_uint_16>(bg_bytes[1]) << 8);
            background.green = static_cast<png_uint_16>(bg_bytes[2]) | (static_cast<png_uint_16>(bg_bytes[3]) << 8);
            background.blue = static_cast<png_uint_16>(bg_bytes[4]) | (static_cast<png_uint_16>(bg_bytes[5]) << 8);
        }
        png_set_bKGD(png_ptr, info_ptr, &background);
    }

    // Fuzz png_get_bKGD
    {
        png_color_16 *bg_ptr = nullptr;
        png_uint_32 flag = png_get_bKGD(png_ptr, info_ptr, &bg_ptr);
        (void)flag; // Suppress unused warning
    }

    // Fuzz png_get_tRNS
    {
        png_byte *trans_alpha = nullptr;
        int num_trans = 0;
        png_color_16 *trans_color = nullptr;
        png_uint_32 flag = png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, &trans_color);
        (void)flag; // Suppress unused warning
    }

    // Fuzz png_set_hIST
    // png_set_hIST requires that a PLTE has been set, otherwise it may access invalid memory or fail.
    // We ensure PLTE is set before calling this.
    {
        int num_palette = read_int() & 0xFFFF;
        if (num_palette > 256) num_palette = 256;
        
        // Ensure we have a valid palette count from the previous PLTE call if possible, 
        // but here we just generate a histogram matching the current palette size if set.
        // However, libpng checks if PLTE is set. If not, it might crash or warn.
        // To be safe, we only call if we can ensure PLTE exists, but since we called set_PLTE above,
        // we assume it's set. If num_palette is 0, we skip to avoid issues.
        if (num_palette == 0) {
            // Read dummy bytes to advance offset
            std::vector<uint8_t> dummy = read_bytes(2); 
            (void)dummy;
        } else {
            std::vector<png_uint_16> hist(num_palette);
            for (int i = 0; i < num_palette; ++i) {
                std::vector<uint8_t> bytes = read_bytes(2);
                hist[i] = static_cast<png_uint_16>(bytes[0]) | (static_cast<png_uint_16>(bytes[1]) << 8);
            }
            png_set_hIST(png_ptr, info_ptr, hist.data());
        }
    }

    // Cleanup
    cleanup_png(png_ptr, info_ptr);
    return 0;
}