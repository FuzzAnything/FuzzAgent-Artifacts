#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_000 {

// Buffer structure for memory I/O
struct mem_buffer {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

// Callback functions for libpng error handling
static void png_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    // Error callback - longjmp will be called by libpng
    (void)png_ptr;
    (void)error_msg;
}

static void png_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    // Warning callback - ignore warnings during fuzzing
    (void)png_ptr;
    (void)warning_msg;
}

// Custom read function for reading from memory buffer
static void png_read_from_memory(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) {
        png_error(png_ptr, "No IO pointer");
        return;
    }
    
    mem_buffer* buffer = (mem_buffer*)io_ptr;
    
    if (buffer->offset + length > buffer->size) {
        png_error(png_ptr, "Read beyond buffer");
        return;
    }
    
    memcpy(data, buffer->data + buffer->offset, length);
    buffer->offset += length;
}

// Custom write function for writing to memory buffer
static void png_write_to_memory(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Just consume the data - we don't need to store it for fuzzing
    (void)png_ptr;
    (void)data;
    (void)length;
}

// Custom flush function
static void png_flush_memory(png_structp png_ptr) {
    (void)png_ptr;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 100) {
        // Need minimum size for meaningful PNG operations
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test 1: Try to read the input as PNG data using low-level API
    {
        // Prepare memory buffer for custom read function
        struct mem_buffer buffer;
        buffer.data = data;
        buffer.size = size;
        buffer.offset = 0;
        
        png_structp png_ptr = png_create_read_struct(
            PNG_LIBPNG_VER_STRING, 
            NULL, 
            png_error_callback, 
            png_warning_callback);
        
        if (png_ptr == NULL) {
            return 0;
        }
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == NULL) {
            png_destroy_read_struct(&png_ptr, NULL, NULL);
            return 0;
        }
        
        // Setup custom read function
        png_set_read_fn(png_ptr, &buffer, png_read_from_memory);
        
        // Set up error handling via setjmp
        if (setjmp(png_jmpbuf(png_ptr))) {
            // Error occurred during PNG processing
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            // Continue with other tests
        } else {
            // Try to read PNG info
            png_read_info(png_ptr, info_ptr);
            
            // Get image dimensions
            png_uint_32 width, height;
            int bit_depth, color_type, interlace_type;
            png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, 
                        &color_type, &interlace_type, NULL, NULL);
            
            // Try to read the image if it looks reasonable
            if (width > 0 && height > 0 && width < 10000 && height < 10000) {
                // Calculate row bytes
                png_size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
                if (row_bytes > 0 && row_bytes < 100000) {
                    // Allocate rows
                    png_bytep* row_pointers = (png_bytep*)malloc(height * sizeof(png_bytep));
                    if (row_pointers != NULL) {
                        for (png_uint_32 y = 0; y < height; y++) {
                            row_pointers[y] = (png_bytep)malloc(row_bytes);
                        }
                        
                        // Try to read the image
                        png_read_image(png_ptr, row_pointers);
                        
                        // Read end of PNG
                        png_read_end(png_ptr, NULL);
                        
                        // Clean up rows
                        for (png_uint_32 y = 0; y < height; y++) {
                            free(row_pointers[y]);
                        }
                        free(row_pointers);
                    }
                }
            }
            
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        }
    }
    
    // Test 2: Try writing a simple PNG using fuzzer-generated parameters
    {
        // Consume parameters for PNG creation
        int width = fdp.ConsumeIntegralInRange<int>(1, 100);
        int height = fdp.ConsumeIntegralInRange<int>(1, 100);
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
        
        png_structp png_ptr = png_create_write_struct(
            PNG_LIBPNG_VER_STRING,
            NULL,
            png_error_callback,
            png_warning_callback);
        
        if (png_ptr == NULL) {
            return 0;
        }
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == NULL) {
            png_destroy_write_struct(&png_ptr, NULL);
            return 0;
        }
        
        // Setup custom write function (discards output)
        png_set_write_fn(png_ptr, NULL, png_write_to_memory, png_flush_memory);
        
        if (setjmp(png_jmpbuf(png_ptr))) {
            // Error occurred during PNG writing
            png_destroy_write_struct(&png_ptr, &info_ptr);
            // Continue
        } else {
            // Set PNG header
            png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth,
                        color_type, interlace_type,
                        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
            
            // Write info
            png_write_info(png_ptr, info_ptr);
            
            // Generate and write some image data
            if (width > 0 && height > 0) {
                png_size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
                if (row_bytes > 0 && row_bytes < 10000) {
                    std::vector<png_byte> row(row_bytes);
                    
                    // Fill row with fuzzer data
                    std::vector<uint8_t> row_data = fdp.ConsumeBytes<uint8_t>(row_bytes);
                    if (row_data.size() >= row_bytes) {
                        memcpy(row.data(), row_data.data(), row_bytes);
                        
                        // Write multiple rows
                        for (int y = 0; y < height && y < 10; y++) {
                            png_write_row(png_ptr, row.data());
                        }
                    }
                }
            }
            
            // Write end
            png_write_end(png_ptr, NULL);
            
            png_destroy_write_struct(&png_ptr, &info_ptr);
        }
    }
    
    // Test 3: Test the high-level png_image API if there's enough data left
    if (fdp.remaining_bytes() > 50) {
        png_image image;
        memset(&image, 0, sizeof(image));
        image.version = PNG_IMAGE_VERSION;
        
        // Consume remaining bytes as "PNG data" for this test
        std::vector<uint8_t> png_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        // Set some image parameters from fuzzer data
        if (png_data.size() > sizeof(png_uint_32) * 2) {
            png_uint_32* dims = (png_uint_32*)png_data.data();
            image.width = dims[0] % 1000;
            image.height = dims[1] % 1000;
            image.format = PNG_FORMAT_RGBA;
            
            // Test PNG_IMAGE_SIZE macro
            size_t image_size = PNG_IMAGE_SIZE(image);
            (void)image_size; // Use to avoid unused variable warning
            
            // Test PNG_IMAGE_ROW_STRIDE macro
            size_t row_stride = PNG_IMAGE_ROW_STRIDE(image);
            (void)row_stride;
        }
    }
    
    return 0;
}

} // namespace harness_000
