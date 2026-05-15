// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_PLTE at pngset.c:739:1 in png.h
// png_get_tRNS at pngget.c:1202:1 in png.h
// png_set_bKGD at pngset.c:24:1 in png.h
// png_set_background_fixed at pngrtran.c:125:1 in png.h
// png_set_tRNS at pngset.c:1144:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_background at pngrtran.c:155:1 in png.h
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

static void dummy_error_fn(png_structp, png_const_charp) {}
static void dummy_warning_fn(png_structp, png_const_charp) {}

static png_structp create_png_struct(bool is_read) {
    png_structp png_ptr;
    if (is_read) {
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    } else {
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    }
    return png_ptr;
}

static void cleanup_png_struct(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        if (info_ptr) {
            png_destroy_info_struct(png_ptr, &info_ptr);
        }
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_239(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create read structure and info
    png_structp read_ptr = create_png_struct(true);
    if (!read_ptr) return 0;
    
    png_infop read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        cleanup_png_struct(read_ptr, nullptr);
        return 0;
    }

    // Create write structure and info
    png_structp write_ptr = create_png_struct(false);
    if (!write_ptr) {
        cleanup_png_struct(read_ptr, read_info);
        return 0;
    }
    
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        cleanup_png_struct(read_ptr, read_info);
        png_destroy_write_struct(&write_ptr, nullptr);
        return 0;
    }

    // Use input data to seed parameters
    const uint8_t *data_ptr = Data;
    size_t remaining = Size;

    // Extract parameters for png_set_background
    png_color_16 background_color;
    if (remaining >= sizeof(png_color_16)) {
        memcpy(&background_color, data_ptr, sizeof(png_color_16));
        data_ptr += sizeof(png_color_16);
        remaining -= sizeof(png_color_16);
    } else {
        memset(&background_color, 0, sizeof(png_color_16));
    }

    int background_gamma_code = PNG_BACKGROUND_GAMMA_UNKNOWN;
    int need_expand = 0;
    double background_gamma = 1.0;
    
    if (remaining >= 2) {
        background_gamma_code = data_ptr[0] % 4;
        need_expand = data_ptr[1] % 2;
        data_ptr += 2;
        remaining -= 2;
    }

    // Call png_set_background
    png_set_background(read_ptr, &background_color, background_gamma_code, need_expand, background_gamma);

    // Extract parameters for png_set_PLTE
    int num_palette = 0;
    png_color palette[256];
    
    if (remaining >= 1) {
        num_palette = data_ptr[0] % 256;
        data_ptr++;
        remaining--;
        
        size_t palette_bytes = num_palette * sizeof(png_color);
        if (remaining >= palette_bytes && num_palette > 0) {
            memcpy(palette, data_ptr, palette_bytes);
            data_ptr += palette_bytes;
            remaining -= palette_bytes;
            
            // Call png_set_PLTE
            png_set_PLTE(write_ptr, write_info, palette, num_palette);
        }
    }

    // Call png_get_tRNS
    png_byte *trans_alpha = nullptr;
    int num_trans = 0;
    png_color_16 *trans_color = nullptr;
    png_uint_32 tRNS_result = png_get_tRNS(read_ptr, read_info, &trans_alpha, &num_trans, &trans_color);

    // Extract parameters for png_set_bKGD
    png_color_16 bkgd_color;
    if (remaining >= sizeof(png_color_16)) {
        memcpy(&bkgd_color, data_ptr, sizeof(png_color_16));
        data_ptr += sizeof(png_color_16);
        remaining -= sizeof(png_color_16);
        
        // Call png_set_bKGD
        png_set_bKGD(write_ptr, write_info, &bkgd_color);
    }

    // Call png_set_background_fixed
    png_fixed_point fixed_gamma = 0;
    if (remaining >= sizeof(png_fixed_point)) {
        memcpy(&fixed_gamma, data_ptr, sizeof(png_fixed_point));
        data_ptr += sizeof(png_fixed_point);
        remaining -= sizeof(png_fixed_point);
    }
    png_set_background_fixed(read_ptr, &background_color, background_gamma_code, need_expand, fixed_gamma);

    // Extract parameters for png_set_tRNS
    if (remaining >= 1) {
        int trans_num = data_ptr[0] % 256;
        data_ptr++;
        remaining--;
        
        png_byte trans_array[256];
        png_color_16 trans_color_param;
        
        if (remaining >= trans_num) {
            memcpy(trans_array, data_ptr, trans_num);
            data_ptr += trans_num;
            remaining -= trans_num;
            
            if (remaining >= sizeof(png_color_16)) {
                memcpy(&trans_color_param, data_ptr, sizeof(png_color_16));
                data_ptr += sizeof(png_color_16);
                remaining -= sizeof(png_color_16);
                
                // Call png_set_tRNS
                png_set_tRNS(write_ptr, write_info, trans_array, trans_num, &trans_color_param);
            }
        }
    }

    // Cleanup
    cleanup_png_struct(read_ptr, read_info);
    png_destroy_write_struct(&write_ptr, &write_info);
    
    return 0;
}