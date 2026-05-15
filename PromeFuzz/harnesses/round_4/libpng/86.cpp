// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_quantize at pngrtran.c:472:1 in png.h
// png_get_PLTE at pngget.c:1121:1 in png.h
// png_set_palette_to_rgb at pngrtran.c:945:1 in png.h
// png_set_palette_to_rgb at pngrtran.c:945:1 in png.h
// png_set_tRNS at pngset.c:1144:1 in png.h
// png_build_grayscale_palette at png.c:852:1 in png.h
// png_set_PLTE at pngset.c:739:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_init_io at png.c:705:1 in png.h
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
#include <cstring>
#include <cstdlib>

#include "png.h"

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - suppress error output during fuzzing
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing - suppress warning output during fuzzing
}

static png_structp create_png_struct(bool for_write) {
    png_structp png_ptr;
    if (for_write) {
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                          dummy_error_fn, dummy_warning_fn);
    } else {
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                         dummy_error_fn, dummy_warning_fn);
    }
    return png_ptr;
}

static png_infop create_info_struct(png_structp png_ptr) {
    return png_create_info_struct(png_ptr);
}

extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;  // Need minimum data

    // Create write structures for functions that require them
    png_structp write_png_ptr = create_png_struct(true);
    if (!write_png_ptr) return 0;
    
    png_infop write_info_ptr = create_info_struct(write_png_ptr);
    if (!write_info_ptr) {
        png_destroy_write_struct(&write_png_ptr, nullptr);
        return 0;
    }

    // Create read structures for functions that require them
    png_structp read_png_ptr = create_png_struct(false);
    if (!read_png_ptr) {
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        return 0;
    }
    
    png_infop read_info_ptr = create_info_struct(read_png_ptr);
    if (!read_info_ptr) {
        png_destroy_read_struct(&read_png_ptr, nullptr, nullptr);
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        return 0;
    }

    // Set up dummy file for reading/writing
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        png_destroy_read_struct(&read_png_ptr, &read_info_ptr, nullptr);
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        return 0;
    }
    
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    // Reopen for reading
    dummy_file = fopen("./dummy_file", "rb");
    if (!dummy_file) {
        png_destroy_read_struct(&read_png_ptr, &read_info_ptr, nullptr);
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        return 0;
    }

    // Initialize read structure with dummy file
    png_init_io(read_png_ptr, dummy_file);
    
    // Try to read PNG header (may fail, that's OK for fuzzing)
    png_read_info(read_png_ptr, read_info_ptr);

    // Initialize write structure with dummy file (we'll write to /dev/null)
    FILE* null_file = fopen("/dev/null", "wb");
    if (null_file) {
        png_init_io(write_png_ptr, null_file);
        fclose(null_file);
    }

    // Set up some basic parameters for write structure
    png_set_IHDR(write_png_ptr, write_info_ptr, 
                 64, 64, 8, PNG_COLOR_TYPE_PALETTE,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, 
                 PNG_FILTER_TYPE_BASE);

    // Allocate palette arrays
    const int max_palette_size = 256;
    png_color palette[max_palette_size];
    png_uint_16 histogram[max_palette_size];
    
    // Initialize palette and histogram with fuzzer data
    size_t data_offset = 0;
    for (int i = 0; i < max_palette_size && data_offset + 2 < Size; i++) {
        palette[i].red = Data[data_offset++ % Size];
        palette[i].green = Data[data_offset++ % Size];
        palette[i].blue = Data[data_offset++ % Size];
        histogram[i] = (data_offset + 1 < Size) ? 
                      (Data[data_offset++ % Size] << 8) | Data[data_offset++ % Size] : 0;
    }

    // Test png_set_quantize with various parameters
    int num_palette = (Size > 0) ? Data[0] % (max_palette_size + 1) : 16;
    int maximum_colors = (Size > 1) ? Data[1] % (max_palette_size + 1) : 64;
    int full_quantize = (Size > 2) ? Data[2] % 2 : 0;
    
    png_set_quantize(write_png_ptr, palette, num_palette, maximum_colors, 
                     histogram, full_quantize);

    // Test png_get_PLTE
    png_color* retrieved_palette = nullptr;
    int retrieved_num_palette = 0;
    png_get_PLTE(read_png_ptr, read_info_ptr, &retrieved_palette, &retrieved_num_palette);

    // Test png_set_palette_to_rgb
    png_set_palette_to_rgb(write_png_ptr);
    png_set_palette_to_rgb(read_png_ptr);

    // Test png_set_tRNS
    const int max_trans = 256;
    png_byte trans_alpha[max_trans];
    png_color_16 trans_color;
    
    // Initialize transparency data
    data_offset = 3;
    int num_trans = (Size > 3) ? Data[3] % (max_trans + 1) : 0;
    for (int i = 0; i < num_trans && data_offset < Size; i++) {
        trans_alpha[i] = Data[data_offset++ % Size];
    }
    
    trans_color.index = (Size > data_offset) ? Data[data_offset++ % Size] : 0;
    trans_color.red = (Size > data_offset + 1) ? 
                     (Data[data_offset++ % Size] << 8) | Data[data_offset++ % Size] : 0;
    trans_color.green = (Size > data_offset + 1) ? 
                       (Data[data_offset++ % Size] << 8) | Data[data_offset++ % Size] : 0;
    trans_color.blue = (Size > data_offset + 1) ? 
                      (Data[data_offset++ % Size] << 8) | Data[data_offset++ % Size] : 0;
    
    png_set_tRNS(write_png_ptr, write_info_ptr, trans_alpha, num_trans, &trans_color);

    // Test png_build_grayscale_palette with various bit depths
    int bit_depth = (Size > 4) ? (Data[4] % 5) + 1 : 8;  // 1-5, but valid are 1,2,4,8
    if (bit_depth == 3) bit_depth = 4;
    if (bit_depth == 5) bit_depth = 8;
    
    png_build_grayscale_palette(bit_depth, palette);

    // Test png_set_PLTE
    png_set_PLTE(write_png_ptr, write_info_ptr, palette, num_palette);

    // Clean up
    fclose(dummy_file);
    png_destroy_read_struct(&read_png_ptr, &read_info_ptr, nullptr);
    png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
    
    // Remove dummy file
    remove("./dummy_file");

    return 0;
}