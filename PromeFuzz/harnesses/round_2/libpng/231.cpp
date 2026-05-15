// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_palette_to_rgb at pngrtran.c:945:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_PLTE at pngset.c:739:1 in png.h
// png_set_hIST at pngset.c:375:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_PLTE at pngget.c:1121:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_get_palette_max at pngget.c:1349:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include <csetjmp>

struct ReadData {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

static void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    ReadData* read_data = (ReadData*)png_get_io_ptr(png_ptr);
    if (!read_data || !read_data->data) return;
    
    if (read_data->pos + length > read_data->size) {
        length = read_data->size - read_data->pos;
    }
    
    if (length > 0) {
        memcpy(data, read_data->data + read_data->pos, length);
        read_data->pos += length;
    }
}

static void user_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void user_flush_data(png_structp png_ptr) {
    (void)png_ptr;
}

static void error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)error_msg;
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_231(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    // Create read structure
    png_structp png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                      nullptr, error_handler, warning_handler);
    if (!png_ptr_read) return 0;
    
    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr_read))) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    // Set up custom read function with proper bounds checking
    ReadData read_data = {Data, Size, 0};
    png_set_read_fn(png_ptr_read, &read_data, user_read_data);
    
    // Read PNG header
    png_read_info(png_ptr_read, info_ptr_read);
    
    // Test png_get_PLTE
    png_colorp palette = nullptr;
    int num_palette = 0;
    png_uint_32 plte_result = png_get_PLTE(png_ptr_read, info_ptr_read, &palette, &num_palette);
    (void)plte_result;
    
    // Test png_get_hIST
    png_uint_16* hist = nullptr;
    png_uint_32 hist_result = png_get_hIST(png_ptr_read, info_ptr_read, &hist);
    (void)hist_result;
    
    // Test png_get_palette_max
    int max_palette = png_get_palette_max(png_ptr_read, info_ptr_read);
    (void)max_palette;
    
    // Test png_set_palette_to_rgb
    png_set_palette_to_rgb(png_ptr_read);
    
    // Clean up read structures
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    
    // Now test write functions
    png_structp png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                       nullptr, error_handler, warning_handler);
    if (!png_ptr_write) return 0;
    
    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_write_struct(&png_ptr_write, nullptr);
        return 0;
    }
    
    // Set up error handling for write
    if (setjmp(png_jmpbuf(png_ptr_write))) {
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        return 0;
    }
    
    // Set up write function
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        return 0;
    }
    png_set_write_fn(png_ptr_write, dummy_file, user_write_data, user_flush_data);
    
    // Create dummy palette data from fuzzer input with bounds checking
    const int max_palette_size = 256;
    int palette_size = Data[0] % max_palette_size;
    if (palette_size == 0) palette_size = 1;
    
    png_color* write_palette = (png_color*)malloc(palette_size * sizeof(png_color));
    if (!write_palette) {
        fclose(dummy_file);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        return 0;
    }
    
    // Fill palette with data from fuzzer input
    size_t data_offset = 1;
    for (int i = 0; i < palette_size; i++) {
        if (data_offset < Size) write_palette[i].red = Data[data_offset++];
        else write_palette[i].red = 0;
        
        if (data_offset < Size) write_palette[i].green = Data[data_offset++];
        else write_palette[i].green = 0;
        
        if (data_offset < Size) write_palette[i].blue = Data[data_offset++];
        else write_palette[i].blue = 0;
    }
    
    // Test png_set_PLTE
    png_set_PLTE(png_ptr_write, info_ptr_write, write_palette, palette_size);
    
    // Create dummy histogram data with bounds checking
    png_uint_16* write_hist = (png_uint_16*)malloc(palette_size * sizeof(png_uint_16));
    if (write_hist) {
        for (int i = 0; i < palette_size; i++) {
            if (data_offset + 1 < Size) {
                write_hist[i] = (Data[data_offset] << 8) | Data[data_offset + 1];
                data_offset += 2;
            } else {
                write_hist[i] = 0;
            }
        }
        
        // Test png_set_hIST
        png_set_hIST(png_ptr_write, info_ptr_write, write_hist);
        free(write_hist);
    }
    
    // Set basic image info
    png_set_IHDR(png_ptr_write, info_ptr_write, 16, 16, 8, 
                 PNG_COLOR_TYPE_PALETTE, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    
    // Write info
    png_write_info(png_ptr_write, info_ptr_write);
    
    // Clean up
    free(write_palette);
    fclose(dummy_file);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    return 0;
}