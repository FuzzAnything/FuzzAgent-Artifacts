#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>
#include <algorithm>
#include <cstdint>

namespace harness_018 {

// Custom error handler functions
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

// Custom read function for libpng that reads from fuzzer input
void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Handle zero-length reads according to libpng's API contract
    if (length == 0) {
        return;  // Nothing to read, nothing to copy
    }
    
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) return;
    
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(io_ptr);
    if (fdp->remaining_bytes() < length) {
        png_error(png_ptr, "Insufficient fuzzer data");
        return;
    }
    
    std::vector<uint8_t> bytes = fdp->ConsumeBytes<uint8_t>(length);
    // length > 0 at this point, so bytes.data() is valid (even for empty vector)
    memcpy(data, bytes.data(), length);
}

// Helper function to allocate and fill palette from fuzzer data
png_color* create_palette_from_fdp(FuzzedDataProvider& fdp, int num_palette) {
    if (num_palette <= 0 || num_palette > 256) return NULL;
    
    // Each png_color has 3 bytes: red, green, blue
    size_t palette_size = num_palette * sizeof(png_color);
    png_color* palette = (png_color*)malloc(palette_size);
    if (palette == NULL) return NULL;
    
    // Fill palette with fuzzer data
    for (int i = 0; i < num_palette; i++) {
        if (fdp.remaining_bytes() >= 3) {
            palette[i].red = fdp.ConsumeIntegral<uint8_t>();
            palette[i].green = fdp.ConsumeIntegral<uint8_t>();
            palette[i].blue = fdp.ConsumeIntegral<uint8_t>();
        } else {
            // Use pattern if not enough data
            palette[i].red = (i * 7) % 256;
            palette[i].green = (i * 13) % 256;
            palette[i].blue = (i * 17) % 256;
        }
    }
    
    return palette;
}

// Helper function to create histogram from fuzzer data
png_uint_16* create_histogram_from_fdp(FuzzedDataProvider& fdp, int num_palette) {
    if (num_palette <= 0 || num_palette > 256) return NULL;
    
    png_uint_16* histogram = (png_uint_16*)malloc(num_palette * sizeof(png_uint_16));
    if (histogram == NULL) return NULL;
    
    // Fill histogram with fuzzer data
    for (int i = 0; i < num_palette; i++) {
        if (fdp.remaining_bytes() >= sizeof(png_uint_16)) {
            histogram[i] = fdp.ConsumeIntegral<png_uint_16>();
        } else {
            histogram[i] = i * 100; // Pattern
        }
    }
    
    return histogram;
}

