// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_set_cLLI_fixed at pngset.c:162:1 in png.h
// png_set_cLLI_fixed at pngset.c:162:1 in png.h
// png_set_cLLI_fixed at pngset.c:162:1 in png.h
// png_get_cLLI_fixed at pngget.c:788:1 in png.h
// png_get_cLLI_fixed at pngget.c:788:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <cstring>

static void png_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    return;
}

static void png_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    return;
}

struct FuzzReadState {
    const uint8_t *data;
    size_t size;
    size_t offset;
};

static void fuzz_read_data(png_structp png_ptr, png_bytep out, png_size_t length) {
    FuzzReadState *state = (FuzzReadState *)png_get_io_ptr(png_ptr);
    if (!state) return;
    
    if (state->offset + length > state->size) {
        length = state->size - state->offset;
    }
    if (length == 0) {
        return;
    }
    memcpy(out, state->data + state->offset, length);
    state->offset += length;
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_set_error_fn(png_ptr, nullptr, png_error_handler, png_warning_handler);

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    FuzzReadState state = { Data, Size, 0 };
    png_init_io(png_ptr, nullptr);
    png_set_read_fn(png_ptr, &state, fuzz_read_data);

    // Use setjmp to catch errors during reading
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    png_read_info(png_ptr, info_ptr);

    // Test png_get_gAMA_fixed
    png_fixed_point gama_value = 0;
    png_get_gAMA_fixed(png_ptr, info_ptr, &gama_value);
    png_get_gAMA_fixed(png_ptr, info_ptr, nullptr);

    // Test png_get_x_pixels_per_meter
    png_uint_32 x_pmm = png_get_x_pixels_per_meter(png_ptr, info_ptr);

    // Test png_set_cLLI_fixed
    png_fixed_point max_cll = 100000;
    png_fixed_point max_fall = 50000;
    png_set_cLLI_fixed(png_ptr, info_ptr, max_cll, max_fall);
    png_set_cLLI_fixed(png_ptr, info_ptr, 200000, 200000);
    png_set_cLLI_fixed(png_ptr, info_ptr, 300000, 300000);

    // Test png_get_cLLI_fixed with correct types (png_uint_32)
    png_uint_32 get_max_cll = 0, get_max_fall = 0;
    png_get_cLLI_fixed(png_ptr, info_ptr, &get_max_cll, &get_max_fall);
    png_get_cLLI_fixed(png_ptr, info_ptr, nullptr, nullptr);

    // Test png_get_sCAL_fixed
    int unit = 0;
    png_fixed_point width = 0, height = 0;
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width, &height);
    png_get_sCAL_fixed(png_ptr, info_ptr, nullptr, nullptr, nullptr);

    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}