// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_chunk at pngwutil.c:209:1 in png.h
// png_write_chunk_start at pngwutil.c:132:1 in png.h
// png_write_chunk_data at pngwutil.c:144:1 in png.h
// png_write_chunk_start at pngwutil.c:132:1 in png.h
// png_write_chunk_data at pngwutil.c:144:1 in png.h
// png_write_chunk_end at pngwutil.c:163:1 in png.h
// png_write_sig at pngwutil.c:68:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_write_png at pngwrite.c:1376:1 in png.h
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
#include "code/png.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void dummy_png_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function that does nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_png_flush(png_structp png_ptr) {
    // Dummy flush function that does nothing
    (void)png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    // Create dummy file for writing
    FILE *fp = fopen("./dummy_file", "wb");
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
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 0;
    }
    
    // Set up custom write functions
    png_set_write_fn(png_ptr, fp, dummy_png_write_data, dummy_png_flush);
    
    // Use fuzz data to determine which function to test and with what parameters
    if (Size == 0) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 0;
    }
    
    // Use first byte to select function
    uint8_t function_selector = Data[0];
    const uint8_t* remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    switch (function_selector % 6) {
        case 0: {
            // Test png_write_png
            int transforms = 0;
            if (remaining_size >= sizeof(int)) {
                memcpy(&transforms, remaining_data, sizeof(int));
            }
            png_write_png(png_ptr, info_ptr, transforms, nullptr);
            break;
        }
        case 1: {
            // Test png_write_flush
            png_write_flush(png_ptr);
            break;
        }
        case 2: {
            // Test png_write_chunk
            if (remaining_size >= 4) {
                png_byte chunk_name[4];
                memcpy(chunk_name, remaining_data, 4);
                size_t data_length = 0;
                const png_byte* chunk_data = nullptr;
                
                if (remaining_size > 4) {
                    data_length = remaining_size - 4;
                    chunk_data = remaining_data + 4;
                }
                
                png_write_chunk(png_ptr, chunk_name, chunk_data, data_length);
            }
            break;
        }
        case 3: {
            // Test png_write_chunk_data
            // First need to start a chunk
            png_byte chunk_name[] = {'t', 'E', 'S', 't'};
            png_write_chunk_start(png_ptr, chunk_name, remaining_size);
            png_write_chunk_data(png_ptr, remaining_data, remaining_size);
            break;
        }
        case 4: {
            // Test png_write_chunk_end
            // First need to start and write some chunk data
            png_byte chunk_name[] = {'t', 'E', 'S', 't'};
            png_write_chunk_start(png_ptr, chunk_name, remaining_size);
            if (remaining_size > 0) {
                png_write_chunk_data(png_ptr, remaining_data, remaining_size);
            }
            png_write_chunk_end(png_ptr);
            break;
        }
        case 5: {
            // Test png_write_sig
            png_write_sig(png_ptr);
            break;
        }
    }
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    
    return 0;
}