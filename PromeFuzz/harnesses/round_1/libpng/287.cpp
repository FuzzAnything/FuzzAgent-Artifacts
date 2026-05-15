// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_first_frame_is_hidden at pngget.c:1509:1 in png.h
// png_get_next_frame_dispose_op at pngget.c:1489:1 in png.h
// png_permit_mng_features at pngset.c:1622:1 in png.h
// png_get_next_frame_x_offset at pngget.c:1449:1 in png.h
// png_get_acTL at pngget.c:1361:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_num_frames at pngget.c:1379:1 in png.h
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
#include "code/png.h"

struct buffer_data {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    buffer_data* buf = static_cast<buffer_data*>(png_get_io_ptr(png_ptr));
    
    if (buf == nullptr || buf->data == nullptr) {
        memset(data, 0, length);
        return;
    }
    
    size_t to_read = length;
    if (buf->pos + to_read > buf->size) {
        to_read = buf->size - buf->pos;
    }
    
    if (to_read > 0) {
        memcpy(data, buf->data + buf->pos, to_read);
        buf->pos += to_read;
    }
    
    // Fill remaining with zeros if needed
    if (to_read < length) {
        memset(data + to_read, 0, length - to_read);
    }
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Error handler - do nothing to allow fuzzing to continue
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Warning handler - do nothing
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_287(const uint8_t *Data, size_t Size) {
    // Write data to dummy file as requested
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                  dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return 0;
    }
    
    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Set up custom read function with buffer data
    buffer_data buf = {Data, Size, 0};
    png_set_read_fn(png_ptr, &buf, dummy_read_fn);
    
    // Try to read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Call target API functions
    
    // 1. png_get_num_frames
    png_uint_32 num_frames = png_get_num_frames(png_ptr, info_ptr);
    (void)num_frames;
    
    // 2. png_get_first_frame_is_hidden
    png_byte first_hidden = png_get_first_frame_is_hidden(png_ptr, info_ptr);
    (void)first_hidden;
    
    // 3. png_get_next_frame_dispose_op
    png_byte dispose_op = png_get_next_frame_dispose_op(png_ptr, info_ptr);
    (void)dispose_op;
    
    // 4. png_permit_mng_features - use some bits from input data
    png_uint_32 mng_features = 0;
    if (Size > 0) {
        mng_features = Data[0];
        if (Size > 1) {
            mng_features |= (Data[1] << 8);
        }
        if (Size > 2) {
            mng_features |= (Data[2] << 16);
        }
        if (Size > 3) {
            mng_features |= (Data[3] << 24);
        }
    }
    png_uint_32 applied_features = png_permit_mng_features(png_ptr, mng_features);
    (void)applied_features;
    
    // 5. png_get_next_frame_x_offset
    png_uint_32 x_offset = png_get_next_frame_x_offset(png_ptr, info_ptr);
    (void)x_offset;
    
    // 6. png_get_acTL
    png_uint_32 acTL_frames = 0;
    png_uint_32 acTL_plays = 0;
    png_uint_32 acTL_result = png_get_acTL(png_ptr, info_ptr, &acTL_frames, &acTL_plays);
    (void)acTL_result;
    (void)acTL_frames;
    (void)acTL_plays;
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}