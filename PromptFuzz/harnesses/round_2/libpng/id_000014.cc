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
//<ID> 261
//<Prompt> []
/*<Combination>: [
*/
//<score> 272, nr_unique_branch: 94
//<Quality> {"density":16,"unique_branches":{"png_muldiv":[[2748,11,2748,17,0,0,4,0]],"png_get_hIST":[[916,8,916,23,0,0,4,0],[916,27,916,43,0,0,4,0],[917,8,917,46,0,0,4,1]],"png_resolve_file_gamma":[[1346,8,1346,23,0,0,4,0]],"png_XYZ_from_xy":[[1195,8,1195,22,0,0,4,1],[1195,26,1195,44,0,0,4,1],[1196,8,1196,22,0,0,4,1],[1196,26,1196,53,0,0,4,1],[1197,8,1197,22,0,0,4,1],[1197,26,1197,46,0,0,4,1],[1198,8,1198,22,0,0,4,1],[1198,26,1198,57,0,0,4,1],[1199,8,1199,22,0,0,4,1],[1199,26,1199,45,0,0,4,1],[1200,8,1200,22,0,0,4,1],[1200,26,1200,55,0,0,4,1],[1201,8,1201,22,0,0,4,1],[1201,26,1201,46,0,0,4,1],[1202,8,1202,22,0,0,4,1],[1202,26,1202,57,0,0,4,1],[1394,11,1394,80,0,0,4,1],[1396,11,1397,14,0,0,4,1],[1400,11,1400,16,0,0,4,1],[1403,11,1403,80,0,0,4,1],[1405,11,1406,14,0,0,4,1],[1414,11,1415,59,0,0,4,1],[1415,63,1415,68,0,0,4,1],[1416,11,1416,36,0,0,4,1],[1420,11,1420,78,0,0,4,1],[1422,11,1422,79,0,0,4,1],[1424,11,1425,59,0,0,4,1],[1425,63,1425,68,0,0,4,1],[1426,11,1426,38,0,0,4,1],[1435,11,1435,16,0,0,4,1],[1435,20,1435,35,0,0,4,1],[1443,8,1443,69,0,0,4,1],[1445,8,1445,69,0,0,4,1],[1447,8,1448,25,0,0,4,1],[1451,8,1451,75,0,0,4,1],[1453,8,1453,75,0,0,4,1],[1455,8,1456,27,0,0,4,1],[1459,8,1459,70,0,0,4,1],[1461,8,1461,70,0,0,4,1],[1463,8,1464,22,0,0,4,1]],"png_image_is_not_sRGB":[[104,4,104,61,6,0,4,0]],"png_fixed_inches_from_microns":[[385,8,385,51,0,0,4,0]],"png_set_gamma_fixed":[[878,8,878,37,0,0,4,1],[896,8,896,23,0,0,4,1],[898,8,898,23,0,0,4,1],[901,8,901,57,0,0,4,1],[902,8,902,57,0,0,4,1]],"png_read_start_row":[[4643,8,4643,52,0,0,4,1],[534,6,534,23,22,0,4,1]],"png_get_x_offset_microns":[[249,8,249,23,0,0,4,0],[249,27,249,43,0,0,4,0],[250,8,250,46,0,0,4,0],[250,8,250,46,0,0,4,1],[252,11,252,62,0,0,4,0]],"png_get_cHRM_XYZ":[[567,8,567,23,0,0,4,0],[567,27,567,43,0,0,4,0],[568,8,568,46,0,0,4,0],[568,8,568,46,0,0,4,1],[569,8,569,51,0,0,4,0],[571,11,571,24,0,0,4,0],[573,11,573,24,0,0,4,0],[575,11,575,24,0,0,4,0],[577,11,577,26,0,0,4,0],[579,11,579,26,0,0,4,0],[581,11,581,26,0,0,4,0],[583,11,583,25,0,0,4,0],[585,11,585,25,0,0,4,0],[587,11,587,25,0,0,4,0]],"png_get_eXIf_1":[[897,8,897,23,0,0,4,0],[897,27,897,43,0,0,4,0],[898,8,898,46,0,0,4,1]],"png_image_free_function":[[3943,11,3943,30,0,0,4,0],[3949,14,3949,24,0,0,4,0]],"png_fp_sub":[[1071,8,1071,19,0,0,4,0],[1073,11,1073,43,0,0,4,0]],"png_get_cLLI":[[813,8,813,23,0,0,4,0],[813,27,813,43,0,0,4,0],[814,8,814,46,0,0,4,0],[814,8,814,46,0,0,4,1],[816,11,816,25,0,0,4,0],[817,11,817,26,0,0,4,0]],"png_start_read_image":[[295,8,295,23,0,0,4,0],[297,11,297,52,0,0,4,0]],"png_image_begin_read_from_file":[[1456,8,1456,21,0,0,4,0],[1456,25,1456,60,0,0,4,0],[1458,11,1458,28,0,0,4,0],[1462,14,1462,24,0,0,4,0],[1462,14,1462,24,0,0,4,1],[1464,17,1464,48,0,0,4,0]]},"library_calls":["png_image_begin_read_from_file","png_image_free","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_set_gamma_fixed","png_read_info","png_start_read_image","png_get_cLLI","png_get_hIST","png_get_x_offset_inches_fixed","png_get_tRNS","png_get_eXIf_1","png_get_cHRM_XYZ","png_destroy_read_struct"],"critical_calls":["png_image_begin_read_from_file","png_image_free","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_gamma_fixed","png_read_info","png_start_read_image","png_get_cLLI","png_get_hIST","png_get_x_offset_inches_fixed","png_get_tRNS","png_get_eXIf_1","png_get_cHRM_XYZ","png_destroy_read_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to read and process a PNG image:

