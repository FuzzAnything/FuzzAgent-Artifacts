#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <zlib.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

namespace harness_013 {

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

// Custom write function for memory buffer
struct png_memory_buffer {
    uint8_t* data;
    size_t size;
    size_t capacity;
    size_t offset;
};

static void pngtest_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_memory_buffer* buffer = (png_memory_buffer*)png_get_io_ptr(png_ptr);
    
    // Resize buffer if needed
    if (buffer->offset + length > buffer->capacity) {
        size_t new_capacity = buffer->capacity * 2;
        if (new_capacity < buffer->offset + length) {
            new_capacity = buffer->offset + length;
        }
        buffer->data = (uint8_t*)realloc(buffer->data, new_capacity);
        buffer->capacity = new_capacity;
    }
    
    memcpy(buffer->data + buffer->offset, data, length);
    buffer->offset += length;
    buffer->size = buffer->offset;
}

static void pngtest_flush_data(png_structp png_ptr) {
    (void)png_ptr;
    // Do nothing
}

// Custom read function for memory buffer
static void pngtest_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_memory_buffer* buffer = (png_memory_buffer*)png_get_io_ptr(png_ptr);
    
    if (buffer->offset + length > buffer->size) {
        png_error(png_ptr, "Read beyond buffer");
        return;
    }
    
    memcpy(data, buffer->data + buffer->offset, length);
    buffer->offset += length;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size calculation for chromaticity operations:
    // - PNG structures initialization: minimal
    // - Basic image parameters (width, height, etc.): ~20 bytes
    // - Chromaticity coordinates: 9 png_fixed_point values for cHRM_XYZ_fixed = 9 * 4 = 36 bytes
    // - Legacy chromaticity (cLLI): 2 png_uint_32 values = 2 * 4 = 8 bytes
    // - Return value storage: 9 png_fixed_point + 2 png_uint_32 = 44 bytes
    // Total minimum: ~100+ bytes
    if (size < 150) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ============================================
    // PART 1: INITIALIZATION AND BASIC SETUP
    // ============================================
    
    // Step 1: Create PNG read structure
    png_structp png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        pngtest_error, 
        pngtest_warning);
    
    if (!png_ptr) {
        return 0;
    }
    
    // Step 2: Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // ============================================
    // PART 2: BASIC IMAGE SETUP
    // ============================================
    
    // Consume basic image parameters from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 1000);
    int bit_depth = fdp.ConsumeIntegralInRange(1, 16);
    int color_type = fdp.PickValueInArray({PNG_COLOR_TYPE_GRAY, PNG_COLOR_TYPE_RGB, 
                                          PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_GRAY_ALPHA, 
                                          PNG_COLOR_TYPE_RGB_ALPHA});
    int interlace_method = fdp.ConsumeBool() ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE;
    int compression_method = PNG_COMPRESSION_TYPE_BASE;
    int filter_method = PNG_FILTER_TYPE_BASE;
    
    // Set up IHDR (Image Header) - required for chromaticity support
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_method, compression_method, filter_method);
    
    // ============================================
    // PART 3: CHROMATICITY (cHRM) OPERATIONS
    // ============================================
    
    // Consume chromaticity coordinates from fuzzer input
    // 9 png_fixed_point values for XYZ chromaticity
    png_fixed_point red_X = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point red_Y = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point red_Z = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point green_X = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point green_Y = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point green_Z = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point blue_X = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point blue_Y = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point blue_Z = fdp.ConsumeIntegral<png_fixed_point>();
    
    // Set chromaticity coordinates (cHRM) - TARGET API 1
    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr,
                          red_X, red_Y, red_Z,
                          green_X, green_Y, green_Z,
                          blue_X, blue_Y, blue_Z);
    
    // ============================================
    // PART 4: LEGACY CHROMATICITY (cLLI) OPERATIONS
    // ============================================
    
    // Consume legacy chromaticity values from fuzzer input
    // cLLI values are scaled by 10,000 (not 100,000 like png_fixed_point)
    png_uint_32 max_content_light = fdp.ConsumeIntegral<png_uint_32>();
    png_uint_32 max_frame_avg_light = fdp.ConsumeIntegral<png_uint_32>();
    
    // Choose which cLLI API to test based on fuzzer input
    bool use_fixed_api = fdp.ConsumeBool();
    
    if (use_fixed_api) {
        // Set legacy chromaticity with fixed-point API - TARGET API 2
        png_set_cLLI_fixed(png_ptr, info_ptr, max_content_light, max_frame_avg_light);
    } else {
        // Convert scaled values to double for floating-point API
        double max_content_light_double = static_cast<double>(max_content_light) / 10000.0;
        double max_frame_avg_light_double = static_cast<double>(max_frame_avg_light) / 10000.0;
        
        // Set legacy chromaticity with floating-point API - TARGET API 3
        png_set_cLLI(png_ptr, info_ptr, max_content_light_double, max_frame_avg_light_double);
    }
    
    // ============================================
    // PART 5: RETRIEVE CHROMATICITY DATA
    // ============================================
    
    // Variables to store retrieved chromaticity data
    png_fixed_point retrieved_red_X, retrieved_red_Y, retrieved_red_Z;
    png_fixed_point retrieved_green_X, retrieved_green_Y, retrieved_green_Z;
    png_fixed_point retrieved_blue_X, retrieved_blue_Y, retrieved_blue_Z;
    
    // Choose which getter API to test
    bool use_fixed_getter = fdp.ConsumeBool();
    
    if (use_fixed_getter) {
        // Retrieve chromaticity with fixed-point getter - TARGET API 4
        png_uint_32 ret = png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
                                                &retrieved_red_X, &retrieved_red_Y, &retrieved_red_Z,
                                                &retrieved_green_X, &retrieved_green_Y, &retrieved_green_Z,
                                                &retrieved_blue_X, &retrieved_blue_Y, &retrieved_blue_Z);
        (void)ret; // Return value can be ignored for fuzzing
    } else {
        // Variables for floating-point retrieval
        double d_red_X, d_red_Y, d_red_Z;
        double d_green_X, d_green_Y, d_green_Z;
        double d_blue_X, d_blue_Y, d_blue_Z;
        
        // Retrieve chromaticity with floating-point getter - TARGET API 5
        png_uint_32 ret = png_get_cHRM_XYZ(png_ptr, info_ptr,
                                          &d_red_X, &d_red_Y, &d_red_Z,
                                          &d_green_X, &d_green_Y, &d_green_Z,
                                          &d_blue_X, &d_blue_Y, &d_blue_Z);
        (void)ret; // Return value can be ignored for fuzzing
    }
    
    // ============================================
    // PART 6: RETRIEVE LEGACY CHROMATICITY DATA
    // ============================================
    
    // Variables to store retrieved legacy chromaticity data
    png_uint_32 retrieved_max_content_light = 0;
    png_uint_32 retrieved_max_frame_avg_light = 0;
    double retrieved_max_content_light_double = 0.0;
    double retrieved_max_frame_avg_light_double = 0.0;
    
    // Choose which cLLI getter API to test
    bool use_fixed_cLLI_getter = fdp.ConsumeBool();
    
    if (use_fixed_cLLI_getter) {
        // Retrieve legacy chromaticity with fixed-point getter - TARGET API 6
        png_uint_32 ret = png_get_cLLI_fixed(png_ptr, info_ptr,
                                            &retrieved_max_content_light,
                                            &retrieved_max_frame_avg_light);
        (void)ret; // Return value can be ignored for fuzzing
    } else {
        // Retrieve legacy chromaticity with floating-point getter - TARGET API 7
        png_uint_32 ret = png_get_cLLI(png_ptr, info_ptr,
                                      &retrieved_max_content_light_double,
                                      &retrieved_max_frame_avg_light_double);
        (void)ret; // Return value can be ignored for fuzzing
    }
    
    // ============================================
    // PART 7: CLEANUP
    // ============================================
    
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}

} // namespace harness_013
