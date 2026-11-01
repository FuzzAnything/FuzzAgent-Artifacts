// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_option at png.c:3740:1 in png.h
// png_set_option at png.c:3740:1 in png.h
// png_set_benign_errors at pngset.c:1994:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_set_check_for_invalid_index at pngset.c:2024:1 in png.h
// png_set_interlace_handling at pngtrans.c:98:1 in png.h
// png_reset_zstream at png.c:951:1 in png.h
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
#include <png.h>
#include <cstdint>
#include <cstring>
#include <vector>

static void cleanup_png_struct(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
}

static void write_dummy_png_file(const char* filename, const uint8_t* data, size_t size) {
    FILE* f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0; // Minimum size for basic PNG header

    // Create a dummy file for reading
    const char* dummy_file = "./dummy_file";
    write_dummy_png_file(dummy_file, Data, Size);

    // Initialize libpng read structures
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Create read struct
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    // Create info struct
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        cleanup_png_struct(png_ptr, nullptr);
        return 0;
    }

    // Use setjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png_struct(png_ptr, info_ptr);
        return 0;
    }

    // Open the dummy file
    FILE* fp = fopen(dummy_file, "rb");
    if (!fp) {
        cleanup_png_struct(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Extract fuzz data for API arguments
    int option = Data[0] & 0x3F; // Limit option index
    int onoff = (Data[1] & 0x01) ? 1 : 0;
    int benign_allowed = (Data[2] & 0x01) ? 1 : 0;
    png_uint_32 cache_max = (png_uint_32)(Data[3] | (Data[4] << 8) | (Data[5] << 16) | (Data[6] << 24));
    int check_invalid = (Data[7] & 0x01) ? 1 : 0;

    // Fuzz png_set_option
    // Ensure option is even as per spec for valid behavior, but fuzz odd too to test error handling
    png_set_option(png_ptr, option, onoff);
    png_set_option(png_ptr, option | 1, onoff); // Force odd index

    // Fuzz png_set_benign_errors
    png_set_benign_errors(png_ptr, benign_allowed);

    // Fuzz png_set_chunk_cache_max
    png_set_chunk_cache_max(png_ptr, cache_max);

    // Fuzz png_set_check_for_invalid_index
    png_set_check_for_invalid_index(png_ptr, check_invalid);

    // Fuzz png_set_interlace_handling
    // This must be called before png_read_update_info
    int passes = png_set_interlace_handling(png_ptr);
    (void)passes; // Suppress unused variable warning

    // Fuzz png_reset_zstream
    // This is deprecated but we test it
    png_reset_zstream(png_ptr);

    // Attempt to read info to trigger more state
    // This might fail due to invalid data, which is expected in fuzzing
    png_read_info(png_ptr, info_ptr);

    // Cleanup
    fclose(fp);
    cleanup_png_struct(png_ptr, info_ptr);

    return 0;
}