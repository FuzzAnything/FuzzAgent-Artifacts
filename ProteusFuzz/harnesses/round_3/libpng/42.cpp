// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_gAMA at pngset.c:365:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_set_cHRM at pngset.c:99:1 in png.h
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_set_cHRM_XYZ at pngset.c:115:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_gamma at pngrtran.c:901:1 in png.h
// png_set_gamma_fixed at pngrtran.c:859:1 in png.h
// png_get_gAMA at pngget.c:677:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_XYZ at pngget.c:559:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
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
#include <cstdlib>
#include <cstdio>

static void write_dummy_png(const char* filename, size_t size, const uint8_t* data) {
    FILE* f = fopen(filename, "wb");
    if (!f) return;
    
    // Write a minimal valid PNG header
    const uint8_t png_sig[] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(png_sig, 1, 8, f);
    
    // Write IHDR chunk (minimal)
    uint32_t width = 1, height = 1, bit_depth = 8, color_type = 2, compression = 0, filter = 0, interlace = 0;
    uint32_t len = 13;
    uint8_t type[] = {'I', 'H', 'D', 'R'};
    
    fwrite(&len, 4, 1, f);
    fwrite(type, 1, 4, f);
    fwrite(&width, 4, 1, f);
    fwrite(&height, 4, 1, f);
    fwrite(&bit_depth, 1, 1, f);
    fwrite(&color_type, 1, 1, f);
    fwrite(&compression, 1, 1, f);
    fwrite(&filter, 1, 1, f);
    fwrite(&interlace, 1, 1, f);
    
    uint32_t crc = 0; // Dummy CRC
    fwrite(&crc, 4, 1, f);
    
    // Write IDAT chunk (minimal, dummy data)
    uint32_t idat_len = 1;
    uint8_t idat_type[] = {'I', 'D', 'A', 'T'};
    uint8_t dummy_data = 0;
    
    fwrite(&idat_len, 4, 1, f);
    fwrite(idat_type, 1, 4, f);
    fwrite(&dummy_data, 1, 1, f);
    fwrite(&crc, 4, 1, f);
    
    // Write IEND chunk
    uint32_t iend_len = 0;
    uint8_t iend_type[] = {'I', 'E', 'N', 'D'};
    
    fwrite(&iend_len, 4, 1, f);
    fwrite(iend_type, 1, 4, f);
    fwrite(&crc, 4, 1, f);
    
    fclose(f);
}

