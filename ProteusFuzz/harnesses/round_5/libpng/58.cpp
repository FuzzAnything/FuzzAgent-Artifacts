// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_oFFs at pngset.c:465:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_get_y_offset_microns at pngget.c:264:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_get_y_offset_inches_fixed at pngget.c:403:1 in png.h
// png_get_pixel_aspect_ratio_fixed at pngget.c:213:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
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
#include <vector>

static void
write_dummy_png(const uint8_t *data, size_t size, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    // Write minimal PNG signature
    unsigned char signature[] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(signature, 1, 8, f);
    
    // Write IHDR chunk
    unsigned char ihdr[13];
    memset(ihdr, 0, sizeof(ihdr));
    ihdr[0] = 1; // Width (1 byte for simplicity, though normally 4)
    ihdr[1] = 0;
    ihdr[2] = 0;
    ihdr[3] = 1; // Height
    ihdr[4] = 8; // Bit depth
    ihdr[5] = 2; // Color type (RGB)
    ihdr[6] = 0; // Compression
    ihdr[7] = 0; // Filter
    ihdr[8] = 0; // Interlace
    
    uint32_t len = 13;
    uint32_t crc = 0; // Simplified, real CRC calculation omitted for fuzzing dummy
    
    fwrite(&len, 4, 1, f);
    fwrite("IHDR", 4, 1, f);
    fwrite(ihdr, 13, 1, f);
    fwrite(&crc, 4, 1, f);
    
    // Write IDAT chunk (empty or minimal)
    len = 0;
    fwrite(&len, 4, 1, f);
    fwrite("IDAT", 4, 1, f);
    fwrite(&crc, 4, 1, f);
    
    // Write IEND chunk
    len = 0;
    fwrite(&len, 4, 1, f);
    fwrite("IEND", 4, 1, f);
    fwrite(&crc, 4, 1, f);
    
    fclose(f);
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    // Create dummy PNG file
    write_dummy_png(Data, Size, "./dummy_file");
    
    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Open file for reading
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    png_init_io(png_ptr, fp);
    
    // Read header
    png_read_info(png_ptr, info_ptr);
    
    // Extract fuzzing data for parameters
    png_int_32 offset_x = 0;
    png_int_32 offset_y = 0;
    int unit_type = 0;
    
    if (Size >= 10) {
        offset_x = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
        offset_y = Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24);
        unit_type = Data[8]; // 0: pixels, 1: meters, 2: micrometers
    }
    
    // Fuzz png_set_oFFs
    png_set_oFFs(png_ptr, info_ptr, offset_x, offset_y, unit_type);
    
    // Fuzz png_get_x_offset_pixels
    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);
    (void)x_offset_pixels;
    
    // Fuzz png_get_y_offset_microns
    png_int_32 y_offset_microns = png_get_y_offset_microns(png_ptr, info_ptr);
    (void)y_offset_microns;
    
    // Fuzz png_get_x_offset_inches_fixed
    png_fixed_point x_offset_inches_fixed = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    (void)x_offset_inches_fixed;
    
    // Fuzz png_get_y_offset_inches_fixed
    png_fixed_point y_offset_inches_fixed = png_get_y_offset_inches_fixed(png_ptr, info_ptr);
    (void)y_offset_inches_fixed;
    
    // Fuzz png_get_pixel_aspect_ratio_fixed
    png_fixed_point aspect_ratio = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
    (void)aspect_ratio;
    
    // Fuzz png_get_pHYs_dpi
    png_uint_32 res_x = 0, res_y = 0;
    int phy_unit_type = 0;
    png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &phy_unit_type);
    (void)res_x;
    (void)res_y;
    (void)phy_unit_type;
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    
    return 0;
}