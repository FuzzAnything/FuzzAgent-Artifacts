#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <zlib.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_000 {

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

// Custom read/write functions for memory buffers
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

static void pngtest_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Write to nowhere - just consume the data
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void pngtest_flush_data(png_structp png_ptr) {
    (void)png_ptr;
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
    
    // Try to create read structure
    png_structp png_ptr_read = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        pngtest_error, 
        pngtest_warning);
    
    if (!png_ptr_read) {
        return 0;
    }
    
    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        return 0;
    }
    
    // Set up custom read functions
    png_set_read_fn(png_ptr_read, &buffer, pngtest_read_data);
    
    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr_read))) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    // Try to read PNG info
    png_read_info(png_ptr_read, info_ptr_read);
    
    // Get image information
    png_uint_32 width = png_get_image_width(png_ptr_read, info_ptr_read);
    png_uint_32 height = png_get_image_height(png_ptr_read, info_ptr_read);
    png_byte color_type = png_get_color_type(png_ptr_read, info_ptr_read);
    png_byte bit_depth = png_get_bit_depth(png_ptr_read, info_ptr_read);
    
    // Transformations based on fuzzed input
    if (fdp.ConsumeBool() && color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr_read);
    }
    
    if (fdp.ConsumeBool() && color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png_ptr_read);
    }
    
    if (fdp.ConsumeBool() && png_get_valid(png_ptr_read, info_ptr_read, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr_read);
    }
    
    // Set various transformations based on fuzzed input
    if (fdp.ConsumeBool()) {
        png_set_bgr(png_ptr_read);
    }
    
    if (fdp.ConsumeBool()) {
        png_set_swap_alpha(png_ptr_read);
    }
    
    if (fdp.ConsumeBool()) {
        png_set_invert_alpha(png_ptr_read);
    }
    
    // Update info after transformations
    png_read_update_info(png_ptr_read, info_ptr_read);
    
    // Get updated row bytes
    png_size_t rowbytes = png_get_rowbytes(png_ptr_read, info_ptr_read);
    
    // Allocate row pointers
    png_bytep* row_pointers = (png_bytep*)png_malloc(png_ptr_read, height * sizeof(png_bytep));
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)png_malloc(png_ptr_read, rowbytes);
    }
    
    // Try to read the image
    png_read_image(png_ptr_read, row_pointers);
    
    // Try to read the end
    png_read_end(png_ptr_read, nullptr);
    
    // Now test writing
    png_structp png_ptr_write = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        pngtest_error,
        pngtest_warning);
    
    if (!png_ptr_write) {
        // Clean up read structures
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr_read, row_pointers[y]);
        }
        png_free(png_ptr_read, row_pointers);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_write_struct(&png_ptr_write, nullptr);
        // Clean up read structures
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr_read, row_pointers[y]);
        }
        png_free(png_ptr_read, row_pointers);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    // Set up write functions
    png_set_write_fn(png_ptr_write, nullptr, pngtest_write_data, pngtest_flush_data);
    
    // Set error handling for write
    if (setjmp(png_jmpbuf(png_ptr_write))) {
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        // Clean up read structures
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr_read, row_pointers[y]);
        }
        png_free(png_ptr_read, row_pointers);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    // Set IHDR info for writing
    png_set_IHDR(png_ptr_write, info_ptr_write, width, height,
                 bit_depth, color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    
    // Write info
    png_write_info(png_ptr_write, info_ptr_write);
    
    // Try to write the image
    png_write_image(png_ptr_write, row_pointers);
    
    // Write end
    png_write_end(png_ptr_write, nullptr);
    
    // Clean up
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr_read, row_pointers[y]);
    }
    png_free(png_ptr_read, row_pointers);
    
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    
    return 0;
}

} // namespace harness_000
