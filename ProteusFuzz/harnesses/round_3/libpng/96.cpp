// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
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
#include <cstddef>
#include <png.h>

static void initialize_png_struct(png_structpp png_ptr, png_infopp info_ptr) {
    *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (*png_ptr == NULL) return;
    *info_ptr = png_create_info_struct(*png_ptr);
    if (*info_ptr == NULL) {
        png_destroy_read_struct(png_ptr, NULL, NULL);
        *png_ptr = NULL;
    }
}

static void cleanup_png_struct(png_structpp png_ptr, png_infopp info_ptr) {
    if (*png_ptr != NULL) {
        png_destroy_read_struct(png_ptr, info_ptr, NULL);
        *png_ptr = NULL;
    }
}

extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_byte buf[4];
    png_uint_32 val32;
    png_int_32 val32s;
    unsigned int val16;

    // Test png_save_uint_16
    val16 = Size > 0 ? Data[0] : 0;
    val16 = (val16 << 8) | (Size > 1 ? Data[1] : 0);
    png_save_uint_16(buf, val16);

    // Test png_get_uint_32
    if (Size >= 4) {
        val32 = png_get_uint_32(Data);
        (void)val32;
    }

    // Test png_save_int_32
    png_int_32 signed_val = (Size > 0 ? Data[0] : 0);
    signed_val = (signed_val << 24) | (Size > 1 ? Data[1] : 0) << 16;
    signed_val = signed_val | (Size > 2 ? Data[2] : 0) << 8;
    signed_val = signed_val | (Size > 3 ? Data[3] : 0);
    png_save_int_32(buf, signed_val);

    // Test png_save_uint_32
    png_uint_32 unsigned_val = (Size > 0 ? Data[0] : 0);
    unsigned_val = (unsigned_val << 24) | (Size > 1 ? Data[1] : 0) << 16;
    unsigned_val = unsigned_val | (Size > 2 ? Data[2] : 0) << 8;
    unsigned_val = unsigned_val | (Size > 3 ? Data[3] : 0);
    png_save_uint_32(buf, unsigned_val);

    // Test png_get_int_32
    if (Size >= 4) {
        val32s = png_get_int_32(Data);
        (void)val32s;
    }

    // Test png_get_uint_31
    initialize_png_struct(&png_ptr, &info_ptr);
    if (png_ptr != NULL && Size >= 4) {
        // Ensure the buffer does not trigger an error for valid range
        // We pass the data directly; if it exceeds PNG_UINT_31_MAX, 
        // libpng will call png_error which terminates the process.
        // To prevent crash in fuzzer, we might want to clamp or skip, 
        // but per task we test the API behavior.
        // Note: png_get_uint_31 expects a valid png_ptr.
        // If the value is too large, it will call png_error.
        // In a real fuzzing scenario, we might catch this, but here we let it be.
        // However, to keep the fuzzer alive, we can check the value first.
        png_uint_32 check_val = png_get_uint_32(Data);
        if (check_val <= PNG_UINT_31_MAX) {
            val32 = png_get_uint_31(png_ptr, Data);
            (void)val32;
        }
    }
    cleanup_png_struct(&png_ptr, &info_ptr);

    return 0;
}