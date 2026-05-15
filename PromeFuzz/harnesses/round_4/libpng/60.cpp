// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_malloc at pngmem.c:172:1 in png.h
// png_set_hIST at pngset.c:375:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_PLTE at pngset.c:739:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
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
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_palette_max at pngget.c:1349:1 in png.h
// png_get_PLTE at pngget.c:1121:1 in png.h
// png_set_palette_to_rgb at pngrtran.c:945:1 in png.h
// png_set_check_for_invalid_index at pngset.c:2024:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include "png.h"

static void user_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function
}

static void user_flush_data(png_structp png_ptr) {
    // Dummy flush function
}

static void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy read function - will be set up with fuzz data
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0; // Need minimum data

    // Create read structures
    png_structp png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr_read) return 0;
    
    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        return 0;
    }
    
    // Create write structures
    png_structp png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
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
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr_read)) || setjmp(png_jmpbuf(png_ptr_write))) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        return 0;
    }
    
    // Set up read from fuzz data
    struct read_data_state {
        const uint8_t *data;
        size_t size;
        size_t offset;
    };
    
    static read_data_state state = {Data, Size, 0};
    state.offset = 0;
    
    png_set_read_fn(png_ptr_read, &state, [](png_structp png_ptr, png_bytep out, png_size_t length) {
        read_data_state *s = static_cast<read_data_state*>(png_get_io_ptr(png_ptr));
        if (s->offset + length > s->size) {
            png_error(png_ptr, "Read beyond buffer");
        }
        memcpy(out, s->data + s->offset, length);
        s->offset += length;
    });
    
    // Set up write to dummy destination
    png_set_write_fn(png_ptr_write, nullptr, user_write_data, user_flush_data);
    
    // Try to read PNG header
    png_read_info(png_ptr_read, info_ptr_read);
    
    // Test png_get_palette_max
    int max_palette = png_get_palette_max(png_ptr_read, info_ptr_read);
    
    // Test png_get_PLTE
    png_colorp palette = nullptr;
    int num_palette = 0;
    png_uint_32 plte_info = png_get_PLTE(png_ptr_read, info_ptr_read, &palette, &num_palette);
    
    // Test png_set_palette_to_rgb
    png_set_palette_to_rgb(png_ptr_read);
    
    // Test png_set_check_for_invalid_index
    int allowed = Data[0] & 1; // Use first byte to determine if allowed
    png_set_check_for_invalid_index(png_ptr_read, allowed);
    
    // Test png_set_hIST if we have palette
    if (plte_info & PNG_INFO_PLTE && num_palette > 0) {
        // Allocate histogram array
        png_uint_16 *hist = static_cast<png_uint_16*>(png_malloc(png_ptr_read, num_palette * sizeof(png_uint_16)));
        if (hist) {
            // Fill histogram with fuzz data
            size_t hist_data_size = num_palette * sizeof(png_uint_16);
            size_t copy_size = (Size - state.offset < hist_data_size) ? Size - state.offset : hist_data_size;
            if (copy_size > 0) {
                memcpy(hist, Data + state.offset, copy_size);
                state.offset += copy_size;
            }
            // Fill remaining with zeros
            if (copy_size < hist_data_size) {
                memset(hist + copy_size/sizeof(png_uint_16), 0, hist_data_size - copy_size);
            }
            
            png_set_hIST(png_ptr_read, info_ptr_read, hist);
            png_free(png_ptr_read, hist);
        }
    }
    
    // Test png_set_PLTE for writing
    if (Size - state.offset >= 10) {
        // Create a simple palette from fuzz data
        int write_num_palette = (Data[state.offset] % 256) + 1;
        if (write_num_palette > 256) write_num_palette = 256;
        
        png_color *write_palette = static_cast<png_color*>(png_malloc(png_ptr_write, write_num_palette * sizeof(png_color)));
        if (write_palette) {
            // Fill palette with fuzz data
            size_t palette_data_needed = write_num_palette * 3; // RGB bytes
            size_t palette_copy_size = (Size - state.offset - 1 < palette_data_needed) ? 
                                       Size - state.offset - 1 : palette_data_needed;
            
            if (palette_copy_size > 0) {
                for (size_t i = 0; i < palette_copy_size/3 && i < static_cast<size_t>(write_num_palette); i++) {
                    write_palette[i].red = Data[state.offset + 1 + i*3];
                    write_palette[i].green = Data[state.offset + 1 + i*3 + 1];
                    write_palette[i].blue = Data[state.offset + 1 + i*3 + 2];
                }
                state.offset += 1 + palette_copy_size;
            }
            
            // Set basic image info for writing
            png_set_IHDR(png_ptr_write, info_ptr_write, 64, 64, 8, 
                        PNG_COLOR_TYPE_PALETTE, PNG_INTERLACE_NONE,
                        PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
            
            png_set_PLTE(png_ptr_write, info_ptr_write, write_palette, write_num_palette);
            png_free(png_ptr_write, write_palette);
            
            // Try to write header
            png_write_info(png_ptr_write, info_ptr_write);
        }
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    return 0;
}