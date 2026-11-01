// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_handle_as_unknown at png.c:903:1 in png.h
// png_set_unknown_chunk_location at pngset.c:1592:1 in png.h
// png_get_unknown_chunks at pngget.c:1249:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_set_keep_unknown_chunks at pngset.c:1665:1 in png.h
// png_set_read_user_chunk_fn at pngset.c:1830:1 in png.h
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
#include <vector>
#include <cstdlib>

// Callback function for png_set_read_user_chunk_fn
static int
user_chunk_callback(png_structp png_ptr, png_unknown_chunkp chunk) {
    (void)png_ptr;
    (void)chunk;
    // Return 0 to save the chunk, or 1 to discard. 
    // Returning -1 would trigger an error.
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file to simulate input
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize libpng read structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Use setjmp/longjmp for error handling as required by libpng
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Initialize the dummy file pointer for reading
    fp = fopen(dummy_file, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // --- Fuzzing Logic ---

    // 1. png_set_chunk_cache_max
    // Extract a value from the input to determine cache max
    png_uint_32 cache_max = 0;
    if (Size >= 4) {
        cache_max = *(const png_uint_32*)(Data);
    }
    png_set_chunk_cache_max(png_ptr, cache_max);

    // 2. png_set_keep_unknown_chunks
    // Use input bytes to determine keep mode and chunk list
    int keep_mode = 0;
    if (Size > 0) {
        keep_mode = Data[Size > 1 ? 1 : 0] % 5; // 0 to 4
    }
    
    // Create a dummy chunk list if we have enough data
    std::vector<png_byte> chunk_list;
    if (Size > 10) {
        chunk_list.assign(Data + 2, Data + std::min(Size, 10UL));
    }
    
    // Ensure chunk_list is null-terminated if used as a string, 
    // but png_set_keep_unknown_chunks expects a list of 4-byte names.
    // We pass a pointer to the vector or NULL.
    const png_byte *chunk_ptr = chunk_list.empty() ? NULL : chunk_list.data();
    int num_chunks = chunk_list.empty() ? 0 : (int)(chunk_list.size() / 4);
    
    png_set_keep_unknown_chunks(png_ptr, keep_mode, chunk_ptr, num_chunks);

    // 3. png_set_read_user_chunk_fn
    // Register the callback
    void *user_data = (void*)(Data + 10); // Arbitrary user pointer from input
    png_set_read_user_chunk_fn(png_ptr, user_data, user_chunk_callback);

    // 4. png_handle_as_unknown
    // Try to check a chunk name from input
    if (Size >= 4) {
        const png_byte *chunk_name = Data;
        int result = png_handle_as_unknown(png_ptr, chunk_name);
        (void)result; // Use result to avoid unused warning
    }

    // 5. png_set_unknown_chunk_location
    // This requires a valid chunk index. Since we haven't read any chunks yet,
    // the index will likely be invalid, which is fine for fuzzing (it should handle errors).
    // We try indices 0 and 1.
    int chunk_index = (Size > 2) ? Data[2] : 0;
    int location = (Size > 3) ? Data[3] % 4 : 0; // 0 to 3
    
    // Note: This function might fail if no unknown chunks are registered yet, 
    // which is expected behavior for fuzzing.
    png_set_unknown_chunk_location(png_ptr, info_ptr, chunk_index, location);

    // 6. png_get_unknown_chunks
    // Retrieve unknown chunks. Since we haven't read the image yet, this should return 0.
    png_unknown_chunkp unknowns = NULL;
    int count = png_get_unknown_chunks(png_ptr, info_ptr, &unknowns);
    (void)count;
    (void)unknowns;

    // Attempt to read info to trigger more internal logic (optional but good for coverage)
    // We expect this to fail on the dummy file unless it's a valid PNG, which is fine.
    // The setjmp handler above will catch errors.
    png_read_info(png_ptr, info_ptr);

    // Cleanup
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    // Remove dummy file
    remove(dummy_file);

    return 0;
}