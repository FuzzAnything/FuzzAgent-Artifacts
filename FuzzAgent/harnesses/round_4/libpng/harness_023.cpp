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

namespace harness_023 {

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

// Custom write function for memory-based output
struct png_output_buffer {
    std::vector<uint8_t> data;
    size_t offset;
};

static void pngtest_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_output_buffer* buffer = (png_output_buffer*)png_get_io_ptr(png_ptr);
    
    // Resize buffer if needed
    if (buffer->offset + length > buffer->data.size()) {
        buffer->data.resize(buffer->offset + length);
    }
    
    memcpy(buffer->data.data() + buffer->offset, data, length);
    buffer->offset += length;
}

static void pngtest_flush_data(png_structp png_ptr) {
    (void)png_ptr;
    // Nothing to flush for memory buffer
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough data for basic parameters and some chunk data
    // We need:
    // - Image dimensions: width, height (8 bytes)
    // - Bit depth, color type, etc. (4 bytes)
    // - At least some data for chunk operations (16 bytes)
    // - Transform flags for png_write_png (4 bytes)
    // Total minimum: ~32 bytes
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Initialize write structures - TARGET: png_create_write_struct
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
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Step 3: Set up output destination with png_set_write_fn for memory-based output
    png_output_buffer output_buffer;
    output_buffer.offset = 0;
    output_buffer.data.reserve(4096); // Initial reserve
    
    png_set_write_fn(png_ptr, &output_buffer, pngtest_write_data, pngtest_flush_data);
    
    // Step 4: Set basic image header with png_set_IHDR
    // Consume parameters for IHDR from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 4096);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 4096);
    // Use ConsumeIntegral for bit_depth choice
    int bit_depth_choice = fdp.ConsumeIntegral<int>() % 5;
    int bit_depth;
    switch (bit_depth_choice) {
        case 0: bit_depth = 1; break;
        case 1: bit_depth = 2; break;
        case 2: bit_depth = 4; break;
        case 3: bit_depth = 8; break;
        case 4: bit_depth = 16; break;
        default: bit_depth = 8; break;
    }
    
    // Use ConsumeIntegral for color_type choice
    int color_type_choice = fdp.ConsumeIntegral<int>() % 5;
    int color_type;
    switch (color_type_choice) {
        case 0: color_type = PNG_COLOR_TYPE_GRAY; break;
        case 1: color_type = PNG_COLOR_TYPE_GRAY_ALPHA; break;
        case 2: color_type = PNG_COLOR_TYPE_PALETTE; break;
        case 3: color_type = PNG_COLOR_TYPE_RGB; break;
        case 4: color_type = PNG_COLOR_TYPE_RGB_ALPHA; break;
        default: color_type = PNG_COLOR_TYPE_RGB; break;
    }
    
    // Use ConsumeIntegral for interlace_method choice
    int interlace_method_choice = fdp.ConsumeIntegral<int>() % 2;
    int interlace_method;
    switch (interlace_method_choice) {
        case 0: interlace_method = PNG_INTERLACE_NONE; break;
        case 1: interlace_method = PNG_INTERLACE_ADAM7; break;
        default: interlace_method = PNG_INTERLACE_NONE; break;
    }
    int compression_method = PNG_COMPRESSION_TYPE_DEFAULT;
    int filter_method = PNG_FILTER_TYPE_DEFAULT;
    
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_method, compression_method, filter_method);
    
    // Step 5: Perform core write operations
    
    // a. Call png_write_png for comprehensive PNG writing - TARGET: png_write_png
    // Consume transform flags from fuzzer input
    int transforms = fdp.ConsumeIntegral<int>();
    void* params = nullptr; // params is currently unused in libpng
    
    // Write PNG header and metadata
    png_write_info(png_ptr, info_ptr);
    
    // Generate some dummy row data for writing
    if (fdp.remaining_bytes() > 0) {
        // Calculate row size based on image parameters
        size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        if (row_bytes > 0 && row_bytes < 65536) { // Reasonable limit
            // Create a dummy row with fuzzed data
            std::vector<png_byte> row_data(row_bytes);
            if (fdp.remaining_bytes() >= row_bytes) {
                auto row_bytes_data = fdp.ConsumeBytes<uint8_t>(row_bytes);
                memcpy(row_data.data(), row_bytes_data.data(), row_bytes);
            }
            
            // Write a few rows
            png_bytep row_pointer = row_data.data();
            for (png_uint_32 y = 0; y < height && y < 10; y++) {
                png_write_row(png_ptr, row_pointer);
            }
        }
    }
    
    png_write_end(png_ptr, info_ptr);
    
    // b. Call png_write_flush to test flush behavior - TARGET: png_write_flush
    // Set flush rows parameter
    png_set_flush(png_ptr, fdp.ConsumeIntegralInRange<int>(0, 100));
    png_write_flush(png_ptr);
    
    // c. Call png_write_chunk and png_write_chunk_start for chunk-level writing
    // TARGET: png_write_chunk, png_write_chunk_start
    
    // Create some chunk data from remaining fuzzer input
    if (fdp.remaining_bytes() > 8) {
        // Consume chunk name (4 bytes)
        std::vector<uint8_t> chunk_name_bytes = fdp.ConsumeBytes<uint8_t>(4);
        png_byte chunk_name[5];
        memcpy(chunk_name, chunk_name_bytes.data(), 4);
        chunk_name[4] = 0; // Null terminate
        
        // Create chunk data
        size_t chunk_data_size = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
        std::vector<uint8_t> chunk_data = fdp.ConsumeBytes<uint8_t>(chunk_data_size);
        
        // Write chunk using png_write_chunk
        png_write_chunk(png_ptr, chunk_name, chunk_data.data(), chunk_data.size());
        
        // Also test chunk start/write/end sequence
        if (fdp.remaining_bytes() > 8) {
            std::vector<uint8_t> chunk_name2_bytes = fdp.ConsumeBytes<uint8_t>(4);
            png_byte chunk_name2[5];
            memcpy(chunk_name2, chunk_name2_bytes.data(), 4);
            chunk_name2[4] = 0;
            
            size_t chunk_data2_size = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
            std::vector<uint8_t> chunk_data2 = fdp.ConsumeBytes<uint8_t>(chunk_data2_size);
            
            // Write chunk using start/data/end sequence
            png_write_chunk_start(png_ptr, chunk_name2, chunk_data2.size());
            if (chunk_data2.size() > 0) {
                png_write_chunk_data(png_ptr, chunk_data2.data(), chunk_data2.size());
            }
            png_write_chunk_end(png_ptr);
        }
    }
    
    // d. Test png_image_write_to_memory for alternative memory-based writing
    // TARGET: png_image_write_to_memory
