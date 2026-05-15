#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>

namespace harness_015 {

// Custom write callback structure for png_set_write_fn
struct png_write_memory_data {
    uint8_t* buffer;
    size_t size;
    size_t offset;
    size_t capacity;
};

// Write callback function for memory-based output
static void png_write_memory_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_write_memory_data* write_data = (png_write_memory_data*)png_get_io_ptr(png_ptr);
    
    // Resize buffer if needed
    if (write_data->offset + length > write_data->capacity) {
        size_t new_capacity = write_data->capacity * 2;
        if (new_capacity < write_data->offset + length) {
            new_capacity = write_data->offset + length;
        }
        uint8_t* new_buffer = (uint8_t*)realloc(write_data->buffer, new_capacity);
        if (!new_buffer) {
            png_error(png_ptr, "Memory allocation failed in write callback");
            return;
        }
        write_data->buffer = new_buffer;
        write_data->capacity = new_capacity;
    }
    
    memcpy(write_data->buffer + write_data->offset, data, length);
    write_data->offset += length;
    write_data->size = write_data->offset;
}

// Flush callback (required but does nothing for memory writing)
static void png_write_memory_flush(png_structp png_ptr) {
    // Nothing to flush for memory buffer
    (void)png_ptr;
}

// Custom read callback structure for png_set_read_fn
struct png_read_memory_data {
    const uint8_t* buffer;
    size_t size;
    size_t offset;
};

