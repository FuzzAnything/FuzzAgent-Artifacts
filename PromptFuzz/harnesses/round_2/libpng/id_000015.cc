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
//<ID> 263
//<Prompt> []
/*<Combination>: [
*/
//<score> 31.5, nr_unique_branch: 2
//<Quality> {"density":21,"unique_branches":{"png_set_gamma_fixed":[[901,8,901,57,0,0,4,0]],"png_rtran_ok":[[102,11,102,52,0,0,4,0]]},"library_calls":["png_image_begin_read_from_file","png_image_free","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_start_read_image","png_set_gamma_fixed","png_get_cLLI","png_get_hIST","png_get_x_offset_inches_fixed","png_get_tRNS","png_get_eXIf_1","png_get_cHRM_XYZ","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_rowbytes","png_read_image","png_read_end","png_destroy_read_struct"],"critical_calls":["png_image_begin_read_from_file","png_image_free","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_start_read_image","png_set_gamma_fixed","png_get_cLLI","png_get_hIST","png_get_x_offset_inches_fixed","png_get_tRNS","png_get_eXIf_1","png_get_cHRM_XYZ","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_rowbytes","png_read_image","png_read_end","png_destroy_read_struct"],"visited":1}
/*To create a fuzz driver for libpng using the specified APIs, we'll need to read a PNG file from memory, process it with libpng, extract various metadata, and clean up resources. Here's the complete implementation:

*/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
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

	FUZZ_FILENAME_PUSH("input_file")




    // Step 1: Create a temporary file with the input data
    const char* temp_filename = "input_file";
    FILE* temp_file = fopen(temp_filename, "wb");
	FUZZ_FILE_PUSH(temp_file)
    if (!temp_file) {
        assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    fwrite(data, 1, size, temp_file);
    assert_file_closed(&temp_file);;
    
    // Initialize variables
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_image image;
    int ret = 0;
    
    // Step 2: Use png_image_begin_read_from_file with the temporary file
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    if (png_image_begin_read_from_file(&image, temp_filename) != 0) {
        // For this fuzzing driver, we'll just clean up the image structure
        png_image_free(&image);
    }
    
    // Step 3: Set up traditional libpng reading structure
    FILE* in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        // Fallback: reopen the temporary file
        in_file = fopen(temp_filename, "rb");
        if (!in_file) {
            remove(temp_filename);
            assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	return 0;
        }
    }
    
    // Step 4: Create read structures
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        remove(temp_filename);
        assert_file_closed(&in_file);
	assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        remove(temp_filename);
        assert_file_closed(&in_file);
	assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        remove(temp_filename);
        assert_file_closed(&in_file);
	assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Step 5: Initialize I/O and read PNG info
    png_init_io(png_ptr, in_file);
    png_read_info(png_ptr, info_ptr);
    
    // Step 6: Call png_start_read_image
    png_start_read_image(png_ptr);
    
    // Step 7: Apply gamma correction using png_set_gamma_fixed
    // Using some fixed values for fuzzing
    png_set_gamma_fixed(png_ptr, 45000, 45000);
    
    // Step 8: Try to extract various PNG metadata
    // 8.1 Get CLLI (Content Light Level Information)
    double max_content_light_level = 0.0;
    double max_frame_avg_light_level = 0.0;
    png_uint_32 clli_result = png_get_cLLI(png_ptr, info_ptr, 
                                          &max_content_light_level, 
                                          &max_frame_avg_light_level);
    
    // 8.2 Get histogram (hIST)
    png_uint_16* hist = NULL;
    png_uint_32 hist_result = png_get_hIST(png_ptr, info_ptr, &hist);
    
    // 8.3 Get x offset in inches (fixed point)
    png_fixed_point x_offset_inches = 0;
    png_fixed_point x_offset_result = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    
    // 8.4 Get transparency info (tRNS)
    png_byte* trans_alpha = NULL;
    int num_trans = 0;
    png_color_16* trans_color = NULL;
    png_uint_32 trans_result = png_get_tRNS(png_ptr, info_ptr, 
                                           &trans_alpha, &num_trans, &trans_color);
    
    // 8.5 Get EXIF data
    png_uint_32 num_exif = 0;
    png_byte* exif_data = NULL;
    png_uint_32 exif_result = png_get_eXIf_1(png_ptr, info_ptr, 
                                            &num_exif, &exif_data);
    
    // 8.6 Get chromaticity XYZ values
    double red_X = 0.0, red_Y = 0.0, red_Z = 0.0;
    double green_X = 0.0, green_Y = 0.0, green_Z = 0.0;
    double blue_X = 0.0, blue_Y = 0.0, blue_Z = 0.0;
    png_uint_32 chroma_result = png_get_cHRM_XYZ(png_ptr, info_ptr,
                                                &red_X, &red_Y, &red_Z,
                                                &green_X, &green_Y, &green_Z,
                                                &blue_X, &blue_Y, &blue_Z);
    
    // Step 9: Read the actual image data
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Allocate row pointers
    png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    if (row_pointers) {
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
        }
        
        // Read the image
        png_read_image(png_ptr, row_pointers);
        
        // Clean up row pointers
        for (png_uint_32 y = 0; y < height; y++) {
            free(row_pointers[y]);
        }
        free(row_pointers);
    }
    
    // Step 10: Read end of PNG
    png_read_end(png_ptr, NULL);
    
    // Step 11: Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    assert_file_closed(&in_file);;
    
    // Remove temporary file
    remove(temp_filename);
    
    assert_file_closed(&in_file);
	assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	return 0;
}