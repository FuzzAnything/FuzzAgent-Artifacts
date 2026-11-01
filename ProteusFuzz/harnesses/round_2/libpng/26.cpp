// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_read_rows at pngread.c:641:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_interlace_type at pngget.c:107:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_get_current_pass_number at pngtrans.c:859:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
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
#include <cstdio>
#include <cstring>
#include <vector>

static void fuzz_png_read(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;

    // Write fuzzing data to a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    fp = fopen("./dummy_file", "rb");
    if (!fp) return;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);

    // Read PNG info (headers)
    png_read_info(png_ptr, info_ptr);

    // Get interlace type
    png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);

    // Get image dimensions
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);

    // Avoid allocating for invalid dimensions
    if (width == 0 || height == 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return;
    }

    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    
    // Allocate row pointers using raw array to match png_byte ** signature
    std::vector<png_bytep> row_pointers(height);
    for (png_uint_32 y = 0; y < height; ++y) {
        row_pointers[y] = new png_byte[row_bytes];
    }

    // Test png_read_image
    // Cast vector data pointer to png_byte ** to satisfy API
    png_read_image(png_ptr, row_pointers.data());

    // Test png_get_current_pass_number
    png_byte pass_num = png_get_current_pass_number(png_ptr);

    // Test png_read_row (single row read)
    // Note: png_read_row expects to be called in a loop for interlaced images, 
    // but for fuzzing we just invoke it once to exercise the code path.
    if (height > 0 && row_pointers[0]) {
        png_byte *display_row = new png_byte[row_bytes];
        png_read_row(png_ptr, row_pointers[0], display_row);
        delete[] display_row;
    }

    // Test png_read_rows (multiple rows)
    png_uint_32 num_rows = height > 2 ? 2 : height;
    if (num_rows > 0) {
        std::vector<png_byte*> row_buf(num_rows);
        std::vector<png_byte*> display_buf(num_rows);
        for (png_uint_32 i = 0; i < num_rows; ++i) {
            row_buf[i] = new png_byte[row_bytes];
            display_buf[i] = new png_byte[row_bytes];
        }
        png_read_rows(png_ptr, row_buf.data(), display_buf.data(), num_rows);
        for (png_uint_32 i = 0; i < num_rows; ++i) {
            delete[] row_buf[i];
            delete[] display_buf[i];
        }
    }

    // Cleanup
    for (png_uint_32 y = 0; y < height; ++y) {
        delete[] row_pointers[y];
    }
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    fuzz_png_read(Data, Size);
    return 0;
}