// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_image at pngwrite.c:659:1 in png.h
// png_write_chunk_start at pngwutil.c:132:1 in png.h
// png_write_chunk at pngwutil.c:209:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
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

static void setup_png_structs(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr, const uint8_t *data, size_t size) {
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Create write struct
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return;

    // Create info struct
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return;
    }

    // Setup error handling to avoid abort
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        *png_ptr_ptr = nullptr;
        *info_ptr_ptr = nullptr;
        return;
    }

    // Open dummy file for writing
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        *png_ptr_ptr = nullptr;
        *info_ptr_ptr = nullptr;
        return;
    }

    png_init_io(png_ptr, f);

    // Set basic image parameters to allow writing
    // We use a minimal 1x1 image configuration
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Store pointers for the fuzzer to use
    *png_ptr_ptr = png_ptr;
    *info_ptr_ptr = info_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Initialize PNG structures
    setup_png_structs(&png_ptr, &info_ptr, Data, Size);
    
    if (!png_ptr || !info_ptr) {
        return 0;
    }

    // Write the info header
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    png_write_info(png_ptr, info_ptr);

    // Prepare row data for testing row writing functions
    // Create a small buffer for a single row (1 pixel, RGB = 3 bytes)
    std::vector<png_byte> row_data(3);
    if (Size >= 3) {
        row_data[0] = Data[0];
        row_data[1] = Data[1];
        row_data[2] = Data[2];
    } else {
        row_data[0] = 0; row_data[1] = 0; row_data[2] = 0;
    }

    // Correct pointer setup for png_write_rows and png_write_image
    // row_ptr points to the data buffer
    png_byte *row_ptr = row_data.data();
    // image_array is an array of row pointers (size 1)
    png_byte *image_array[1];
    image_array[0] = row_ptr;

    // Fuzz png_set_swap_alpha
    // This function modifies internal flags, safe to call multiple times
    png_set_swap_alpha(png_ptr);

    // Fuzz png_write_row
    // Write the single row
    png_write_row(png_ptr, row_data.data());

    // Fuzz png_write_rows
    // Write the same row again using the batch function
    png_write_rows(png_ptr, image_array, 1);

    // Fuzz png_write_image
    // This will attempt to write the image again (might fail due to state, but safe to test)
    // We reuse the image_array
    png_write_image(png_ptr, image_array);

    // Fuzz png_write_chunk_start and png_write_chunk
    // We need a valid chunk name (4 bytes)
    png_byte chunk_name[4] = {0x74, 0x45, 0x58, 0x74}; // "tEXt"
    
    // Safely populate chunk_name from input data if available
    if (Size >= 4) {
        // Use modulo to ensure we stay within bounds even for small sizes
        chunk_name[0] = Data[Size % 4];
        chunk_name[1] = Data[(Size + 1) % 4];
        chunk_name[2] = Data[(Size + 2) % 4];
        chunk_name[3] = Data[(Size + 3) % 4];
    }
    
    // Prepare dummy data for the chunk
    std::vector<png_byte> chunk_data(10);
    for (size_t i = 0; i < 10 && i < Size; ++i) {
        chunk_data[i] = Data[i];
    }

    // Test png_write_chunk_start
    // Note: This starts a chunk but doesn't finish it. 
    // If we don't finish it, the next write operation might fail or trigger an error.
    // However, libpng handles this via setjmp/longjmp if configured correctly.
    // We rely on the setjmp check in png_write_end or subsequent calls to catch errors.
    png_write_chunk_start(png_ptr, chunk_name, 10);
    
    // Test png_write_chunk (complete operation)
    // This writes a complete chunk including header, data, and CRC.
    png_write_chunk(png_ptr, chunk_name, chunk_data.data(), 10);

    // End the PNG file
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    png_write_end(png_ptr, info_ptr);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}