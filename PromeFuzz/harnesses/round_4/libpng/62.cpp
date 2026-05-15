// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_set_sBIT at pngset.c:804:1 in png.h
// png_set_iCCP at pngset.c:859:1 in png.h
// png_write_png at pngwrite.c:1376:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_sRGB at pngset.c:819:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "png.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void dummy_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function that does nothing
}

static void dummy_flush_data(png_structp png_ptr) {
    // Dummy flush function that does nothing
}

static void error_handler(png_structp png_ptr, png_const_charp error_msg) {
    // Error handler to prevent crashes on invalid input
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Warning handler - do nothing
}

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0; // Need minimum data for testing
    
    // Create dummy file for writing
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    
    // Initialize write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, error_handler, warning_handler);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }
    
    // Initialize info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
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
    
    // Use fuzzer data to determine parameters
    png_uint_32 width = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
    png_uint_32 height = (Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7];
    
    // Ensure reasonable dimensions
    if (width == 0) width = 1;
    if (height == 0) height = 1;
    if (width > 1000) width = 1000;
    if (height > 1000) height = 1000;
    
    // Extract other parameters from fuzzer data
    int bit_depth = Data[8] % 5; // 0-4, will be mapped to valid values
    if (bit_depth == 0) bit_depth = 1;
    else if (bit_depth == 1) bit_depth = 2;
    else if (bit_depth == 2) bit_depth = 4;
    else if (bit_depth == 3) bit_depth = 8;
    else bit_depth = 16;
    
    int color_type = Data[9] % 6; // Valid values: 0,2,3,4,6
    if (color_type == 1) color_type = 0;
    if (color_type == 5) color_type = 6;
    
    int interlace_method = Data[10] % 2; // 0 or 1
    int compression_method = 0; // Only 0 is valid
    int filter_method = 0; // Only 0 is valid
    
    int srgb_intent = Data[11] % 4; // 0-3
    
    // Test png_set_IHDR
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_method, compression_method, filter_method);
    
    // Test png_set_sRGB with different intents
    png_set_sRGB(png_ptr, info_ptr, srgb_intent);
    
    // Test png_set_sRGB_gAMA_and_cHRM
    png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, (srgb_intent + 1) % 4);
    
    // Test png_set_sBIT
    png_color_8 sig_bit;
    sig_bit.red = Data[12] % (bit_depth + 1);
    sig_bit.green = Data[13] % (bit_depth + 1);
    sig_bit.blue = Data[14] % (bit_depth + 1);
    sig_bit.alpha = Data[15] % (bit_depth + 1);
    sig_bit.gray = Data[16] % (bit_depth + 1);
    png_set_sBIT(png_ptr, info_ptr, &sig_bit);
    
    // Test png_set_iCCP if we have enough data
    if (Size > 50) {
        const char *name = "dummy";
        int compression_type = Data[17] % 2; // 0 or 1
        png_uint_32 proflen = Size - 20;
        if (proflen > 10000) proflen = 10000; // Limit profile size
        
        const png_byte *profile = Data + 20;
        png_set_iCCP(png_ptr, info_ptr, name, compression_type, profile, proflen);
    }
    
    // Test png_write_png with different transforms
    int transforms = Data[18] % 16; // Various transform combinations
    png_write_png(png_ptr, info_ptr, transforms, NULL);
    
    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    
    return 0;
}