// Helper function to create standard color cube palette (6x6x6)
void create_std_color_cube(png_color* palette, int max_colors) {
    int color_index = 0;
    for (int r = 0; r < 6 && color_index < max_colors; r++) {
        for (int g = 0; g < 6 && color_index < max_colors; g++) {
            for (int b = 0; b < 6 && color_index < max_colors; b++) {
                palette[color_index].red = (r * 51);   // 0, 51, 102, 153, 204, 255
                palette[color_index].green = (g * 51);
                palette[color_index].blue = (b * 51);
                color_index++;
            }
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for quantization testing
    // Minimum: palette size (1-256) + some palette data + control parameters
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume control parameters from fuzzer input
    int num_palette = fdp.ConsumeIntegralInRange<int>(1, 256);
    int maximum_colors = fdp.ConsumeIntegralInRange<int>(1, 256);
    int full_quantize = fdp.ConsumeBool() ? 1 : 0;
    int use_histogram = fdp.ConsumeBool();
    int use_std_color_cube = fdp.ConsumeBool();
    int color_type = fdp.ConsumeIntegralInRange<int>(0, 6);
    
    // Create PNG read structure for quantization testing
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 NULL, 
                                                 user_error_fn, 
                                                 user_warning_fn);
    if (png_ptr == NULL) {
        return 0;
    }
    
    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    // Set up error handling using setjmp/longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Set up read function to read from fuzzer input
    png_set_read_fn(png_ptr, &fdp, user_read_data);
    
    // Set basic image information (required for some quantization paths)
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
    int bit_depth = fdp.ConsumeIntegralInRange<int>(1, 16);
    int interlace_type = fdp.ConsumeIntegralInRange<int>(0, 1);
    int compression_type = fdp.ConsumeIntegralInRange<int>(0, 1);
    int filter_type = fdp.ConsumeIntegralInRange<int>(0, 5);
    
    // Set IHDR - this is needed for quantization to work properly
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);
    
    // Prepare palette and histogram data
    png_color* palette = NULL;
    png_uint_16* histogram = NULL;
    
    if (use_std_color_cube) {
        // Use standard color cube (6x6x6 = 216 colors)
        int cube_colors = 216;
        palette = (png_color*)malloc(cube_colors * sizeof(png_color));
        if (palette != NULL) {
            create_std_color_cube(palette, cube_colors);
            num_palette = cube_colors;
        }
    } else {
        // Create palette from fuzzer data
        palette = create_palette_from_fdp(fdp, num_palette);
    }
    
    if (use_histogram && palette != NULL) {
        histogram = create_histogram_from_fdp(fdp, num_palette);
    }
    
    // PRIMARY TEST: Call png_set_quantize with various parameters
    if (palette != NULL) {
        // Test 1: Basic quantization call
        png_set_quantize(png_ptr, palette, num_palette, maximum_colors, histogram, full_quantize);
        
        // Test 2: Edge case - num_palette equals maximum_colors
        if (num_palette > 1) {
            int equal_max = num_palette;
            png_set_quantize(png_ptr, palette, num_palette, equal_max, histogram, full_quantize);
        }
        
        // Test 3: Edge case - maximum_colors larger than num_palette
        int larger_max = num_palette + (num_palette / 2);
        if (larger_max > 256) larger_max = 256;
        png_set_quantize(png_ptr, palette, num_palette, larger_max, histogram, full_quantize);
        
        // Test 4: Edge case - maximum_colors = 1
        png_set_quantize(png_ptr, palette, num_palette, 1, histogram, full_quantize);
        
        // Test 5: With NULL histogram
        png_set_quantize(png_ptr, palette, num_palette, maximum_colors, NULL, full_quantize);
        
        // Test 6: Different full_quantize values
        png_set_quantize(png_ptr, palette, num_palette, maximum_colors, histogram, !full_quantize);
    }
    
    // Test related quantization functions if available
    // Note: libpng may have other quantization-related functions
    
    // Test quantization with different color types
    // Force palette-based color type to test quantization more thoroughly
    if ((color_type & PNG_COLOR_MASK_PALETTE) != 0 && palette != NULL) {
        // Set PLTE chunk to make palette available to libpng
        png_set_PLTE(png_ptr, info_ptr, palette, num_palette);
        
        // Test quantization after PLTE is set
        png_set_quantize(png_ptr, palette, num_palette, maximum_colors, histogram, full_quantize);
    }
    
    // Test reading update info (required for some quantization transformations)
    png_read_update_info(png_ptr, info_ptr);
    
    // Test with different image dimensions to exercise different code paths
    if (width > 1 && height > 1) {
        // Allocate row pointers for testing
        png_byte** row_pointers = (png_byte**)malloc(height * sizeof(png_byte*));
        if (row_pointers != NULL) {
            png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
            for (png_uint_32 y = 0; y < height; y++) {
                row_pointers[y] = (png_byte*)malloc(rowbytes);
                if (row_pointers[y] != NULL && fdp.remaining_bytes() >= rowbytes) {
                    std::vector<uint8_t> row_data = fdp.ConsumeBytes<uint8_t>(rowbytes);
                    memcpy(row_pointers[y], row_data.data(), rowbytes);
                }
            }
            
            // Test reading image with quantization applied
            png_read_image(png_ptr, row_pointers);
            
            // Free row pointers
            for (png_uint_32 y = 0; y < height; y++) {
                free(row_pointers[y]);
            }
            free(row_pointers);
        }
    }
    
    // Clean up allocated memory
    if (palette != NULL) {
        free(palette);
    }
    if (histogram != NULL) {
        free(histogram);
    }
    
    // Clean up PNG structures
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    return 0;
}

} // namespace harness_018
