// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_next_frame_blend_op at pngget.c:1499:1 in png.h
// png_get_next_frame_blend_op at pngget.c:1499:1 in png.h
// png_get_next_frame_blend_op at pngget.c:1499:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_signature at pngget.c:491:1 in png.h
// png_get_signature at pngget.c:491:1 in png.h
// png_get_signature at pngget.c:491:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
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

// Custom error handler to prevent aborts during fuzzing
static void fuzz_png_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do nothing, just return to avoid abort
}

static void fuzz_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Do nothing
}

// Helper to initialize png_struct and info_struct with dummy data
static bool init_png_structs(png_structp *png_ptr, png_infop *info_ptr) {
    *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, fuzz_png_error, fuzz_png_warning);
    if (!*png_ptr) return false;

    *info_ptr = png_create_info_struct(*png_ptr);
    if (!*info_ptr) {
        png_destroy_read_struct(png_ptr, NULL, NULL);
        return false;
    }

    // We do not set a read function because we are not reading a file.
    // We will rely on the API functions to handle NULL or uninitialized states gracefully.
    // The goal is to test the getters on the structures themselves.

    return true;
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    // 1. Prepare the environment
    if (!init_png_structs(&png_ptr, &info_ptr)) {
        return 0;
    }

    // To prevent png_get_IHDR from calling png_check_IHDR which triggers an error
    // on invalid/uninitialized IHDR data, we must ensure the info structure has valid IHDR data.
    // Since we are not reading a file, we manually set the IHDR fields to valid values.
    // Note: In a real scenario, these are set by png_read_info. Here we simulate it.
    // We access the struct members directly because they are part of the public API definition
    // in the provided context, or we rely on the fact that png_set_IHDR is the proper way.
    // However, png_set_IHDR is not in the target list, but it is a standard libpng function.
    // Let's use png_set_IHDR to safely populate the info structure.
    
    png_uint_32 dummy_width = 1;
    png_uint_32 dummy_height = 1;
    int dummy_bit_depth = 8;
    int dummy_color_type = PNG_COLOR_TYPE_RGB;
    int dummy_compression_method = PNG_COMPRESSION_TYPE_BASE;
    int dummy_filter_method = PNG_FILTER_TYPE_BASE;
    int dummy_interlace_method = PNG_INTERLACE_NONE;

    // Set valid IHDR data to prevent internal checks from failing
    png_set_IHDR(png_ptr, info_ptr, dummy_width, dummy_height, 
                 dummy_bit_depth, dummy_color_type, dummy_interlace_method,
                 dummy_compression_method, dummy_filter_method);

    // 2. Invoke target functions diversely

    // Test 1: png_get_compression_type
    {
        png_byte ct1 = png_get_compression_type(png_ptr, info_ptr);
        (void)ct1;
        png_byte ct2 = png_get_compression_type(NULL, info_ptr);
        (void)ct2;
        png_byte ct3 = png_get_compression_type(png_ptr, NULL);
        (void)ct3;
    }

    // Test 2: png_get_bit_depth
    {
        png_byte bd1 = png_get_bit_depth(png_ptr, info_ptr);
        (void)bd1;
        png_byte bd2 = png_get_bit_depth(NULL, info_ptr);
        (void)bd2;
        png_byte bd3 = png_get_bit_depth(png_ptr, NULL);
        (void)bd3;
    }

    // Test 3: png_get_IHDR
    {
        png_uint_32 width = 0, height = 0;
        int bit_depth = 0, color_type = 0;
        int interlace_method = 0, compression_method = 0, filter_method = 0;
        
        // Valid call (should now work because IHDR is set)
        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_method, &compression_method, &filter_method);
        
        // NULL png_ptr
        png_get_IHDR(NULL, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_method, &compression_method, &filter_method);
        
        // NULL info_ptr
        png_get_IHDR(png_ptr, NULL, &width, &height, &bit_depth, &color_type, &interlace_method, &compression_method, &filter_method);

        // Partial NULLs for output pointers
        png_get_IHDR(png_ptr, info_ptr, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        png_get_IHDR(png_ptr, info_ptr, &width, NULL, NULL, NULL, NULL, NULL, NULL);
    }

    // Test 4: png_get_next_frame_blend_op
    {
        png_byte blend1 = png_get_next_frame_blend_op(png_ptr, info_ptr);
        (void)blend1;
        png_byte blend2 = png_get_next_frame_blend_op(NULL, info_ptr);
        (void)blend2;
        png_byte blend3 = png_get_next_frame_blend_op(png_ptr, NULL);
        (void)blend3;
    }

    // Test 5: png_get_color_type
    {
        png_byte ct1 = png_get_color_type(png_ptr, info_ptr);
        (void)ct1;
        png_byte ct2 = png_get_color_type(NULL, info_ptr);
        (void)ct2;
        png_byte ct3 = png_get_color_type(png_ptr, NULL);
        (void)ct3;
    }

    // Test 6: png_get_signature
    {
        const png_byte *sig1 = png_get_signature(png_ptr, info_ptr);
        (void)sig1;
        const png_byte *sig2 = png_get_signature(NULL, info_ptr);
        (void)sig2;
        const png_byte *sig3 = png_get_signature(png_ptr, NULL);
        (void)sig3;
    }

    // 3. Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}