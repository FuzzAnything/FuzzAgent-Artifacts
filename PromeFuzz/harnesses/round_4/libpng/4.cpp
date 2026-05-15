// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_write_png at pngwrite.c:1376:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "png.h"

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function that does nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Dummy flush function
    (void)png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Initialize dummy file if needed
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return 0;
    }
    
    // Write fuzzer data to dummy file
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    // Create write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }
    
    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Set up write function
    png_set_write_fn(png_ptr, NULL, dummy_write_fn, dummy_flush_fn);
    
    // Set image parameters using libpng API functions instead of direct access
    png_set_IHDR(png_ptr, info_ptr, 64, 64, 8, 
                 PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    
    // Test different API functions based on input data
    if (Size > 0) {
        uint8_t selector = Data[0] % 5;
        
        switch (selector) {
            case 0:
                // Test png_write_info
                png_write_info(png_ptr, info_ptr);
                break;
                
            case 1:
                // Test png_write_info_before_PLTE
                png_write_info_before_PLTE(png_ptr, info_ptr);
                break;
                
            case 2:
                // Test png_write_info then png_write_end
                png_write_info(png_ptr, info_ptr);
                png_write_end(png_ptr, info_ptr);
                break;
                
            case 3:
                // Test png_write_png with different transforms
                {
                    int transforms = PNG_TRANSFORM_IDENTITY;
                    if (Size > 1) {
                        transforms = Data[1] % 16;
                    }
                    png_write_png(png_ptr, info_ptr, transforms, NULL);
                }
                break;
                
            case 4:
                // Test sequence: info before PLTE, then info, then end
                png_write_info_before_PLTE(png_ptr, info_ptr);
                png_write_info(png_ptr, info_ptr);
                png_write_end(png_ptr, info_ptr);
                break;
        }
    }
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}