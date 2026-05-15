// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_acTL at pngset.c:1318:1 in png.h
// png_get_acTL at pngget.c:1361:1 in png.h
// png_get_num_frames at pngget.c:1379:1 in png.h
// png_get_num_frames at pngget.c:1379:1 in png.h
// png_get_next_frame_height at pngget.c:1439:1 in png.h
// png_get_next_frame_height at pngget.c:1439:1 in png.h
// png_get_next_frame_x_offset at pngget.c:1449:1 in png.h
// png_get_next_frame_x_offset at pngget.c:1449:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_permit_mng_features at pngset.c:1622:1 in png.h
// png_permit_mng_features at pngset.c:1622:1 in png.h
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
#include <fstream>

#include "png.h"

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy read function that does nothing
}

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function that does nothing
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Dummy flush function that does nothing
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Dummy error handler
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Dummy warning handler
}

extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    std::ofstream outfile("./dummy_file", std::ios::binary);
    if (outfile.is_open()) {
        outfile.write(reinterpret_cast<const char*>(Data), Size);
        outfile.close();
    } else {
        return 0;
    }

    // Initialize png_struct for reading
    png_structp png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                      dummy_error_fn, dummy_warning_fn);
    if (!png_ptr_read) {
        return 0;
    }

    // Initialize png_info for reading
    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        return 0;
    }

    // Initialize png_struct for writing
    png_structp png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr,
                                                       dummy_error_fn, dummy_warning_fn);
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }

    // Initialize png_info for writing
    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, nullptr);
        return 0;
    }

    // Set up custom I/O functions for reading
    png_set_read_fn(png_ptr_read, nullptr, dummy_read_fn);
    
    // Set up custom I/O functions for writing
    png_set_write_fn(png_ptr_write, nullptr, dummy_write_fn, dummy_flush_fn);

    // Try to read PNG signature from dummy data
    if (Size >= 8) {
        png_set_sig_bytes(png_ptr_read, 8);
    }

    // Test png_permit_mng_features with random bitmask
    png_uint_32 mng_features = 0;
    if (Size >= sizeof(png_uint_32)) {
        memcpy(&mng_features, Data, sizeof(png_uint_32));
    }
    png_permit_mng_features(png_ptr_read, mng_features);
    png_permit_mng_features(png_ptr_write, mng_features);

    // Test png_set_acTL with random frame and play counts
    png_uint_32 num_frames = 0;
    png_uint_32 num_plays = 0;
    
    if (Size >= 2 * sizeof(png_uint_32)) {
        memcpy(&num_frames, Data, sizeof(png_uint_32));
        memcpy(&num_plays, Data + sizeof(png_uint_32), sizeof(png_uint_32));
    }
    
    png_set_acTL(png_ptr_write, info_ptr_write, num_frames, num_plays);

    // Test png_get_acTL
    png_uint_32 retrieved_frames = 0;
    png_uint_32 retrieved_plays = 0;
    png_get_acTL(png_ptr_write, info_ptr_write, &retrieved_frames, &retrieved_plays);

    // Test png_get_num_frames
    png_uint_32 frame_count = png_get_num_frames(png_ptr_read, info_ptr_read);
    frame_count = png_get_num_frames(png_ptr_write, info_ptr_write);

    // Test png_get_next_frame_height
    png_uint_32 next_height = png_get_next_frame_height(png_ptr_read, info_ptr_read);
    next_height = png_get_next_frame_height(png_ptr_write, info_ptr_write);

    // Test png_get_next_frame_x_offset
    png_uint_32 x_offset = png_get_next_frame_x_offset(png_ptr_read, info_ptr_read);
    x_offset = png_get_next_frame_x_offset(png_ptr_write, info_ptr_write);

    // Clean up
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);

    return 0;
}