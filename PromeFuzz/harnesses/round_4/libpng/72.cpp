// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_image at pngwrite.c:659:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_write_chunk_start at pngwutil.c:132:1 in png.h
// png_write_chunk_data at pngwutil.c:144:1 in png.h
// png_write_chunk_end at pngwutil.c:163:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_write_chunk at pngwutil.c:209:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "png.h"

static void dummy_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function that does nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush_data(png_structp png_ptr) {
    // Dummy flush function that does nothing
    (void)png_ptr;
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Dummy error handler
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Dummy warning handler
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create dummy file for writing
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;

    // Create write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                   nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        fclose(fp);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 0;
    }

    // Set up custom write functions
    png_set_write_fn(png_ptr, fp, dummy_write_data, dummy_flush_data);

    // Use first byte to determine which function to test
    uint8_t selector = Data[0] % 6;
    Data++;
    Size--;

    switch (selector) {
        case 0: { // png_write_chunk
            if (Size >= 4) {
                png_byte chunk_name[4];
                memcpy(chunk_name, Data, 4);
                size_t data_len = Size > 4 ? Size - 4 : 0;
                const png_byte* chunk_data = data_len > 0 ? Data + 4 : nullptr;
                png_write_chunk(png_ptr, chunk_name, chunk_data, data_len);
            }
            break;
        }
        case 1: { // png_write_image
            if (Size >= 64) {
                // Create a simple 8x8 grayscale image
                png_set_IHDR(png_ptr, info_ptr, 8, 8, 8, 
                            PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
                            PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
                png_write_info_before_PLTE(png_ptr, info_ptr);
                
                png_byte rows[8][8];
                png_byte* row_pointers[8];
                
                // Fill image data from fuzzer input
                for (int i = 0; i < 8 && i * 8 < (int)Size; i++) {
                    size_t copy_len = Size - i * 8 < 8 ? Size - i * 8 : 8;
                    memcpy(rows[i], Data + i * 8, copy_len);
                    row_pointers[i] = rows[i];
                }
                
                png_write_image(png_ptr, row_pointers);
                png_write_end(png_ptr, info_ptr);
            }
            break;
        }
        case 2: { // png_write_chunk_start
            if (Size >= 4) {
                png_byte chunk_name[4];
                memcpy(chunk_name, Data, 4);
                png_uint_32 length = Size > 4 ? (png_uint_32)(Size - 4) : 0;
                png_write_chunk_start(png_ptr, chunk_name, length);
                
                // Write chunk data if available
                if (length > 0 && Size > 4) {
                    png_write_chunk_data(png_ptr, Data + 4, length);
                }
                png_write_chunk_end(png_ptr);
            }
            break;
        }
        case 3: { // png_write_info_before_PLTE
            // Set up basic image info
            png_set_IHDR(png_ptr, info_ptr, 16, 16, 8, 
                        PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
                        PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
            png_write_info_before_PLTE(png_ptr, info_ptr);
            break;
        }
        case 4: { // png_write_row
            if (Size >= 16) {
                // Set up basic image info first
                png_set_IHDR(png_ptr, info_ptr, 16, 16, 8, 
                            PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
                            PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
                png_write_info_before_PLTE(png_ptr, info_ptr);
                
                // Write a single row
                png_byte row[16];
                size_t copy_len = Size < 16 ? Size : 16;
                memcpy(row, Data, copy_len);
                png_write_row(png_ptr, row);
            }
            break;
        }
        case 5: { // png_write_rows
            if (Size >= 32) {
                // Set up basic image info first
                png_set_IHDR(png_ptr, info_ptr, 8, 4, 8, 
                            PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
                            PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
                png_write_info_before_PLTE(png_ptr, info_ptr);
                
                // Create rows
                png_byte rows[4][8];
                png_byte* row_pointers[4];
                
                for (int i = 0; i < 4 && i * 8 < (int)Size; i++) {
                    size_t copy_len = Size - i * 8 < 8 ? Size - i * 8 : 8;
                    memcpy(rows[i], Data + i * 8, copy_len);
                    row_pointers[i] = rows[i];
                }
                
                png_write_rows(png_ptr, row_pointers, 4);
            }
            break;
        }
    }

    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    
    return 0;
}