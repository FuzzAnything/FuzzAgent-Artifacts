// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_get_chunk_cache_max at pngget.c:1318:1 in png.h
// png_set_keep_unknown_chunks at pngset.c:1665:1 in png.h
// png_get_io_chunk_type at pngget.c:1340:1 in png.h
// png_set_read_user_chunk_fn at pngset.c:1830:1 in png.h
// png_get_user_chunk_ptr at pngget.c:1272:1 in png.h
// png_handle_as_unknown at png.c:903:1 in png.h
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

extern "C" {
#include "code/png.h"
}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    return png_ptr;
}

static void cleanup_png_struct(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
}

static int dummy_user_chunk_callback(png_structp png_ptr, png_unknown_chunkp chunk) {
    (void)png_ptr;
    (void)chunk;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_267(const uint8_t *Data, size_t Size) {
    if (Size < 9) {
        return 0;
    }

    png_structp png_ptr = create_png_struct();
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png_struct(png_ptr, info_ptr);
        return 0;
    }

    size_t offset = 0;
    
    png_uint_32 chunk_cache_max = png_get_chunk_cache_max(png_ptr);
    (void)chunk_cache_max;

    if (Size - offset >= 5) {
        int keep = Data[offset] % 3;
        offset++;
        
        int num_chunks = (Size - offset) / 5;
        if (num_chunks > 0) {
            png_set_keep_unknown_chunks(png_ptr, keep, Data + offset, num_chunks);
            offset += num_chunks * 5;
        }
    }

    png_uint_32 io_chunk_type = png_get_io_chunk_type(png_ptr);
    (void)io_chunk_type;

    void* user_data = reinterpret_cast<void*>(const_cast<uint8_t*>(Data));
    png_set_read_user_chunk_fn(png_ptr, user_data, dummy_user_chunk_callback);

    void* retrieved_user_data = png_get_user_chunk_ptr(png_ptr);
    (void)retrieved_user_data;

    if (Size - offset >= 4) {
        png_byte chunk_name[4];
        memcpy(chunk_name, Data + offset, 4);
        int handle_result = png_handle_as_unknown(png_ptr, chunk_name);
        (void)handle_result;
    }

    cleanup_png_struct(png_ptr, info_ptr);
    return 0;
}