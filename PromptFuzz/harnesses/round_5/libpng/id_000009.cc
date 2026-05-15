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
//<ID> 88
//<Prompt> []
/*<Combination>: [
*/
//<score> 111, nr_unique_branch: 6
//<Quality> {"density":37,"unique_branches":{"png_get_compression_buffer_size":[[1281,8,1281,23,0,0,4,1],[1285,8,1285,49,0,0,4,0]],"png_set_gamma_fixed":[[878,8,878,37,0,0,4,1],[896,8,896,23,0,0,4,0],[898,8,898,23,0,0,4,1],[901,8,901,57,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_compression_buffer_size","png_set_gamma_fixed","png_set_background_fixed","png_read_update_info","png_get_text","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_destroy_read_struct","png_get_rowbytes","png_destroy_read_struct","png_read_image","png_read_end","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_write_sig","png_set_IHDR","png_set_text","png_write_info","png_write_image","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_compression_buffer_size","png_set_gamma_fixed","png_set_background_fixed","png_read_update_info","png_get_text","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_rowbytes","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_write_sig","png_set_IHDR","png_set_text","png_write_info","png_write_image","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to achieve a PNG image processing event:

*/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for PNG processing

    // Create input file from memory
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

    png_struct *png_ptr = nullptr;
    png_info *info_ptr = nullptr;
    png_info *end_info = nullptr;

    // Initialize PNG read structure
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
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

    end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize IO
    png_init_io(png_ptr, in_file);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // Event: Process PNG with gamma correction and background setting
    // 1. Get compression buffer size
    size_t compression_buffer_size = png_get_compression_buffer_size(png_ptr);
    
    // 2. Apply gamma correction
    png_fixed_point screen_gamma = 45000; // 2.2 gamma as fixed point
    png_fixed_point override_gamma = 0; // No override
    png_set_gamma_fixed(png_ptr, screen_gamma, override_gamma);
    
    // 3. Set background color
    png_color_16 background_color;
    background_color.red = 255;
    background_color.green = 255;
    background_color.blue = 255;
    background_color.gray = 255;
    background_color.index = 0;
    
    png_set_background_fixed(png_ptr, &background_color, 
                            PNG_BACKGROUND_GAMMA_SCREEN, 0, 0);
    
    // Update info after transformations
    png_read_update_info(png_ptr, info_ptr);
    
    // 4. Get text chunks (if any)
    png_text *text_ptr = nullptr;
    int num_text = 0;
    png_get_text(png_ptr, info_ptr, &text_ptr, &num_text);
    
    // Read image data
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Allocate row pointers
    png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    if (!row_pointers) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
        if (!row_pointers[y]) {
            for (png_uint_32 i = 0; i < y; i++) free(row_pointers[i]);
            free(row_pointers);
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // Read image
    png_read_image(png_ptr, row_pointers);
    png_read_end(png_ptr, end_info);
    
    // Create write structure for output
    png_struct *write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                   nullptr, nullptr, nullptr);
    if (!write_ptr) {
        for (png_uint_32 y = 0; y < height; y++) free(row_pointers[y]);
        free(row_pointers);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_info *write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        for (png_uint_32 y = 0; y < height; y++) free(row_pointers[y]);
        free(row_pointers);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&write_ptr, &write_info);
        for (png_uint_32 y = 0; y < height; y++) free(row_pointers[y]);
        free(row_pointers);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize write IO
    png_init_io(write_ptr, out_file);
    
    // 5. Write PNG signature (required API call)
    png_write_sig(write_ptr);
    
    // Set output image info
    png_set_IHDR(write_ptr, write_info, width, height, bit_depth, color_type,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    
    // Copy text chunks to output
    if (num_text > 0 && text_ptr) {
        png_set_text(write_ptr, write_info, text_ptr, num_text);
    }
    
    png_write_info(write_ptr, write_info);
    png_write_image(write_ptr, row_pointers);
    png_write_end(write_ptr, nullptr);
    
    // Cleanup
    png_destroy_write_struct(&write_ptr, &write_info);
    
    for (png_uint_32 y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);
    
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}