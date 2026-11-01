// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_chunk_start at pngwutil.c:132:1 in png.h
// png_write_chunk_data at pngwutil.c:144:1 in png.h
// png_write_chunk at pngwutil.c:209:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_sig at pngwutil.c:68:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
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

static void write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    FILE *fp = (FILE *)png_get_io_ptr(png_ptr);
    if (fp) {
        fwrite(data, 1, length, fp);
    }
}

static void write_data_custom(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Custom write function if needed, but we use standard file write here
    FILE *fp = (FILE *)png_get_io_ptr(png_ptr);
    if (fp) {
        fwrite(data, 1, length, fp);
    }
}

extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    // Create a dummy file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Set basic image info
    png_uint_32 width = (Data[0] % 100) + 1;
    png_uint_32 height = (Data[1] % 100) + 1;
    int bit_depth = (Data[2] % 8) + 1;
    int color_type = Data[3] % 6; // 0 to 5
    int interlace_type = (Data[4] % 2) ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE;

    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, interlace_type,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Write signature
    png_write_sig(png_ptr);

    // Write info
    png_write_info(png_ptr, info_ptr);

    // Prepare row data
    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    std::vector<png_byte> row(row_bytes, 0);

    // Fill row data with fuzz input
    for (size_t i = 0; i < row_bytes && i < Size - 5; ++i) {
        row[i] = Data[5 + i];
    }

    // Write rows
    for (png_uint_32 y = 0; y < height; ++y) {
        png_write_row(png_ptr, &row[0]);
        // Occasionally flush
        if ((y % 10) == 0) {
            png_write_flush(png_ptr);
        }
    }

    // Write custom chunk
    const png_byte chunk_name[] = {'t', 'E', 'X', 't'};
    const png_byte chunk_data[] = "test";
    png_write_chunk_start(png_ptr, chunk_name, 4);
    png_write_chunk_data(png_ptr, chunk_data, 4);
    png_write_chunk(png_ptr, chunk_name, chunk_data, 4);

    // End the image
    png_write_end(png_ptr, info_ptr);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    remove(filename);

    return 0;
}