#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>

namespace harness_017 {

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

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for meaningful testing: enough for basic PNG setup
    // plus pHYs parameters
    if (size < 50) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume pHYs parameters from fuzzer input
    png_uint_32 res_x = fdp.ConsumeIntegral<png_uint_32>();
    png_uint_32 res_y = fdp.ConsumeIntegral<png_uint_32>();
    int unit_type = fdp.ConsumeIntegralInRange<int>(0, 2); // PNG_RESOLUTION_UNKNOWN, PNG_RESOLUTION_METER, PNG_RESOLUTION_INCH
    
    // Consume image parameters for basic PNG
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
    int bit_depth = fdp.ConsumeIntegralInRange<int>(1, 16);
    int color_type = fdp.ConsumeIntegralInRange<int>(0, 6);
    int interlace_type = fdp.ConsumeIntegralInRange<int>(0, 1);
    int compression_type = fdp.ConsumeIntegralInRange<int>(0, 1);
    int filter_type = fdp.ConsumeIntegralInRange<int>(0, 5);
    
    // Create PNG write structure (we'll create a PNG to test pHYs retrieval)
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
    
    // SETUP: Set pHYs data using png_set_pHYs
    png_set_pHYs(png_ptr, info_ptr, res_x, res_y, unit_type);
    
    // EXECUTION: Test all pHYs retrieval functions
    
    // 1. Test png_get_pHYs - get resolution and unit type
    png_uint_32 retrieved_res_x, retrieved_res_y;
    int retrieved_unit_type;
    png_uint_32 pHYs_valid = png_get_pHYs(png_ptr, info_ptr, &retrieved_res_x, &retrieved_res_y, &retrieved_unit_type);
    
    // 2. Test png_get_pHYs_dpi - get DPI resolution
    png_uint_32 dpi_res_x, dpi_res_y;
    int dpi_unit_type;
    png_uint_32 pHYs_dpi_valid = png_get_pHYs_dpi(png_ptr, info_ptr, &dpi_res_x, &dpi_res_y, &dpi_unit_type);
    
    // 3. Test png_get_pixels_per_meter - get pixels per meter
    png_uint_32 pixels_per_meter = png_get_pixels_per_meter(png_ptr, info_ptr);
    
    // 4. Test png_get_pixels_per_inch - get pixels per inch
    png_uint_32 pixels_per_inch = png_get_pixels_per_inch(png_ptr, info_ptr);
    
    // 5. Test png_get_x_pixels_per_meter - get X pixels per meter
    png_uint_32 x_pixels_per_meter = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    
    // 6. Test png_get_y_pixels_per_meter - get Y pixels per meter
    png_uint_32 y_pixels_per_meter = png_get_y_pixels_per_meter(png_ptr, info_ptr);
    
    // 7. Test png_get_x_pixels_per_inch - get X pixels per inch
    png_uint_32 x_pixels_per_inch = png_get_x_pixels_per_inch(png_ptr, info_ptr);
    
    // 8. Test png_get_y_pixels_per_inch - get Y pixels per inch  
    png_uint_32 y_pixels_per_inch = png_get_y_pixels_per_inch(png_ptr, info_ptr);
    
    // Also test with NULL pointers to exercise different code paths
    png_uint_32 pHYs_null_x = png_get_pHYs(png_ptr, info_ptr, NULL, &retrieved_res_y, &retrieved_unit_type);
    png_uint_32 pHYs_null_y = png_get_pHYs(png_ptr, info_ptr, &retrieved_res_x, NULL, &retrieved_unit_type);
    png_uint_32 pHYs_null_unit = png_get_pHYs(png_ptr, info_ptr, &retrieved_res_x, &retrieved_res_y, NULL);
    png_uint_32 pHYs_all_null = png_get_pHYs(png_ptr, info_ptr, NULL, NULL, NULL);
    
    // Test png_get_pHYs_dpi with NULL pointers
    png_uint_32 pHYs_dpi_null_x = png_get_pHYs_dpi(png_ptr, info_ptr, NULL, &dpi_res_y, &dpi_unit_type);
    png_uint_32 pHYs_dpi_null_y = png_get_pHYs_dpi(png_ptr, info_ptr, &dpi_res_x, NULL, &dpi_unit_type);
    png_uint_32 pHYs_dpi_null_unit = png_get_pHYs_dpi(png_ptr, info_ptr, &dpi_res_x, &dpi_res_y, NULL);
    png_uint_32 pHYs_dpi_all_null = png_get_pHYs_dpi(png_ptr, info_ptr, NULL, NULL, NULL);
    
    // Additional tests: Check if pHYs info is valid
    png_uint_32 valid_flags = png_get_valid(png_ptr, info_ptr, PNG_INFO_pHYs);
    bool pHYs_is_valid = (valid_flags & PNG_INFO_pHYs) != 0;
    
    // Also test edge cases with zero resolution values
    png_set_pHYs(png_ptr, info_ptr, 0, 0, unit_type);
    png_uint_32 zero_pHYs_valid = png_get_pHYs(png_ptr, info_ptr, &retrieved_res_x, &retrieved_res_y, &retrieved_unit_type);
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}

} // namespace harness_017
