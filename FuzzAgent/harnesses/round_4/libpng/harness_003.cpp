#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <zlib.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_003 {

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

// Progressive read callbacks
static void progressive_info_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
    // Called when info is available
}

static void progressive_row_callback(png_structp png_ptr, png_bytep new_row,
                                     png_uint_32 row_num, int pass) {
    (void)png_ptr;
    (void)new_row;
    (void)row_num;
    (void)pass;
    // Called for each row as it's decoded
}

static void progressive_end_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
    // Called when decoding is complete
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
    if (size < 64) {
        return 0;  // Need minimum data for progressive operations
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create a memory buffer for PNG operations
    png_memory_buffer buffer;
    buffer.data = data;
    buffer.size = size;
    buffer.offset = 0;
    
    // Try progressive reading first
    {
        // Create read structure for progressive reading
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
        
        // Set up progressive read functions
        png_set_progressive_read_fn(png_ptr_read, 
                                   nullptr,  // progressive_ptr
                                   progressive_info_callback,
                                   progressive_row_callback,
                                   progressive_end_callback);
        
        // Set up custom read functions
        png_set_read_fn(png_ptr_read, &buffer, pngtest_read_data);
        
        // Set error handling for read
        if (setjmp(png_jmpbuf(png_ptr_read))) {
            png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
            // Continue to write testing
        } else {
            // Try to read info (non-progressive)
            png_read_info(png_ptr_read, info_ptr_read);
            
            // Get image dimensions
            png_uint_32 width = png_get_image_width(png_ptr_read, info_ptr_read);
            png_uint_32 height = png_get_image_height(png_ptr_read, info_ptr_read);
            png_byte color_type = png_get_color_type(png_ptr_read, info_ptr_read);
            png_byte bit_depth = png_get_bit_depth(png_ptr_read, info_ptr_read);
            
            // Set transformations based on fuzzed input
            int transform_flags = fdp.ConsumeIntegral<uint8_t>();
            
            if (transform_flags & 0x01) {
                png_set_expand(png_ptr_read);
            }
            if (transform_flags & 0x02) {
                png_set_strip_16(png_ptr_read);
            }
            if (transform_flags & 0x04) {
                png_set_gray_to_rgb(png_ptr_read);
            }
            if (transform_flags & 0x08) {
                png_set_palette_to_rgb(png_ptr_read);
            }
            
            // Update info after transformations
            png_read_update_info(png_ptr_read, info_ptr_read);
            
            // Now test png_process_data (progressive read)
            // Reset buffer offset for progressive processing
            buffer.offset = 0;
            
            // Process data in chunks to simulate progressive reading
            size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            if (chunk_size > size) chunk_size = size;
            
            // Use png_process_data with chunked input
            // Note: This would normally be called multiple times with incoming data
            // For fuzzing, we'll call it once with our chunk
            if (chunk_size > 0) {
                png_process_data(png_ptr_read, info_ptr_read, 
                                const_cast<png_byte*>(data), chunk_size);
            }
            
            // Test png_process_data_pause
            int save_pause = fdp.ConsumeBool() ? 1 : 0;
            size_t pause_result = png_process_data_pause(png_ptr_read, save_pause);
            (void)pause_result;
            
            // Test png_process_data_skip (though deprecated)
            png_uint_32 skip_result = png_process_data_skip(png_ptr_read);
            (void)skip_result;
            
            png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        }
    }
    
    // Test writing with png_write_rows
    {
        // Create write structure
        png_structp png_ptr_write = png_create_write_struct(
            PNG_LIBPNG_VER_STRING, 
            nullptr, 
            pngtest_error, 
            pngtest_warning);
        
        if (!png_ptr_write) {
            return 0;
        }
        
        png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
        if (!info_ptr_write) {
            png_destroy_write_struct(&png_ptr_write, nullptr);
            return 0;
        }
        
        // Set up custom write functions
        png_set_write_fn(png_ptr_write, &buffer, pngtest_write_data, pngtest_flush_data);
        
        // Set error handling for write
        if (setjmp(png_jmpbuf(png_ptr_write))) {
            png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
            return 0;
        }
        
        // Set image parameters based on fuzzed input
        png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
        png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
        int bit_depth = fdp.ConsumeIntegralInRange<int>(1, 16);
        int color_type = fdp.ConsumeIntegralInRange<int>(0, 6);
        int interlace_type = fdp.ConsumeIntegralInRange<int>(0, 1);
        int compression_type = fdp.ConsumeIntegralInRange<int>(0, 1);
        int filter_type = fdp.ConsumeIntegralInRange<int>(0, 1);
        
        // Set IHDR
        png_set_IHDR(png_ptr_write, info_ptr_write, width, height, bit_depth,
                    color_type, interlace_type, compression_type, filter_type);
        
        // Set some optional chunks based on fuzzed input
        if (fdp.ConsumeBool()) {
            png_set_gAMA(png_ptr_write, info_ptr_write, fdp.ConsumeFloatingPoint<double>());
        }
        
        if (fdp.ConsumeBool()) {
            png_set_cHRM(png_ptr_write, info_ptr_write,
                        fdp.ConsumeFloatingPoint<double>(),
                        fdp.ConsumeFloatingPoint<double>(),
                        fdp.ConsumeFloatingPoint<double>(),
                        fdp.ConsumeFloatingPoint<double>(),
                        fdp.ConsumeFloatingPoint<double>(),
                        fdp.ConsumeFloatingPoint<double>(),
                        fdp.ConsumeFloatingPoint<double>(),
                        fdp.ConsumeFloatingPoint<double>());
        }
        
        // Write header
        png_write_info(png_ptr_write, info_ptr_write);
        
        // Test png_write_rows with fuzzed row data
        size_t row_bytes = png_get_rowbytes(png_ptr_write, info_ptr_write);
        if (row_bytes == 0 || row_bytes > 1024 * 1024) {  // Limit to 1MB per row
            png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
            return 0;
        }
        
        // Allocate row buffer
        std::vector<png_byte> row_buffer(row_bytes);
        png_byte* row_pointer = row_buffer.data();
        
        // Fill row with fuzzed data
        size_t fill_size = fdp.ConsumeIntegralInRange<size_t>(0, row_bytes);
        if (fill_size > 0) {
            auto fill_data = fdp.ConsumeBytes<uint8_t>(fill_size);
            size_t actual_size = std::min(fill_data.size(), row_bytes);
            memcpy(row_buffer.data(), fill_data.data(), actual_size);
        }
        
        // Test png_write_rows with different row counts
        png_uint_32 num_rows = fdp.ConsumeIntegralInRange<png_uint_32>(1, height);
        if (num_rows > height) num_rows = height;
        
        // Create array of row pointers
        std::vector<png_byte*> rows(num_rows);
        for (png_uint_32 i = 0; i < num_rows; ++i) {
            rows[i] = row_pointer;
        }
        
        // Call png_write_rows
        png_write_rows(png_ptr_write, rows.data(), num_rows);
        
        // Also test png_write_row (single row)
        if (fdp.ConsumeBool()) {
            png_write_row(png_ptr_write, row_pointer);
        }
        
        // Write end
        png_write_end(png_ptr_write, info_ptr_write);
        
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    }
    
    return 0;
}

} // namespace harness_003
