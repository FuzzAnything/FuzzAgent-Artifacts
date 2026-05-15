#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <zlib.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_001 {

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
    
    // Set error handling for read
    if (setjmp(png_jmpbuf(png_ptr_read))) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    // Consume fuzzer input to determine which transforms to apply
    int read_transforms = 0;
    
    // Select read transforms based on fuzzed input
    // Avoid transforms that change pixel layout when using png_get_rows() + png_set_rows()
    // Safe transforms that don't change pixel layout:
    if (fdp.ConsumeBool()) read_transforms |= PNG_TRANSFORM_PACKING;
    if (fdp.ConsumeBool()) read_transforms |= PNG_TRANSFORM_PACKSWAP;
    if (fdp.ConsumeBool()) read_transforms |= PNG_TRANSFORM_INVERT_MONO;
    if (fdp.ConsumeBool()) read_transforms |= PNG_TRANSFORM_SHIFT;
    if (fdp.ConsumeBool()) read_transforms |= PNG_TRANSFORM_BGR;
    if (fdp.ConsumeBool()) read_transforms |= PNG_TRANSFORM_SWAP_ALPHA;
    if (fdp.ConsumeBool()) read_transforms |= PNG_TRANSFORM_SWAP_ENDIAN;
    if (fdp.ConsumeBool()) read_transforms |= PNG_TRANSFORM_INVERT_ALPHA;
    
    // DO NOT use transforms that change pixel layout with png_get_rows() pattern:
    // PNG_TRANSFORM_EXPAND, PNG_TRANSFORM_GRAY_TO_RGB, PNG_TRANSFORM_STRIP_16,
    // PNG_TRANSFORM_STRIP_ALPHA, PNG_TRANSFORM_EXPAND_16, PNG_TRANSFORM_SCALE_16
    // Try to read the entire PNG using png_read_png
    // This is the high-level API that reads everything at once
    
    // Read the PNG file
    png_read_png(png_ptr_read, info_ptr_read, read_transforms, NULL);
    
    // Get image information after reading
    png_uint_32 width = png_get_image_width(png_ptr_read, info_ptr_read);
    png_uint_32 height = png_get_image_height(png_ptr_read, info_ptr_read);
    png_byte color_type = png_get_color_type(png_ptr_read, info_ptr_read);
    png_byte bit_depth = png_get_bit_depth(png_ptr_read, info_ptr_read);
    
    // Get row pointers that were allocated by png_read_png
    png_bytep* row_pointers = png_get_rows(png_ptr_read, info_ptr_read);
    
    // Check if we got valid row pointers
    if (row_pointers == NULL) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    // Validate image dimensions to avoid excessive memory usage
    if (width == 0 || height == 0 || width > 4096 || height > 4096) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    // Now test writing with png_write_png
    png_structp png_ptr_write = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        pngtest_error,
        pngtest_warning);
    
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_write_struct(&png_ptr_write, nullptr);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    // Set up write functions
    png_set_write_fn(png_ptr_write, nullptr, pngtest_write_data, pngtest_flush_data);
    
    // Set error handling for write
    if (setjmp(png_jmpbuf(png_ptr_write))) {
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    // Set IHDR info for writing
    png_set_IHDR(png_ptr_write, info_ptr_write, width, height,
                 bit_depth, color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    
    // Copy other chunks from read info to write info for better compatibility
    // This helps with tRNS and other special chunks
    png_uint_32 valid = png_get_valid(png_ptr_read, info_ptr_read, 0xFFFFFFFF);
    if (valid & PNG_INFO_tRNS) {
        png_bytep trans;
        int num_trans;
        png_color_16p trans_values;
        png_get_tRNS(png_ptr_read, info_ptr_read, &trans, &num_trans, &trans_values);
        png_set_tRNS(png_ptr_write, info_ptr_write, trans, num_trans, trans_values);
    }
    
    // Set rows for writing (png_write_png expects rows to be set)
    png_set_rows(png_ptr_write, info_ptr_write, row_pointers);
    
    // Consume fuzzer input to determine which write transforms to apply
    int write_transforms = 0;
    
    // Select write transforms based on fuzzed input
    // Use only safe transforms that don't conflict with tRNS and other special chunks
    if (fdp.ConsumeBool()) write_transforms |= PNG_TRANSFORM_PACKING;
    if (fdp.ConsumeBool()) write_transforms |= PNG_TRANSFORM_PACKSWAP;
    if (fdp.ConsumeBool()) write_transforms |= PNG_TRANSFORM_INVERT_MONO;
    if (fdp.ConsumeBool()) write_transforms |= PNG_TRANSFORM_SHIFT;
    if (fdp.ConsumeBool()) write_transforms |= PNG_TRANSFORM_BGR;
    if (fdp.ConsumeBool()) write_transforms |= PNG_TRANSFORM_SWAP_ALPHA;
    if (fdp.ConsumeBool()) write_transforms |= PNG_TRANSFORM_SWAP_ENDIAN;
    if (fdp.ConsumeBool()) write_transforms |= PNG_TRANSFORM_INVERT_ALPHA;
    
    // Avoid STRIP_FILLER and STRIP_FILLER_AFTER as they can cause issues with tRNS chunks
    // and other PNG formats
    
    // Try to write the PNG using png_write_png
    // This is the high-level API that writes everything at once
    png_write_png(png_ptr_write, info_ptr_write, write_transforms, NULL);
    
    // Clean up
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    
    return 0;
}

} // namespace harness_001
