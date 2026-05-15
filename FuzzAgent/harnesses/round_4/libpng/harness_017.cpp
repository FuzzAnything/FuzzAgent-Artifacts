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

namespace harness_017 {

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

// Custom write function for memory buffers
struct png_memory_buffer {
    std::vector<uint8_t> data;
    size_t offset;
};

static void pngtest_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_memory_buffer* buffer = (png_memory_buffer*)png_get_io_ptr(png_ptr);
    
    if (buffer) {
        // Append data to buffer
        buffer->data.insert(buffer->data.end(), data, data + length);
    }
    // Otherwise, just discard the data
}

static void pngtest_flush_data(png_structp png_ptr) {
    (void)png_ptr;
    // Nothing to flush for memory buffer
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for PNG write operations with compression settings
    // We need enough data for:
    // - Basic image parameters (width, height, bit_depth, color_type): ~20 bytes
    // - Compression parameters (level, strategy, window_bits, method, mem_level): ~20 bytes
    // - Filter settings: ~4 bytes
    // - Image data (small dummy buffer): ~100 bytes
    if (size < 150) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create PNG write structure - TARGET HELPER: png_create_write_struct
    png_structp png_ptr = png_create_write_struct(
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
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }
    
    // Set error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_info_struct(png_ptr, &info_ptr);
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }
    
    // Step 3: Set up custom write functions
    png_memory_buffer write_buffer;
    write_buffer.offset = 0;
    png_set_write_fn(png_ptr, &write_buffer, pngtest_write_data, pngtest_flush_data);
    
    // Step 4: Set basic image parameters - TARGET HELPER: png_set_IHDR
    // Consume image parameters from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    int bit_depth = fdp.PickValueInArray({1, 2, 4, 8, 16});
    int color_type = fdp.PickValueInArray({PNG_COLOR_TYPE_GRAY, PNG_COLOR_TYPE_RGB, 
                                           PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_GRAY_ALPHA,
                                           PNG_COLOR_TYPE_RGB_ALPHA});
    int interlace_method = fdp.PickValueInArray({PNG_INTERLACE_NONE, PNG_INTERLACE_ADAM7});
    int compression_method = PNG_COMPRESSION_TYPE_DEFAULT;
    int filter_method = PNG_FILTER_TYPE_DEFAULT;
    
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_method, compression_method, filter_method);
    
    // Step 5: Configure compression settings - PRIMARY TARGETS
    
    // PRIMARY TARGET: png_set_compression_level (0-9 as per zlib)
    int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
    png_set_compression_level(png_ptr, compression_level);
    
    // PRIMARY TARGET: png_set_compression_mem_level (1-9 as per zlib)
    int mem_level = fdp.ConsumeIntegralInRange<int>(1, 9);
    png_set_compression_mem_level(png_ptr, mem_level);
    
    // PRIMARY TARGET: png_set_compression_strategy 
    // zlib strategies: Z_DEFAULT_STRATEGY, Z_FILTERED, Z_HUFFMAN_ONLY, Z_RLE, Z_FIXED
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4); // 0-4 covers zlib strategies
    png_set_compression_strategy(png_ptr, strategy);
    
    // PRIMARY TARGET: png_set_compression_window_bits (8-15)
    int window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
    png_set_compression_window_bits(png_ptr, window_bits);
    
    // PRIMARY TARGET: png_set_compression_method (should be 8 for deflate)
    // According to pngwrite.c, only method 8 is supported
    int method = 8; // Fixed as per PNG specification
    png_set_compression_method(png_ptr, method);
    
    // Step 6: Configure filter settings - PRIMARY TARGET: png_set_filter
    // Filter method should be 0 (as per png.h documentation)
    int filter_method_param = 0;
    
    // Filter flags: combinations of PNG_FILTER_NONE, PNG_FILTER_SUB, etc.
    int filter_flags = 0;
    
    // Consume filter configuration from fuzzer input
    uint8_t filter_config = fdp.ConsumeIntegral<uint8_t>();
    
    // Build filter flags based on consumed bits
    if (filter_config & 0x01) filter_flags |= PNG_FILTER_NONE;
    if (filter_config & 0x02) filter_flags |= PNG_FILTER_SUB;
    if (filter_config & 0x04) filter_flags |= PNG_FILTER_UP;
    if (filter_config & 0x08) filter_flags |= PNG_FILTER_AVG;
    if (filter_config & 0x10) filter_flags |= PNG_FILTER_PAETH;
    
    // If no filters selected, use default (all filters)
    if (filter_flags == 0) {
        filter_flags = PNG_ALL_FILTERS;
    }
    
    png_set_filter(png_ptr, filter_method_param, filter_flags);
    
    // Step 7: Write PNG header
    png_write_info(png_ptr, info_ptr);
    
    // Step 8: Prepare and write some dummy image data
    // Calculate row bytes based on image parameters
    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    if (row_bytes > 0 && row_bytes < 10000) { // Reasonable limit
        // Create a dummy row buffer
        std::vector<png_byte> row_buffer(row_bytes, 0);
        
        // Fill with some pattern from fuzzer input if available
        if (fdp.remaining_bytes() >= row_bytes) {
            auto row_data = fdp.ConsumeBytes<uint8_t>(row_bytes);
            memcpy(row_buffer.data(), row_data.data(), row_data.size());
        }
        
        // Write a few rows (up to min(height, 10) to avoid excessive memory)
        int rows_to_write = fdp.ConsumeIntegralInRange<int>(1, height < 10 ? height : 10);
        for (int i = 0; i < rows_to_write; i++) {
            png_write_row(png_ptr, row_buffer.data());
        }
    }
    
    // Step 9: Write end of PNG
    png_write_end(png_ptr, info_ptr);
    
    // Step 10: Cleanup
    png_destroy_info_struct(png_ptr, &info_ptr);
    png_destroy_write_struct(&png_ptr, nullptr);
    
    return 0;
}

} // namespace harness_017
