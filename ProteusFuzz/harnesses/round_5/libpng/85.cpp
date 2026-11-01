// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
// png_write_chunk_start at pngwutil.c:132:1 in png.h
// png_write_chunk_data at pngwutil.c:144:1 in png.h
// png_write_chunk at pngwutil.c:209:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_write_sig at pngwutil.c:68:1 in png.h
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
#include <cstdlib>

static void dummy_write_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
    // Do not actually write to a file to avoid I/O overhead in fuzzing
}

static void dummy_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Silently ignore errors to prevent abort during fuzzing
}

static void dummy_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Silently ignore warnings
}

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for potential file-based operations if needed
    // Although we use callbacks, this satisfies the requirement to handle file scenarios
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fclose(fp);
    }

    // Initialize libpng structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    // Set up error handling to avoid crashes
    png_set_error_fn(png_ptr, NULL, dummy_error_callback, dummy_warning_callback);

    // Use setjmp for safe error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Initialize the write structure with a dummy callback (no actual file I/O)
    png_init_io(png_ptr, NULL);
    png_set_write_fn(png_ptr, NULL, dummy_write_callback, NULL);

    // Extract basic image parameters from fuzz input if possible
    // We'll use a small fixed size for safety, derived from input bytes
    png_uint_32 width = 1;
    png_uint_32 height = 1;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;

    if (Size >= 4) {
        width = Data[0] ? Data[0] : 1;
        height = Data[1] ? Data[1] : 1;
        bit_depth = (Data[2] % 8) + 1; // 1 to 8
        color_type = Data[3] % 10;     // 0 to 9
    }

    // Ensure valid color types
    if (color_type != PNG_COLOR_TYPE_GRAY &&
        color_type != PNG_COLOR_TYPE_RGB &&
        color_type != PNG_COLOR_TYPE_PALETTE &&
        color_type != PNG_COLOR_TYPE_GRAY_ALPHA &&
        color_type != PNG_COLOR_TYPE_RGB_ALPHA) {
        color_type = PNG_COLOR_TYPE_RGB;
    }

    // Write the PNG signature
    png_write_sig(png_ptr);

    // Write the IHDR chunk manually to simulate png_write_info behavior for testing
    // Since we don't have a full info struct setup, we'll try to write a minimal header
    // Note: In real usage, png_write_info is required before png_write_row
    // Here we attempt to trigger the error path if png_write_info is missing
    
    // Prepare a dummy row buffer
    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    if (row_bytes == 0) {
        // Fallback for uninitialized info
        row_bytes = width * 3; // Assume RGB
    }
    
    // Allocate row buffer
    png_byte *row = (png_byte *)malloc(row_bytes);
    if (!row) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Fill row with fuzz data
    size_t data_len = (Size > row_bytes) ? row_bytes : Size;
    memcpy(row, Data, data_len);
    if (data_len < row_bytes) {
        memset(row + data_len, 0, row_bytes - data_len);
    }

    // Test png_write_row without png_write_info (should trigger error handling)
    png_write_row(png_ptr, row);

    // Now properly initialize info to test successful paths
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Write info header
    png_write_info(png_ptr, info_ptr);

    // Test png_write_row again (should succeed)
    png_write_row(png_ptr, row);

    // Test png_write_flush
    png_write_flush(png_ptr);

    // Test custom chunk writing
    const png_byte chunk_name[] = "tEXt";
    const png_byte chunk_data[] = "test";
    size_t chunk_len = sizeof(chunk_data) - 1;

    // Test png_write_chunk_start and png_write_chunk_data
    png_write_chunk_start(png_ptr, chunk_name, chunk_len);
    png_write_chunk_data(png_ptr, chunk_data, chunk_len);
    // Note: png_write_chunk_end is not in the target list, but required for valid PNG
    // We rely on libpng's internal state to handle incomplete chunks gracefully or error

    // Test png_write_chunk directly
    png_write_chunk(png_ptr, chunk_name, chunk_data, chunk_len);

    // Write another row to test interlacing or multi-row scenarios
    png_write_row(png_ptr, row);

    // Cleanup
    free(row);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}