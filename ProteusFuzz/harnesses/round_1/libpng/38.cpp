// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_get_chunk_malloc_max at pngget.c:1325:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_info_init_3 at png.c:433:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_malloc_warn at pngmem.c:213:1 in png.h
// png_free at pngmem.c:234:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <cstring>

static void dummy_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void setup_png_struct(png_structp *png_ptr) {
    if (png_ptr == nullptr) return;
    *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error, dummy_warning);
    if (*png_ptr != nullptr) {
        // PNG_ERROR_ACTION_WARNING might not be defined in all versions, 
        // so we rely on the default behavior or omit setting it if undefined.
        // If the macro is missing, this line is removed by preprocessor or we skip it.
        #ifdef PNG_ERROR_ACTION_WARNING
        png_set_error_action(*png_ptr, PNG_ERROR_ACTION_WARNING);
        #endif
    }
}

static void cleanup_png_struct(png_structp *png_ptr) {
    if (png_ptr != nullptr && *png_ptr != nullptr) {
        png_destroy_read_struct(png_ptr, nullptr, nullptr);
        *png_ptr = nullptr;
    }
}

static void cleanup_png_info(png_structp png_ptr, png_infopp info_ptr) {
    if (info_ptr != nullptr && *info_ptr != nullptr) {
        png_destroy_info_struct(png_ptr, info_ptr);
        *info_ptr = nullptr;
    }
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = nullptr;
    png_infopp info_ptr = nullptr;
    png_info *info = nullptr;

    setup_png_struct(&png_ptr);
    if (png_ptr == nullptr) {
        return 0;
    }

    info = png_create_info_struct(png_ptr);
    if (info == nullptr) {
        cleanup_png_struct(&png_ptr);
        return 0;
    }
    info_ptr = &info;

    // Fuzz png_info_init_3 (Deprecated)
    // Since png_info is opaque, we cannot use sizeof(png_info) directly.
    // We pass the size of the pointer or a small constant to trigger internal logic.
    // The function expects the size of the structure as it was compiled in libpng.
    // We use a safe default size or 0 to let it handle reallocation if needed.
    // However, passing 0 or a very small size might trigger the reallocation path.
    size_t struct_size = 0;
    if (Size > 0) {
        // Use input data to vary the size, but keep it reasonable to avoid immediate crash
        // if the library checks for minimums.
        struct_size = Data[0];
        if (struct_size > 1024) struct_size = 1024;
    }
    png_info_init_3(info_ptr, struct_size);

    // Fuzz png_get_rowbytes
    size_t rowbytes = png_get_rowbytes(png_ptr, info);
    (void)rowbytes;

    // Fuzz png_malloc_warn
    png_alloc_size_t alloc_size_warn = 0;
    if (Size > 1) {
        alloc_size_warn = (png_alloc_size_t)(Data[1] * 256 + Data[0]);
        if (alloc_size_warn == 0) alloc_size_warn = 1;
        if (alloc_size_warn > 1024 * 1024) alloc_size_warn = 1024 * 1024;
    }
    void *mem_warn = png_malloc_warn(png_ptr, alloc_size_warn);
    if (mem_warn != nullptr) {
        png_free(png_ptr, mem_warn);
    }

    // Fuzz png_malloc
    png_alloc_size_t alloc_size = 0;
    if (Size > 2) {
        alloc_size = (png_alloc_size_t)(Data[2] * 256 + Data[1]);
        if (alloc_size == 0) alloc_size = 1;
        if (alloc_size > 1024 * 1024) alloc_size = 1024 * 1024;
    }
    void *mem = nullptr;
    // png_malloc calls png_error on failure, which might abort.
    // The fuzzer will treat an abort as a crash, which is expected behavior for this test.
    mem = png_malloc(png_ptr, alloc_size);
    if (mem != nullptr) {
        png_free(png_ptr, mem);
    }

    // Fuzz png_get_chunk_malloc_max
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max(png_ptr);
    (void)chunk_max;

    // Fuzz png_calloc
    png_alloc_size_t calloc_size = 0;
    if (Size > 3) {
        calloc_size = (png_alloc_size_t)(Data[3] * 256 + Data[2]);
        if (calloc_size == 0) calloc_size = 1;
        if (calloc_size > 1024 * 1024) calloc_size = 1024 * 1024;
    }
    void *mem_calloc = png_calloc(png_ptr, calloc_size);
    if (mem_calloc != nullptr) {
        png_free(png_ptr, mem_calloc);
    }

    // Cleanup
    cleanup_png_info(png_ptr, info_ptr);
    cleanup_png_struct(&png_ptr);

    return 0;
}