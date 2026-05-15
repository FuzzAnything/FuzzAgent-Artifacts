// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_filter_type at pngget.c:98:1 in png.h
// png_get_interlace_type at pngget.c:107:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" {
#include "code/png.h"
}

struct BufferState {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Read from the user-provided buffer
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) return;
    
    BufferState* state = static_cast<BufferState*>(io_ptr);
    size_t to_read = length;
    if (state->offset + to_read > state->size) {
        to_read = state->size - state->offset;
    }
    
    if (to_read > 0) {
        memcpy(data, state->data + state->offset, to_read);
        state->offset += to_read;
    }
    
    // Fill remaining bytes with zeros if we run out of data
    if (to_read < length) {
        memset(data + to_read, 0, length - to_read);
    }
}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    return png_ptr;
}

static png_infop create_info_struct(png_structp png_ptr) {
    if (!png_ptr) return nullptr;
    return png_create_info_struct(png_ptr);
}

extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    // Check PNG signature
    if (png_sig_cmp((png_const_bytep)Data, 0, 8) != 0) {
        return 0;
    }
    
    png_structp png_ptr = create_png_struct();
    if (!png_ptr) return 0;
    
    png_infop info_ptr = create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Set up custom read function
    BufferState state = {Data, Size, 0};
    png_set_read_fn(png_ptr, &state, dummy_read_fn);
    
    // Skip the signature bytes we already checked
    png_set_sig_bytes(png_ptr, 8);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Call target API functions
    png_byte compression_type = png_get_compression_type(png_ptr, info_ptr);
    (void)compression_type;
    
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_uint_32 result = png_get_IHDR(png_ptr, info_ptr, &width, &height,
                                      &bit_depth, &color_type, &interlace_method,
                                      &compression_method, &filter_method);
    (void)result;
    
    png_byte color_type2 = png_get_color_type(png_ptr, info_ptr);
    (void)color_type2;
    
    png_byte filter_type = png_get_filter_type(png_ptr, info_ptr);
    (void)filter_type;
    
    png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    (void)interlace_type;
    
    png_byte channels = png_get_channels(png_ptr, info_ptr);
    (void)channels;
    
    // Try to read image data if available
    if (Size > 100) {
        png_read_update_info(png_ptr, info_ptr);
        
        png_bytep row = (png_bytep)malloc(png_get_rowbytes(png_ptr, info_ptr));
        if (row) {
            for (png_uint_32 y = 0; y < height && y < 10; y++) {
                png_read_row(png_ptr, row, nullptr);
            }
            free(row);
        }
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}