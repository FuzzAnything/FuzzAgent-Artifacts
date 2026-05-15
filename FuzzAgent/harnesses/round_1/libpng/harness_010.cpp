#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

namespace harness_010 {

// Buffer structure for memory I/O
struct mem_buffer {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

// Error callback
static void png_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

// Warning callback
static void png_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
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

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful PNG read operations
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Prepare memory buffer for custom read function
    mem_buffer buffer;
    buffer.data = data;
    buffer.size = size;
    buffer.offset = 0;
    
    // Create read structure
    png_structp png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING,
        NULL,
        png_error_callback,
        png_warning_callback);
    
    if (png_ptr == NULL) {
        return 0;
    }
    
    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    // Set up error handling via setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during PNG processing
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Setup custom read function
    png_set_read_fn(png_ptr, &buffer, png_read_from_memory);
    
    // Consume operation type from fuzzer input
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // 1. SETUP VARIOUS PNG_SET_* TRANSFORMATIONS (targeting uncovered functions)
    
    // png_set_sig_bytes - set number of signature bytes already read
    int sig_bytes = fdp.ConsumeIntegralInRange<int>(0, 8);
    png_set_sig_bytes(png_ptr, sig_bytes);
    
    // png_set_crc_action - handle CRC errors (32 undiscovered branches)
    int crit_action = fdp.ConsumeIntegralInRange<int>(PNG_CRC_DEFAULT, PNG_CRC_NO_CHANGE);
    int ancil_action = fdp.ConsumeIntegralInRange<int>(PNG_CRC_DEFAULT, PNG_CRC_NO_CHANGE);
    png_set_crc_action(png_ptr, crit_action, ancil_action);
    
    // png_set_filter - set filtering method (61 undiscovered branches)
    int filter_method = fdp.ConsumeIntegralInRange<int>(0, 5);
    png_set_filter(png_ptr, 0, filter_method);
    
    // png_set_compression_buffer_size - set compression buffer size (29 undiscovered branches)
    size_t comp_buf_size = fdp.ConsumeIntegralInRange<size_t>(1024, 65536);
    png_set_compression_buffer_size(png_ptr, comp_buf_size);
    
    // Read PNG header info
    png_read_info(png_ptr, info_ptr);
    
    // Get image dimensions
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Update info based on operation type
    switch (operation) {
        case 0: {
            // PRIMARY TARGET: png_read_png with various transforms
            // Consume transforms from fuzzer input
            int transforms = 0;
            
            // Build transforms based on fuzzer input
            if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_IDENTITY;
            if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_STRIP_16;
            if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_STRIP_ALPHA;
            if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_PACKING;
            if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_PACKSWAP;
            if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_EXPAND;
            if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_INVERT_MONO;
            if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_SHIFT;
            if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_BGR;
            if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_SWAP_ALPHA;
            if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_SWAP_ENDIAN;
            if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_INVERT_ALPHA;
            if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_STRIP_FILLER;
            if (fdp.ConsumeBool()) transforms |= PNG_TRANSFORM_GRAY_TO_RGB;
            
            // Call png_read_png - primary target with 243 undiscovered branches
            png_read_png(png_ptr, info_ptr, transforms, NULL);
            
            // Test png_set_rows if we have valid image data (34 undiscovered branches)
            if (width > 0 && height > 0) {
                // Allocate row pointers
                png_bytep* row_pointers = (png_bytep*)png_malloc(png_ptr, height * sizeof(png_bytep));
                if (row_pointers) {
                    for (png_uint_32 y = 0; y < height; y++) {
                        row_pointers[y] = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
                    }
                    png_set_rows(png_ptr, info_ptr, row_pointers);
                    // Free allocated memory
                    for (png_uint_32 y = 0; y < height; y++) {
                        png_free(png_ptr, row_pointers[y]);
                    }
                    png_free(png_ptr, row_pointers);
                }
            }
            break;
        }
        
        case 1: {
            // SECONDARY TARGET: png_read_rows (59 undiscovered branches)
            if (width > 0 && height > 0) {
                // Allocate row buffers
                png_bytep row = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
                png_bytep display_row = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
                
                if (row && display_row) {
                    // Read rows one by one
                    for (png_uint_32 y = 0; y < height; y++) {
                        png_read_rows(png_ptr, &row, &display_row, 1);
                    }
                }
                
                if (row) png_free(png_ptr, row);
                if (display_row) png_free(png_ptr, display_row);
            }
            break;
        }
        
        case 2: {
            // TERTIARY TARGET: png_read_frame_head for APNG (42 undiscovered branches)
            // First check if we have APNG data by trying to get APNG info
            #ifdef PNG_APNG_SUPPORTED
            png_uint_32 num_frames = 0;
            png_uint_32 num_plays = 0;
            
            // Try to get APNG control chunk
            if (png_get_acTL(png_ptr, info_ptr, &num_frames, &num_plays)) {
                // We have APNG data, try to read frame head
                png_read_frame_head(png_ptr, info_ptr);
                
                // Try to get frame info
                png_uint_32 frame_width, frame_height, x_offset, y_offset;
                png_uint_16 delay_num, delay_den;
                png_byte dispose_op, blend_op;
                
                if (png_get_next_frame_fcTL(png_ptr, info_ptr,
                                           &frame_width, &frame_height,
                                           &x_offset, &y_offset,
                                           &delay_num, &delay_den,
                                           &dispose_op, &blend_op)) {
                    // Successfully got frame info
                }
            }
            #endif
            break;
        }
    }
    
    // 4. METADATA RETRIEVAL: Test various png_get_* functions (targeting pngget.c low coverage)
    
    // Get physical dimensions if available
    png_uint_32 res_x, res_y;
    int unit_type;
    if (png_get_pHYs(png_ptr, info_ptr, &res_x, &res_y, &unit_type)) {
        // Got physical dimensions
        if (unit_type == PNG_RESOLUTION_METER) {
            // Try to get DPI
            png_uint_32 x_dpi = 0, y_dpi = 0;
            #ifdef PNG_pHYs_SUPPORTED
            x_dpi = png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
            #endif
        }
    }
    
    // Get text chunks if available
    png_textp text_ptr = NULL;
    int num_text = 0;
    if (png_get_text(png_ptr, info_ptr, &text_ptr, &num_text) > 0) {
        // Text chunks available
    }
    
    // Get time if available
    png_timep mod_time;
    if (png_get_tIME(png_ptr, info_ptr, &mod_time)) {
        // Time chunk available
    }
    
    // Get palette if available
    png_colorp palette = NULL;
    int num_palette = 0;
    if (png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette)) {
        // Palette available
    }
    
    // Get significant bits if available
    png_color_8p sig_bits;
    if (png_get_sBIT(png_ptr, info_ptr, &sig_bits)) {
        // Significant bits available
    }
    
    // Get background color if available
    png_color_16p background;
    if (png_get_bKGD(png_ptr, info_ptr, &background)) {
        // Background color available
    }
    
    // Get histogram if available
    png_uint_16p hist;
    if (png_get_hIST(png_ptr, info_ptr, &hist)) {
        // Histogram available
    }
    
    // Get transparency if available
    png_bytep trans = NULL;
    int num_trans = 0;
    png_color_16p trans_values = NULL;
    if (png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans, &trans_values)) {
        // Transparency available
    }
    
    // 5. CLEANUP
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    return 0;
}

} // namespace harness_010
