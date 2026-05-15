#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "png.h"

namespace harness_000 {


// Simple error handling for fuzzing
static void pngtest_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void pngtest_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Custom read function for reading from memory buffer
struct png_read_data {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void png_read_from_buffer(png_structp png_ptr, png_bytep out, png_size_t length) {
    png_read_data* rd = (png_read_data*)png_get_io_ptr(png_ptr);
    if (rd->offset + length > rd->size) {
        png_error(png_ptr, "Read beyond buffer");
    }
    memcpy(out, rd->data + rd->offset, length);
    rd->offset += length;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 8) {
        // PNG signature is 8 bytes minimum
        return 0;
    }

    // Use all input as PNG data (no config flags for simplicity)
    
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_infop end_info = NULL;
    
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, pngtest_error, pngtest_warning);
    if (png_ptr == NULL) {
        return 0;
    }

    // Set up error handling with setjmp - MUST be done before any libpng calls that can longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during PNG processing
        if (png_ptr) {
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        }
        return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    end_info = png_create_info_struct(png_ptr);
    if (end_info == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Set up custom read function to read from our buffer
    png_read_data read_data = {data, size, 0};
    png_set_read_fn(png_ptr, &read_data, png_read_from_buffer);
    
    // Tell libpng we haven't read any signature bytes yet
    png_set_sig_bytes(png_ptr, 0);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Get image dimensions
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    
    // Update info (no transformations for simplicity)
    png_read_update_info(png_ptr, info_ptr);
    
    // Allocate row pointers
    png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    png_bytep* row_pointers = (png_bytep*)png_malloc(png_ptr, height * sizeof(png_bytep));
    if (row_pointers == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return 0;
    }
    
    // Allocate memory for each row
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)png_malloc(png_ptr, rowbytes);
        if (row_pointers[y] == NULL) {
            // Clean up already allocated rows
            for (png_uint_32 y2 = 0; y2 < y; y2++) {
                png_free(png_ptr, row_pointers[y2]);
            }
            png_free(png_ptr, row_pointers);
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            return 0;
        }
    }
    
    // Read the image
    png_read_image(png_ptr, row_pointers);
    
    // Read the end info
    png_read_end(png_ptr, end_info);
    
    // Clean up row memory
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);
    
    // Clean up PNG structures
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    
    return 0;
}

} // namespace harness_000
