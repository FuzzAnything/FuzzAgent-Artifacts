// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_image_begin_read_from_stdio at pngread.c:1425:1 in png.h
// png_image_finish_read at pngread.c:4069:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_image_begin_read_from_memory at pngread.c:1523:5 in png.h
// png_image_finish_read at pngread.c:4069:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_begin_read_from_file at pngread.c:1455:1 in png.h
// png_image_finish_read at pngread.c:4069:1 in png.h
// png_image_free at png.c:3989:1 in png.h
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

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Data == nullptr || Size == 0) {
        return 0;
    }

    // Initialize png_image structure
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    // Test png_sig_cmp
    // Ensure we have enough data to check signature
    if (Size >= 8) {
        png_sig_cmp(Data, 0, 8);
        png_sig_cmp(Data, 1, 7);
        png_sig_cmp(Data, 7, 1);
        // Edge cases: start > 7 or num_to_check == 0
        png_sig_cmp(Data, 8, 1);
        png_sig_cmp(Data, 0, 0);
    }

    // Test png_image_begin_read_from_memory
    // Use the fuzzing data as the memory buffer
    png_image_begin_read_from_memory(&image, Data, Size);

    // Prepare a buffer for reading the image
    png_int_32 row_stride = 0;
    if (image.width > 0 && image.height > 0) {
        // Calculate row stride based on format. 
        // For simplicity, assume 3 bytes per pixel (RGB) if not specified, 
        // but png_image_finish_read handles the actual format conversion.
        // The row_stride parameter is the number of bytes per row.
        // We need to ensure it's large enough for the worst case (e.g., RGBA 32-bit).
        // However, the API documentation says row_stride can be 0 for tight packing.
        // Let's try 0 first, then a calculated value if needed.
        // Using 0 is the safest "tight packing" approach.
        row_stride = 0; 
        
        // If we want to provide a specific stride, we need to know the bit depth and channels.
        // Since 'depth' is not a direct member, we rely on the image struct's internal state 
        // or just use 0 to let libpng calculate it.
        
        size_t buffer_size = 0;
        // Calculate a safe upper bound for buffer size if row_stride is 0.
        // Max bytes per pixel is 4 (RGBA 32-bit).
        buffer_size = image.width * image.height * 4;
        
        void *buffer = malloc(buffer_size);
        if (buffer != nullptr) {
            // Test png_image_finish_read
            // Background color is optional, pass nullptr
            png_image_finish_read(&image, nullptr, buffer, row_stride, nullptr);
            free(buffer);
        }
    }

    // Test png_image_free
    png_image_free(&image);

    // Test file-based reading with a dummy file
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp != nullptr) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);

        // Reset image structure for file-based test
        memset(&image, 0, sizeof(image));
        image.version = PNG_IMAGE_VERSION;

        // Test png_image_begin_read_from_file
        png_image_begin_read_from_file(&image, dummy_file);

        // Prepare buffer for file-based read
        if (image.width > 0 && image.height > 0) {
            row_stride = 0; // Let libpng calculate tight packing
            size_t buffer_size = image.width * image.height * 4;
            void *buffer = malloc(buffer_size);
            if (buffer != nullptr) {
                png_image_finish_read(&image, nullptr, buffer, row_stride, nullptr);
                free(buffer);
            }
        }

        png_image_free(&image);

        // Test png_image_begin_read_from_stdio
        fp = fopen(dummy_file, "rb");
        if (fp != nullptr) {
            memset(&image, 0, sizeof(image));
            image.version = PNG_IMAGE_VERSION;

            png_image_begin_read_from_stdio(&image, fp);

            if (image.width > 0 && image.height > 0) {
                row_stride = 0;
                size_t buffer_size = image.width * image.height * 4;
                void *buffer = malloc(buffer_size);
                if (buffer != nullptr) {
                    png_image_finish_read(&image, nullptr, buffer, row_stride, nullptr);
                    free(buffer);
                }
            }

            png_image_free(&image);
            fclose(fp);
        }

        // Clean up dummy file
        remove(dummy_file);
    }

    return 0;
}