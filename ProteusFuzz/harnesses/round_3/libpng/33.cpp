// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_set_acTL at pngset.c:1318:1 in png.h
// png_set_next_frame_fcTL at pngset.c:1357:1 in png.h
// png_write_frame_head at pngwrite.c:1501:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_frame_tail at pngwrite.c:1529:1 in png.h
// png_write_chunk_start at pngwutil.c:132:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_longjmp at pngerror.c:663:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
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
#include <setjmp.h>

// Custom error handler to prevent libpng from calling exit()
static void
fuzz_png_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Use png_longjmp to safely jump out of libpng
    png_longjmp(png_ptr, 1);
}

// Custom warning handler
static void
fuzz_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Custom write function that discards data
static void
fuzz_png_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

// Helper to initialize png_struct and png_info
static png_structp
init_png_struct(png_infop *info_ptr) {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return NULL;

    *info_ptr = png_create_info_struct(png_ptr);
    if (!*info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return NULL;
    }

    // Set custom error/warning handlers to avoid exit()
    png_set_error_fn(png_ptr, NULL, fuzz_png_error, fuzz_png_warning);
    png_set_write_fn(png_ptr, NULL, fuzz_png_write_data, NULL);

    return png_ptr;
}

// Helper to clean up
static void
cleanup_png(png_structp *png_ptr, png_infop *info_ptr) {
    if (*png_ptr) {
        png_destroy_write_struct(png_ptr, info_ptr);
        *png_ptr = NULL;
        *info_ptr = NULL;
    }
}

// Helper to read 16-bit integer from data safely
static png_uint_16
read_uint16(const uint8_t *data, size_t offset, size_t size) {
    if (offset + 2 > size) return 0;
    return (data[offset] << 8) | data[offset + 1];
}

// Helper to read 32-bit integer from data safely
static png_uint_32
read_uint32(const uint8_t *data, size_t offset, size_t size) {
    if (offset + 4 > size) return 0;
    return (png_uint_32)data[offset] << 24 |
           (png_uint_32)data[offset + 1] << 16 |
           (png_uint_32)data[offset + 2] << 8 |
           (png_uint_32)data[offset + 3];
}

// Helper to read single byte safely
static png_byte
read_byte(const uint8_t *data, size_t offset, size_t size) {
    if (offset >= size) return 0;
    return data[offset];
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    // Minimum size check to ensure we can read basic parameters
    if (Size < 40) return 0;

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_byte **row_pointers = NULL;
    size_t num_rows = 0;
    size_t row_size = 0;
    bool initialized = false;

    // Parse parameters from input data
    png_uint_32 width = read_uint32(Data, 0, Size);
    png_uint_32 height = read_uint32(Data, 4, Size);
    png_uint_32 bit_depth = read_uint32(Data, 8, Size);
    png_uint_32 color_type = read_uint32(Data, 12, Size);

    // Limit dimensions to prevent OOM and ensure valid ranges
    if (width == 0 || width > 2048) width = 256;
    if (height == 0 || height > 2048) height = 256;
    
    // Validate bit_depth (1, 2, 4, 8, 16)
    if (bit_depth != 1 && bit_depth != 2 && bit_depth != 4 && bit_depth != 8 && bit_depth != 16) {
        bit_depth = 8;
    }

    // Validate color_type (0, 2, 3, 4, 6)
    if (color_type != 0 && color_type != 2 && color_type != 3 && color_type != 4 && color_type != 6) {
        color_type = 2; // RGB
    }

    // Initialize PNG structures
    png_ptr = init_png_struct(&info_ptr);
    if (!png_ptr || !info_ptr) return 0;

    initialized = true;

    // Use setjmp to catch libpng errors during initialization
    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
        // Initialize info structure first to set dimensions
        png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, 
                     PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

        // Calculate row bytes using the correct function signature
        row_size = png_get_rowbytes(png_ptr, info_ptr);
        
        // Initialize row pointers if we have enough data
        if (row_size > 0 && row_size < 1024 * 1024) { // Limit row size
            num_rows = (Size - 40) / row_size;
            if (num_rows > height) num_rows = height;
            
            if (num_rows > 0) {
                row_pointers = (png_byte **)malloc(num_rows * sizeof(png_byte *));
                if (!row_pointers) return 0;
                
                for (size_t i = 0; i < num_rows; i++) {
                    row_pointers[i] = (png_byte *)malloc(row_size);
                    if (!row_pointers[i]) {
                        for (size_t j = 0; j < i; j++) free(row_pointers[j]);
                        free(row_pointers);
                        row_pointers = NULL;
                        num_rows = 0;
                        break;
                    }
                    // Fill with data from input
                    size_t data_offset = 40 + i * row_size;
                    size_t copy_size = (data_offset + row_size <= Size) ? row_size : (Size - data_offset);
                    if (copy_size > 0) {
                        memcpy(row_pointers[i], Data + data_offset, copy_size);
                    }
                }
            }
        }

        // Call png_set_acTL (APNG animation control)
        png_uint_32 num_frames = read_uint32(Data, 16, Size) % 1000 + 1;
        png_uint_32 num_plays = read_uint32(Data, 20, Size) % 1000 + 1;
        png_set_acTL(png_ptr, info_ptr, num_frames, num_plays);

        // Call png_set_next_frame_fcTL
        png_uint_32 frame_width = width;
        png_uint_32 frame_height = height;
        png_uint_32 x_offset = read_uint32(Data, 24, Size) % width;
        png_uint_32 y_offset = read_uint32(Data, 28, Size) % height;
        png_uint_16 delay_num = read_uint16(Data, 32, Size) % 1000;
        png_uint_16 delay_den = read_uint16(Data, 34, Size) % 1000 + 1;
        png_byte dispose_op = read_byte(Data, 36, Size) % 3;
        png_byte blend_op = read_byte(Data, 37, Size) % 2;

        png_set_next_frame_fcTL(png_ptr, info_ptr, frame_width, frame_height, 
                                x_offset, y_offset, delay_num, delay_den, 
                                dispose_op, blend_op);

        // Call png_write_frame_head
        if (row_pointers && num_rows > 0) {
            png_write_frame_head(png_ptr, info_ptr, row_pointers, 
                                 frame_width, frame_height, x_offset, y_offset,
                                 delay_num, delay_den, dispose_op, blend_op);
        }

        // Call png_write_rows
        if (row_pointers && num_rows > 0) {
            png_write_rows(png_ptr, row_pointers, num_rows);
        }

        // Call png_write_frame_tail
        if (row_pointers && num_rows > 0) {
            png_write_frame_tail(png_ptr, info_ptr);
        }

        // Call png_write_chunk_start
        png_byte chunk_name[4] = {'t', 'E', 'X', 't'};
        png_uint_32 chunk_length = read_uint32(Data, 38, Size) % 1024;
        png_write_chunk_start(png_ptr, chunk_name, chunk_length);
    }

    // Cleanup
    if (row_pointers) {
        for (size_t i = 0; i < num_rows; i++) {
            if (row_pointers[i]) free(row_pointers[i]);
        }
        free(row_pointers);
    }

    if (initialized) {
        cleanup_png(&png_ptr, &info_ptr);
    } else {
        if (png_ptr) png_destroy_write_struct(&png_ptr, &info_ptr);
    }

    return 0;
}