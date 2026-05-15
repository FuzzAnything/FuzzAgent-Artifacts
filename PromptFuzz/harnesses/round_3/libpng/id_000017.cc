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
//<ID> 149
//<Prompt> []
/*<Combination>: [
*/
//<score> 100, nr_unique_branch: 14
//<Quality> {"density":40,"unique_branches":{"png_init_read_transformations":[[2000,8,2000,51,0,0,4,0],[2001,8,2001,52,0,0,4,0],[2002,8,2002,55,0,0,4,1]],"png_read_frame_head":[[201,8,201,40,0,0,4,0]],"png_set_packing":[[56,8,56,30,0,0,4,1]],"png_set_shift":[[95,11,95,60,0,0,4,0],[97,14,97,33,0,0,4,1],[97,37,97,63,0,0,4,1],[98,14,98,35,0,0,4,0],[104,38,104,65,0,0,4,1],[108,11,108,60,0,0,4,0],[109,12,109,33,0,0,4,1],[109,37,109,65,0,0,4,1],[112,11,112,18,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_read_info","png_set_packing","png_set_shift","png_read_update_info","png_read_frame_head","png_get_eXIf_1","png_init_io","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_set_IHDR","png_write_frame_head","png_write_info","png_set_packing","png_set_shift","png_malloc","png_malloc","png_get_rowbytes","png_read_image","png_write_image","png_write_end","png_free","png_free","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_read_info","png_set_packing","png_set_shift","png_read_update_info","png_read_frame_head","png_get_eXIf_1","png_init_io","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_set_IHDR","png_write_frame_head","png_write_info","png_set_packing","png_set_shift","png_malloc","png_malloc","png_get_rowbytes","png_read_image","png_write_image","png_write_end","png_free","png_free","png_destroy_read_struct","png_destroy_write_struct"],"visited":1}
/*Looking at these APIs, I can see they're related to reading/writing PNG files with specific transformations. Let me create a fuzz driver that uses these APIs together to process a PNG file with potential EXIF data, color shifting, and packing.

*/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for PNG header
    
    // Create input file from memory buffer
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
    
    // Initialize structures
    png_struct *png_ptr_read = png_create_read_struct(fuzz_str_1, 
                                                     nullptr, nullptr, nullptr);
    if (!png_ptr_read) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_info *info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_struct *png_ptr_write = png_create_write_struct(fuzz_str_2,
                                                       nullptr, nullptr, nullptr);
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_info *info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr_read)) || setjmp(png_jmpbuf(png_ptr_write))) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up input reading
    png_init_io(png_ptr_read, in_file);
    
    // Read PNG info
    png_read_info(png_ptr_read, info_ptr_read);
    
    // Apply transformations
    png_set_packing(png_ptr_read);  // Pack pixels to bytes
    
    // Set color shift if needed (using sample data from input)
    png_color_8 shift_bits;
    if (size >= sizeof(shift_bits) + 8) {
        // Use some bytes from input to set shift values
        shift_bits.red = data[0] % 8;
        shift_bits.green = data[1] % 8;
        shift_bits.blue = data[2] % 8;
        shift_bits.gray = data[3] % 8;
        shift_bits.alpha = data[4] % 8;
        png_set_shift(png_ptr_read, &shift_bits);
    }
    
    // Update info after transformations
    png_read_update_info(png_ptr_read, info_ptr_read);
    
    // Try to read frame head (for APNG files)
    png_read_frame_head(png_ptr_read, info_ptr_read);
    
    // Get EXIF data if present
    png_uint_32 num_exif = 0;
    png_byte *exif_data = nullptr;
    png_uint_32 exif_result = png_get_eXIf_1(png_ptr_read, info_ptr_read, &num_exif, &exif_data);
    
    // Set up output writing
    png_init_io(png_ptr_write, out_file);
    
    // Copy info from read to write
    png_uint_32 width = png_get_image_width(png_ptr_read, info_ptr_read);
    png_uint_32 height = png_get_image_height(png_ptr_read, info_ptr_read);
    int bit_depth = png_get_bit_depth(png_ptr_read, info_ptr_read);
    int color_type = png_get_color_type(png_ptr_read, info_ptr_read);
    int interlace_type = png_get_interlace_type(png_ptr_read, info_ptr_read);
    int compression_type = png_get_compression_type(png_ptr_read, info_ptr_read);
    int filter_type = png_get_filter_type(png_ptr_read, info_ptr_read);
    
    // Set output image info
    png_set_IHDR(png_ptr_write, info_ptr_write, width, height, bit_depth,
                 color_type, interlace_type, compression_type, filter_type);
    
    // Write frame head if we have animation data
    if (size > 100) {
        png_byte **row_pointers = nullptr;
        png_uint_32 x_offset = (data[5] % width);
        png_uint_32 y_offset = (data[6] % height);
        png_uint_16 delay_num = (data[7] << 8) | data[8];
        png_uint_16 delay_den = (data[9] << 8) | data[10];
        png_byte dispose_op = data[11] % 4;
        png_byte blend_op = data[12] % 2;
        
        png_write_frame_head(png_ptr_write, info_ptr_write, row_pointers,
                            width, height, x_offset, y_offset,
                            delay_num, delay_den, dispose_op, blend_op);
    }
    
    // Write info
    png_write_info(png_ptr_write, info_ptr_write);
    
    // Apply same transformations for writing
    png_set_packing(png_ptr_write);
    if (size >= sizeof(shift_bits) + 8) {
        png_set_shift(png_ptr_write, &shift_bits);
    }
    
    // Read image data
    png_bytep *row_pointers = (png_bytep*)png_malloc(png_ptr_read, 
                                                     height * sizeof(png_bytep));
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)png_malloc(png_ptr_read, 
                                               png_get_rowbytes(png_ptr_read, info_ptr_read));
    }
    
    png_read_image(png_ptr_read, row_pointers);
    
    // Write image data
    png_write_image(png_ptr_write, row_pointers);
    
    // Write end
    png_write_end(png_ptr_write, info_ptr_write);
    
    // Free row pointers
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr_read, row_pointers[y]);
    }
    png_free(png_ptr_read, row_pointers);
    
    // Clean up
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}