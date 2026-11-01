// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_alpha_mode at pngrtran.c:444:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_rgb_to_gray at pngrtran.c:1085:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_bgr at pngtrans.c:20:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_strip_alpha at pngrtran.c:198:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_background at pngrtran.c:155:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_gamma at pngrtran.c:901:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
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
#include <cmath>

static void write_dummy_png(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        // Write a minimal valid PNG header (8 bytes)
        uint8_t header[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
        fwrite(header, 1, 8, f);
        
        // Write IHDR chunk (13 bytes data + 4 bytes length + 4 bytes CRC)
        uint8_t ihdr[13] = {
            0x00, 0x00, 0x00, 0x01, // width: 1
            0x00, 0x00, 0x00, 0x01, // height: 1
            0x08,                   // bit depth: 8
            0x02,                   // color type: RGB
            0x00,                   // compression: 0
            0x00,                   // filter: 0
            0x00                    // interlace: 0
        };
        uint32_t len = 13;
        fwrite(&len, 4, 1, f);
        fwrite("IHDR", 4, 1, f);
        fwrite(ihdr, 1, 13, f);
        // Dummy CRC (0)
        uint32_t crc = 0;
        fwrite(&crc, 4, 1, f);

        // Write IDAT chunk (minimal compressed data)
        // We just write some dummy data to satisfy the parser
        uint8_t idat_data[10];
        if (size > 0) {
            memcpy(idat_data, data, size < 10 ? size : 10);
        } else {
            memset(idat_data, 0, 10);
        }
        len = 10;
        fwrite(&len, 4, 1, f);
        fwrite("IDAT", 4, 1, f);
        fwrite(idat_data, 1, 10, f);
        fwrite(&crc, 4, 1, f);

        // Write IEND chunk
        len = 0;
        fwrite(&len, 4, 1, f);
        fwrite("IEND", 4, 1, f);
        fwrite(&crc, 4, 1, f);

        fclose(f);
    }
}

static png_structp create_png_struct(const uint8_t *data, size_t size) {
    write_dummy_png(data, size);
    
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return NULL;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return NULL;
    }
    
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return NULL;
    }
    
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return NULL;
    }
    
    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);
    
    // Close file immediately after reading info to allow subsequent operations
    fclose(fp);
    
    return png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create a valid PNG struct and read info
    png_structp png_ptr = create_png_struct(Data, Size);
    if (!png_ptr) return 0;
    
    // Helper to safely call functions that might jump
    int result = 0;
    
    // Test png_set_alpha_mode
    // Modes: PNG_ALPHA_PNG, PNG_ALPHA_GAMMA, PNG_ALPHA_BROKEN, PNG_ALPHA_OPTIMIZE
    // Gammas: PNG_DEFAULT_sRGB, PNG_GAMMA_MAC_18, PNG_GAMMA_sRGB, PNG_GAMMA_LINEAR
    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
        int mode = Data[0] % 4;
        double gamma = 0.0;
        switch (Data[1] % 4) {
            case 0: gamma = -1.0; break; // PNG_DEFAULT_sRGB
            case 1: gamma = -2.0; break; // PNG_GAMMA_MAC_18
            case 2: gamma = 2.2; break;  // Approx sRGB
            case 3: gamma = 1.0; break;  // Linear
        }
        png_set_alpha_mode(png_ptr, mode, gamma);
        result = 1;
    }
    
    // Test png_set_rgb_to_gray
    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
        int error_action = (Data[2] % 2) ? 1 : 0;
        double red = (Data[3] % 256) / 255.0;
        double green = (Data[4] % 256) / 255.0;
        png_set_rgb_to_gray(png_ptr, error_action, red, green);
        result = 1;
    }
    
    // Test png_set_bgr
    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
        png_set_bgr(png_ptr);
        result = 1;
    }
    
    // Test png_set_strip_alpha
    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
        png_set_strip_alpha(png_ptr);
        result = 1;
    }
    
    // Test png_set_background
    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
        png_color_16 background_color;
        background_color.red = Data[5] % 256;
        background_color.green = Data[6] % 256;
        background_color.blue = Data[7] % 256;
        background_color.index = Data[8] % 256;
        
        int gamma_code = Data[9] % 4; // 0-3
        int need_expand = (Data[10] % 2) ? 1 : 0;
        double bg_gamma = (Data[11] % 256) / 100.0;
        
        png_set_background(png_ptr, &background_color, gamma_code, need_expand, bg_gamma);
        result = 1;
    }
    
    // Test png_set_gamma
    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
        double screen_gamma = (Data[12] % 256) / 100.0;
        double file_gamma = (Data[13] % 256) / 100.0;
        png_set_gamma(png_ptr, screen_gamma, file_gamma);
        result = 1;
    }
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    
    return 0;
}