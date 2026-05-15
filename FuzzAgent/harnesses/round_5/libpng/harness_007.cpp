#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "png.h"

namespace harness_007 {


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

// Harness specifically targeting PNG write transformation module and high-level image writing APIs
// Focuses on comprehensive PNG writing with all metadata chunks and transformations
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 200) {
        // Need enough data for comprehensive configuration
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume image parameters from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 256);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 256);
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
    
    // Consume operation mode: 0 = manual write sequence, 1 = png_write_png, 2 = chunk writing, 3 = mixed
    int operation_mode = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    // Consume PNG transform flags
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
    
    // Consume chunk configuration
    uint8_t chunk_config = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume remaining bytes for image data and chunk data
    std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (remaining_data.size() < 100) {
        return 0; // Need enough data for various operations
    }
    
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
    
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }
    
    // Set up write buffer
    size_t buffer_size = 65536; // Reasonable size for fuzzing
    png_write_data write_data;
    write_data.buffer = (uint8_t*)malloc(buffer_size);
    write_data.capacity = buffer_size;
    write_data.size = 0;
    write_data.offset = 0;
    
    if (write_data.buffer == NULL) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    png_set_write_fn(png_ptr, &write_data, png_write_to_buffer, png_flush_buffer);
    
    // Set basic image parameters
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_type, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    
    // Test png_write_info and png_write_info_before_PLTE
    if (operation_mode != 2) { // Skip for pure chunk writing mode
        if (chunk_config & 0x01) {
            png_write_info_before_PLTE(png_ptr, info_ptr);
        } else {
            png_write_info(png_ptr, info_ptr);
        }
    }
    
    // Test writing various metadata chunks based on configuration
    if ((chunk_config & 0x02) && operation_mode != 0) {
        // Test gAMA chunk
        double gamma = 0.45455 + (fdp.ConsumeIntegral<uint8_t>() / 255.0) * 0.5;
        png_set_gAMA(png_ptr, info_ptr, gamma);
    }
    
    if ((chunk_config & 0x04) && operation_mode != 0) {
        // Test sRGB chunk
        int intent = fdp.ConsumeIntegralInRange<int>(0, 3);
        png_set_sRGB(png_ptr, info_ptr, intent);
    }
    
    if ((chunk_config & 0x08) && operation_mode != 0) {
        // Test cHRM chunk
        png_set_cHRM(png_ptr, info_ptr,
                     0.3127 + (fdp.ConsumeIntegral<uint8_t>() / 255.0) * 0.1,
                     0.3290 + (fdp.ConsumeIntegral<uint8_t>() / 255.0) * 0.1,
                     0.6400 + (fdp.ConsumeIntegral<uint8_t>() / 255.0) * 0.1,
                     0.3300 + (fdp.ConsumeIntegral<uint8_t>() / 255.0) * 0.1,
                     0.3000 + (fdp.ConsumeIntegral<uint8_t>() / 255.0) * 0.1,
                     0.6000 + (fdp.ConsumeIntegral<uint8_t>() / 255.0) * 0.1,
                     0.1500 + (fdp.ConsumeIntegral<uint8_t>() / 255.0) * 0.1,
                     0.0600 + (fdp.ConsumeIntegral<uint8_t>() / 255.0) * 0.1);
    }
    
    if ((chunk_config & 0x10) && operation_mode != 0) {
        // Test sBIT chunk
        png_color_8 sig_bit;
        sig_bit.red = fdp.ConsumeIntegralInRange<int>(1, bit_depth);
        sig_bit.green = fdp.ConsumeIntegralInRange<int>(1, bit_depth);
        sig_bit.blue = fdp.ConsumeIntegralInRange<int>(1, bit_depth);
        sig_bit.alpha = fdp.ConsumeIntegralInRange<int>(1, bit_depth);
        sig_bit.gray = fdp.ConsumeIntegralInRange<int>(1, bit_depth);
        png_set_sBIT(png_ptr, info_ptr, &sig_bit);
    }
    
    if ((chunk_config & 0x20) && operation_mode != 0 && remaining_data.size() > 50) {
        // Test text chunks using fuzzer input
        // Create a FDP from remaining data for text strings
        FuzzedDataProvider text_fdp(remaining_data.data(), remaining_data.size());
        
        // Consume strings from fuzzer input
        std::string keyword = text_fdp.ConsumeRandomLengthString(20);
        std::string text = text_fdp.ConsumeRandomLengthString(100);
        
        png_text text_chunk;
        text_chunk.compression = PNG_TEXT_COMPRESSION_NONE;
        text_chunk.key = (png_charp)keyword.c_str();
        text_chunk.text = (png_charp)text.c_str();
        text_chunk.text_length = text.length();
        text_chunk.itxt_length = 0;
        text_chunk.lang = NULL;
        text_chunk.lang_key = NULL;
        png_set_text(png_ptr, info_ptr, &text_chunk, 1);
    }
    
    // Generate image data for writing
    png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    if (rowbytes == 0 || height == 0 || width == 0) {
        // Invalid parameters
        free(write_data.buffer);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    png_bytep* row_pointers = (png_bytep*)png_malloc(png_ptr, height * sizeof(png_bytep));
    if (row_pointers == NULL) {
        free(write_data.buffer);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)png_malloc(png_ptr, rowbytes);
        if (row_pointers[y] != NULL) {
            // Fill with pattern based on fuzzer input
            for (png_size_t x = 0; x < rowbytes; x++) {
                row_pointers[y][x] = (y * x) & 0xFF;
            }
        }
    }
    
    // Test different writing sequences based on operation mode
    if (operation_mode == 0) {
        // Manual write sequence: png_write_rows
        for (png_uint_32 y = 0; y < height; y++) {
            png_write_row(png_ptr, row_pointers[y]);
        }
    } else if (operation_mode == 1) {
        // High-level write: png_write_image or png_write_png
        if (chunk_config & 0x40) {
            png_write_image(png_ptr, row_pointers);
        } else {
            png_write_rows(png_ptr, row_pointers, height);
        }
    } else if (operation_mode == 2) {
        // Direct chunk writing tests
        if (remaining_data.size() > 20) {
            // Test png_write_chunk
            png_byte chunk_name[4] = {'t', 'E', 'S', 'T'};
            png_write_chunk(png_ptr, chunk_name, remaining_data.data(), 
                           std::min((size_t)16, remaining_data.size()));
            
            // Test png_write_chunk_start/png_write_chunk_data/png_write_chunk_end
            if (remaining_data.size() > 30) {
                png_write_chunk_start(png_ptr, (const png_byte*)"teST", 
                                     std::min((size_t)20, remaining_data.size()));
                png_write_chunk_data(png_ptr, remaining_data.data() + 10, 
                                    std::min((size_t)10, remaining_data.size() - 10));
                png_write_chunk_end(png_ptr);
            }
        }
    }
    
    // Test png_write_flush
    if (operation_mode != 2 && (chunk_config & 0x80)) {
        png_write_flush(png_ptr);
    }
    
    // Test png_write_end (finish writing)
    if (operation_mode != 2) {
        png_write_end(png_ptr, info_ptr);
    }
    
    // Clean up row pointers
    for (png_uint_32 y = 0; y < height; y++) {
        if (row_pointers[y] != NULL) {
            png_free(png_ptr, row_pointers[y]);
        }
    }
    png_free(png_ptr, row_pointers);
    
    // Clean up
    free(write_data.buffer);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}

} // namespace harness_007
