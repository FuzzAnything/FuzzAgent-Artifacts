// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_read_info at pngread.c:102:1 in png.h
// png_start_read_image at pngread.c:291:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_read_rows at pngread.c:641:1 in png.h
// png_read_rows at pngread.c:641:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_read_rows at pngread.c:641:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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
#include <vector>
#include <cstdlib>

static void static_cleanup(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}

static void static_write_to_dummy(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Create a dummy file for file-based operations
    static_write_to_dummy(Data, Size);

    // Fuzzing png_write_rows, png_set_swap, png_set_swap_alpha
    {
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!png_ptr) return 0;

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_write_struct(&png_ptr, nullptr);
            return 0;
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return 0;
        }

        // Initialize with minimal valid parameters
        png_byte dummy_row[256];
        memset(dummy_row, 0, sizeof(dummy_row));
        png_byte *row_ptr = dummy_row;
        png_byte **rows = &row_ptr;

        // Test png_set_swap
        png_set_swap(png_ptr);

        // Test png_set_swap_alpha
        png_set_swap_alpha(png_ptr);

        // Test png_write_rows with NULL png_ptr (safe)
        png_write_rows(nullptr, rows, 1);

        // Test png_write_rows with valid png_ptr but uninitialized file (will fail safely)
        png_write_rows(png_ptr, rows, 1);

        static_cleanup(png_ptr, info_ptr);
    }

    // Fuzzing png_read_rows, png_read_row, png_start_read_image
    {
        FILE *f = fopen("./dummy_file", "rb");
        if (!f) return 0;

        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!png_ptr) {
            fclose(f);
            return 0;
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, nullptr, nullptr);
            fclose(f);
            return 0;
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            fclose(f);
            return 0;
        }

        png_init_io(png_ptr, f);

        // Attempt to read header and info
        png_read_info(png_ptr, info_ptr);

        // Test png_start_read_image
        png_start_read_image(png_ptr);

        // Prepare row buffers
        png_uint_32 width = 1;
        png_uint_32 height = 1;
        png_byte *row = new (std::nothrow) png_byte[width * 4]; // Max bytes per pixel for RGBA
        png_byte *display_row = new (std::nothrow) png_byte[width * 4];
        if (!row || !display_row) {
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            fclose(f);
            return 0;
        }

        // Test png_read_row
        png_read_row(png_ptr, row, display_row);

        // Test png_read_rows
        png_byte **row_ptr = &row;
        png_byte **display_row_ptr = &display_row;
        png_read_rows(png_ptr, row_ptr, display_row_ptr, 1);

        // Test with NULL display_row
        png_read_rows(png_ptr, row_ptr, nullptr, 1);

        // Test with NULL png_ptr
        png_read_row(nullptr, row, display_row);
        png_read_rows(nullptr, row_ptr, display_row_ptr, 1);

        delete[] row;
        delete[] display_row;
        static_cleanup(png_ptr, info_ptr);
        fclose(f);
    }

    return 0;
}