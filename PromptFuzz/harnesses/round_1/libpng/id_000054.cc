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
//<ID> 1213
//<Prompt> []
/*<Combination>: [
*/
//<score> 62, nr_unique_branch: 12
//<Quality> {"density":31,"unique_branches":{"png_set_filter":[[1072,8,1072,23,0,0,4,1],[1076,8,1076,71,0,0,4,1],[1081,8,1081,38,0,0,4,0],[1086,10,1086,16,0,0,4,0],[1087,10,1087,16,0,0,4,1],[1088,10,1088,16,0,0,4,1],[1091,10,1091,36,0,0,4,1],[1095,10,1095,35,0,0,4,1],[1098,10,1098,34,0,0,4,1],[1101,10,1101,35,0,0,4,1],[1104,10,1104,37,0,0,4,1],[1107,10,1107,17,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_IHDR","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_set_IHDR","png_set_filter","png_set_sPLT","png_set_iCCP","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_free","png_get_acTL","png_get_next_frame_y_offset","png_get_current_pass_number","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_IHDR","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_set_filter","png_set_sPLT","png_set_iCCP","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_free","png_get_acTL","png_get_next_frame_y_offset","png_get_current_pass_number","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<65) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeCharArray(unsigned char, fuzz_char_array_6, fuzz_char_array_size_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	
		const png_byte * fuzzer_var_png_set_iCCP_20_4 = fuzz_char_array_6;
		png_uint_32 fuzzer_size_png_set_iCCP_20_4 = static_cast<png_uint_32>(fuzz_char_array_size_6);//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need enough data for PNG operations
    
    // Create input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	FUZZ_FILENAME_PUSH("output_file")
	
    
    // Create output file
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
    
    // Initialize libpng structures
    png_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up reading
    png_init_io(png_ptr, in_file);
    png_read_info(png_ptr, info_ptr);
    
    // Get image information
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);
    
    // Create write structure
    png_structp png_write_ptr = png_create_write_struct(fuzz_str_2, nullptr, nullptr, nullptr);
    if (!png_write_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop write_info_ptr = png_create_info_struct(png_write_ptr);
    if (!write_info_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    if (setjmp(png_jmpbuf(png_write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_init_io(png_write_ptr, out_file);
    
    // Set IHDR for output
    png_set_IHDR(png_write_ptr, write_info_ptr, width, height, bit_depth, color_type,
                 interlace_method, compression_method, filter_method);
    
    // 1. Set filter method using input data
    int filter_value = data[0] % 6; // 0-5 for different filter methods
    png_set_filter(png_write_ptr, fuzz_int32_t_3, filter_value);
    
    // 2. Create dummy sPLT entries (simplified)
    png_sPLT_t splt_entry;
    splt_entry.name = (char*)"dummy";
    splt_entry.depth = 8;
    splt_entry.nentries = 0;
    splt_entry.entries = nullptr;
    
    png_set_sPLT(png_write_ptr, write_info_ptr, &splt_entry, fuzz_int32_t_4);
    
    // 3. Create dummy ICC profile
    png_byte icc_profile[] = {0,1,2,3,4,5,6,7,8,9};
    png_set_iCCP(png_write_ptr, write_info_ptr, fuzz_str_5, 
                 fuzz_int32_t_7, fuzzer_var_png_set_iCCP_20_4, fuzzer_size_png_set_iCCP_20_4);
    
    // 4. Write image data
    png_write_info(png_write_ptr, write_info_ptr);
    
    // Allocate row buffer
    png_bytep row = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
    
    // Read and write rows
    for (png_uint_32 y = 0; y < height; y++) {
        png_read_row(png_ptr, row, nullptr);
        png_write_row(png_write_ptr, row);
    }
    
    png_free(png_ptr, row);
    
    // 5. Try to get next frame y offset (for APNG)
    if (png_get_acTL(png_ptr, info_ptr, nullptr, nullptr)) {
        png_uint_32 y_offset = png_get_next_frame_y_offset(png_ptr, info_ptr);
        (void)y_offset; // Use variable to avoid unused warning
    }
    
    // 6. Get current pass number for interlaced images
    if (interlace_method == PNG_INTERLACE_ADAM7) {
        png_byte pass_num = png_get_current_pass_number(png_ptr);
        (void)pass_num; // Use variable to avoid unused warning
    }
    
    // Complete writing
    png_write_end(png_write_ptr, write_info_ptr);
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}