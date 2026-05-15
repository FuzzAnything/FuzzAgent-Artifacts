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
//<ID> 824
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"png_malloc_default":[[196,8,196,23,0,0,4,0]]},"library_calls":["png_image_begin_read_from_stdio","png_malloc_default","png_image_free","png_create_read_struct_2","png_create_info_struct","png_set_longjmp_fn","png_destroy_read_struct","png_get_compression_type","png_set_text_compression_mem_level","png_set_option","png_set_sCAL_s","png_image_write_to_stdio","png_free_default","png_destroy_read_struct","png_image_free"],"critical_calls":["png_image_begin_read_from_stdio","png_malloc_default","png_create_read_struct_2","png_create_info_struct","png_set_longjmp_fn","png_get_compression_type","png_set_text_compression_mem_level","png_set_option","png_set_sCAL_s","png_image_write_to_stdio","png_free_default","png_destroy_read_struct","png_image_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<32) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    if (size < 50) {
	return 0;
	}
	 // Need minimum data for PNG operations
    
    // Create input FILE from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	FUZZ_FILENAME_PUSH("output_file")
	
    
    // Create output FILE
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize png_image structure
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    // Begin reading image from stdio
    int read_result = png_image_begin_read_from_stdio(&image, in_file);
    if (!read_result) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        remove("output_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }
    
    // Allocate buffer for image data
    png_alloc_size_t buffer_size = PNG_IMAGE_SIZE(image);
    png_byte *image_buffer = (png_byte*)png_malloc_default(NULL, buffer_size);
    if (!image_buffer) {
        png_image_free(&image);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        remove("output_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }
    
    // Set up row stride
    png_int_32 row_stride = PNG_IMAGE_ROW_STRIDE(image);
    
    // Parse some data using png_get_int_32 and png_get_uint_16
    png_int_32 parsed_int32 = 0;
    png_uint_16 parsed_uint16 = 0;
    
    if (size >= 4) {
        parsed_int32 = png_get_int_32(data);
    }
    if (size >= 2) {
        parsed_uint16 = png_get_uint_16(data);
    }
    
    // Create png_struct and png_info for compression type checking
    png_structp png_ptr = png_create_read_struct_2(fuzz_str_1, 
                                                   NULL, NULL, NULL, 
                                                   NULL, NULL, NULL);
    png_infop info_ptr = NULL;
    
    if (png_ptr) {
        info_ptr = png_create_info_struct(png_ptr);
    }
    
    // Get compression type if structures were created successfully
    png_byte compression_type = 0;
    if (png_ptr && info_ptr) {
        // Set up error handling
        if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
            // Error handling - clean up and continue
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        } else {
            // Get compression type
            compression_type = png_get_compression_type(png_ptr, info_ptr);
            
            // Set text compression memory level
            png_set_text_compression_mem_level(png_ptr, fuzz_int32_t_2);
            
            // Set an option
            png_set_option(png_ptr, fuzz_int32_t_3, fuzz_int32_t_4);
            
            // Set sCAL chunk - use parsed values for width/height
            char swidth[32], sheight[32];
            snprintf(swidth, sizeof(swidth), "%d", (int)(parsed_int32 % 1000 + 1));
            snprintf(sheight, sizeof(sheight), "%d", (int)(parsed_uint16 % 1000 + 1));
            png_set_sCAL_s(png_ptr, info_ptr, fuzz_int32_t_5, swidth, sheight);
        }
    }
    
    // Write image to stdio using png_image_write_to_stdio
    int write_result = png_image_write_to_stdio(&image, out_file, fuzz_int32_t_6, 
                                                image_buffer, row_stride, NULL);
    
    // Clean up allocated buffer
    png_free_default(NULL, image_buffer);
    
    // Clean up PNG structures
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    }
    
    // Free image resources
    png_image_free(&image);
    
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