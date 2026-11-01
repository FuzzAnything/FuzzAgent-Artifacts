// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_free at pngmem.c:234:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc_default at pngmem.c:190:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_get_chunk_malloc_max at pngget.c:1325:1 in png.h
// png_malloc_warn at pngmem.c:213:1 in png.h
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
#include <cstddef>

// Suppress deprecation warnings for png_malloc_default if compiling with strict flags
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

static void dummy_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // In a real app, this would log and abort. For fuzzing, we just return.
    // Note: png_malloc calls this, which usually aborts. We can't easily override
    // the abort behavior without complex setup, so we rely on the fuzzer
    // catching crashes or timeouts if png_error is triggered.
}

static void dummy_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set custom error/warning callbacks to prevent immediate aborts during fuzzing
    // Note: png_malloc still calls png_error which typically aborts. 
    // We cannot fully suppress the abort from png_error without modifying libpng source,
    // but setting callbacks is the standard way to handle errors.
    png_set_error_fn(png_ptr, NULL, dummy_error_callback, dummy_warning_callback);

    // Extract data from input
    png_alloc_size_t size = 0;
    png_alloc_size_t max_limit = 0;

    if (Size >= sizeof(png_alloc_size_t)) {
        memcpy(&size, Data, sizeof(png_alloc_size_t));
        Data += sizeof(png_alloc_size_t);
        Size -= sizeof(png_alloc_size_t);
    }

    if (Size >= sizeof(png_alloc_size_t)) {
        memcpy(&max_limit, Data, sizeof(png_alloc_size_t));
        Data += sizeof(png_alloc_size_t);
        Size -= sizeof(png_alloc_size_t);
    }

    // Avoid excessively large allocations that might OOM the fuzzer process immediately
    if (size > 1024 * 1024 * 10) { // 10MB limit for fuzzing safety
        size = 1024 * 1024 * 10;
    }
    
    // Cap max_limit to reasonable values for testing
    if (max_limit > 1024 * 1024 * 100) {
        max_limit = 1024 * 1024 * 100;
    }

    // 1. Test png_set_chunk_malloc_max
    png_set_chunk_malloc_max(png_ptr, max_limit);

    // 2. Test png_get_chunk_malloc_max
    png_alloc_size_t current_max = png_get_chunk_malloc_max(png_ptr);
    (void)current_max; // Use variable to avoid unused warning

    // 3. Test png_malloc_warn
    // This should not abort on failure, just return NULL and warn
    void *ptr_warn = png_malloc_warn(png_ptr, size);
    if (ptr_warn != NULL) {
        // Free manually since png_free is not available in this specific snippet context
        // but typically png_malloc_warn memory should be freed with png_free.
        // However, png_free is not in the target list. We use free() as a fallback
        // if we assume standard malloc was used, but strictly libpng memory should be freed with png_free.
        // Since we don't have png_free in the target list, we rely on the fact that 
        // png_malloc_warn might use the default handler or a custom one.
        // To be safe and avoid leaks in the fuzzer loop, we try to free if we can.
        // Note: In a real scenario, we would call png_free(png_ptr, ptr_warn).
        // Since png_free is not listed, we assume the fuzzer environment handles memory
        // or we skip freeing to avoid linking errors if png_free isn't exposed.
        // Actually, png_free is standard. Let's assume it exists.
        png_free(png_ptr, ptr_warn);
    }

    // 4. Test png_malloc
    // This calls png_error on failure, which usually aborts. 
    // The fuzzer will catch this as a crash. We try with a small size first to avoid immediate OOM.
    void *ptr_malloc = png_malloc(png_ptr, size);
    if (ptr_malloc != NULL) {
        png_free(png_ptr, ptr_malloc);
    }

    // 5. Test png_calloc
    void *ptr_calloc = png_calloc(png_ptr, size);
    if (ptr_calloc != NULL) {
        png_free(png_ptr, ptr_calloc);
    }

    // 6. Test png_malloc_default (Deprecated)
    void *ptr_default = png_malloc_default(png_ptr, size);
    if (ptr_default != NULL) {
        // png_malloc_default uses system malloc directly, so we use free()
        free(ptr_default);
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}

#ifdef _MSC_VER
#pragma warning(default : 4996)
#else
#pragma GCC diagnostic pop
#endif