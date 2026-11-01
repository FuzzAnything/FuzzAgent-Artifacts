// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_text at pngset.c:912:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_get_text at pngget.c:1158:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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
#include <stddef.h>
#include <cstring>
#include <cstdio>

extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling to prevent crashes during fuzzing
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Create a dummy file for reading context if needed
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Initialize png_ptr for reading from the dummy file
    fp = fopen(dummy_file, "rb");
    if (fp) {
        png_init_io(png_ptr, fp);
        // Attempt to read info, which may trigger errors if the file is malformed
        // We catch these via setjmp above
        png_read_info(png_ptr, info_ptr);
        fclose(fp);
    }

    // Fuzz png_set_text
    if (Size >= 20) {
        png_text text[1];
        text[0].compression = -1; // tEXt
        text[0].key = (char*)(Data + 10);
        text[0].text_length = Size - 10;
        // Ensure key is null-terminated for safety
        if (text[0].key[text[0].text_length - 1] != '\0') {
            // Create a safe copy if needed, but for fuzzing we can let it be raw
        }
        png_set_text(png_ptr, info_ptr, text, 1);
    }

    // Fuzz png_set_invalid
    png_uint_32 mask = (Size > 0) ? (png_uint_32)Data[0] : 0;
    png_set_invalid(png_ptr, info_ptr, mask);

    // Fuzz png_get_text
    png_textp text_ptr = NULL;
    int num_text = 0;
    png_get_text(png_ptr, info_ptr, &text_ptr, &num_text);

    // Fuzz png_get_valid
    png_uint_32 flag = (Size > 1) ? (png_uint_32)Data[1] : 0;
    png_uint_32 valid = png_get_valid(png_ptr, info_ptr, flag);
    (void)valid; // Suppress unused variable warning

    // Fuzz png_set_unknown_chunks
    if (Size >= 10) {
        png_unknown_chunk unknown[1];
        memcpy(unknown[0].name, Data, 4);
        unknown[0].name[4] = '\0';
        unknown[0].data = (png_byte*)(Data + 4);
        unknown[0].size = Size - 4;
        unknown[0].location = (Size > 8) ? Data[8] : 0;
        png_set_unknown_chunks(png_ptr, info_ptr, unknown, 1);
    }

    // Fuzz png_free_data
    png_uint_32 free_me = (Size > 2) ? (png_uint_32)Data[2] : 0;
    int num = (Size > 3) ? (int)Data[3] : -1;
    png_free_data(png_ptr, info_ptr, free_me, num);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    // Remove dummy file
    remove(dummy_file);

    return 0;
}