// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_unknown_chunks at pngget.c:1249:1 in png.h
// png_set_unknown_chunk_location at pngset.c:1592:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_keep_unknown_chunks at pngset.c:1665:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_set_read_user_chunk_fn at pngset.c:1830:1 in png.h
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
#include <png.h>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <cstddef>

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
    // Do nothing for fuzzing
}

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
    // Do nothing for fuzzing
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do nothing, just avoid crash
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Do nothing
}

static int dummy_user_chunk_fn(png_structp png_ptr, png_unknown_chunkp chunk) {
    (void)png_ptr;
    (void)chunk;
    // Return 0 to save chunk, or 1 to discard
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up dummy I/O functions
    png_set_read_fn(png_ptr, nullptr, dummy_read_fn);
    png_set_write_fn(png_ptr, nullptr, dummy_write_fn, nullptr);

    // Use input data to drive fuzzing logic
    int mode = Data[0] % 4; // 0, 1, 2, 3 for different keep modes
    int location = (Data[0] >> 2) % 4; // 0, 1, 2, 3 for different locations
    int chunk_index = (Data[0] >> 4) % 10;
    uint32_t cache_max = (Data[0] >> 6) | ((Size > 1 ? Data[1] : 0) << 2);

    // Fuzz png_set_keep_unknown_chunks
    png_byte chunk_list[4] = {0};
    if (Size >= 4) {
        memcpy(chunk_list, Data, 4);
    }
    png_set_keep_unknown_chunks(png_ptr, mode, chunk_list, Size >= 4 ? 1 : 0);

    // Fuzz png_set_chunk_cache_max
    png_set_chunk_cache_max(png_ptr, cache_max);

    // Fuzz png_set_read_user_chunk_fn
    png_set_read_user_chunk_fn(png_ptr, (void*)Data, dummy_user_chunk_fn);

    // Fuzz png_handle_as_unknown
    if (Size >= 4) {
        png_byte chunk_name[5];
        memcpy(chunk_name, Data, 4);
        chunk_name[4] = '\0';
        png_handle_as_unknown(png_ptr, chunk_name);
    }

    // Attempt to read minimal info to populate unknown chunks if possible
    // We use a dummy buffer to simulate reading
    std::vector<png_byte> dummy_buffer(1024, 0);
    if (Size > 0) {
        memcpy(dummy_buffer.data(), Data, std::min(Size, (size_t)1024));
    }

    // Set read function to use dummy buffer
    png_set_read_fn(png_ptr, &dummy_buffer, dummy_read_fn);

    // Try to read info (may fail due to invalid data, which is expected)
    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
        png_read_info(png_ptr, info_ptr);
    }

    // Fuzz png_get_unknown_chunks
    png_unknown_chunkp unknowns = nullptr;
    int num_unknown = png_get_unknown_chunks(png_ptr, info_ptr, &unknowns);
    if (num_unknown > 0 && unknowns != nullptr) {
        // Fuzz png_set_unknown_chunk_location for each chunk
        for (int i = 0; i < num_unknown; ++i) {
            if (i < chunk_index) {
                png_set_unknown_chunk_location(png_ptr, info_ptr, i, location);
            }
        }
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}