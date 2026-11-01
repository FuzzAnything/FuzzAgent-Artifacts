// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_text_compression_mem_level at pngwrite.c:1284:1 in png.h
// png_set_benign_errors at pngset.c:1994:1 in png.h
// png_set_compression_level at pngwrite.c:1185:1 in png.h
// png_set_compression_window_bits at pngwrite.c:1224:1 in png.h
// png_set_text_compression_window_bits at pngwrite.c:1309:1 in png.h
// png_reset_zstream at png.c:951:1 in png.h
#include <png.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csetjmp>
#include <cstdint>
#include <cstddef>

static void write_dummy_png_file(const uint8_t *data, size_t size) {
    if (!data || size == 0) return;
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

static void cleanup_png_struct(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    } else if (info_ptr) {
        png_destroy_read_struct(NULL, &info_ptr, NULL);
    }
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file if data is provided
    write_dummy_png_file(Data, Size);

    // Initialize libpng write structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        remove("./dummy_file");
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        remove("./dummy_file");
        return 0;
    }

    // Use setjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        cleanup_png_struct(png_ptr, info_ptr);
        remove("./dummy_file");
        return 0;
    }

    // Initialize IO (using dummy file)
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        cleanup_png_struct(png_ptr, info_ptr);
        remove("./dummy_file");
        return 0;
    }
    png_init_io(png_ptr, fp);
    fclose(fp);

    // Extract values from fuzz input to drive API calls safely
    // Ensure we have enough data before accessing indices
    int mem_level = 1;
    int benign_errors = 0;
    int compression_level = 0;
    int window_bits = 8;

    if (Size > 0) mem_level = (Data[0] % 9) + 1; // 1-9
    if (Size > 1) benign_errors = (Data[1] % 2); // 0 or 1
    if (Size > 2) compression_level = Data[2] % 10; // 0-9
    if (Size > 3) window_bits = 8 + (Data[3] % 8); // 8-15

    // Call target API functions
    png_set_text_compression_mem_level(png_ptr, mem_level);
    png_set_benign_errors(png_ptr, benign_errors);
    png_set_compression_level(png_ptr, compression_level);
    png_set_compression_window_bits(png_ptr, window_bits);
    png_set_text_compression_window_bits(png_ptr, window_bits);

    // Test png_reset_zstream (deprecated, but still callable)
    int reset_result = png_reset_zstream(png_ptr);
    (void)reset_result; // Suppress unused warning

    // Cleanup
    cleanup_png_struct(png_ptr, info_ptr);

    // Clean up dummy file
    remove("./dummy_file");

    return 0;
}