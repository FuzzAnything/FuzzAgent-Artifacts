// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_eXIf_1 at pngset.c:319:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_set_rows at pngset.c:1845:1 in png.h
// png_write_image at pngwrite.c:659:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_cICP at pngset.c:136:1 in png.h
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
#include <vector>
#include <fstream>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        if (Size > 0) {
            file.write(reinterpret_cast<const char*>(Data), Size);
        }
        file.close();
    }
}

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file to write to
    write_dummy_file(Data, Size);

    // Initialize PNG structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Set up error handling to avoid crashes during fuzzing
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Open the dummy file for writing
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Set basic image parameters with bounds checking
    png_uint_32 width = 1;
    png_uint_32 height = 1;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;

    // Use fuzzed data to vary parameters slightly within safe bounds
    if (Size > 0) {
        width = (Data[0] % 100) + 1;
        if (Size > 1) {
            height = (Data[1] % 100) + 1;
        }
        if (Size > 2) {
            bit_depth = (Data[2] % 8) + 1;
        }
        if (Size > 3) {
            color_type = (Data[3] % 6); // Valid color types: 0, 2, 3, 4, 6
        }
    }

    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Test png_set_cICP
    png_byte colour_primaries = 1;
    png_byte transfer_function = 2;
    png_byte matrix_coefficients = 0;
    png_byte video_full_range_flag = 0;
    
    if (Size > 4) colour_primaries = Data[4];
    if (Size > 5) transfer_function = Data[5];
    if (Size > 6) matrix_coefficients = Data[6];
    if (Size > 7) video_full_range_flag = Data[7];
    
    png_set_cICP(png_ptr, info_ptr, colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag);

    // Test png_set_eXIf_1
    if (Size > 8) {
        png_uint_32 num_exif = (Data[8] % 100) + 1;
        std::vector<png_byte> exif_data(num_exif);
        size_t available_data = (Size > 9) ? (Size - 9) : 0;
        size_t copy_size = (num_exif < available_data) ? num_exif : available_data;
        
        for (png_uint_32 i = 0; i < copy_size; ++i) {
            exif_data[i] = Data[9 + i];
        }
        // Fill remaining with 0 if needed
        for (png_uint_32 i = copy_size; i < num_exif; ++i) {
            exif_data[i] = 0;
        }
        png_set_eXIf_1(png_ptr, info_ptr, num_exif, exif_data.data());
    }

    // Test png_write_info_before_PLTE
    png_write_info_before_PLTE(png_ptr, info_ptr);

    // Prepare row pointers for image data
    std::vector<png_byte*> row_pointers(height);
    for (png_uint_32 y = 0; y < height; ++y) {
        size_t row_size = png_get_rowbytes(png_ptr, info_ptr);
        row_pointers[y] = (png_byte*)malloc(row_size);
        if (row_pointers[y]) {
            // Fill with fuzzed data or default
            if (Size > 10) {
                size_t data_to_copy = std::min(row_size, Size - 10);
                std::memcpy(row_pointers[y], Data + 10, data_to_copy);
                if (data_to_copy < row_size) {
                    std::memset(row_pointers[y] + data_to_copy, 0, row_size - data_to_copy);
                }
            } else {
                std::memset(row_pointers[y], 0, row_size);
            }
        }
    }

    // Test png_set_rows
    png_set_rows(png_ptr, info_ptr, row_pointers.data());

    // Test png_write_image
    png_write_image(png_ptr, row_pointers.data());

    // Test png_write_end
    png_write_end(png_ptr, info_ptr);

    // Cleanup
    fclose(fp);
    for (png_uint_32 y = 0; y < height; ++y) {
        free(row_pointers[y]);
    }
    cleanup_png(png_ptr, info_ptr);

    return 0;
}