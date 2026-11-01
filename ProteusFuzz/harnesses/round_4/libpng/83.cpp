// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_mDCV at pngset.c:297:1 in png.h
// png_set_sCAL at pngset.c:664:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_gAMA at pngset.c:365:1 in png.h
// png_set_cLLI at pngset.c:194:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void safe_png_error(png_structp png_ptr, png_const_charp error_msg) {
    // Silently ignore errors for fuzzing to prevent crashes
}

static void safe_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    // Silently ignore warnings
}

// Custom write function that discards data to avoid file I/O issues in fuzzing
static void custom_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing, effectively discarding the data
    (void)png_ptr;
    (void)data;
    (void)length;
}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, safe_png_error, safe_png_warning);
    if (!png_ptr) return NULL;
    return png_ptr;
}

static png_infop create_png_info(png_structp png_ptr) {
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return NULL;
    }
    return info_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Create dummy file if needed (though we use memory here)
    write_dummy_file(Data, Size);

    png_structp png_ptr = create_png_struct();
    if (!png_ptr) return 0;

    png_infop info_ptr = create_png_info(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    // Initialize PNG write struct with custom write function to avoid file I/O
    png_set_write_fn(png_ptr, NULL, custom_write_data, NULL);

    // Set minimal image parameters to allow write operations
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Fuzz png_set_gAMA
    if (Size > 0) {
        double gamma = static_cast<int8_t>(Data[0]) / 100.0;
        if (gamma > 0 && gamma < 5.0) {
            png_set_gAMA(png_ptr, info_ptr, gamma);
        }
    }

    // Fuzz png_set_cLLI
    if (Size > 2) {
        double max_content = static_cast<uint16_t>(Data[1] << 8 | Data[2]) / 1000.0;
        double max_frame = static_cast<uint16_t>(Data[2] << 8 | Data[1]) / 1000.0;
        if (max_content >= 0 && max_frame >= 0) {
            png_set_cLLI(png_ptr, info_ptr, max_content, max_frame);
        }
    }

    // Fuzz png_set_mDCV
    if (Size > 10) {
        double wx = Data[3] / 255.0;
        double wy = Data[4] / 255.0;
        double rx = Data[5] / 255.0;
        double ry = Data[6] / 255.0;
        double gx = Data[7] / 255.0;
        double gy = Data[8] / 255.0;
        double bx = Data[9] / 255.0;
        double by = Data[10] / 255.0;
        double max_lum = static_cast<uint32_t>(Data[11] << 24 | Data[12] << 16 | Data[13] << 8 | Data[14]) / 10000.0;
        double min_lum = static_cast<uint32_t>(Data[15] << 24 | Data[16] << 16 | Data[17] << 8 | Data[18]) / 10000.0;
        
        // Ensure valid ranges for chromaticities (0 to 1)
        if (wx >= 0 && wx <= 1 && wy >= 0 && wy <= 1 &&
            rx >= 0 && rx <= 1 && ry >= 0 && ry <= 1 &&
            gx >= 0 && gx <= 1 && gy >= 0 && gy <= 1 &&
            bx >= 0 && bx <= 1 && by >= 0 && by <= 1 &&
            max_lum >= 0 && min_lum >= 0 && min_lum <= max_lum) {
            png_set_mDCV(png_ptr, info_ptr, wx, wy, rx, ry, gx, gy, bx, by, max_lum, min_lum);
        }
    }

    // Fuzz png_set_sCAL
    if (Size > 20) {
        int unit = Data[19] % 2; // 0 or 1
        double width = static_cast<uint32_t>(Data[20] << 24 | Data[21] << 16 | Data[22] << 8 | Data[23]) / 10000.0;
        double height = static_cast<uint32_t>(Data[24] << 24 | Data[25] << 16 | Data[26] << 8 | Data[27]) / 10000.0;
        if (width > 0 && height > 0) {
            png_set_sCAL(png_ptr, info_ptr, unit, width, height);
        }
    }

    // Call png_write_info
    png_write_info(png_ptr, info_ptr);

    // Write minimal image data (1x1 RGB pixel)
    png_byte row[3] = {0, 0, 0};
    png_write_row(png_ptr, row);

    // Call png_write_end
    png_write_end(png_ptr, info_ptr);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}