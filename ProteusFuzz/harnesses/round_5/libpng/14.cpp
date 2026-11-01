// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_expand_16 at pngrtran.c:985:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_set_background_fixed at pngrtran.c:125:1 in png.h
// png_set_gamma at pngrtran.c:901:1 in png.h
// png_set_gamma_fixed at pngrtran.c:859:1 in png.h
// png_set_alpha_mode at pngrtran.c:444:1 in png.h
// png_set_alpha_mode_fixed at pngrtran.c:344:1 in png.h
// png_set_rgb_to_gray at pngrtran.c:1085:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_set_scale_16 at pngrtran.c:171:1 in png.h
// png_set_flush at pngwrite.c:966:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
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

// Helper to create a minimal valid PNG file in memory or on disk
static bool create_dummy_png(const char* filename, const uint8_t* data, size_t size) {
    FILE* f = fopen(filename, "wb");
    if (!f) return false;
    
    // Minimal PNG signature
    const uint8_t png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(png_sig, 1, 8, f);
    
    // Write IHDR chunk (13 bytes)
    uint32_t width = 1, height = 1;
    uint8_t bit_depth = 8;
    uint8_t color_type = 2; // RGB
    uint8_t compression = 0;
    uint8_t filter = 0;
    uint8_t interlace = 0;
    
    uint8_t ihdr_data[13];
    ihdr_data[0] = (width >> 24) & 0xFF;
    ihdr_data[1] = (width >> 16) & 0xFF;
    ihdr_data[2] = (width >> 8) & 0xFF;
    ihdr_data[3] = width & 0xFF;
    ihdr_data[4] = (height >> 24) & 0xFF;
    ihdr_data[5] = (height >> 16) & 0xFF;
    ihdr_data[6] = (height >> 8) & 0xFF;
    ihdr_data[7] = height & 0xFF;
    ihdr_data[8] = bit_depth;
    ihdr_data[9] = color_type;
    ihdr_data[10] = compression;
    ihdr_data[11] = filter;
    ihdr_data[12] = interlace;
    
    // Write IHDR length
    uint32_t len = 13;
    fwrite(&len, 4, 1, f);
    const char* type = "IHDR";
    fwrite(type, 4, 1, f);
    fwrite(ihdr_data, 13, 1, f);
    // Dummy CRC
    uint32_t crc = 0;
    fwrite(&crc, 4, 1, f);
    
    // Write IDAT chunk with fuzz data
    if (size > 0) {
        uint32_t idat_len = size;
        fwrite(&idat_len, 4, 1, f);
        const char* idat_type = "IDAT";
        fwrite(idat_type, 4, 1, f);
        fwrite(data, size, 1, f);
        uint32_t idat_crc = 0;
        fwrite(&idat_crc, 4, 1, f);
    }
    
    // IEND chunk
    uint32_t iend_len = 0;
    fwrite(&iend_len, 4, 1, f);
    const char* iend_type = "IEND";
    fwrite(iend_type, 4, 1, f);
    uint32_t iend_crc = 0;
    fwrite(&iend_crc, 4, 1, f);
    
    fclose(f);
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    const char* filename = "./dummy_file";
    if (!create_dummy_png(filename, Data, Size)) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Read the PNG signature
    // In newer libpng versions, png_read_sig is deprecated or removed in favor of checking manually
    // or just calling png_read_info which handles it. 
    // However, if png_read_sig exists, use it. If not, skip.
    // To be safe across versions, we can try to read the signature manually or just call png_read_info.
    // png_read_info expects the signature to be read or checked. 
    // Let's read the signature manually to be safe.
    png_byte sig[8];
    if (fread(sig, 1, 8, fp) != 8) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    if (png_sig_cmp(sig, 0, 8)) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Read info (requires 2 arguments in newer versions)
    png_read_info(png_ptr, info_ptr);

    // --- Fuzzing Target APIs ---

    // 1. png_set_expand_16
    png_set_expand_16(png_ptr);

    // 2. png_set_swap_alpha
    png_set_swap_alpha(png_ptr);

    // 3. png_set_background
    png_color_16 background_color;
    memset(&background_color, 0, sizeof(background_color));
    background_color.red = Data[0];
    background_color.green = Data[1];
    background_color.blue = Data[2];
    
    int bg_gamma_code = Data[3] % 4;
    int need_expand = (Data[4] % 2);
    png_fixed_point bg_gamma = (png_fixed_point)((int32_t)(Data[5] << 16));
    
    #ifdef PNG_READ_BACKGROUND_SUPPORTED
    png_set_background_fixed(png_ptr, &background_color, bg_gamma_code, need_expand, bg_gamma);
    #endif

    // 4. png_set_gamma
    double screen_gamma = (double)(Data[6] / 255.0) + 0.1;
    double file_gamma = (double)(Data[7] / 255.0) + 0.1;
    
    #ifdef PNG_GAMMA_SUPPORTED
    png_set_gamma(png_ptr, screen_gamma, file_gamma);
    
    png_fixed_point sg_fixed = (png_fixed_point)(screen_gamma * 100000);
    png_fixed_point fg_fixed = (png_fixed_point)(file_gamma * 100000);
    png_set_gamma_fixed(png_ptr, sg_fixed, fg_fixed);
    #endif

    // 5. png_set_alpha_mode
    int alpha_mode = Data[8] % 4;
    double output_gamma = (double)(Data[9] / 255.0) + 0.1;
    
    #ifdef PNG_READ_ALPHA_MODE_SUPPORTED
    png_set_alpha_mode(png_ptr, alpha_mode, output_gamma);
    
    png_fixed_point og_fixed = (png_fixed_point)(output_gamma * 100000);
    png_set_alpha_mode_fixed(png_ptr, alpha_mode, og_fixed);
    #endif

    // 6. png_set_rgb_to_gray
    int error_action = (Data[10] % 2);
    double red_coeff = (double)(Data[11] / 255.0);
    double green_coeff = (double)(Data[12] / 255.0);
    
    #ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
    png_set_rgb_to_gray(png_ptr, error_action, red_coeff, green_coeff);
    
    png_fixed_point rc_fixed = (png_fixed_point)(red_coeff * 100000);
    png_fixed_point gc_fixed = (png_fixed_point)(green_coeff * 100000);
    png_set_rgb_to_gray_fixed(png_ptr, error_action, rc_fixed, gc_fixed);
    
    png_byte status = png_get_rgb_to_gray_status(png_ptr);
    (void)status;
    #endif

    // 7. png_set_scale_16
    #ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
    png_set_scale_16(png_ptr);
    #endif

    // 8. png_set_flush
    #ifdef PNG_WRITE_FLUSH_SUPPORTED
    int nrows = Data[13] % 100;
    png_set_flush(png_ptr, nrows);
    #endif

    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}