// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_read_rows at pngread.c:641:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_progressive_read_fn at pngpread.c:1109:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_process_data at pngpread.c:50:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_progressive_ptr at pngpread.c:1136:1 in png.h
// png_get_current_pass_number at pngtrans.c:859:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_progressive_combine_row at pngpread.c:1093:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
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
#include "png.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>

static void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp a = png_get_io_ptr(png_ptr);
    // Simple implementation that reads from fixed buffer
    static size_t offset = 0;
    if (offset + length <= 1024) {
        memcpy(data, (uint8_t*)a + offset, length);
        offset += length;
    } else {
        png_error(png_ptr, "Read error");
    }
}

static void info_callback(png_structp png_ptr, png_infop info_ptr) {
    // Empty info callback for progressive reading
}

static void row_callback(png_structp png_ptr, png_bytep new_row, png_uint_32 row_num, int pass) {
    // Test png_get_progressive_ptr and png_get_current_pass_number
    void* prog_ptr = png_get_progressive_ptr(png_ptr);
    png_byte pass_num = png_get_current_pass_number(png_ptr);
    
    // Test png_progressive_combine_row when new_row is not NULL
    if (new_row) {
        // Get rowbytes from png_struct using png_get_rowbytes
        png_infop info_ptr = (png_infop)prog_ptr;
        if (info_ptr) {
            size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
            // Allocate old_row buffer
            png_bytep old_row = (png_bytep)malloc(row_bytes);
            if (old_row) {
                png_progressive_combine_row(png_ptr, old_row, new_row);
                free(old_row);
            }
        }
    }
}

static void end_callback(png_structp png_ptr, png_infop info_ptr) {
    // Empty end callback for progressive reading
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Write data to file for file-based reading
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Test 1: Non-progressive reading with png_read_rows
    {
        fp = fopen("./dummy_file", "rb");
        if (!fp) goto cleanup1;
        
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) {
            fclose(fp);
            goto cleanup1;
        }
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, NULL, NULL);
            fclose(fp);
            goto cleanup1;
        }
        
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            goto cleanup1;
        }
        
        png_init_io(png_ptr, fp);
        png_read_info(png_ptr, info_ptr);
        
        png_uint_32 width, height;
        int bit_depth, color_type;
        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);
        
        // Allocate row pointers
        png_bytep row_pointers[height];
        png_uint_32 row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_bytep)malloc(row_bytes);
        }
        
        // Test png_read_rows
        png_read_rows(png_ptr, row_pointers, NULL, height);
        
        // Cleanup
        for (png_uint_32 y = 0; y < height; y++) {
            free(row_pointers[y]);
        }
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
    }
    
cleanup1:
    
    // Test 2: Non-progressive reading with png_read_row
    {
        fp = fopen("./dummy_file", "rb");
        if (!fp) goto cleanup2;
        
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) {
            fclose(fp);
            goto cleanup2;
        }
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, NULL, NULL);
            fclose(fp);
            goto cleanup2;
        }
        
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            goto cleanup2;
        }
        
        png_init_io(png_ptr, fp);
        png_read_info(png_ptr, info_ptr);
        
        png_uint_32 width, height;
        int bit_depth, color_type;
        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);
        
        // Allocate row buffer
        png_uint_32 row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        png_bytep row = (png_bytep)malloc(row_bytes);
        
        // Test png_read_row in a loop
        for (png_uint_32 y = 0; y < height; y++) {
            png_read_row(png_ptr, row, NULL);
        }
        
        free(row);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
    }
    
cleanup2:
    
    // Test 3: Non-progressive reading with png_read_image
    {
        fp = fopen("./dummy_file", "rb");
        if (!fp) goto cleanup3;
        
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) {
            fclose(fp);
            goto cleanup3;
        }
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, NULL, NULL);
            fclose(fp);
            goto cleanup3;
        }
        
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            goto cleanup3;
        }
        
        png_init_io(png_ptr, fp);
        png_read_info(png_ptr, info_ptr);
        
        png_uint_32 width, height;
        int bit_depth, color_type;
        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);
        
        // Allocate image buffer
        png_bytep image[height];
        png_uint_32 row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        for (png_uint_32 y = 0; y < height; y++) {
            image[y] = (png_bytep)malloc(row_bytes);
        }
        
        // Test png_read_image
        png_read_image(png_ptr, image);
        
        // Cleanup
        for (png_uint_32 y = 0; y < height; y++) {
            free(image[y]);
        }
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
    }
    
cleanup3:
    
    // Test 4: Progressive reading
    {
        // Create buffer for memory reading
        uint8_t buffer[1024];
        size_t copy_size = Size < 1024 ? Size : 1024;
        memcpy(buffer, Data, copy_size);
        
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) goto cleanup4;
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, NULL, NULL);
            goto cleanup4;
        }
        
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            goto cleanup4;
        }
        
        // Set up progressive reading - pass info_ptr as user pointer
        png_set_progressive_read_fn(png_ptr, info_ptr, info_callback, row_callback, end_callback);
        png_set_read_fn(png_ptr, buffer, user_read_data);
        
        // Process data in chunks to simulate progressive reading
        size_t chunk_size = 64;
        for (size_t i = 0; i < copy_size; i += chunk_size) {
            size_t this_chunk = (i + chunk_size <= copy_size) ? chunk_size : copy_size - i;
            png_process_data(png_ptr, info_ptr, (png_bytep)(buffer + i), this_chunk);
        }
        
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    }
    
cleanup4:
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}