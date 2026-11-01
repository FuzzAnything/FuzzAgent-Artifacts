// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_set_text at pngset.c:912:1 in png.h
// png_set_text at pngset.c:912:1 in png.h
// png_set_text at pngset.c:912:1 in png.h
// png_set_text at pngset.c:912:1 in png.h
// png_set_text at pngset.c:912:1 in png.h
// png_get_text at pngget.c:1158:1 in png.h
// png_get_text at pngget.c:1158:1 in png.h
// png_get_text at pngget.c:1158:1 in png.h
// png_get_text at pngget.c:1158:1 in png.h
// png_get_text at pngget.c:1158:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_free_data at png.c:480:1 in png.h
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

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Use setjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Create a dummy file and write input data to it
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    } else {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Initialize I/O
    fp = fopen(dummy_file, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    png_init_io(png_ptr, fp);

    // Try to read header and info to populate info_ptr
    // Note: If the file is not a valid PNG, this might jump to the error handler.
    // We wrap it in setjmp to catch errors gracefully.
    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        remove(dummy_file);
        return 0;
    }
    
    png_read_info(png_ptr, info_ptr);
    fclose(fp);

    // Fuzzing logic for target API functions

    // 1. png_set_invalid
    // Use valid masks defined in png.h. PNG_INFO_IDAT is the correct constant.
    // PNG_INFO_IHDR is also valid but we ensure we use standard defines.
    png_set_invalid(png_ptr, info_ptr, PNG_INFO_IDAT);
    png_set_invalid(png_ptr, info_ptr, PNG_INFO_PLTE);
    png_set_invalid(png_ptr, info_ptr, 0xFFFFFFFF);
    png_set_invalid(NULL, info_ptr, PNG_INFO_IDAT);
    png_set_invalid(png_ptr, NULL, PNG_INFO_IDAT);

    // 2. png_data_freer
    png_data_freer(png_ptr, info_ptr, PNG_DESTROY_WILL_FREE_DATA, PNG_INFO_IDAT);
    png_data_freer(png_ptr, info_ptr, PNG_USER_WILL_FREE_DATA, PNG_INFO_IDAT);
    png_data_freer(png_ptr, info_ptr, PNG_DESTROY_WILL_FREE_DATA, 0xFFFFFFFF);
    png_data_freer(NULL, info_ptr, PNG_DESTROY_WILL_FREE_DATA, PNG_INFO_IDAT);
    png_data_freer(png_ptr, NULL, PNG_DESTROY_WILL_FREE_DATA, PNG_INFO_IDAT);

    // 3. png_free_data
    png_free_data(png_ptr, info_ptr, PNG_FREE_TEXT, -1);
    png_free_data(png_ptr, info_ptr, PNG_FREE_PLTE, -1);
    png_free_data(png_ptr, info_ptr, PNG_FREE_ICCP, -1);
    png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    png_free_data(png_ptr, info_ptr, PNG_FREE_TEXT, 0);
    png_free_data(NULL, info_ptr, PNG_FREE_TEXT, -1);
    png_free_data(png_ptr, NULL, PNG_FREE_TEXT, -1);

    // 4. png_set_unknown_chunks
    // Create dummy unknown chunks
    png_unknown_chunk unknown_chunk;
    memset(unknown_chunk.name, 0, 5);
    memcpy(unknown_chunk.name, "XYZA", 4); // Valid chunk name
    unknown_chunk.size = Size > 10 ? 10 : Size;
    unknown_chunk.data = (png_byte *)malloc(unknown_chunk.size);
    if (unknown_chunk.data) {
        memcpy(unknown_chunk.data, Data, unknown_chunk.size);
        unknown_chunk.location = PNG_HAVE_IHDR | PNG_HAVE_PLTE;
        png_set_unknown_chunks(png_ptr, info_ptr, &unknown_chunk, 1);
        free(unknown_chunk.data);
    }
    png_set_unknown_chunks(NULL, info_ptr, &unknown_chunk, 1);
    png_set_unknown_chunks(png_ptr, NULL, &unknown_chunk, 1);
    png_set_unknown_chunks(png_ptr, info_ptr, NULL, 1);
    png_set_unknown_chunks(png_ptr, info_ptr, &unknown_chunk, 0);

    // 5. png_set_text
    // Create dummy text chunks
    png_text text_chunk;
    text_chunk.compression = PNG_TEXT_COMPRESSION_NONE;
    text_chunk.key = (char *)malloc(10);
    text_chunk.text_length = 5;
    if (text_chunk.key) {
        strncpy(text_chunk.key, "Title", 9);
        text_chunk.key[9] = '\0';
        text_chunk.text = (char *)malloc(10);
        if (text_chunk.text) {
            strncpy(text_chunk.text, "Fuzz", 9);
            text_chunk.text[9] = '\0';
            png_set_text(png_ptr, info_ptr, &text_chunk, 1);
            free(text_chunk.text);
        }
        free(text_chunk.key);
    }
    png_set_text(NULL, info_ptr, &text_chunk, 1);
    png_set_text(png_ptr, NULL, &text_chunk, 1);
    png_set_text(png_ptr, info_ptr, NULL, 1);
    png_set_text(png_ptr, info_ptr, &text_chunk, 0);

    // 6. png_get_text
    png_textp text_ptr = NULL;
    int num_text = 0;
    int result = png_get_text(png_ptr, info_ptr, &text_ptr, &num_text);
    // result is the number of text chunks, text_ptr points to the array
    if (text_ptr) {
        // Do not free text_ptr here as it is managed by libpng
        // Just iterate safely
        for (int i = 0; i < num_text && i < 100; i++) {
            if (text_ptr[i].key) {
                // Access key safely
            }
            if (text_ptr[i].text) {
                // Access text safely
            }
        }
    }
    png_get_text(NULL, info_ptr, &text_ptr, &num_text);
    png_get_text(png_ptr, NULL, &text_ptr, &num_text);
    png_get_text(png_ptr, info_ptr, NULL, &num_text);
    png_get_text(png_ptr, info_ptr, &text_ptr, NULL);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    remove(dummy_file);

    return 0;
}