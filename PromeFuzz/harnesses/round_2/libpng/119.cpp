// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_read_png at pngread.c:957:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_start_read_image at pngread.c:291:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_start_read_image at pngread.c:291:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_get_compression_buffer_size at pngget.c:1279:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void cleanup_file() {
    remove("./dummy_file");
}

extern "C" int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    // Write input to file
    write_dummy_file(Data, Size);
    
    // Open the file for reading
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        cleanup_file();
        return 0;
    }
    
    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        cleanup_file();
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        cleanup_file();
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
        cleanup_file();
        return 0;
    }
    
    // Initialize I/O
    png_init_io(png_ptr, fp);
    
    // Read PNG header
    png_read_info(png_ptr, info_ptr);
    
    // Call target functions in various sequences
    // Sequence 1: Normal flow
    png_start_read_image(png_ptr);
    png_read_update_info(png_ptr, info_ptr);
    
    // Get rowbytes and compression buffer size
    size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    size_t comp_buffer_size = png_get_compression_buffer_size(png_ptr);
    
    // Try to read image if we have valid dimensions
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, 
                 &color_type, &interlace_type, nullptr, nullptr);
    
    if (height > 0 && rowbytes > 0) {
        // Allocate row pointers for png_read_image
        png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr, 
                                                         height * sizeof(png_bytep));
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_bytep)png_malloc(png_ptr, rowbytes);
        }
        
        png_read_image(png_ptr, row_pointers);
        
        // Clean up row pointers
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
        png_free(png_ptr, row_pointers);
    }
    
    // Sequence 2: Try png_read_png with different transforms
    int transforms = PNG_TRANSFORM_IDENTITY;
    png_read_png(png_ptr, info_ptr, transforms, nullptr);
    
    // Sequence 3: Reset and try different order
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);
    
    // Re-open file and structures
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                         nullptr, nullptr, nullptr);
        if (png_ptr) {
            info_ptr = png_create_info_struct(png_ptr);
            if (info_ptr) {
                if (setjmp(png_jmpbuf(png_ptr))) {
                    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
                    fclose(fp);
                    cleanup_file();
                    return 0;
                }
                
                png_init_io(png_ptr, fp);
                png_read_info(png_ptr, info_ptr);
                
                // Call functions in different order
                png_read_update_info(png_ptr, info_ptr);
                png_start_read_image(png_ptr);
                
                // Try with different transform flags
                transforms = PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING;
                png_read_png(png_ptr, info_ptr, transforms, nullptr);
                
                png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            } else {
                png_destroy_read_struct(&png_ptr, nullptr, nullptr);
            }
        }
        fclose(fp);
    }
    
    cleanup_file();
    return 0;
}