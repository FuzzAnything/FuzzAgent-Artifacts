// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_tRNS at pngset.c:1144:1 in png.h
// png_set_tRNS at pngset.c:1144:1 in png.h
// png_set_tRNS at pngset.c:1144:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_PLTE at pngget.c:1121:1 in png.h
// png_get_tRNS at pngget.c:1202:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "code/png.h"

static void dummy_png_error(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - prevent libpng from exiting
}

static void dummy_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing
}

static void read_from_memory(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) return;
    
    uint8_t** input_ptr = (uint8_t**)io_ptr;
    if (*input_ptr) {
        memcpy(data, *input_ptr, length);
        *input_ptr += length;
    }
}

extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0; // Need at least PNG signature
    
    // Write data to file as backup method
    FILE* fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    // Create read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                 dummy_png_error, dummy_png_warning);
    if (!png_ptr) return 0;
    
    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Set up custom read function
    const uint8_t* data_ptr = Data;
    png_set_read_fn(png_ptr, (png_voidp)&data_ptr, read_from_memory);
    
    // Read PNG signature
    png_set_sig_bytes(png_ptr, 0);
    
    // Target function 2: png_read_info
    png_read_info(png_ptr, info_ptr);
    
    // Target function 6: png_get_color_type
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    
    // Target function 3: png_get_bit_depth
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Target function 1: png_get_PLTE
    png_colorp palette = nullptr;
    int num_palette = 0;
    png_uint_32 plte_result = png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);
    
    // Target function 4: png_get_tRNS
    png_bytep trans_alpha = nullptr;
    int num_trans = 0;
    png_color_16p trans_color = nullptr;
    png_uint_32 trns_result = png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, &trans_color);
    
    // Target function 5: png_set_tRNS
    // Prepare dummy transparency data
    png_byte dummy_trans_alpha[256];
    png_color_16 dummy_trans_color;
    
    // Initialize with some values
    for (int i = 0; i < 256; ++i) {
        dummy_trans_alpha[i] = (png_byte)(i % 256);
    }
    
    dummy_trans_color.index = 0;
    dummy_trans_color.red = 100;
    dummy_trans_color.green = 150;
    dummy_trans_color.blue = 200;
    dummy_trans_color.gray = 128;
    
    // Call with different parameter combinations
    png_set_tRNS(png_ptr, info_ptr, dummy_trans_alpha, 
                 (Size % 256), &dummy_trans_color);
    
    // Try with null parameters
    png_set_tRNS(png_ptr, info_ptr, nullptr, 0, nullptr);
    
    // Try with partial data
    if (Size > 10) {
        png_set_tRNS(png_ptr, info_ptr, dummy_trans_alpha, 
                     (int)(Size % 100), &dummy_trans_color);
    }
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}