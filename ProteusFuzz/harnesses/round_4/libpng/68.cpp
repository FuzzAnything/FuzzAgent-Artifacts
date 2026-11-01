// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_image_begin_read_from_file at pngread.c:1455:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_begin_read_from_stdio at pngread.c:1425:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_begin_read_from_memory at pngread.c:1523:5 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_begin_read_from_memory at pngread.c:1523:5 in png.h
// png_image_finish_read at pngread.c:4069:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_set_interlace_handling at pngtrans.c:98:1 in png.h
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
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test 1: png_image_begin_read_from_file
    {
        png_image image;
        memset(&image, 0, sizeof(image));
        image.version = PNG_IMAGE_VERSION;
        
        write_dummy_file(Data, Size);
        png_image_begin_read_from_file(&image, "./dummy_file");
        png_image_free(&image);
    }

    // Test 2: png_image_begin_read_from_stdio
    {
        png_image image;
        memset(&image, 0, sizeof(image));
        image.version = PNG_IMAGE_VERSION;
        
        FILE *f = fopen("./dummy_file", "rb");
        if (f) {
            png_image_begin_read_from_stdio(&image, f);
            fclose(f);
        }
        png_image_free(&image);
    }

    // Test 3: png_image_begin_read_from_memory
    {
        png_image image;
        memset(&image, 0, sizeof(image));
        image.version = PNG_IMAGE_VERSION;
        
        png_image_begin_read_from_memory(&image, Data, Size);
        png_image_free(&image);
    }

    // Test 4: png_image_finish_read (requires successful begin_read)
    {
        png_image image;
        memset(&image, 0, sizeof(image));
        image.version = PNG_IMAGE_VERSION;
        
        if (png_image_begin_read_from_memory(&image, Data, Size)) {
            // Allocate buffer based on image dimensions
            size_t buffer_size = image.width * image.height * 4; // RGBA
            std::vector<uint8_t> buffer(buffer_size);
            png_int_32 row_stride = image.width * 4;
            
            png_image_finish_read(&image, nullptr, buffer.data(), row_stride, nullptr);
        }
        png_image_free(&image);
    }

    // Test 5: png_set_interlace_handling (requires png_struct)
    // Note: png_set_interlace_handling works with png_struct, not png_image directly.
    // We create a minimal setup to test this API.
    {
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (png_ptr) {
            png_infop info_ptr = png_create_info_struct(png_ptr);
            if (info_ptr) {
                // Setup dummy IO
                FILE *f = fopen("./dummy_file", "rb");
                if (f) {
                    png_init_io(png_ptr, f);
                    
                    // Try to read signature
                    if (png_sig_cmp(Data, 0, Size > 8 ? 8 : Size) == 0) {
                        png_set_sig_bytes(png_ptr, Size > 8 ? 8 : Size);
                        
                        // Call target API
                        int passes = png_set_interlace_handling(png_ptr);
                        (void)passes; // Suppress unused warning
                    }
                    fclose(f);
                }
                png_destroy_info_struct(png_ptr, &info_ptr);
            }
            png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        }
    }

    return 0;
}