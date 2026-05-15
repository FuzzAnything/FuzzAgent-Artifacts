#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <zlib.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

namespace harness_020 {

// Error callback function for libpng
static void pngtest_error(png_structp png_ptr, png_const_charp msg) {
    (void)png_ptr;
    (void)msg;
    // Do nothing - just return
}

// Warning callback function for libpng
static void pngtest_warning(png_structp png_ptr, png_const_charp msg) {
    (void)png_ptr;
    (void)msg;
    // Do nothing - just return
}

// Custom read function for memory buffers (for read operations)
struct png_memory_buffer {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void pngtest_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_memory_buffer* buffer = (png_memory_buffer*)png_get_io_ptr(png_ptr);
    
    if (buffer->offset + length > buffer->size) {
        png_error(png_ptr, "Read beyond buffer");
        return;
    }
    
    memcpy(data, buffer->data + buffer->offset, length);
    buffer->offset += length;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough for basic PNG structure setup and parameters
    // We need:
    // - Bit depth (1 byte)
    // - Image dimensions: width, height (8 bytes)
    // - Various flags and parameters (8 bytes)
    // - Some data for grayscale palette (up to 256 * 3 bytes = 768 bytes)
    // - Overall minimum: ~800 bytes
    if (size < 800) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create PNG structures - TARGET: png_create_read_struct / png_create_write_struct
    // We'll use read struct for this harness
    png_structp png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        pngtest_error, 
        pngtest_warning);
    
    if (!png_ptr) {
        return 0;
    }
    
    // Step 2: Create info structure - TARGET HELPER: png_create_info_struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Step 3: Initialize info structure - TARGET: png_info_init_3
    // This is a deprecated internal function but we can still call it for coverage
    // It expects a pointer to pointer and the size of the structure
    // png_info is opaque, so we don't know its size. We'll pass 0 to trigger reallocation path.
    png_infop info_ptr_2 = nullptr;
    // Allocate a small buffer to pass as the pointer (will be freed and reallocated if needed)
    info_ptr_2 = (png_infop)malloc(1); // Any small size
    if (info_ptr_2) {
        png_info* info_ptr_ref = info_ptr_2;
        png_info_init_3(&info_ptr_ref, 0); // Pass 0 as size to trigger reallocation
        // info_ptr_ref may have changed if reallocated
        // Free the allocated memory (could be the original or new allocation)
        if (info_ptr_ref) {
            free(info_ptr_ref);
        }
    }
    // Step 4: Consume parameters for image setup
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    int bit_depth = fdp.ConsumeIntegralInRange<int>(1, 8);
    // Ensure valid bit depth for grayscale (1, 2, 4, or 8)
    if (bit_depth != 1 && bit_depth != 2 && bit_depth != 4 && bit_depth != 8) {
        bit_depth = (bit_depth % 4) + 1; // Map to valid values
    }
    int color_type = PNG_COLOR_TYPE_GRAY;
    int interlace_type = fdp.ConsumeBool() ? PNG_INTERLACE_NONE : PNG_INTERLACE_ADAM7;
    int compression_type = PNG_COMPRESSION_TYPE_DEFAULT;
    int filter_type = PNG_FILTER_TYPE_DEFAULT;
    
