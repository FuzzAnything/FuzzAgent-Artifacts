#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_018 {
/*
 * Harness for testing libpng PNG writing operations targeting frame-by-frame 
 * and row-by-row writing based on coverage analysis guidance.
 * 
 * Targets high-complexity, currently uncovered APIs in pngwrite.c:
 * - png_write_rows (completely uncovered, 74 undiscovered branches)
 * - png_write_frame_head (completely uncovered, 16 undiscovered branches)
 * - png_write_frame_tail (minimally covered, needs deeper exploration)
 * - png_write_image (partially covered, 72 undiscovered branches)
 * - png_write_png (partially covered, 221 undiscovered branches)
 *
 * Required helper functions:
 * - png_create_write_struct, png_create_info_struct, png_init_io
 * - png_set_IHDR, png_set_acTL (for animation), png_set_next_frame_fcTL
 * - png_write_info, png_set_packing/png_set_swap, png_write_end
 * - png_destroy_write_struct, png_destroy_info_struct
 *
 * Invocation sequence:
 * 1. Create write and info structs
 * 2. Initialize I/O with memory buffers
 * 3. Set image header
 * 4. For animated PNGs set acTL and fcTL chunks
 * 5. Write info
 * 6. Test row-level writing with png_write_rows
 * 7. Test frame-level writing with png_write_frame_head, png_write_rows for frame, png_write_frame_tail
 * 8. Test whole-image writing with png_write_image
 * 9. Test high-level writing with png_write_png
 * 10. Cleanup
 *
 * The pngwrite.c module has the lowest coverage (35.38%) with critical writing functions 
 * either completely or minimally covered. This writing-focused harness provides semantic 
 * diversity from existing reading-focused harnesses and targets 221+ points of 
 * undiscovered complexity.
 */


// ==================== MEMORY BUFFER FOR WRITING ====================

struct write_buffer {
    std::vector<uint8_t> data;
    size_t offset;
};

// Custom write function for writing to memory buffer
static void png_write_to_memory(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) {
        png_error(png_ptr, "No IO pointer");
        return;
    }
    
    write_buffer* buffer = (write_buffer*)io_ptr;
    
    // Append data to buffer
    size_t new_size = buffer->data.size() + length;
    buffer->data.resize(new_size);
    memcpy(buffer->data.data() + buffer->offset, data, length);
    buffer->offset += length;
}

