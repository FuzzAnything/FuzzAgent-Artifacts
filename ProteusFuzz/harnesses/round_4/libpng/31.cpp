// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_set_keep_unknown_chunks at pngset.c:1665:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_unknown_chunks at pngget.c:1249:1 in png.h
// png_handle_as_unknown at png.c:903:1 in png.h
// png_handle_as_unknown at png.c:903:1 in png.h
// png_set_unknown_chunk_location at pngset.c:1592:1 in png.h
// png_set_unknown_chunk_location at pngset.c:1592:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
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
#include <setjmp.h>
#include <stdint.h>
#include <stddef.h>

// Dummy file path as per requirements
static const char* DUMMY_FILE = "./dummy_file";

// Global jump buffer for error handling
static jmp_buf fuzzer_jmp_buf;

// Custom error handler that jumps back to the fuzzer instead of aborting
static void dummy_error(png_structp png_ptr, const char* error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(fuzzer_jmp_buf, 1);
}

static void dummy_warning(png_structp png_ptr, const char* warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    return;
}

// Helper to create a minimal valid PNG file for reading
static bool create_dummy_png(const uint8_t* data, size_t size) {
    FILE* f = fopen(DUMMY_FILE, "wb");
    if (!f) return false;

    // Write PNG signature
    const uint8_t png_sig[] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(png_sig, 1, 8, f);

    // Write a minimal IHDR chunk (13 bytes data)
    uint8_t ihdr_data[13] = {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0};
    uint32_t ihdr_len = 13;
    uint8_t ihdr_type[] = {'I', 'H', 'D', 'R'};
    
    fwrite(&ihdr_len, 4, 1, f);
    fwrite(ihdr_type, 1, 4, f);
    fwrite(ihdr_data, 1, 13, f);
    uint32_t ihdr_crc = 0; 
    fwrite(&ihdr_crc, 4, 1, f);

    // Write a minimal IDAT chunk (1 byte data to stop read_info)
    uint8_t idat_data[1] = {0};
    uint32_t idat_len = 1;
    uint8_t idat_type[] = {'I', 'D', 'A', 'T'};
    
    fwrite(&idat_len, 4, 1, f);
    fwrite(idat_type, 1, 4, f);
    fwrite(idat_data, 1, 1, f);
    uint32_t idat_crc = 0;
    fwrite(&idat_crc, 4, 1, f);

    // Append fuzz data as a custom chunk if available
    if (size > 0) {
        uint32_t chunk_len = (size > 100) ? 100 : size;
        if (chunk_len > 0) {
            uint8_t custom_type[] = {'F', 'U', 'Z', 'Z'};
            fwrite(&chunk_len, 4, 1, f);
            fwrite(custom_type, 1, 4, f);
            fwrite(data, 1, chunk_len, f);
            uint32_t custom_crc = 0;
            fwrite(&custom_crc, 4, 1, f);
        }
    }

    fclose(f);
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy PNG file to read from
    if (!create_dummy_png(Data, Size)) {
        return 0;
    }

    FILE* fp = fopen(DUMMY_FILE, "rb");
    if (!fp) {
        remove(DUMMY_FILE);
        return 0;
    }

    // Create read struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error, dummy_warning);
    if (!png_ptr) {
        fclose(fp);
        remove(DUMMY_FILE);
        return 0;
    }

    // Create info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        remove(DUMMY_FILE);
        return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, fp);

    // Set up error handling with setjmp
    if (setjmp(fuzzer_jmp_buf) != 0) {
        // An error occurred, jump back here
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        remove(DUMMY_FILE);
        return 0;
    }

    // Test 1: png_set_chunk_cache_max
    png_uint_32 cache_max = 0;
    if (Size > 0) {
        cache_max = Data[0] * 100; 
        if (cache_max == 0) cache_max = 1;
    }
    png_set_chunk_cache_max(png_ptr, cache_max);

    // Test 2: png_set_keep_unknown_chunks
    int keep_mode = 0;
    const png_byte* chunk_list = nullptr;
    int num_chunks = 0;
    
    if (Size > 0) {
        keep_mode = Data[0] % 4; 
    }
    
    if (Size >= 4) {
        chunk_list = Data;
        num_chunks = 1;
    }
    png_set_keep_unknown_chunks(png_ptr, keep_mode, chunk_list, num_chunks);

    // Test 3: png_read_info
    // This will read the headers from the dummy file
    png_read_info(png_ptr, info_ptr);

    // Test 4: png_get_unknown_chunks
    png_unknown_chunkp unknowns = nullptr;
    int count = png_get_unknown_chunks(png_ptr, info_ptr, &unknowns);

    // Test 5: png_handle_as_unknown
    const png_byte chunk_name[] = {'t', 'E', 'S', 'T'};
    if (Size >= 4) {
        const png_byte* fuzz_name = Data;
        png_handle_as_unknown(png_ptr, fuzz_name);
    }
    png_handle_as_unknown(png_ptr, chunk_name);

    // Test 6: png_set_unknown_chunk_location
    if (count > 0) {
        int location = 0;
        if (Size > 0) {
            location = Data[0] % 3; 
        }
        png_set_unknown_chunk_location(png_ptr, info_ptr, 0, location);
        
        // Try with an invalid index
        png_set_unknown_chunk_location(png_ptr, info_ptr, 999, location);
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    remove(DUMMY_FILE);

    return 0;
}