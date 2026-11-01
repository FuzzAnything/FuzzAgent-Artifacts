// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc_warn at pngmem.c:213:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
// png_info_init_3 at png.c:433:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstdio>

// Custom error function that does not abort
static void custom_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // In a real scenario, we might log, but for fuzzing we just return to avoid abort
}

// Custom warning function
static void custom_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    // Create the read struct
    png_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        NULL,
        custom_error,
        custom_warning,
        NULL,
        NULL,
        NULL
    );

    if (png_ptr == NULL) {
        return 0;
    }

    // Create the info struct
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Extract data for fuzzing
    size_t size_val = 0;
    if (Size >= 2) {
        size_val = (Data[0] << 8) | Data[1];
        // Cap size to prevent excessive allocation or overflow in tests
        if (size_val > 1024 * 1024) {
            size_val = 1024 * 1024;
        }
    }

    // Test 1: png_malloc
    void *malloc_ptr = png_malloc(png_ptr, size_val);
    if (malloc_ptr != NULL) {
        free(malloc_ptr);
    }

    // Test 2: png_malloc_warn
    void *malloc_warn_ptr = png_malloc_warn(png_ptr, size_val);
    if (malloc_warn_ptr != NULL) {
        free(malloc_warn_ptr);
    }

    // Test 3: png_calloc
    void *calloc_ptr = png_calloc(png_ptr, size_val);
    if (calloc_ptr != NULL) {
        free(calloc_ptr);
    }

    // Test 4: png_info_init_3 (Deprecated)
    // Since png_info is opaque in newer libpng versions, we cannot use sizeof(png_info).
    // The function expects the size of the structure as it was defined at the time of compilation.
    // However, png_create_info_struct already initializes the struct correctly.
    // To satisfy the API call without crashing due to size mismatch or incomplete type:
    // We can pass a dummy size. The function reallocates if the size is too small.
    // A safe approach is to pass 0 or a small value, letting the function handle reallocation,
    // or simply skip this call if the struct is already initialized.
    // Given the deprecation and opacity, passing 0 is often used in fuzzing to trigger reallocation logic safely.
    // Note: png_info_init_3 takes png_infopp, so we pass &info_ptr.
    png_info_init_3(&info_ptr, 0);

    // Test 5: png_get_rowbytes
    size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    (void)rowbytes; // Use the value to avoid unused warning

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}