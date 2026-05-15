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
//<ID> 757
//<Prompt> []
/*<Combination>: [
*/
//<score> 144, nr_unique_branch: 30
//<Quality> {"density":36,"unique_branches":{"png_set_filler":[[152,8,152,23,0,0,4,1],[158,8,158,49,0,0,4,0],[224,8,224,38,0,0,4,0]],"png_read_start_row":[[4645,11,4645,56,0,0,4,0],[4647,14,4647,37,0,0,4,0],[4647,14,4647,37,0,0,4,1],[4693,8,4693,54,0,0,4,0],[4695,11,4695,53,0,0,4,0],[4695,11,4695,53,0,0,4,1],[4697,14,4697,34,0,0,4,0],[4697,14,4697,34,0,0,4,1],[4704,16,4704,57,0,0,4,0],[4704,16,4704,57,0,0,4,1],[4705,10,4705,55,0,0,4,0],[4705,10,4705,55,0,0,4,1],[4707,14,4707,35,0,0,4,0],[4707,14,4707,35,0,0,4,1]],"png_read_transform_info":[[2057,11,2057,57,0,0,4,0],[2058,11,2058,36,0,0,4,0],[2058,40,2058,64,0,0,4,0],[2073,11,2073,57,0,0,4,0],[2079,14,2079,36,0,0,4,0],[2079,14,2079,36,0,0,4,1],[2088,14,2088,38,0,0,4,1],[2228,8,2228,52,0,0,4,0],[2229,9,2229,51,0,0,4,0],[2229,9,2229,51,0,0,4,1],[2230,8,2230,51,0,0,4,0],[2230,8,2230,51,0,0,4,1],[2234,11,2234,58,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_set_expand","png_set_filler","png_set_text_compression_method","png_read_update_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_uint_31","png_get_rowbytes","png_get_rowbytes","png_set_rows","png_start_read_image","png_read_image","png_set_PLTE","png_get_pCAL","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_set_IHDR","png_set_PLTE","png_write_info","png_write_image","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_set_expand","png_set_filler","png_set_text_compression_method","png_read_update_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_uint_31","png_get_rowbytes","png_get_rowbytes","png_set_rows","png_start_read_image","png_read_image","png_set_PLTE","png_get_pCAL","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_set_PLTE","png_write_info","png_write_image","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need enough data for PNG operations

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Initialize reading structures
    png_struct *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
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

    // API 1: png_set_expand - expand various PNG types
    png_set_expand(png_ptr);

    // API 2: png_set_filler - add filler byte
    png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);

    // API 3: png_set_text_compression_method - set text compression
    png_set_text_compression_method(png_ptr, PNG_TEXT_COMPRESSION_zTXt);

    // Read updated info after transformations
    png_read_update_info(png_ptr, info_ptr);

    // Get image dimensions
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // API 4: png_get_uint_31 - demonstrate with buffer
    png_byte test_buf[4];
    if (size >= 4) {
        memcpy(test_buf, data, 4);
        png_uint_32 test_val = png_get_uint_31(png_ptr, test_buf);
        // Use test_val somehow
        (void)test_val;
    }

    // Allocate row pointers
    png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    png_byte bytes_per_pixel = png_get_rowbytes(png_ptr, info_ptr) / width;
    
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
    }

    // API 5: png_set_rows - set row pointers
    png_set_rows(png_ptr, info_ptr, row_pointers);

    // API 6: png_start_read_image - start reading image data
    png_start_read_image(png_ptr);

    // Read image data
    png_read_image(png_ptr, row_pointers);

    // API 7: png_set_PLTE - set palette (even if not palette image)
    png_color palette[256];
    int palette_size = 256;
    // Create a simple grayscale palette
    for (int i = 0; i < palette_size; i++) {
        palette[i].red = palette[i].green = palette[i].blue = i;
    }
    png_set_PLTE(png_ptr, info_ptr, palette, palette_size);

    // API 8: png_get_pCAL - try to get pCAL chunk data
    char *purpose = nullptr;
    png_int_32 X0, X1;
    int type, nparams;
    char *units = nullptr;
    char **params = nullptr;
    
    png_uint_32 pcal_ret = png_get_pCAL(png_ptr, info_ptr, &purpose, &X0, &X1, 
                                        &type, &nparams, &units, &params);
	FUZZ_FILENAME_PUSH("output_file")

    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        // Cleanup
        for (png_uint_32 y = 0; y < height; y++) {
            free(row_pointers[y]);
        }
        free(row_pointers);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create write structure
    png_struct *write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                    nullptr, nullptr, nullptr);
    if (!write_ptr) {
        assert_file_closed(&out_file);;
        for (png_uint_32 y = 0; y < height; y++) {
            free(row_pointers[y]);
        }
        free(row_pointers);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_info *write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        assert_file_closed(&out_file);;
        for (png_uint_32 y = 0; y < height; y++) {
            free(row_pointers[y]);
        }
        free(row_pointers);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&out_file);;
        for (png_uint_32 y = 0; y < height; y++) {
            free(row_pointers[y]);
        }
        free(row_pointers);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up output
    png_init_io(write_ptr, out_file);
    
    // Copy basic info
    png_set_IHDR(write_ptr, write_info, width, height, bit_depth, 
                 color_type, PNG_INTERLACE_NONE, 
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Set palette for output
    png_set_PLTE(write_ptr, write_info, palette, palette_size);

    // Write info
    png_write_info(write_ptr, write_info);
    
    // Write image
    png_write_image(write_ptr, row_pointers);
    png_write_end(write_ptr, write_info);

    // Cleanup
    png_destroy_write_struct(&write_ptr, &write_info);
    assert_file_closed(&out_file);;

    // Free pCAL data if it exists
    if (pcal_ret) {
        if (purpose) free(purpose);
        if (units) free(units);
        if (params) {
            for (int i = 0; i < nparams; i++) {
                if (params[i]) free(params[i]);
            }
            free(params);
        }
    }

    for (png_uint_32 y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);

    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}