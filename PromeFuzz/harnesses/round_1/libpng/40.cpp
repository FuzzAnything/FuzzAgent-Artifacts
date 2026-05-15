// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_expand_gray_1_2_4_to_8 at pngrtran.c:957:1 in png.h
// png_set_strip_16 at pngrtran.c:185:1 in png.h
// png_set_scale_16 at pngrtran.c:171:1 in png.h
// png_start_read_image at pngread.c:291:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_set_expand_16 at pngrtran.c:985:1 in png.h
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
#include "code/png.h"
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <csetjmp>

struct read_state {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    read_state* state = static_cast<read_state*>(png_get_io_ptr(png_ptr));
    if (!state || !state->data) {
        png_error(png_ptr, "No input data");
        return;
    }
    
    if (state->offset + length > state->size) {
        png_error(png_ptr, "Read beyond buffer");
        return;
    }
    
    memcpy(data, state->data + state->offset, length);
    state->offset += length;
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    static const uint8_t png_header[] = {137, 80, 78, 71, 13, 10, 26, 10};
    if (memcmp(Data, png_header, 8) != 0) {
        return 0;
    }
    
    read_state state = {Data, Size, 0};
    
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    png_set_read_fn(png_ptr, &state, user_read_data);
    
    png_read_info(png_ptr, info_ptr);
    
    int choice = Data[0] % 6;
    switch (choice) {
        case 0:
            png_set_expand_gray_1_2_4_to_8(png_ptr);
            break;
        case 1:
            png_set_strip_16(png_ptr);
            break;
        case 2:
            png_set_scale_16(png_ptr);
            break;
        case 3:
            png_start_read_image(png_ptr);
            break;
        case 4:
            png_set_expand(png_ptr);
            break;
        case 5:
            png_set_expand_16(png_ptr);
            break;
    }
    
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    return 0;
}