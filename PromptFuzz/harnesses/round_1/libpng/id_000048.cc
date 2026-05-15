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
//<ID> 1052
//<Prompt> []
/*<Combination>: [
*/
//<score> 160, nr_unique_branch: 41
//<Quality> {"density":40,"unique_branches":{"png_write_frame_head":[[1523,8,1523,42,0,0,4,0]],"png_do_read_transformations":[[5122,11,5122,45,0,0,4,1],[5125,11,5125,48,0,0,4,1],[534,6,534,23,47,0,4,1]],"png_combine_row":[[3426,8,3426,21,0,0,4,0],[3432,11,3432,57,0,0,4,1],[3461,11,3461,26,0,0,4,0],[3596,14,3596,60,0,0,4,1],[3615,17,3615,23,0,0,4,0],[3615,17,3615,23,0,0,4,1],[3617,20,3617,29,0,0,4,0],[3627,17,3627,45,0,0,4,0],[3627,17,3627,45,0,0,4,1],[3689,13,3689,19,0,0,4,0],[3694,23,3694,49,0,0,4,0],[3694,23,3694,49,0,0,4,1],[3702,13,3702,19,0,0,4,0],[3710,23,3710,49,0,0,4,0],[3710,23,3710,49,0,0,4,1],[3717,23,3717,36,0,0,4,0],[3723,13,3723,19,0,0,4,0],[3731,23,3731,49,0,0,4,0],[3731,23,3731,49,0,0,4,1],[3739,13,3739,20,0,0,4,1],[3757,23,3757,66,0,0,4,1],[3774,32,3774,37,0,0,4,0],[3816,32,3816,37,0,0,4,0],[3816,32,3816,37,0,0,4,1],[3818,29,3818,55,0,0,4,0],[3818,29,3818,55,0,0,4,1],[3825,29,3825,55,0,0,4,0],[3869,8,3869,23,0,0,4,0],[534,6,534,23,2,0,4,1],[534,6,534,23,3,0,4,1],[3576,14,3576,23,21,0,4,1],[398,4,398,77,26,0,4,1],[534,6,534,23,32,0,4,1],[3551,33,3551,39,90,0,4,0],[3551,33,3551,39,90,0,4,1],[3551,43,3551,49,90,0,4,0],[3551,43,3551,49,90,0,4,1]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_set_gamma","png_set_read_user_transform_fn","png_set_user_transform_info","png_read_png","png_get_header_ver","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_get_gAMA","png_get_sCAL_s","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_set_IHDR","png_write_info","png_set_write_user_transform_fn","png_get_rows","png_write_frame_head","png_write_rows","png_write_end","png_free","png_free","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_set_gamma","png_set_read_user_transform_fn","png_set_user_transform_info","png_read_png","png_get_header_ver","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_get_gAMA","png_get_sCAL_s","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_write_info","png_set_write_user_transform_fn","png_get_rows","png_write_frame_head","png_write_rows","png_write_end","png_free","png_free","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_10, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_11, fdp);
	//fuzzer shim end}




    // Check minimum size for PNG signature
    if (size < 8) {
        return 0;
    }

    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    // Create FILE* for output data
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Check PNG signature
    png_byte sig[8];
    fread(sig, 1, 8, in_file);
    if (png_sig_cmp(sig, fuzz_uint64_t_1, 8) != 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Create read structure
    png_struct *png_read_ptr = png_create_read_struct(
        fuzz_str_2, 
        NULL, 
        NULL, 
        NULL
    );
    if (!png_read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 3: Create info structure for reading
    png_info *read_info_ptr = png_create_info_struct(png_read_ptr);
    if (!read_info_ptr) {
        png_destroy_read_struct(&png_read_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 4: Set up longjmp for error handling
    jmp_buf *jmpbuf = png_set_longjmp_fn(png_read_ptr, longjmp, sizeof(jmp_buf));
    if (setjmp(*jmpbuf)) {
	FUZZ_FILE_CLOSE();
        // Error occurred, clean up
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Initialize I/O
    png_init_io(png_read_ptr, in_file);
    png_set_sig_bytes(png_read_ptr, fuzz_int32_t_3);

    // Step 6: Set various transformations
    png_set_gamma(png_read_ptr, 2.2, 1.0);
    
    // Set user transform functions (dummy implementation)
    png_set_read_user_transform_fn(png_read_ptr, [](png_struct *png_ptr, png_row_info *row_info, png_byte *data) {
        // Simple transformation - do nothing
    });
    
    png_set_user_transform_info(png_read_ptr, NULL, fuzz_int32_t_4, fuzz_int32_t_5);

    // Step 7: Read PNG image
    png_read_png(png_read_ptr, read_info_ptr, fuzz_int32_t_6, NULL);

    // Step 8: Get header version (call API as required)
    const char *header_ver = png_get_header_ver(png_read_ptr);
    if (header_ver) {
        // Do nothing, just called as required
    }

    // Step 9: Get image properties
    png_uint_32 width = png_get_image_width(png_read_ptr, read_info_ptr);
    png_uint_32 height = png_get_image_height(png_read_ptr, read_info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_read_ptr, read_info_ptr);
    png_byte color_type = png_get_color_type(png_read_ptr, read_info_ptr);
    png_byte interlace_type = png_get_interlace_type(png_read_ptr, read_info_ptr);
    png_byte compression_type = png_get_compression_type(png_read_ptr, read_info_ptr);
    png_byte filter_type = png_get_filter_type(png_read_ptr, read_info_ptr);
    
    // Get gamma information
    double file_gamma;
    png_get_gAMA(png_read_ptr, read_info_ptr, &file_gamma);
    
    // Get sCAL information
    int unit;
    char *swidth = NULL, *sheight = NULL;
    png_get_sCAL_s(png_read_ptr, read_info_ptr, &unit, &swidth, &sheight);

    // Step 10: Create write structure
    png_struct *png_write_ptr = png_create_write_struct(
        fuzz_str_7,
        NULL,
        [](png_struct *png_ptr, const char *warning_message) {
            // This is png_benign_error, which we must use
        },
        NULL
    );
    if (!png_write_ptr) {
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 11: Create info structure for writing
    png_info *write_info_ptr = png_create_info_struct(png_write_ptr);
    if (!write_info_ptr) {
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, NULL);
        png_destroy_write_struct(&png_write_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 12: Set up error handling for write
    if (setjmp(png_jmpbuf(png_write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, NULL);
        png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 13: Initialize write I/O
    png_init_io(png_write_ptr, out_file);

    // Step 14: Set IHDR for output
    png_set_IHDR(png_write_ptr, write_info_ptr, 
                width, height, bit_depth, color_type,
                interlace_type, compression_type, filter_type);

    // Step 15: Write PNG info
    png_write_info(png_write_ptr, write_info_ptr);

    // Step 16: Set write user transform function
    png_set_write_user_transform_fn(png_write_ptr, [](png_struct *png_ptr, png_row_info *row_info, png_byte *data) {
        // Simple transformation - do nothing
    });

    // Step 17: Get rows from input image
    png_byte **rows = png_get_rows(png_read_ptr, read_info_ptr);

    // Step 18: Write frame head (for APNG support)
    if (rows) {
        png_write_frame_head(png_write_ptr, write_info_ptr, rows,
                            width, height, fuzz_uint32_t_8, fuzz_uint32_t_9, fuzz_uint16_t_10, fuzz_uint16_t_11, 0, 0);
    }

    // Step 19: Write rows
    png_write_rows(png_write_ptr, rows, height);

    // Step 20: Write end
    png_write_end(png_write_ptr, write_info_ptr);

    // Clean up sCAL allocated memory
    if (swidth) png_free(png_read_ptr, swidth);
    if (sheight) png_free(png_read_ptr, sheight);

    // Step 21: Destroy structures
    png_destroy_read_struct(&png_read_ptr, &read_info_ptr, NULL);
    png_destroy_write_struct(&png_write_ptr, &write_info_ptr);

    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}