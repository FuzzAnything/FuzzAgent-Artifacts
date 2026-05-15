// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_set_rows at pngset.c:1845:1 in png.h
// png_write_image at pngwrite.c:659:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
// png_get_rows at pngget.c:50:1 in png.h
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
#include <cstddef>
#include <cstdlib>
#include <cstring>

#include "code/png.h"

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function that does nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Dummy flush function that does nothing
    (void)png_ptr;
}

static void error_handler(png_structp png_ptr, png_const_charp error_msg) {
    // Error handler to prevent crashes on invalid data
    (void)png_ptr;
    (void)error_msg;
    // Do nothing - just prevent the default abort behavior
}

static void warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Warning handler
    (void)png_ptr;
    (void)warning_msg;
    // Do nothing
}

extern "C" int LLVMFuzzerTestOneInput_260(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    // Write data to dummy file for reading operations
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);

    // Try to read the PNG file
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        return 0;
    }

    // Create read structures
    png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, error_handler, warning_handler);
    if (!read_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(read_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        fclose(fp);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(read_ptr))) {
        png_destroy_read_struct(&read_ptr, &info_ptr, nullptr);
        fclose(fp);
        // Continue with write operations even if read fails
    } else {
        png_init_io(read_ptr, fp);
        png_set_sig_bytes(read_ptr, 0);
        
        // Try to read the PNG info
        png_read_info(read_ptr, info_ptr);
        
        // Test png_get_rowbytes
        size_t rowbytes = png_get_rowbytes(read_ptr, info_ptr);
        (void)rowbytes;
        
        // Try to read the entire image
        png_read_png(read_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);
        
        // Test png_get_rows
        png_bytepp rows = png_get_rows(read_ptr, info_ptr);
        (void)rows;
        
        png_destroy_read_struct(&read_ptr, &info_ptr, nullptr);
        fclose(fp);
    }

    // Now test write operations
    // Create write structures
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, error_handler, warning_handler);
    if (!write_ptr) {
        return 0;
    }

    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        return 0;
    }

    // Set up error handling for write operations
    if (setjmp(png_jmpbuf(write_ptr))) {
        png_destroy_write_struct(&write_ptr, &write_info);
        return 0;
    }

    // Set up dummy I/O functions
    png_set_write_fn(write_ptr, nullptr, dummy_write_fn, dummy_flush_fn);

    // Set up basic image parameters
    png_set_IHDR(write_ptr, write_info, 
                 (png_uint_32)(Data[0] * 256 + Data[1]),  // width
                 (png_uint_32)(Data[2] * 256 + Data[3]),  // height
                 8,                                        // bit depth
                 PNG_COLOR_TYPE_RGB,                      // color type
                 PNG_INTERLACE_NONE,                      // interlace
                 PNG_COMPRESSION_TYPE_DEFAULT,            // compression
                 PNG_FILTER_TYPE_DEFAULT);                // filter

    // Write info
    png_write_info(write_ptr, write_info);

    // Create dummy row data based on fuzzer input
    png_uint_32 width = png_get_image_width(write_ptr, write_info);
    png_uint_32 height = png_get_image_height(write_ptr, write_info);
    size_t row_size = png_get_rowbytes(write_ptr, write_info);
    
    if (row_size == 0 || width == 0 || height == 0) {
        png_destroy_write_struct(&write_ptr, &write_info);
        return 0;
    }

    // Allocate rows
    png_bytepp row_pointers = (png_bytepp)png_malloc(write_ptr, height * sizeof(png_bytep));
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)png_malloc(write_ptr, row_size);
        // Fill with fuzzer data (wrap around if needed)
        for (size_t x = 0; x < row_size; x++) {
            row_pointers[y][x] = Data[(y * row_size + x) % Size];
        }
    }

    // Test png_set_rows
    png_set_rows(write_ptr, write_info, row_pointers);

    // Test png_write_image
    png_write_image(write_ptr, row_pointers);

    // Test png_write_rows
    // Reset rows to point to beginning
    png_write_rows(write_ptr, row_pointers, height);

    // Test png_write_row
    for (png_uint_32 y = 0; y < height && y < 10; y++) {  // Limit to 10 rows to avoid timeout
        png_write_row(write_ptr, row_pointers[y]);
    }

    // Write end
    png_write_end(write_ptr, write_info);

    // Clean up
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(write_ptr, row_pointers[y]);
    }
    png_free(write_ptr, row_pointers);
    png_destroy_write_struct(&write_ptr, &write_info);

    return 0;
}