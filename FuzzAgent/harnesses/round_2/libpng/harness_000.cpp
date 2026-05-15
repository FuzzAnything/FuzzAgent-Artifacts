#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>

namespace harness_000 {

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

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 100) return 0; // Need minimum data for meaningful testing
    
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
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Get image dimensions and format
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type, compression_type, filter_type;
    
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, &compression_type, &filter_type);
    
    // Apply some transformations based on fuzzer input
    uint8_t transform_flags = fdp.ConsumeIntegral<uint8_t>();
    
    if (transform_flags & 0x01) {
        // Strip 16-bit to 8-bit
        png_set_strip_16(png_ptr);
    }
    
    if (transform_flags & 0x02 && (color_type & PNG_COLOR_MASK_ALPHA)) {
        // Strip alpha channel
        png_set_strip_alpha(png_ptr);
    }
    
    if (transform_flags & 0x04) {
        // Expand paletted images to RGB
        if (color_type == PNG_COLOR_TYPE_PALETTE) {
            png_set_palette_to_rgb(png_ptr);
        }
    }
    
    if (transform_flags & 0x08) {
        // Expand grayscale to 8-bit
        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
            png_set_expand_gray_1_2_4_to_8(png_ptr);
        }
    }
    
    // Update info after transformations
    png_read_update_info(png_ptr, info_ptr);
    
    // Get updated image info
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, &compression_type, &filter_type);
    
    // Calculate row bytes
    png_size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    
    // Limit dimensions to prevent excessive memory usage
    if (width > 4096 || height > 4096 || row_bytes > 1048576) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Allocate memory for rows
    png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)png_malloc(png_ptr, row_bytes);
    }
    
    // Read the image data
    png_read_image(png_ptr, row_pointers);
    
    // Read the end of PNG file
    png_read_end(png_ptr, NULL);
    
    // Test some getter functions
    int num_palette = 0;
    png_colorp palette = NULL;
    if (png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette)) {
        // Palette retrieved
    }
    
    png_bytep trans_alpha = NULL;
    int num_trans = 0;
    png_color_16p trans_color = NULL;
    if (png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, &trans_color)) {
        // Transparency retrieved
    }
    
    double gamma;
    if (png_get_gAMA(png_ptr, info_ptr, &gamma)) {
        // Gamma retrieved
    }
    
    png_charp text = NULL;
    int num_text = 0;
    png_textp text_ptr = NULL;
    if (png_get_text(png_ptr, info_ptr, &text_ptr, &num_text)) {
        // Text chunks retrieved
    }
    
    // Free allocated memory
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    return 0;
}

} // namespace harness_000
