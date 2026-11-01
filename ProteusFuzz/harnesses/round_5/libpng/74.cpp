// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_init_io at png.c:705:1 in png.h
// png_set_compression_level at pngwrite.c:1185:1 in png.h
// png_set_compression_window_bits at pngwrite.c:1224:1 in png.h
// png_set_filter at pngwrite.c:1058:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_image_write_to_stdio at pngwrite.c:2353:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_access_version_number at png.c:963:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (data && size > 0) {
            fwrite(data, 1, size, f);
        }
        fclose(f);
    }
}

static void cleanup_png_struct(png_structp *png_ptr, png_infop *info_ptr) {
    if (png_ptr && *png_ptr) {
        png_destroy_write_struct(png_ptr, info_ptr);
        *png_ptr = nullptr;
    }
    if (info_ptr && *info_ptr) {
        png_destroy_info_struct(nullptr, info_ptr);
        *info_ptr = nullptr;
    }
}

extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    // Test 1: png_access_version_number (constant behavior, no setup needed)
    png_uint_32 version = png_access_version_number();
    (void)version; // Use the value to avoid unused variable warning

    // If no data, just return after testing the constant function
    if (Size == 0) {
        return 0;
    }

    // Test 2, 3, 4, 5, 6: Complex workflow involving png_create_write_struct, 
    // compression settings, filter settings, and writing.
    
    // We need a buffer for pixel data. If input is too small, create a minimal one.
    size_t min_data_size = 100;
    const uint8_t *pixel_data = Data;
    size_t pixel_size = Size;
    
    // Create a temporary buffer if input is too small to act as a valid image buffer
    std::vector<uint8_t> temp_buffer;
    if (Size < min_data_size) {
        temp_buffer.resize(min_data_size);
        // Fill with dummy pattern
        for (size_t i = 0; i < min_data_size; ++i) {
            temp_buffer[i] = static_cast<uint8_t>(i % 256);
        }
        pixel_data = temp_buffer.data();
        pixel_size = min_data_size;
    }

    // Create a dummy file for writing
    write_dummy_file(Data, Size);
    FILE *file = fopen("./dummy_file", "wb");
    if (!file) {
        return 0;
    }

    // Initialize PNG structures
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // 6. png_create_write_struct
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(file);
        return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        cleanup_png_struct(&png_ptr, &info_ptr);
        fclose(file);
        return 0;
    }

    // Setup error handling to prevent crashes during fuzzing
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png_struct(&png_ptr, &info_ptr);
        fclose(file);
        return 0;
    }

    // Initialize the file writing
    png_init_io(png_ptr, file);

    // Extract parameters from fuzz data for diverse testing
    int level = (Size > 0) ? (Data[0] % 10) : 6;
    int window_bits = (Size > 1) ? (8 + (Data[1] % 8)) : 12; // Range 8-15
    int method = (Size > 2) ? (Data[2] % 1) : 0; // Only 0 is valid
    int filters = (Size > 3) ? (Data[3] % 32) : 1; // Bitmask

    // 3. png_set_compression_level
    png_set_compression_level(png_ptr, level);

    // 4. png_set_compression_window_bits
    png_set_compression_window_bits(png_ptr, window_bits);

    // 5. png_set_filter
    png_set_filter(png_ptr, method, filters);

    // Set up a basic image info for writing (1x1 pixel, 8-bit RGB)
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    
    // Prepare row data
    png_byte row[3]; // RGB
    row[0] = (Size > 4) ? Data[4] : 255;
    row[1] = (Size > 5) ? Data[5] : 0;
    row[2] = (Size > 6) ? Data[6] : 0;

    // Write the info
    png_write_info(png_ptr, info_ptr);

    // Write the row
    png_write_row(png_ptr, row);

    // End the image
    png_write_end(png_ptr, info_ptr);

    // Cleanup
    cleanup_png_struct(&png_ptr, &info_ptr);
    fclose(file);

    // Test 2: png_image_write_to_stdio (Simplified API)
    // We reuse the dummy file or create a new one.
    // Note: png_image_write_to_stdio requires a valid png_image struct.
    // Constructing a valid png_image struct from raw fuzz data is complex and error-prone
    // without a full image decoder. We will attempt to initialize a basic one.
    
    png_image image = {0};
    image.version = PNG_IMAGE_VERSION;
    image.width = 1;
    image.height = 1;
    image.format = PNG_FORMAT_RGB;
    // PNG_IMAGE_FLAG_NONE might not be defined in all versions, use 0 directly
    image.flags = 0; 
    
    // Write to a new dummy file to avoid conflicts
    FILE *file2 = fopen("./dummy_file2", "wb");
    if (file2) {
        // Use the same row data as buffer
        int result = png_image_write_to_stdio(&image, file2, 0, row, 3, nullptr);
        (void)result; // Result indicates success/failure, we don't assert for fuzzing
        fclose(file2);
    }

    return 0;
}