// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_image at pngwrite.c:659:1 in png.h
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
// png_set_PLTE at pngset.c:739:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_set_unknown_chunk_location at pngset.c:1592:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include <vector>
#include <fstream>

static void write_dummy_file(const uint8_t *data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        if (size > 0) {
            file.write(reinterpret_cast<const char*>(data), size);
        }
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare dummy file if needed
    write_dummy_file(Data, Size);

    // Initialize libpng structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Set up error handling to prevent crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Create a dummy file for writing
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Set basic image parameters
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_PALETTE, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Prepare palette data on stack to avoid heap allocation issues
    png_color palette[256];
    for (int i = 0; i < 256; ++i) {
        palette[i].red = Data[i % Size];
        palette[i].green = Data[(i + 1) % Size];
        palette[i].blue = Data[(i + 2) % Size];
    }

    // Fuzz png_set_PLTE
    int num_palette = (Size > 0) ? (Data[0] % 257) : 0;
    png_set_PLTE(png_ptr, info_ptr, palette, num_palette);

    // Prepare unknown chunks on stack
    png_unknown_chunk unknown_chunks[1];
    unknown_chunks[0].name[0] = 'x';
    unknown_chunks[0].name[1] = 'x';
    unknown_chunks[0].name[2] = 'x';
    unknown_chunks[0].name[3] = 'x';
    unknown_chunks[0].name[4] = '\0';
    unknown_chunks[0].data = const_cast<png_byte*>(Data);
    unknown_chunks[0].size = Size;
    unknown_chunks[0].location = (Size > 1) ? (Data[1] % 4) : 0;

    // Fuzz png_set_unknown_chunks
    png_set_unknown_chunks(png_ptr, info_ptr, unknown_chunks, 1);

    // Fuzz png_set_unknown_chunk_location
    png_set_unknown_chunk_location(png_ptr, info_ptr, 0, unknown_chunks[0].location);

    // Fuzz png_write_info_before_PLTE
    png_write_info_before_PLTE(png_ptr, info_ptr);

    // Fuzz png_write_info
    png_write_info(png_ptr, info_ptr);

    // Prepare image row data on stack
    png_byte row[1];
    row[0] = (Size > 0) ? Data[0] : 0;
    png_bytep row_ptr = row;

    // Fuzz png_write_image
    png_write_image(png_ptr, &row_ptr);

    // Fuzz png_write_end
    png_write_end(png_ptr, info_ptr);

    // Cleanup
    fclose(fp);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}