// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_user_limits at pngset.c:1937:1 in png.h
// png_set_user_limits at pngset.c:1937:1 in png.h
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_option at png.c:3740:1 in png.h
// png_set_option at png.c:3740:1 in png.h
// png_set_option at png.c:3740:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_get_chunk_cache_max at pngget.c:1318:1 in png.h
// png_get_chunk_cache_max at pngget.c:1318:1 in png.h
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

// Global state for the custom read callback
static const uint8_t* g_fuzz_data = NULL;
static size_t g_fuzz_size = 0;
static size_t g_fuzz_offset = 0;

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Custom read function that reads from the fuzz input buffer
static void custom_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    if (g_fuzz_offset + length > g_fuzz_size) {
        size_t available = g_fuzz_size - g_fuzz_offset;
        if (available == 0) return;
        memcpy(data, g_fuzz_data + g_fuzz_offset, available);
        g_fuzz_offset += available;
        return;
    }
    memcpy(data, g_fuzz_data + g_fuzz_offset, length);
    g_fuzz_offset += length;
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling for setjmp/longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Initialize IO (we will override this later for the read_info test)
    png_init_io(png_ptr, NULL); 

    // Fuzz png_set_option
    if (Size > 0) {
        int option = (Data[0] % 100) * 2; // Ensure even
        int onoff = (Data[0] & 1);
        png_set_option(png_ptr, option, onoff);
        
        // Try invalid option (odd)
        png_set_option(png_ptr, option + 1, onoff);
        
        // Try NULL png_ptr
        png_set_option(NULL, option, onoff);
    }

    // Fuzz png_set_chunk_cache_max
    if (Size > 4) {
        png_uint_32 cache_max = (png_uint_32)(Data[1] << 24 | Data[2] << 16 | Data[3] << 8 | Data[4]);
        png_set_chunk_cache_max(png_ptr, cache_max);
        
        // Try NULL
        png_set_chunk_cache_max(NULL, cache_max);
    }

    // Fuzz png_get_chunk_cache_max
    png_uint_32 retrieved_max = png_get_chunk_cache_max(png_ptr);
    (void)retrieved_max; 
    png_uint_32 null_max = png_get_chunk_cache_max(NULL);
    (void)null_max;

    // Fuzz png_set_user_limits
    // Ensure we have enough data before accessing indices 9-12
    if (Size > 12) {
        png_uint_32 width_max = (png_uint_32)(Data[5] << 24 | Data[6] << 16 | Data[7] << 8 | Data[8]);
        png_uint_32 height_max = (png_uint_32)(Data[9] << 24 | Data[10] << 16 | Data[11] << 8 | Data[12]);
        png_set_user_limits(png_ptr, width_max, height_max);
        
        // Try NULL
        png_set_user_limits(NULL, width_max, height_max);
    }

    // Fuzz png_set_chunk_malloc_max
    // Ensure we have enough data before accessing indices 13-16
    if (Size > 16) {
        png_alloc_size_t malloc_max = (png_alloc_size_t)(Data[13] << 24 | Data[14] << 16 | Data[15] << 8 | Data[16]);
        png_set_chunk_malloc_max(png_ptr, malloc_max);
        
        // Try NULL
        png_set_chunk_malloc_max(NULL, malloc_max);
    }

    // Fuzz png_read_info with custom data
    // We need a fresh struct for this to avoid state issues with the previous init_io
    png_structp png_ptr2 = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr2) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    png_infop info_ptr2 = png_create_info_struct(png_ptr2);
    if (!info_ptr2) {
        png_destroy_read_struct(&png_ptr2, NULL, NULL);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Set up error handling for the second struct
    if (setjmp(png_jmpbuf(png_ptr2))) {
        png_destroy_read_struct(&png_ptr2, &info_ptr2, NULL);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Set up the custom read callback with the fuzz data
    g_fuzz_data = Data;
    g_fuzz_size = Size;
    g_fuzz_offset = 0;
    
    png_set_read_fn(png_ptr2, NULL, custom_read_fn);

    // Call png_read_info. If the data is not a valid PNG, png_error will be called,
    // which triggers the longjmp to the setjmp block above.
    png_read_info(png_ptr2, info_ptr2);

    // If we reach here, the read was successful (or at least didn't trigger an error).
    // Cleanup the second struct
    png_destroy_read_struct(&png_ptr2, &info_ptr2, NULL);

    // Cleanup the first struct
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return 0;
}