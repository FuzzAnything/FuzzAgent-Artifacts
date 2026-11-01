// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_interlace_type at pngget.c:107:1 in png.h
// png_get_signature at pngget.c:491:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
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
#include <stddef.h>
#include <cstring>
#include <cstdlib>

// Helper function to create a minimal valid PNG file for fuzzing
static bool create_dummy_png_file(const uint8_t *data, size_t size, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return false;

    // Write minimal PNG header (8 bytes signature)
    const uint8_t png_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(png_signature, 1, 8, f);

    // If input data is too small to form a valid IHDR, create a minimal one
    if (size < 25) {
        // Create a minimal IHDR chunk (13 bytes data + 4 bytes length + 4 bytes CRC placeholder)
        uint8_t ihdr[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        // Set width and height to 1
        ihdr[0] = 0; ihdr[1] = 0; ihdr[2] = 0; ihdr[3] = 1; // width
        ihdr[4] = 0; ihdr[5] = 0; ihdr[6] = 0; ihdr[7] = 1; // height
        ihdr[8] = 8; // bit depth
        ihdr[9] = 2; // color type (RGB)
        ihdr[10] = 0; // compression
        ihdr[11] = 0; // filter
        ihdr[12] = 0; // interlace

        uint32_t len = 13;
        fwrite(&len, 4, 1, f);
        fwrite("IHDR", 4, 1, f);
        fwrite(ihdr, 1, 13, f);
        // Dummy CRC (0)
        uint32_t crc = 0;
        fwrite(&crc, 4, 1, f);

        // Write IEND chunk
        len = 0;
        fwrite(&len, 4, 1, f);
        fwrite("IEND", 4, 1, f);
        fwrite(&crc, 4, 1, f);
    } else {
        // Write the provided data as the rest of the file
        fwrite(data, 1, size, f);
    }

    fclose(f);
    return true;
}

static void read_png_and_test(const char *filename, png_structp *png_ptr_out, png_infop *info_ptr_out) {
    FILE *fp = fopen(filename, "rb");
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

    // Set error handling to prevent crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);

    // Read the header
    png_read_info(png_ptr, info_ptr);

    fclose(fp);

    *png_ptr_out = png_ptr;
    *info_ptr_out = info_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const char *filename = "./dummy_file";
    
    // Create a dummy PNG file
    if (!create_dummy_png_file(Data, Size, filename)) {
        return 0;
    }

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    // Read the PNG file and get pointers
    read_png_and_test(filename, &png_ptr, &info_ptr);

    if (!png_ptr || !info_ptr) {
        return 0;
    }

    // Test the target API functions
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    (void)bit_depth; // Suppress unused variable warning

    png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    (void)interlace_type;

    const png_byte *signature = png_get_signature(png_ptr, info_ptr);
    (void)signature;

    png_byte compression_type = png_get_compression_type(png_ptr, info_ptr);
    (void)compression_type;

    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    (void)color_type;

    png_byte channels = png_get_channels(png_ptr, info_ptr);
    (void)channels;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}