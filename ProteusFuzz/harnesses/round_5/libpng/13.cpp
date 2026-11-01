// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_set_user_limits at pngset.c:1937:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_get_chunk_cache_max at pngget.c:1318:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
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
#include <cstddef>
#include <cstring>
#include <cstdio>

static void *fuzz_malloc(png_structp png_ptr, png_size_t size) {
    if (size == 0) return NULL;
    void *ptr = malloc(size);
    if (ptr) memset(ptr, 0, size);
    return ptr;
}

static void fuzz_free(png_structp png_ptr, png_voidp ptr) {
    if (ptr) free(ptr);
}

static void fuzz_error(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing or log if needed
}

static void fuzz_warn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing or log if needed
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a dummy file to simulate input
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize png_struct with custom handlers
    png_structp png_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        NULL,
        fuzz_error,
        fuzz_warn,
        NULL,
        fuzz_malloc,
        fuzz_free
    );

    if (!png_ptr) {
        remove(dummy_file);
        return 0;
    }

    // Create info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        remove(dummy_file);
        return 0;
    }

    // Setup error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        remove(dummy_file);
        return 0;
    }

    // Initialize IO
    fp = fopen(dummy_file, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        remove(dummy_file);
        return 0;
    }
    png_init_io(png_ptr, fp);

    // Fuzz various configuration functions
    png_uint_32 cache_max = 0;
    png_alloc_size_t malloc_max = 0;
    png_uint_32 width_max = 0;
    png_uint_32 height_max = 0;

    // Extract values from fuzz input if available
    if (Size >= 4) {
        cache_max = *(const png_uint_32 *)(Data);
    }
    if (Size >= 8) {
        malloc_max = *(const png_alloc_size_t *)(Data + 4);
    }
    if (Size >= 12) {
        width_max = *(const png_uint_32 *)(Data + 8);
    }
    if (Size >= 16) {
        height_max = *(const png_uint_32 *)(Data + 12);
    }

    // Call target API functions
    png_set_chunk_cache_max(png_ptr, cache_max);
    png_set_chunk_malloc_max(png_ptr, malloc_max);
    png_set_user_limits(png_ptr, width_max, height_max);
    png_set_mem_fn(png_ptr, NULL, fuzz_malloc, fuzz_free);

    // Retrieve cache max to ensure state consistency
    png_uint_32 retrieved_cache_max = png_get_chunk_cache_max(png_ptr);
    (void)retrieved_cache_max; // Suppress unused warning

    // Attempt to read info (may fail due to invalid data, which is expected)
    png_read_info(png_ptr, info_ptr);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    remove(dummy_file);

    return 0;
}