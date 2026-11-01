// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_text at pngset.c:912:1 in png.h
// png_get_text at pngget.c:1158:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_png_set_invalid(png_structp png_ptr, png_infop info_ptr, const uint8_t *data, size_t size) {
    if (size < 1) return;
    int mask = data[0];
    png_set_invalid(png_ptr, info_ptr, mask);
}

static void fuzz_png_data_freer(png_structp png_ptr, png_infop info_ptr, const uint8_t *data, size_t size) {
    if (size < 2) return;
    int freer = data[0];
    png_uint_32 mask = data[1];
    png_data_freer(png_ptr, info_ptr, freer, mask);
}

static void fuzz_png_free_data(png_structp png_ptr, png_infop info_ptr, const uint8_t *data, size_t size) {
    if (size < 2) return;
    png_uint_32 free_me = data[0];
    int num = data[1];
    png_free_data(png_ptr, info_ptr, free_me, num);
}

static void fuzz_png_set_unknown_chunks(png_structp png_ptr, png_infop info_ptr, const uint8_t *data, size_t size) {
    if (size < 1) return;
    int num_unknowns = data[0];
    if (num_unknowns == 0) return;
    
    // Each chunk needs at least 5 bytes for name + 1 byte for size + 1 byte for location = 7 bytes minimum per chunk
    // We use a simple stride of 10 to be safe and allow some data
    size_t required_size = 1 + num_unknowns * 10;
    if (size < required_size) return;
    
    png_unknown_chunk *unknowns = (png_unknown_chunk *)malloc(sizeof(png_unknown_chunk) * num_unknowns);
    if (!unknowns) return;
    
    // Initialize array to ensure cleanup happens even if we return early
    for (int i = 0; i < num_unknowns; i++) {
        unknowns[i].data = NULL;
    }

    for (int i = 0; i < num_unknowns; i++) {
        memset(unknowns[i].name, 0, 5);
        // Copy 5 bytes for name
        memcpy(unknowns[i].name, data + 1 + i * 10, 5);
        
        // Use next byte for size (limit to reasonable value)
        unknowns[i].size = (data[1 + i * 10 + 5] % 100);
        
        // Allocate data if size > 0
        if (unknowns[i].size > 0) {
            unknowns[i].data = (png_byte *)malloc(unknowns[i].size);
            if (unknowns[i].data) {
                // Fill with zeros or pattern, don't read from input for data content to avoid OOB
                memset(unknowns[i].data, 0, unknowns[i].size);
            }
        }
        
        // Use next byte for location
        unknowns[i].location = data[1 + i * 10 + 6];
    }
    
    png_set_unknown_chunks(png_ptr, info_ptr, unknowns, num_unknowns);
    
    // Cleanup: libpng does not free the 'unknowns' array or the internal 'data' pointers
    for (int i = 0; i < num_unknowns; i++) {
        if (unknowns[i].data) {
            free(unknowns[i].data);
        }
    }
    free(unknowns);
}

static void fuzz_png_set_text(png_structp png_ptr, png_infop info_ptr, const uint8_t *data, size_t size) {
    if (size < 1) return;
    int num_text = data[0];
    if (num_text == 0) return;
    
    // Each text entry needs: 1 (compression) + 1 (key_len) + key_len + 1 (text_len) + text_len
    // To simplify, we enforce a fixed stride of 20 bytes per entry to avoid complex bounds checks
    size_t required_size = 1 + num_text * 20;
    if (size < required_size) return;
    
    png_text *text_ptr = (png_text *)malloc(sizeof(png_text) * num_text);
    if (!text_ptr) return;
    
    // Initialize pointers for safe cleanup
    for (int i = 0; i < num_text; i++) {
        text_ptr[i].key = NULL;
        text_ptr[i].text = NULL;
    }

    for (int i = 0; i < num_text; i++) {
        size_t offset = 1 + i * 20;
        
        // Compression
        text_ptr[i].compression = (data[offset] % 4);
        
        // Key length
        int key_len = (data[offset + 1] % 10); // Limit key length to 10
        
        if (key_len > 0) {
            text_ptr[i].key = (char *)malloc(key_len + 1);
            if (text_ptr[i].key) {
                // Copy key data, ensure we don't read past buffer
                if (size >= offset + 2 + key_len) {
                    memcpy(text_ptr[i].key, data + offset + 2, key_len);
                    text_ptr[i].key[key_len] = '\0';
                } else {
                    text_ptr[i].key[0] = '\0';
                }
            }
        }
        
        // Text length
        size_t text_len = (data[offset + 2 + key_len] % 10); // Limit text length to 10
        
        // Ensure text field is valid (non-NULL)
        text_ptr[i].text = (char *)malloc(text_len + 1);
        if (text_ptr[i].text) {
            if (text_len > 0) {
                // Check bounds for text data
                if (size >= offset + 3 + key_len + text_len) {
                    memcpy(text_ptr[i].text, data + offset + 3 + key_len, text_len);
                    text_ptr[i].text[text_len] = '\0';
                } else {
                    text_ptr[i].text[0] = '\0';
                }
            } else {
                text_ptr[i].text[0] = '\0';
            }
        } else {
            // Fallback to empty string literal if malloc fails
            text_ptr[i].text = (char *)"";
        }
        
        text_ptr[i].text_length = text_len;
        
        #ifdef PNG_READ_iTXt_SUPPORTED
        text_ptr[i].itxt_length = 0;
        text_ptr[i].lang = NULL;
        text_ptr[i].lang_key = NULL;
        #endif
    }
    
    png_set_text(png_ptr, info_ptr, text_ptr, num_text);
    
    for (int i = 0; i < num_text; i++) {
        if (text_ptr[i].key) {
            free(text_ptr[i].key);
        }
        // Only free text if it's not the static empty string literal
        if (text_ptr[i].text && text_ptr[i].text != (char *)"" && text_ptr[i].text != NULL) {
            free(text_ptr[i].text);
        }
    }
    free(text_ptr);
}

static void fuzz_png_get_text(png_structp png_ptr, png_infop info_ptr) {
    png_textp text_ptr = NULL;
    int num_text = 0;
    int result = png_get_text(png_ptr, info_ptr, &text_ptr, &num_text);
    (void)result;
    // No cleanup needed as libpng manages the memory for text_ptr returned by png_get_text
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    // Set up error handling to prevent crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Fuzz the target functions
    fuzz_png_set_invalid(png_ptr, info_ptr, Data, Size);
    fuzz_png_data_freer(png_ptr, info_ptr, Data, Size);
    fuzz_png_free_data(png_ptr, info_ptr, Data, Size);
    fuzz_png_set_unknown_chunks(png_ptr, info_ptr, Data, Size);
    fuzz_png_set_text(png_ptr, info_ptr, Data, Size);
    fuzz_png_get_text(png_ptr, info_ptr);
    
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return 0;
}