// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_gamma_fixed at pngrtran.c:859:1 in png.h
// png_set_background at pngrtran.c:155:1 in png.h
// png_set_background_fixed at pngrtran.c:125:1 in png.h
// png_set_tRNS_to_alpha at pngrtran.c:969:1 in png.h
// png_set_scale_16 at pngrtran.c:171:1 in png.h
// png_set_flush at pngwrite.c:966:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_set_invert_alpha at pngtrans.c:236:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_set_rgb_to_gray at pngrtran.c:1085:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_set_alpha_mode at pngrtran.c:444:1 in png.h
// png_set_alpha_mode_fixed at pngrtran.c:344:1 in png.h
// png_set_gamma at pngrtran.c:901:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <setjmp.h>

// Global state for the read callback to track offset
static size_t g_read_offset = 0;
static const uint8_t* g_read_data = nullptr;
static size_t g_read_size = 0;

static void read_png_data(png_structp png_ptr, png_bytep outBuffer, png_size_t length) {
    if (g_read_offset + length > g_read_size) {
        png_error(png_ptr, "Not enough data");
    }
    memcpy(outBuffer, g_read_data + g_read_offset, length);
    g_read_offset += length;
}

static void safe_png_destroy(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    } else if (info_ptr) {
        png_destroy_read_struct(nullptr, &info_ptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size < 18) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Initialize global read state
    g_read_data = Data;
    g_read_size = Size;
    g_read_offset = 0;

    // Create PNG struct
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        safe_png_destroy(png_ptr, info_ptr);
        return 0;
    }

    // Set read function
    png_set_read_fn(png_ptr, (png_voidp)Data, (png_rw_ptr)read_png_data);

    // Try to read the header
    png_uint_32 width, height;
    int bit_depth, color_type;
    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, nullptr, nullptr, nullptr);

    // 1. Test png_set_invert_alpha
    png_set_invert_alpha(png_ptr);

    // 2. Test png_set_rgb_to_gray
    png_fixed_point red_fixed = (png_fixed_point)(Data[0] * 256);
    png_fixed_point green_fixed = (png_fixed_point)(Data[1] * 256);
    int error_action = (Data[2] & 1) ? 1 : 0;
    png_set_rgb_to_gray_fixed(png_ptr, error_action, red_fixed, green_fixed);

    double red_double = (double)Data[3] / 255.0;
    double green_double = (double)Data[4] / 255.0;
    png_set_rgb_to_gray(png_ptr, error_action, red_double, green_double);

    png_byte status = png_get_rgb_to_gray_status(png_ptr);
    (void)status;

    // 3. Test png_set_alpha_mode
    int mode = (Data[5] % 4);
    double output_gamma = (double)Data[6] / 255.0;
    if (Data[6] == 0xFF) output_gamma = PNG_DEFAULT_sRGB;
    else if (Data[6] == 0xFE) output_gamma = PNG_GAMMA_MAC_18;
    else if (Data[6] == 0xFD) output_gamma = PNG_GAMMA_LINEAR;
    
    png_set_alpha_mode(png_ptr, mode, output_gamma);

    png_fixed_point output_gamma_fixed = (png_fixed_point)(output_gamma * 100000);
    png_set_alpha_mode_fixed(png_ptr, mode, output_gamma_fixed);

    // 4. Test png_set_gamma
    double screen_gamma = (double)Data[7] / 255.0;
    double file_gamma = (double)Data[8] / 255.0;
    if (Data[7] == 0xFF) screen_gamma = PNG_DEFAULT_sRGB;
    if (Data[8] == 0xFF) file_gamma = PNG_DEFAULT_sRGB;
    
    png_set_gamma(png_ptr, screen_gamma, file_gamma);

    png_fixed_point screen_gamma_fixed = (png_fixed_point)(screen_gamma * 100000);
    png_fixed_point file_gamma_fixed = (png_fixed_point)(file_gamma * 100000);
    png_set_gamma_fixed(png_ptr, screen_gamma_fixed, file_gamma_fixed);

    // 5. Test png_set_background
    png_color_16 background_color;
    background_color.red = Data[9];
    background_color.green = Data[10];
    background_color.blue = Data[11];
    background_color.gray = Data[12];
    background_color.index = Data[13];
    
    int background_gamma_code = Data[14] % 4;
    int need_expand = (Data[15] & 1) ? 1 : 0;
    double background_gamma = (double)Data[16] / 255.0;
    if (Data[16] == 0xFF) background_gamma = PNG_DEFAULT_sRGB;

    png_set_background(png_ptr, &background_color, background_gamma_code, need_expand, background_gamma);

    png_fixed_point background_gamma_fixed = (png_fixed_point)(background_gamma * 100000);
    png_set_background_fixed(png_ptr, &background_color, background_gamma_code, need_expand, background_gamma_fixed);

    // 6. Test png_set_tRNS_to_alpha
    png_set_tRNS_to_alpha(png_ptr);

    // Additional: png_set_scale_16
    png_set_scale_16(png_ptr);

    // Additional: png_set_flush
    int nrows = Data[17] % 100;
    png_set_flush(png_ptr, nrows);

    // Allocate dummy row pointers
    png_bytep *row_pointers = (png_bytep *)png_malloc(png_ptr, height * sizeof(png_bytep));
    if (!row_pointers) {
        safe_png_destroy(png_ptr, info_ptr);
        return 0;
    }

    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
        if (!row_pointers[y]) {
            // Cleanup partial allocation
            for (png_uint_32 k = 0; k < y; k++) {
                png_free(png_ptr, row_pointers[k]);
            }
            png_free(png_ptr, row_pointers);
            safe_png_destroy(png_ptr, info_ptr);
            return 0;
        }
    }

    png_read_image(png_ptr, row_pointers);

    // Clean up row pointers
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);

    png_read_end(png_ptr, nullptr);

    safe_png_destroy(png_ptr, info_ptr);
    return 0;
}