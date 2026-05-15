#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_021 {

// Structure to hold memory reading callback data
struct png_read_memory_data {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

// Read callback function for memory-based input
static void png_read_memory_callback(png_structp png_ptr, png_bytep out_bytes, png_size_t byte_count) {
    png_read_memory_data* read_data = (png_read_memory_data*)png_get_io_ptr(png_ptr);
    
    if (read_data->offset + byte_count > read_data->size) {
        png_error(png_ptr, "Insufficient data in memory buffer");
        return;
    }
    
    memcpy(out_bytes, read_data->data + read_data->offset, byte_count);
    read_data->offset += byte_count;
}

// Structure to hold memory writing callback data
struct png_write_memory_data {
    uint8_t* buffer;
    size_t capacity;
    size_t size;
};

// Write callback function for memory-based output
static void png_write_memory_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_write_memory_data* write_data = (png_write_memory_data*)png_get_io_ptr(png_ptr);
    
    if (write_data->size + length > write_data->capacity) {
        // Reallocate buffer if needed
        size_t new_capacity = write_data->capacity * 2;
        if (new_capacity < write_data->size + length) {
            new_capacity = write_data->size + length;
        }
        uint8_t* new_buffer = (uint8_t*)realloc(write_data->buffer, new_capacity);
        if (new_buffer == NULL) {
            png_error(png_ptr, "Memory allocation failed in write callback");
            return;
        }
        write_data->buffer = new_buffer;
        write_data->capacity = new_capacity;
    }
    
    memcpy(write_data->buffer + write_data->size, data, length);
    write_data->size += length;
}

// Flush callback function (optional)
static void png_flush_callback(png_structp png_ptr) {
    // Nothing to do for memory-based output
    (void)png_ptr;
}

