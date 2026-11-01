// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_free at pngmem.c:234:1 in png.h
// png_malloc_warn at pngmem.c:213:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_info_init_3 at png.c:433:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file for potential I/O operations if needed
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Initialize error handling to prevent crashes from png_error
    png_error_ptr error_fn = nullptr;
    png_error_ptr warn_fn = nullptr;
    void *error_ptr = nullptr;
    void *mem_ptr = nullptr;
    png_malloc_ptr malloc_fn = nullptr;
    png_free_ptr free_fn = nullptr;

    // 1. png_create_read_struct_2
    png_structp png_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        error_ptr,
        error_fn,
        warn_fn,
        mem_ptr,
        malloc_fn,
        free_fn
    );

    if (!png_ptr) {
        return 0;
    }

    // 2. png_info_init_3 (Deprecated but tested as per request)
    // png_info is opaque, so we cannot use sizeof(png_info).
    // We must create the structure using png_create_info_struct or pass a pointer 
    // to a pre-allocated block if the API allows, but png_info_init_3 expects 
    // a pointer to a pointer that it will initialize.
    // Since png_info is opaque, we cannot allocate it manually with png_malloc 
    // and pass the address. Instead, we should use png_create_info_struct 
    // to get a valid png_info*, then pass its address to png_info_init_3 
    // if we strictly want to test that function's behavior on an existing struct.
    // However, png_info_init_3 is designed to initialize a struct passed by reference.
    // The safest way to get a valid png_info* for testing is png_create_info_struct.
    
    png_infop info_struct = png_create_info_struct(png_ptr);
    if (!info_struct) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    png_infopp info_ptr = &info_struct;
    
    // Use fuzz data to determine struct size, but ensure it's reasonable
    size_t struct_size = 0;
    if (Size >= sizeof(size_t)) {
        memcpy(&struct_size, Data, sizeof(size_t));
        // Cap size to prevent huge allocations
        if (struct_size > 1024 * 1024) struct_size = 1024 * 1024;
    } else {
        // Use a default size if fuzz data is insufficient
        struct_size = 1024; 
    }

    // Initialize info structure
    // Note: png_info_init_3 expects a pointer to a pointer. 
    // Since info_struct is already allocated by png_create_info_struct, 
    // passing its address to png_info_init_3 might cause double allocation 
    // or reallocation issues depending on implementation. 
    // However, for fuzzing purposes, we invoke it as requested.
    // We cast to png_infopp to match the signature.
    png_info_init_3(info_ptr, struct_size);
    
    // After init_3, info_struct might have been reallocated, so use *info_ptr
    png_infop current_info = *info_ptr;

    // 3. png_get_rowbytes
    // This function takes png_const_inforp (const png_info*), so we pass current_info
    size_t rowbytes = png_get_rowbytes(png_ptr, current_info);
    (void)rowbytes; // Suppress unused variable warning

    // 4. png_malloc
    png_alloc_size_t malloc_size = 0;
    if (Size >= sizeof(png_alloc_size_t)) {
        memcpy(&malloc_size, Data + sizeof(size_t), sizeof(png_alloc_size_t));
        // Cap size
        if (malloc_size > 1024 * 1024) malloc_size = 1024 * 1024;
    } else {
        malloc_size = 64;
    }

    void *malloc_buf = png_malloc(png_ptr, malloc_size);
    if (malloc_buf) {
        // Write some data to the buffer to ensure it's usable
        memset(malloc_buf, 0xAB, malloc_size);
        png_free(png_ptr, malloc_buf);
    }

    // 5. png_malloc_warn
    void *malloc_warn_buf = png_malloc_warn(png_ptr, malloc_size);
    if (malloc_warn_buf) {
        memset(malloc_warn_buf, 0xCD, malloc_size);
        png_free(png_ptr, malloc_warn_buf);
    }

    // 6. png_calloc
    void *calloc_buf = png_calloc(png_ptr, malloc_size);
    if (calloc_buf) {
        // Verify it is zeroed
        // (Optional: check first few bytes)
        png_free(png_ptr, calloc_buf);
    }

    // Cleanup
    // Destroy the info struct first
    if (current_info) {
        png_destroy_info_struct(png_ptr, &current_info);
    }
    
    // Destroy the png_struct
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);

    return 0;
}