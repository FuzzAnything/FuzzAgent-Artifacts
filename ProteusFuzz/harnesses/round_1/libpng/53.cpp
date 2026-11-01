// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_compression_window_bits at pngwrite.c:1224:1 in png.h
// png_set_compression_mem_level at pngwrite.c:1196:1 in png.h
// png_set_compression_buffer_size at pngset.c:1865:1 in png.h
// png_set_text_compression_strategy at pngwrite.c:1295:1 in png.h
// png_get_compression_buffer_size at pngget.c:1279:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
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
#include <vector>

static void write_dummy_png_header(const uint8_t *data, size_t size, FILE *fp) {
    // Write a minimal valid PNG header (8 bytes signature + IHDR chunk)
    // Signature
    const uint8_t signature[] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(signature, 1, 8, fp);
    
    // IHDR chunk: length (4), type (4), data (13), CRC (4)
    uint32_t width = 1;
    uint32_t height = 1;
    uint8_t bit_depth = 8;
    uint8_t color_type = 2; // RGB
    uint8_t compression = 0;
    uint8_t filter = 0;
    uint8_t interlace = 0;
    
    uint32_t length = 13;
    fwrite(&length, 4, 1, fp);
    const char type[] = "IHDR";
    fwrite(type, 4, 1, fp);
    fwrite(&width, 4, 1, fp);
    fwrite(&height, 4, 1, fp);
    fwrite(&bit_depth, 1, 1, fp);
    fwrite(&color_type, 1, 1, fp);
    fwrite(&compression, 1, 1, fp);
    fwrite(&filter, 1, 1, fp);
    fwrite(&interlace, 1, 1, fp);
    // Dummy CRC (not validated by libpng in this context usually, but needed for structure)
    uint32_t crc = 0;
    fwrite(&crc, 4, 1, fp);
}

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a temporary file for the fuzzer to use
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }

    // Write a minimal valid PNG header to the file
    write_dummy_png_header(Data, Size, fp);
    fclose(fp);

    // Reopen for reading/writing as needed by libpng
    fp = fopen(filename, "r+b");
    if (!fp) {
        return 0;
    }

    // Create write struct
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        remove(filename);
        return 0;
    }

    // Create info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        remove(filename);
        return 0;
    }

    // Setup error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        remove(filename);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Use input data to derive parameters for fuzzing with bounds checking
    int window_bits = (Data[0] % 16) + 8; // Range 8-23, libpng clamps to 8-15
    int mem_level = 1;
    size_t buffer_size = 1024;
    int strategy = 0;

    if (Size > 1) {
        mem_level = (Data[1] % 9) + 1;    // Range 1-9
    }
    
    if (Size > 3) {
        buffer_size = ((Data[2] << 8) | Data[3]) + 1;
        // Clamp buffer_size to avoid invalid values (0 or > 2GB)
        if (buffer_size == 0) buffer_size = 1;
        if (buffer_size > 2147483647UL) buffer_size = 2147483647UL;
    }
    
    if (Size > 4) {
        strategy = Data[4] % 5; // Z_DEFAULT_STRATEGY to Z_FIXED (0-4)
    }

    // Fuzz png_set_compression_window_bits
    png_set_compression_window_bits(png_ptr, window_bits);

    // Fuzz png_set_compression_mem_level
    png_set_compression_mem_level(png_ptr, mem_level);

    // Fuzz png_set_compression_buffer_size
    png_set_compression_buffer_size(png_ptr, buffer_size);

    // Fuzz png_set_text_compression_strategy
    png_set_text_compression_strategy(png_ptr, strategy);

    // Fuzz png_get_compression_buffer_size
    size_t retrieved_size = png_get_compression_buffer_size(png_ptr);
    (void)retrieved_size; // Use variable to avoid unused warning

    // Fuzz png_write_flush
    // We need to write at least some data to make flush meaningful, 
    // but for fuzzing the API call itself, we can call it directly.
    // Note: Calling png_write_flush before png_write_info or png_write_row 
    // might not do much, but it tests the function's robustness.
    png_write_flush(png_ptr);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);

    // Remove dummy file
    remove(filename);

    return 0;
}