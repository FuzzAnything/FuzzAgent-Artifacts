#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>
#include <algorithm>
#include <cstdint>

namespace harness_019 {

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

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for PNG header and chunk testing
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create PNG read structure
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
    
    // Set up custom read function using fuzzer data
    png_set_read_fn(png_ptr, &fdp, user_read_data);
    
    // Read PNG info - this will parse the PNG header and chunks
    png_read_info(png_ptr, info_ptr);
    
    // TARGET 1: png_get_oFFs - get image offset
    png_int_32 offset_x, offset_y;
    int unit_type;
    png_uint_32 oFFs_valid = png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, &unit_type);
    
    // TARGET 2: png_get_tIME - get modification time
    png_timep mod_time;
    png_uint_32 tIME_valid = png_get_tIME(png_ptr, info_ptr, &mod_time);
    
    // TARGET 3: png_get_sRGB - get sRGB intent
    int srgb_intent;
    png_uint_32 sRGB_valid = png_get_sRGB(png_ptr, info_ptr, &srgb_intent);
    
    // TARGET 4: png_get_bKGD - get background color
    png_color_16p background;
    png_uint_32 bKGD_valid = png_get_bKGD(png_ptr, info_ptr, &background);
    
    // TARGET 5: png_get_hIST - get histogram
    png_uint_16p histogram;
    png_uint_32 hIST_valid = png_get_hIST(png_ptr, info_ptr, &histogram);
    
    // TARGET 6: png_get_sBIT - get significant bits
    png_color_8p sig_bit;
    png_uint_32 sBIT_valid = png_get_sBIT(png_ptr, info_ptr, &sig_bit);
    
    // TARGET 7: png_get_sPLT - get suggested palette
    png_sPLT_tp sPLT_tables;
    int sPLT_valid = png_get_sPLT(png_ptr, info_ptr, &sPLT_tables);
    
    // TARGET 8: png_get_unknown_chunks - get unknown chunks
    png_unknown_chunkp unknown_chunks;
    int unknown_chunks_valid = png_get_unknown_chunks(png_ptr, info_ptr, &unknown_chunks);
    
    // TARGET 9: png_get_cICP - get coding-independent code points
    png_byte colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag;
    png_uint_32 cICP_valid = png_get_cICP(png_ptr, info_ptr, &colour_primaries, &transfer_function, &matrix_coefficients, &video_full_range_flag);
    
    // TARGET 10: png_get_pixel_aspect_ratio_fixed - get pixel aspect ratio
    png_fixed_point pixel_aspect_ratio = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
    
    // Also test edge cases with NULL pointers
    png_uint_32 oFFs_null = png_get_oFFs(png_ptr, info_ptr, NULL, NULL, NULL);
    png_uint_32 tIME_null = png_get_tIME(png_ptr, info_ptr, NULL);
    png_uint_32 sRGB_null = png_get_sRGB(png_ptr, info_ptr, NULL);
    png_uint_32 bKGD_null = png_get_bKGD(png_ptr, info_ptr, NULL);
    png_uint_32 hIST_null = png_get_hIST(png_ptr, info_ptr, NULL);
    png_uint_32 sBIT_null = png_get_sBIT(png_ptr, info_ptr, NULL);
    int sPLT_null = png_get_sPLT(png_ptr, info_ptr, NULL);
    int unknown_chunks_null = png_get_unknown_chunks(png_ptr, info_ptr, NULL);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    return 0;
}

} // namespace harness_019
