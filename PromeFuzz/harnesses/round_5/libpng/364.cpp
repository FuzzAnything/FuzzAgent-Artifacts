// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_io_ptr at png.c:688:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_user_chunk_fn at pngset.c:1830:1 in png.h
// png_set_progressive_read_fn at pngpread.c:1109:1 in png.h
// png_get_progressive_ptr at pngpread.c:1136:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "code/png.h"

// Structure to hold memory buffer for reading
struct mem_buffer {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

// Custom read function that reads from memory buffer
static void custom_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Get the user I/O pointer which we set to our buffer structure
    void* io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) {
        png_error(png_ptr, "No I/O pointer");
        return;
    }
    
    // Cast to our buffer structure
    mem_buffer* buf = (mem_buffer*)io_ptr;
    
    // Check if we have enough data
    if (buf->pos + length > buf->size) {
        png_error(png_ptr, "Insufficient data in buffer");
        return;
    }
    
    // Copy data from buffer
    memcpy(data, buf->data + buf->pos, length);
    buf->pos += length;
}

// User chunk callback function
static int user_chunk_callback(png_structp png_ptr, png_unknown_chunkp chunk) {
    // Return 0 to save chunk, 1 to discard, or negative for error
    // For fuzzing, we can randomly decide based on chunk data
    if (chunk->size > 0) {
        // Simple heuristic: if first byte is even, save it
        return (chunk->data[0] % 2 == 0) ? 0 : 1;
    }
    return 0; // Save empty chunks
}

// Progressive read callbacks
static void info_callback(png_structp png_ptr, png_infop info_ptr) {
    // Called when header info is available
}

static void row_callback(png_structp png_ptr, png_bytep new_row, 
                         png_uint_32 row_num, int pass) {
    // Called for each row during progressive reading
}

static void end_callback(png_structp png_ptr, png_infop info_ptr) {
    // Called when image reading is complete
}

extern "C" int LLVMFuzzerTestOneInput_364(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        // Too small to be a valid PNG
        return 0;
    }
    
    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during PNG processing
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Test png_set_read_fn and png_get_io_ptr
    mem_buffer buffer = {Data, Size, 0};
    png_set_read_fn(png_ptr, &buffer, custom_read_fn);
    
    void* io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr != &buffer) {
        // This shouldn't happen, but handle gracefully
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Test png_set_read_user_chunk_fn
    png_set_read_user_chunk_fn(png_ptr, nullptr, user_chunk_callback);
    
    // Test png_set_progressive_read_fn and png_get_progressive_ptr
    void* progressive_data = (void*)0x1234; // Dummy pointer
    png_set_progressive_read_fn(png_ptr, progressive_data, 
                                info_callback, row_callback, end_callback);
    
    void* retrieved_progressive_ptr = png_get_progressive_ptr(png_ptr);
    if (retrieved_progressive_ptr != progressive_data) {
        // Mismatch, but continue fuzzing
    }
    
    // Try to read the PNG header
    png_read_info(png_ptr, info_ptr);
    
    // Get image dimensions
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, 
                 &color_type, &interlace_type, nullptr, nullptr);
    
    // Test png_read_image if we have valid dimensions
    if (width > 0 && height > 0 && width < 10000 && height < 10000) {
        // Calculate row bytes
        png_size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        
        // Allocate row pointers
        png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr, 
                                                         height * sizeof(png_bytep));
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_bytep)png_malloc(png_ptr, row_bytes);
        }
        
        // Try to read the image
        png_read_image(png_ptr, row_pointers);
        
        // Clean up row pointers
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
        png_free(png_ptr, row_pointers);
    }
    
    // Read the rest of the image
    png_read_end(png_ptr, info_ptr);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}