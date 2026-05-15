#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <png.h>
}
//<ID> 370
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 1
//<Quality> {"density":33,"unique_branches":{"png_permit_mng_features":[[1649,8,1649,23,0,0,4,1]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_set_sig_bytes","png_init_io","png_permit_mng_features","png_set_expand_16","png_set_invert_alpha","png_read_info","png_get_y_offset_pixels","png_get_next_frame_blend_op","png_get_IHDR","png_set_IHDR","png_read_update_info","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_read_end","png_write_end","png_free","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_write_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_init_io","png_permit_mng_features","png_set_expand_16","png_set_invert_alpha","png_read_info","png_get_y_offset_pixels","png_get_next_frame_blend_op","png_get_IHDR","png_set_IHDR","png_read_update_info","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_read_end","png_write_end","png_free","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/*Here's a C++ fuzzer that uses the specified libpng APIs to process PNG data:

*/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if data is too small for PNG processing
    if (size < 8) {
	return 0;
	}
	
    
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    png_struct *png_read_ptr = nullptr;
    png_struct *png_write_ptr = nullptr;
    png_info *info_ptr = nullptr;
    png_info *end_info = nullptr;
    
    // Open input file from memory buffer
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Check PNG signature
    unsigned char sig[8];
    if (fread(sig, 1, 8, in_file) != 8) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    if (png_sig_cmp(sig, 0, 8) != 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    rewind(in_file);
    
    // Open output file
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create PNG read structure
    png_read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create PNG write structure
    png_write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_write_ptr) {
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize info structures using png_info_init_3
    info_ptr = png_create_info_struct(png_read_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    end_info = png_create_info_struct(png_read_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_read_ptr, &info_ptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_read_ptr)) || setjmp(png_jmpbuf(png_write_ptr))) {
        png_destroy_read_struct(&png_read_ptr, &info_ptr, &end_info);
        png_destroy_write_struct(&png_write_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up I/O for reading
    png_init_io(png_read_ptr, in_file);
    png_set_sig_bytes(png_read_ptr, 8);
    
    // Set up I/O for writing
    png_init_io(png_write_ptr, out_file);
    
    // Apply the specified APIs
    
    // 1. Permit MNG features (allow MNG functionality)
    png_uint_32 permitted_features = PNG_FLAG_MNG_EMPTY_PLTE | PNG_FLAG_MNG_FILTER_64;
    png_permit_mng_features(png_read_ptr, permitted_features);
    
    // 2. Enable 16-bit expansion
    png_set_expand_16(png_read_ptr);
    
    // 3. Invert alpha channel
    png_set_invert_alpha(png_read_ptr);
    
    // Read PNG info
    png_read_info(png_read_ptr, info_ptr);
    
    // 4. Get y-offset in pixels (for APNG frames)
    png_int_32 y_offset = png_get_y_offset_pixels(png_read_ptr, info_ptr);
    
    // 5. Get next frame blend operation (for APNG)
    png_byte blend_op = png_get_next_frame_blend_op(png_read_ptr, info_ptr);
    
    // Process image data
    png_uint_32 width, height;
    int bit_depth, color_type;
    png_get_IHDR(png_read_ptr, info_ptr, &width, &height, &bit_depth, 
                 &color_type, nullptr, nullptr, nullptr);
    
    // Update info for writing
    png_set_IHDR(png_write_ptr, info_ptr, width, height, bit_depth, 
                 color_type, PNG_INTERLACE_NONE, 
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    
    // Copy other chunks
    png_read_update_info(png_read_ptr, info_ptr);
    
    // Write header
    png_write_info(png_write_ptr, info_ptr);
    
    // Allocate row buffers
    png_bytep row = (png_bytep)png_malloc(png_read_ptr, 
                                         png_get_rowbytes(png_read_ptr, info_ptr));
    
    // Read and write image data
    for (png_uint_32 y = 0; y < height; y++) {
        png_read_row(png_read_ptr, row, nullptr);
        png_write_row(png_write_ptr, row);
    }
    
    // Read and write end info
    png_read_end(png_read_ptr, end_info);
    png_write_end(png_write_ptr, nullptr);
    
    // Cleanup
    png_free(png_read_ptr, row);
    png_destroy_read_struct(&png_read_ptr, &info_ptr, &end_info);
    png_destroy_write_struct(&png_write_ptr, nullptr);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}