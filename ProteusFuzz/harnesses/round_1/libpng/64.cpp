// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_sig_cmp at png.c:80:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_x_offset_microns at pngget.c:244:1 in png.h
// png_get_y_offset_microns at pngget.c:264:1 in png.h
// png_get_y_offset_pixels at pngget.c:304:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_set_oFFs at pngset.c:465:1 in png.h
// png_get_x_offset_microns at pngget.c:244:1 in png.h
// png_get_y_offset_microns at pngget.c:264:1 in png.h
// png_get_y_offset_pixels at pngget.c:304:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
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
#include <vector>
#include <fstream>
#include <sstream>

// Helper to create a minimal valid PNG file from input data
static bool create_png_file(const uint8_t *data, size_t size, const char *filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    // Minimal PNG header (8 bytes)
    const uint8_t png_sig[] = {137, 80, 78, 71, 13, 10, 26, 10};
    file.write(reinterpret_cast<const char*>(png_sig), 8);

    // IHDR chunk (13 bytes data + 4 bytes length + 4 bytes type + 4 bytes CRC)
    // We'll create a minimal 1x1 grayscale image
    uint32_t width = 1;
    uint32_t height = 1;
    uint8_t bit_depth = 8;
    uint8_t color_type = 0; // Grayscale
    uint8_t compression = 0;
    uint8_t filter = 0;
    uint8_t interlace = 0;

    uint8_t ihdr_data[13];
    memcpy(ihdr_data, &width, 4);
    memcpy(ihdr_data + 4, &height, 4);
    ihdr_data[8] = bit_depth;
    ihdr_data[9] = color_type;
    ihdr_data[10] = compression;
    ihdr_data[11] = filter;
    ihdr_data[12] = interlace;

    uint32_t ihdr_len = 13;
    uint8_t ihdr_type[] = {'I', 'H', 'D', 'R'};
    
    file.write(reinterpret_cast<const char*>(&ihdr_len), 4);
    file.write(reinterpret_cast<const char*>(ihdr_type), 4);
    file.write(reinterpret_cast<const char*>(ihdr_data), 13);
    
    // Dummy CRC (0) - libpng will validate but we just need a structure to parse
    uint32_t dummy_crc = 0;
    file.write(reinterpret_cast<const char*>(&dummy_crc), 4);

    // IDAT chunk (minimal compressed data)
    // For a 1x1 grayscale image, we need at least one byte of compressed data
    uint8_t idat_data[] = {0x78, 0x9c, 0x62, 0x60, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01};
    uint32_t idat_len = sizeof(idat_data);
    uint8_t idat_type[] = {'I', 'D', 'A', 'T'};
    
    file.write(reinterpret_cast<const char*>(&idat_len), 4);
    file.write(reinterpret_cast<const char*>(idat_type), 4);
    file.write(reinterpret_cast<const char*>(idat_data), idat_len);
    file.write(reinterpret_cast<const char*>(&dummy_crc), 4);

    // IEND chunk
    uint32_t iend_len = 0;
    uint8_t iend_type[] = {'I', 'E', 'N', 'D'};
    
    file.write(reinterpret_cast<const char*>(&iend_len), 4);
    file.write(reinterpret_cast<const char*>(iend_type), 4);
    file.write(reinterpret_cast<const char*>(&dummy_crc), 4);

    file.close();
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Create a minimal PNG file
    if (!create_png_file(Data, Size, filename)) {
        return 0;
    }

    // Initialize libpng structures
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Read the PNG file
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        return 0;
    }

    // Check for PNG signature
    png_byte header[8];
    if (fread(header, 1, 8, fp) != 8 || png_sig_cmp(header, 0, 8)) {
        fclose(fp);
        return 0;
    }

    // Create png_struct
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    // Create png_info
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
        return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, fp);

    // Read the header
    png_read_info(png_ptr, info_ptr);

    // Test the target API functions
    png_int_32 x_offset_microns = png_get_x_offset_microns(png_ptr, info_ptr);
    png_int_32 y_offset_microns = png_get_y_offset_microns(png_ptr, info_ptr);
    png_int_32 y_offset_pixels = png_get_y_offset_pixels(png_ptr, info_ptr);
    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);

    // Test png_get_x_offset_inches_fixed if available
    #ifdef PNG_FIXED_POINT_SUPPORTED
    png_fixed_point x_offset_inches_fixed = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    #endif

    // Test png_set_oFFs with various values from input data
    if (Size >= 9) {
        png_int_32 offset_x = static_cast<png_int_32>(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
        png_int_32 offset_y = static_cast<png_int_32>(Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24));
        int unit_type = Data[8];
        
        png_set_oFFs(png_ptr, info_ptr, offset_x, offset_y, unit_type);
        
        // Re-read the values after setting
        x_offset_microns = png_get_x_offset_microns(png_ptr, info_ptr);
        y_offset_microns = png_get_y_offset_microns(png_ptr, info_ptr);
        y_offset_pixels = png_get_y_offset_pixels(png_ptr, info_ptr);
        x_offset_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);
        
        #ifdef PNG_FIXED_POINT_SUPPORTED
        x_offset_inches_fixed = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
        #endif
    }

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);

    return 0;
}