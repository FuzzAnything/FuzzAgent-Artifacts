#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <zlib.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_021 {

// Error handling callbacks for libpng
static void pngtest_error(png_structp png_ptr, png_const_charp msg) {
    (void)png_ptr;
    (void)msg;
    // Do nothing - just return
}

static void pngtest_warning(png_structp png_ptr, png_const_charp msg) {
    (void)png_ptr;
    (void)msg;
    // Do nothing - just return
}

// Custom read functions for memory buffers
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
    if (size < 8) {
        return 0;  // Need at least PNG signature
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test PNG signature checking
    png_byte png_sig[8];
    if (size >= 8) {
        auto sig_bytes = fdp.ConsumeBytes<uint8_t>(8);
        memcpy(png_sig, sig_bytes.data(), 8);
        
        // Check if it's a PNG file
        int is_png = png_sig_cmp(png_sig, 0, 8);
        (void)is_png; // Result can be ignored for fuzzing
    }
    
    // Create a memory buffer for PNG operations
    png_memory_buffer buffer;
    buffer.data = data;
    buffer.size = size;
    buffer.offset = 0;
    
    // Step 1: Initialize PNG read structure
    png_structp png_ptr_read = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        pngtest_error, 
        pngtest_warning);
    
    if (!png_ptr_read) {
        return 0;
    }
    
    // Step 2: Create info structure
    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        return 0;
    }
    
    // Step 3: Set up read callback
    png_set_read_fn(png_ptr_read, &buffer, pngtest_read_data);
    
    // Set error handling for read
    if (setjmp(png_jmpbuf(png_ptr_read))) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    // Step 4: Read PNG header and chunks (including pHYs chunk if present)
    png_read_info(png_ptr_read, info_ptr_read);
    
    // Step 5: Call all physical dimension getter functions
    
    // Pixel density functions (required helpers)
    png_uint_32 pixels_per_inch = png_get_pixels_per_inch(png_ptr_read, info_ptr_read);
    (void)pixels_per_inch;
    
    png_uint_32 x_pixels_per_inch = png_get_x_pixels_per_inch(png_ptr_read, info_ptr_read);
    (void)x_pixels_per_inch;
    
    png_uint_32 y_pixels_per_inch = png_get_y_pixels_per_inch(png_ptr_read, info_ptr_read);
    (void)y_pixels_per_inch;
    
    png_uint_32 pixels_per_meter = png_get_pixels_per_meter(png_ptr_read, info_ptr_read);
    (void)pixels_per_meter;
    
    png_uint_32 x_pixels_per_meter = png_get_x_pixels_per_meter(png_ptr_read, info_ptr_read);
    (void)x_pixels_per_meter;
    
    png_uint_32 y_pixels_per_meter = png_get_y_pixels_per_meter(png_ptr_read, info_ptr_read);
    (void)y_pixels_per_meter;
    
    // Pixel aspect ratio
    float pixel_aspect_ratio = png_get_pixel_aspect_ratio(png_ptr_read, info_ptr_read);
    (void)pixel_aspect_ratio;
    
    // Offset in inches (floating point)
    float x_offset_inches = png_get_x_offset_inches(png_ptr_read, info_ptr_read);
    (void)x_offset_inches;
    
    float y_offset_inches = png_get_y_offset_inches(png_ptr_read, info_ptr_read);
    (void)y_offset_inches;
    
#ifdef PNG_FIXED_POINT_SUPPORTED
    // Offset in inches (fixed point)
    png_fixed_point x_offset_inches_fixed = png_get_x_offset_inches_fixed(png_ptr_read, info_ptr_read);
    (void)x_offset_inches_fixed;
    
    png_fixed_point y_offset_inches_fixed = png_get_y_offset_inches_fixed(png_ptr_read, info_ptr_read);
    (void)y_offset_inches_fixed;
#endif
    
    // Offset in pixels
    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr_read, info_ptr_read);
    (void)x_offset_pixels;
    
    png_int_32 y_offset_pixels = png_get_y_offset_pixels(png_ptr_read, info_ptr_read);
    (void)y_offset_pixels;
    
    // Offset in microns
    png_int_32 x_offset_microns = png_get_x_offset_microns(png_ptr_read, info_ptr_read);
    (void)x_offset_microns;
    
    png_int_32 y_offset_microns = png_get_y_offset_microns(png_ptr_read, info_ptr_read);
    (void)y_offset_microns;
    
#ifdef PNG_FIXED_POINT_SUPPORTED
    // Fixed point pixel aspect ratio
    png_fixed_point pixel_aspect_ratio_fixed = png_get_pixel_aspect_ratio_fixed(png_ptr_read, info_ptr_read);
    (void)pixel_aspect_ratio_fixed;
#endif
    
    // Step 6: Clean up
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    
    return 0;
}

} // namespace harness_021
