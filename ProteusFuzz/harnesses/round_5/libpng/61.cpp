// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_unknown_chunks at pngget.c:1249:1 in png.h
// png_get_eXIf_1 at pngget.c:892:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_get_acTL at pngget.c:1361:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_set_unknown_chunk_location at pngset.c:1592:1 in png.h
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
#include <cstdio>
#include <cstdlib>

static void dummy_error_fn(png_structp png_ptr, const char *error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, const char *warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void dummy_write_data_fn(png_structp png_ptr, png_bytep data, size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    png_infopp info_ptr_ptr = nullptr;

    // Initialize png_struct
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return 0;

    // Initialize png_info
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    info_ptr_ptr = &info_ptr;

    // Set up error handling with setjmp/longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, info_ptr_ptr, nullptr);
        return 0;
    }

    // Initialize dummy file for write operations if needed
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Initialize write function for png_ptr
    png_set_write_fn(png_ptr, nullptr, dummy_write_data_fn, nullptr);

    // Fuzz png_get_acTL
    png_uint_32 num_frames = 0, num_plays = 0;
    png_get_acTL(png_ptr, info_ptr, &num_frames, &num_plays);

    // Fuzz png_set_unknown_chunk_location
    // Prepare some unknown chunks first
    png_unknown_chunk unknown_chunk;
    memset(&unknown_chunk, 0, sizeof(unknown_chunk));
    unknown_chunk.name[0] = 'X';
    unknown_chunk.name[1] = 'c';
    unknown_chunk.name[2] = 'h';
    unknown_chunk.name[3] = 'k';
    unknown_chunk.name[4] = '\0';
    unknown_chunk.data = (png_byte*)malloc(Size > 0 ? Size : 1);
    if (unknown_chunk.data) {
        memcpy(unknown_chunk.data, Data, Size > 0 ? Size : 1);
        unknown_chunk.size = Size > 0 ? Size : 1;
        unknown_chunk.location = PNG_HAVE_IHDR; // Default location

        // Set unknown chunks
        png_set_unknown_chunks(png_ptr, info_ptr, &unknown_chunk, 1);

        // Set location for the chunk
        png_set_unknown_chunk_location(png_ptr, info_ptr, 0, PNG_AFTER_IDAT);
        
        // Free allocated data
        free(unknown_chunk.data);
    }

    // Fuzz png_get_unknown_chunks
    // The function expects a pointer to a pointer (png_unknown_chunkpp), 
    // which is defined as png_unknown_chunk_t **. 
    // We declare a variable of type png_unknown_chunk_t * and pass its address.
    png_unknown_chunk_t *entries = nullptr;
    int num_unknown = png_get_unknown_chunks(png_ptr, info_ptr, &entries);
    (void)num_unknown; // Use the return value to ensure it's compiled in

    // Fuzz png_get_eXIf_1
    png_uint_32 num_exif = 0;
    png_byte *exif = nullptr;
    png_get_eXIf_1(png_ptr, info_ptr, &num_exif, &exif);

    // Cleanup
    png_destroy_info_struct(png_ptr, info_ptr_ptr);
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);

    // Clean up dummy file
    remove(dummy_file);

    return 0;
}