static void safe_png_destroy(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    const char* filename = "./dummy_file";
    
    // Create dummy PNG file
    write_dummy_png(filename, Size, Data);
    
    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Open file for reading
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    png_init_io(png_ptr, fp);
    
    // Read PNG header
    png_read_info(png_ptr, info_ptr);
    
    // Extract data from fuzzer input for testing
    double gamma_val = 0.0;
    double white_x = 0.0, white_y = 0.0;
    double red_x = 0.0, red_y = 0.0;
    double green_x = 0.0, green_y = 0.0;
    double blue_x = 0.0, blue_y = 0.0;
    
    double red_X = 0.0, red_Y = 0.0, red_Z = 0.0;
    double green_X = 0.0, green_Y = 0.0, green_Z = 0.0;
    double blue_X = 0.0, blue_Y = 0.0, blue_Z = 0.0;
    
    double screen_gamma = 0.0, file_gamma = 0.0;
    
    png_fixed_point fixed_gamma = 0;
    png_fixed_point fixed_white_x = 0, fixed_white_y = 0;
    png_fixed_point fixed_red_x = 0, fixed_red_y = 0;
    png_fixed_point fixed_green_x = 0, fixed_green_y = 0;
    png_fixed_point fixed_blue_x = 0, fixed_blue_y = 0;
    
    png_fixed_point fixed_red_X = 0, fixed_red_Y = 0, fixed_red_Z = 0;
    png_fixed_point fixed_green_X = 0, fixed_green_Y = 0, fixed_green_Z = 0;
    png_fixed_point fixed_blue_X = 0, fixed_blue_Y = 0, fixed_blue_Z = 0;
    
    // Parse input data if available
    if (Size >= 2) {
        gamma_val = (double)(Data[0]) / 255.0;
        screen_gamma = (double)(Data[1]) / 255.0;
        file_gamma = (double)(Data[1]) / 255.0;
    }
    
    if (Size >= 10) {
        white_x = (double)(Data[2]) / 255.0;
        white_y = (double)(Data[3]) / 255.0;
        red_x = (double)(Data[4]) / 255.0;
        red_y = (double)(Data[5]) / 255.0;
        green_x = (double)(Data[6]) / 255.0;
        green_y = (double)(Data[7]) / 255.0;
        blue_x = (double)(Data[8]) / 255.0;
        blue_y = (double)(Data[9]) / 255.0;
    }
    
    if (Size >= 19) {
        red_X = (double)(Data[10]) / 255.0;
        red_Y = (double)(Data[11]) / 255.0;
        red_Z = (double)(Data[12]) / 255.0;
        green_X = (double)(Data[13]) / 255.0;
        green_Y = (double)(Data[14]) / 255.0;
        green_Z = (double)(Data[15]) / 255.0;
        blue_X = (double)(Data[16]) / 255.0;
        blue_Y = (double)(Data[17]) / 255.0;
        blue_Z = (double)(Data[18]) / 255.0;
    }
    
    // Convert to fixed point
    fixed_gamma = (png_fixed_point)(gamma_val * 100000);
    fixed_white_x = (png_fixed_point)(white_x * 100000);
    fixed_white_y = (png_fixed_point)(white_y * 100000);
    fixed_red_x = (png_fixed_point)(red_x * 100000);
    fixed_red_y = (png_fixed_point)(red_y * 100000);
    fixed_green_x = (png_fixed_point)(green_x * 100000);
    fixed_green_y = (png_fixed_point)(green_y * 100000);
    fixed_blue_x = (png_fixed_point)(blue_x * 100000);
    fixed_blue_y = (png_fixed_point)(blue_y * 100000);
    
    fixed_red_X = (png_fixed_point)(red_X * 100000);
    fixed_red_Y = (png_fixed_point)(red_Y * 100000);
    fixed_red_Z = (png_fixed_point)(red_Z * 100000);
    fixed_green_X = (png_fixed_point)(green_X * 100000);
    fixed_green_Y = (png_fixed_point)(green_Y * 100000);
    fixed_green_Z = (png_fixed_point)(green_Z * 100000);
    fixed_blue_X = (png_fixed_point)(blue_X * 100000);
    fixed_blue_Y = (png_fixed_point)(blue_Y * 100000);
    fixed_blue_Z = (png_fixed_point)(blue_Z * 100000);
    
    // Test png_set_gAMA
    png_set_gAMA(png_ptr, info_ptr, gamma_val);
    png_set_gAMA_fixed(png_ptr, info_ptr, fixed_gamma);
    
    // Test png_set_cHRM
    png_set_cHRM(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y);
    png_set_cHRM_fixed(png_ptr, info_ptr, fixed_white_x, fixed_white_y, fixed_red_x, fixed_red_y, fixed_green_x, fixed_green_y, fixed_blue_x, fixed_blue_y);
    
    // Test png_set_cHRM_XYZ
    png_set_cHRM_XYZ(png_ptr, info_ptr, red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z);
    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, fixed_red_X, fixed_red_Y, fixed_red_Z, fixed_green_X, fixed_green_Y, fixed_green_Z, fixed_blue_X, fixed_blue_Y, fixed_blue_Z);
    
    // Test png_set_gamma
    png_set_gamma(png_ptr, screen_gamma, file_gamma);
    png_set_gamma_fixed(png_ptr, (png_fixed_point)(screen_gamma * 100000), (png_fixed_point)(file_gamma * 100000));
    
    // Test png_get_gAMA
    double retrieved_gamma = 0.0;
    png_get_gAMA(png_ptr, info_ptr, &retrieved_gamma);
    png_fixed_point retrieved_fixed_gamma = 0;
    png_get_gAMA_fixed(png_ptr, info_ptr, &retrieved_fixed_gamma);
    
    // Test png_get_cHRM
    double r_white_x = 0, r_white_y = 0, r_red_x = 0, r_red_y = 0, r_green_x = 0, r_green_y = 0, r_blue_x = 0, r_blue_y = 0;
    png_get_cHRM(png_ptr, info_ptr, &r_white_x, &r_white_y, &r_red_x, &r_red_y, &r_green_x, &r_green_y, &r_blue_x, &r_blue_y);
    
    png_fixed_point r_fixed_white_x = 0, r_fixed_white_y = 0, r_fixed_red_x = 0, r_fixed_red_y = 0, r_fixed_green_x = 0, r_fixed_green_y = 0, r_fixed_blue_x = 0, r_fixed_blue_y = 0;
    png_get_cHRM_fixed(png_ptr, info_ptr, &r_fixed_white_x, &r_fixed_white_y, &r_fixed_red_x, &r_fixed_red_y, &r_fixed_green_x, &r_fixed_green_y, &r_fixed_blue_x, &r_fixed_blue_y);
    
    // Test png_get_cHRM_XYZ
    double r_red_X = 0, r_red_Y = 0, r_red_Z = 0, r_green_X = 0, r_green_Y = 0, r_green_Z = 0, r_blue_X = 0, r_blue_Y = 0, r_blue_Z = 0;
    png_get_cHRM_XYZ(png_ptr, info_ptr, &r_red_X, &r_red_Y, &r_red_Z, &r_green_X, &r_green_Y, &r_green_Z, &r_blue_X, &r_blue_Y, &r_blue_Z);
    
    png_fixed_point r_fixed_red_X = 0, r_fixed_red_Y = 0, r_fixed_red_Z = 0, r_fixed_green_X = 0, r_fixed_green_Y = 0, r_fixed_green_Z = 0, r_fixed_blue_X = 0, r_fixed_blue_Y = 0, r_fixed_blue_Z = 0;
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &r_fixed_red_X, &r_fixed_red_Y, &r_fixed_red_Z, &r_fixed_green_X, &r_fixed_green_Y, &r_fixed_green_Z, &r_fixed_blue_X, &r_fixed_blue_Y, &r_fixed_blue_Z);
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    
    return 0;
}