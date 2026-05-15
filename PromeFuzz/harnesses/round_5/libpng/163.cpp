// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_bgr at pngtrans.c:20:1 in png.h
// png_set_interlace_handling at pngtrans.c:98:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_image at pngwrite.c:659:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_interlace_type at pngget.c:107:1 in png.h
// png_set_interlace_handling at pngtrans.c:98:1 in png.h
// png_set_bgr at pngtrans.c:20:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_get_current_pass_number at pngtrans.c:859:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
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

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {}
static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {}

extern "C" int LLVMFuzzerTestOneInput_163(const uint8_t *Data, size_t Size) {
    // Write input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize reading structures
    png_structp png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                      nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr_read) return 0;
    
    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        return 0;
    }

    // Initialize writing structures
    png_structp png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                       nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, nullptr);
        return 0;
    }

    // Try to read the PNG file
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        png_init_io(png_ptr_read, fp);
        
        // Read PNG header
        png_read_info(png_ptr_read, info_ptr_read);
        
        // Test png_get_interlace_type
        png_byte interlace_type = png_get_interlace_type(png_ptr_read, info_ptr_read);
        
        // Test png_set_interlace_handling
        int passes = png_set_interlace_handling(png_ptr_read);
        
        // Test png_set_bgr
        png_set_bgr(png_ptr_read);
        
        // Allocate image buffer for reading
        png_uint_32 width = png_get_image_width(png_ptr_read, info_ptr_read);
        png_uint_32 height = png_get_image_height(png_ptr_read, info_ptr_read);
        png_byte bit_depth = png_get_bit_depth(png_ptr_read, info_ptr_read);
        png_byte color_type = png_get_color_type(png_ptr_read, info_ptr_read);
        
        // Calculate row bytes
        png_size_t row_bytes = png_get_rowbytes(png_ptr_read, info_ptr_read);
        
        // Allocate row pointers
        png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr_read, height * sizeof(png_bytep));
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_bytep)png_malloc(png_ptr_read, row_bytes);
        }
        
        // Test png_read_image
        png_read_image(png_ptr_read, row_pointers);
        
        // Test png_get_current_pass_number
        png_byte current_pass = png_get_current_pass_number(png_ptr_read);
        
        // Clean up reading structures
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr_read, row_pointers[y]);
        }
        png_free(png_ptr_read, row_pointers);
        
        png_read_end(png_ptr_read, info_ptr_read);
        fclose(fp);
    }
    
    // Test writing functions
    // Create a simple 1x1 image for writing
    const png_uint_32 write_width = 1;
    const png_uint_32 write_height = 1;
    png_bytepp write_rows = (png_bytepp)png_malloc(png_ptr_write, write_height * sizeof(png_bytep));
    write_rows[0] = (png_bytep)png_malloc(png_ptr_write, 3); // RGB
    
    // Initialize pixel data
    write_rows[0][0] = 255; // R
    write_rows[0][1] = 128; // G
    write_rows[0][2] = 64;  // B
    
    // Set up writing info
    png_set_IHDR(png_ptr_write, info_ptr_write, write_width, write_height,
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    
    // Test png_set_bgr for writing
    png_set_bgr(png_ptr_write);
    
    // Test png_set_interlace_handling for writing
    int write_passes = png_set_interlace_handling(png_ptr_write);
    
    // Open file for writing
    fp = fopen("./dummy_file", "wb");
    if (fp) {
        png_init_io(png_ptr_write, fp);
        png_write_info(png_ptr_write, info_ptr_write);
        
        // Test png_write_image
        png_write_image(png_ptr_write, write_rows);
        
        png_write_end(png_ptr_write, nullptr);
        fclose(fp);
    }
    
    // Clean up writing allocations
    png_free(png_ptr_write, write_rows[0]);
    png_free(png_ptr_write, write_rows);
    
    // Destroy all structures
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    return 0;
}