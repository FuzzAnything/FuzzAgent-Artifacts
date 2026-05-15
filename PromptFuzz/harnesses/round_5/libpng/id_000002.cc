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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 58.5, nr_unique_branch: 5
//<Quality> {"density":39,"unique_branches":{"convert_gamma_value":[[301,8,301,24,0,0,4,0],[301,28,301,46,0,0,4,0],[307,8,307,33,0,0,4,1],[307,37,307,62,0,0,4,1]],"png_set_tRNS_to_alpha":[[987,8,987,37,0,0,4,1]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_set_alpha_mode","png_set_tRNS_to_alpha","png_read_info","png_get_x_offset_inches_fixed","png_get_y_pixels_per_meter","png_get_y_offset_pixels","png_get_progressive_ptr","png_set_oFFs","png_read_update_info","png_get_image_height","png_malloc","png_get_rowbytes","png_malloc","png_read_image","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_get_image_width","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_set_IHDR","png_write_info","png_write_image","png_write_end","png_destroy_info_struct","png_destroy_write_struct","png_free","png_free","png_read_end","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_set_alpha_mode","png_set_tRNS_to_alpha","png_read_info","png_get_x_offset_inches_fixed","png_get_y_pixels_per_meter","png_get_y_offset_pixels","png_get_progressive_ptr","png_set_oFFs","png_read_update_info","png_get_image_height","png_malloc","png_get_rowbytes","png_malloc","png_read_image","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_get_image_width","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_set_IHDR","png_write_info","png_write_image","png_write_end","png_destroy_info_struct","png_destroy_write_struct","png_free","png_free","png_read_end","png_destroy_read_struct"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	
    
    // Check if data has PNG signature
    if (png_sig_cmp(data, 0, 8) != 0) {
	return 0;
	}
	
    
    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create read structure
    png_struct *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create info structure
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
    
    // Initialize I/O
    png_init_io(png_ptr, in_file);
    png_set_sig_bytes(png_ptr, 8);
    
    // Set transformations
    png_set_alpha_mode(png_ptr, PNG_ALPHA_PNG, 2.2);
    png_set_tRNS_to_alpha(png_ptr);
    
    // Read image info
    png_read_info(png_ptr, info_ptr);
    
    // Get various image properties
    png_fixed_point x_offset_inches = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    png_uint_32 y_pixels_per_meter = png_get_y_pixels_per_meter(png_ptr, info_ptr);
    png_int_32 y_offset_pixels = png_get_y_offset_pixels(png_ptr, info_ptr);
    
    // Get progressive pointer (returns NULL if not set)
    void *progressive_ptr = png_get_progressive_ptr(png_ptr);
    
    // Set image offsets (using values from input or defaults)
    png_set_oFFs(png_ptr, info_ptr, 
                 static_cast<png_int_32>(x_offset_inches), 
                 y_offset_pixels, 
                 PNG_OFFSET_PIXEL);
    
    // Update info after transformations
    png_read_update_info(png_ptr, info_ptr);
    
    // Allocate memory for row pointers
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_bytep *row_pointers = (png_bytep*)png_malloc(png_ptr, 
        sizeof(png_bytep) * height);
    
    // Allocate memory for each row
    png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)png_malloc(png_ptr, rowbytes);
    }
    
    // Read the image data
    png_read_image(png_ptr, row_pointers);
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file) {
        // Create write structure
        png_struct *write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                       nullptr, nullptr, nullptr);
        if (write_ptr) {
            png_info *write_info = png_create_info_struct(write_ptr);
            if (write_info) {
                if (!setjmp(png_jmpbuf(write_ptr))) {
                    png_init_io(write_ptr, out_file);
                    
                    // Copy image info to write structure
                    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
                    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
                    int color_type = png_get_color_type(png_ptr, info_ptr);
                    int interlace_type = png_get_interlace_type(png_ptr, info_ptr);
                    int compression_type = png_get_compression_type(png_ptr, info_ptr);
                    int filter_type = png_get_filter_type(png_ptr, info_ptr);
                    
                    png_set_IHDR(write_ptr, write_info, width, height, bit_depth,
                                color_type, interlace_type, compression_type, filter_type);
                    
                    // Write image
                    png_write_info(write_ptr, write_info);
                    png_write_image(write_ptr, row_pointers);
                    png_write_end(write_ptr, nullptr);
                }
                png_destroy_info_struct(write_ptr, &write_info);
            }
            png_destroy_write_struct(&write_ptr, nullptr);
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);
    
    png_read_end(png_ptr, nullptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}