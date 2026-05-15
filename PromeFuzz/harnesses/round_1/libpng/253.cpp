// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_tRNS at pngset.c:1144:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_set_background at pngrtran.c:155:1 in png.h
// png_set_background_fixed at pngrtran.c:125:1 in png.h
// png_get_tRNS at pngget.c:1202:1 in png.h
// png_set_tRNS_to_alpha at pngrtran.c:969:1 in png.h
// png_set_expand_16 at pngrtran.c:985:1 in png.h
// png_set_scale_16 at pngrtran.c:171:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <setjmp.h>

#include "code/png.h"

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing
}

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing - we won't actually read image data
}

static png_byte dummy_trans_alpha[256];
static png_color_16 dummy_trans_color;
static png_color_16 dummy_background_color;

extern "C" int LLVMFuzzerTestOneInput_253(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                  dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Set up read function
    png_set_read_fn(png_ptr, nullptr, dummy_read_fn);

    // Read PNG header info
    png_read_info(png_ptr, info_ptr);

    // Get PNG color type and bit depth to inform transformations
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    // Use fuzz data to initialize parameters
    // We'll use the first bytes of data to determine parameters
    size_t data_offset = 0;
    
    // Initialize dummy arrays with fuzz data
    if (Size > 0) {
        size_t trans_size = Size > 256 ? 256 : Size;
        for (size_t i = 0; i < trans_size; i++) {
            dummy_trans_alpha[i] = Data[i % Size];
        }
        data_offset += trans_size;
    }

    // Initialize color structures with fuzz data
    if (Size > data_offset + 5) {
        dummy_trans_color.index = Data[data_offset % Size];
        dummy_trans_color.red = (Data[(data_offset + 1) % Size] << 8) | Data[(data_offset + 2) % Size];
        dummy_trans_color.green = (Data[(data_offset + 3) % Size] << 8) | Data[(data_offset + 4) % Size];
        dummy_trans_color.blue = (Data[(data_offset + 5) % Size] << 8) | Data[(data_offset + 6) % Size];
        data_offset += 7;
    }

    if (Size > data_offset + 5) {
        dummy_background_color.index = Data[data_offset % Size];
        dummy_background_color.red = (Data[(data_offset + 1) % Size] << 8) | Data[(data_offset + 2) % Size];
        dummy_background_color.green = (Data[(data_offset + 3) % Size] << 8) | Data[(data_offset + 4) % Size];
        dummy_background_color.blue = (Data[(data_offset + 5) % Size] << 8) | Data[(data_offset + 6) % Size];
        data_offset += 7;
    }

    // Determine integer parameters from fuzz data
    int num_trans = Size > 0 ? Data[0] % 257 : 0;  // 0-256
    int background_gamma_code = Size > 1 ? Data[1] % 4 : 0;  // 0-3
    int need_expand = Size > 2 ? Data[2] % 2 : 0;  // 0 or 1
    double background_gamma = Size > 3 ? (Data[3] / 255.0) * 10.0 : 1.0;
    png_fixed_point fixed_gamma = Size > 4 ? (Data[4] * 1000) : 1000;

    // Call target API functions with different combinations
    // Try png_set_tRNS with different configurations
    if (color_type == PNG_COLOR_TYPE_PALETTE || color_type == PNG_COLOR_TYPE_GRAY || 
        color_type == PNG_COLOR_TYPE_RGB) {
        png_set_tRNS(png_ptr, info_ptr, dummy_trans_alpha, num_trans, &dummy_trans_color);
    }

    // Try png_set_gray_to_rgb if appropriate
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png_ptr);
    }

    // Try png_set_background with different configurations
    #ifdef PNG_READ_BACKGROUND_SUPPORTED
    png_set_background(png_ptr, &dummy_background_color, background_gamma_code, 
                       need_expand, background_gamma);
    
    png_set_background_fixed(png_ptr, &dummy_background_color, background_gamma_code,
                             need_expand, fixed_gamma);
    #endif

    // Try png_get_tRNS to retrieve transparency data
    png_byte* trans_alpha_out = nullptr;
    int num_trans_out = 0;
    png_color_16p trans_color_out = nullptr;
    png_get_tRNS(png_ptr, info_ptr, &trans_alpha_out, &num_trans_out, &trans_color_out);

    // Try png_set_tRNS_to_alpha
    png_set_tRNS_to_alpha(png_ptr);

    // Try png_set_expand_16
    png_set_expand_16(png_ptr);

    // Try png_set_scale_16 if supported
    #ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
    png_set_scale_16(png_ptr);
    #endif

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}