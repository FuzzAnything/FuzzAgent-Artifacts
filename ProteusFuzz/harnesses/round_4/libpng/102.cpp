// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_acTL at pngset.c:1318:1 in png.h
// png_get_acTL at pngget.c:1361:1 in png.h
// png_get_num_frames at pngget.c:1379:1 in png.h
// png_get_num_plays at pngget.c:1389:1 in png.h
// png_get_next_frame_width at pngget.c:1429:1 in png.h
// png_get_next_frame_y_offset at pngget.c:1459:1 in png.h
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
#include <cstdio>
#include <cstdlib>

static void static_cleanup(png_structp png_ptr, png_infop info_ptr) {
    if (info_ptr != nullptr) {
        png_destroy_info_struct(png_ptr, &info_ptr);
    }
    if (png_ptr != nullptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create a dummy file to simulate a PNG stream if needed, 
    // though for these specific API calls we mostly need struct initialization.
    // We will write data to a file to potentially trigger read paths if we were reading,
    // but for set/get acTL we can operate on in-memory structs.
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Initialize libpng structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Use setjmp/longjmp to handle libpng errors gracefully
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        static_cleanup(png_ptr, info_ptr);
        return 0;
    }

    // Extract fuzzing parameters from input
    // We use the first few bytes to determine frame counts and play counts
    png_uint_32 num_frames = 0;
    png_uint_32 num_plays = 0;
    
    if (Size >= 4) {
        num_frames = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
    }
    if (Size >= 8) {
        num_plays = Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24);
    }

    // 1. Test png_set_acTL
    // This sets the animation control chunk
    png_set_acTL(png_ptr, info_ptr, num_frames, num_plays);

    // 2. Test png_get_acTL
    png_uint_32 ret_num_frames = 0;
    png_uint_32 ret_num_plays = 0;
    int acTL_result = png_get_acTL(png_ptr, info_ptr, &ret_num_frames, &ret_num_plays);

    // 3. Test png_get_num_frames
    png_uint_32 get_frames = png_get_num_frames(png_ptr, info_ptr);

    // 4. Test png_get_num_plays
    png_uint_32 get_plays = png_get_num_plays(png_ptr, info_ptr);

    // 5. Test png_get_next_frame_width
    // This typically requires fcTL chunks to be present, but calling it tests the pointer validation
    png_uint_32 next_width = png_get_next_frame_width(png_ptr, info_ptr);

    // 6. Test png_get_next_frame_y_offset
    png_uint_32 next_y_offset = png_get_next_frame_y_offset(png_ptr, info_ptr);

    // Cleanup
    static_cleanup(png_ptr, info_ptr);

    // Remove dummy file
    remove("./dummy_file");

    return 0;
}