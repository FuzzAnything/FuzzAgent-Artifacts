// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_XYZ at pngget.c:559:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_mDCV_fixed at pngget.c:829:1 in png.h
// png_get_cLLI at pngget.c:808:1 in png.h
// png_get_cLLI_fixed at pngget.c:788:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_get_sCAL at pngget.c:1049:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_sCAL_s at pngget.c:1067:1 in png.h
// png_set_cHRM at pngset.c:99:1 in png.h
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_set_mDCV_fixed at pngset.c:225:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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
#include <setjmp.h>
#include <fstream>

static void safe_png_error(png_structp png_ptr, png_const_charp error_msg) {
    // Suppress error output for fuzzing
}

static void safe_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    // Suppress warning output for fuzzing
}

static void write_dummy_png(const uint8_t *data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) return;
    if (size > 0) {
        file.write(reinterpret_cast<const char*>(data), size);
    } else {
        // Write a minimal valid PNG header if input is empty to avoid immediate read failure
        // Signature: 8 bytes
        uint8_t signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
        file.write(reinterpret_cast<char*>(signature), 8);
        // IHDR chunk: 4 bytes length, 4 bytes type, data, 4 bytes CRC
        uint32_t ihdr_len = 13;
        uint8_t ihdr_type[4] = {'I', 'H', 'D', 'R'};
        uint8_t ihdr_data[13] = {0, 0, 0, 1, 0, 0, 0, 1, 8, 2, 0, 0, 0}; // 1x1 RGB
        uint32_t ihdr_crc = 0; // Fake CRC for fuzzing
        
        file.write(reinterpret_cast<char*>(&ihdr_len), 4);
        file.write(reinterpret_cast<char*>(ihdr_type), 4);
        file.write(reinterpret_cast<char*>(ihdr_data), 13);
        file.write(reinterpret_cast<char*>(&ihdr_crc), 4);
        
        // IDAT chunk (minimal)
        uint32_t idat_len = 10;
        uint8_t idat_type[4] = {'I', 'D', 'A', 'T'};
        uint8_t idat_data[10] = {0x78, 0x9c, 0x62, 0x60, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01};
        uint32_t idat_crc = 0;
        
        file.write(reinterpret_cast<char*>(&idat_len), 4);
        file.write(reinterpret_cast<char*>(idat_type), 4);
        file.write(reinterpret_cast<char*>(idat_data), 10);
        file.write(reinterpret_cast<char*>(&idat_crc), 4);
        
        // IEND chunk
        uint32_t iend_len = 0;
        uint8_t iend_type[4] = {'I', 'E', 'N', 'D'};
        uint32_t iend_crc = 0;
        
        file.write(reinterpret_cast<char*>(&iend_len), 4);
        file.write(reinterpret_cast<char*>(iend_type), 4);
        file.write(reinterpret_cast<char*>(&iend_crc), 4);
    }
    file.close();
}

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_png(Data, Size);

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    png_infop end_info = nullptr;

    // Initialize PNG read structure
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, safe_png_error, safe_png_warning);
    if (!png_ptr) return 0;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Use setjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return 0;
    }

    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Read signature manually to avoid missing symbol in some builds
    png_byte signature[8];
    if (fread(signature, 1, 8, fp) != 8) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }
    
    // Check signature
    if (png_sig_cmp(signature, 0, 8) != 0) {
        // Signature mismatch, but we continue to test other APIs on potentially invalid data
    }

    // Read IHDR
    png_read_info(png_ptr, info_ptr);

    // Test png_get_IHDR
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);

    // Test png_get_cHRM (and variants)
    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    png_get_cHRM(png_ptr, info_ptr, &white_x, &white_y, &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y);
    
    png_fixed_point int_white_x, int_white_y, int_red_x, int_red_y, int_green_x, int_green_y, int_blue_x, int_blue_y;
    png_get_cHRM_fixed(png_ptr, info_ptr, &int_white_x, &int_white_y, &int_red_x, &int_red_y, &int_green_x, &int_green_y, &int_blue_x, &int_blue_y);

    double red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z;
    png_get_cHRM_XYZ(png_ptr, info_ptr, &red_X, &red_Y, &red_Z, &green_X, &green_Y, &green_Z, &blue_X, &blue_Y, &blue_Z);

    png_fixed_point int_red_X, int_red_Y, int_red_Z, int_green_X, int_green_Y, int_green_Z, int_blue_X, int_blue_Y, int_blue_Z;
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &int_red_X, &int_red_Y, &int_red_Z, &int_green_X, &int_green_Y, &int_green_Z, &int_blue_X, &int_blue_Y, &int_blue_Z);

    // Test png_get_mDCV_fixed
    png_uint_32 max_lum, min_lum;
    png_get_mDCV_fixed(png_ptr, info_ptr, &int_white_x, &int_white_y, &int_red_x, &int_red_y,
                       &int_green_x, &int_green_y, &int_blue_x, &int_blue_y,
                       &max_lum, &min_lum);

    // Test png_get_cLLI
    double max_content, max_frame_avg;
    png_get_cLLI(png_ptr, info_ptr, &max_content, &max_frame_avg);
    
    png_uint_32 max_content_fixed, max_frame_avg_fixed;
    png_get_cLLI_fixed(png_ptr, info_ptr, &max_content_fixed, &max_frame_avg_fixed);

    // Test png_get_oFFs
    png_int_32 offset_x, offset_y;
    int unit_type;
    png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, &unit_type);

    // Test png_get_cICP
    png_byte colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag;
    png_get_cICP(png_ptr, info_ptr, &colour_primaries, &transfer_function, &matrix_coefficients, &video_full_range_flag);

    // Test png_get_sCAL
    int unit;
    double width_s, height_s;
    png_get_sCAL(png_ptr, info_ptr, &unit, &width_s, &height_s);
    
    png_fixed_point width_fixed, height_fixed;
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width_fixed, &height_fixed);
    
    char *swidth, *sheight;
    png_get_sCAL_s(png_ptr, info_ptr, &unit, &swidth, &sheight);

    // Test setters to populate data for subsequent gets if needed
    png_set_cHRM(png_ptr, info_ptr, 0.3127, 0.3290, 0.6400, 0.3300, 0.3000, 0.6000, 0.1500, 0.0600);
    png_set_cHRM_fixed(png_ptr, info_ptr, 31270, 32900, 64000, 33000, 30000, 60000, 15000, 6000);
    
    png_set_mDCV_fixed(png_ptr, info_ptr, 31270, 32900, 64000, 33000, 30000, 60000, 15000, 6000, 100000, 100);
    
    // Read the rest of the file
    png_read_end(png_ptr, end_info);
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    
    fclose(fp);
    
    return 0;
}