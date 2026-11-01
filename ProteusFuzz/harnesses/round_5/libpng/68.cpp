// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_first_frame_is_hidden at pngset.c:1435:1 in png.h
// png_set_next_frame_fcTL at pngset.c:1357:1 in png.h
// png_write_frame_head at pngwrite.c:1501:1 in png.h
// png_get_next_frame_height at pngget.c:1439:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_next_frame_x_offset at pngget.c:1449:1 in png.h
// png_get_next_frame_fcTL at pngget.c:1399:1 in png.h
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

static void dummy_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void dummy_write(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Initialize png_struct
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error, dummy_warning);
    if (png_ptr == nullptr) {
        return 0;
    }

    // Initialize png_info
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Setup error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Prepare dummy file for read operations
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Re-open for reading
    fp = fopen(filename, "rb");
    if (fp) {
        png_init_io(png_ptr, fp);

        // Try to read the signature to ensure basic validity
        // Use png_sig_cmp to check signature manually since png_sig_read might not be available
        // or we can just skip the check and let png_read_info handle it.
        // To be safe and compile everywhere, we skip the explicit signature read check here
        // and rely on png_read_info.
        
        // Attempt to read the IHDR chunk to initialize basic info
        png_read_info(png_ptr, info_ptr);
        fclose(fp);
    }

    // --- Test 1: png_get_next_frame_x_offset ---
    // This function requires valid pointers. If info is not fully initialized, it may return 0.
    png_uint_32 x_offset = png_get_next_frame_x_offset(png_ptr, info_ptr);
    (void)x_offset;

    // --- Test 2: png_get_next_frame_fcTL ---
    png_uint_32 width, height, x_off, y_off;
    png_uint_16 delay_num, delay_den;
    png_byte dispose_op, blend_op;
    width = height = x_off = y_off = 0;
    delay_num = delay_den = 0;
    dispose_op = blend_op = 0;

    png_uint_32 ret_fcTL = png_get_next_frame_fcTL(png_ptr, info_ptr,
        &width, &height, &x_off, &y_off,
        &delay_num, &delay_den, &dispose_op, &blend_op);
    (void)ret_fcTL;

    // --- Test 3: png_set_first_frame_is_hidden ---
    // This is a write-time function. We will call it with the read struct to test robustness.
    // It returns 0 if png_ptr is NULL, but we have a valid png_ptr.
    png_uint_32 ret_hidden = png_set_first_frame_is_hidden(png_ptr, info_ptr, (png_byte)(Data[0] & 1));
    (void)ret_hidden;

    // --- Test 4: png_set_next_frame_fcTL ---
    // Extract parameters from fuzz input
    png_uint_32 f_width = (Size > 4) ? *(const png_uint_32*)(Data + 1) : 10;
    png_uint_32 f_height = (Size > 8) ? *(const png_uint_32*)(Data + 5) : 10;
    png_uint_32 f_x_off = (Size > 12) ? *(const png_uint_32*)(Data + 9) : 0;
    png_uint_32 f_y_off = (Size > 16) ? *(const png_uint_32*)(Data + 13) : 0;
    png_uint_16 f_delay_num = (Size > 20) ? *(const png_uint_16*)(Data + 17) : 1;
    png_uint_16 f_delay_den = (Size > 22) ? *(const png_uint_16*)(Data + 19) : 100;
    png_byte f_dispose = (png_byte)(Data[21] & 0x03); // Valid range 0-3
    png_byte f_blend = (png_byte)(Data[22] & 0x01);   // Valid range 0-1

    png_uint_32 ret_set_fcTL = png_set_next_frame_fcTL(png_ptr, info_ptr,
        f_width, f_height, f_x_off, f_y_off,
        f_delay_num, f_delay_den, f_dispose, f_blend);
    (void)ret_set_fcTL;

    // --- Test 5: png_write_frame_head ---
    // This function writes to the output stream. Since we didn't initialize a write context,
    // this might trigger an error or do nothing. We pass a dummy row pointer array.
    png_byte **row_pointers = nullptr;
    // Allocate a small dummy row pointer array if needed, but nullptr is a valid test case
    // for robustness testing.
    png_write_frame_head(png_ptr, info_ptr, row_pointers,
        f_width, f_height, f_x_off, f_y_off,
        f_delay_num, f_delay_den, f_dispose, f_blend);

    // --- Test 6: png_get_next_frame_height ---
    png_uint_32 next_height = png_get_next_frame_height(png_ptr, info_ptr);
    (void)next_height;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    // Remove dummy file
    std::remove(filename);

    return 0;
}