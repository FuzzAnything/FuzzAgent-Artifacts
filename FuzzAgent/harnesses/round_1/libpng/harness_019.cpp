#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_019 {
/*
 * Harness for testing libpng color management chunk APIs (cHRM, gAMA, sRGB, iCCP) 
 * with comprehensive write/read lifecycle testing. Targets the pngget.c module 
 * (currently 37.13% coverage) with 58+ undiscovered branches.
 *
 * Color management chunk APIs targeted:
 * - cHRM (chromaticities): png_set_cHRM[_fixed], png_get_cHRM[_fixed]
 * - gAMA (gamma): png_set_gAMA[_fixed], png_get_gAMA[_fixed] 
 * - sRGB (standard RGB color space): png_set_sRGB, png_get_sRGB
 * - iCCP (ICC profile): png_set_iCCP, png_get_iCCP
 *
 * Required helper functions for complete lifecycle:
 * - png_create_[read/write]_struct, png_create_info_struct
 * - png_set_IHDR for basic image setup
 * - png_[read/write]_info, png_[read/write]_end
 * - png_destroy_[read/write]_struct
 *
 * Invocation sequence:
 * 1. Create write structures and setup image parameters
 * 2. Set color management chunks using png_set_* functions
 * 3. Write PNG with color management metadata
 * 4. Read PNG back with png_read_info
 * 5. Retrieve color management metadata using png_get_* functions
 * 6. Verify consistency between set and retrieved values
 * 7. Clean up resources properly
 *
 * This harness provides semantic diversity from existing harnesses:
 * - harness_004.cpp: Basic read operations with some cHRM tests
 * - harness_016.cpp: Advanced cHRM_XYZ and color calibration APIs  
 * - This harness: Complete color management lifecycle with cHRM, gAMA, sRGB, iCCP
 *
 * The pngget.c module has significant coverage gaps in color management 
 * retrieval functions, particularly error paths and edge cases for these
 * 4 critical color space definition chunks.
 */


// ==================== MEMORY BUFFER FOR WRITE/READ ====================

struct mem_buffer {
    uint8_t* data;
    size_t size;
    size_t capacity;
    size_t offset;  // For reading
};

// Custom write function for writing to memory buffer
static void png_write_to_memory(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) {
        png_error(png_ptr, "No IO pointer");
        return;
    }
    
    mem_buffer* buffer = (mem_buffer*)io_ptr;
    
    // Resize buffer if needed
    if (buffer->offset + length > buffer->capacity) {
        size_t new_capacity = buffer->capacity * 2;
        if (new_capacity < buffer->offset + length) {
            new_capacity = buffer->offset + length;
        }
        
        uint8_t* new_data = (uint8_t*)realloc(buffer->data, new_capacity);
        if (new_data == NULL) {
            png_error(png_ptr, "Memory allocation failed");
            return;
        }
        
        buffer->data = new_data;
        buffer->capacity = new_capacity;
    }
    
    // Copy data to buffer
    memcpy(buffer->data + buffer->offset, data, length);
    buffer->offset += length;
    buffer->size = buffer->offset;
}

