// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_PLTE at pngget.c:1121:1 in png.h
// png_set_hIST at pngset.c:375:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "code/png.h"

static void user_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Error handler to prevent crashes on invalid data
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Warning handler - do nothing
}

extern "C" int LLVMFuzzerTestOneInput_195(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open dummy file for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        return 0;
    }
    
    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 nullptr, 
                                                 user_error_fn, 
                                                 user_warning_fn);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }
    
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
    
    // Initialize file reading
    png_init_io(png_ptr, fp);
    
    // Test png_read_info
    png_read_info(png_ptr, info_ptr);
    
    // Test png_get_valid with various flags
    png_uint_32 valid_flags = png_get_valid(png_ptr, info_ptr, PNG_INFO_PLTE);
    valid_flags = png_get_valid(png_ptr, info_ptr, PNG_INFO_hIST);
    valid_flags = png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);
    valid_flags = png_get_valid(png_ptr, info_ptr, PNG_INFO_gAMA);
    
    // Test png_set_hIST if palette exists
    // Use png_get_PLTE to get palette information safely
    png_colorp palette;
    int num_palette;
    if (png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette)) {
        if (num_palette > 0 && num_palette <= 256) {
            png_uint_16 hist[256];
            for (int i = 0; i < num_palette; i++) {
                hist[i] = (i * 65535) / (num_palette - 1);
            }
            png_set_hIST(png_ptr, info_ptr, hist);
        }
    }
    
    // Test png_read_png with no transforms
    png_read_png(png_ptr, info_ptr, 0, nullptr);
    
    // Test png_read_end
    png_read_end(png_ptr, info_ptr);
    
    // Test png_destroy_info_struct
    png_infopp info_ptr_ptr = &info_ptr;
    png_destroy_info_struct(png_ptr, info_ptr_ptr);
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    fclose(fp);
    
    return 0;
}