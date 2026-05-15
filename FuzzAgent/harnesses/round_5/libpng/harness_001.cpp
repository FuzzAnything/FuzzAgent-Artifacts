#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "png.h"

namespace harness_001 {


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

// Custom write function for writing to memory buffer
struct png_write_data {
    uint8_t* buffer;
    size_t size;
    size_t capacity;
    size_t offset;
};

static void png_write_to_buffer(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_write_data* wd = (png_write_data*)png_get_io_ptr(png_ptr);
    
    // Check if we need to expand buffer
    if (wd->offset + length > wd->capacity) {
        // For fuzzing, we don't actually need to store all data
        // Just simulate successful write
        wd->offset += length;
        return;
    }
    
    // Write data to buffer
    if (wd->buffer != NULL && wd->offset + length <= wd->capacity) {
        memcpy(wd->buffer + wd->offset, data, length);
    }
    wd->offset += length;
}

static void png_flush_buffer(png_structp png_ptr) {
    (void)png_ptr;
    // Nothing to do for memory buffer
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        // Need minimum data for image parameters
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume image parameters from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1024);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1024);
    int bit_depth = fdp.PickValueInArray({1, 2, 4, 8, 16});
    int color_type = fdp.PickValueInArray({
        PNG_COLOR_TYPE_GRAY,
        PNG_COLOR_TYPE_GRAY_ALPHA,
        PNG_COLOR_TYPE_PALETTE,
        PNG_COLOR_TYPE_RGB,
        PNG_COLOR_TYPE_RGB_ALPHA
    });
    int interlace_type = fdp.PickValueInArray({
        PNG_INTERLACE_NONE,
        PNG_INTERLACE_ADAM7
    });
    int compression_type = PNG_COMPRESSION_TYPE_BASE;
    int filter_type = PNG_FILTER_TYPE_BASE;
    
    // Choose writing method: 0 = manual write, 1 = png_write_png
    int write_method = fdp.ConsumeIntegralInRange<int>(0, 1);
    
    // Consume remaining bytes for image data
    std::vector<uint8_t> image_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    
    // Create write structure
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                      NULL, pngtest_error, pngtest_warning);
    if (png_ptr == NULL) {
        return 0;
    }
    
    // Set up error handling with setjmp - MUST be done before any libpng calls that can longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during PNG processing
        if (png_ptr) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
        }
        return 0;
    }
    
    // Create info structure
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }
    
    // Set up custom write function to write to memory buffer
    png_write_data write_data = {NULL, 0, 65536, 0}; // 64KB buffer
    write_data.buffer = (uint8_t*)malloc(write_data.capacity);
    if (write_data.buffer == NULL) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    png_set_write_fn(png_ptr, &write_data, png_write_to_buffer, png_flush_buffer);
    
    // Set image parameters
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);
    
    // Set some optional chunks based on fuzzer input
    if (fdp.ConsumeBool()) {
        // Set gamma
        double gamma = fdp.ConsumeFloatingPointInRange<double>(0.1, 5.0);
        png_set_gAMA(png_ptr, info_ptr, gamma);
    }
    
    if (fdp.ConsumeBool() && color_type == PNG_COLOR_TYPE_PALETTE) {
        // Create a simple palette for palette images
        png_color palette[256];
        for (int i = 0; i < 256 && i * 3 < (int)image_data.size(); i++) {
            palette[i].red = i < image_data.size() ? image_data[i] : 0;
            palette[i].green = i + 1 < image_data.size() ? image_data[i + 1] : 0;
            palette[i].blue = i + 2 < image_data.size() ? image_data[i + 2] : 0;
        }
        png_set_PLTE(png_ptr, info_ptr, palette, 256);
    }
    
    // Write PNG based on chosen method
    if (write_method == 0) {
        // Manual write sequence
        png_write_info(png_ptr, info_ptr);
        
        // Calculate bytes per row
        png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
        if (rowbytes == 0 || height == 0) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            free(write_data.buffer);
            return 0;
        }
        
        // Allocate and write rows
        png_bytep row = (png_bytep)png_malloc(png_ptr, rowbytes);
        if (row == NULL) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            free(write_data.buffer);
            return 0;
        }
        
        // Fill rows with data from fuzzer input
        size_t data_offset = 0;
        for (png_uint_32 y = 0; y < height; y++) {
            // Fill row with pattern from fuzzer data
            for (png_size_t x = 0; x < rowbytes; x++) {
                if (data_offset < image_data.size()) {
                    row[x] = image_data[data_offset++];
                } else {
                    row[x] = (png_byte)((x + y) & 0xFF);
                }
            }
            png_write_row(png_ptr, row);
        }
        
        png_free(png_ptr, row);
        png_write_end(png_ptr, info_ptr);
    } else {
        // Use png_write_png convenience function
        
        // First need to set up row pointers with image data
        png_bytep* row_pointers = (png_bytep*)png_malloc(png_ptr, height * sizeof(png_bytep));
        if (row_pointers == NULL) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            free(write_data.buffer);
            return 0;
        }
        
        png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
        if (rowbytes == 0) {
            png_free(png_ptr, row_pointers);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            free(write_data.buffer);
            return 0;
        }
        
        size_t data_offset = 0;
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_bytep)png_malloc(png_ptr, rowbytes);
            if (row_pointers[y] == NULL) {
                // Clean up already allocated rows
                for (png_uint_32 i = 0; i < y; i++) {
                    png_free(png_ptr, row_pointers[i]);
                }
                png_free(png_ptr, row_pointers);
                png_destroy_write_struct(&png_ptr, &info_ptr);
                free(write_data.buffer);
                return 0;
            }
            
            // Fill row with data
            for (png_size_t x = 0; x < rowbytes; x++) {
                if (data_offset < image_data.size()) {
                    row_pointers[y][x] = image_data[data_offset++];
                } else {
                    row_pointers[y][x] = (png_byte)((x + y) & 0xFF);
                }
            }
        }
        
        // Apply some transformations based on fuzzer input
        int transforms = 0;
        if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_IDENTITY;
        if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_STRIP_16;
        if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_STRIP_ALPHA;
        if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_PACKING;
        if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_PACKSWAP;
        if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_EXPAND;
        if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_INVERT_MONO;
        if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_SHIFT;
        if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_BGR;
        if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_SWAP_ALPHA;
        if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_SWAP_ENDIAN;
        if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_INVERT_ALPHA;
        if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_STRIP_FILLER;
        
        // Write the PNG using the convenience function
        png_write_png(png_ptr, info_ptr, transforms, NULL);
        
        // Clean up row pointers
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
        png_free(png_ptr, row_pointers);
    }
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    free(write_data.buffer);
    
    return 0;
}

} // namespace harness_001
