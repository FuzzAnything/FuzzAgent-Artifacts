// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_chunk_start at pngwutil.c:132:1 in png.h
// png_write_chunk_data at pngwutil.c:144:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_sig at pngwutil.c:68:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
// png_write_chunk at pngwutil.c:209:1 in png.h
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

// Custom write callback to write to a memory buffer or file
static void write_to_file(png_structp png_ptr, png_bytep data, png_size_t length) {
    FILE *fp = static_cast<FILE*>(png_get_io_ptr(png_ptr));
    if (fp) {
        fwrite(data, 1, length, fp);
    }
}

// Custom error handler to prevent abort
static void error_handler(png_structp png_ptr, png_const_charp error_msg) {
    // In fuzzing, we often want to catch errors without aborting
    (void)png_ptr;
    (void)error_msg;
    // Do nothing or log if needed
}

// Custom warning handler
static void warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a temporary file for writing
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }

    // Initialize PNG write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    // Initialize PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        fclose(fp);
        return 0;
    }

    // Set error handlers to prevent abort
    png_set_error_fn(png_ptr, nullptr, error_handler, warning_handler);

    // Set I/O callback
    png_set_write_fn(png_ptr, fp, write_to_file, nullptr);

    // Create a minimal image info
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Try to write signature (png_write_sig)
    png_write_sig(png_ptr);

    // Try to write info
    png_write_info(png_ptr, info_ptr);

    // Prepare a dummy row of data (3 bytes for RGB 1x1 image)
    std::vector<png_byte> row_data(3, 0);
    if (Size > 0) {
        row_data[0] = Data[0];
        if (Size > 1) row_data[1] = Data[1];
        if (Size > 2) row_data[2] = Data[2];
    }

    // Try to write a row (png_write_row)
    png_write_row(png_ptr, row_data.data());

    // Try to flush (png_write_flush)
    png_write_flush(png_ptr);

    // Try to write a custom chunk (png_write_chunk)
    const png_byte chunk_name[] = {'t', 'E', 'S', 'T'};
    const png_byte *chunk_data = Data;
    size_t chunk_length = Size > 10 ? 10 : Size;
    png_write_chunk(png_ptr, chunk_name, chunk_data, chunk_length);

    // Try to write chunk start and data separately (png_write_chunk_start, png_write_chunk_data)
    const png_byte chunk_name2[] = {'t', 'E', 'S', 'T'};
    png_uint_32 chunk_length2 = Size > 20 ? 20 : Size;
    png_write_chunk_start(png_ptr, chunk_name2, chunk_length2);
    if (Size > 0) {
        png_write_chunk_data(png_ptr, Data, chunk_length2);
    }

    // End the PNG write
    png_write_end(png_ptr, info_ptr);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);

    return 0;
}