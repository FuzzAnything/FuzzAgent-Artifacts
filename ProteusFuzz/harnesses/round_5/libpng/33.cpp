// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_chunk_start at pngwutil.c:132:1 in png.h
// png_write_chunk_data at pngwutil.c:144:1 in png.h
// png_write_chunk_end at pngwutil.c:163:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_compression_method at pngwrite.c:1253:1 in png.h
// png_set_compression_strategy at pngwrite.c:1207:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
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
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void write_to_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    FILE *fp = NULL;
    png_byte *row_data = NULL;

    // Create a dummy file to write to
    write_to_file(Data, Size);
    fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }

    // Initialize libpng write structure
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Cleanup on error
        if (row_data) free(row_data);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, fp);

    // Set a minimal IHDR chunk manually to allow further writes
    // We create a minimal header: width=1, height=1, bit_depth=8, color_type=0, etc.
    png_uint_32 width = 1;
    png_uint_32 height = 1;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_GRAY;
    int compression_method = 0;
    int filter_method = 0;
    int interlace_method = 0;

    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_method, compression_method, filter_method);

    // Fuzz target: png_set_compression_method
    int method = Data[0];
    png_set_compression_method(png_ptr, method);

    // Fuzz target: png_set_compression_strategy
    int strategy = Data[1];
    png_set_compression_strategy(png_ptr, strategy);

    // Fuzz target: png_set_swap (only effective for 16-bit, but safe to call)
    png_set_swap(png_ptr);

    // Write the info header
    png_write_info(png_ptr, info_ptr);

    // Prepare dummy row data for png_write_rows
    row_data = (png_byte *)malloc(width);
    if (!row_data) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 0;
    }
    memset(row_data, Data[2], width);

    png_byte *row_ptr = row_data;
    png_byte **row = &row_ptr;
    png_uint_32 num_rows = 1;

    // Fuzz target: png_write_rows
    png_write_rows(png_ptr, row, num_rows);

    // Fuzz target: png_write_chunk_start, png_write_chunk_data, png_write_chunk_end
    // We manually start a chunk, write some data, and end it.
    // Chunk type: "tEXt" (4 bytes)
    png_byte chunk_type[] = {'t', 'E', 'X', 't'};
    png_uint_32 chunk_length = (Size > 4) ? (png_uint_32)(Data[3] & 0x0F) : 0;

    // Start chunk
    png_write_chunk_start(png_ptr, chunk_type, chunk_length);

    // Write chunk data
    const png_byte *chunk_data = (Size > 4) ? &Data[4] : NULL;
    png_write_chunk_data(png_ptr, chunk_data, chunk_length);

    // End chunk (writes CRC)
    png_write_chunk_end(png_ptr);

    // End the image
    png_write_end(png_ptr, info_ptr);

    // Cleanup
    if (row_data) {
        free(row_data);
    }
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);

    return 0;
}