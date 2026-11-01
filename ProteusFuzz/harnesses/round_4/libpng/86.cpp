// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_compression_strategy at pngwrite.c:1207:1 in png.h
// png_set_compression_method at pngwrite.c:1253:1 in png.h
// png_write_sig at pngwutil.c:68:1 in png.h
// png_write_chunk_start at pngwutil.c:132:1 in png.h
// png_write_chunk_data at pngwutil.c:144:1 in png.h
// png_write_chunk_end at pngwutil.c:163:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
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

static void dummy_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Initialize a dummy file for writing
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);
    png_set_write_fn(png_ptr, NULL, dummy_write_data, NULL);

    // Fuzz compression strategy and method
    if (Size > 0) {
        int strategy = Data[0] % 10; // Limit to reasonable range
        png_set_compression_strategy(png_ptr, strategy);
    }
    if (Size > 1) {
        int method = Data[1] % 10; // Limit to reasonable range
        png_set_compression_method(png_ptr, method);
    }

    // Write PNG signature
    png_write_sig(png_ptr);

    // Create a dummy IHDR chunk to test chunk writing
    png_byte chunk_type[] = {0x49, 0x48, 0x44, 0x52}; // "IHDR"
    png_byte chunk_data[13] = {0, 0, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0}; // Dummy IHDR data

    // Start chunk
    png_write_chunk_start(png_ptr, chunk_type, 13);

    // Write chunk data
    if (Size > 2) {
        size_t data_len = Data[2] % 13;
        png_write_chunk_data(png_ptr, chunk_data, data_len);
    }

    // End chunk
    png_write_chunk_end(png_ptr);

    // Flush buffer
    png_write_flush(png_ptr);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);

    return 0;
}