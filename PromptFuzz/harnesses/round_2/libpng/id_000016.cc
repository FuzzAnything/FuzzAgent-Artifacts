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
//<ID> 266
//<Prompt> []
/*<Combination>: [
*/
//<score> 388.5, nr_unique_branch: 139
//<Quality> {"density":37,"unique_branches":{"png_free_data":[[562,8,562,57,0,0,4,0]],"png_default_write_data":[[54,8,54,23,0,0,4,1],[59,8,59,23,0,0,4,1]],"png_write_complete_chunk":[[195,8,195,23,0,0,4,1],[199,8,199,32,0,0,4,1]],"png_write_chunk_data":[[147,8,147,23,0,0,4,1],[150,8,150,20,0,0,4,0],[150,24,150,34,0,0,4,0]],"png_write_chunk_header":[[100,8,100,23,0,0,4,1]],"png_write_sig":[[81,8,81,30,0,0,4,0]],"png_write_data":[[35,8,35,38,0,0,4,0]],"write_unknown_chunks":[[26,8,26,41,0,0,4,1]],"png_get_unknown_chunks":[[1252,8,1252,23,0,0,4,0],[1252,27,1252,43,0,0,4,0],[1252,47,1252,63,0,0,4,0]],"png_get_user_transform_ptr":[[866,8,866,23,0,0,4,1]],"png_get_rgb_to_gray_status":[[1266,22,1266,29,0,0,4,0]],"png_set_chunk_cache_max":[[1982,8,1982,23,0,0,4,0]],"png_write_iCCP":[[1156,8,1156,23,0,0,4,1],[1159,8,1159,25,0,0,4,0]],"png_write_chunk_end":[[167,8,167,23,0,0,4,1]],"png_get_current_pass_number":[[889,8,889,23,0,0,4,0]],"png_write_info":[[236,8,236,23,0,0,4,1],[236,27,236,43,0,0,4,1],[241,8,241,46,0,0,4,1],[245,13,245,59,0,0,4,1],[249,8,249,45,0,0,4,1],[272,8,272,46,0,0,4,1],[277,8,277,46,0,0,4,1],[285,8,285,46,0,0,4,1],[290,8,290,46,0,0,4,1],[296,8,296,46,0,0,4,1],[303,8,303,46,0,0,4,1],[309,8,309,46,0,0,4,1],[315,8,315,46,0,0,4,1],[323,8,323,46,0,0,4,1],[330,16,330,38,0,0,4,1]],"png_set_iCCP":[[880,8,880,23,0,0,4,1],[880,27,880,43,0,0,4,1],[880,47,880,59,0,0,4,1],[880,63,880,78,0,0,4,1],[883,8,883,53,0,0,4,1],[889,8,889,29,0,0,4,1],[900,8,900,32,0,0,4,1]],"png_write_IHDR":[[701,7,701,31,0,0,4,0],[701,7,701,31,0,0,4,1],[704,13,704,19,0,0,4,0],[704,13,704,19,0,0,4,1],[705,13,705,19,0,0,4,0],[705,13,705,19,0,0,4,1],[706,13,706,19,0,0,4,0],[706,13,706,19,0,0,4,1],[707,13,707,19,0,0,4,0],[707,13,707,19,0,0,4,1],[709,13,709,20,0,0,4,0],[709,13,709,20,0,0,4,1],[713,13,713,20,0,0,4,1],[719,7,719,30,0,0,4,0],[719,7,719,30,0,0,4,1],[722,30,722,46,0,0,4,0],[722,30,722,46,0,0,4,1],[722,50,722,65,0,0,4,1],[724,14,724,30,0,0,4,1],[730,7,730,34,0,0,4,0],[730,7,730,34,0,0,4,1],[733,13,733,19,0,0,4,0],[733,13,733,19,0,0,4,1],[734,13,734,19,0,0,4,0],[734,13,734,19,0,0,4,1],[735,13,735,19,0,0,4,0],[735,13,735,19,0,0,4,1],[736,13,736,19,0,0,4,0],[736,13,736,19,0,0,4,1],[740,13,740,20,0,0,4,1],[745,7,745,37,0,0,4,0],[745,7,745,37,0,0,4,1],[748,30,748,46,0,0,4,0],[748,30,748,46,0,0,4,1],[748,50,748,65,0,0,4,1],[750,14,750,30,0,0,4,1],[756,7,756,36,0,0,4,0],[756,7,756,36,0,0,4,1],[759,30,759,46,0,0,4,0],[759,30,759,46,0,0,4,1],[759,50,759,65,0,0,4,1],[761,14,761,30,0,0,4,1],[767,7,767,14,0,0,4,1],[771,8,771,53,0,0,4,1],[788,10,788,73,0,0,4,1],[794,8,794,43,0,0,4,1],[801,8,801,44,0,0,4,1],[846,8,846,46,0,0,4,0],[848,11,848,56,0,0,4,0],[848,11,848,56,0,0,4,1],[849,11,849,33,0,0,4,0],[849,11,849,33,0,0,4,1],[534,6,534,23,19,0,4,0],[534,6,534,23,19,0,4,1]],"png_write_info_before_PLTE":[[87,8,87,23,0,0,4,1],[87,27,87,43,0,0,4,1],[90,8,90,57,0,0,4,0],[96,11,96,56,0,0,4,0],[97,11,97,47,0,0,4,1],[131,14,131,52,0,0,4,1],[157,14,157,52,0,0,4,1],[172,8,172,46,0,0,4,1],[178,8,178,46,0,0,4,1],[190,8,190,46,0,0,4,1],[201,14,201,52,0,0,4,0],[209,14,209,52,0,0,4,1],[214,11,214,49,0,0,4,1],[219,14,219,52,0,0,4,1]],"png_set_quantize":[[476,8,476,37,0,0,4,1],[479,8,479,23,0,0,4,1],[484,8,484,26,0,0,4,1],[504,8,504,36,0,0,4,1],[796,8,796,32,0,0,4,0],[796,8,796,32,0,0,4,1],[808,8,808,26,0,0,4,0],[826,19,826,34,0,0,4,0],[826,19,826,34,0,0,4,1],[833,23,833,35,0,0,4,0],[833,23,833,35,0,0,4,1],[836,23,836,31,0,0,4,0],[836,23,836,31,0,0,4,1],[840,26,840,40,0,0,4,0],[840,26,840,40,0,0,4,1],[843,26,843,34,0,0,4,0],[843,26,843,34,0,0,4,1],[845,26,845,35,0,0,4,0],[845,26,845,35,0,0,4,1],[848,29,848,42,0,0,4,0],[848,29,848,42,0,0,4,1],[852,29,852,37,0,0,4,0],[852,29,852,37,0,0,4,1],[853,31,853,40,0,0,4,0],[853,31,853,40,0,0,4,1],[856,23,856,49,0,0,4,0],[856,23,856,49,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_set_iCCP","png_set_bKGD","png_get_unknown_chunks","png_get_rgb_to_gray_status","png_get_current_pass_number","png_set_quantize","png_set_chunk_cache_max","png_get_user_transform_ptr","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_set_IHDR","png_set_iCCP","png_set_bKGD","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_free","png_write_end","png_destroy_info_struct","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_set_iCCP","png_set_bKGD","png_get_unknown_chunks","png_get_rgb_to_gray_status","png_get_current_pass_number","png_set_quantize","png_set_chunk_cache_max","png_get_user_transform_ptr","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_set_IHDR","png_set_iCCP","png_set_bKGD","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_free","png_write_end","png_destroy_info_struct","png_destroy_write_struct","png_destroy_read_struct"],"visited":1}
/*Here's a fuzz driver that uses all the specified libpng APIs to handle PNG files:

*/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Too small for meaningful PNG data
    
    // Step 1: Create file handle for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Step 2: Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 3: Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 4: Initialize PNG reading
    png_init_io(png_ptr, in_file);
    png_read_info(png_ptr, info_ptr);
    
    // Step 5: Use required APIs
    
    // 1. png_set_iCCP - set ICC profile
    const char icc_name[] = "sRGB IEC61966-2.1";
    const png_byte icc_profile[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; // Dummy profile
    png_set_iCCP(png_ptr, info_ptr, icc_name, PNG_COMPRESSION_TYPE_BASE, 
                 icc_profile, sizeof(icc_profile));
    
    // 2. png_set_bKGD - set background color
    png_color_16 background_color;
    background_color.index = 0;
    background_color.red = 65535;
    background_color.green = 65535;
    background_color.blue = 65535;
    background_color.gray = 65535;
    png_set_bKGD(png_ptr, info_ptr, &background_color);
    
    // 3. png_get_unknown_chunks - retrieve unknown chunks
    png_unknown_chunk *unknown_chunks = nullptr;
    int num_unknowns = png_get_unknown_chunks(png_ptr, info_ptr, &unknown_chunks);
    (void)num_unknowns; // Use variable to avoid unused warning
    
    // 4. png_get_rgb_to_gray_status - check RGB to gray conversion status
    png_byte rgb_to_gray_status = png_get_rgb_to_gray_status(png_ptr);
    (void)rgb_to_gray_status;
    
    // 5. png_get_current_pass_number - get current interlace pass
    png_byte current_pass = png_get_current_pass_number(png_ptr);
    (void)current_pass;
    
    // 6. png_set_quantize - set quantization parameters
    png_color palette[256];
    png_uint_16 histogram[256];
    for (int i = 0; i < 256; ++i) {
        palette[i].red = i;
        palette[i].green = i;
        palette[i].blue = i;
        histogram[i] = i * 10;
    }
    png_set_quantize(png_ptr, palette, 256, 256, histogram, 1);
    
    // 7. png_set_chunk_cache_max - set chunk cache limit
    png_set_chunk_cache_max(png_ptr, 1000);
    
    // 8. png_get_user_transform_ptr - get user transform pointer
    void *user_transform_ptr = png_get_user_transform_ptr(png_ptr);
    (void)user_transform_ptr;
	FUZZ_FILENAME_PUSH("output_file")
    
    // Step 6: Create output file and write processed PNG
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file) {
        png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                        nullptr, nullptr, nullptr);
        if (write_ptr) {
            png_infop write_info = png_create_info_struct(write_ptr);
            if (write_info) {
                if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
                    png_destroy_info_struct(write_ptr, &write_info);
                    png_destroy_write_struct(&write_ptr, nullptr);
                    assert_file_closed(&out_file);;
                    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
                    assert_file_closed(&in_file);;
                    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
                }
                
                png_init_io(write_ptr, out_file);
                
                // Copy information from read structure to write structure
                png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
                png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
                png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
                png_byte color_type = png_get_color_type(png_ptr, info_ptr);
                
                png_set_IHDR(write_ptr, write_info, width, height, bit_depth,
                            color_type, PNG_INTERLACE_NONE,
                            PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
                
                // Apply the transformations we set earlier
                png_set_iCCP(write_ptr, write_info, icc_name, PNG_COMPRESSION_TYPE_BASE,
                           icc_profile, sizeof(icc_profile));
                png_set_bKGD(write_ptr, write_info, &background_color);
                
                png_write_info(write_ptr, write_info);
                
                // Read image rows and write them out
                png_bytep row = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
                for (png_uint_32 y = 0; y < height; y++) {
                    png_read_row(png_ptr, row, nullptr);
                    png_write_row(write_ptr, row);
                }
                png_free(png_ptr, row);
                
                png_write_end(write_ptr, nullptr);
                png_destroy_info_struct(write_ptr, &write_info);
            }
            png_destroy_write_struct(&write_ptr, nullptr);
        }
        assert_file_closed(&out_file);;
    }
    
    // Step 7: Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}