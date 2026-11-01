// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_x_offset_microns at pngget.c:244:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_get_y_offset_pixels at pngget.c:304:1 in png.h
// png_set_oFFs at pngset.c:465:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_get_y_offset_pixels at pngget.c:304:1 in png.h
// png_get_x_offset_microns at pngget.c:244:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_get_pixel_aspect_ratio_fixed at pngget.c:213:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_pixel_aspect_ratio_fixed at pngget.c:213:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
#include <png.h>
#include <cstdio>
#include <setjmp.h>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <vector>

static void write_dummy_png(const uint8_t *data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) return;

    // PNG Signature (8 bytes)
    const uint8_t png_sig[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
    file.write(reinterpret_cast<const char*>(png_sig), 8);

    // IHDR chunk
    // Length: 13 bytes
    uint32_t length = 13;
    file.write(reinterpret_cast<const char*>(&length), 4);
    
    // Type: "IHDR"
    const char type[] = {'I', 'H', 'D', 'R'};
    file.write(type, 4);

    // IHDR Data: Width=1, Height=1, BitDepth=8, ColorType=2 (RGB), Compression=0, Filter=0, Interlace=0
    uint8_t ihdr_data[13];
    memset(ihdr_data, 0, 13);
    // Width (4 bytes)
    ihdr_data[0] = 0; ihdr_data[1] = 0; ihdr_data[2] = 0; ihdr_data[3] = 1;
    // Height (4 bytes)
    ihdr_data[4] = 0; ihdr_data[5] = 0; ihdr_data[6] = 0; ihdr_data[7] = 1;
    // Bit depth
    ihdr_data[8] = 8;
    // Color type
    ihdr_data[9] = 2;
    // Compression, Filter, Interlace are already 0

    file.write(reinterpret_cast<const char*>(ihdr_data), 13);

    // Dummy CRC (4 bytes)
    uint32_t crc = 0;
    file.write(reinterpret_cast<const char*>(&crc), 4);

    // Write input data as raw image data
    if (size > 0) {
        file.write(reinterpret_cast<const char*>(data), size);
    }

    file.close();
}

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    write_dummy_png(Data, Size);

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    png_read_info(png_ptr, info_ptr);

    // Target API 1: png_get_pixel_aspect_ratio_fixed
    png_fixed_point aspect_ratio = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
    (void)aspect_ratio;

    // Target API 2: png_get_x_offset_pixels
    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);
    (void)x_offset_pixels;

    // Target API 3: png_get_x_offset_microns
    png_int_32 x_offset_microns = png_get_x_offset_microns(png_ptr, info_ptr);
    (void)x_offset_microns;

    // Target API 4: png_get_x_offset_inches_fixed
    png_fixed_point x_offset_inches_fixed = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    (void)x_offset_inches_fixed;

    // Target API 5: png_get_y_offset_pixels
    png_int_32 y_offset_pixels = png_get_y_offset_pixels(png_ptr, info_ptr);
    (void)y_offset_pixels;

    // Target API 6: png_set_oFFs
    png_int_32 offset_x = 0;
    png_int_32 offset_y = 0;
    int unit_type = 0;

    if (Size >= 4) {
        offset_x = (png_int_32)(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
    }
    if (Size >= 8) {
        offset_y = (png_int_32)(Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24));
    }
    if (Size >= 9) {
        unit_type = Data[8] % 3; // 0: pixels, 1: microns, 2: undefined
    }

    png_set_oFFs(png_ptr, info_ptr, offset_x, offset_y, unit_type);

    // Re-check values after setting
    (void)png_get_x_offset_pixels(png_ptr, info_ptr);
    (void)png_get_y_offset_pixels(png_ptr, info_ptr);
    (void)png_get_x_offset_microns(png_ptr, info_ptr);
    (void)png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    (void)png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);

    fclose(fp);
    cleanup_png(png_ptr, info_ptr);

    return 0;
}