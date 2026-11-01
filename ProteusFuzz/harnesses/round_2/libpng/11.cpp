// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_set_gamma at pngrtran.c:901:1 in png.h
// png_set_gamma_fixed at pngrtran.c:859:1 in png.h
// png_set_rgb_to_gray at pngrtran.c:1085:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_set_alpha_mode at pngrtran.c:444:1 in png.h
// png_set_alpha_mode_fixed at pngrtran.c:344:1 in png.h
// png_set_tRNS_to_alpha at pngrtran.c:969:1 in png.h
// png_set_background at pngrtran.c:155:1 in png.h
// png_set_background_fixed at pngrtran.c:125:1 in png.h
// png_set_scale_16 at pngrtran.c:171:1 in png.h
// png_set_flush at pngwrite.c:966:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <png.h>
#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <setjmp.h>

static void write_dummy_png(const char* filename) {
    FILE* f = fopen(filename, "wb");
    if (!f) return;
    
    // Write a minimal valid PNG file header
    unsigned char signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(signature, 1, 8, f);
    
    // IHDR chunk (13 bytes)
    unsigned char ihdr[] = {
        0x00, 0x00, 0x00, 0x0D, // Length: 13
        0x49, 0x48, 0x44, 0x52, // Type: IHDR
        0x00, 0x00, 0x00, 0x01, // Width: 1
        0x00, 0x00, 0x00, 0x01, // Height: 1
        0x08,                   // Bit depth: 8
        0x02,                   // Color type: RGB
        0x00,                   // Compression: 0
        0x00,                   // Filter: 0
        0x00,                   // Interlace: 0
        0x90, 0x77, 0x53, 0xDE  // CRC (dummy)
    };
    fwrite(ihdr, 1, 19, f);
    
    // IDAT chunk (minimal compressed data)
    unsigned char idat[] = {
        0x00, 0x00, 0x00, 0x0C, // Length: 12
        0x49, 0x44, 0x41, 0x54, // Type: IDAT
        0x78, 0x9C, 0x62, 0x60, // Compressed data (dummy)
        0x00, 0x00, 0x00, 0x04,
        0x00, 0x01, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, // CRC (dummy)
    };
    fwrite(idat, 1, 20, f);
    
    // IEND chunk
    unsigned char iend[] = {
        0x00, 0x00, 0x00, 0x00, // Length: 0
        0x49, 0x45, 0x4E, 0x44, // Type: IEND
        0xAE, 0x42, 0x60, 0x82  // CRC (dummy)
    };
    fwrite(iend, 1, 12, f);
    
    fclose(f);
}

static png_structp create_png_struct(const uint8_t* data, size_t size) {
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
    
    // Create a file pointer from the data
    FILE* fp = fmemopen((void*)data, size, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return NULL;
    }
    
    png_init_io(png_ptr, fp);
    
    // Read the header
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    // Re-create for further testing
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return NULL;
    
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return NULL;
    }
    
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return NULL;
    }
    
    fp = fmemopen((void*)data, size, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return NULL;
    }
    
    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);
    
    return png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0; // Minimal size check
    
    // Create a dummy PNG file for file-based operations
    write_dummy_png("./dummy_file");
    
    // Test with in-memory data
    png_structp png_ptr = create_png_struct(Data, Size);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = NULL;
    
    // Test png_set_swap_alpha
    png_set_swap_alpha(png_ptr);
    
    // Test png_set_gamma
    double screen_gamma = 2.2;
    double file_gamma = 0.45;
    png_set_gamma(png_ptr, screen_gamma, file_gamma);
    
    // Test png_set_gamma_fixed
    png_fixed_point screen_gamma_fixed = 220000;
    png_fixed_point file_gamma_fixed = 45455;
    png_set_gamma_fixed(png_ptr, screen_gamma_fixed, file_gamma_fixed);
    
    // Test png_set_rgb_to_gray
    png_set_rgb_to_gray(png_ptr, 0, 0.2126, 0.7152);
    png_set_rgb_to_gray_fixed(png_ptr, 1, 21260, 71520);
    png_byte status = png_get_rgb_to_gray_status(png_ptr);
    
    // Test png_set_alpha_mode
    png_set_alpha_mode(png_ptr, PNG_ALPHA_PNG, PNG_DEFAULT_sRGB);
    png_set_alpha_mode_fixed(png_ptr, PNG_ALPHA_PNG, PNG_GAMMA_sRGB);
    
    // Test png_set_tRNS_to_alpha
    png_set_tRNS_to_alpha(png_ptr);
    
    // Test png_set_background
    // Fixed: png_color_16 has 5 members: index, red, green, blue, gray
    png_color_16 background_color;
    background_color.index = 0;
    background_color.red = 255;
    background_color.green = 255;
    background_color.blue = 255;
    background_color.gray = 0;
    
    png_set_background(png_ptr, &background_color, PNG_BACKGROUND_GAMMA_SCREEN, 0, 1.0);
    png_set_background_fixed(png_ptr, &background_color, PNG_BACKGROUND_GAMMA_FILE, 1, 100000);
    
    // Test png_set_scale_16
    png_set_scale_16(png_ptr);
    
    // Test png_set_flush
    png_set_flush(png_ptr, 10);
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    return 0;
}