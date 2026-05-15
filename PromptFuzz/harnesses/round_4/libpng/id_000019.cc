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
//<ID> 532
//<Prompt> []
/*<Combination>: [
*/
//<score> 217, nr_unique_branch: 71
//<Quality> {"density":31,"unique_branches":{"png_get_cHRM_XYZ_fixed":[[608,8,608,23,0,0,4,0],[608,27,608,43,0,0,4,0],[609,8,609,47,0,0,4,0],[610,8,610,51,0,0,4,0],[612,11,612,28,0,0,4,0],[613,11,613,28,0,0,4,0],[614,11,614,28,0,0,4,0],[615,11,615,30,0,0,4,0],[616,11,616,30,0,0,4,0],[617,11,617,30,0,0,4,0],[618,11,618,29,0,0,4,0],[619,11,619,29,0,0,4,0],[620,11,620,29,0,0,4,0]],"png_get_valid":[[22,8,22,23,0,0,4,0],[22,27,22,43,0,0,4,0],[29,11,29,32,0,0,4,1]],"png_XYZ_from_xy":[[1195,8,1195,22,0,0,4,1],[1195,26,1195,44,0,0,4,1],[1196,8,1196,22,0,0,4,1],[1196,26,1196,53,0,0,4,1],[1197,8,1197,22,0,0,4,1],[1197,26,1197,46,0,0,4,1],[1198,8,1198,22,0,0,4,1],[1198,26,1198,57,0,0,4,1],[1199,8,1199,22,0,0,4,1],[1199,26,1199,45,0,0,4,1],[1200,8,1200,22,0,0,4,1],[1200,26,1200,55,0,0,4,1],[1201,8,1201,22,0,0,4,1],[1201,26,1201,46,0,0,4,1],[1202,8,1202,22,0,0,4,1],[1202,26,1202,57,0,0,4,1],[1394,11,1394,80,0,0,4,1],[1396,11,1397,14,0,0,4,1],[1400,11,1400,16,0,0,4,1],[1403,11,1403,80,0,0,4,1],[1405,11,1406,14,0,0,4,1],[1414,11,1415,59,0,0,4,1],[1415,63,1415,68,0,0,4,1],[1416,11,1416,36,0,0,4,1],[1420,11,1420,78,0,0,4,1],[1422,11,1422,79,0,0,4,1],[1424,11,1425,59,0,0,4,1],[1425,63,1425,68,0,0,4,1],[1426,11,1426,38,0,0,4,1],[1435,11,1435,16,0,0,4,1],[1435,20,1435,35,0,0,4,1],[1443,8,1443,69,0,0,4,1],[1445,8,1445,69,0,0,4,1],[1447,8,1448,25,0,0,4,1],[1451,8,1451,75,0,0,4,1],[1453,8,1453,75,0,0,4,1],[1455,8,1456,27,0,0,4,1],[1459,8,1459,70,0,0,4,1],[1461,8,1461,70,0,0,4,1],[1463,8,1464,22,0,0,4,1]],"png_fp_sub":[[1071,8,1071,19,0,0,4,0],[1073,11,1073,43,0,0,4,0]],"png_get_cHRM_fixed":[[636,8,636,23,0,0,4,0],[636,27,636,43,0,0,4,0],[637,8,637,46,0,0,4,0],[639,11,639,25,0,0,4,0],[640,11,640,25,0,0,4,0],[641,11,641,25,0,0,4,0],[642,11,642,25,0,0,4,0],[643,11,643,25,0,0,4,0],[644,11,644,25,0,0,4,0],[645,11,645,25,0,0,4,0],[646,11,646,25,0,0,4,0]],"png_set_crc_action":[[37,7,37,28,0,0,4,1],[52,7,52,30,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_set_crc_action","png_read_info","png_get_valid","png_get_palette_max","png_get_valid","png_get_acTL","png_get_valid","png_get_cHRM_XYZ_fixed","png_get_cHRM_fixed","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_set_sCAL_s","png_set_quantize","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_crc_action","png_read_info","png_get_valid","png_get_palette_max","png_get_valid","png_get_acTL","png_get_valid","png_get_cHRM_XYZ_fixed","png_get_cHRM_fixed","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sCAL_s","png_set_quantize","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/**/


