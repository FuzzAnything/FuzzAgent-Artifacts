// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_eXIf_1 at pngset.c:319:1 in png.h
// png_set_cICP at pngset.c:136:1 in png.h
// png_set_rows at pngset.c:1845:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_image at pngwrite.c:659:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
// Fuzz driver for libpng write functions
#include <png.h>
#include <cstdint>
#include <cstring>
#include <vector>
#include <fstream>
#include <iostream>

// Custom error handler to prevent crashes during fuzzing
static void
fuzz_png_error(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
    // Silently handle errors to allow fuzzing to continue
}

static void
fuzz_png_warning(png_structp png_ptr, png_const_charp warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

// Helper to safely initialize libpng structures
static bool
init_png_write(png_structpp png_ptr, png_infopp info_ptr, const uint8_t* data, size_t size) {
    if (size < 4) return false;

    // Create dummy file
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) return false;
    file.write(reinterpret_cast<const char*>(data), size);
    file.close();

    *png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, fuzz_png_error, fuzz_png_warning);
    if (!*png_ptr) return false;

    *info_ptr = png_create_info_struct(*png_ptr);
    if (!*info_ptr) {
        png_destroy_write_struct(png_ptr, nullptr);
        return false;
    }

    // Set up file I/O
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) {
        png_destroy_write_struct(png_ptr, info_ptr);
        return false;
    }
    png_init_io(*png_ptr, fp);

    // Set basic image info (required for writing)
    png_set_IHDR(*png_ptr, *info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    return true;
}

// Helper to cleanup libpng structures
static void
cleanup_png(png_structpp png_ptr, png_infopp info_ptr) {
    if (*png_ptr) {
        png_destroy_write_struct(png_ptr, info_ptr);
    } else if (*info_ptr) {
        png_destroy_info_struct(nullptr, info_ptr);
    }
    // Close file if open (libpng handles this, but ensure cleanup)
    remove("./dummy_file");
}

// Helper to create row data
static std::vector<png_byte*>
create_row_data(png_structp png_ptr, png_infop info_ptr, size_t height, size_t width, const uint8_t* data, size_t size) {
    std::vector<png_byte*> rows;
    if (!png_ptr || !info_ptr) return rows;

    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    if (row_bytes == 0) return rows;

    rows.resize(height);
    for (size_t i = 0; i < height; ++i) {
        rows[i] = new png_byte[row_bytes];
        // Fill with pattern from fuzz data
        for (size_t j = 0; j < row_bytes; ++j) {
            rows[i][j] = data ? data[(i * row_bytes + j) % size] : 0;
        }
    }
    return rows;
}

extern "C" int
LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Initialize libpng
    if (!init_png_write(&png_ptr, &info_ptr, Data, Size)) {
        return 0;
    }

    // Try to set EXIF data (if data is large enough)
    if (Size > 10) {
        png_set_eXIf_1(png_ptr, info_ptr, Size - 10, const_cast<png_byte*>(Data + 10));
    }

    // Try to set cICP data
    png_set_cICP(png_ptr, info_ptr, 
                 Data[0] % 256, 
                 Data[1] % 256, 
                 Data[2] % 256, 
                 Data[3] % 256);

    // Create row data
    std::vector<png_byte*> rows = create_row_data(png_ptr, info_ptr, 1, 1, Data, Size);
    if (rows.empty()) {
        cleanup_png(&png_ptr, &info_ptr);
        return 0;
    }

    // Set rows
    png_set_rows(png_ptr, info_ptr, rows.data());

    // Write info
    png_write_info(png_ptr, info_ptr);

    // Write image
    png_write_image(png_ptr, rows.data());

    // Write end
    png_write_end(png_ptr, info_ptr);

    // Cleanup
    for (auto row : rows) {
        delete[] row;
    }
    cleanup_png(&png_ptr, &info_ptr);

    return 0;
}