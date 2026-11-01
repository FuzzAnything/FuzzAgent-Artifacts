// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_compression_buffer_size at pngset.c:1865:1 in png.h
// png_set_text_compression_strategy at pngwrite.c:1295:1 in png.h
// png_set_compression_level at pngwrite.c:1185:1 in png.h
// png_set_compression_window_bits at pngwrite.c:1224:1 in png.h
// png_get_compression_buffer_size at pngget.c:1279:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_compression_buffer_size at pngset.c:1865:1 in png.h
// png_get_compression_buffer_size at pngget.c:1279:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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
#include <cstdint>
#include <cstring>
#include <cstdlib>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static png_structp create_png_write_struct() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return NULL;
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return NULL;
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return NULL;
    }
    return png_ptr;
}

static void cleanup_png_write(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}

static png_structp create_png_read_struct(FILE *f) {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return NULL;
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return NULL;
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return NULL;
    }
    if (f) {
        png_init_io(png_ptr, f);
    }
    return png_ptr;
}

static void cleanup_png_read(png_structp png_ptr, png_infop info_ptr, FILE *f) {
    if (f) fclose(f);
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    }
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Use first byte to decide between read and write mode
    int mode = Data[0] % 2;

    if (mode == 0) {
        // Write mode
        png_structp png_ptr = create_png_write_struct();
        if (!png_ptr) return 0;
        png_infop info_ptr = NULL;
        try {
            info_ptr = png_create_info_struct(png_ptr);
            if (!info_ptr) {
                cleanup_png_write(png_ptr, info_ptr);
                return 0;
            }

            if (setjmp(png_jmpbuf(png_ptr))) {
                cleanup_png_write(png_ptr, info_ptr);
                return 0;
            }

            // Initialize dummy file
            FILE *f = fopen("./dummy_file", "wb");
            if (!f) {
                cleanup_png_write(png_ptr, info_ptr);
                return 0;
            }
            png_init_io(png_ptr, f);

            // Set compression parameters using fuzzer data
            size_t buffer_size = 0;
            if (Size > 1) buffer_size = Data[1];
            if (Size > 2) buffer_size |= ((size_t)Data[2] << 8);
            if (Size > 3) buffer_size |= ((size_t)Data[3] << 16);
            if (Size > 4) buffer_size |= ((size_t)Data[4] << 24);
            if (buffer_size > 2147483647UL) buffer_size = 2147483647UL; // Cap at 2GB
            png_set_compression_buffer_size(png_ptr, buffer_size);

            int strategy = (Data[5] % 4) - 2; // Range: -2 to 1 (valid: -1 to 3)
            png_set_text_compression_strategy(png_ptr, strategy);

            int level = Data[6] % 10; // 0-9
            png_set_compression_level(png_ptr, level);

            int window_bits = (Data[7] % 8) + 8; // 8-15
            png_set_compression_window_bits(png_ptr, window_bits);

            // Query buffer size
            size_t current_size = png_get_compression_buffer_size(png_ptr);
            (void)current_size;

            // Create a minimal image to trigger write
            png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
            png_byte row[3] = {0, 0, 0};
            png_write_info(png_ptr, info_ptr);
            png_write_row(png_ptr, row);
            png_write_flush(png_ptr);
            png_write_end(png_ptr, info_ptr);

            fclose(f);
        } catch (...) {
            if (png_ptr) png_destroy_write_struct(&png_ptr, &info_ptr);
            return 0;
        }
        cleanup_png_write(png_ptr, info_ptr);
    } else {
        // Read mode
        write_dummy_file(Data, Size);
        FILE *f = fopen("./dummy_file", "rb");
        if (!f) return 0;
        png_structp png_ptr = create_png_read_struct(f);
        if (!png_ptr) {
            fclose(f);
            return 0;
        }
        png_infop info_ptr = NULL;
        try {
            if (setjmp(png_jmpbuf(png_ptr))) {
                cleanup_png_read(png_ptr, info_ptr, f);
                return 0;
            }
            info_ptr = png_create_info_struct(png_ptr);
            if (!info_ptr) {
                cleanup_png_read(png_ptr, info_ptr, f);
                return 0;
            }

            // Try to set compression buffer size (only valid during read setup)
            size_t buffer_size = 0;
            if (Size > 1) buffer_size = Data[1];
            if (Size > 2) buffer_size |= ((size_t)Data[2] << 8);
            if (Size > 3) buffer_size |= ((size_t)Data[3] << 16);
            if (Size > 4) buffer_size |= ((size_t)Data[4] << 24);
            if (buffer_size > 2147483647UL) buffer_size = 2147483647UL;
            png_set_compression_buffer_size(png_ptr, buffer_size);

            // Query buffer size
            size_t current_size = png_get_compression_buffer_size(png_ptr);
            (void)current_size;

            // Attempt to read (may fail on invalid data, which is expected)
            png_read_info(png_ptr, info_ptr);
            png_read_end(png_ptr, info_ptr);

        } catch (...) {
            cleanup_png_read(png_ptr, info_ptr, f);
            return 0;
        }
        cleanup_png_read(png_ptr, info_ptr, f);
    }

    return 0;
}