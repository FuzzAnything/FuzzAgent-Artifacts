// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_rows at pngget.c:50:1 in png.h
// png_progressive_combine_row at pngpread.c:1093:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_progressive_read_fn at pngpread.c:1109:1 in png.h
// png_set_read_status_fn at pngread.c:945:1 in png.h
// png_set_progressive_frame_fn at pngpread.c:1125:1 in png.h
// png_process_data at pngpread.c:50:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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
#include <csetjmp>
#include <cstdio>
#include <cstring>

#include "code/png.h"

// Static callback functions to avoid name conflicts
static void info_callback(png_structp png_ptr, png_infop info_ptr) {
    // Do nothing - just a placeholder
}

static void row_callback(png_structp png_ptr, png_bytep new_row,
                         png_uint_32 row_num, int pass) {
    // If we have a new row and old row exists, combine them
    if (new_row != NULL) {
        png_byte* old_row = png_get_rows(png_ptr, NULL)[row_num];
        if (old_row != NULL) {
            png_progressive_combine_row(png_ptr, old_row, new_row);
        }
    }
}

static void end_callback(png_structp png_ptr, png_infop info_ptr) {
    // Do nothing - just a placeholder
}

static void frame_info_callback(png_structp png_ptr, png_uint_32 frame_num) {
    // Do nothing - just a placeholder
}

static void frame_end_callback(png_structp png_ptr, png_uint_32 frame_num) {
    // Do nothing - just a placeholder
}

static void read_status_callback(png_structp png_ptr, png_uint_32 row_num,
                                 int pass) {
    // Do nothing - just a placeholder
}

// Main fuzzer entry point
extern "C" int LLVMFuzzerTestOneInput_307(const uint8_t *Data, size_t Size) {
    // First, test progressive reading functions
    if (Size > 0) {
        // Create PNG structures for progressive reading
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                                     NULL, NULL, NULL);
        if (!png_ptr) {
            return 0;
        }
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, NULL, NULL);
            return 0;
        }
        
        // Set up error handling
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            return 0;
        }
        
        // Set up progressive callbacks
        png_set_progressive_read_fn(png_ptr, NULL, info_callback,
                                    row_callback, end_callback);
        
        // Set read status callback
        png_set_read_status_fn(png_ptr, read_status_callback);
        
        // Set progressive frame callbacks (for APNG)
        png_set_progressive_frame_fn(png_ptr, frame_info_callback,
                                     frame_end_callback);
        
        // Process data in chunks
        size_t chunk_size = 1024;
        size_t processed = 0;
        while (processed < Size) {
            size_t to_process = (Size - processed < chunk_size) ? 
                                Size - processed : chunk_size;
            png_process_data(png_ptr, info_ptr,
                             const_cast<png_byte*>(Data + processed),
                             to_process);
            processed += to_process;
        }
        
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    }
    
    // Second, test non-progressive reading with file
    if (Size > 0) {
        // Write data to dummy file
        FILE* fp = fopen("./dummy_file", "wb");
        if (!fp) {
            return 0;
        }
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        
        // Open the file for reading
        fp = fopen("./dummy_file", "rb");
        if (!fp) {
            remove("./dummy_file");
            return 0;
        }
        
        // Create PNG structures
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                                     NULL, NULL, NULL);
        if (!png_ptr) {
            fclose(fp);
            remove("./dummy_file");
            return 0;
        }
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, NULL, NULL);
            fclose(fp);
            remove("./dummy_file");
            return 0;
        }
        
        // Set up error handling
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            remove("./dummy_file");
            return 0;
        }
        
        // Initialize I/O
        png_init_io(png_ptr, fp);
        
        // Read PNG info
        png_read_info(png_ptr, info_ptr);
        
        // Get image dimensions
        png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
        png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
        
        if (width > 0 && height > 0) {
            // Allocate row buffer
            png_bytep row = (png_bytep)malloc(png_get_rowbytes(png_ptr, info_ptr));
            if (row) {
                // Read rows
                for (png_uint_32 y = 0; y < height; y++) {
                    png_read_row(png_ptr, row, NULL);
                }
                free(row);
            }
        }
        
        // Clean up
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        remove("./dummy_file");
    }
    
    return 0;
}