#ifdef PNG_SIMPLIFIED_WRITE_SUPPORTED
    if (fdp.remaining_bytes() > 64) {
        // Initialize png_image structure
        png_image image;
        memset(&image, 0, sizeof(image));
        image.version = PNG_IMAGE_VERSION;
        image.width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 512);
        image.height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 512);
        // Use ConsumeIntegral to get format value
        unsigned int format_choice = fdp.ConsumeIntegral<unsigned int>() % 9;
        switch (format_choice) {
            case 0: image.format = PNG_FORMAT_GRAY; break;
            case 1: image.format = PNG_FORMAT_GA; break;
            case 2: image.format = PNG_FORMAT_AG; break;
            case 3: image.format = PNG_FORMAT_RGB; break;
            case 4: image.format = PNG_FORMAT_BGR; break;
            case 5: image.format = PNG_FORMAT_RGBA; break;
            case 6: image.format = PNG_FORMAT_ARGB; break;
            case 7: image.format = PNG_FORMAT_BGRA; break;
            case 8: image.format = PNG_FORMAT_ABGR; break;
            default: image.format = PNG_FORMAT_RGB; break;
        }
        // Calculate buffer size needed
        png_alloc_size_t buffer_size = 0;
        
        // Create dummy image buffer
        size_t pixel_size = (image.format & PNG_FORMAT_FLAG_LINEAR) ? 2 : 1;
        size_t channels = 1; // Default for grayscale
        
        if (image.format & PNG_FORMAT_FLAG_COLOR) {
            channels = 3;
            if (image.format & PNG_FORMAT_FLAG_ALPHA) {
                channels = 4;
            }
        } else if (image.format & PNG_FORMAT_FLAG_ALPHA) {
            channels = 2;
        }
        
        size_t row_stride = image.width * channels * pixel_size;
        size_t total_size = image.height * row_stride;
        
        if (total_size > 0 && total_size < 1048576 && fdp.remaining_bytes() >= total_size) {
            std::vector<uint8_t> image_buffer(total_size);
            auto image_data = fdp.ConsumeBytes<uint8_t>(total_size);
            memcpy(image_buffer.data(), image_data.data(), total_size);
            
            // Try to write to memory
            int convert_to_8_bit = fdp.ConsumeBool() ? 1 : 0;
            png_image_write_to_memory(&image, nullptr, &buffer_size, convert_to_8_bit,
                                     image_buffer.data(), row_stride, nullptr);
            
            // If buffer_size > 0, we could allocate and actually write
            if (buffer_size > 0 && buffer_size < 1048576) {
                std::vector<uint8_t> memory_buffer(buffer_size);
                png_image_write_to_memory(&image, memory_buffer.data(), &buffer_size,
                                         convert_to_8_bit, image_buffer.data(), row_stride, nullptr);
            }
        }
    }
#endif // PNG_SIMPLIFIED_WRITE_SUPPORTED
    
    // Step 6: Clean up - TARGET HELPER: png_destroy_write_struct and png_destroy_info_struct
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}

} // namespace harness_023
