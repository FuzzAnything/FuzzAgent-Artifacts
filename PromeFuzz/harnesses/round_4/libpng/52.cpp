// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_num_plays at pngget.c:1389:1 in png.h
// png_get_num_frames at pngget.c:1379:1 in png.h
// png_get_next_frame_y_offset at pngget.c:1459:1 in png.h
// png_get_next_frame_width at pngget.c:1429:1 in png.h
// png_get_next_frame_x_offset at pngget.c:1449:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_acTL at pngset.c:1318:1 in png.h
// png_get_num_plays at pngget.c:1389:1 in png.h
// png_get_num_frames at pngget.c:1379:1 in png.h
// png_get_next_frame_y_offset at pngget.c:1459:1 in png.h
// png_get_next_frame_width at pngget.c:1429:1 in png.h
// png_get_next_frame_x_offset at pngget.c:1449:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
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
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "png.h"

struct read_state {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Do nothing
}

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Read from fuzzer input
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) {
        png_error(png_ptr, "No io_ptr");
        return;
    }
    
    read_state* state = static_cast<read_state*>(io_ptr);
    
    if (state->offset + length > state->size) {
        png_error(png_ptr, "Read beyond buffer");
        return;
    }
    
    memcpy(data, state->data + state->offset, length);
    state->offset += length;
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    // Create read structures
    png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!read_ptr) {
        return 0;
    }
    
    png_infop read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Create write structures
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!write_ptr) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        return 0;
    }
    
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(read_ptr))) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        return 0;
    }
    
    if (setjmp(png_jmpbuf(write_ptr))) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        return 0;
    }
    
    // Set up read from fuzzer input
    read_state state = {Data, Size, 0};
    png_set_read_fn(read_ptr, &state, dummy_read_fn);
    
    // Try to read PNG header
    png_read_info(read_ptr, read_info);
    
    // Call getter functions on read structures
    png_uint_32 num_plays = png_get_num_plays(read_ptr, read_info);
    png_uint_32 num_frames = png_get_num_frames(read_ptr, read_info);
    png_uint_32 y_offset = png_get_next_frame_y_offset(read_ptr, read_info);
    png_uint_32 frame_width = png_get_next_frame_width(read_ptr, read_info);
    png_uint_32 x_offset = png_get_next_frame_x_offset(read_ptr, read_info);
    
    // Set up write structures
    png_set_write_fn(write_ptr, nullptr, dummy_write_fn, dummy_flush_fn);
    
    // Set basic image info for writing
    png_set_IHDR(write_ptr, write_info, 
                 100, 100, 8, PNG_COLOR_TYPE_RGB, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, 
                 PNG_FILTER_TYPE_BASE);
    
    // Extract parameters for png_set_acTL from fuzzer input
    png_uint_32 acTL_num_frames = 0;
    png_uint_32 acTL_num_plays = 0;
    
    if (Size >= 16) {
        // Use bytes 8-15 for acTL parameters
        memcpy(&acTL_num_frames, Data + 8, sizeof(png_uint_32));
        memcpy(&acTL_num_plays, Data + 12, sizeof(png_uint_32));
    }
    
    // Call setter function
    png_set_acTL(write_ptr, write_info, acTL_num_frames, acTL_num_plays);
    
    // Call getter functions on write structures too
    png_get_num_plays(write_ptr, write_info);
    png_get_num_frames(write_ptr, write_info);
    png_get_next_frame_y_offset(write_ptr, write_info);
    png_get_next_frame_width(write_ptr, write_info);
    png_get_next_frame_x_offset(write_ptr, write_info);
    
    // Cleanup
    png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    png_destroy_write_struct(&write_ptr, &write_info);
    
    return 0;
}