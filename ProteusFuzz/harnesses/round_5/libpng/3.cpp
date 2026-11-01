// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_iCCP at pngset.c:859:1 in png.h
// png_get_iCCP at pngget.c:718:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
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
#include <cstdint>
#include <cstring>
#include <cstdlib>

static void dummy_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do nothing to prevent abort during fuzzing
}

static void dummy_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Do nothing to prevent abort during fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Create a dummy file to simulate input
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize png_struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        remove(dummy_file);
        return 0;
    }

    // Set custom error handlers to prevent aborts
    png_set_error_fn(png_ptr, NULL, dummy_error, dummy_warning);

    // Initialize png_info
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        remove(dummy_file);
        return 0;
    }

    // Initialize IO
    fp = fopen(dummy_file, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        remove(dummy_file);
        return 0;
    }
    png_init_io(png_ptr, fp);

    // Read PNG info (header)
    // This might fail if the data is not a valid PNG, which is expected in fuzzing
    png_read_info(png_ptr, info_ptr);

    // Fuzz png_set_iCCP
    // We create a small dummy profile from the input data
    png_byte dummy_profile[64];
    size_t profile_len = (Size > 64) ? 64 : Size;
    memcpy(dummy_profile, Data, profile_len);
    const char *profile_name = "TestProfile";
    int compression_type = PNG_COMPRESSION_TYPE_BASE;

    // Test png_set_iCCP with valid data
    png_set_iCCP(png_ptr, info_ptr, profile_name, compression_type, dummy_profile, profile_len);

    // Test png_get_iCCP
    char *name = NULL;
    int comp_type = 0;
    png_byte *profile = NULL;
    png_uint_32 proflen = 0;
    png_uint_32 result = png_get_iCCP(png_ptr, info_ptr, &name, &comp_type, &profile, &proflen);
    (void)result; // Use result to suppress warnings

    // Test png_get_compression_type
    png_byte comp_method = png_get_compression_type(png_ptr, info_ptr);
    (void)comp_method;

    // Test png_free_data
    // Free all allocated text data
    png_free_data(png_ptr, info_ptr, PNG_FREE_TEXT, -1);
    // Free iCCP data
    png_free_data(png_ptr, info_ptr, PNG_FREE_ICCP, 0);

    // Cleanup
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    remove(dummy_file);

    return 0;
}