    // Step 5: Set image dimensions and color type - using png_set_IHDR
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, 
                interlace_type, compression_type, filter_type);
    
    // Step 6: Build grayscale palette - TARGET: png_build_grayscale_palette
    // This function requires a palette array of size (1 << bit_depth)
    int palette_size = 1 << bit_depth;
    png_color palette[256]; // Maximum size for 8-bit (256 entries)
    
    // Clear the palette first
    memset(palette, 0, sizeof(palette));
    
    // Call the target function
    png_build_grayscale_palette(bit_depth, palette);
    
    // Step 7: Set color calibration - TARGET: png_set_sRGB_gAMA_and_cHRM
    int srgb_intent = fdp.ConsumeIntegralInRange<int>(0, 3); // 0-3 are valid SRGB intents
    png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, srgb_intent);
    
    // Step 8: Query dimension information - TARGET: png_get_pixels_per_inch and png_get_pixels_per_meter
    // These functions return 0 if pHYs chunk not present, but we can still call them
    png_uint_32 pixels_per_inch = png_get_pixels_per_inch(png_ptr, info_ptr);
    png_uint_32 pixels_per_meter = png_get_pixels_per_meter(png_ptr, info_ptr);
    
    // Also get x and y variants
    png_uint_32 x_pixels_per_inch = png_get_x_pixels_per_inch(png_ptr, info_ptr);
    png_uint_32 y_pixels_per_inch = png_get_y_pixels_per_inch(png_ptr, info_ptr);
    png_uint_32 x_pixels_per_meter = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    png_uint_32 y_pixels_per_meter = png_get_y_pixels_per_meter(png_ptr, info_ptr);
    
    (void)pixels_per_inch;
    (void)pixels_per_meter;
    (void)x_pixels_per_inch;
    (void)y_pixels_per_inch;
    (void)x_pixels_per_meter;
    (void)y_pixels_per_meter;
    // Step 9: Test data freer functionality - TARGET: png_data_freer
    // We need to set up some data that can be freed
    // First, let's set some data in the info struct that we can mark for freeing
    // We'll use text chunk as an example
    
    // Create text chunk using fuzzer-provided data
    // Check if we have enough data for text chunk (key + value + some extra)
    if (fdp.remaining_bytes() >= 32) {
        png_text text;
        
        // Consume strings from fuzzer input
        std::string key_str = fdp.ConsumeRandomLengthString(16);
        std::string text_str = fdp.ConsumeRandomLengthString(16);
        
        // Allocate memory for the text structure
        // Note: png_set_text will allocate its own copies, so we need to provide pointers
        text.compression = PNG_TEXT_COMPRESSION_NONE;
        text.key = const_cast<char*>(key_str.c_str());
        text.text = const_cast<char*>(text_str.c_str());
        text.text_length = text_str.length();
        
        // Set the text (this allocates memory internally)
        png_set_text(png_ptr, info_ptr, &text, 1);
        
        // Now mark the text data for freeing using png_data_freer
        // PNG_DESTROY_WILL_FREE_DATA means libpng will free the data
        png_data_freer(png_ptr, info_ptr, PNG_DESTROY_WILL_FREE_DATA, PNG_FREE_TEXT);
        
        // Also test PNG_USER_WILL_FREE_DATA
        png_data_freer(png_ptr, info_ptr, PNG_USER_WILL_FREE_DATA, PNG_FREE_TEXT);
    }
    // Step 10: Test png_malloc_default functionality indirectly
    // We can test allocation by trying to allocate some memory
    // Note: png_malloc_default is internal, but we can test memory allocation patterns
    // by using png_malloc which may call it internally
    
    // Step 11: Test write structure creation for completeness
    png_structp write_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        pngtest_error,
        pngtest_warning);
    
    if (write_ptr) {
        png_infop write_info_ptr = png_create_info_struct(write_ptr);
        if (write_info_ptr) {
            // Test similar operations on write structure
            png_set_IHDR(write_ptr, write_info_ptr, width, height, bit_depth, color_type,
                        interlace_type, compression_type, filter_type);
            
            // Build grayscale palette for write structure
            png_build_grayscale_palette(bit_depth, palette);
            
            // Clean up write structure
            png_destroy_write_struct(&write_ptr, &write_info_ptr);
        } else {
            png_destroy_write_struct(&write_ptr, nullptr);
        }
    }
    
    // Step 12: Set up custom read functions for completeness (optional)
    // Consume remaining bytes for PNG data
    std::vector<uint8_t> png_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    if (png_data.size() > 0) {
        png_memory_buffer buffer;
        buffer.data = png_data.data();
        buffer.size = png_data.size();
        buffer.offset = 0;
        
        png_set_read_fn(png_ptr, &buffer, pngtest_read_data);
    }
    
    // Step 13: Clean up - TARGET: png_destroy_read_struct
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}

} // namespace harness_020
