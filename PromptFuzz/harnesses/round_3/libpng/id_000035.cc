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
//<ID> 724
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 5
//<Quality> {"density":15,"unique_branches":{"png_get_user_height_max":[[1313,12,1313,19,0,0,4,0]],"png_get_next_frame_y_offset":[[1463,8,1463,23,0,0,4,0],[1463,27,1463,43,0,0,4,0]],"png_get_num_frames":[[1383,8,1383,23,0,0,4,0],[1383,27,1383,43,0,0,4,0]]},"library_calls":["png_create_write_struct_2","png_create_info_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_write_struct","png_init_io","png_write_sig","png_set_user_transform_info","png_create_read_struct","png_destroy_write_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_read_info","png_get_io_state","png_get_user_height_max","png_get_num_frames","png_get_next_frame_y_offset","png_destroy_read_struct","png_set_IHDR","png_write_info","png_write_row","png_write_end","png_destroy_write_struct"],"critical_calls":["png_create_write_struct_2","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_write_sig","png_set_user_transform_info","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_io_state","png_get_user_height_max","png_get_num_frames","png_get_next_frame_y_offset","png_destroy_read_struct","png_set_IHDR","png_write_info","png_write_row","png_write_end","png_destroy_write_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<44) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 8) {
	return 0;
	}
	
    
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	FUZZ_FILENAME_PUSH("output_file")
	
    
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    
    // Create write structure with custom memory management
    png_ptr = png_create_write_struct_2(
        fuzz_str_1,
        nullptr,  // error_ptr
        nullptr,  // error_fn
        nullptr,  // warn_fn
        nullptr,  // mem_ptr
        nullptr,  // malloc_fn (use default)
        nullptr   // free_fn (use default)
    );
    
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create info structure
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&png_ptr, &info_ptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O for writing
    png_init_io(png_ptr, out_file);
    
    // Write PNG signature
    png_write_sig(png_ptr);
    
    // Set user transform info (dummy values for demonstration)
    void* user_transform_ptr = nullptr;
    int user_transform_depth = 8;
    int user_transform_channels = 4;
    png_set_user_transform_info(png_ptr, user_transform_ptr, 
                               fuzz_int32_t_2, fuzz_int32_t_3);
    
    // Now read the input PNG data to extract information
    fseek(in_file, 0, SEEK_SET);
    
    png_structp read_ptr = png_create_read_struct(
        fuzz_str_4, nullptr, nullptr, nullptr);
    if (!read_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    if (setjmp(png_jmpbuf(read_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_init_io(read_ptr, in_file);
    png_read_info(read_ptr, read_info);
    
    // Use the reading APIs from the requirements
    png_uint_32 io_state = png_get_io_state(read_ptr);
    png_uint_32 user_height_max = png_get_user_height_max(read_ptr);
    
    // For png_get_next_frame_y_offset, we need to handle APNG frames
    // This may return 0 for non-APNG files
    png_uint_32 next_frame_y_offset = 0;
    
    // Check if we have APNG data
    png_uint_32 num_frames = png_get_num_frames(read_ptr, read_info);
    if (num_frames > 0) {
        next_frame_y_offset = png_get_next_frame_y_offset(read_ptr, read_info);
    }
    
    // Clean up read structures
    png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    
    // For writing: Set minimal IHDR info (1x1 pixel grayscale image)
    png_set_IHDR(png_ptr, info_ptr, 
                 1, 1,  // width, height
                 8,     // bit depth
                 PNG_COLOR_TYPE_GRAY,  // color type
                 fuzz_int32_t_5,
                 fuzz_int32_t_6,
                 fuzz_int32_t_7);
    
    // Write info
    png_write_info(png_ptr, info_ptr);
    
    // Write a single pixel
    png_byte pixel = 128;
    png_write_row(png_ptr, &pixel);
    
    // End write
    png_write_end(png_ptr, info_ptr);
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove the output file
    remove("output_file");
    
    // Use the variables to avoid unused variable warnings
    (void)io_state;
    (void)user_height_max;
    (void)next_frame_y_offset;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}