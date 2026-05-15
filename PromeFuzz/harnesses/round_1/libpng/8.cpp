// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_invert_mono at pngtrans.c:249:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_set_palette_to_rgb at pngrtran.c:945:1 in png.h
// png_set_packing at pngtrans.c:49:1 in png.h
// png_set_invert_alpha at pngtrans.c:236:1 in png.h
// png_set_bgr at pngtrans.c:20:1 in png.h
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
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "code/png.h"

static void error_handler(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - suppress error output during fuzzing
    (void)png_ptr;
    (void)error_msg;
}

static void warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing - suppress warning output during fuzzing
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    // Create a dummy file with the fuzzer data
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open the dummy file for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        return 0;
    }
    
    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 nullptr, error_handler, warning_handler);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }
    
    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Initialize PNG I/O
    png_init_io(png_ptr, fp);
    
    // Read PNG header
    png_read_info(png_ptr, info_ptr);
    
    // Get image information
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type, compression_type, filter_type;
    
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, &compression_type, &filter_type);
    
    // Use fuzzer data to determine which transformations to apply
    // We'll use the first 6 bits of the first byte to decide which functions to call
    if (Size > 0) {
        uint8_t selector = Data[0];
        
        if (selector & 0x01) {
            png_set_palette_to_rgb(png_ptr);
        }
        if (selector & 0x02) {
            png_set_packing(png_ptr);
        }
        if (selector & 0x04) {
            png_set_invert_alpha(png_ptr);
        }
        if (selector & 0x08) {
            png_set_bgr(png_ptr);
        }
        if (selector & 0x10) {
            png_set_swap_alpha(png_ptr);
        }
        if (selector & 0x20) {
            png_set_invert_mono(png_ptr);
        }
    }
    
    // Update the info structure after transformations
    png_read_update_info(png_ptr, info_ptr);
    
    // Try to read some image data to exercise the transformations
    // Allocate row pointers
    png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    png_bytep row = (png_bytep)png_malloc(png_ptr, rowbytes);
    
    if (row) {
        // Read a few rows if possible
        for (png_uint_32 y = 0; y < height && y < 10; y++) {
            png_read_row(png_ptr, row, nullptr);
        }
        png_free(png_ptr, row);
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);
    
    // Remove the dummy file
    remove("./dummy_file");
    
    return 0;
}