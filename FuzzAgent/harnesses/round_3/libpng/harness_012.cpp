#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>

namespace harness_012 {

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

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // PNG signature is 8 bytes, need minimum data for testing
    if (size < 8) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // PHASE 1: INITIALIZATION
    
    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                  nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        return 0;
    }
    
    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Set up memory-based read data structure
    png_read_memory_data read_data;
    read_data.data = data;
    read_data.size = size;
    read_data.offset = 0;
    
    // Set up the read function with our memory-based callback
    png_set_read_fn(png_ptr, &read_data, png_read_memory_callback);
    
    // Check if the data starts with PNG signature
    bool has_png_signature = false;
    if (size >= 8) {
        has_png_signature = png_sig_cmp((png_bytep)data, 0, 8) == 0;
    }
    
    // Set signature bytes if PNG signature is present
    if (has_png_signature) {
        png_set_sig_bytes(png_ptr, 8);
        read_data.offset = 8; // Skip the signature bytes
    }
    
    // PHASE 2: PNG HEADER READING AND TRANSFORM CONFIGURATION
    
    // Read PNG info to process chunks
    png_read_info(png_ptr, info_ptr);
    
    // Get image dimensions and color info
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Use fuzzer input to decide which transforms to apply
    uint8_t transform_flags = fdp.ConsumeIntegral<uint8_t>();
    
    // Apply various transform functions to increase coverage
    if (transform_flags & 0x01) {
        png_set_expand(png_ptr);  // Expand palettes to RGB, grayscale to 8 bits
    }
    
    if (transform_flags & 0x02) {
        png_set_gray_to_rgb(png_ptr);  // Convert grayscale to RGB
    }
    
    if (transform_flags & 0x04) {
        png_set_interlace_handling(png_ptr);  // Handle interlaced images
    }
    
    if (transform_flags & 0x08) {
        png_set_strip_16(png_ptr);  // Strip 16-bit samples to 8-bit
    }
    
    if (transform_flags & 0x10) {
        png_set_packing(png_ptr);  // Expand 1, 2, 4-bit samples to bytes
    }
    
    // Update transformations based on the transforms we set
    png_read_update_info(png_ptr, info_ptr);
    
    // Get updated rowbytes after transformations
    size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    // PHASE 3: APNG SUPPORT (if enabled and if it's an APNG file)
    
#ifdef PNG_APNG_SUPPORTED
    // Check if this is an APNG file
    png_uint_32 num_frames = 0;
    png_uint_32 num_plays = 0;
    png_uint_32 apng_flags = png_get_acTL(png_ptr, info_ptr, &num_frames, &num_plays);
    
    // If it's an APNG, read frame head information
    if (apng_flags & PNG_INFO_acTL) {
        // Primary APNG frame reading target: png_read_frame_head
        png_read_frame_head(png_ptr, info_ptr);
        
        // Get frame information
        png_uint_32 frame_width = png_get_next_frame_width(png_ptr, info_ptr);
        png_uint_32 frame_height = png_get_next_frame_height(png_ptr, info_ptr);
        png_uint_32 x_offset = png_get_next_frame_x_offset(png_ptr, info_ptr);
        png_uint_32 y_offset = png_get_next_frame_y_offset(png_ptr, info_ptr);
        png_uint_16 delay_num = png_get_next_frame_delay_num(png_ptr, info_ptr);
        png_uint_16 delay_den = png_get_next_frame_delay_den(png_ptr, info_ptr);
        png_byte dispose_op = png_get_next_frame_dispose_op(png_ptr, info_ptr);
        png_byte blend_op = png_get_next_frame_blend_op(png_ptr, info_ptr);
        
        (void)frame_width; (void)frame_height; (void)x_offset; (void)y_offset;
        (void)delay_num; (void)delay_den; (void)dispose_op; (void)blend_op;
    }
#endif
    
    // PHASE 4: IMAGE DATA READING - PRIMARY TARGETS
    
    // Allocate row pointers for png_read_image
    png_bytep* row_pointers = (png_bytep*)malloc(height * sizeof(png_bytep));
    if (row_pointers == nullptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Allocate memory for each row
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(rowbytes);
        if (row_pointers[y] == nullptr) {
            // Clean up already allocated rows
            for (png_uint_32 i = 0; i < y; i++) {
                free(row_pointers[i]);
            }
            free(row_pointers);
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            return 0;
        }
    }
    
    // PRIMARY TARGET: png_read_image - read the whole image into memory
    png_read_image(png_ptr, row_pointers);
    
    // Access some pixel data to ensure it's been read
    if (height > 0 && rowbytes > 0) {
        for (png_uint_32 y = 0; y < height && y < 5; y++) {
            if (row_pointers[y] != nullptr) {
                // Access first few bytes of each row
                volatile png_byte dummy = row_pointers[y][0];
                (void)dummy;
            }
        }
    }
    
    // PHASE 5: COMPLETE READING - SECONDARY TARGET
    
    // SECONDARY TARGET: png_read_end - complete the reading process
    png_read_end(png_ptr, info_ptr);
    
    // PHASE 6: CLEANUP
    
    // Free allocated row memory
    for (png_uint_32 y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);
    
    // Clean up PNG structures
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}

} // namespace harness_012
