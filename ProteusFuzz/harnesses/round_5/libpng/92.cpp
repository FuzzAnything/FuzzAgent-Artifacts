// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_info_init_3 at png.c:433:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_get_chunk_malloc_max at pngget.c:1325:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_malloc_warn at pngmem.c:213:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
// png_free at pngmem.c:234:1 in png.h
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

static void dummy_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do not abort in fuzzing context
}

static void dummy_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Do nothing in fuzzing context
}

extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file if needed
    const char* dummy_file = "./dummy_file";
    FILE* f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Initialize png_struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    // Set custom error/warning handlers to prevent aborts during fuzzing
    png_set_error_fn(png_ptr, NULL, dummy_error_callback, dummy_warning_callback);

    // Initialize png_info
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Initialize a second info struct for testing png_info_init_3 if needed
    png_infop info_ptr2 = png_create_info_struct(png_ptr);
    if (!info_ptr2) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Extract size from input data to vary allocation sizes
    png_alloc_size_t alloc_size = 0;
    if (Size >= sizeof(png_alloc_size_t)) {
        memcpy(&alloc_size, Data, sizeof(png_alloc_size_t));
        // Cap size to prevent OOM or excessive memory usage
        if (alloc_size > 1024 * 1024) {
            alloc_size = 1024 * 1024;
        }
    } else {
        alloc_size = Size;
    }

    // Test png_malloc_warn
    void* malloc_warn_result = png_malloc_warn(png_ptr, alloc_size);
    if (malloc_warn_result) {
        png_free(png_ptr, malloc_warn_result);
    }

    // Test png_malloc
    void* malloc_result = png_malloc(png_ptr, alloc_size);
    if (malloc_result) {
        png_free(png_ptr, malloc_result);
    }

    // Test png_calloc
    void* calloc_result = png_calloc(png_ptr, alloc_size);
    if (calloc_result) {
        png_free(png_ptr, calloc_result);
    }

    // Test png_info_init_3 (deprecated, but we test it)
    // Since png_info is opaque in png.h, we pass the size of the pointer itself or a safe small value.
    // The function internally checks the size, but passing sizeof(png_info) fails due to opacity.
    // We pass 0 or a small value to trigger the internal reallocation logic safely or just the pointer.
    // According to libpng source, it expects the size of the struct as known by the library.
    // Since we can't access the real size, we pass 0 to force it to use its internal size or handle the error.
    // However, the API signature requires a size_t. Let's pass 0 which might trigger a reallocation or use default.
    // Actually, passing 0 is often handled by the function to mean "use current size" or similar in some contexts, 
    // but for png_info_init_3 specifically, it expects the size of the struct passed to it.
    // Since we cannot get the real size, we will skip the size check by passing a value that is likely small enough 
    // to trigger the internal "too small" logic if it checks, or just pass 0.
    // A safer approach for fuzzing opaque structs is to pass 0, letting the library handle it if it has a fallback,
    // or just pass the size of the pointer which is definitely smaller than the struct.
    png_info_init_3(&info_ptr2, 0);

    // Test png_get_rowbytes
    size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    (void)rowbytes;

    // Test png_get_chunk_malloc_max
    png_alloc_size_t chunk_malloc_max = png_get_chunk_malloc_max(png_ptr);
    (void)chunk_malloc_max;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, &info_ptr2);

    // Remove dummy file
    remove(dummy_file);

    return 0;
}