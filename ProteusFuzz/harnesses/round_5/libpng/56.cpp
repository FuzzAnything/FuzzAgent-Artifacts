// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
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
#include <cstring>
#include <png.h>

static void initialize_png_struct(png_structp *png_ptr, png_infop *info_ptr) {
    *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (*png_ptr == NULL) return;
    
    *info_ptr = png_create_info_struct(*png_ptr);
    if (*info_ptr == NULL) {
        png_destroy_read_struct(png_ptr, NULL, NULL);
        *png_ptr = NULL;
        return;
    }
    
    if (setjmp(png_jmpbuf(*png_ptr)) != 0) {
        png_destroy_read_struct(png_ptr, info_ptr, NULL);
        *png_ptr = NULL;
        *info_ptr = NULL;
        return;
    }
}

static void cleanup_png_struct(png_structp *png_ptr, png_infop *info_ptr) {
    if (*png_ptr != NULL) {
        png_destroy_read_struct(png_ptr, info_ptr, NULL);
        *png_ptr = NULL;
        *info_ptr = NULL;
    }
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    
    // Initialize PNG structures for functions that require them
    initialize_png_struct(&png_ptr, &info_ptr);

    // Buffer for 32-bit operations
    png_byte buf32[4];
    // Buffer for 16-bit operations
    png_byte buf16[2];

    // Test png_get_uint_32
    if (Size >= 4) {
        png_uint_32 val = png_get_uint_32(Data);
        (void)val;
    }

    // Test png_save_uint_32
    png_uint_32 test_uint32 = 0;
    if (Size >= 4) {
        test_uint32 = png_get_uint_32(Data);
    }
    png_save_uint_32(buf32, test_uint32);

    // Test png_get_int_32
    if (Size >= 4) {
        png_int_32 val = png_get_int_32(Data);
        (void)val;
    }

    // Test png_save_int_32
    png_int_32 test_int32 = 0;
    if (Size >= 4) {
        test_int32 = png_get_int_32(Data);
    }
    png_save_int_32(buf32, test_int32);

    // Test png_get_uint_31 (requires valid png_ptr)
    if (png_ptr != NULL && Size >= 4) {
        // Use setjmp to catch potential errors from png_get_uint_31
        if (setjmp(png_jmpbuf(png_ptr)) == 0) {
            png_uint_32 val = png_get_uint_31(png_ptr, Data);
            (void)val;
        }
    }

    // Test png_save_uint_16
    unsigned int test_uint16 = 0;
    if (Size >= 2) {
        test_uint16 = (unsigned int)(Data[0] << 8 | Data[1]);
    }
    png_save_uint_16(buf16, test_uint16);

    // Cleanup
    cleanup_png_struct(&png_ptr, &info_ptr);

    return 0;
}