// Custom flush function (required but does nothing for memory buffer)
static void png_flush_memory(png_structp png_ptr) {
    (void)png_ptr;
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
    // Need minimum size for meaningful PNG writing operations
    if (size < 200) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Prepare write buffer for custom write function
    write_buffer buffer;
    buffer.offset = 0;
    
    // Initialize row pointers to NULL
    png_bytepp row_pointers = NULL;
    png_uint_32 height = 0;
    png_size_t row_bytes = 0;
    
    // ==================== TEST 1: BASIC PNG WRITING ====================
    
    // Consume image parameters from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 256);
    height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 256);
    int bit_depth = fdp.PickValueInArray({1, 2, 4, 8, 16});
    int color_type = fdp.PickValueInArray({
        PNG_COLOR_TYPE_GRAY,
        PNG_COLOR_TYPE_GRAY_ALPHA,
        PNG_COLOR_TYPE_PALETTE,
        PNG_COLOR_TYPE_RGB,
        PNG_COLOR_TYPE_RGB_ALPHA
    });
    int interlace_type = fdp.ConsumeBool() ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE;
    int compression_type = PNG_COMPRESSION_TYPE_BASE;
    int filter_type = PNG_FILTER_TYPE_BASE;
    
    // Create write structure
    png_structp png_ptr = png_create_write_struct(
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
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }
    
    // Set up error handling via setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during PNG processing
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Initialize I/O with memory buffer
    png_set_write_fn(png_ptr, &buffer, png_write_to_memory, png_flush_memory);
    
    // Set image header (IHDR)
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);
    
    // Check if we should create animated PNG
    bool create_animated = fdp.ConsumeBool();
    if (create_animated && fdp.remaining_bytes() > 50) {
        // Set animation control (acTL) chunk for APNG
        png_uint_32 num_frames = fdp.ConsumeIntegralInRange<png_uint_32>(1, 10);
        png_uint_32 num_plays = fdp.ConsumeIntegralInRange<png_uint_32>(0, 100);
        png_set_acTL(png_ptr, info_ptr, num_frames, num_plays);
        
        // Set first frame control (fcTL) chunk
        png_uint_32 frame_width = fdp.ConsumeIntegralInRange<png_uint_32>(1, width);
        png_uint_32 frame_height = fdp.ConsumeIntegralInRange<png_uint_32>(1, height);
        png_uint_32 x_offset = fdp.ConsumeIntegralInRange<png_uint_32>(0, width - frame_width);
        png_uint_32 y_offset = fdp.ConsumeIntegralInRange<png_uint_32>(0, height - frame_height);
        png_uint_16 delay_num = fdp.ConsumeIntegralInRange<png_uint_16>(1, 1000);
        png_uint_16 delay_den = fdp.ConsumeIntegralInRange<png_uint_16>(1, 1000);
        png_byte dispose_op = fdp.ConsumeIntegralInRange<png_byte>(0, 3);
        png_byte blend_op = fdp.ConsumeIntegralInRange<png_byte>(0, 1);
        
        png_set_next_frame_fcTL(png_ptr, info_ptr, frame_width, frame_height,
                                x_offset, y_offset, delay_num, delay_den,
                                dispose_op, blend_op);
    }
    
    // Set packing/swap if needed based on bit depth
    if (bit_depth < 8) {
        png_set_packing(png_ptr);
    }
    if (bit_depth == 16) {
        png_set_swap(png_ptr);
    }
    
    // Write info
    png_write_info(png_ptr, info_ptr);
    
    // Calculate row bytes
    row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    
    // Allocate row pointers
    row_pointers = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)png_malloc(png_ptr, row_bytes);
        
        // Fill with data from fuzzer input
        std::vector<uint8_t> row_data = fdp.ConsumeBytes<uint8_t>(row_bytes);
        if (row_data.size() < row_bytes) {
            // Not enough data, fill with zeros
            memset(row_pointers[y], 0, row_bytes);
            if (!row_data.empty()) {
                memcpy(row_pointers[y], row_data.data(), row_data.size());
            }
        } else {
            memcpy(row_pointers[y], row_data.data(), row_bytes);
        }
    }
    
    // ==================== TARGET API 1: png_write_rows ====================
    // Test row-by-row writing
    png_write_rows(png_ptr, row_pointers, height);
    
    // ==================== TARGET API 2: png_write_image ====================
    // Recreate fresh structures for png_write_image test
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 100) {
        // Free current structures
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
        png_free(png_ptr, row_pointers);
        row_pointers = NULL;
        png_write_end(png_ptr, info_ptr);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        buffer.data.clear();
        buffer.offset = 0;
        
        png_ptr = png_create_write_struct(
            PNG_LIBPNG_VER_STRING,
            NULL,
            png_error_callback,
            png_warning_callback);
        
        if (png_ptr == NULL) {
            return 0;
        }
        
        info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == NULL) {
            png_destroy_write_struct(&png_ptr, NULL);
            return 0;
        }
        
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return 0;
        }
        
        png_set_write_fn(png_ptr, &buffer, png_write_to_memory, png_flush_memory);
        png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                     interlace_type, compression_type, filter_type);
        png_write_info(png_ptr, info_ptr);
        
        // Reallocate row pointers with new data
        row_pointers = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_bytep)png_malloc(png_ptr, row_bytes);
            std::vector<uint8_t> row_data = fdp.ConsumeBytes<uint8_t>(row_bytes);
            if (row_data.size() < row_bytes) {
                memset(row_pointers[y], 0, row_bytes);
                if (!row_data.empty()) {
                    memcpy(row_pointers[y], row_data.data(), row_data.size());
                }
            } else {
                memcpy(row_pointers[y], row_data.data(), row_bytes);
            }
        }
        
        // Test whole-image writing
        png_write_image(png_ptr, row_pointers);
        png_write_end(png_ptr, info_ptr);
    }
    
    // ==================== TARGET API 3 & 4: APNG FRAME WRITING ====================
    // Test frame-by-frame writing for animated PNGs
    if (create_animated && fdp.remaining_bytes() > 150) {
        // Reset buffer for frame writing test
        buffer.data.clear();
        buffer.offset = 0;
        
        // Recreate structures for APNG test
        png_destroy_write_struct(&png_ptr, &info_ptr);
        
        png_ptr = png_create_write_struct(
            PNG_LIBPNG_VER_STRING,
            NULL,
            png_error_callback,
            png_warning_callback);
        
        if (png_ptr == NULL) {
            return 0;
        }
        
        info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == NULL) {
            png_destroy_write_struct(&png_ptr, NULL);
            return 0;
        }
        
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return 0;
        }
        
        png_set_write_fn(png_ptr, &buffer, png_write_to_memory, png_flush_memory);
        
        // Set up APNG with multiple frames
        png_uint_32 num_frames = fdp.ConsumeIntegralInRange<png_uint_32>(2, 5);
        png_uint_32 num_plays = fdp.ConsumeIntegralInRange<png_uint_32>(0, 5);
        png_set_acTL(png_ptr, info_ptr, num_frames, num_plays);
        
        // Set image header
        png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                     interlace_type, compression_type, filter_type);
        
        // Write info
        png_write_info(png_ptr, info_ptr);
        
        // Calculate row bytes for APNG
        row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        
        // Create row pointers for frames
        row_pointers = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_bytep)png_malloc(png_ptr, row_bytes);
        }
        
        // Write each frame
        for (png_uint_32 frame = 0; frame < num_frames && fdp.remaining_bytes() > 50; frame++) {
            // Generate frame parameters
            png_uint_32 frame_width = fdp.ConsumeIntegralInRange<png_uint_32>(1, width);
            png_uint_32 frame_height = fdp.ConsumeIntegralInRange<png_uint_32>(1, height);
            png_uint_32 x_offset = fdp.ConsumeIntegralInRange<png_uint_32>(0, width - frame_width);
            png_uint_32 y_offset = fdp.ConsumeIntegralInRange<png_uint_32>(0, height - frame_height);
            png_uint_16 delay_num = fdp.ConsumeIntegralInRange<png_uint_16>(1, 100);
            png_uint_16 delay_den = fdp.ConsumeIntegralInRange<png_uint_16>(1, 100);
            png_byte dispose_op = fdp.ConsumeIntegralInRange<png_byte>(0, 3);
            png_byte blend_op = fdp.ConsumeIntegralInRange<png_byte>(0, 1);
            
            // Fill frame data
            for (png_uint_32 y = 0; y < frame_height; y++) {
                std::vector<uint8_t> row_data = fdp.ConsumeBytes<uint8_t>(row_bytes);
                if (row_data.size() < row_bytes) {
                    memset(row_pointers[y], 0, row_bytes);
                    if (!row_data.empty()) {
                        memcpy(row_pointers[y], row_data.data(), row_data.size());
                    }
                } else {
                    memcpy(row_pointers[y], row_data.data(), row_bytes);
                }
            }
            
            // ==================== TARGET API 5 & 6 ====================
            // Write frame head
            png_write_frame_head(png_ptr, info_ptr, row_pointers,
                                 frame_width, frame_height,
                                 x_offset, y_offset,
                                 delay_num, delay_den,
                                 dispose_op, blend_op);
            
            // Write frame rows
            png_write_rows(png_ptr, row_pointers, frame_height);
            
            // Write frame tail
            png_write_frame_tail(png_ptr, info_ptr);
        }
        
        // Clean up row pointers
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
        png_free(png_ptr, row_pointers);
        row_pointers = NULL;
    }
    
    // ==================== TARGET API 7: png_write_png ====================
    // Test high-level API
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 100) {
        // Reset buffer
        buffer.data.clear();
        buffer.offset = 0;
        
        // Recreate structures
        png_destroy_write_struct(&png_ptr, &info_ptr);
        
        png_ptr = png_create_write_struct(
            PNG_LIBPNG_VER_STRING,
            NULL,
            png_error_callback,
            png_warning_callback);
        
        if (png_ptr == NULL) {
            return 0;
        }
        
        info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == NULL) {
            png_destroy_write_struct(&png_ptr, NULL);
            return 0;
        }
        
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return 0;
        }
        
        png_set_write_fn(png_ptr, &buffer, png_write_to_memory, png_flush_memory);
        png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                     interlace_type, compression_type, filter_type);
        
        // Allocate and fill row pointers
        row_pointers = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_bytep)png_malloc(png_ptr, row_bytes);
            std::vector<uint8_t> row_data = fdp.ConsumeBytes<uint8_t>(row_bytes);
            if (row_data.size() < row_bytes) {
                memset(row_pointers[y], 0, row_bytes);
                if (!row_data.empty()) {
                    memcpy(row_pointers[y], row_data.data(), row_data.size());
                }
            } else {
                memcpy(row_pointers[y], row_data.data(), row_bytes);
            }
        }
        
        // Set row pointers for png_write_png
        png_set_rows(png_ptr, info_ptr, row_pointers);
        // Test high-level writing API
        png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
        
        // Free row pointers
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
        png_free(png_ptr, row_pointers);
        row_pointers = NULL;
    }
    
    // ==================== CLEANUP ====================
    
    // Free row pointers if they still exist
    if (row_pointers != NULL) {
        for (png_uint_32 y = 0; y < height; y++) {
            if (row_pointers[y] != NULL) {
                png_free(png_ptr, row_pointers[y]);
            }
        }
        png_free(png_ptr, row_pointers);
    }
    
    // Write end and cleanup
    if (png_ptr != NULL && info_ptr != NULL) {
        png_write_end(png_ptr, info_ptr);
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
    
    return 0;
}

} // namespace harness_018