// Include libpng headers

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<567) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegralArray(uint16_t, fuzz_uint16_t_array_9, fuzz_uint16_t_array_size_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
	
		const png_uint_16 * fuzzer_var_png_set_quantize_30_4 = fuzz_uint16_t_array_9;
		int fuzzer_size_png_set_quantize_30_4 = static_cast<int>(fuzz_uint16_t_array_size_9);//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful PNG operations

    // Initialize PNG structures
    png_struct *png_ptr = NULL;
    png_info *info_ptr = NULL;
    png_info *end_info = NULL;
    
    // Create read structure
    png_ptr = png_create_read_struct(fuzz_str_1, NULL, NULL, NULL);
    if (!png_ptr) {
	return 0;
	}
	
    
    // Create info structure
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    // Create end info structure
    end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return 0;
    }
    
    // Create FILE* from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O
    png_init_io(png_ptr, in_file);
    
    // Set CRC action (API #8)
    png_set_crc_action(png_ptr, fuzz_int32_t_2, fuzz_int32_t_3);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Get palette max if available (API #1)
    int palette_max = -1;
    if (png_get_valid(png_ptr, info_ptr, fuzz_uint32_t_4)) {
        palette_max = png_get_palette_max(png_ptr, info_ptr);
    }
    
    // Get animation control if available (API #2)
    png_uint_32 num_frames = 0, num_plays = 0;
    if (png_get_valid(png_ptr, info_ptr, fuzz_uint32_t_5)) {
        png_get_acTL(png_ptr, info_ptr, &num_frames, &num_plays);
    }
    
    // Get chromaticity data (APIs #4 and #6)
    png_fixed_point int_red_X, int_red_Y, int_red_Z;
    png_fixed_point int_green_X, int_green_Y, int_green_Z;
    png_fixed_point int_blue_X, int_blue_Y, int_blue_Z;
    
    png_fixed_point int_white_x, int_white_y;
    png_fixed_point int_red_x, int_red_y;
    png_fixed_point int_green_x, int_green_y;
    png_fixed_point int_blue_x, int_blue_y;
    
    if (png_get_valid(png_ptr, info_ptr, fuzz_uint32_t_6)) {
        // Get XYZ chromaticity
        png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
                               &int_red_X, &int_red_Y, &int_red_Z,
                               &int_green_X, &int_green_Y, &int_green_Z,
                               &int_blue_X, &int_blue_Y, &int_blue_Z);
        
        // Get CIE 1931 chromaticity
        png_get_cHRM_fixed(png_ptr, info_ptr,
                           &int_white_x, &int_white_y,
                           &int_red_x, &int_red_y,
                           &int_green_x, &int_green_y,
                           &int_blue_x, &int_blue_y);
    }
    
    // Get uint16 from data (API #5)
    png_uint_16 sample_value = 0;
    if (size >= 2) {
        sample_value = png_get_uint_16(data);
    }
    
    // Setup for writing (create write structures)
    png_struct *write_ptr = png_create_write_struct(fuzz_str_7, NULL, NULL, NULL);
    if (!write_ptr) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_info *write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, NULL);
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);
	return 0;
    }
    
    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_init_io(write_ptr, out_file);
    
    // Set sCAL data (API #7)
    // Using sample values from input data
    char swidth[20], sheight[20];
    snprintf(swidth, sizeof(swidth), "%u.%u", sample_value / 100, sample_value % 100);
    snprintf(sheight, sizeof(sheight), "%u.%u", (sample_value + 50) / 100, (sample_value + 50) % 100);
    png_set_sCAL_s(write_ptr, write_info, fuzz_int32_t_8, swidth, sheight);
    
    // Set quantization if we have palette data (API #3)
    if (palette_max > 0 && palette_max <= 256) {
        png_color palette[256];
        png_uint_16 histogram[256] = {0};
        
        // Build a simple grayscale palette
        for (int i = 0; i < palette_max; i++) {
            palette[i].red = i;
            palette[i].green = i;
            palette[i].blue = i;
            histogram[i] = (png_uint_16)(i * 100);
        }
        
        png_set_quantize(write_ptr, palette, palette_max, 
                        fuzzer_size_png_set_quantize_30_4, fuzzer_var_png_set_quantize_30_4, fuzz_int32_t_10);
    }
    
    // Clean up
    png_destroy_write_struct(&write_ptr, &write_info);
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}