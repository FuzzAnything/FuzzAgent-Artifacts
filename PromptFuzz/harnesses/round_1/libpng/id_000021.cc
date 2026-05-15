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
//<ID> 340
//<Prompt> []
/*<Combination>: [
*/
//<score> 119, nr_unique_branch: 35
//<Quality> {"density":34,"unique_branches":{"png_read_image":[[708,8,708,49,0,0,4,0]],"png_do_read_transformations":[[4862,8,4862,61,0,0,4,1],[5108,8,5108,60,0,0,4,1]],"png_permit_mng_features":[[1649,8,1649,23,0,0,4,1]],"png_handle_PLTE":[[981,13,981,49,0,0,4,0]],"png_read_end":[[825,16,825,38,0,0,4,0],[833,15,833,25,0,0,4,0],[833,29,833,71,0,0,4,1],[834,17,834,65,0,0,4,1]],"png_data_freer":[[466,8,466,23,0,0,4,1],[466,27,466,43,0,0,4,1],[469,8,469,43,0,0,4,0]],"png_check_IHDR":[[2033,53,2033,66,0,0,4,0],[2067,13,2067,76,0,0,4,0],[2068,11,2068,55,0,0,4,1]],"png_start_read_image":[[297,11,297,52,0,0,4,0]],"png_read_row":[[555,8,555,71,0,0,4,0],[556,8,556,61,0,0,4,1],[564,8,564,32,0,0,4,1],[566,13,566,42,0,0,4,0],[595,11,595,22,0,0,4,1]],"png_set_PLTE":[[770,16,770,80,0,0,4,1],[801,8,801,23,0,0,4,1]],"png_free_data":[[546,11,546,40,0,0,4,1],[562,8,562,57,0,0,4,0],[631,11,631,25,0,0,4,1],[642,8,642,57,0,0,4,0],[663,11,663,41,0,0,4,1]],"png_set_filler":[[158,8,158,49,0,0,4,0],[224,8,224,38,0,0,4,0]],"png_get_y_offset_microns":[[269,8,269,23,0,0,4,0],[269,27,269,43,0,0,4,0],[270,8,270,46,0,0,4,0],[270,8,270,46,0,0,4,1],[272,11,272,62,0,0,4,0]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_permit_mng_features","png_read_info","png_get_acTL","png_read_frame_head","png_get_next_frame_blend_op","png_read_rows","png_get_image_height","png_read_image","png_get_y_offset_inches_fixed","png_get_color_type","png_set_filler","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_set_IHDR","png_write_info","png_write_end","png_destroy_info_struct","png_destroy_write_struct","png_data_freer","png_read_end","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_permit_mng_features","png_read_info","png_get_acTL","png_read_frame_head","png_get_next_frame_blend_op","png_read_rows","png_get_y_offset_inches_fixed","png_get_color_type","png_set_filler","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_set_IHDR","png_write_info","png_write_end","png_destroy_info_struct","png_destroy_write_struct","png_data_freer","png_read_end","png_destroy_read_struct"],"visited":3}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to process PNG data. The event this achieves is: reading a PNG image (potentially animated APNG), processing its frames, adding alpha filler, and extracting metadata.

*/



extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<52) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	  // Need at least PNG signature
    
    // Create input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
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
    if (png_sig_cmp(sig, fuzz_uint64_t_1, 8) != 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;  // Not a PNG file
    }
    
    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(fuzz_str_2, 
                                                 nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize info structure (using png_info_init_3 equivalent)
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up PNG reading
    png_init_io(png_ptr, in_file);
    png_set_sig_bytes(png_ptr, fuzz_int32_t_3);
    
    // Permit MNG features (for APNG support)
    png_permit_mng_features(png_ptr, fuzz_uint32_t_4);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Extract and process frames (for APNG)
    png_uint_32 num_frames = 0, num_plays = 0;
    png_get_acTL(png_ptr, info_ptr, &num_frames, &num_plays);
    
    // Process each frame if animated
    if (num_frames > 0) {
        for (png_uint_32 frame = 0; frame < num_frames; frame++) {
            if (frame > 0) {
                // Read next frame header
                png_read_frame_head(png_ptr, info_ptr);
            }
            
            // Get blend operation for this frame
            png_byte blend_op = png_get_next_frame_blend_op(png_ptr, info_ptr);
            (void)blend_op;  // Use variable to avoid unused warning
            
            // Read frame data
            png_bytep row_pointers[1];
            png_read_rows(png_ptr, row_pointers, nullptr, 1);
        }
    } else {
        // Read single image
        png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
        png_bytep row_pointers[height];
        png_read_image(png_ptr, row_pointers);
    }
    
    // Get Y offset in inches (fixed point)
    png_fixed_point y_offset_inches = png_get_y_offset_inches_fixed(png_ptr, info_ptr);
    (void)y_offset_inches;  // Use variable
    
    // Set filler for RGBA conversion (if needed)
    int color_type = png_get_color_type(png_ptr, info_ptr);
    if (!(color_type & PNG_COLOR_MASK_ALPHA)) {
        png_set_filler(png_ptr, fuzz_uint32_t_5, fuzz_int32_t_6);
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create output file to test writing
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file) {
        png_structp write_ptr = png_create_write_struct(fuzz_str_7,
                                                       nullptr, nullptr, nullptr);
        if (write_ptr) {
            png_infop write_info = png_create_info_struct(write_ptr);
            if (write_info) {
                if (!setjmp(png_jmpbuf(write_ptr))) {
                    png_init_io(write_ptr, out_file);
                    
                    // Copy info from read to write
                    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
                    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
                    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
                    int interlace = png_get_interlace_type(png_ptr, info_ptr);
                    int compression = png_get_compression_type(png_ptr, info_ptr);
                    int filter = png_get_filter_type(png_ptr, info_ptr);
                    
                    png_set_IHDR(write_ptr, write_info, width, height, bit_depth,
                               color_type | PNG_COLOR_MASK_ALPHA, interlace,
                               compression, filter);
                    
                    png_write_info(write_ptr, write_info);
                    png_write_end(write_ptr, nullptr);
                }
                png_destroy_info_struct(write_ptr, &write_info);
            }
            png_destroy_write_struct(&write_ptr, nullptr);
        }
        assert_file_closed(&out_file);;
    }
    
    // Data freer (clean up data chunks)
    png_data_freer(png_ptr, info_ptr, PNG_DESTROY_WILL_FREE_DATA, 
                   fuzz_uint32_t_8);
    
    // Clean up
    png_read_end(png_ptr, info_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}