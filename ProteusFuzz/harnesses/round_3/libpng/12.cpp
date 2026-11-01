// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_free_data at png.c:480:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_text at pngset.c:912:1 in png.h
// png_get_text at pngget.c:1158:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_data_freer at png.c:461:1 in png.h
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

static void dummy_read(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_write(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush(png_structp png_ptr) {
    (void)png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    png_set_read_fn(png_ptr, NULL, dummy_read);
    png_set_write_fn(png_ptr, NULL, dummy_write, dummy_flush);

    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Test png_set_text
    // We must ensure that the strings pointed to by png_text persist for the duration of the call.
    // Using stack arrays inside a block that goes out of scope before the call causes use-after-scope.
    {
        png_text text[1];
        char key_buf[256];
        char text_buf[256];
        
        // Initialize with safe defaults
        text[0].compression = PNG_TEXT_COMPRESSION_NONE;
        text[0].key = key_buf;
        text[0].text = text_buf;
        text[0].text_length = 0;
        
        // Fill buffers with fuzz data if available
        size_t key_len = (Size > 255) ? 255 : Size;
        size_t text_len = (Size > 255) ? 255 : Size;
        
        if (Size > 0) {
            memcpy(key_buf, Data, key_len);
            key_buf[key_len] = '\0';
            
            // Ensure text is also valid
            memcpy(text_buf, Data, text_len);
            text_buf[text_len] = '\0';
            
            text[0].text_length = strlen(text_buf);
        } else {
            key_buf[0] = '\0';
            text_buf[0] = '\0';
        }

        png_set_text(png_ptr, info_ptr, text, 1);
    }

    // Test png_get_text
    {
        png_textp text_ptr = NULL;
        int num_text = 0;
        int ret = png_get_text(png_ptr, info_ptr, &text_ptr, &num_text);
        (void)ret;
    }

    // Test png_data_freer
    {
        png_data_freer(png_ptr, info_ptr, PNG_DESTROY_WILL_FREE_DATA, PNG_FREE_TEXT);
        png_data_freer(png_ptr, info_ptr, PNG_USER_WILL_FREE_DATA, PNG_FREE_TEXT);
    }

    // Test png_free_data
    {
        png_free_data(png_ptr, info_ptr, PNG_FREE_TEXT, -1);
        png_free_data(png_ptr, info_ptr, PNG_FREE_TEXT, 0);
    }

    // Test png_set_unknown_chunks
    {
        png_unknown_chunk unknown[1];
        memset(unknown[0].name, 'X', 4);
        unknown[0].name[4] = '\0';
        unknown[0].data = NULL;
        unknown[0].size = 0;
        unknown[0].location = PNG_HAVE_IHDR | PNG_HAVE_PLTE;
        
        if (Size > 10) {
            unknown[0].data = (png_byte*)malloc(Size);
            if (unknown[0].data) {
                memcpy(unknown[0].data, Data, Size);
                unknown[0].size = Size;
            }
        }
        
        png_set_unknown_chunks(png_ptr, info_ptr, unknown, 1);
        
        if (unknown[0].data && Size > 10) {
            free(unknown[0].data);
        }
    }

    // Test png_set_invalid
    {
        png_set_invalid(png_ptr, info_ptr, PNG_INFO_IDAT);
        png_set_invalid(png_ptr, info_ptr, PNG_INFO_PLTE);
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}