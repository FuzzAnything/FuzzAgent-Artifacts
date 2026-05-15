#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <zlib.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_004 {

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
    if (size < 256) {
        return 0;  // Need minimum data for color management operations
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create a memory buffer for PNG operations
    png_memory_buffer buffer;
    buffer.data = data;
    buffer.size = size;
    buffer.offset = 0;
    
    // Create PNG read structure for color management testing
    png_structp png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        pngtest_error, 
        pngtest_warning);
    
    if (!png_ptr) {
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up custom read functions
    png_set_read_fn(png_ptr, &buffer, pngtest_read_data);
    
    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Set IHDR for a palette-based image (required for quantization)
    // Use fuzzed data for image dimensions and bit depth
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 256);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 256);
    int bit_depth = fdp.PickValueInArray({1, 2, 4, 8});
    int color_type = PNG_COLOR_TYPE_PALETTE;  // Palette type for quantization
    int interlace_type = fdp.ConsumeBool() ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE;
    int compression_type = PNG_COMPRESSION_TYPE_BASE;
    int filter_type = PNG_FILTER_TYPE_BASE;
    
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);
    
    // Set up palette (PLTE) for quantization
    // Create a palette with fuzzed colors
    int num_palette = fdp.ConsumeIntegralInRange<int>(1, 256);
    std::vector<png_color> palette(num_palette);
    for (int i = 0; i < num_palette && fdp.remaining_bytes() >= 3; i++) {
        palette[i].red = fdp.ConsumeIntegral<uint8_t>();
        palette[i].green = fdp.ConsumeIntegral<uint8_t>();
        palette[i].blue = fdp.ConsumeIntegral<uint8_t>();
    }
    
    png_set_PLTE(png_ptr, info_ptr, palette.data(), num_palette);
    
    // Set transparency (tRNS) for alpha operations
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= num_palette) {
        std::vector<png_byte> trans(num_palette);
        for (int i = 0; i < num_palette && i < (int)fdp.remaining_bytes(); i++) {
            trans[i] = fdp.ConsumeIntegral<uint8_t>();
        }
        png_set_tRNS(png_ptr, info_ptr, trans.data(), num_palette, nullptr);
    }
    
    // Now test color management APIs as specified in coverage guidance
    
    // 1. png_set_quantize - highest priority (130 undiscovered branches)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= sizeof(png_uint_16) * num_palette + 4) {
        int maximum_colors = fdp.ConsumeIntegralInRange<int>(1, num_palette);
        int full_quantize = fdp.ConsumeBool() ? 1 : 0;
        
        // Create histogram if requested
        std::vector<png_uint_16> histogram;
        if (full_quantize && fdp.remaining_bytes() >= sizeof(png_uint_16) * num_palette) {
            histogram.resize(num_palette);
            for (size_t i = 0; i < histogram.size(); i++) {
                histogram[i] = fdp.ConsumeIntegral<png_uint_16>();
            }
        }
        
        png_set_quantize(png_ptr, palette.data(), num_palette, maximum_colors,
                        histogram.empty() ? nullptr : histogram.data(), full_quantize);
    }
    
    // 2. png_set_iCCP - ICC profile handling (63 undiscovered branches)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 64) {
        std::string profile_name = fdp.ConsumeRandomLengthString(31);
        int compression_type_icc = fdp.ConsumeBool() ? PNG_COMPRESSION_TYPE_BASE : PNG_COMPRESSION_TYPE_BASE;
        
        // Create a simple ICC profile from fuzzed data
        size_t profile_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
        std::vector<png_byte> icc_profile;
        if (fdp.remaining_bytes() >= profile_size) {
            icc_profile = fdp.ConsumeBytes<png_byte>(profile_size);
            png_set_iCCP(png_ptr, info_ptr, profile_name.c_str(), compression_type_icc,
                        icc_profile.data(), icc_profile.size());
        }
    }
    
    // 3. png_set_gamma / png_set_gamma_fixed - gamma correction (17 undiscovered branches)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 8) {
        // Choose between floating point and fixed point version
        if (fdp.ConsumeBool()) {
            // Floating point version
            double screen_gamma = fdp.ConsumeFloatingPointInRange<double>(0.1, 5.0);
            double file_gamma = fdp.ConsumeFloatingPointInRange<double>(0.1, 5.0);
            png_set_gamma(png_ptr, screen_gamma, file_gamma);
        } else {
            // Fixed point version
            png_fixed_point screen_gamma_fixed = fdp.ConsumeIntegral<png_fixed_point>();
            png_fixed_point file_gamma_fixed = fdp.ConsumeIntegral<png_fixed_point>();
            png_set_gamma_fixed(png_ptr, screen_gamma_fixed, file_gamma_fixed);
        }
    }
    
    // 4. png_set_background / png_set_background_fixed - background setting (15 undiscovered branches)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 8) {
        png_color_16 background_color;
        background_color.index = fdp.ConsumeIntegral<uint8_t>();
        background_color.red = fdp.ConsumeIntegral<uint16_t>();
        background_color.green = fdp.ConsumeIntegral<uint16_t>();
        background_color.blue = fdp.ConsumeIntegral<uint16_t>();
        background_color.gray = fdp.ConsumeIntegral<uint16_t>();
        
        int background_gamma_code = fdp.ConsumeIntegralInRange<int>(1, 3);
        
        if (fdp.ConsumeBool()) {
            // Floating point version
            double background_gamma = fdp.ConsumeFloatingPointInRange<double>(0.1, 5.0);
            png_set_background(png_ptr, &background_color, background_gamma_code,
                              background_gamma, 1);
        } else {
            // Fixed point version  
            png_fixed_point background_gamma_fixed = fdp.ConsumeIntegral<png_fixed_point>();
            png_set_background_fixed(png_ptr, &background_color, background_gamma_code,
                                    background_gamma_fixed, 1);
        }
    }
    
    // 5. png_set_alpha_mode - alpha compositing (13 undiscovered branches)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 4) {
        int alpha_mode = fdp.ConsumeIntegralInRange<int>(0, 3);  // 0-3 corresponds to PNG_ALPHA_* modes
        
        if (fdp.ConsumeBool()) {
            // Floating point version
            double output_gamma = fdp.ConsumeFloatingPointInRange<double>(0.1, 5.0);
            png_set_alpha_mode(png_ptr, alpha_mode, output_gamma);
        } else {
            // Fixed point version
            png_fixed_point output_gamma_fixed = fdp.ConsumeIntegral<png_fixed_point>();
            png_set_alpha_mode_fixed(png_ptr, alpha_mode, output_gamma_fixed);
        }
    }
    
    // 6. png_set_cHRM_XYZ / png_set_cHRM_XYZ_fixed - chromaticity (13 undiscovered branches)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 24) {
        double red_X, red_Y, red_Z;
        double green_X, green_Y, green_Z;
        double blue_X, blue_Y, blue_Z;
        
        red_X = fdp.ConsumeFloatingPoint<double>();
        red_Y = fdp.ConsumeFloatingPoint<double>();
        red_Z = fdp.ConsumeFloatingPoint<double>();
        green_X = fdp.ConsumeFloatingPoint<double>();
        green_Y = fdp.ConsumeFloatingPoint<double>();
        green_Z = fdp.ConsumeFloatingPoint<double>();
        blue_X = fdp.ConsumeFloatingPoint<double>();
        blue_Y = fdp.ConsumeFloatingPoint<double>();
        blue_Z = fdp.ConsumeFloatingPoint<double>();
        
        if (fdp.ConsumeBool()) {
            // Floating point version
            png_set_cHRM_XYZ(png_ptr, info_ptr, red_X, red_Y, red_Z,
                            green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z);
        } else {
            // Fixed point version
            png_fixed_point red_X_fixed = fdp.ConsumeIntegral<png_fixed_point>();
            png_fixed_point red_Y_fixed = fdp.ConsumeIntegral<png_fixed_point>();
            png_fixed_point red_Z_fixed = fdp.ConsumeIntegral<png_fixed_point>();
            png_fixed_point green_X_fixed = fdp.ConsumeIntegral<png_fixed_point>();
            png_fixed_point green_Y_fixed = fdp.ConsumeIntegral<png_fixed_point>();
            png_fixed_point green_Z_fixed = fdp.ConsumeIntegral<png_fixed_point>();
            png_fixed_point blue_X_fixed = fdp.ConsumeIntegral<png_fixed_point>();
            png_fixed_point blue_Y_fixed = fdp.ConsumeIntegral<png_fixed_point>();
            png_fixed_point blue_Z_fixed = fdp.ConsumeIntegral<png_fixed_point>();
            
            png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, red_X_fixed, red_Y_fixed, red_Z_fixed,
                                  green_X_fixed, green_Y_fixed, green_Z_fixed,
                                  blue_X_fixed, blue_Y_fixed, blue_Z_fixed);
        }
    }
    
    // Try to read info after setting all transformations
    png_read_info(png_ptr, info_ptr);
    
    // Apply transformations
    png_start_read_image(png_ptr);
    
    // Read a few rows to exercise the transformations
    if (fdp.remaining_bytes() > 0) {
        png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
        if (rowbytes > 0 && rowbytes < 65536) {
            std::vector<png_byte> row(rowbytes);
            
            // Read first few rows
            for (png_uint_32 row_num = 0; row_num < height && row_num < 5; row_num++) {
                if (fdp.remaining_bytes() >= rowbytes) {
                    png_read_row(png_ptr, row.data(), nullptr);
                }
            }
        }
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}

} // namespace harness_004
