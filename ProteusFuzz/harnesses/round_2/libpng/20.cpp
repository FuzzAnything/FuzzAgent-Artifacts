// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_interlace_handling at pngtrans.c:98:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_image_begin_read_from_memory at pngread.c:1523:5 in png.h
// png_image_finish_read at pngread.c:4069:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_begin_read_from_file at pngread.c:1455:1 in png.h
// png_image_finish_read at pngread.c:4069:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_begin_read_from_stdio at pngread.c:1425:1 in png.h
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
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstddef>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f != nullptr) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file for file-based APIs
    write_dummy_file(Data, Size);

    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    // Allocate buffer for reading image data
    // Size is chosen to be reasonably large to avoid allocation failures for typical fuzz inputs
    size_t max_buffer_size = 1024 * 1024; // 1MB limit for safety
    uint8_t *buffer = nullptr;
    if (Size > 0 && Size <= max_buffer_size) {
        buffer = (uint8_t *)malloc(Size);
        if (buffer == nullptr) return 0;
        memcpy(buffer, Data, Size);
    } else if (Size > max_buffer_size) {
        // If input is too large, allocate a fixed size buffer and copy part of data
        buffer = (uint8_t *)malloc(max_buffer_size);
        if (buffer == nullptr) return 0;
        memcpy(buffer, Data, max_buffer_size);
    }

    // 1. Test png_image_begin_read_from_memory
    {
        png_image img_mem;
        memset(&img_mem, 0, sizeof(img_mem));
        img_mem.version = PNG_IMAGE_VERSION;
        png_image_begin_read_from_memory(&img_mem, buffer, Size);
        if (img_mem.opaque != nullptr) {
            // Try to finish read
            png_color background = {0, 0, 0};
            png_image_finish_read(&img_mem, &background, buffer, Size, nullptr);
        }
        png_image_free(&img_mem);
    }

    // 2. Test png_image_begin_read_from_file
    {
        png_image img_file;
        memset(&img_file, 0, sizeof(img_file));
        img_file.version = PNG_IMAGE_VERSION;
        png_image_begin_read_from_file(&img_file, "./dummy_file");
        if (img_file.opaque != nullptr) {
            png_color background = {0, 0, 0};
            png_image_finish_read(&img_file, &background, buffer, Size, nullptr);
        }
        png_image_free(&img_file);
    }

    // 3. Test png_image_begin_read_from_stdio
    {
        FILE *f = fopen("./dummy_file", "rb");
        if (f != nullptr) {
            png_image img_stdio;
            memset(&img_stdio, 0, sizeof(img_stdio));
            img_stdio.version = PNG_IMAGE_VERSION;
            png_image_begin_read_from_stdio(&img_stdio, f);
            if (img_stdio.opaque != nullptr) {
                png_color background = {0, 0, 0};
                png_image_finish_read(&img_stdio, &background, buffer, Size, nullptr);
            }
            png_image_free(&img_stdio);
            fclose(f);
        }
    }

    // 4. Test png_set_interlace_handling with legacy API (requires png_struct)
    // Since we are using png_image API primarily, we can't directly test png_set_interlace_handling
    // without a full png_struct setup which is complex and not directly related to png_image.
    // However, we can attempt to create a png_struct if needed, but the prompt focuses on png_image.
    // We will skip direct testing of png_set_interlace_handling as it requires a different context
    // (png_struct) not directly initialized by png_image functions in the provided scope.
    // If we were to test it, we would need to use png_create_read_struct and png_create_info_struct,
    // then open the file and call png_init_io, etc. This is outside the direct scope of the provided
    // png_image functions but we can try a minimal setup if we want to be thorough.
    // Given the constraints and focus on png_image, we will omit this to avoid complexity
    // unless specifically required. The task asks to fuzz the library using the provided APIs.
    // png_set_interlace_handling is listed, so let's try a minimal setup.
    {
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (png_ptr != nullptr) {
            png_infop info_ptr = png_create_info_struct(png_ptr);
            if (info_ptr != nullptr) {
                FILE *f = fopen("./dummy_file", "rb");
                if (f != nullptr) {
                    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
                        png_init_io(png_ptr, f);
                        int passes = png_set_interlace_handling(png_ptr);
                        (void)passes; // Use the return value to suppress warnings
                    }
                    fclose(f);
                }
                png_destroy_info_struct(png_ptr, &info_ptr);
            }
            png_destroy_read_struct(&png_ptr, NULL, NULL);
        }
    }

    // Cleanup
    if (buffer != nullptr) {
        free(buffer);
    }

    // Cleanup dummy file
    remove("./dummy_file");

    return 0;
}