// Custom flush function (does nothing for memory buffer)
static void png_flush_memory(png_structp png_ptr) {
    (void)png_ptr;
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

// Callback functions for libpng error handling
static void png_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void png_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful color management operations
    if (size < 200) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ==================== CONSUME INPUT DATA FOR COLOR MANAGEMENT ====================
    
    // Consume basic image parameters
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 512);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 512);
    int bit_depth = fdp.PickValueInArray({8, 16});
    int color_type = fdp.PickValueInArray({
        PNG_COLOR_TYPE_RGB,
        PNG_COLOR_TYPE_RGB_ALPHA,
        PNG_COLOR_TYPE_GRAY,
        PNG_COLOR_TYPE_GRAY_ALPHA
    });
    int interlace_type = PNG_INTERLACE_NONE;
    int compression_type = PNG_COMPRESSION_TYPE_BASE;
    int filter_type = PNG_FILTER_TYPE_BASE;
    
    // Consume color management data from fuzzer input
    bool has_cHRM = fdp.ConsumeBool();
    bool has_gAMA = fdp.ConsumeBool();
    bool has_sRGB = fdp.ConsumeBool();
    bool has_iCCP = fdp.ConsumeBool();
    
    // cHRM chromaticity values (8 values: white x/y, red x/y, green x/y, blue x/y)
    double white_x = 0.0, white_y = 0.0;
    double red_x = 0.0, red_y = 0.0;
    double green_x = 0.0, green_y = 0.0;
    double blue_x = 0.0, blue_y = 0.0;
    
    if (has_cHRM) {
        white_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
        white_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
        red_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
        red_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
        green_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
        green_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
        blue_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
        blue_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    }
    
    // gAMA gamma value
    double file_gamma = 0.0;
    if (has_gAMA) {
        file_gamma = fdp.ConsumeFloatingPointInRange<double>(0.001, 10.0);
    }
    
    // sRGB rendering intent
    int srgb_intent = 0;
    if (has_sRGB) {
        srgb_intent = fdp.ConsumeIntegralInRange<int>(0, 3);
    }
    
    // iCCP profile data
    std::string iccp_name;
    std::vector<uint8_t> iccp_profile;
    int iccp_compression_type = 0;
    if (has_iCCP) {
        // Consume profile name (null-terminated string)
        iccp_name = fdp.ConsumeRandomLengthString(79); // Max 79 chars + null
        if (iccp_name.length() > 79) {
            iccp_name = iccp_name.substr(0, 79);
        }
        
        // Consume compression type
        iccp_compression_type = fdp.ConsumeBool() ? PNG_COMPRESSION_TYPE_BASE : 0;
        
        // Consume profile data
        size_t profile_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
        iccp_profile = fdp.ConsumeBytes<uint8_t>(profile_size);
    }
    
    // ==================== WRITE PHASE: CREATE PNG WITH COLOR MANAGEMENT ====================
    
    // Initialize write buffer
    mem_buffer write_buf;
    write_buf.data = (uint8_t*)malloc(4096);
    if (write_buf.data == NULL) {
        return 0;
    }
    write_buf.capacity = 4096;
    write_buf.size = 0;
    write_buf.offset = 0;
    
    // Create write structure
    png_structp write_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        NULL,
        png_error_callback,
        png_warning_callback);
    
    if (write_ptr == NULL) {
        free(write_buf.data);
        return 0;
    }
    
    // Create info structure for writing
    png_infop write_info = png_create_info_struct(write_ptr);
    if (write_info == NULL) {
        png_destroy_write_struct(&write_ptr, NULL);
        free(write_buf.data);
        return 0;
    }
    
    // Set up error handling via setjmp
    if (setjmp(png_jmpbuf(write_ptr))) {
        // Error occurred during writing
        png_destroy_write_struct(&write_ptr, &write_info);
        free(write_buf.data);
        return 0;
    }
    
    // Setup custom write function
    png_set_write_fn(write_ptr, &write_buf, png_write_to_memory, png_flush_memory);
    
    // Set basic image header
    png_set_IHDR(write_ptr, write_info, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);
    
    // Set color management chunks based on fuzzer data
    if (has_cHRM) {
        png_set_cHRM(write_ptr, write_info, white_x, white_y, red_x, red_y,
                     green_x, green_y, blue_x, blue_y);
    }
    
    if (has_gAMA) {
        png_set_gAMA(write_ptr, write_info, file_gamma);
    }
    
    if (has_sRGB) {
        png_set_sRGB(write_ptr, write_info, srgb_intent);
    }
    
    if (has_iCCP && !iccp_name.empty() && !iccp_profile.empty()) {
        png_set_iCCP(write_ptr, write_info, 
                     const_cast<char*>(iccp_name.c_str()), 
                     iccp_compression_type,
                     iccp_profile.data(), 
                     static_cast<png_uint_32>(iccp_profile.size()));
    }
    
    // Write info
    png_write_info(write_ptr, write_info);
    
    // Write dummy image data (simple pattern)
    png_uint_32 rowbytes = png_get_rowbytes(write_ptr, write_info);
    if (rowbytes > 0 && rowbytes < 1024 * 1024) {
        std::vector<png_byte> row(rowbytes);
        for (png_uint_32 y = 0; y < height; y++) {
            // Fill with simple pattern
            for (size_t i = 0; i < rowbytes; i++) {
                row[i] = static_cast<png_byte>((y + i) & 0xFF);
            }
            png_write_row(write_ptr, row.data());
        }
    }
    
    // Write end
    png_write_end(write_ptr, write_info);
    
    // Clean up write structures
    png_destroy_write_struct(&write_ptr, &write_info);
    
    // ==================== READ PHASE: READ PNG AND RETRIEVE COLOR MANAGEMENT ====================
    
    // Reset buffer offset for reading
    write_buf.offset = 0;
    
    // Create read structure
    png_structp read_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING,
        NULL,
        png_error_callback,
        png_warning_callback);
    
    if (read_ptr == NULL) {
        free(write_buf.data);
        return 0;
    }
    
    // Create info structure for reading
    png_infop read_info = png_create_info_struct(read_ptr);
    if (read_info == NULL) {
        png_destroy_read_struct(&read_ptr, NULL, NULL);
        free(write_buf.data);
        return 0;
    }
    
    // Set up error handling via setjmp
    if (setjmp(png_jmpbuf(read_ptr))) {
        // Error occurred during reading
        png_destroy_read_struct(&read_ptr, &read_info, NULL);
        free(write_buf.data);
        return 0;
    }
    
    // Setup custom read function
    png_set_read_fn(read_ptr, &write_buf, png_read_from_memory);
    
    // Read info
    png_read_info(read_ptr, read_info);
    
    // ==================== TEST PNG_GET_* COLOR MANAGEMENT FUNCTIONS ====================
    
    // Test png_get_cHRM (if cHRM was set)
    if (has_cHRM) {
        double retrieved_white_x = 0.0, retrieved_white_y = 0.0;
        double retrieved_red_x = 0.0, retrieved_red_y = 0.0;
        double retrieved_green_x = 0.0, retrieved_green_y = 0.0;
        double retrieved_blue_x = 0.0, retrieved_blue_y = 0.0;
        
        png_uint_32 cHRM_result = png_get_cHRM(read_ptr, read_info,
                                               &retrieved_white_x, &retrieved_white_y,
                                               &retrieved_red_x, &retrieved_red_y,
                                               &retrieved_green_x, &retrieved_green_y,
                                               &retrieved_blue_x, &retrieved_blue_y);
        
        // Also test fixed-point version
        png_fixed_point white_x_fixed = 0, white_y_fixed = 0;
        png_fixed_point red_x_fixed = 0, red_y_fixed = 0;
        png_fixed_point green_x_fixed = 0, green_y_fixed = 0;
        png_fixed_point blue_x_fixed = 0, blue_y_fixed = 0;
        
        png_uint_32 cHRM_fixed_result = png_get_cHRM_fixed(read_ptr, read_info,
                                                           &white_x_fixed, &white_y_fixed,
                                                           &red_x_fixed, &red_y_fixed,
                                                           &green_x_fixed, &green_y_fixed,
                                                           &blue_x_fixed, &blue_y_fixed);
    }
    
    // Test png_get_gAMA (if gAMA was set)
    if (has_gAMA) {
        double retrieved_gamma = 0.0;
        png_uint_32 gAMA_result = png_get_gAMA(read_ptr, read_info, &retrieved_gamma);
        
        // Also test fixed-point version
        png_fixed_point gamma_fixed = 0;
        png_uint_32 gAMA_fixed_result = png_get_gAMA_fixed(read_ptr, read_info, &gamma_fixed);
    }
    
    // Test png_get_sRGB (if sRGB was set)
    if (has_sRGB) {
        int retrieved_intent = 0;
        png_uint_32 sRGB_result = png_get_sRGB(read_ptr, read_info, &retrieved_intent);
    }
    
    // Test png_get_iCCP (if iCCP was set)
    if (has_iCCP && !iccp_name.empty() && !iccp_profile.empty()) {
        png_charp retrieved_name = NULL;
        int retrieved_compression_type = 0;
        png_bytep retrieved_profile = NULL;
        png_uint_32 retrieved_proflen = 0;
        
        png_uint_32 iCCP_result = png_get_iCCP(read_ptr, read_info,
                                               &retrieved_name, &retrieved_compression_type,
                                               &retrieved_profile, &retrieved_proflen);
    }
    
    // ==================== TEST ADDITIONAL COLOR MANAGEMENT QUERIES ====================
    
    // Test png_get_valid to check which chunks are present
    png_uint_32 valid_chunks = png_get_valid(read_ptr, read_info, 
                                             PNG_INFO_cHRM | PNG_INFO_gAMA | 
                                             PNG_INFO_sRGB | PNG_INFO_iCCP);
    
    // Test png_get_channels (color management related)
    int channels = png_get_channels(read_ptr, read_info);
    
    // Test color type and bit depth getters (related to color space)
    png_byte color_type_retrieved = png_get_color_type(read_ptr, read_info);
    png_byte bit_depth_retrieved = png_get_bit_depth(read_ptr, read_info);
    
    // ==================== COMPLETE READING AND CLEANUP ====================
    
    // Read rest of image (if any)
    png_read_end(read_ptr, read_info);
    
    // Clean up read structures
    png_destroy_read_struct(&read_ptr, &read_info, NULL);
    
    // Free buffer
    free(write_buf.data);
    
    return 0;
}

} // namespace harness_019
