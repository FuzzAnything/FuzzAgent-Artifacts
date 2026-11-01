// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_interlace_type at pngget.c:107:1 in png.h
// png_get_signature at pngget.c:491:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void dummy_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file to simulate reading from a file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Reopen for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        return 0;
    }

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    // Set error handlers to prevent crashes
    png_set_error_fn(png_ptr, NULL, dummy_error_handler, dummy_warning_handler);

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return 0;
    }

    // Use setjmp/longjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Try to read the IHDR chunk
    png_read_info(png_ptr, info_ptr);

    // Fuzz target API functions
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    (void)bit_depth;

    png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    (void)interlace_type;

    const png_byte *signature = png_get_signature(png_ptr, info_ptr);
    if (signature) {
        // Access signature to ensure it's valid
        volatile png_byte sig_byte = signature[0];
        (void)sig_byte;
    }

    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    (void)color_type;

    png_byte channels = png_get_channels(png_ptr, info_ptr);
    (void)channels;

    // Attempt to read the full image to test png_get_rows
    png_byte **row_pointers = NULL;
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_IDAT)) {
        png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
        row_pointers = png_get_rows(png_ptr, info_ptr);
        if (row_pointers) {
            // Access a small portion to ensure validity
            volatile png_byte *row = row_pointers[0];
            (void)row;
        }
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    // Remove the dummy file
    remove(filename);

    return 0;
}