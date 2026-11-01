// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_user_transform_info at pngtrans.c:805:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_read_user_chunk_fn at pngset.c:1830:1 in png.h
// png_get_user_chunk_ptr at pngget.c:1272:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
#include <png.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <setjmp.h>
#include <cstdint>
#include <cstddef>

static int user_chunk_callback(png_structp png_ptr, png_unknown_chunkp chunk) {
    (void)png_ptr;
    if (!chunk) {
        return 0;
    }
    // Access the chunk type using the correct member name 'name'
    if (chunk->name[0] == 'C') {
        return -1; // Error
    }
    if (chunk->name[0] == 'S') {
        return 1; // Discard
    }
    return 0; // Save
}

static void dummy_error(png_structp png_ptr, const char* error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do not abort, just return to allow fuzzing to continue
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void dummy_warning(png_structp png_ptr, const char* warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Do nothing
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file to simulate input
    const char* dummy_file = "./dummy_file";
    FILE* fp = fopen(dummy_file, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Create read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set custom error handlers to prevent aborts during fuzzing
    png_set_error_fn(png_ptr, nullptr, dummy_error, dummy_warning);

    // Set user chunk callback
    void* user_chunk_data = const_cast<uint8_t*>(Data);
    png_set_read_user_chunk_fn(png_ptr, user_chunk_data, user_chunk_callback);

    // Test png_get_user_chunk_ptr
    void* retrieved_chunk_ptr = png_get_user_chunk_ptr(png_ptr);
    (void)retrieved_chunk_ptr;

    // Test png_get_error_ptr
    void* error_ptr = png_get_error_ptr(png_ptr);
    (void)error_ptr;

    // Test png_get_mem_ptr
    void* mem_ptr = png_get_mem_ptr(png_ptr);
    (void)mem_ptr;

    // Test png_get_user_transform_ptr
    void* transform_ptr = png_get_user_transform_ptr(png_ptr);
    (void)transform_ptr;

    // Attempt to open the file for reading
    fp = fopen(dummy_file, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Test png_get_io_ptr (retrieves the pointer set by png_init_io)
    void* retrieved_io_ptr = png_get_io_ptr(png_ptr);
    (void)retrieved_io_ptr;

    // Set user transform pointer for testing
    void* custom_transform_data = const_cast<uint8_t*>(Data);
    // png_set_user_transform_info takes (png_ptr, user_transform_ptr, transform_mask, num_transforms)
    // We pass 0 for mask and num_transforms to just set the pointer.
    png_set_user_transform_info(png_ptr, custom_transform_data, 0, 0);
    void* retrieved_transform_ptr = png_get_user_transform_ptr(png_ptr);
    (void)retrieved_transform_ptr;

    // Try to read the PNG (may fail if data is invalid, which is expected in fuzzing)
    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
        png_read_info(png_ptr, info_ptr);
        // We don't need to read the full image for this fuzz target,
        // as the focus is on the user chunk and pointer retrieval functions.
    }

    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}