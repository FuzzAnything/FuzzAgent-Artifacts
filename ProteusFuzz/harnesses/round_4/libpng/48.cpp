// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_interlace_type at pngget.c:107:1 in png.h
// png_get_signature at pngget.c:491:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_filter_type at pngget.c:98:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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

static void fuzz_png_read_and_query(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;

    // Create a dummy file to simulate input
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) return;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Reopen for reading
    fp = fopen(filename, "rb");
    if (!fp) return;

    // Initialize PNG structures
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

    // Set up error handling to avoid crashes on invalid data
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the signature and info
    // We do not call png_set_sig_bytes to allow the library to read the full signature
    // from the file, which is more robust for fuzzing malformed headers.
    png_read_info(png_ptr, info_ptr);

    // Call the target API functions
    // 1. png_get_bit_depth
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    (void)bit_depth;

    // 2. png_get_interlace_type
    png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    (void)interlace_type;

    // 3. png_get_signature
    const png_byte *signature = png_get_signature(png_ptr, info_ptr);
    if (signature) {
        // Just access to ensure no crash, no need to use data
        (void)signature[0];
    }

    // 4. png_get_compression_type
    png_byte compression_type = png_get_compression_type(png_ptr, info_ptr);
    (void)compression_type;

    // 5. png_get_color_type
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    (void)color_type;

    // 6. png_get_filter_type
    png_byte filter_type = png_get_filter_type(png_ptr, info_ptr);
    (void)filter_type;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    fuzz_png_read_and_query(Data, Size);
    return 0;
}