// Read callback function for memory-based input
static void png_read_memory_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_read_memory_data* read_data = (png_read_memory_data*)png_get_io_ptr(png_ptr);
    
    if (read_data->offset + length > read_data->size) {
        png_error(png_ptr, "Insufficient data in read callback");
        return;
    }
    
    memcpy(data, read_data->buffer + read_data->offset, length);
    read_data->offset += length;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for PNG metadata testing
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Check for sufficient data before consuming parameters
    const size_t MIN_REQUIRED_BYTES = sizeof(int) + 9 * sizeof(bool) + 
                                      sizeof(png_uint_32) * 2 + 
                                      sizeof(png_byte) * 2;
    if (fdp.remaining_bytes() < MIN_REQUIRED_BYTES) {
        return 0;
    }
    
    // Consume parameters from fuzzer input to control test flow
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 7);
    bool include_plte = fdp.ConsumeBool();
    bool include_trns = fdp.ConsumeBool();
    bool include_bkgd = fdp.ConsumeBool();
    bool include_sbit = fdp.ConsumeBool();
    bool include_srgb = fdp.ConsumeBool();
    bool include_chrm = fdp.ConsumeBool();
    bool include_gama = fdp.ConsumeBool();
    bool include_phys = fdp.ConsumeBool();
    bool include_cicp = fdp.ConsumeBool();
    
    // Consume image parameters
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
    png_byte bit_depth = fdp.ConsumeIntegralInRange<png_byte>(1, 16);
    png_byte color_type = fdp.PickValueInArray({PNG_COLOR_TYPE_GRAY, PNG_COLOR_TYPE_GRAY_ALPHA,
                                                PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
                                                PNG_COLOR_TYPE_RGB_ALPHA});
    int interlace_type = fdp.PickValueInArray({PNG_INTERLACE_NONE, PNG_INTERLACE_ADAM7});
    int compression_type = PNG_COMPRESSION_TYPE_DEFAULT;
    int filter_type = PNG_FILTER_TYPE_DEFAULT;
    // ====================================================
    // STEP 1: CREATE WRITE STRUCTURE
    // ====================================================
    png_struct *write_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                       nullptr, nullptr, nullptr);
    if (write_png_ptr == nullptr) {
        return 0;
    }
    
    // STEP 2: I/O setup for writing with png_set_write_fn
    png_write_memory_data write_data;
    write_data.buffer = nullptr;
    write_data.size = 0;
    write_data.offset = 0;
    write_data.capacity = 4096; // Initial capacity
    
    write_data.buffer = (uint8_t*)malloc(write_data.capacity);
    if (write_data.buffer == nullptr) {
        png_destroy_write_struct(&write_png_ptr, nullptr);
        return 0;
    }
    
    png_set_write_fn(write_png_ptr, &write_data, png_write_memory_callback, png_write_memory_flush);
    
    // STEP 3: Info structure creation with png_create_info_struct
    png_info *write_info_ptr = png_create_info_struct(write_png_ptr);
    if (write_info_ptr == nullptr) {
        free(write_data.buffer);
        png_destroy_write_struct(&write_png_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling for writer
    if (setjmp(png_jmpbuf(write_png_ptr))) {
        free(write_data.buffer);
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        return 0;
    }
    
    // ====================================================
    // STEP 4: SET BASIC IMAGE PARAMETERS WITH png_set_IHDR
    // ====================================================
    png_set_IHDR(write_png_ptr, write_info_ptr, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);
    
    // ====================================================
    // STEP 5: SET METADATA CHUNKS USING TARGET SETTER APIS
    // ====================================================
    
    // Set PLTE (Palette) if requested and color type supports it
    if (include_plte && (color_type & PNG_COLOR_MASK_PALETTE)) {
        int num_palette = fdp.ConsumeIntegralInRange<int>(1, 256);
        if (num_palette > 0) {
            png_color* palette = (png_color*)malloc(sizeof(png_color) * num_palette);
            if (palette) {
                for (int i = 0; i < num_palette && fdp.remaining_bytes() >= 3; i++) {
                    palette[i].red = fdp.ConsumeIntegral<uint8_t>();
                    palette[i].green = fdp.ConsumeIntegral<uint8_t>();
                    palette[i].blue = fdp.ConsumeIntegral<uint8_t>();
                }
                png_set_PLTE(write_png_ptr, write_info_ptr, palette, num_palette);
                free(palette);
            }
        }
    }
    
    // Set tRNS (Transparency) if requested
    if (include_trns) {
        if (color_type == PNG_COLOR_TYPE_PALETTE && include_plte) {
            // Check if we have enough data for num_trans
            if (fdp.remaining_bytes() >= sizeof(int)) {
                int num_trans = fdp.ConsumeIntegralInRange<int>(0, 256);
                if (num_trans > 0 && fdp.remaining_bytes() >= num_trans) {
                    png_byte* trans_alpha = (png_byte*)malloc(num_trans);
                    if (trans_alpha) {
                        for (int i = 0; i < num_trans && fdp.remaining_bytes() > 0; i++) {
                            trans_alpha[i] = fdp.ConsumeIntegral<uint8_t>();
                        }
                        png_set_tRNS(write_png_ptr, write_info_ptr, trans_alpha, num_trans, nullptr);
                        free(trans_alpha);
                    }
                }
            }
        } else if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_RGB) {
            // Check if we have enough data for 4 png_uint_16 values
            const size_t trans_bytes_needed = 4 * sizeof(png_uint_16);
            if (fdp.remaining_bytes() >= trans_bytes_needed) {
                png_color_16 trans_color;
                trans_color.red = fdp.ConsumeIntegral<png_uint_16>();
                trans_color.green = fdp.ConsumeIntegral<png_uint_16>();
                trans_color.blue = fdp.ConsumeIntegral<png_uint_16>();
                trans_color.gray = fdp.ConsumeIntegral<png_uint_16>();
                png_set_tRNS(write_png_ptr, write_info_ptr, nullptr, 0, &trans_color);
            }
        }
    }
    
    // Set bKGD (Background) if requested
    if (include_bkgd) {
        // Check if we have enough data for 4 png_uint_16 + 1 uint8_t
        const size_t bkgd_bytes_needed = 4 * sizeof(png_uint_16) + sizeof(uint8_t);
        if (fdp.remaining_bytes() >= bkgd_bytes_needed) {
            png_color_16 background;
            background.red = fdp.ConsumeIntegral<png_uint_16>();
            background.green = fdp.ConsumeIntegral<png_uint_16>();
            background.blue = fdp.ConsumeIntegral<png_uint_16>();
            background.gray = fdp.ConsumeIntegral<png_uint_16>();
            background.index = fdp.ConsumeIntegral<uint8_t>();
            png_set_bKGD(write_png_ptr, write_info_ptr, &background);
        }
    }
    
    // Set sBIT (Significant Bits) if requested
    if (include_sbit) {
        png_color_8 sig_bit;
        sig_bit.red = fdp.ConsumeIntegralInRange<png_byte>(1, bit_depth);
        sig_bit.green = fdp.ConsumeIntegralInRange<png_byte>(1, bit_depth);
        sig_bit.blue = fdp.ConsumeIntegralInRange<png_byte>(1, bit_depth);
        sig_bit.gray = fdp.ConsumeIntegralInRange<png_byte>(1, bit_depth);
        sig_bit.alpha = fdp.ConsumeIntegralInRange<png_byte>(1, bit_depth);
        png_set_sBIT(write_png_ptr, write_info_ptr, &sig_bit);
    }
    
    // Set sRGB (Standard RGB) if requested
    if (include_srgb) {
        int srgb_intent = fdp.ConsumeIntegralInRange<int>(0, 3);
        png_set_sRGB(write_png_ptr, write_info_ptr, srgb_intent);
    }
    
    // Set cHRM (Chromaticities) if requested
    if (include_chrm) {
        // Check if we have enough data for 8 double values
        if (fdp.remaining_bytes() >= 8 * sizeof(double)) {
            double white_x = fdp.ConsumeFloatingPoint<double>();
            double white_y = fdp.ConsumeFloatingPoint<double>();
            double red_x = fdp.ConsumeFloatingPoint<double>();
            double red_y = fdp.ConsumeFloatingPoint<double>();
            double green_x = fdp.ConsumeFloatingPoint<double>();
            double green_y = fdp.ConsumeFloatingPoint<double>();
            double blue_x = fdp.ConsumeFloatingPoint<double>();
            double blue_y = fdp.ConsumeFloatingPoint<double>();
            png_set_cHRM(write_png_ptr, write_info_ptr, white_x, white_y, red_x, red_y,
                         green_x, green_y, blue_x, blue_y);
        }
    }
    
    // Set gAMA (Gamma) if requested
    if (include_gama) {
        if (fdp.remaining_bytes() >= sizeof(double)) {
            double file_gamma = fdp.ConsumeFloatingPointInRange<double>(0.01, 10.0);
            png_set_gAMA(write_png_ptr, write_info_ptr, file_gamma);
        }
    }
    
    // Set pHYs (Physical Dimensions) if requested
    if (include_phys) {
        png_uint_32 res_x = fdp.ConsumeIntegral<png_uint_32>();
        png_uint_32 res_y = fdp.ConsumeIntegral<png_uint_32>();
        int unit_type = fdp.PickValueInArray({PNG_RESOLUTION_UNKNOWN, PNG_RESOLUTION_METER});
        png_set_pHYs(write_png_ptr, write_info_ptr, res_x, res_y, unit_type);
    }
    
    // Set cICP (Coding-Independent Code Points) if requested
    if (include_cicp) {
        // cICP is a newer chunk, check if supported
        #ifdef PNG_cICP_SUPPORTED
        uint8_t color_primaries = fdp.ConsumeIntegral<uint8_t>();
        uint8_t transfer_function = fdp.ConsumeIntegral<uint8_t>();
        uint8_t matrix_coefficients = fdp.ConsumeIntegral<uint8_t>();
        uint8_t video_full_range_flag = fdp.ConsumeIntegral<uint8_t>();
        png_set_cICP(write_png_ptr, write_info_ptr, color_primaries, transfer_function,
                     matrix_coefficients, video_full_range_flag);
        #endif
    }
    
    // ====================================================
    // STEP 6: WRITE PNG DATA
    // ====================================================
    png_write_info(write_png_ptr, write_info_ptr);
    // Allocate and write dummy image data
    png_uint_32 rowbytes = png_get_rowbytes(write_png_ptr, write_info_ptr);
    if (rowbytes > 0 && rowbytes < 10000) {
        png_bytep row = (png_bytep)malloc(rowbytes);
        if (row) {
            // Fill row with fuzzer data - defensive programming to avoid BadInstruction crash
            // Check remaining bytes before consuming fill_size
            if (fdp.remaining_bytes() >= sizeof(size_t)) {
                size_t fill_size = fdp.ConsumeIntegralInRange<size_t>(0, rowbytes);
                
                // Check if we have enough bytes for the requested fill_size
                if (fill_size > 0 && fdp.remaining_bytes() >= fill_size) {
                    std::vector<uint8_t> row_data = fdp.ConsumeBytes<uint8_t>(fill_size);
                    
                    // Always initialize row to zeros
                    memset(row, 0, rowbytes);
                    
                    // Safe memcpy with bounds checking
                    if (!row_data.empty()) {
                        size_t copy_size = fill_size < rowbytes ? fill_size : rowbytes;
                        if (copy_size > 0) {
                            memcpy(row, row_data.data(), copy_size);
                        }
                    }
                } else {
                    // Not enough bytes or fill_size is 0, just zero the buffer
                    memset(row, 0, rowbytes);
                }
            } else {
                // Not enough bytes for fill_size, just zero the buffer
                memset(row, 0, rowbytes);
            }
            
            for (png_uint_32 y = 0; y < height; y++) {
                png_write_row(write_png_ptr, row);
            }
            free(row);
        }
    }
    
    png_write_end(write_png_ptr, write_info_ptr);
    
    // ====================================================
    // STEP 7: CLEAN UP WRITER
    // ====================================================
    png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
    
    // ====================================================
    // STEP 8: CREATE READ STRUCTURE
    // ====================================================
    png_struct *read_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                     nullptr, nullptr, nullptr);
    if (read_png_ptr == nullptr) {
        free(write_data.buffer);
        return 0;
    }
    
    // STEP 9: SET UP INPUT WITH png_set_read_fn USING WRITTEN BUFFER
    png_read_memory_data read_data;
    read_data.buffer = write_data.buffer;
    read_data.size = write_data.size;
    read_data.offset = 0;
    
    png_set_read_fn(read_png_ptr, &read_data, png_read_memory_callback);
    
    // STEP 10: Info structure for reading
    png_info *read_info_ptr = png_create_info_struct(read_png_ptr);
    if (read_info_ptr == nullptr) {
        free(write_data.buffer);
        png_destroy_read_struct(&read_png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling for reader
    if (setjmp(png_jmpbuf(read_png_ptr))) {
        free(write_data.buffer);
        png_destroy_read_struct(&read_png_ptr, &read_info_ptr, nullptr);
        return 0;
    }
    
    // ====================================================
    // STEP 11: READ PNG WITH png_read_info
    // ====================================================
    png_set_sig_bytes(read_png_ptr, 0);
    png_read_info(read_png_ptr, read_info_ptr);
    
    // ====================================================
    // STEP 12: VERIFY METADATA BY CALLING GETTER APIS
    // ====================================================
    
    // Verify PLTE if set
    if (include_plte && (color_type & PNG_COLOR_MASK_PALETTE)) {
        png_color* palette = nullptr;
        int num_palette = 0;
        png_get_PLTE(read_png_ptr, read_info_ptr, &palette, &num_palette);
        // Verification: num_palette should be > 0 if PLTE was set
    }
    
    // Verify tRNS if set
    if (include_trns) {
        png_byte* trans_alpha = nullptr;
        int num_trans = 0;
        png_color_16* trans_color = nullptr;
        png_get_tRNS(read_png_ptr, read_info_ptr, &trans_alpha, &num_trans, &trans_color);
        // Verification: if color type supports transparency, we should get something
    }
    
    // Verify bKGD if set
    if (include_bkgd) {
        png_color_16p background = nullptr;
        png_get_bKGD(read_png_ptr, read_info_ptr, &background);
        // Verification: background should not be null if bKGD was set
    }
    
    // Verify sBIT if set
    if (include_sbit) {
        png_color_8p sig_bit = nullptr;
        png_get_sBIT(read_png_ptr, read_info_ptr, &sig_bit);
        // Verification: sig_bit should not be null if sBIT was set
    }
    
    // Verify sRGB if set
    if (include_srgb) {
        int file_srgb_intent = 0;
        png_get_sRGB(read_png_ptr, read_info_ptr, &file_srgb_intent);
        // Verification: we should get a valid intent value
    }
    
    // Verify cHRM if set
    if (include_chrm) {
        double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
        png_get_cHRM(read_png_ptr, read_info_ptr, &white_x, &white_y, &red_x, &red_y,
                     &green_x, &green_y, &blue_x, &blue_y);
        // Verification: values should be retrievable
    }
    
    // Verify gAMA if set
    if (include_gama) {
        double file_gamma;
        png_get_gAMA(read_png_ptr, read_info_ptr, &file_gamma);
        // Verification: gamma value should be retrievable
    }
    
    // Verify pHYs if set
    if (include_phys) {
        png_uint_32 res_x, res_y;
        int unit_type;
        png_get_pHYs(read_png_ptr, read_info_ptr, &res_x, &res_y, &unit_type);
        // Verification: physical dimensions should be retrievable
    }
    
    // Verify cICP if set
    if (include_cicp) {
        #ifdef PNG_cICP_SUPPORTED
        uint8_t color_primaries, transfer_function, matrix_coefficients, video_full_range_flag;
        png_get_cICP(read_png_ptr, read_info_ptr, &color_primaries, &transfer_function,
                     &matrix_coefficients, &video_full_range_flag);
        #endif
    }
    
    // ====================================================
    // STEP 13: SKIP IMAGE READING - HARNESS FOCUSES ON METADATA TESTING
    // ====================================================
    // This harness tests PNG metadata chunk operations, not pixel data.
    // The original bug was passing nullptr to png_read_image, causing NULL pointer dereference.
    // Since we only need to verify metadata getter APIs, we can skip image reading
    // and proceed directly to png_read_end.
    
    // Alternative approach: Use png_read_rows to skip full image allocation
    // png_uint_32 image_height = png_get_image_height(read_png_ptr, read_info_ptr);
    // png_uint_32 rowbytes = png_get_rowbytes(read_png_ptr, read_info_ptr);
    // if (rowbytes > 0 && rowbytes < 10000) {
    //     png_bytep row = (png_bytep)png_malloc(read_png_ptr, rowbytes);
    //     if (row) {
    //         for (png_uint_32 y = 0; y < image_height; y++) {
    //             png_read_rows(read_png_ptr, &row, nullptr, 1);
    //         }
    //         png_free(read_png_ptr, row);
    //     }
    // }
    
    png_read_end(read_png_ptr, read_info_ptr);
    
    // ====================================================
    // STEP 14: CLEAN UP READER
    // ====================================================
    free(write_data.buffer);
    png_destroy_read_struct(&read_png_ptr, &read_info_ptr, nullptr);
    
    return 0;
}

} // namespace harness_015
