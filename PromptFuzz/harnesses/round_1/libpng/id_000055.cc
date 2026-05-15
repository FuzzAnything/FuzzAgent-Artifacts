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
//<ID> 1284
//<Prompt> []
/*<Combination>: [
*/
//<score> 175, nr_unique_branch: 12
//<Quality> {"density":35,"unique_branches":{"png_get_cICP":[[769,9,769,24,0,0,4,0],[769,28,769,44,0,0,4,0],[770,9,770,47,0,0,4,0],[770,9,770,47,0,0,4,1],[771,9,771,33,0,0,4,0],[771,37,771,62,0,0,4,0],[772,9,772,36,0,0,4,0],[772,40,772,69,0,0,4,0]],"png_set_strip_alpha":[[202,8,202,37,0,0,4,1]],"png_check_fp_string":[[2238,8,2238,67,0,0,4,1]],"png_handle_sCAL":[[2276,8,2276,22,0,0,4,0],[2276,26,2276,40,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_read_info","png_set_strip_alpha","png_set_acTL","png_get_cICP","png_set_pCAL","png_set_next_frame_fcTL","png_set_cHRM_fixed","png_set_unknown_chunks","png_set_unknown_chunk_location","png_read_update_info","png_init_io","png_get_image_width","png_get_image_height","png_malloc","png_get_rowbytes","png_read_image","png_write_info","png_write_frame_head","png_write_image","png_write_frame_tail","png_write_end","png_free","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_read_info","png_set_strip_alpha","png_set_acTL","png_get_cICP","png_set_pCAL","png_set_next_frame_fcTL","png_set_cHRM_fixed","png_set_unknown_chunks","png_set_unknown_chunk_location","png_read_update_info","png_init_io","png_get_image_width","png_get_image_height","png_malloc","png_get_rowbytes","png_read_image","png_write_info","png_write_frame_head","png_write_image","png_write_frame_tail","png_write_end","png_free","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<99) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_11, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_12, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_16, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_17, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_18, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_19, fdp);
	//fuzzer shim end}




    if (size < 50) {
	return 0;
	}
	 // Need minimum data for PNG operations
    
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize libpng structures
    png_struct *png_ptr_r = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (!png_ptr_r) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_info *info_ptr_r = png_create_info_struct(png_ptr_r);
    if (!info_ptr_r) {
        png_destroy_read_struct(&png_ptr_r, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_struct *png_ptr_w = png_create_write_struct(fuzz_str_2, NULL, NULL, NULL);
    if (!png_ptr_w) {
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_info *info_ptr_w = png_create_info_struct(png_ptr_w);
    if (!info_ptr_w) {
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, NULL);
        png_destroy_write_struct(&png_ptr_w, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr_r)) || setjmp(png_jmpbuf(png_ptr_w))) {
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, NULL);
        png_destroy_write_struct(&png_ptr_w, &info_ptr_w);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Setup reading
    png_init_io(png_ptr_r, in_file);
    png_read_info(png_ptr_r, info_ptr_r);
    
    // 1. Use png_set_strip_alpha
    png_set_strip_alpha(png_ptr_r);
    
    // 2. Use png_get_int_32 (get a 32-bit integer from data)
    png_int_32 int_val = png_get_int_32((const png_byte*)(data + 4)); // Offset by 4 bytes
    
    // 3. Use png_set_acTL - set animation control
    png_set_acTL(png_ptr_w, info_ptr_w, fuzz_uint32_t_3, fuzz_uint32_t_4); // 2 frames, play once
    
    // 4. Use png_get_cICP - get colorimetric characteristics
    png_byte colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag;
    if (png_get_cICP(png_ptr_r, info_ptr_r, &colour_primaries, &transfer_function, 
                     &matrix_coefficients, &video_full_range_flag)) {
        // Do something with the values
    }
    
    // 5. Use png_set_pCAL - set pixel calibration
    char *params[] = {(char*)"param1", (char*)"param2"};
    png_set_pCAL(png_ptr_w, info_ptr_w, fuzz_str_5, 
                 int_val, int_val + 100, 1, fuzz_int32_t_7, fuzz_str_6, params);
    
    // 6. Use png_set_next_frame_fcTL - set frame control for APNG
    png_set_next_frame_fcTL(png_ptr_w, info_ptr_w, fuzz_uint32_t_8, fuzz_uint32_t_9, fuzz_uint32_t_10, fuzz_uint32_t_11, fuzz_uint16_t_12, fuzz_uint16_t_13, 0, 0);
    
    // 7. Use png_set_cHRM_fixed - set chromaticities
    png_set_cHRM_fixed(png_ptr_w, info_ptr_w, 
                       png_fixed_point(31270), png_fixed_point(32900),  // white x,y
                       png_fixed_point(64000), png_fixed_point(33000),  // red x,y
                       png_fixed_point(30000), png_fixed_point(60000),  // green x,y
                       png_fixed_point(15000), png_fixed_point( 6000)); // blue x,y
    
    // 8. Use png_set_unknown_chunk_location - set location for unknown chunks
    png_unknown_chunk unknown;
    memcpy(unknown.name, "tEST", 5);
    unknown.data = (png_byte*)"test";
    unknown.size = 4;
    unknown.location = 0;
    png_set_unknown_chunks(png_ptr_w, info_ptr_w, &unknown, 1);
    png_set_unknown_chunk_location(png_ptr_w, info_ptr_w, fuzz_int32_t_14, fuzz_int32_t_15);
    
    // Update reading info after transformations
    png_read_update_info(png_ptr_r, info_ptr_r);
    
    // Setup writing
    png_init_io(png_ptr_w, out_file);
    
    // Get image dimensions
    png_uint_32 width = png_get_image_width(png_ptr_r, info_ptr_r);
    png_uint_32 height = png_get_image_height(png_ptr_r, info_ptr_r);
    
    // Allocate row pointers
    png_bytep row_pointers[height];
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)png_malloc(png_ptr_r, 
            png_get_rowbytes(png_ptr_r, info_ptr_r));
    }
    
    // Read image data
    png_read_image(png_ptr_r, row_pointers);
    
    // Write image info
    png_write_info(png_ptr_w, info_ptr_w);
    
    // 9. Use png_write_frame_head - write APNG frame header
    png_write_frame_head(png_ptr_w, info_ptr_w, row_pointers, 
                         width, height, fuzz_uint32_t_16, fuzz_uint32_t_17, fuzz_uint16_t_18, fuzz_uint16_t_19, 0, 0);
    
    // Write image data
    png_write_image(png_ptr_w, row_pointers);
    png_write_frame_tail(png_ptr_w, info_ptr_w);
    
    // Write end
    png_write_end(png_ptr_w, info_ptr_w);
    
    // Cleanup
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr_r, row_pointers[y]);
    }
    
    png_destroy_read_struct(&png_ptr_r, &info_ptr_r, NULL);
    png_destroy_write_struct(&png_ptr_w, &info_ptr_w);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}