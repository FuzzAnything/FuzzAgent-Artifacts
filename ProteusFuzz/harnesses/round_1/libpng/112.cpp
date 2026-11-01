// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
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
#include <cstdint>
#include <cstring>
#include <png.h>

static void dummy_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    return;
}

static void dummy_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    return;
}

extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    png_byte buf16[2];
    png_byte buf32[4];
    png_int_32 int32_result;
    png_uint_32 uint32_result;
    png_uint_32 uint31_result;

    // Initialize buffers with input data
    std::memcpy(buf16, Data, std::min(Size, (size_t)2));
    std::memcpy(buf32, Data, std::min(Size, (size_t)4));

    // Test png_save_uint_16
    unsigned int val16 = (Size >= 2) ? (Data[0] << 8 | Data[1]) : 0;
    png_save_uint_16(buf16, val16);

    // Test png_save_uint_32
    png_uint_32 val32 = (Size >= 4) ? 
        ((png_uint_32)Data[0] << 24 | (png_uint_32)Data[1] << 16 | 
         (png_uint_32)Data[2] << 8 | (png_uint_32)Data[3]) : 0;
    png_save_uint_32(buf32, val32);

    // Test png_get_int_32
    int32_result = png_get_int_32(buf32);
    (void)int32_result;

    // Test png_get_uint_32
    uint32_result = png_get_uint_32(buf32);
    (void)uint32_result;

    // Test png_save_int_32
    png_int_32 val_int32 = (int32_result > 1000) ? 1000 : int32_result;
    png_save_int_32(buf32, val_int32);

    // Test png_get_uint_31 with safety check
    // Create a minimal png_struct for the call
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        return 0;
    }
    
    // Set dummy error handlers to prevent aborts
    png_set_error_fn(png_ptr, NULL, dummy_error_handler, dummy_warning_handler);

    // Pre-validate to ensure value does not exceed PNG_UINT_31_MAX
    png_uint_32 test_val = png_get_uint_32(buf32);
    if (test_val <= PNG_UINT_31_MAX) {
        uint31_result = png_get_uint_31(png_ptr, buf32);
        (void)uint31_result;
    }

    png_destroy_read_struct(&png_ptr, NULL, NULL);

    return 0;
}