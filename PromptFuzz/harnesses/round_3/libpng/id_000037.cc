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
//<ID> 741
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_acTL","png_get_next_frame_height","png_get_next_frame_fcTL","png_set_quantize","png_set_acTL","png_set_sCAL_s","png_read_frame_head","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_write_struct","png_init_io","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_set_IHDR","png_write_info","png_write_end","png_destroy_write_struct","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_acTL","png_get_next_frame_height","png_get_next_frame_fcTL","png_set_quantize","png_set_acTL","png_set_sCAL_s","png_read_frame_head","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_set_IHDR","png_write_info","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<575) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegralArray(uint16_t, fuzz_uint16_t_array_2, fuzz_uint16_t_array_size_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
	
		const png_uint_16 * fuzzer_var_png_set_quantize_10_4 = fuzz_uint16_t_array_2;
		int fuzzer_size_png_set_quantize_10_4 = static_cast<int>(fuzz_uint16_t_array_size_2);//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimal data for PNG processing

    // Create input FILE from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Initialize read structures
    png_struct *png_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize PNG reading
    png_init_io(png_ptr, in_file);
    png_read_info(png_ptr, info_ptr);

    // Get animation control (acTL) - API 1
    png_uint_32 num_frames = 0, num_plays = 0;
    png_uint_32 has_actl = png_get_acTL(png_ptr, info_ptr, &num_frames, &num_plays);
    
    // If animated PNG, process frame information
    if (has_actl && num_frames > 0) {
        // Get next frame height - API 2
        png_uint_32 next_height = png_get_next_frame_height(png_ptr, info_ptr);
        
        // Get next frame control (fcTL) - API 3
        png_uint_32 width = 0, height = 0, x_offset = 0, y_offset = 0;
        png_uint_16 delay_num = 0, delay_den = 0;
        png_byte dispose_op = 0, blend_op = 0;
        
        png_uint_32 has_fctl = png_get_next_frame_fcTL(png_ptr, info_ptr, &width, &height, 
                                                      &x_offset, &y_offset, &delay_num, 
                                                      &delay_den, &dispose_op, &blend_op);
        
        // Prepare quantization palette - API 4
        png_color palette[256];
        png_uint_16 histogram[256] = {0};
        int num_palette = 256;
        int maximum_colors = 256;
        int full_quantize = 1;
        
        // Initialize palette with gradient
        for (int i = 0; i < 256; i++) {
            palette[i].red = i;
            palette[i].green = i;
            palette[i].blue = i;
            histogram[i] = 1;
        }
        
        png_set_quantize(png_ptr, palette, num_palette, fuzzer_size_png_set_quantize_10_4, fuzzer_var_png_set_quantize_10_4, fuzz_int32_t_3);
        
        // Set animation control - API 5
        png_set_acTL(png_ptr, info_ptr, fuzz_uint32_t_4, fuzz_uint32_t_5);
        
        // Set physical scale - API 6
        png_set_sCAL_s(png_ptr, info_ptr, fuzz_int32_t_8, fuzz_str_6, fuzz_str_7);
        
        // Read frame header
        png_read_frame_head(png_ptr, info_ptr);
    }
	FUZZ_FILENAME_PUSH("output_file")

    // Create output file for writing processed data
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file) {
        // Create write structures
        png_struct *write_ptr = png_create_write_struct(fuzz_str_9, nullptr, nullptr, nullptr);
        if (write_ptr) {
            png_info *write_info = png_create_info_struct(write_ptr);
            if (write_info) {
                if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
                    png_destroy_write_struct(&write_ptr, &write_info);
                } else {
                    png_init_io(write_ptr, out_file);
                    
                    // Copy basic info from read to write
                    png_uint_32 img_width = png_get_image_width(png_ptr, info_ptr);
                    png_uint_32 img_height = png_get_image_height(png_ptr, info_ptr);
                    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
                    int color_type = png_get_color_type(png_ptr, info_ptr);
                    int interlace_type = png_get_interlace_type(png_ptr, info_ptr);
                    
                    png_set_IHDR(write_ptr, write_info, img_width, img_height,
                                bit_depth, color_type, interlace_type,
                                fuzz_int32_t_10, fuzz_int32_t_11);
                    
                    // Write minimal PNG
                    png_write_info(write_ptr, write_info);
                    png_write_end(write_ptr, nullptr);
                    
                    png_destroy_write_struct(&write_ptr, &write_info);
                }
            } else {
                png_destroy_write_struct(&write_ptr, nullptr);
            }
        }
        assert_file_closed(&out_file);;
        remove("output_file");
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}