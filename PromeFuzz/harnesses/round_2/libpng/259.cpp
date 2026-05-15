// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_acTL at pngget.c:1361:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_acTL at pngset.c:1318:1 in png.h
// png_permit_mng_features at pngset.c:1622:1 in png.h
// png_get_num_frames at pngget.c:1379:1 in png.h
// png_get_next_frame_x_offset at pngget.c:1449:1 in png.h
// png_get_next_frame_height at pngget.c:1439:1 in png.h
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
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "code/png.h"

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing - dummy write function
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Do nothing - dummy flush function
}

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing - dummy read function
}

static void error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing on error
}

static void warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing on warning
}

extern "C" int LLVMFuzzerTestOneInput_259(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    // Create write structures
    png_structp png_write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                       nullptr, error_fn, warning_fn);
    if (!png_write_ptr) {
        return 0;
    }
    
    png_infop info_write_ptr = png_create_info_struct(png_write_ptr);
    if (!info_write_ptr) {
        png_destroy_write_struct(&png_write_ptr, nullptr);
        return 0;
    }
    
    // Create read structures
    png_structp png_read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                                     nullptr, error_fn, warning_fn);
    if (!png_read_ptr) {
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        return 0;
    }
    
    png_infop info_read_ptr = png_create_info_struct(png_read_ptr);
    if (!info_read_ptr) {
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        return 0;
    }
    
    // Set up write functions
    png_set_write_fn(png_write_ptr, nullptr, dummy_write_fn, dummy_flush_fn);
    
    // Set up read functions
    png_set_read_fn(png_read_ptr, nullptr, dummy_read_fn);
    
    // Extract parameters from fuzz data
    png_uint_32 num_frames = *reinterpret_cast<const png_uint_32*>(Data);
    png_uint_32 num_plays = *reinterpret_cast<const png_uint_32*>(Data + 4);
    png_uint_32 mng_features = *reinterpret_cast<const png_uint_32*>(Data + 8);
    png_uint_32 dummy_param = *reinterpret_cast<const png_uint_32*>(Data + 12);
    
    // Test png_set_acTL
    png_set_acTL(png_write_ptr, info_write_ptr, num_frames, num_plays);
    
    // Test png_permit_mng_features
    png_permit_mng_features(png_read_ptr, mng_features);
    
    // Test png_get_num_frames
    png_uint_32 frame_count = png_get_num_frames(png_read_ptr, info_read_ptr);
    
    // Test png_get_next_frame_x_offset
    png_uint_32 x_offset = png_get_next_frame_x_offset(png_read_ptr, info_read_ptr);
    
    // Test png_get_next_frame_height
    png_uint_32 frame_height = png_get_next_frame_height(png_read_ptr, info_read_ptr);
    
    // Test png_get_acTL
    png_uint_32 retrieved_frames, retrieved_plays;
    png_uint_32 actl_result = png_get_acTL(png_read_ptr, info_read_ptr, 
                                          &retrieved_frames, &retrieved_plays);
    
    // Clean up
    png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
    png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
    
    return 0;
}