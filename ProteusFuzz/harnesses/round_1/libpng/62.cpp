// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_interlace_type at pngget.c:107:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
// png_get_signature at pngget.c:491:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
// png_get_rows at pngget.c:50:1 in png.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    }
}

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

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

    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Read header info
    png_read_info(png_ptr, info_ptr);

    // Test png_get_interlace_type
    png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    (void)interlace_type;

    // Test png_get_bit_depth
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    (void)bit_depth;

    // Test png_get_color_type
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    (void)color_type;

    // Test png_get_channels
    png_byte channels = png_get_channels(png_ptr, info_ptr);
    (void)channels;

    // Test png_get_signature
    const png_byte *signature = png_get_signature(png_ptr, info_ptr);
    if (signature) {
        volatile png_byte first_byte = signature[0];
        (void)first_byte;
    }

    // Attempt to read full image to populate row pointers for png_get_rows
    if (!setjmp(png_jmpbuf(png_ptr))) {
        png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    }

    // Test png_get_rows
    png_byte **row_pointers = png_get_rows(png_ptr, info_ptr);
    if (row_pointers) {
        // Use a non-volatile pointer to avoid type mismatch with volatile
        png_byte **ptr = row_pointers;
        (void)ptr;
    }

    cleanup_png(png_ptr, info_ptr);
    fclose(fp);

    return 0;
}