// User transform function for testing png_set_user_transform_info
static void user_transform_function(png_structp png_ptr, png_row_infop row_info, png_bytep data) {
    // Simple transform: invert bits for demonstration
    if (png_ptr == NULL || data == NULL) return;
    
    png_uint_32 rowbytes = row_info->rowbytes;
    for (png_uint_32 i = 0; i < rowbytes; i++) {
        data[i] = ~data[i];  // Invert bits
    }
    
    (void)png_ptr;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for comprehensive transformation testing
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ====================================================
    // PHASE 1: CREATE PNG STRUCTURES FOR WRITING
    // ====================================================
    
    // Create PNG write structure
    png_structp write_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                         nullptr, nullptr, nullptr);
    if (write_png_ptr == nullptr) {
        return 0;
    }
    
    // Create PNG info structure for writing
    png_infop write_info_ptr = png_create_info_struct(write_png_ptr);
    if (write_info_ptr == nullptr) {
        png_destroy_write_struct(&write_png_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling for writing
    if (setjmp(png_jmpbuf(write_png_ptr))) {
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        return 0;
    }
    
    // Set up memory-based writing
    png_write_memory_data write_data;
    write_data.buffer = nullptr;
    write_data.capacity = 8192;
    write_data.size = 0;
    
    write_data.buffer = (uint8_t*)malloc(write_data.capacity);
    if (write_data.buffer == nullptr) {
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        return 0;
    }
    
    png_set_write_fn(write_png_ptr, &write_data, png_write_memory_callback, png_flush_callback);
    
    // ====================================================
    // PHASE 2: CONSUME FUZZER DATA FOR TRANSFORMATION PARAMETERS
    // ====================================================
    
    // Consume parameters for various transformation functions
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 12;
    
    // Consume png_color_8 structure for png_set_shift
    png_color_8 shift_bits;
    shift_bits.red = fdp.ConsumeIntegralInRange<png_byte>(1, 16);
    shift_bits.green = fdp.ConsumeIntegralInRange<png_byte>(1, 16);
    shift_bits.blue = fdp.ConsumeIntegralInRange<png_byte>(1, 16);
    shift_bits.gray = fdp.ConsumeIntegralInRange<png_byte>(1, 16);
    shift_bits.alpha = fdp.ConsumeIntegralInRange<png_byte>(1, 16);
    
    // Consume parameters for png_set_filler and png_set_add_alpha
    png_uint_32 filler = fdp.ConsumeIntegral<png_uint_32>();
    int filler_loc = fdp.ConsumeIntegralInRange<int>(0, 1); // 0=after, 1=before
    
    // Consume parameters for user transform info
    int user_transform_depth = fdp.ConsumeIntegralInRange<int>(1, 16);
    int user_transform_channels = fdp.ConsumeIntegralInRange<int>(1, 4);
    
    // Create user transform data structure
    struct user_transform_data {
        int dummy1;
        float dummy2;
        char dummy3[16];
    };
    user_transform_data user_data;
    user_data.dummy1 = fdp.ConsumeIntegral<int>();
    user_data.dummy2 = fdp.ConsumeFloatingPoint<float>();
    std::string dummy_str = fdp.ConsumeRandomLengthString(15);
    strncpy(user_data.dummy3, dummy_str.c_str(), sizeof(user_data.dummy3) - 1);
    user_data.dummy3[sizeof(user_data.dummy3) - 1] = '\0';
    
    // Consume image dimensions and color type
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 128);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 128);
    int color_type_idx = fdp.ConsumeIntegralInRange<int>(0, 6);
    png_byte color_type;
    switch (color_type_idx) {
        case 0: color_type = PNG_COLOR_TYPE_GRAY; break;
        case 1: color_type = PNG_COLOR_TYPE_GRAY_ALPHA; break;
        case 2: color_type = PNG_COLOR_TYPE_PALETTE; break;
        case 3: color_type = PNG_COLOR_TYPE_RGB; break;
        case 4: color_type = PNG_COLOR_TYPE_RGB_ALPHA; break;
        case 5: color_type = PNG_COLOR_MASK_COLOR; break;
        default: color_type = PNG_COLOR_TYPE_RGB; break;
    }
    
    png_byte bit_depth = fdp.PickValueInArray({1, 2, 4, 8, 16});
    
    // Set basic PNG header for writing
    png_set_IHDR(write_png_ptr, write_info_ptr,
                 width, height,
                 bit_depth,
                 color_type,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);
    
    // ====================================================
    // PHASE 3: APPLY TRANSFORMATION FUNCTIONS BASED ON TEST SCENARIO
    // ====================================================
    
    // Apply different transformation functions based on test scenario
    switch (test_scenario) {
        case 0:
            // Test png_set_shift - PRIMARY TARGET
            png_set_shift(write_png_ptr, &shift_bits);
            break;
            
        case 1:
            // Test png_set_user_transform_info - PRIMARY TARGET
            png_set_user_transform_info(write_png_ptr, &user_data, 
                                        user_transform_depth, user_transform_channels);
            // Also set user transform function
            png_set_read_user_transform_fn(write_png_ptr, user_transform_function);
            break;
            
        case 2:
            // Test png_set_bgr
            png_set_bgr(write_png_ptr);
            break;
            
        case 3:
            // Test png_set_swap (only effective for 16-bit depth)
            png_set_swap(write_png_ptr);
            break;
            
        case 4:
            // Test png_set_packing (only effective for bit_depth < 8)
            png_set_packing(write_png_ptr);
            break;
            
        case 5:
            // Test png_set_packswap (only effective for bit_depth < 8)
            png_set_packswap(write_png_ptr);
            break;
            
        case 6:
            // Test png_set_filler
            png_set_filler(write_png_ptr, filler, filler_loc);
            break;
            
        case 7:
            // Test png_set_add_alpha
            png_set_add_alpha(write_png_ptr, filler, filler_loc);
            break;
            
        case 8:
            // Test png_set_swap_alpha
            png_set_swap_alpha(write_png_ptr);
            break;
            
        case 9:
            // Test png_set_invert_alpha
            png_set_invert_alpha(write_png_ptr);
            break;
            
        case 10:
            // Test png_set_invert_mono
            png_set_invert_mono(write_png_ptr);
            break;
            
        case 11:
            // Test multiple transformations together
            png_set_bgr(write_png_ptr);
            png_set_swap(write_png_ptr);
            png_set_shift(write_png_ptr, &shift_bits);
            png_set_user_transform_info(write_png_ptr, &user_data, 
                                        user_transform_depth, user_transform_channels);
            break;
    }
    
    // Write PNG info
    png_write_info(write_png_ptr, write_info_ptr);
    
    // Create and write some dummy row data
    size_t rowbytes = png_get_rowbytes(write_png_ptr, write_info_ptr);
    std::vector<png_byte> row_data(rowbytes);
    
    // Fill row with fuzzer data
    std::vector<uint8_t> row_fill_data = fdp.ConsumeBytes<uint8_t>(rowbytes);
    for (size_t i = 0; i < rowbytes && i < row_fill_data.size(); i++) {
        row_data[i] = row_fill_data[i];
    }
    
    png_bytep row_pointer = row_data.data();
    
    // Write a few rows
    for (png_uint_32 y = 0; y < height && y < 5; y++) {
        png_write_row(write_png_ptr, row_pointer);
    }
    
    // End writing
    png_write_end(write_png_ptr, write_info_ptr);
    
    // ====================================================
    // PHASE 4: CREATE PNG STRUCTURES FOR READING
    // ====================================================
    
    // Now test transformations in reading context
    if (write_data.size > 8) {
        // Create PNG read structure
        png_structp read_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                          nullptr, nullptr, nullptr);
        if (read_png_ptr == nullptr) {
            free(write_data.buffer);
            png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
            return 0;
        }
        
        // Create PNG info structure for reading
        png_infop read_info_ptr = png_create_info_struct(read_png_ptr);
        if (read_info_ptr == nullptr) {
            png_destroy_read_struct(&read_png_ptr, nullptr, nullptr);
            free(write_data.buffer);
            png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
            return 0;
        }
        
        // Set up error handling for reading
        if (setjmp(png_jmpbuf(read_png_ptr))) {
            png_destroy_read_struct(&read_png_ptr, &read_info_ptr, nullptr);
            free(write_data.buffer);
            png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
            return 0;
        }
        
        // Set up memory-based reading of the written PNG data
        png_read_memory_data read_data;
        read_data.data = write_data.buffer;
        read_data.size = write_data.size;
        read_data.offset = 0;
        
        png_set_read_fn(read_png_ptr, &read_data, png_read_memory_callback);
        
        // Set signature bytes
        png_set_sig_bytes(read_png_ptr, 0);
        
        // Read PNG info
        png_read_info(read_png_ptr, read_info_ptr);
        
        // Get image info
        png_uint_32 read_width = png_get_image_width(read_png_ptr, read_info_ptr);
        png_uint_32 read_height = png_get_image_height(read_png_ptr, read_info_ptr);
        png_byte read_color_type = png_get_color_type(read_png_ptr, read_info_ptr);
        png_byte read_bit_depth = png_get_bit_depth(read_png_ptr, read_info_ptr);
        
        // Apply transformations during reading
        if (read_bit_depth == 16) {
            png_set_swap(read_png_ptr);
        }
        
        if (read_bit_depth < 8) {
            png_set_packing(read_png_ptr);
        }
        
        // Test png_set_interlace_handling
        int num_passes = png_set_interlace_handling(read_png_ptr);
        
        // Update info after transformations
        png_read_update_info(read_png_ptr, read_info_ptr);
        
        // Clean up read structures
        png_destroy_read_struct(&read_png_ptr, &read_info_ptr, nullptr);
    }
    
    // ====================================================
    // PHASE 5: CLEANUP
    // ====================================================
    
    free(write_data.buffer);
    png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
    
    return 0;
}

} // namespace harness_021
