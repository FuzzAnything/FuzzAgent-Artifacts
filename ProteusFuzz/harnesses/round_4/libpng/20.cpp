// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_next_frame_delay_num at pngget.c:1469:1 in png.h
// png_get_next_frame_fcTL at pngget.c:1399:1 in png.h
// png_read_frame_head at pngread.c:195:1 in png.h
// png_write_frame_head at pngwrite.c:1501:1 in png.h
// png_set_next_frame_fcTL at pngset.c:1357:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_get_next_frame_delay_den at pngget.c:1479:1 in png.h
#include <png.h>
#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>
#include <cstring>
#include <cstdlib>

// Global buffer for setjmp/longjmp
static jmp_buf g_jmp_buf;
static volatile int g_error_occurred = 0;

// Static helper to handle errors using longjmp
static void fuzz_png_error(png_structp png_ptr, const char *error_message) {
    (void)png_ptr;
    (void)error_message;
    g_error_occurred = 1;
    longjmp(g_jmp_buf, 1);
}

// Static helper to handle warnings
static void fuzz_png_warning(png_structp png_ptr, const char *warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

// Static helper for reading data (dummy implementation for fuzzing)
static void fuzz_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    if (data) {
        memset(data, 0, length);
    }
}

// Static helper for writing data (dummy implementation)
static void fuzz_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

// Static helper for flushing (dummy)
static void fuzz_flush(png_structp png_ptr) {
    (void)png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    // Create png_struct and png_info
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, fuzz_png_error, fuzz_png_warning);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling with setjmp
    if (setjmp(g_jmp_buf)) {
        // Error occurred, cleanup and return
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Initialize read callbacks
    png_set_read_fn(png_ptr, NULL, fuzz_read_data);

    // Initialize write callbacks for write tests
    png_set_write_fn(png_ptr, NULL, fuzz_write_data, fuzz_flush);

    // Parse input data to extract parameters for fuzzing
    png_uint_32 width = 0, height = 0, x_offset = 0, y_offset = 0;
    png_uint_16 delay_num = 0, delay_den = 0;
    png_byte dispose_op = 0, blend_op = 0;

    if (Size >= 16) {
        width = *(const png_uint_32*)(Data + 0);
        height = *(const png_uint_32*)(Data + 4);
        x_offset = *(const png_uint_32*)(Data + 8);
        y_offset = *(const png_uint_32*)(Data + 12);
    }
    if (Size >= 24) {
        delay_num = *(const png_uint_16*)(Data + 16);
        delay_den = *(const png_uint_16*)(Data + 18);
        dispose_op = Data[20];
        blend_op = Data[21];
    }

    // Limit dimensions to reasonable values to prevent allocation issues
    if (width > 1024) width = 1024;
    if (height > 1024) height = 1024;
    if (x_offset > width) x_offset = 0;
    if (y_offset > height) y_offset = 0;

    // Test 1: png_get_next_frame_delay_den (Read context)
    png_uint_16 den = png_get_next_frame_delay_den(png_ptr, info_ptr);
    (void)den;

    // Test 2: png_get_next_frame_delay_num
    png_uint_16 num = png_get_next_frame_delay_num(png_ptr, info_ptr);
    (void)num;

    // Test 3: png_get_next_frame_fcTL
    png_uint_32 w, h, xo, yo;
    png_uint_16 dn, dd;
    png_byte do_op, bo_op;
    png_uint_32 res = png_get_next_frame_fcTL(png_ptr, info_ptr, &w, &h, &xo, &yo, &dn, &dd, &do_op, &bo_op);
    (void)res;

    // Test 4: png_read_frame_head
    // This might trigger an error if acTL is missing, which is expected in fuzzing
    png_read_frame_head(png_ptr, info_ptr);

    // Test 5: png_write_frame_head
    // Prepare dummy row pointers for writing
    png_byte **row_pointers = NULL;
    if (width > 0 && height > 0) {
        row_pointers = (png_byte **)malloc(height * sizeof(png_byte *));
        if (row_pointers) {
            for (png_uint_32 i = 0; i < height; ++i) {
                row_pointers[i] = (png_byte *)malloc(width);
                if (row_pointers[i]) {
                    memset(row_pointers[i], 0, width);
                }
            }
        }
    }

    png_write_frame_head(png_ptr, info_ptr, row_pointers, width, height, x_offset, y_offset, delay_num, delay_den, dispose_op, blend_op);

    // Cleanup row pointers
    if (row_pointers) {
        for (png_uint_32 i = 0; i < height; ++i) {
            free(row_pointers[i]);
        }
        free(row_pointers);
    }

    // Test 6: png_set_next_frame_fcTL
    png_uint_32 set_res = png_set_next_frame_fcTL(png_ptr, info_ptr, width, height, x_offset, y_offset, delay_num, delay_den, dispose_op, blend_op);
    (void)set_res;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}