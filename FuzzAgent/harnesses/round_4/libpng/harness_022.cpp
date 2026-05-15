#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <zlib.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_022 {

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

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough data for all parameters
    // cICP: 4 bytes, mDCV: 10 doubles (80 bytes), mDCV_fixed: 8 fixed points + 2 uint32 (approx 40+8 bytes)
    if (size < 132) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Initialize PNG write structure
    png_structp png_ptr = png_create_write_struct(
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
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }
    
    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Step 3: Set basic image header with png_set_IHDR
    // Consume parameters for IHDR
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 4096);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 4096);
    int bit_depth = fdp.PickValueInArray({1, 2, 4, 8, 16});
    int color_type = fdp.PickValueInArray({PNG_COLOR_TYPE_GRAY, PNG_COLOR_TYPE_GRAY_ALPHA, 
                                           PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB, 
                                           PNG_COLOR_TYPE_RGB_ALPHA, PNG_COLOR_TYPE_RGBA});
    int interlace_method = fdp.PickValueInArray({PNG_INTERLACE_NONE, PNG_INTERLACE_ADAM7});
    int compression_method = PNG_COMPRESSION_TYPE_DEFAULT;
    int filter_method = PNG_FILTER_TYPE_DEFAULT;
    
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_method, compression_method, filter_method);
    
    // Now test cICP and mDCV color management functions
#ifdef PNG_cICP_SUPPORTED
    // Step 4: Set cICP color information
    png_byte colour_primaries = fdp.ConsumeIntegral<png_byte>();
    png_byte transfer_function = fdp.ConsumeIntegral<png_byte>();
    png_byte matrix_coefficients = fdp.ConsumeIntegral<png_byte>();
    png_byte video_full_range_flag = fdp.ConsumeIntegral<png_byte>();
    
    png_set_cICP(png_ptr, info_ptr, colour_primaries, transfer_function,
                 matrix_coefficients, video_full_range_flag);
    
    // Step 5: Get cICP to verify
    png_byte got_colour_primaries;
    png_byte got_transfer_function;
    png_byte got_matrix_coefficients;
    png_byte got_video_full_range_flag;
    
    png_uint_32 cICP_result = png_get_cICP(png_ptr, info_ptr, &got_colour_primaries,
                                           &got_transfer_function, &got_matrix_coefficients,
                                           &got_video_full_range_flag);
    (void)cICP_result; // Result can be ignored for fuzzing
#endif // PNG_cICP_SUPPORTED

#ifdef PNG_mDCV_SUPPORTED
    // Step 6: Set mDCV color information (floating point version)
    double white_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
    double white_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
    double red_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
    double red_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
    double green_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
    double green_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
    double blue_x = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
    double blue_y = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.3107);
    double mastering_display_maximum_luminance = fdp.ConsumeFloatingPointInRange<double>(0.0, 10000.0);
    double mastering_display_minimum_luminance = fdp.ConsumeFloatingPointInRange<double>(0.0, 1000.0);
    
    png_set_mDCV(png_ptr, info_ptr, white_x, white_y, red_x, red_y,
                 green_x, green_y, blue_x, blue_y,
                 mastering_display_maximum_luminance, mastering_display_minimum_luminance);
    
    // Step 7: Get mDCV to verify
    double got_white_x, got_white_y, got_red_x, got_red_y;
    double got_green_x, got_green_y, got_blue_x, got_blue_y;
    double got_max_luminance, got_min_luminance;
    
    png_uint_32 mDCV_result = png_get_mDCV(png_ptr, info_ptr, &got_white_x, &got_white_y,
                                           &got_red_x, &got_red_y, &got_green_x, &got_green_y,
                                           &got_blue_x, &got_blue_y,
                                           &got_max_luminance, &got_min_luminance);
    (void)mDCV_result; // Result can be ignored for fuzzing
    
    // Step 8: Set fixed-point mDCV values
    png_fixed_point int_white_x = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_white_y = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_red_x = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_red_y = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_green_x = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_green_y = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_blue_x = fdp.ConsumeIntegral<png_fixed_point>();
    png_fixed_point int_blue_y = fdp.ConsumeIntegral<png_fixed_point>();
    png_uint_32 mastering_display_maximum_luminance_scaled = fdp.ConsumeIntegral<png_uint_32>();
    png_uint_32 mastering_display_minimum_luminance_scaled = fdp.ConsumeIntegral<png_uint_32>();
    
    png_set_mDCV_fixed(png_ptr, info_ptr, int_white_x, int_white_y,
                       int_red_x, int_red_y, int_green_x, int_green_y,
                       int_blue_x, int_blue_y,
                       mastering_display_maximum_luminance_scaled,
                       mastering_display_minimum_luminance_scaled);
    
    // Step 9: Get fixed-point mDCV values to verify
    png_fixed_point got_int_white_x, got_int_white_y, got_int_red_x, got_int_red_y;
    png_fixed_point got_int_green_x, got_int_green_y, got_int_blue_x, got_int_blue_y;
    png_uint_32 got_max_luminance_scaled, got_min_luminance_scaled;
    
    png_uint_32 mDCV_fixed_result = png_get_mDCV_fixed(png_ptr, info_ptr,
                                                       &got_int_white_x, &got_int_white_y,
                                                       &got_int_red_x, &got_int_red_y,
                                                       &got_int_green_x, &got_int_green_y,
                                                       &got_int_blue_x, &got_int_blue_y,
                                                       &got_max_luminance_scaled,
                                                       &got_min_luminance_scaled);
    (void)mDCV_fixed_result; // Result can be ignored for fuzzing
#endif // PNG_mDCV_SUPPORTED

    // Step 10: Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}

} // namespace harness_022
