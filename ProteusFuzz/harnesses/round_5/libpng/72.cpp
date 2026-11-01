// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
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
#include <png.h>
#include <setjmp.h>

static jmp_buf jmpbuf;
static bool jump_occurred = false;

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Trigger longjmp to catch the error instead of aborting
    jump_occurred = true;
    longjmp(jmpbuf, 1);
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Allocate buffers for operations
    png_byte buf[4];
    png_byte buf16[2];
    png_uint_32 u32_val;
    png_int_32 i32_val;
    unsigned int u16_val;

    // Initialize buffers with input data
    std::memcpy(buf, Data, 4);
    if (Size >= 2) {
        std::memcpy(buf16, Data, 2);
    } else {
        buf16[0] = Data[0];
        buf16[1] = 0;
    }

    // Extract values from input for save operations
    u32_val = 0;
    if (Size >= 4) {
        u32_val = (png_uint_32)Data[0] | ((png_uint_32)Data[1] << 8) | 
                  ((png_uint_32)Data[2] << 16) | ((png_uint_32)Data[3] << 24);
    }
    i32_val = (png_int_32)u32_val;
    u16_val = (Size >= 2) ? ((unsigned int)Data[0] | ((unsigned int)Data[1] << 8)) : ((unsigned int)Data[0]);

    // Test png_get_uint_32
    png_uint_32 result_uint32 = png_get_uint_32(buf);
    (void)result_uint32;

    // Test png_save_uint_32
    png_save_uint_32(buf, u32_val);

    // Test png_get_int_32
    png_int_32 result_int32 = png_get_int_32(buf);
    (void)result_int32;

    // Test png_save_int_32
    png_save_int_32(buf, i32_val);

    // Test png_save_uint_16
    png_save_uint_16(buf16, u16_val);

    // Test png_get_uint_31 (requires png_struct)
    // We wrap this in setjmp/longjmp to handle the png_error call safely
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error_fn, dummy_warning_fn);
    
    if (png_ptr != NULL) {
        // Set the error handler to use our custom one
        png_set_error_fn(png_ptr, NULL, dummy_error_fn, dummy_warning_fn);

        if (Size >= 4) {
            jump_occurred = false;
            if (setjmp(jmpbuf) == 0) {
                // Try to read uint_31, which might trigger png_error if value > PNG_UINT_31_MAX
                png_uint_32 result_uint31 = png_get_uint_31(png_ptr, buf);
                (void)result_uint31;
            } else {
                // Error was triggered, jump_occurred is true
                // Just continue, don't abort
            }
        }
        png_destroy_read_struct(&png_ptr, NULL, NULL);
    }

    return 0;
}