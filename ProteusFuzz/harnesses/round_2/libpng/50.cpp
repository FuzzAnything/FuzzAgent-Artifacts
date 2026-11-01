// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_packing at pngtrans.c:49:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_set_strip_alpha at pngrtran.c:198:1 in png.h
// png_set_packswap at pngtrans.c:69:1 in png.h
// png_set_read_user_transform_fn at pngrtran.c:1099:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_write_user_transform_fn at pngwrite.c:1360:1 in png.h
// png_set_packing at pngtrans.c:49:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_set_strip_alpha at pngrtran.c:198:1 in png.h
// png_set_packswap at pngtrans.c:69:1 in png.h
// png_set_read_user_transform_fn at pngrtran.c:1099:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
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

static void dummy_user_transform_fn(png_structp png_ptr, png_row_infop row_info, png_bytep row) {
    (void)png_ptr;
    (void)row_info;
    (void)row;
    // Do nothing
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a minimal valid PNG header for testing
    const uint8_t png_signature[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
    
    // Prepare data buffer
    uint8_t *fuzz_data = nullptr;
    size_t fuzz_size = Size;
    bool need_free = false;

    // If the input doesn't start with PNG signature, prepend it
    if (Size < 8 || memcmp(Data, png_signature, 8) != 0) {
        fuzz_data = (uint8_t *)malloc(Size + 8);
        if (!fuzz_data) {
            return 0;
        }
        memcpy(fuzz_data, png_signature, 8);
        memcpy(fuzz_data + 8, Data, Size);
        fuzz_size = Size + 8;
        need_free = true;
    } else {
        fuzz_data = (uint8_t *)Data;
    }

    // Write to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        if (need_free) free(fuzz_data);
        return 0;
    }
    fwrite(fuzz_data, 1, fuzz_size, fp);
    fclose(fp);

    // We can safely free the allocated buffer here if we allocated it,
    // because we've already written to the file.
    if (need_free) {
        free(fuzz_data);
        need_free = false;
    }

    // Create read struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Setup error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Open the file
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Skip signature if we added it or if it was already there
    // Since we ensured the file starts with signature, we skip 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // Read IHDR
    png_read_info(png_ptr, info_ptr);

    // Now test the target API functions for reading
    png_set_packing(png_ptr);
    png_set_swap_alpha(png_ptr);
    png_set_strip_alpha(png_ptr);
    png_set_packswap(png_ptr);
    png_set_read_user_transform_fn(png_ptr, dummy_user_transform_fn);

    // Also test write functions by creating a write struct
    png_structp png_write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_write_ptr) {
        png_infop write_info_ptr = png_create_info_struct(png_write_ptr);
        if (write_info_ptr) {
            // Set up write transformations
            png_set_write_user_transform_fn(png_write_ptr, dummy_user_transform_fn);
            png_set_packing(png_write_ptr);
            png_set_swap_alpha(png_write_ptr);
            png_set_strip_alpha(png_write_ptr);
            png_set_packswap(png_write_ptr);
            png_set_read_user_transform_fn(png_write_ptr, dummy_user_transform_fn);
            
            png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
        } else {
            png_destroy_write_struct(&png_write_ptr, NULL);
        }
    }

    // Clean up
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}