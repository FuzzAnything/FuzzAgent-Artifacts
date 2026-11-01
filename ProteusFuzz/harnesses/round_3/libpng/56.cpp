// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

static void*
safe_malloc(size_t size) {
    if (size == 0 || size > 1024 * 1024) return NULL;
    void* ptr = malloc(size);
    if (ptr) memset(ptr, 0, size);
    return ptr;
}

static void
cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    } else if (info_ptr) {
        // If png_ptr is null but info_ptr exists, we can't safely destroy via png_destroy_read_struct
        // In a real scenario, info_ptr should not exist without png_ptr in a read context.
        // We rely on the caller to manage this or accept that this path is unreachable in valid usage.
        // For fuzzing safety, we do nothing here to avoid double-free or invalid free.
    }
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file for file-based reading if needed, though we will primarily use memory reading
    // to avoid file system overhead and race conditions.
    // However, the prompt mentions using "./dummy_file" if a file is required.
    // We will attempt to read from memory first as it's more robust for fuzzing.
    
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    
    // Initialize PNG read structure
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling to prevent crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Initialize the info structure with the input data
    // We use png_set_read_fn with a memory buffer
    png_bytep* row_pointers = NULL;
    
    // Try to read the PNG data from the fuzz input
    // We use a simple approach: pass the data directly to png_read_info
    // Note: png_read_info expects a file pointer or custom I/O function.
    // We will use a custom I/O function for memory reading.
    
    static uint8_t* input_buffer = NULL;
    static size_t input_size = 0;
    static size_t input_offset = 0;
    
    // We cannot use static buffers safely for concurrent fuzzing without locks, 
    // but for a single-threaded fuzzer, this is acceptable.
    // However, to be safer and cleaner, we allocate per call.
    
    // Re-implementing a simple memory reader approach
    // Since we can't easily pass state to the read function without a struct,
    // we will use the standard approach of creating a file if the prompt insists,
    // OR we can use png_set_read_fn with a custom callback.
    
    // Let's use the file approach as requested by the prompt logic "If the code requires a file..."
    // Writing data to a dummy file.
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    png_set_read_fn(png_ptr, fp, NULL);

    // Read the header and info
    png_read_info(png_ptr, info_ptr);
    
    // Now invoke the target API functions
    
    // 1. png_get_gAMA_fixed
    png_fixed_point gamma_value = 0;
    png_uint_32 gama_result = png_get_gAMA_fixed(png_ptr, info_ptr, &gamma_value);
    (void)gama_result; // Suppress unused warning
    
    // 2. png_get_valid
    png_uint_32 valid_result = png_get_valid(png_ptr, info_ptr, PNG_INFO_gAMA);
    (void)valid_result;
    
    // 3. png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)x_ppm;
    
    // 4. png_get_hIST
    png_uint_16* hist_ptr = NULL;
    png_uint_32 hist_result = png_get_hIST(png_ptr, info_ptr, &hist_ptr);
    if (hist_result && hist_ptr) {
        // Just access a few bytes to ensure pointer is valid
        // No need to free hist_ptr, it's owned by info_ptr
    }
    (void)hist_result;
    
    // 5. png_get_image_width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    (void)width;
    
    // 6. png_get_uint_16
    // This function takes a byte buffer. We can use the input data if Size >= 2.
    if (Size >= 2) {
        png_uint_16 val = png_get_uint_16(Data);
        (void)val;
    }
    
    // Cleanup
    fclose(fp);
    cleanup_png(png_ptr, info_ptr);
    
    return 0;
}