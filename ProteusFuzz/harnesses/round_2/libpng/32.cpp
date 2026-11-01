// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
// png_get_num_frames at pngget.c:1379:1 in png.h
// png_read_frame_head at pngread.c:195:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
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
#include <png.h>
#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>
#include <cstring>
#include <cstdio>

// Fuzzing entry point
extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file for reading
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Reopen for reading
    fp = fopen(filename, "rb");
    if (!fp) return 0;

    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return 0;
    }

    // Set up error handling with longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, fp);

    // Try to read the PNG info
    png_read_info(png_ptr, info_ptr);

    // Call png_get_IHDR to retrieve header info
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);

    // Call png_read_update_info to prepare for row reading
    png_read_update_info(png_ptr, info_ptr);

    // Try to read the full PNG image
    png_read_png(png_ptr, info_ptr, 0, NULL);

    // Try to get number of frames (for APNG)
    png_uint_32 num_frames = png_get_num_frames(png_ptr, info_ptr);

    // Try to read frame head (for APNG)
    if (num_frames > 1) {
        png_read_frame_head(png_ptr, info_ptr);
    }

    // Finalize reading
    png_read_end(png_ptr, info_ptr);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    // Remove dummy file
    remove(filename);

    return 0;
}