// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_pHYs at pngset.c:723:1 in png.h
// png_set_sBIT at pngset.c:804:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_set_rows at pngset.c:1845:1 in png.h
// png_write_png at pngwrite.c:1376:1 in png.h
// png_process_data_skip at pngpread.c:92:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
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

extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    // Create dummy file for writing
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    
    // Write fuzzer data to dummy file
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);
    
    // Reopen for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;
    
    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
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
    
    // Initialize I/O
    png_init_io(png_ptr, fp);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Create write structures
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!write_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Set up error handling for write struct
    if (setjmp(png_jmpbuf(write_ptr))) {
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Prepare output file for writing
    FILE *out_fp = fopen("./dummy_out.png", "wb");
    if (!out_fp) {
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
        return 0;
    }
    png_init_io(write_ptr, out_fp);
    
    // Get image dimensions from read info
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    
    // Set IHDR parameters using fuzzer data for some parameters
    png_set_IHDR(write_ptr, write_info, 
                 width > 0 ? width : 100, 
                 height > 0 ? height : 100,
                 bit_depth > 0 ? bit_depth : 8,
                 color_type,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    
    // Set pHYs using fuzzer data if available
    png_uint_32 res_x = 72;
    png_uint_32 res_y = 72;
    int unit_type = PNG_RESOLUTION_METER;
    
    if (Size >= 8) {
        // Use first 8 bytes for resolution values
        memcpy(&res_x, Data, 4);
        memcpy(&res_y, Data + 4, 4);
        unit_type = (Data[0] % 2) ? PNG_RESOLUTION_METER : PNG_RESOLUTION_UNKNOWN;
    }
    
    png_set_pHYs(write_ptr, write_info, res_x, res_y, unit_type);
    
    // Set sBIT if we have enough data
    png_color_8 sig_bit;
    if (Size >= 5) {
        sig_bit.red = Data[0];
        sig_bit.green = Data[1];
        sig_bit.blue = Data[2];
        sig_bit.gray = Data[3];
        sig_bit.alpha = Data[4];
        png_set_sBIT(write_ptr, write_info, &sig_bit);
    }
    
    // Write info
    png_write_info(write_ptr, write_info);
    
    // Try to write PNG with different transforms based on fuzzer input
    int transforms = 0;
    if (Size > 0) {
        transforms = Data[0] % 16;  // Use first byte for transform selection
    }
    
    // Allocate row pointers for image data
    png_bytepp row_pointers = nullptr;
    int rowbytes = png_get_rowbytes(write_ptr, write_info);
    if (rowbytes > 0 && height > 0) {
        row_pointers = (png_bytepp)png_malloc(write_ptr, height * sizeof(png_bytep));
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_bytep)png_malloc(write_ptr, rowbytes);
            memset(row_pointers[y], y % 256, rowbytes);  // Simple pattern
        }
        png_set_rows(write_ptr, write_info, row_pointers);
    }
    
    // Write PNG
    png_write_png(write_ptr, write_info, transforms, nullptr);
    
    // Call deprecated function (should do nothing but might trigger warnings)
    png_uint_32 skip_result = png_process_data_skip(write_ptr);
    (void)skip_result;  // Suppress unused variable warning
    
    // Clean up row pointers
    if (row_pointers) {
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(write_ptr, row_pointers[y]);
        }
        png_free(write_ptr, row_pointers);
    }
    
    // Clean up
    png_destroy_write_struct(&write_ptr, &write_info);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);
    fclose(out_fp);
    
    // Remove temporary files
    remove("./dummy_file");
    remove("./dummy_out.png");
    
    return 0;
}