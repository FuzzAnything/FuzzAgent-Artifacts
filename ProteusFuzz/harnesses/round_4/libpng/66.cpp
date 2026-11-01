// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_set_rows at pngset.c:1845:1 in png.h
// png_set_rows at pngset.c:1845:1 in png.h
// png_get_rows at pngget.c:50:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
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

static void dummy_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        return 0;
    }

    png_inforp info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    png_set_error_fn(png_ptr, nullptr, dummy_error_callback, dummy_warning_callback);

    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    size_t num_rows = 0;
    if (Size >= 4) {
        num_rows = (Data[0] % 10) + 1;
    }
    
    // Use C-style allocation for the array of pointers to match libpng's expectations
    png_byte **row_pointers = (png_byte **)malloc(num_rows * sizeof(png_byte *));
    if (row_pointers == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        remove(dummy_file);
        return 0;
    }

    for (size_t i = 0; i < num_rows; ++i) {
        size_t row_size = (Data[(i + 1) % Size] % 256) + 1;
        row_pointers[i] = (png_byte*)malloc(row_size);
        if (row_pointers[i]) {
            memset(row_pointers[i], 0, row_size);
        }
    }

    png_uint_32 mask = (png_uint_32)Data[Size % Size];
    int freer_val = (Data[(Size + 1) % Size] % 2) ? PNG_USER_WILL_FREE_DATA : PNG_DESTROY_WILL_FREE_DATA;
    png_data_freer(png_ptr, info_ptr, freer_val, mask);

    png_set_rows(png_ptr, info_ptr, row_pointers);
    
    // Pass nullptr to clear rows. libpng will attempt to free the array if it owns it.
    png_set_rows(png_ptr, info_ptr, nullptr);

    png_byte **retrieved_rows = png_get_rows(png_ptr, info_ptr);
    (void)retrieved_rows;

    png_uint_32 free_mask = (png_uint_32)Data[(Size + 2) % Size];
    int free_num = (int)(Data[(Size + 3) % Size] % 5) - 1;
    png_free_data(png_ptr, info_ptr, free_mask, free_num);

    int invalid_mask = (int)(Data[(Size + 4) % Size]);
    png_set_invalid(png_ptr, info_ptr, invalid_mask);

    // Fix: Cast to png_infopp to match the expected type for png_destroy_info_struct
    png_infopp info_ptr_ptr = (png_infopp)&info_ptr;
    png_destroy_info_struct(png_ptr, info_ptr_ptr);

    png_destroy_read_struct(&png_ptr, nullptr, nullptr);

    // Cleanup row_pointers if libpng didn't free them (e.g. if we set USER_WILL_FREE_DATA)
    // If freer_val was PNG_USER_WILL_FREE_DATA, libpng won't free it.
    // If freer_val was PNG_DESTROY_WILL_FREE_DATA, libpng already freed it.
    if (freer_val == PNG_USER_WILL_FREE_DATA) {
        for (size_t i = 0; i < num_rows; ++i) {
            if (row_pointers[i]) {
                free(row_pointers[i]);
            }
        }
        free(row_pointers);
    }

    remove(dummy_file);

    return 0;
}