*/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 8) {
	return 0;
	}
	FUZZ_FILENAME_PUSH("input_file")
	  // Too small to be a valid PNG
    
    // Write input data to a temporary file
    FILE *temp_file = fopen("input_file", "wb");
	FUZZ_FILE_PUSH(temp_file)
    if (!temp_file) {
	assert_file_closed(&temp_file);
	return 0;
	}
	
    fwrite(data, 1, size, temp_file);
    assert_file_closed(&temp_file);;
    
    // Initialize png_image for png_image_begin_read_from_file
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    // Step 1: Begin reading PNG using png_image API
    if (!png_image_begin_read_from_file(&image, "input_file")) {
        remove("input_file");
        assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Clean up png_image
    png_image_free(&image);
    
    // Now use traditional libpng API for the rest of the functions
    
    // Create read structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                 nullptr, nullptr);
    if (!png_ptr) {
        remove("input_file");
        assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        remove("input_file");
        assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Setup error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        remove("input_file");
        assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
	FUZZ_FILENAME_PUSH("input_file")
    
    // Open file for reading with traditional API
    FILE *in_file = fopen("input_file", "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Initialize I/O
    png_init_io(png_ptr, in_file);
    
    // Step 2: Apply gamma correction
    png_set_gamma_fixed(png_ptr, PNG_FP_1, PNG_FP_1);  // Use 1.0 gamma for both
    
    // Read PNG header
    png_read_info(png_ptr, info_ptr);
    
    // Step 3: Start reading image
    png_start_read_image(png_ptr);
    
    // Step 4: Call the various getter functions
    
    // Get cLLI (Content Light Level Information)
    double max_content_light = 0, max_frame_average = 0;
    png_uint_32 clli_result = png_get_cLLI(png_ptr, info_ptr, 
                                          &max_content_light, &max_frame_average);
    
    // Get hIST (Histogram)
    png_uint_16 *hist = nullptr;
    png_uint_32 hist_result = png_get_hIST(png_ptr, info_ptr, &hist);
    
    // Get x offset in inches (fixed point)
    png_fixed_point x_offset_inches_fixed = 0;
    png_fixed_point x_offset_result = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    
    // Get tRNS (Transparency)
    png_byte *trans_alpha = nullptr;
    int num_trans = 0;
    png_color_16 *trans_color = nullptr;
    png_uint_32 trns_result = png_get_tRNS(png_ptr, info_ptr, 
                                          &trans_alpha, &num_trans, &trans_color);
    
    // Get eXIf (EXIF data)
    png_uint_32 num_exif = 0;
    png_byte *exif = nullptr;
    png_uint_32 exif_result = png_get_eXIf_1(png_ptr, info_ptr, &num_exif, &exif);
    
    // Get cHRM XYZ (Chromaticity XYZ)
    double red_X = 0, red_Y = 0, red_Z = 0;
    double green_X = 0, green_Y = 0, green_Z = 0;
    double blue_X = 0, blue_Y = 0, blue_Z = 0;
    png_uint_32 chrm_result = png_get_cHRM_XYZ(png_ptr, info_ptr,
                                              &red_X, &red_Y, &red_Z,
                                              &green_X, &green_Y, &green_Z,
                                              &blue_X, &blue_Y, &blue_Z);
    
    // Clean up
    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    // Clean up temporary file
    remove("input_file");
	FUZZ_FILENAME_PUSH("output_file")
    
    // Also clean up output file if it exists
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&temp_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}