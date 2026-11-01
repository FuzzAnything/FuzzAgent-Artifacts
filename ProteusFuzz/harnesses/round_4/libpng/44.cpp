// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
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

static jmp_buf jump_buffer;
static int jump_occurred = 0;

static void dummy_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Trigger longjmp to catch the error instead of aborting
    jump_occurred = 1;
    longjmp(jump_buffer, 1);
}

static void dummy_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    png_byte buf[4];
    png_int_32 int_val;
    png_uint_32 uint_val;
    png_uint_16 uint16_val;
    unsigned int input_uint;
    png_int_32 input_int;

    // Initialize buffers from input data
    memcpy(buf, Data, std::min(Size, static_cast<size_t>(4)));

    // Fuzz png_get_int_32
    int_val = png_get_int_32(buf);
    (void)int_val;

    // Fuzz png_get_uint_32
    uint_val = png_get_uint_32(buf);
    (void)uint_val;

    // Fuzz png_save_int_32
    input_int = (png_int_32)Data[0];
    if (Size > 1) input_int |= ((png_int_32)Data[1] << 8);
    if (Size > 2) input_int |= ((png_int_32)Data[2] << 16);
    if (Size > 3) input_int |= ((png_int_32)Data[3] << 24);
    png_save_int_32(buf, input_int);

    // Fuzz png_save_uint_32
    input_uint = Data[0];
    if (Size > 1) input_uint |= ((unsigned int)Data[1] << 8);
    if (Size > 2) input_uint |= ((unsigned int)Data[2] << 16);
    if (Size > 3) input_uint |= ((unsigned int)Data[3] << 24);
    png_save_uint_32(buf, (png_uint_32)input_uint);

    // Fuzz png_save_uint_16
    uint16_val = (png_uint_16)(Data[0] | (Size > 1 ? (Data[1] << 8) : 0));
    png_save_uint_16(buf, (unsigned int)uint16_val);

    // Fuzz png_get_uint_31
    // Requires a valid png_struct pointer. We create a minimal one.
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        return 0;
    }
    
    // Set error handler to avoid crash on invalid data
    png_set_error_fn(png_ptr, NULL, dummy_error_handler, dummy_warning_handler);
    
    // Re-initialize buf from input for this specific call
    memcpy(buf, Data, std::min(Size, static_cast<size_t>(4)));
    
    // Check value to decide whether to call png_get_uint_31
    png_uint_32 check_val = png_get_uint_32(buf);
    
    // png_get_uint_31 triggers png_error if value > PNG_UINT_31_MAX.
    // We wrap the call in setjmp/longjmp to handle the error gracefully.
    if (setjmp(jump_buffer) == 0) {
        jump_occurred = 0;
        png_get_uint_31(png_ptr, buf);
    } else {
        // Error occurred, jump_occurred is set to 1
        // We continue execution instead of aborting
    }

    png_destroy_read_struct(&png_ptr, NULL, NULL);

    return 0;
}