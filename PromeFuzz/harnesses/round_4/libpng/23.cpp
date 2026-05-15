// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_write_sig at pngwutil.c:68:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_chunk at pngwutil.c:209:1 in png.h
// png_write_png at pngwrite.c:1376:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
// png_write_chunk_end at pngwutil.c:163:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "png.h"

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing - discard written data
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Do nothing
}

static png_byte dummy_row[1024]; // Static buffer for row data

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    // Initialize dummy file if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    
    // Write fuzzer data to dummy file
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);
    
    // Create PNG write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;
    
    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Set up write function to discard output
    png_set_write_fn(png_ptr, nullptr, dummy_write_fn, dummy_flush_fn);
    
    // Test png_write_sig
    png_write_sig(png_ptr);
    
    // Set up a minimal valid PNG header for testing
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_GRAY,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    
    // Test png_write_chunk with fuzzer data
    if (Size >= 4) {
        const png_byte chunk_name[4] = {
            static_cast<png_byte>(Data[0]),
            static_cast<png_byte>(Data[1]),
            static_cast<png_byte>(Data[2]),
            static_cast<png_byte>(Data[3])
        };
        const png_byte* chunk_data = (Size > 4) ? Data + 4 : nullptr;
        size_t chunk_length = (Size > 4) ? Size - 4 : 0;
        png_write_chunk(png_ptr, chunk_name, chunk_data, chunk_length);
    }
    
    // Test png_write_png with various transformations
    int transforms = 0;
    if (Size > 0) {
        transforms = Data[0] % 16; // Use first byte to select transforms
    }
    png_write_png(png_ptr, info_ptr, transforms, nullptr);
    
    // Test png_write_row with dummy data
    size_t row_size = 1; // For 1x1 grayscale image
    if (Size > 0 && Size < sizeof(dummy_row)) {
        row_size = Size % sizeof(dummy_row);
        if (row_size == 0) row_size = 1;
        memcpy(dummy_row, Data, row_size);
    }
    png_write_row(png_ptr, dummy_row);
    
    // Test png_write_flush
    png_write_flush(png_ptr);
    
    // Test png_write_chunk_end (requires chunk start, but we'll test anyway)
    png_write_chunk_end(png_ptr);
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}