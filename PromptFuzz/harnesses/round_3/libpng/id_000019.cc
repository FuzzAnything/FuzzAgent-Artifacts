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
//<ID> 180
//<Prompt> []
/*<Combination>: [
*/
//<score> 152, nr_unique_branch: 3
//<Quality> {"density":38,"unique_branches":{"png_write_IHDR":[[713,13,713,20,0,0,4,0]],"png_get_chunk_malloc_max":[[1327,12,1327,19,0,0,4,0]],"png_get_user_transform_ptr":[[866,8,866,23,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_init_io","png_read_info","png_get_chunk_malloc_max","png_set_chunk_malloc_max","png_get_oFFs","png_set_user_transform_info","png_get_user_transform_ptr","png_write_info","png_set_cHRM_XYZ","png_get_image_width","png_get_image_height","png_malloc","png_get_rowbytes","png_malloc","png_read_row","png_write_frame_head","png_write_rows","png_write_frame_tail","png_write_chunk_start","png_write_chunk_data","png_write_chunk_end","png_free","png_free","png_read_end","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_init_io","png_read_info","png_get_chunk_malloc_max","png_set_chunk_malloc_max","png_get_oFFs","png_set_user_transform_info","png_get_user_transform_ptr","png_write_info","png_set_cHRM_XYZ","png_get_image_width","png_get_image_height","png_malloc","png_get_rowbytes","png_malloc","png_read_row","png_write_frame_head","png_write_rows","png_write_frame_tail","png_write_chunk_start","png_write_chunk_data","png_write_chunk_end","png_free","png_free","png_read_end","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<55) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_7, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_8, fdp);
		FDPConsumeCharArray(unsigned char, fuzz_char_array_9, fuzz_char_array_size_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
	
		const png_byte * fuzzer_var_png_write_chunk_start_31_1 = fuzz_char_array_9;//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful processing

    // Create input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize PNG structures
    png_struct *png_read_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (!png_read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_info *read_info_ptr = png_create_info_struct(png_read_ptr);
    if (!read_info_ptr) {
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_struct *png_write_ptr = png_create_write_struct(fuzz_str_2, nullptr, nullptr, nullptr);
    if (!png_write_ptr) {
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_info *write_info_ptr = png_create_info_struct(png_write_ptr);
    if (!write_info_ptr) {
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_read_ptr)) || setjmp(png_jmpbuf(png_write_ptr))) {
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize I/O
    png_init_io(png_read_ptr, in_file);
    png_init_io(png_write_ptr, out_file);

    // Read PNG info from input
    png_read_info(png_read_ptr, read_info_ptr);

    // Get chunk malloc max
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max(png_read_ptr);
    png_set_chunk_malloc_max(png_write_ptr, chunk_max);

    // Get offset information
    png_int_32 offset_x, offset_y;
    int unit_type;
    png_uint_32 offset_result = png_get_oFFs(png_read_ptr, read_info_ptr, &offset_x, &offset_y, &unit_type);

    // Set up user transform info
    int user_transform_data = 42; // Arbitrary data
    png_set_user_transform_info(png_write_ptr, &user_transform_data, fuzz_int32_t_3, fuzz_int32_t_4);

    // Get user transform pointer
    void *transform_ptr = png_get_user_transform_ptr(png_write_ptr);

    // Write basic info to output
    png_write_info(png_write_ptr, write_info_ptr);

    // Set cHRM XYZ color space (using arbitrary reasonable values)
    png_set_cHRM_XYZ(png_write_ptr, write_info_ptr,
                    0.4124564, 0.2126729, 0.0193339,  // Red X,Y,Z
                    0.3575761, 0.7151522, 0.1191920,  // Green X,Y,Z
                    0.1804375, 0.0721750, 0.9503041); // Blue X,Y,Z

    // Prepare for frame writing (for APNG)
    png_uint_32 width = png_get_image_width(png_read_ptr, read_info_ptr);
    png_uint_32 height = png_get_image_height(png_read_ptr, read_info_ptr);
    
    // Allocate row pointers
    png_byte **row_pointers = (png_byte**)png_malloc(png_write_ptr, height * sizeof(png_byte*));
    size_t rowbytes = png_get_rowbytes(png_read_ptr, read_info_ptr);
    
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)png_malloc(png_write_ptr, rowbytes);
        png_read_row(png_read_ptr, row_pointers[y], nullptr);
    }

    // Get uint16 from buffer for delay values
    png_uint_16 delay_num = 0, delay_den = 0;
    if (size >= 2) {
        delay_num = png_get_uint_16(data);
    }
    if (size >= 4) {
        delay_den = png_get_uint_16(data + 2);
    }

    // Write frame head (for APNG)
    png_write_frame_head(png_write_ptr, write_info_ptr, row_pointers,
                         width, height, fuzz_uint32_t_5, fuzz_uint32_t_6,  // x_offset, y_offset
                         fuzz_uint16_t_7, fuzz_uint16_t_8,  // delay
                         0, 0);                 // dispose_op, blend_op

    // Write rows
    png_write_rows(png_write_ptr, row_pointers, height);

    // Write frame tail
    png_write_frame_tail(png_write_ptr, write_info_ptr);

    // Write custom chunk to use png_write_chunk_end
    const png_byte chunk_name[] = { 't', 'E', 'S', 't' }; // Custom chunk
    png_write_chunk_start(png_write_ptr, fuzzer_var_png_write_chunk_start_31_1, fuzz_uint32_t_10);
    // Write some chunk data
    png_write_chunk_data(png_write_ptr, data, size < 10 ? size : 10);
    png_write_chunk_end(png_write_ptr);

    // Cleanup
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_write_ptr, row_pointers[y]);
    }
    png_free(png_write_ptr, row_pointers);

    png_read_end(png_read_ptr, read_info_ptr);
    png_write_end(png_write_ptr, write_info_ptr);

    // Destroy structures
    png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
    png_destroy_write_struct(&png_write_ptr, &write_info_ptr);

    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}