// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_free at pngmem.c:234:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
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

static void *custom_malloc(png_structp png_ptr, png_alloc_size_t size) {
    (void)png_ptr;
    if (size == 0) {
        return nullptr;
    }
    void *ptr = malloc(size);
    if (ptr == nullptr && size > 0) {
        png_error(png_ptr, "Out of memory");
    }
    return ptr;
}

static void custom_free(png_structp png_ptr, void *ptr) {
    (void)png_ptr;
    if (ptr != nullptr) {
        free(ptr);
    }
}

static void custom_error(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
}

static void custom_warning(png_structp png_ptr, png_const_charp warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    uint32_t size_to_alloc = *reinterpret_cast<const uint32_t*>(Data);
    if (size_to_alloc > 1024 * 1024) {
        size_to_alloc = 1024 * 1024;
    }

    FILE *dummy_file = fopen("./dummy_file", "w");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    void *user_mem_ptr = (void*)0x12345678;

    png_structp png_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        custom_error,
        custom_warning,
        user_mem_ptr,
        custom_malloc,
        custom_free
    );

    if (png_ptr == nullptr) {
        return 0;
    }

    void *retrieved_ptr = png_get_mem_ptr(png_ptr);
    (void)retrieved_ptr;

    png_set_mem_fn(png_ptr, user_mem_ptr, custom_malloc, custom_free);

    void *allocated_ptr = png_malloc(png_ptr, size_to_alloc);
    
    if (allocated_ptr != nullptr) {
        memset(allocated_ptr, 0xAB, size_to_alloc);
        png_free(png_ptr, allocated_ptr);
        // Do not double free here; libpng's png_free handles NULL, but calling it twice on a valid pointer 
        // that was already freed causes the ASan error.
        // allocated_ptr is now logically invalid, though the pointer value remains.
    }

    // Test png_free_default
    void *ptr_for_default_free = malloc(64);
    if (ptr_for_default_free != nullptr) {
        png_free_default(png_ptr, ptr_for_default_free);
    }
    png_free_default(png_ptr, nullptr);

    // Test with NULL png_ptr
    void *null_mem_ptr = png_get_mem_ptr(nullptr);
    (void)null_mem_ptr;

    void *null_malloc = png_malloc(nullptr, 100);
    (void)null_malloc;

    png_free(nullptr, nullptr);
    png_free_default(nullptr, nullptr);
    png_set_mem_fn(nullptr, nullptr, nullptr, nullptr);

    png_destroy_read_struct(&png_ptr, nullptr, nullptr);

    return 0;
}