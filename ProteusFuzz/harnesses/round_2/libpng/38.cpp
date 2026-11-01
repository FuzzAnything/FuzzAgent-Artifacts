// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <new>

// Define a structure to hold the buffer state for reading
struct BufferState {
    const uint8_t *buffer;
    size_t offset;
    size_t size;
};

// Custom read function for libpng
static void custom_read_fn(png_structp png_ptr, png_bytep out_data, png_size_t length) {
    BufferState *state = static_cast<BufferState*>(png_get_io_ptr(png_ptr));
    if (!state) return;

    if (state->offset + length > state->size) {
        length = state->size - state->offset;
    }

    if (length > 0) {
        memcpy(out_data, state->buffer + state->offset, length);
        state->offset += length;
    }
}

static png_structp create_png_struct_from_buffer(const uint8_t *data, size_t size, png_infop *info_ptr) {
    if (!data || size < 8) {
        return nullptr;
    }

    // Check PNG signature
    if (memcmp(data, "\x89PNG\r\n\x1a\n", 8) != 0) {
        return nullptr;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return nullptr;
    }

    *info_ptr = png_create_info_struct(png_ptr);
    if (!*info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return nullptr;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, info_ptr, nullptr);
        return nullptr;
    }

    // Create state struct
    BufferState *state = new (std::nothrow) BufferState{data, 0, size};
    if (!state) {
        png_destroy_read_struct(&png_ptr, info_ptr, nullptr);
        return nullptr;
    }

    // Set the custom read function and pass the state
    png_set_read_fn(png_ptr, state, custom_read_fn);

    // Try to read the PNG info
    if (!setjmp(png_jmpbuf(png_ptr))) {
        png_read_info(png_ptr, *info_ptr);
    }

    return png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (!Data || Size == 0) return 0;

    png_infop info_ptr = nullptr;
    png_structp png_ptr = create_png_struct_from_buffer(Data, Size, &info_ptr);

    if (!png_ptr || !info_ptr) {
        if (png_ptr) {
            BufferState *state = static_cast<BufferState*>(png_get_io_ptr(png_ptr));
            if (state) delete state;
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        }
        return 0;
    }

    // Retrieve the state to delete it later
    BufferState *state = static_cast<BufferState*>(png_get_io_ptr(png_ptr));

    // Test png_get_gAMA_fixed
    png_fixed_point gamma_val = 0;
    png_uint_32 gama_result = png_get_gAMA_fixed(png_ptr, info_ptr, &gamma_val);
    (void)gama_result;

    // Test png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)x_ppm;

    // Test png_get_image_width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    (void)width;

    // Test png_get_valid with various flags
    png_uint_32 valid_gama = png_get_valid(png_ptr, info_ptr, PNG_INFO_gAMA);
    png_uint_32 valid_phys = png_get_valid(png_ptr, info_ptr, PNG_INFO_pHYs);
    png_uint_32 valid_hist = png_get_valid(png_ptr, info_ptr, PNG_INFO_hIST);
    png_uint_32 valid_trns = png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);
    (void)valid_gama;
    (void)valid_phys;
    (void)valid_hist;
    (void)valid_trns;

    // Test png_get_uint_16
    if (Size >= 2) {
        png_uint_16 val = png_get_uint_16(Data);
        (void)val;
    }

    // Test png_get_hIST
    png_uint_16 *hist = nullptr;
    png_uint_32 hist_result = png_get_hIST(png_ptr, info_ptr, &hist);
    (void)hist_result;

    // Cleanup
    if (state) {
        delete state;
    }
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}