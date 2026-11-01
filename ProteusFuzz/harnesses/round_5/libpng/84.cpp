// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_set_invert_mono at pngtrans.c:249:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
// png_set_packswap at pngtrans.c:69:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_set_palette_to_rgb at pngrtran.c:945:1 in png.h
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
#include <cstdio>
#include <cstring>
#include <vector>

static void write_dummy_png(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    } else if (info_ptr) {
        // If png_ptr is null but info_ptr exists, try to destroy info (though typically they go together)
        png_destroy_info_struct(nullptr, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input data to a dummy file to simulate a PNG file
    write_dummy_png(Data, Size);

    // Create read struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling to prevent crashes during fuzzing
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Initialize I/O
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the header and info
    // We use png_set_sig_bytes to handle cases where the file might not start with a signature
    // or to skip some bytes if the fuzzer provides partial data.
    // However, for robustness, we try to read the info.
    // If the file is not a valid PNG, png_read_info will throw a longjmp.
    
    // To ensure we test the API functions even on invalid files (if they are called before read_info),
    // we can call some functions immediately after struct creation.
    
    // 1. png_set_swap_alpha
    png_set_swap_alpha(png_ptr);

    // 2. png_set_invert_mono
    png_set_invert_mono(png_ptr);

    // 3. png_set_swap
    png_set_swap(png_ptr);

    // 4. png_set_packswap
    png_set_packswap(png_ptr);

    // Try to read info. This might fail for random data, triggering the setjmp handler.
    // If it succeeds, we can call the functions that require info.
    png_read_info(png_ptr, info_ptr);

    // 5. png_set_gray_to_rgb (requires read_info)
    png_set_gray_to_rgb(png_ptr);

    // 6. png_set_palette_to_rgb (requires read_info)
    png_set_palette_to_rgb(png_ptr);

    // Clean up
    fclose(fp);
    cleanup_png(png_ptr, info_ptr);

    return 0;
}