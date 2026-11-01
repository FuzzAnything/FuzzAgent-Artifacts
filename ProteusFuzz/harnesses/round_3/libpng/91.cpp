// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_current_row_number at pngtrans.c:847:1 in png.h
// png_get_current_pass_number at pngtrans.c:859:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_read_rows at pngread.c:641:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_get_current_row_number at pngtrans.c:847:1 in png.h
// png_get_current_pass_number at pngtrans.c:859:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_get_current_row_number at pngtrans.c:847:1 in png.h
// png_get_current_pass_number at pngtrans.c:859:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <png.h>
#include <cstdint>
#include <cstring>
#include <vector>
#include <cstdlib>

static void write_dummy_png(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    
    // Write minimal PNG signature
    const unsigned char png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(png_sig, 1, 8, f);
    
    // Write IHDR chunk (minimal valid header)
    unsigned char ihdr[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint32_t width = 0, height = 0;
    
    if (Size >= 8) {
        width = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
        height = (Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7];
    }
    
    // Ensure non-zero dimensions for valid PNG
    if (width == 0) width = 1;
    if (height == 0) height = 1;
    
    ihdr[0] = (width >> 24) & 0xFF;
    ihdr[1] = (width >> 16) & 0xFF;
    ihdr[2] = (width >> 8) & 0xFF;
    ihdr[3] = width & 0xFF;
    ihdr[4] = (height >> 24) & 0xFF;
    ihdr[5] = (height >> 16) & 0xFF;
    ihdr[6] = (height >> 8) & 0xFF;
    ihdr[7] = height & 0xFF;
    ihdr[8] = 8;  // bit depth
    ihdr[9] = 2;  // color type (grayscale)
    ihdr[10] = 0; // compression
    ihdr[11] = 0; // filter
    ihdr[12] = 0; // interlace
    
    // Write chunk length
    uint32_t len = 13;
    fwrite(&len, 4, 1, f);
    
    // Write chunk type
    const char *type = "IHDR";
    fwrite(type, 1, 4, f);
    
    // Write chunk data
    fwrite(ihdr, 1, 13, f);
    
    // Write CRC (dummy)
    uint32_t crc = 0;
    fwrite(&crc, 4, 1, f);
    
    // Write IDAT chunk (minimal)
    len = 0;
    fwrite(&len, 4, 1, f);
    const char *idat = "IDAT";
    fwrite(idat, 1, 4, f);
    fwrite(&crc, 4, 1, f);
    
    // Write IEND chunk
    len = 0;
    fwrite(&len, 4, 1, f);
    const char *iend = "IEND";
    fwrite(iend, 1, 4, f);
    fwrite(&crc, 4, 1, f);
    
    fclose(f);
}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return NULL;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return NULL;
    }
    
    return png_ptr;
}

static void destroy_png_struct(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    } else if (info_ptr) {
        png_destroy_read_struct(NULL, &info_ptr, NULL);
    }
}

extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Test with NULL pointer (should handle gracefully)
    png_uint_32 row_num = png_get_current_row_number(NULL);
    png_byte pass_num = png_get_current_pass_number(NULL);
    
    // Write dummy PNG file for read operations
    write_dummy_png(Data, Size);
    
    // Test read operations
    png_structp png_ptr = create_png_struct();
    png_infop info_ptr = NULL;
    
    if (png_ptr) {
        FILE *fp = fopen("./dummy_file", "rb");
        if (fp) {
            png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            if (!png_ptr) {
                fclose(fp);
                return 0;
            }
            
            info_ptr = png_create_info_struct(png_ptr);
            if (!info_ptr) {
                png_destroy_read_struct(&png_ptr, NULL, NULL);
                fclose(fp);
                return 0;
            }
            
            if (setjmp(png_jmpbuf(png_ptr))) {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                fclose(fp);
                return 0;
            }
            
            png_init_io(png_ptr, fp);
            
            // Try to read image info
            png_read_info(png_ptr, info_ptr);
            
            png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
            png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
            
            // Allocate row pointers
            std::vector<png_byte*> row_pointers(height);
            std::vector<png_byte> row_data(width * 2); // 2 bytes per pixel for grayscale with alpha
            
            for (png_uint_32 y = 0; y < height; y++) {
                row_pointers[y] = row_data.data() + y * width * 2;
            }
            
            // Test png_read_rows
            png_read_rows(png_ptr, row_pointers.data(), NULL, height);
            
            // Test png_read_image
            png_read_image(png_ptr, row_pointers.data());
            
            // Test png_read_row (single row)
            if (height > 0) {
                png_read_row(png_ptr, row_pointers[0], NULL);
            }
            
            // Test current row/pass number (though typically used in callbacks)
            row_num = png_get_current_row_number(png_ptr);
            pass_num = png_get_current_pass_number(png_ptr);
            
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
        } else {
            destroy_png_struct(png_ptr, info_ptr);
        }
    }
    
    // Test write operations
    png_structp write_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (write_png_ptr) {
        png_infop write_info_ptr = png_create_info_struct(write_png_ptr);
        if (write_info_ptr) {
            if (setjmp(png_jmpbuf(write_png_ptr))) {
                png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
                return 0;
            }
            
            FILE *out_fp = fopen("./dummy_file", "wb");
            if (out_fp) {
                png_init_io(write_png_ptr, out_fp);
                
                png_set_IHDR(write_png_ptr, write_info_ptr, 1, 1, 8, PNG_COLOR_TYPE_GRAY, 
                            PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
                
                png_write_info(write_png_ptr, write_info_ptr);
                
                // Create test row data
                std::vector<png_byte> test_row(1);
                test_row[0] = Data[0];
                
                std::vector<png_byte*> write_rows(1);
                write_rows[0] = test_row.data();
                
                // Test png_write_rows
                png_write_rows(write_png_ptr, write_rows.data(), 1);
                
                // Test png_read_row with write struct (should handle gracefully)
                png_read_row(write_png_ptr, test_row.data(), NULL);
                
                // Test current row/pass number with write struct
                row_num = png_get_current_row_number(write_png_ptr);
                pass_num = png_get_current_pass_number(write_png_ptr);
                
                png_write_end(write_png_ptr, NULL);
                fclose(out_fp);
            }
            
            png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        } else {
            png_destroy_write_struct(&write_png_ptr, NULL);
        }
    }
    
    return 0;
}