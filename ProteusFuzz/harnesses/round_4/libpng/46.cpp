// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_io_ptr at png.c:688:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
// png_get_progressive_ptr at pngpread.c:1136:1 in png.h
// png_get_user_chunk_ptr at pngget.c:1272:1 in png.h
// png_set_read_user_transform_fn at pngrtran.c:1099:1 in png.h
// png_set_progressive_read_fn at pngpread.c:1109:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
// png_get_progressive_ptr at pngpread.c:1136:1 in png.h
// png_get_user_chunk_ptr at pngget.c:1272:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cstdlib>

// Dummy error handler to prevent libpng from calling abort() or exit()
static void
dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void
dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Dummy memory allocation functions
static void *
dummy_malloc_fn(png_structp png_ptr, png_size_t size) {
    (void)png_ptr;
    return malloc(size);
}

static void
dummy_free_fn(png_structp png_ptr, png_voidp ptr) {
    (void)png_ptr;
    if (ptr) {
        free(ptr);
    }
}

// Dummy user transform function
static void
dummy_user_transform_fn(png_structp png_ptr, png_row_infop row_info, png_bytep row) {
    (void)png_ptr;
    (void)row_info;
    (void)row;
}

// Dummy progressive read functions with correct signatures
// info_fn: called after header data fully read
static void
dummy_progressive_info_fn(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
}

// row_fn: called after a prog. row is decoded
static void
dummy_progressive_row_fn(png_structp png_ptr, png_bytep new_row, png_uint_32 row_num, int pass) {
    (void)png_ptr;
    (void)new_row;
    (void)row_num;
    (void)pass;
}

// end_fn: called after all data has been read
// Signature: void (png_structp png_ptr, png_infop info_ptr)
static void
dummy_progressive_end_fn(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file to satisfy potential file-based operations if needed
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    // Test 1: png_create_read_struct_2 with custom handlers
    png_structp png_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        NULL, // error_ptr
        dummy_error_fn,
        dummy_warning_fn,
        NULL, // mem_ptr
        dummy_malloc_fn,
        dummy_free_fn
    );

    if (png_ptr == NULL) {
        return 0;
    }

    // Test 2: png_get_error_ptr (should return NULL initially)
    void *error_ptr = png_get_error_ptr(png_ptr);
    (void)error_ptr;

    // Test 3: png_get_io_ptr (should return NULL initially)
    void *io_ptr = png_get_io_ptr(png_ptr);
    (void)io_ptr;

    // Test 4: png_get_user_transform_ptr (should return NULL initially)
    void *transform_ptr = png_get_user_transform_ptr(png_ptr);
    (void)transform_ptr;

    // Test 5: png_get_progressive_ptr (should return NULL initially)
    void *progressive_ptr = png_get_progressive_ptr(png_ptr);
    (void)progressive_ptr;

    // Test 6: png_get_user_chunk_ptr (should return NULL initially)
    void *chunk_ptr = png_get_user_chunk_ptr(png_ptr);
    (void)chunk_ptr;

    // Set user transform function
    png_set_read_user_transform_fn(png_ptr, dummy_user_transform_fn);
    
    // Set progressive read functions and pointer
    // Correct signature for end_fn is (png_structp, png_infop)
    png_set_progressive_read_fn(png_ptr, (void*)(Data + 1), 
        dummy_progressive_info_fn, 
        dummy_progressive_row_fn, 
        dummy_progressive_end_fn);

    // Re-check pointers after setting
    io_ptr = png_get_io_ptr(png_ptr);
    transform_ptr = png_get_user_transform_ptr(png_ptr);
    progressive_ptr = png_get_progressive_ptr(png_ptr);
    chunk_ptr = png_get_user_chunk_ptr(png_ptr);

    // Cleanup
    png_destroy_read_struct(&png_ptr, NULL, NULL);

    return 0;
}