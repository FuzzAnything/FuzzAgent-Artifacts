// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_malloc_default at pngmem.c:190:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_get_chunk_malloc_max at pngget.c:1325:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc_warn at pngmem.c:213:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
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
#include <cstdlib>

static void dummy_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do nothing to prevent abort in fuzzing
}

static void dummy_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Do nothing to prevent output in fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error, dummy_warning);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Initialize a dummy file to avoid file-related errors if needed
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Test png_set_chunk_malloc_max
    png_alloc_size_t max_size = 0;
    if (Size > 0) {
        max_size = Data[0];
    }
    if (Size > 1) {
        max_size = (Data[0] << 8) | Data[1];
    }
    if (Size > 2) {
        max_size = (Data[0] << 16) | (Data[1] << 8) | Data[2];
    }
    if (Size > 3) {
        max_size = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
    }

    png_set_chunk_malloc_max(png_ptr, max_size);

    // Test png_get_chunk_malloc_max
    png_alloc_size_t retrieved_max = png_get_chunk_malloc_max(png_ptr);
    (void)retrieved_max;

    // Test png_malloc
    png_alloc_size_t alloc_size = 0;
    if (Size > 4) {
        alloc_size = (Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7];
    }
    // Cap size to avoid excessive allocation
    if (alloc_size > 1024 * 1024) {
        alloc_size = 1024 * 1024;
    }

    void *ptr_malloc = png_malloc(png_ptr, alloc_size);
    if (ptr_malloc) {
        free(ptr_malloc);
    }

    // Test png_malloc_warn
    void *ptr_malloc_warn = png_malloc_warn(png_ptr, alloc_size);
    if (ptr_malloc_warn) {
        free(ptr_malloc_warn);
    }

    // Test png_calloc
    void *ptr_calloc = png_calloc(png_ptr, alloc_size);
    if (ptr_calloc) {
        free(ptr_calloc);
    }

    // Test png_malloc_default (deprecated but still callable)
    void *ptr_malloc_default = png_malloc_default(png_ptr, alloc_size);
    if (ptr_malloc_default) {
        free(ptr_malloc_default);
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    // Remove dummy file
    remove(dummy_file);

    return 0;
}