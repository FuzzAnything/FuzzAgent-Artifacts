// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_get_iCCP at pngget.c:718:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_sRGB at pngget.c:699:1 in png.h
// png_get_pHYs at pngget.c:1087:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

// Custom error handler to prevent aborts during fuzzing
static void
fuzz_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do nothing to allow fuzzing to continue
}

static void
fuzz_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Do nothing
}

// Helper struct for custom read function
struct read_state {
    const uint8_t *data;
    size_t size;
    size_t offset;
};

// Custom read function definition
static void
custom_read_data(png_structp png_ptr, png_bytep data_out, png_size_t length) {
    struct read_state *state = (struct read_state *)png_get_io_ptr(png_ptr);
    if (state->offset + length > state->size) {
        png_error(png_ptr, "Read beyond end of data");
    }
    memcpy(data_out, state->data + state->offset, length);
    state->offset += length;
}

// Helper to read PNG info from fuzz input
static bool
read_png_info_from_data(const uint8_t *data, size_t size,
                        png_structp *png_ptr_out, png_infop *info_ptr_out) {
    if (size == 0 || data == nullptr) {
        return false;
    }

    // Create png_struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, fuzz_error_handler, fuzz_warning_handler);
    if (!png_ptr) {
        return false;
    }

    // Create info_struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return false;
    }

    // Setup error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return false;
    }

    // Initialize custom read state
    struct read_state state;
    state.data = data;
    state.size = size;
    state.offset = 0;
    
    // Set custom read function
    png_set_read_fn(png_ptr, (png_voidp)&state, custom_read_data);
    
    // Read the header and info
    png_read_info(png_ptr, info_ptr);
    
    *png_ptr_out = png_ptr;
    *info_ptr_out = info_ptr;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    if (!read_png_info_from_data(Data, Size, &png_ptr, &info_ptr)) {
        return 0;
    }

    // Ensure we have valid pointers before calling target functions
    if (!png_ptr || !info_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Prepare output variables for all target functions
    png_int_32 offset_x = 0, offset_y = 0;
    int unit_type = 0;
    
    char *name = nullptr;
    int compression_type = 0;
    png_byte *profile = nullptr;
    png_uint_32 proflen = 0;
    
    png_uint_32 res_x = 0, res_y = 0;
    
    png_fixed_point int_white_x = 0, int_white_y = 0;
    png_fixed_point int_red_x = 0, int_red_y = 0;
    png_fixed_point int_green_x = 0, int_green_y = 0;
    png_fixed_point int_blue_x = 0, int_blue_y = 0;
    
    int file_srgb_intent = 0;
    
    png_byte colour_primaries = 0;
    png_byte transfer_function = 0;
    png_byte matrix_coefficients = 0;
    png_byte video_full_range_flag = 0;

    // Call target functions to fuzz them
    // 1. png_get_oFFs
    png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, &unit_type);
    
    // 2. png_get_iCCP
    png_get_iCCP(png_ptr, info_ptr, &name, &compression_type, &profile, &proflen);
    // Note: profile data is allocated by libpng, we should free it if not NULL
    if (profile) {
        // We don't free here as it's managed by libpng's info structure
        // but in a real app we would need to handle this carefully
    }
    
    // 3. png_get_pHYs_dpi
    png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    
    // 4. png_get_cHRM_fixed
    png_get_cHRM_fixed(png_ptr, info_ptr, &int_white_x, &int_white_y,
                       &int_red_x, &int_red_y, &int_green_x, &int_green_y,
                       &int_blue_x, &int_blue_y);
    
    // 5. png_get_sRGB
    png_get_sRGB(png_ptr, info_ptr, &file_srgb_intent);
    
    // 6. png_get_pHYs
    png_get_pHYs(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    
    // Additional: png_get_cICP if available
    #ifdef PNG_cICP_SUPPORTED
    png_get_cICP(png_ptr, info_ptr, &colour_primaries, &transfer_function,
                 &matrix_coefficients, &video_full_range_flag);
    #endif

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}