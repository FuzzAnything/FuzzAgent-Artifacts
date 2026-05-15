#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <zlib.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_026 {

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
    
    if (!buffer) {
        png_error(png_ptr, "Write buffer is null");
        return;
    }
    
    // Append data to buffer
    buffer->data.insert(buffer->data.end(), data, data + length);
    buffer->offset += length;
}

static void pngtest_flush_data(png_structp png_ptr) {
    (void)png_ptr;
}

// User transform callback for writing
static void user_write_transform_callback(png_structp png_ptr, png_row_infop row_info, png_bytep data) {
    // Validate parameters
    if (png_ptr == NULL) {
        png_error(png_ptr, "user_write_transform_callback: bad png_ptr");
        return;
    }
    if (row_info == NULL) {
        png_error(png_ptr, "user_write_transform_callback: bad row_info");
        return;
    }
    if (data == NULL) {
        png_error(png_ptr, "user_write_transform_callback: bad data");
        return;
    }
    
    // Get user transform data
    void* user_transform_ptr = png_get_user_transform_ptr(png_ptr);
    
    // Simple operation to exercise the callback - use transform parameters if available
    png_uint_32 width = row_info->width;
    png_size_t rowbytes = row_info->rowbytes;
    
    // Get a transform value from user data if available
    uint8_t transform_value = 0x00; // Default to no transform
    if (user_transform_ptr != NULL) {
        // Use first byte of user data as transform value
        uint8_t* user_data = (uint8_t*)user_transform_ptr;
        transform_value = user_data[0];
    }
    
    // Apply simple transform using the value (only if non-zero to avoid no-op)
    if (transform_value != 0) {
        for (png_size_t i = 0; i < rowbytes; i++) {
            data[i] ^= transform_value;
        }
    }
}

// Write status callback
static void write_status_callback(png_structp png_ptr, png_uint_32 row_number, int pass) {
    // Validate parameters
    if (png_ptr == NULL) {
        png_error(png_ptr, "write_status_callback: bad png_ptr");
        return;
    }
    if (row_number > PNG_UINT_31_MAX) {
        png_error(png_ptr, "write_status_callback: bad row number");
        return;
    }
    if (pass < 0 || pass > 7) {
        png_error(png_ptr, "write_status_callback: bad pass");
        return;
    }
    
    // Status callback - just a placeholder to exercise the function
    // In a real application, this could update progress indicators
    (void)row_number;
    (void)pass;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0; // Need minimum input for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters for PNG creation
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1024);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1024);
    int bit_depth = fdp.ConsumeIntegralInRange<int>(1, 16);
    int color_type = fdp.ConsumeIntegralInRange<int>(0, 6);
    int interlace_type = fdp.ConsumeIntegralInRange<int>(0, 1);
    int compression_type = fdp.ConsumeIntegralInRange<int>(0, 1);
    int filter_type = fdp.ConsumeIntegralInRange<int>(0, 5);
    
    // Consume user transform parameters
    int user_transform_depth = fdp.ConsumeIntegralInRange<int>(1, 16);
    int user_transform_channels = fdp.ConsumeIntegralInRange<int>(1, 4);
    
    // Remaining data for user transform pointer simulation
    std::vector<uint8_t> user_data = fdp.ConsumeRemainingBytes<uint8_t>();
    void* user_transform_ptr = user_data.empty() ? NULL : (void*)user_data.data();
    
    // Step 1: Create write structure
    png_structp png_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        NULL,
        pngtest_error,
        pngtest_warning);
    
    if (!png_ptr) {
        return 0;
    }
    
    // Step 2: Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Step 3: Set up custom I/O writing functions
    png_memory_buffer buffer;
    buffer.offset = 0;
    png_set_write_fn(png_ptr, &buffer, pngtest_write_data, pngtest_flush_data);
    
    // Step 4: Set image dimensions and format
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);
    
    // Step 5: Register user transform callback for writing
    #ifdef PNG_WRITE_USER_TRANSFORM_SUPPORTED
    png_set_write_user_transform_fn(png_ptr, user_write_transform_callback);
    #endif
    
    // Step 6: Set user transform information
    #ifdef PNG_USER_TRANSFORM_PTR_SUPPORTED
    png_set_user_transform_info(png_ptr, user_transform_ptr,
                                user_transform_depth, user_transform_channels);
    #endif
    
    // Step 7: Register write status callback
    #ifdef PNG_WRITE_SUPPORTED
    png_set_write_status_fn(png_ptr, write_status_callback);
    #endif
    
    // Step 8: Write PNG header
    png_write_info(png_ptr, info_ptr);
    
    // Step 9: Verify callback registration was successful
    #ifdef PNG_USER_TRANSFORM_PTR_SUPPORTED
    void* retrieved_ptr = png_get_user_transform_ptr(png_ptr);
    (void)retrieved_ptr; // Could compare with user_transform_ptr
    #endif
    
    // Create some sample image data to write (simplified for fuzzing)
    png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    std::vector<png_byte> row_data(rowbytes);
    
    // Fill row with some data
    for (png_size_t i = 0; i < rowbytes && i < user_data.size(); i++) {
        row_data[i] = user_data[i % user_data.size()];
    }
    
    png_bytep row_pointer = row_data.data();
    
    // Write some rows to trigger callbacks
    for (png_uint_32 y = 0; y < height && y < 10; y++) {
        png_write_row(png_ptr, row_pointer);
    }
    
    // Write end of PNG
    png_write_end(png_ptr, info_ptr);
    
    // Step 10: Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}

} // namespace harness_026
