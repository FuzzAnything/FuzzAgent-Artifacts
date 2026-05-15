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
//<ID> 699
//<Prompt> []
/*<Combination>: [
*/
//<score> 141, nr_unique_branch: 4
//<Quality> {"density":47,"unique_branches":{"png_get_next_frame_blend_op":[[1503,8,1503,23,0,0,4,0],[1503,27,1503,43,0,0,4,0]],"png_get_next_frame_delay_num":[[1473,8,1473,23,0,0,4,0],[1473,27,1473,43,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_read_info","png_get_y_offset_inches","png_get_next_frame_delay_num","png_get_next_frame_blend_op","png_set_alpha_mode_fixed","png_process_data","png_process_data_pause","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_palette_to_rgb","png_get_valid","png_set_tRNS_to_alpha","png_set_strip_16","png_set_gray_to_rgb","png_read_update_info","png_malloc","png_get_rowbytes","png_malloc","png_read_image","png_convert_from_struct_tm","png_set_tIME","png_init_io","png_set_IHDR","png_write_info","png_write_rows","png_write_end","png_free","png_free","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_read_info","png_get_y_offset_inches","png_get_next_frame_delay_num","png_get_next_frame_blend_op","png_set_alpha_mode_fixed","png_process_data","png_process_data_pause","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_palette_to_rgb","png_get_valid","png_set_tRNS_to_alpha","png_set_strip_16","png_set_gray_to_rgb","png_read_update_info","png_malloc","png_get_rowbytes","png_malloc","png_read_image","png_convert_from_struct_tm","png_set_tIME","png_init_io","png_set_IHDR","png_write_info","png_write_rows","png_write_end","png_free","png_free","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/*Here's a C++ fuzzer that uses the specified libpng APIs to achieve reading and writing PNG images with animation support:

*/


extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for PNG processing
    
    png_struct *read_ptr = nullptr;
    png_struct *write_ptr = nullptr;
    png_info *read_info = nullptr;
    png_info *write_info = nullptr;
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    
    // Create read structure
    read_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (!read_ptr) {
	return 0;
	}
	
    
    read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Create write structure
    write_ptr = png_create_write_struct(fuzz_str_2, nullptr, nullptr, nullptr);
    if (!write_ptr) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        return 0;
    }
    
    write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(read_ptr)) || setjmp(png_jmpbuf(write_ptr))) {
        if (out_file) assert_file_closed(&out_file);;
        if (in_file) assert_file_closed(&in_file);;
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        return 0;
    }
    
    // Open input file from memory buffer
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open output file
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize read I/O
    png_init_io(read_ptr, in_file);
    
    // Read PNG info
    png_read_info(read_ptr, read_info);
    
    // Get vertical offset in inches
    float y_offset_inches = png_get_y_offset_inches(read_ptr, read_info);
    
    // Get next frame delay numerator (for APNG)
    png_uint_16 delay_num = png_get_next_frame_delay_num(read_ptr, read_info);
    
    // Get next frame blend operation (for APNG)
    png_byte blend_op = png_get_next_frame_blend_op(read_ptr, read_info);
    
    // Set alpha mode with fixed gamma (PNG_ALPHA_STANDARD = 1)
    png_set_alpha_mode_fixed(write_ptr, fuzz_int32_t_3, png_fixed_point(10000));
    
    // Process some data and pause
    png_byte buffer[4096];
    size_t processed = 0;
    int save_state = 1;
    
    // Process data in chunks
    while (processed < size && save_state) {
        size_t chunk_size = (size - processed > 4096) ? 4096 : size - processed;
        png_process_data(read_ptr, read_info, buffer, chunk_size);
        
        // Pause processing to simulate incremental loading
        processed += chunk_size;
        png_process_data_pause(read_ptr, fuzz_int32_t_4);
        
        // After first chunk, continue but don't save state
        save_state = 0;
    }
    
    // Get image dimensions
    png_uint_32 width = png_get_image_width(read_ptr, read_info);
    png_uint_32 height = png_get_image_height(read_ptr, read_info);
    png_byte color_type = png_get_color_type(read_ptr, read_info);
    png_byte bit_depth = png_get_bit_depth(read_ptr, read_info);
    
    // Set transformations for reading
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(read_ptr);
    if (png_get_valid(read_ptr, read_info, fuzz_uint32_t_5))
        png_set_tRNS_to_alpha(read_ptr);
    if (bit_depth == 16)
        png_set_strip_16(read_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(read_ptr);
    
    // Update info after transformations
    png_read_update_info(read_ptr, read_info);
    
    // Allocate memory for row pointers
    png_bytep *row_pointers = (png_bytep*)png_malloc(read_ptr,
        height * sizeof(png_bytep));
    
    // Allocate memory for image data
    png_uint_32 rowbytes = png_get_rowbytes(read_ptr, read_info);
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)png_malloc(read_ptr, rowbytes);
    }
    
    // Read image data
    png_read_image(read_ptr, row_pointers);
    
    // Convert struct tm to png_time
    struct tm timeinfo;
    time_t rawtime = time(nullptr);
    localtime_r(&rawtime, &timeinfo);
    png_time ptime;
    png_convert_from_struct_tm(&ptime, &timeinfo);
    
    // Set time in write info
    png_set_tIME(write_ptr, write_info, &ptime);
    
    // Initialize write I/O
    png_init_io(write_ptr, out_file);
    
    // Set write info with same parameters as read
    png_set_IHDR(write_ptr, write_info, width, height,
                 bit_depth, color_type, fuzz_int32_t_6,
                 fuzz_int32_t_7, fuzz_int32_t_8);
    
    // Write info
    png_write_info(write_ptr, write_info);
    
    // Write rows
    png_write_rows(write_ptr, row_pointers, height);
    
    // End write
    png_write_end(write_ptr, write_info);
    
    // Cleanup
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(read_ptr, row_pointers[y]);
    }
    png_free(read_ptr, row_pointers);
    
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    png_destroy_write_struct(&write_ptr, &write_info);
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}