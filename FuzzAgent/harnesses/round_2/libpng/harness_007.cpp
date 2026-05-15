#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>
#include <algorithm>

namespace harness_007 {

// Custom write function for libpng that writes to memory buffer
void user_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Handle zero-length writes according to libpng's API contract
    if (length == 0) {
        return;  // Nothing to write
    }
    
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) return;
    
    // Cast to vector pointer and append data
    std::vector<uint8_t>* buffer = static_cast<std::vector<uint8_t>*>(io_ptr);
    buffer->insert(buffer->end(), data, data + length);
}

// Flush function (required but can be empty for memory buffers)
void user_flush_data(png_structp png_ptr) {
    (void)png_ptr;
    // Nothing to flush for memory buffer
}

// Error handling functions
void user_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do nothing - just return
}

void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Do nothing - just return
}

// Custom user transformation function (for PNG_USER_TRANSFORM testing)
void user_write_transform_fn(png_structp png_ptr, png_row_infop row_info,
                            png_bytep data) {
    (void)png_ptr;
    (void)row_info;
    (void)data;
    // Simple transformation: do nothing (or could implement custom logic)
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for write transformation testing
    // Required: width, height, bit_depth, color_type, transformation flags, row data
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume image parameters from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
    int bit_depth = fdp.ConsumeIntegralInRange<int>(1, 16);
    int color_type = fdp.ConsumeIntegralInRange<int>(0, 6);
    int interlace_type = fdp.ConsumeIntegralInRange<int>(0, 1);
    int compression_type = fdp.ConsumeIntegralInRange<int>(0, 1);
    int filter_type = fdp.ConsumeIntegralInRange<int>(0, 5);
    
    // Create PNG write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                  NULL, 
                                                  user_error_fn, 
                                                  user_warning_fn);
    if (png_ptr == NULL) {
        return 0;
    }
    
    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }
    
    // Set up error handling using setjmp/longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Set up output buffer for writing
    std::vector<uint8_t> output_buffer;
    png_set_write_fn(png_ptr, &output_buffer, user_write_data, user_flush_data);
    
    // Set image header information
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);
    
    // Consume transformation configuration from fuzzer input
    uint32_t transform_flags = fdp.ConsumeIntegral<uint32_t>();
    
    // Configure transformations based on consumed flags
    // Each transformation corresponds to a flag in the PNG_TRANSFORM_* macros
    
    // PNG_PACKING transformation (bit 2)
    if ((transform_flags & 0x04) != 0) {
        png_set_packing(png_ptr);
    }
    
    // PNG_SHIFT transformation (bit 6)
    if ((transform_flags & 0x40) != 0 && fdp.remaining_bytes() >= sizeof(png_color_8)) {
        png_color_8 shift_bits;
        shift_bits.red = fdp.ConsumeIntegral<uint8_t>();
        shift_bits.green = fdp.ConsumeIntegral<uint8_t>();
        shift_bits.blue = fdp.ConsumeIntegral<uint8_t>();
        shift_bits.gray = fdp.ConsumeIntegral<uint8_t>();
        shift_bits.alpha = fdp.ConsumeIntegral<uint8_t>();
        png_set_shift(png_ptr, &shift_bits);
    }
    
    // PNG_SWAP_BYTES transformation (bit 9)
    if ((transform_flags & 0x200) != 0) {
        png_set_swap(png_ptr);
    }
    
    // PNG_BGR transformation (bit 7)
    if ((transform_flags & 0x80) != 0) {
        png_set_bgr(png_ptr);
    }
    
    // PNG_INVERT_MONO transformation (bit 5)
    if ((transform_flags & 0x20) != 0) {
        png_set_invert_mono(png_ptr);
    }
    
    // PNG_SWAP_ALPHA transformation (bit 8)
    if ((transform_flags & 0x100) != 0) {
        png_set_swap_alpha(png_ptr);
    }
    
    // PNG_INVERT_ALPHA transformation (bit 10)
    if ((transform_flags & 0x400) != 0) {
        png_set_invert_alpha(png_ptr);
    }
    
    // PNG_FILLER transformation (bits 11-12)
    if ((transform_flags & 0x1800) != 0 && fdp.remaining_bytes() >= 5) {
        png_uint_32 filler = fdp.ConsumeIntegral<uint32_t>();
        int filler_location = fdp.ConsumeBool() ? PNG_FILLER_AFTER : PNG_FILLER_BEFORE;
        
        // Choose between png_set_filler and png_set_add_alpha based on flag
        if ((transform_flags & 0x0800) != 0) {
            png_set_filler(png_ptr, filler, filler_location);
        } else {
            png_set_add_alpha(png_ptr, filler, filler_location);
        }
    }
    
    // PNG_PACKSWAP transformation (bit 3)
    if ((transform_flags & 0x08) != 0) {
        png_set_packswap(png_ptr);
    }
    
    // PNG_USER_TRANSFORM (bit 0)
    if ((transform_flags & 0x01) != 0) {
        png_set_write_user_transform_fn(png_ptr, user_write_transform_fn);
    }
    
    // Additional setup for specific color types
    if (color_type == PNG_COLOR_TYPE_PALETTE && fdp.remaining_bytes() >= 768) {
        // Set up palette for palette images
        png_color palette[256];
        int num_palette = fdp.ConsumeIntegralInRange<int>(1, 256);
        for (int i = 0; i < num_palette && fdp.remaining_bytes() >= 3; i++) {
            palette[i].red = fdp.ConsumeIntegral<uint8_t>();
            palette[i].green = fdp.ConsumeIntegral<uint8_t>();
            palette[i].blue = fdp.ConsumeIntegral<uint8_t>();
        }
        png_set_PLTE(png_ptr, info_ptr, palette, num_palette);
    }
    
    // Write PNG header information (triggers some setup)
    png_write_info(png_ptr, info_ptr);
    
    // Prepare image data for writing
    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    if (row_bytes == 0 || height == 0) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Check if we have enough fuzzer data for at least one row
    if (fdp.remaining_bytes() < row_bytes) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Allocate and fill row data
    png_bytep row_data = (png_bytep)png_malloc(png_ptr, row_bytes);
    std::vector<uint8_t> row_bytes_data = fdp.ConsumeBytes<uint8_t>(row_bytes);
    memcpy(row_data, row_bytes_data.data(), row_bytes);
    
    // Write rows to trigger transformations
    // Different writing patterns to test various code paths
    
    // Pattern 1: Write single rows
    if ((transform_flags & 0x1000) != 0) {
        // Write each row individually
        for (png_uint_32 y = 0; y < height; y++) {
            if (fdp.remaining_bytes() >= row_bytes) {
                std::vector<uint8_t> additional_row = fdp.ConsumeBytes<uint8_t>(row_bytes);
                memcpy(row_data, additional_row.data(), row_bytes);
            }
            png_write_row(png_ptr, row_data);
        }
    }
    // Pattern 2: Write all rows at once
    else if ((transform_flags & 0x2000) != 0) {
        png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = row_data;
            if (y < height - 1 && fdp.remaining_bytes() >= row_bytes) {
                // Use different data for each row if available
                std::vector<uint8_t> additional_row = fdp.ConsumeBytes<uint8_t>(row_bytes);
                memcpy(row_data, additional_row.data(), row_bytes);
            }
        }
        png_write_rows(png_ptr, row_pointers, height);
        png_free(png_ptr, row_pointers);
    }
    // Pattern 3: Write using png_write_image
    else {
        png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = row_data;
        }
        png_write_image(png_ptr, row_pointers);
        png_free(png_ptr, row_pointers);
    }
    
    // Write PNG end marker
    png_write_end(png_ptr, info_ptr);
    
    // Clean up
    png_free(png_ptr, row_data);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}

} // namespace harness_007
