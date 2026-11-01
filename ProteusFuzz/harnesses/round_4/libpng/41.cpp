// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_tRNS_to_alpha at pngrtran.c:969:1 in png.h
// png_set_strip_alpha at pngrtran.c:198:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_set_packing at pngtrans.c:49:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_set_strip_16 at pngrtran.c:185:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
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
#include <cstdlib>
#include <cstring>

static void write_dummy_png(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    
    // Write a minimal valid PNG header
    uint8_t png_header[] = {
        0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A
    };
    fwrite(png_header, 1, 8, f);
    
    // Write IHDR chunk (minimal valid header)
    // Length: 13 bytes
    uint32_t length = 13;
    fwrite(&length, 4, 1, f);
    // Type: IHDR
    const char type[] = "IHDR";
    fwrite(type, 1, 4, f);
    // Data: Width (1), Height (1), Bit depth (8), Color type (2 RGB), etc.
    uint32_t width = 1;
    uint32_t height = 1;
    uint8_t bit_depth = 8;
    uint8_t color_type = 2; // RGB
    uint8_t compression = 0;
    uint8_t filter = 0;
    uint8_t interlace = 0;
    fwrite(&width, 4, 1, f);
    fwrite(&height, 4, 1, f);
    fwrite(&bit_depth, 1, 1, f);
    fwrite(&color_type, 1, 1, f);
    fwrite(&compression, 1, 1, f);
    fwrite(&filter, 1, 1, f);
    fwrite(&interlace, 1, 1, f);
    // CRC (dummy value)
    uint32_t crc = 0;
    fwrite(&crc, 4, 1, f);
    
    // Write IDAT chunk (minimal compressed data)
    // We'll just write some dummy data if available, otherwise minimal
    size_t data_size = size > 0 ? size : 1;
    if (data_size > 0) {
        uint32_t idat_len = (uint32_t)data_size;
        fwrite(&idat_len, 4, 1, f);
        const char idat_type[] = "IDAT";
        fwrite(idat_type, 1, 4, f);
        fwrite(data, 1, data_size, f);
        fwrite(&crc, 4, 1, f);
    } else {
        // Minimal IDAT
        uint32_t idat_len = 1;
        fwrite(&idat_len, 4, 1, f);
        const char idat_type[] = "IDAT";
        fwrite(idat_type, 1, 4, f);
        uint8_t dummy = 0x00;
        fwrite(&dummy, 1, 1, f);
        fwrite(&crc, 4, 1, f);
    }
    
    // Write IEND chunk
    uint32_t iend_len = 0;
    fwrite(&iend_len, 4, 1, f);
    const char iend_type[] = "IEND";
    fwrite(iend_type, 1, 4, f);
    fwrite(&crc, 4, 1, f);
    
    fclose(f);
}

static void read_png_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    FILE *f = (FILE*)png_get_io_ptr(png_ptr);
    if (f) {
        if (fread(data, 1, length, f) != length) {
            png_error(png_ptr, "Read Error");
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    write_dummy_png(Data, Size);
    
    FILE *f = fopen("./dummy_file", "rb");
    if (!f) return 0;
    
    // Create read struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(f);
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(f);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(f);
        return 0;
    }
    
    png_set_read_fn(png_ptr, f, read_png_callback);
    
    // Read the IHDR chunk
    png_read_info(png_ptr, info_ptr);
    
    // Apply various transformations
    // 1. png_set_tRNS_to_alpha
    png_set_tRNS_to_alpha(png_ptr);
    
    // 2. png_set_strip_alpha
    png_set_strip_alpha(png_ptr);
    
    // 3. png_set_swap_alpha
    png_set_swap_alpha(png_ptr);
    
    // 4. png_set_packing
    png_set_packing(png_ptr);
    
    // 5. png_set_gray_to_rgb
    png_set_gray_to_rgb(png_ptr);
    
    // 6. png_set_strip_16
    png_set_strip_16(png_ptr);
    
    // Try to read image data (may fail due to invalid data, but that's expected in fuzzing)
    png_bytep row = (png_bytep)malloc(png_get_rowbytes(png_ptr, info_ptr));
    if (row) {
        for (png_uint_32 y = 0; y < png_get_image_height(png_ptr, info_ptr); ++y) {
            png_read_row(png_ptr, row, NULL);
        }
        free(row);
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(f);
    
    return 0;
}