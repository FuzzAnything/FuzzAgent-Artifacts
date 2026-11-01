// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_rgb_to_gray at pngrtran.c:1085:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_set_option at png.c:3740:1 in png.h
// png_set_user_transform_info at pngtrans.c:805:1 in png.h
// png_set_expand_gray_1_2_4_to_8 at pngrtran.c:957:1 in png.h
// png_set_interlace_handling at pngtrans.c:98:1 in png.h
// png_set_bgr at pngtrans.c:20:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
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
#include <fstream>

static uint32_t calculate_crc32(const uint8_t *data, size_t length) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; ++i) {
        crc ^= data[i];
        for (int j = 0; j < 8; ++j) {
            crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
        }
    }
    return crc ^ 0xFFFFFFFF;
}

static void write_dummy_png(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary | std::ios::trunc);
    if (!file) return;
    
    const uint8_t png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    file.write(reinterpret_cast<const char*>(png_sig), 8);
    
    uint32_t width = 1;
    uint32_t height = 1;
    uint8_t bit_depth = 8;
    uint8_t color_type = 2; 
    uint8_t compression = 0;
    uint8_t filter = 0;
    uint8_t interlace = 0;
    
    uint8_t ihdr_data[13];
    std::memcpy(ihdr_data, &width, 4);
    std::memcpy(ihdr_data + 4, &height, 4);
    ihdr_data[8] = bit_depth;
    ihdr_data[9] = color_type;
    ihdr_data[10] = compression;
    ihdr_data[11] = filter;
    ihdr_data[12] = interlace;
    
    uint32_t ihdr_len = 13;
    const char *ihdr_name = "IHDR";
    
    std::vector<uint8_t> ihdr_combined(17);
    std::memcpy(ihdr_combined.data(), ihdr_name, 4);
    std::memcpy(ihdr_combined.data() + 4, ihdr_data, 13);
    uint32_t ihdr_crc = calculate_crc32(ihdr_combined.data(), 17);
    
    file.write(reinterpret_cast<const char*>(&ihdr_len), 4);
    file.write(ihdr_name, 4);
    file.write(reinterpret_cast<const char*>(ihdr_data), 13);
    file.write(reinterpret_cast<const char*>(&ihdr_crc), 4);
    
    uint8_t idat_data[4] = {0, 0, 0, 0};
    uint32_t idat_len = 4;
    const char *idat_name = "IDAT";
    
    std::vector<uint8_t> idat_combined(8);
    std::memcpy(idat_combined.data(), idat_name, 4);
    std::memcpy(idat_combined.data() + 4, idat_data, 4);
    uint32_t idat_crc = calculate_crc32(idat_combined.data(), 8);
    
    file.write(reinterpret_cast<const char*>(&idat_len), 4);
    file.write(idat_name, 4);
    file.write(reinterpret_cast<const char*>(idat_data), 4);
    file.write(reinterpret_cast<const char*>(&idat_crc), 4);
    
    uint32_t iend_len = 0;
    const char *iend_name = "IEND";
    
    std::vector<uint8_t> iend_combined(4);
    std::memcpy(iend_combined.data(), iend_name, 4);
    uint32_t iend_crc = calculate_crc32(iend_combined.data(), 4);
    
    file.write(reinterpret_cast<const char*>(&iend_len), 4);
    file.write(iend_name, 4);
    file.write(reinterpret_cast<const char*>(&iend_crc), 4);
    
    if (Data && Size > 0) {
        file.write(reinterpret_cast<const char*>(Data), Size);
    }
}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return nullptr;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return nullptr;
    }
    
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return nullptr;
    }
    
    return png_ptr;
}

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    write_dummy_png(Data, Size);
    
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;
    
    png_structp png_ptr = create_png_struct();
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }
    
    png_infop info_ptr = nullptr;
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        fclose(fp);
        return 0;
    }
    
    png_init_io(png_ptr, fp);
    
    // Read and check signature manually to avoid incorrect png_sig_cmp usage
    png_byte signature[8];
    if (fread(signature, 1, 8, fp) != 8) {
        cleanup_png(png_ptr, info_ptr);
        fclose(fp);
        return 0;
    }
    
    if (png_sig_cmp(signature, 0, 8) != 0) {
        cleanup_png(png_ptr, info_ptr);
        fclose(fp);
        return 0;
    }
    
    // Read the info
    png_read_info(png_ptr, info_ptr);
    
    // Fuzz png_set_rgb_to_gray
    if (Size >= 3) {
        int error_action = Data[0] % 2;
        double red = (Data[1] / 255.0);
        double green = (Data[2] / 255.0);
        png_set_rgb_to_gray(png_ptr, error_action, red, green);
        
        png_byte status = png_get_rgb_to_gray_status(png_ptr);
        (void)status;
    }
    
    // Fuzz png_set_option
    if (Size >= 2) {
        int option = (Data[0] % 10) * 2;
        int onoff = Data[1] % 2;
        int prev_state = png_set_option(png_ptr, option, onoff);
        (void)prev_state;
    }
    
    // Fuzz png_set_user_transform_info
    if (Size >= 3) {
        void *user_ptr = (void*)(uintptr_t)(Data[0] << 24 | Data[1] << 16 | Data[2] << 8 | 0);
        int depth = Data[0] % 16;
        int channels = Data[1] % 4;
        png_set_user_transform_info(png_ptr, user_ptr, depth, channels);
    }
    
    // Fuzz png_set_expand_gray_1_2_4_to_8
    png_set_expand_gray_1_2_4_to_8(png_ptr);
    
    // Fuzz png_set_interlace_handling
    int passes = png_set_interlace_handling(png_ptr);
    (void)passes;
    
    // Fuzz png_set_bgr
    png_set_bgr(png_ptr);
    
    // Cleanup
    cleanup_png(png_ptr, info_ptr);
    fclose(fp);
    
    return 0;
}