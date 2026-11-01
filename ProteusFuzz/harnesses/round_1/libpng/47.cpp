// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_set_expand_gray_1_2_4_to_8 at pngrtran.c:957:1 in png.h
// png_set_bgr at pngtrans.c:20:1 in png.h
// png_set_expand_16 at pngrtran.c:985:1 in png.h
// png_set_strip_alpha at pngrtran.c:198:1 in png.h
// png_set_strip_16 at pngrtran.c:185:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
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
#include <cstdio>
#include <cstring>

static void write_dummy_png(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    if (size > 0) {
        fwrite(data, 1, size, f);
    } else {
        // Write minimal valid PNG header if input is empty to ensure file exists
        const uint8_t png_header[8] = {137, 80, 78, 71, 13, 10, 26, 10};
        fwrite(png_header, 1, 8, f);
    }
    fclose(f);
}

static void safe_cleanup(png_structp png_ptr, png_infop info_ptr, FILE *fp) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    }
    if (fp) {
        fclose(fp);
    }
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        // Handle empty input by creating a minimal dummy file
        write_dummy_png(nullptr, 0);
    } else {
        write_dummy_png(Data, Size);
    }

    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return 0;
    }

    // Use setjmp/longjmp for error handling as required by libpng
    if (setjmp(png_jmpbuf(png_ptr))) {
        safe_cleanup(png_ptr, info_ptr, fp);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the header and info to ensure the file is somewhat valid
    // We do not check for errors here strictly, as the fuzzer will explore invalid states
    png_read_info(png_ptr, info_ptr);

    // Apply the target transformation APIs
    // 1. png_set_gray_to_rgb
    png_set_gray_to_rgb(png_ptr);

    // 2. png_set_expand_gray_1_2_4_to_8
    png_set_expand_gray_1_2_4_to_8(png_ptr);

    // 3. png_set_bgr
    png_set_bgr(png_ptr);

    // 4. png_set_expand_16
    png_set_expand_16(png_ptr);

    // 5. png_set_strip_alpha
    png_set_strip_alpha(png_ptr);

    // 6. png_set_strip_16
    png_set_strip_16(png_ptr);

    // Update info after transformations
    png_read_update_info(png_ptr, info_ptr);

    // Try to read a single row to trigger processing logic
    // We allocate a buffer based on the image width if available
    png_bytep row = NULL;
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);

    if (width > 0 && height > 0 && row_bytes > 0) {
        // Limit allocation to prevent OOM on malformed data claiming huge dimensions
        if (row_bytes < 1024 * 1024) {
            row = (png_bytep)malloc(row_bytes);
            if (row) {
                // Read one row to exercise the transformation logic
                png_read_row(png_ptr, row, NULL);
                free(row);
            }
        }
    }

    // Cleanup
    safe_cleanup(png_ptr, info_ptr, fp);
    return 0;
}