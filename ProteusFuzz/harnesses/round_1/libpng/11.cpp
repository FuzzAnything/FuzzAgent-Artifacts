// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_interlace_type at pngget.c:107:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_filter_type at pngget.c:98:1 in png.h
// png_get_signature at pngget.c:491:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <png.h>
#include <setjmp.h>
#include <stdint.h>
#include <stddef.h>
#include <cstring>

struct IoState {
    const uint8_t *data;
    size_t size;
    size_t offset;
};

static void fuzz_read_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == nullptr) return;

    IoState *io = static_cast<IoState*>(io_ptr);

    if (io == nullptr) return;

    if (io->offset >= io->size) {
        png_error(png_ptr, "Read beyond end of input");
    }

    size_t remaining = io->size - io->offset;
    size_t to_read = (length < remaining) ? length : remaining;

    if (to_read > 0) {
        memcpy(data, io->data + io->offset, to_read);
        io->offset += to_read;
    }
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare IO state
    IoState io_state;
    io_state.data = Data;
    io_state.size = Size;
    io_state.offset = 0;

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Set the custom read function and IO pointer
    png_set_read_fn(png_ptr, &io_state, fuzz_read_callback);

    // Read the PNG info
    png_read_info(png_ptr, info_ptr);

    // Call target API functions
    png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    (void)interlace_type;

    png_byte compression_type = png_get_compression_type(png_ptr, info_ptr);
    (void)compression_type;

    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    (void)bit_depth;

    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    (void)color_type;

    png_byte filter_type = png_get_filter_type(png_ptr, info_ptr);
    (void)filter_type;

    const png_byte *signature = png_get_signature(png_ptr, info_ptr);
    (void)signature;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}