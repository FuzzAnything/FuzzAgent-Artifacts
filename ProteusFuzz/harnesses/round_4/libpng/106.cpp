// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_sig_cmp at png.c:80:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_tRNS_to_alpha at pngrtran.c:969:1 in png.h
// png_set_rgb_to_gray at pngrtran.c:1085:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_set_gamma at pngrtran.c:901:1 in png.h
// png_set_alpha_mode at pngrtran.c:444:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_set_background at pngrtran.c:155:1 in png.h
// png_set_scale_16 at pngrtran.c:171:1 in png.h
// png_set_flush at pngwrite.c:966:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
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
#include <cstddef>
#include <cstring>
#include <vector>
#include <fstream>
#include <sys/stat.h>

static void write_dummy_png(const uint8_t *data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary | std::ios::trunc);
    if (!file.is_open()) return;
    
    // Write a minimal valid PNG header if data is too small
    if (size < 8) {
        const uint8_t png_sig[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
        file.write(reinterpret_cast<const char*>(png_sig), 8);
        // Add a minimal IHDR chunk (13 bytes data + 4 length + 4 type + 4 crc)
        // We'll just write some dummy bytes to make it look like a file
        file.write(reinterpret_cast<const char*>(data), size);
        // Pad to ensure some content
        for (int i = 0; i < 100; ++i) file.put(0x00);
    } else {
        file.write(reinterpret_cast<const char*>(data), size);
    }
    file.close();
}

static void safe_cleanup(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    } else if (info_ptr) {
        png_destroy_read_struct(nullptr, &info_ptr, nullptr);
    }
}

static int try_read_png(png_structp *png_ptr, png_infop *info_ptr, const uint8_t *data, size_t size) {
    *png_ptr = nullptr;
    *info_ptr = nullptr;

    if (size == 0) return 0;

    *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!*png_ptr) return 0;

    *info_ptr = png_create_info_struct(*png_ptr);
    if (!*info_ptr) {
        png_destroy_read_struct(png_ptr, nullptr, nullptr);
        return 0;
    }

    if (setjmp(png_jmpbuf(*png_ptr))) {
        safe_cleanup(*png_ptr, *info_ptr);
        return 0;
    }

    // Use a custom read callback to feed the fuzz data
    struct FuzzReadState {
        const uint8_t *data;
        size_t size;
        size_t offset;
    };
    static FuzzReadState read_state;
    read_state.data = data;
    read_state.size = size;
    read_state.offset = 0;

    png_set_read_fn(*png_ptr, &read_state, [](png_structp png_ptr, png_bytep out, png_size_t length) {
        FuzzReadState *state = static_cast<FuzzReadState*>(png_get_io_ptr(png_ptr));
        if (state->offset + length > state->size) {
            // Truncate to available data
            length = state->size - state->offset;
        }
        if (length > 0) {
            memcpy(out, state->data + state->offset, length);
            state->offset += length;
        }
    });

    // Try to read the signature
    int sig_bytes = 0;
    if (read_state.size >= 8) {
        sig_bytes = 8;
    } else if (read_state.size > 0) {
        sig_bytes = static_cast<int>(read_state.size);
    }

    if (sig_bytes > 0) {
        png_byte sig[8];
        if (read_state.size >= 8) {
            memcpy(sig, read_state.data, 8);
        } else {
            memcpy(sig, read_state.data, read_state.size);
            memset(sig + read_state.size, 0, 8 - read_state.size);
        }
        if (!png_sig_cmp(sig, 0, sig_bytes)) {
            png_set_sig_bytes(*png_ptr, static_cast<png_byte>(sig_bytes));
        }
    }

    // Read info
    png_read_info(*png_ptr, *info_ptr);

    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write data to dummy file for file-based operations if needed
    write_dummy_png(Data, Size);

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    if (!try_read_png(&png_ptr, &info_ptr, Data, Size)) {
        return 0;
    }

    // Ensure we have a valid context
    if (!png_ptr || !info_ptr) {
        safe_cleanup(png_ptr, info_ptr);
        return 0;
    }

    // Use setjmp/longjmp for safety during API calls
    if (setjmp(png_jmpbuf(png_ptr))) {
        safe_cleanup(png_ptr, info_ptr);
        return 0;
    }

    // 1. png_set_tRNS_to_alpha
    png_set_tRNS_to_alpha(png_ptr);

    // 2. png_set_rgb_to_gray (float version)
    double red_coef = 0.0;
    double green_coef = 0.0;
    if (Size >= 2) {
        red_coef = static_cast<int16_t>(Data[0]) / 256.0;
        green_coef = static_cast<int16_t>(Data[1]) / 256.0;
    }
    int error_action = (Size > 2) ? (Data[2] % 2) : 0;
    png_set_rgb_to_gray(png_ptr, error_action, red_coef, green_coef);

    // Check status
    png_byte status = png_get_rgb_to_gray_status(png_ptr);
    (void)status; // Suppress unused warning

    // 3. png_set_gamma
    double screen_gamma = 1.0;
    double file_gamma = 1.0;
    if (Size >= 4) {
        screen_gamma = static_cast<int16_t>(Data[3]) / 256.0;
        file_gamma = static_cast<int16_t>(Data[4]) / 256.0;
    }
    png_set_gamma(png_ptr, screen_gamma, file_gamma);

    // 4. png_set_alpha_mode
    int mode = (Size > 5) ? (Data[5] % 4) : 0; // PNG_ALPHA_PNG, PNG_ALPHA_OPTIMIZE, etc.
    double output_gamma = (Size > 6) ? static_cast<int16_t>(Data[6]) / 256.0 : 1.0;
    png_set_alpha_mode(png_ptr, mode, output_gamma);

    // 5. png_set_gray_to_rgb
    png_set_gray_to_rgb(png_ptr);

    // 6. png_set_background
    png_color_16 background_color;
    memset(&background_color, 0, sizeof(background_color));
    if (Size >= 9) {
        background_color.red = Data[7];
        background_color.green = Data[8];
        background_color.blue = (Size > 9) ? Data[9] : 0;
    }
    int bg_gamma_code = (Size > 10) ? (Data[10] % 4) : 0;
    int need_expand = (Size > 11) ? (Data[11] % 2) : 0;
    double bg_gamma = (Size > 12) ? static_cast<int16_t>(Data[12]) / 256.0 : 1.0;
    png_set_background(png_ptr, &background_color, bg_gamma_code, need_expand, bg_gamma);

    // Additional: png_set_scale_16
    png_set_scale_16(png_ptr);

    // Additional: png_set_flush
    int nrows = (Size > 13) ? Data[13] : 0;
    png_set_flush(png_ptr, nrows);

    // Cleanup
    safe_cleanup(png_ptr, info_ptr);

    return 0;
}