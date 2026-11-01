// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_get_eXIf_1 at pngget.c:892:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_set_eXIf_1 at pngset.c:319:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
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
#include <cstdio>

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void cleanup_png(png_structp *png_ptr, png_infop *info_ptr) {
    if (png_ptr && *png_ptr) {
        png_destroy_write_struct(png_ptr, info_ptr);
        *png_ptr = nullptr;
        *info_ptr = nullptr;
    }
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for writing if needed
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) {
        return 0;
    }
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);

    // Initialize libpng structures
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Create png_struct
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return 0;
    }

    // Create info_struct
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Set up error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(&png_ptr, &info_ptr);
        return 0;
    }

    // Initialize the file pointer for writing
    png_init_io(png_ptr, fopen(dummy_file, "wb"));

    // Fuzzing logic for the target API functions

    // 1. png_get_valid
    // Check various flags
    png_uint_32 valid_flags[] = {
        PNG_INFO_tRNS, PNG_INFO_PLTE, PNG_INFO_eXIf, PNG_INFO_gAMA, PNG_INFO_cHRM
    };
    for (int i = 0; i < 5; ++i) {
        png_get_valid(png_ptr, info_ptr, valid_flags[i]);
    }

    // 2. png_set_eXIf_1
    // Prepare EXIF data from fuzz input
    if (Size > 10) {
        png_byte *exif_data = (png_byte *)malloc(Size - 10);
        if (exif_data) {
            memcpy(exif_data, Data + 10, Size - 10);
            png_set_eXIf_1(png_ptr, info_ptr, Size - 10, exif_data);
            free(exif_data);
        }
    }

    // 3. png_write_info_before_PLTE
    png_write_info_before_PLTE(png_ptr, info_ptr);

    // 4. png_create_info_struct (already done above, but can be tested again)
    png_infop new_info_ptr = png_create_info_struct(png_ptr);
    if (new_info_ptr) {
        png_destroy_info_struct(png_ptr, &new_info_ptr);
    }

    // 5. png_set_unknown_chunks
    // Prepare unknown chunk data
    if (Size > 20) {
        png_unknown_chunk unknown_chunks[2];
        memset(unknown_chunks, 0, sizeof(unknown_chunks));

        // First chunk
        strncpy((char *)unknown_chunks[0].name, "tEX", 4);
        unknown_chunks[0].name[4] = '\0';
        unknown_chunks[0].size = (Size > 25) ? Size - 25 : 1;
        unknown_chunks[0].data = (png_byte *)malloc(unknown_chunks[0].size);
        if (unknown_chunks[0].data) {
            memcpy(unknown_chunks[0].data, Data + 20, unknown_chunks[0].size);
            unknown_chunks[0].location = PNG_HAVE_IHDR | PNG_HAVE_PLTE;
        }

        // Second chunk
        strncpy((char *)unknown_chunks[1].name, "gAM", 4);
        unknown_chunks[1].name[4] = '\0';
        unknown_chunks[1].size = (Size > 30) ? Size - 30 : 1;
        unknown_chunks[1].data = (png_byte *)malloc(unknown_chunks[1].size);
        if (unknown_chunks[1].data) {
            memcpy(unknown_chunks[1].data, Data + 25, unknown_chunks[1].size);
            unknown_chunks[1].location = PNG_HAVE_IHDR;
        }

        png_set_unknown_chunks(png_ptr, info_ptr, unknown_chunks, 2);

        // Cleanup unknown chunk data
        if (unknown_chunks[0].data) free(unknown_chunks[0].data);
        if (unknown_chunks[1].data) free(unknown_chunks[1].data);
    }

    // 6. png_get_eXIf_1
    png_uint_32 num_exif = 0;
    png_byte *exif = nullptr;
    png_get_eXIf_1(png_ptr, info_ptr, &num_exif, &exif);
    // Note: exif is managed by libpng, do not free it manually

    // Cleanup
    cleanup_png(&png_ptr, &info_ptr);

    return 0;
}