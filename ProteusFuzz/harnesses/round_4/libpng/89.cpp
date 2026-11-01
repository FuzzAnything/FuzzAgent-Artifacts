// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_read_rows at pngread.c:641:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_start_read_image at pngread.c:291:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_get_current_pass_number at pngtrans.c:859:1 in png.h
// png_get_current_row_number at pngtrans.c:847:1 in png.h
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

static void cleanup_png_write(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test 1: png_read_row, png_read_rows, png_read_image, png_get_current_pass_number, png_get_current_row_number
    {
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!png_ptr) return 0;

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, nullptr, nullptr);
            return 0;
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            cleanup_png(png_ptr, info_ptr);
            return 0;
        }

        write_dummy_file(Data, Size);

        FILE *fp = fopen("./dummy_file", "rb");
        if (!fp) {
            cleanup_png(png_ptr, info_ptr);
            return 0;
        }

        png_init_io(png_ptr, fp);

        // Attempt to read IHDR
        png_uint_32 width = 0, height = 0;
        int bit_depth, color_type;
        png_read_info(png_ptr, info_ptr);
        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, nullptr, nullptr, nullptr);

        if (width == 0 || height == 0) {
            fclose(fp);
            cleanup_png(png_ptr, info_ptr);
            return 0;
        }

        // Allocate row buffers
        png_uint_32 row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        std::vector<png_byte> row_buffer(row_bytes);
        std::vector<png_byte> display_buffer(row_bytes);
        std::vector<png_byte*> row_ptrs(height);
        std::vector<png_byte*> display_ptrs(height);

        for (png_uint_32 i = 0; i < height; ++i) {
            row_ptrs[i] = row_buffer.data() + (i * row_bytes);
            display_ptrs[i] = display_buffer.data() + (i * row_bytes);
        }

        // Use png_start_read_image instead of png_read_start_row
        png_start_read_image(png_ptr);

        // Fuzz png_read_row
        for (png_uint_32 y = 0; y < height; ++y) {
            png_read_row(png_ptr, row_buffer.data(), display_buffer.data());
            png_byte pass = png_get_current_pass_number(png_ptr);
            (void)pass;
            png_uint_32 row_num = png_get_current_row_number(png_ptr);
            (void)row_num;
        }

        // Fuzz png_read_rows
        for (png_uint_32 y = 0; y < height; ++y) {
            png_read_rows(png_ptr, &row_ptrs[y], &display_ptrs[y], 1);
        }

        // Fuzz png_read_image (requires restarting read process or using fresh data, but we try)
        // Note: png_read_image expects to read the whole image from current state.
        // Since we already read rows, this might fail or do nothing, which is fine for fuzzing.
        png_read_image(png_ptr, row_ptrs.data());

        fclose(fp);
        cleanup_png(png_ptr, info_ptr);
    }

    // Test 2: png_write_rows (requires write context)
    {
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!png_ptr) return 0;

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_write_struct(&png_ptr, nullptr);
            return 0;
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            cleanup_png_write(png_ptr, info_ptr);
            return 0;
        }

        write_dummy_file(Data, Size);

        FILE *fp = fopen("./dummy_file", "wb");
        if (!fp) {
            cleanup_png_write(png_ptr, info_ptr);
            return 0;
        }

        png_init_io(png_ptr, fp);

        // Create minimal IHDR
        png_uint_32 width = 16;
        png_uint_32 height = 16;
        int bit_depth = 8;
        int color_type = PNG_COLOR_TYPE_RGB;
        png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                     PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
        png_write_info(png_ptr, info_ptr);

        // Allocate row buffers
        png_uint_32 row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        std::vector<png_byte> row_buffer(row_bytes);
        std::vector<png_byte*> row_ptrs(height);

        for (png_uint_32 i = 0; i < height; ++i) {
            row_ptrs[i] = row_buffer.data() + (i * row_bytes);
            memset(row_ptrs[i], 0, row_bytes);
        }

        // Fuzz png_write_rows
        png_write_rows(png_ptr, row_ptrs.data(), height);

        png_write_end(png_ptr, nullptr);
        fclose(fp);
        cleanup_png_write(png_ptr, info_ptr);
    }

    return 0;
}