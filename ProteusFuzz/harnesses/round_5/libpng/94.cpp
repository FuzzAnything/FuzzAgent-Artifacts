// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_cHRM at pngset.c:99:1 in png.h
// png_set_cHRM_XYZ at pngset.c:115:1 in png.h
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_set_gAMA at pngset.c:365:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_XYZ at pngget.c:559:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_gAMA at pngget.c:677:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
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
#include <vector>
#include <fstream>
#include <random>

static void write_dummy_png(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) return;
    
    // Write a minimal valid PNG signature
    const uint8_t png_sig[] = {137, 80, 78, 71, 13, 10, 26, 10};
    file.write(reinterpret_cast<const char*>(png_sig), 8);
    
    // Write IHDR chunk (minimal)
    uint32_t width = 1, height = 1, bit_depth = 8, color_type = 2, compression = 0, filter = 0, interlace = 0;
    uint32_t ihdr_len = 13;
    const char ihdr_type[] = "IHDR";
    uint32_t ihdr_crc = 0; // CRC will be invalid, but libpng might handle it gracefully or crash, which is good for fuzzing
    
    file.write(reinterpret_cast<const char*>(&ihdr_len), 4);
    file.write(ihdr_type, 4);
    file.write(reinterpret_cast<const char*>(&width), 4);
    file.write(reinterpret_cast<const char*>(&height), 4);
    file.write(reinterpret_cast<const char*>(&bit_depth), 1);
    file.write(reinterpret_cast<const char*>(&color_type), 1);
    file.write(reinterpret_cast<const char*>(&compression), 1);
    file.write(reinterpret_cast<const char*>(&filter), 1);
    file.write(reinterpret_cast<const char*>(&interlace), 1);
    file.write(reinterpret_cast<const char*>(&ihdr_crc), 4);
    
    // Write IDAT chunk with dummy data
    uint32_t idat_len = Size > 0 ? Size : 1;
    const char idat_type[] = "IDAT";
    uint32_t idat_crc = 0;
    
    file.write(reinterpret_cast<const char*>(&idat_len), 4);
    file.write(idat_type, 4);
    if (Data && Size > 0) {
        file.write(reinterpret_cast<const char*>(Data), Size);
    } else {
        file.write("\0", 1);
    }
    file.write(reinterpret_cast<const char*>(&idat_crc), 4);
    
    // Write IEND chunk
    uint32_t iend_len = 0;
    const char iend_type[] = "IEND";
    uint32_t iend_crc = 0;
    
    file.write(reinterpret_cast<const char*>(&iend_len), 4);
    file.write(iend_type, 4);
    file.write(reinterpret_cast<const char*>(&iend_crc), 4);
    
    file.close();
}

static void* safe_malloc(size_t size) {
    if (size == 0 || size > 1024 * 1024) return nullptr;
    return malloc(size);
}

extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Initialize random number generator for diverse test cases
    static std::mt19937 gen(42);
    std::uniform_real_distribution<double> dist_double(0.0, 1.0);
    std::uniform_int_distribution<int> dist_int(0, 1000000);
    
    // Create dummy file for reading
    write_dummy_png(Data, Size);
    
    // Test writing scenario
    {
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!png_ptr) return 0;
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_write_struct(&png_ptr, nullptr);
            return 0;
        }
        
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return 0;
        }
        
        // Test png_set_cHRM
        double white_x = dist_double(gen);
        double white_y = dist_double(gen);
        double red_x = dist_double(gen);
        double red_y = dist_double(gen);
        double green_x = dist_double(gen);
        double green_y = dist_double(gen);
        double blue_x = dist_double(gen);
        double blue_y = dist_double(gen);
        
        png_set_cHRM(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y);
        
        // Test png_set_cHRM_XYZ
        double red_X = dist_double(gen);
        double red_Y = dist_double(gen);
        double red_Z = dist_double(gen);
        double green_X = dist_double(gen);
        double green_Y = dist_double(gen);
        double green_Z = dist_double(gen);
        double blue_X = dist_double(gen);
        double blue_Y = dist_double(gen);
        double blue_Z = dist_double(gen);
        
        png_set_cHRM_XYZ(png_ptr, info_ptr, red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z);
        
        // Test png_set_cHRM_fixed
        png_fixed_point int_white_x = static_cast<png_fixed_point>(dist_int(gen));
        png_fixed_point int_white_y = static_cast<png_fixed_point>(dist_int(gen));
        png_fixed_point int_red_x = static_cast<png_fixed_point>(dist_int(gen));
        png_fixed_point int_red_y = static_cast<png_fixed_point>(dist_int(gen));
        png_fixed_point int_green_x = static_cast<png_fixed_point>(dist_int(gen));
        png_fixed_point int_green_y = static_cast<png_fixed_point>(dist_int(gen));
        png_fixed_point int_blue_x = static_cast<png_fixed_point>(dist_int(gen));
        png_fixed_point int_blue_y = static_cast<png_fixed_point>(dist_int(gen));
        
        png_set_cHRM_fixed(png_ptr, info_ptr, int_white_x, int_white_y, int_red_x, int_red_y, int_green_x, int_green_y, int_blue_x, int_blue_y);
        
        // Test png_set_cHRM_XYZ_fixed
        png_fixed_point int_red_X = static_cast<png_fixed_point>(dist_int(gen));
        png_fixed_point int_red_Y = static_cast<png_fixed_point>(dist_int(gen));
        png_fixed_point int_red_Z = static_cast<png_fixed_point>(dist_int(gen));
        png_fixed_point int_green_X = static_cast<png_fixed_point>(dist_int(gen));
        png_fixed_point int_green_Y = static_cast<png_fixed_point>(dist_int(gen));
        png_fixed_point int_green_Z = static_cast<png_fixed_point>(dist_int(gen));
        png_fixed_point int_blue_X = static_cast<png_fixed_point>(dist_int(gen));
        png_fixed_point int_blue_Y = static_cast<png_fixed_point>(dist_int(gen));
        png_fixed_point int_blue_Z = static_cast<png_fixed_point>(dist_int(gen));
        
        png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, int_red_X, int_red_Y, int_red_Z, int_green_X, int_green_Y, int_green_Z, int_blue_X, int_blue_Y, int_blue_Z);
        
        // Test png_set_sRGB_gAMA_and_cHRM
        int srgb_intent = dist_int(gen) % 4;
        png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, srgb_intent);
        
        // Test png_set_gAMA
        double file_gamma = dist_double(gen);
        png_set_gAMA(png_ptr, info_ptr, file_gamma);
        
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
    
    // Test reading scenario
    {
        FILE *fp = fopen("./dummy_file", "rb");
        if (!fp) return 0;
        
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
        
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            fclose(fp);
            return 0;
        }
        
        png_init_io(png_ptr, fp);
        
        // Try to read PNG info (may fail due to invalid CRC, but that's okay for fuzzing)
        png_read_info(png_ptr, info_ptr);
        
        // Test png_get_cHRM
        double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
        png_get_cHRM(png_ptr, info_ptr, &white_x, &white_y, &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y);
        
        // Test png_get_cHRM_XYZ
        double red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z;
        png_get_cHRM_XYZ(png_ptr, info_ptr, &red_X, &red_Y, &red_Z, &green_X, &green_Y, &green_Z, &blue_X, &blue_Y, &blue_Z);
        
        // Test png_get_cHRM_fixed
        png_fixed_point int_white_x, int_white_y, int_red_x, int_red_y, int_green_x, int_green_y, int_blue_x, int_blue_y;
        png_get_cHRM_fixed(png_ptr, info_ptr, &int_white_x, &int_white_y, &int_red_x, &int_red_y, &int_green_x, &int_green_y, &int_blue_x, &int_blue_y);
        
        // Test png_get_cHRM_XYZ_fixed
        png_fixed_point int_red_X, int_red_Y, int_red_Z, int_green_X, int_green_Y, int_green_Z, int_blue_X, int_blue_Y, int_blue_Z;
        png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &int_red_X, &int_red_Y, &int_red_Z, &int_green_X, &int_green_Y, &int_green_Z, &int_blue_X, &int_blue_Y, &int_blue_Z);
        
        // Test png_get_gAMA
        double file_gamma;
        png_get_gAMA(png_ptr, info_ptr, &file_gamma);
        
        // Test png_get_gAMA_fixed
        png_fixed_point int_file_gamma;
        png_get_gAMA_fixed(png_ptr, info_ptr, &int_file_gamma);
        
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
    }
    
    return 0;
}