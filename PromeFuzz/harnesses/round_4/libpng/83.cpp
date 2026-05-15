// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_strip_16 at pngrtran.c:185:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
// png_set_scale_16 at pngrtran.c:171:1 in png.h
// png_set_expand_16 at pngrtran.c:985:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_start_read_image at pngread.c:291:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
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

#include "png.h"

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Simple dummy read function that returns zeros
    memset(data, 0, length);
}

extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    static const char* dummy_filename = "./dummy_file";
    
    // Write data to dummy file
    FILE* fp = fopen(dummy_filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open dummy file for reading
    fp = fopen(dummy_filename, "rb");
    if (!fp) {
        return 0;
    }
    
    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
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
    
    // Use first byte to determine which functions to call
    uint8_t selector = 0;
    if (Size > 0) {
        selector = Data[0];
    }
    
    // Call target functions based on selector
    if (selector & 0x01) {
        png_set_strip_16(png_ptr);
    }
    if (selector & 0x02) {
        png_set_swap_alpha(png_ptr);
    }
    if (selector & 0x04) {
        png_set_swap(png_ptr);
    }
    if (selector & 0x08) {
        png_set_scale_16(png_ptr);
    }
    if (selector & 0x10) {
        png_set_expand_16(png_ptr);
    }
    
    // Update info after transformations
    png_read_update_info(png_ptr, info_ptr);
    
    // Call png_start_read_image
    png_start_read_image(png_ptr);
    
    // Try to read some rows to exercise the transformations
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    if (height > 0 && height < 10000 && rowbytes > 0 && rowbytes < 100000) {
        png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_bytep)png_malloc(png_ptr, rowbytes);
        }
        
        // Read image data using dummy read function if needed
        png_set_read_fn(png_ptr, nullptr, dummy_read_fn);
        png_read_image(png_ptr, row_pointers);
        
        // Clean up row pointers
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
        png_free(png_ptr, row_pointers);
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);
    
    return 0;
}