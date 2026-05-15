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
//<ID> 282
//<Prompt> []
/*<Combination>: [
*/
//<score> 144, nr_unique_branch: 25
//<Quality> {"density":48,"unique_branches":{"png_free_data":[[642,8,642,57,0,0,4,0]],"png_handle_oFFs":[[2748,18,2748,33,5,0,4,1]],"png_get_cHRM":[[533,8,533,23,0,0,4,0],[533,27,533,43,0,0,4,0],[534,8,534,46,0,0,4,0],[534,8,534,46,0,0,4,1],[536,11,536,25,0,0,4,0],[538,11,538,25,0,0,4,0],[540,11,540,25,0,0,4,0],[542,11,542,25,0,0,4,0],[544,11,544,25,0,0,4,0],[546,11,546,25,0,0,4,0],[548,11,548,25,0,0,4,0],[550,11,550,25,0,0,4,0]],"png_set_sRGB_gAMA_and_cHRM":[[847,8,847,23,0,0,4,1],[847,27,847,43,0,0,4,1]],"png_set_hIST":[[383,8,383,23,0,0,4,1],[383,27,383,43,0,0,4,1],[383,47,383,59,0,0,4,1],[386,8,386,34,0,0,4,0],[386,8,386,34,0,0,4,1],[386,38,387,32,0,0,4,1],[403,8,403,30,0,0,4,1],[409,16,409,41,0,0,4,0],[409,16,409,41,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_read_info","png_set_chunk_malloc_max","png_get_cHRM","png_set_sRGB_gAMA_and_cHRM","png_set_cLLI_fixed","png_set_hIST","png_process_data_skip","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_malloc","png_malloc","png_get_rowbytes","png_read_image","png_read_end","png_free","png_free","png_destroy_read_struct","png_create_write_struct","png_free","png_free","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct","png_init_io","png_set_IHDR","png_write_rows","png_write_png","png_save_int_32","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_read_info","png_set_chunk_malloc_max","png_get_cHRM","png_set_sRGB_gAMA_and_cHRM","png_set_cLLI_fixed","png_set_hIST","png_process_data_skip","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_malloc","png_malloc","png_get_rowbytes","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_write_rows","png_write_png","png_save_int_32","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
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

    // Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Initialize PNG read structures
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
    
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize IO
    png_init_io(png_ptr, in_file);
    png_set_sig_bytes(png_ptr, 0);

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // Set chunk malloc max
    png_set_chunk_malloc_max(png_ptr, 1024 * 1024);

    // Get cHRM data
    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    png_uint_32 chrm_result = png_get_cHRM(png_ptr, info_ptr, &white_x, &white_y, &red_x, &red_y, 
                                          &green_x, &green_y, &blue_x, &blue_y);

    // Set sRGB gAMA and cHRM
    png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, PNG_sRGB_INTENT_PERCEPTUAL);

    // Set CLLI fixed values
    png_set_cLLI_fixed(png_ptr, info_ptr, 1000, 800);

    // Set hIST (histogram) - create dummy histogram data
    png_uint_16 hist[256];
    for (int i = 0; i < 256; i++) {
        hist[i] = static_cast<png_uint_16>(i * 100);
    }
    png_set_hIST(png_ptr, info_ptr, hist);

    // Process data skip
    png_uint_32 skip_result = png_process_data_skip(png_ptr);

    // Read image data
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    // Set up row pointers
    png_bytep *row_pointers = (png_bytep*)png_malloc(png_ptr, height * sizeof(png_bytep));
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
    }

    // Read image
    png_read_image(png_ptr, row_pointers);
    png_read_end(png_ptr, end_info);
	FUZZ_FILENAME_PUSH("output_file")

    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        // Cleanup
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
        png_free(png_ptr, row_pointers);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create write structure
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!write_ptr) {
        assert_file_closed(&out_file);;
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
        png_free(png_ptr, row_pointers);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        assert_file_closed(&out_file);;
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
        png_free(png_ptr, row_pointers);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
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
            png_free(png_ptr, row_pointers[y]);
        }
        png_free(png_ptr, row_pointers);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize write IO
    png_init_io(write_ptr, out_file);

    // Copy info from read to write
    png_uint_32 write_width = width;
    png_uint_32 write_height = height;
    int write_bit_depth = bit_depth;
    int write_color_type = color_type;
    
    png_set_IHDR(write_ptr, write_info, write_width, write_height,
                 write_bit_depth, write_color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Write rows
    png_write_rows(write_ptr, row_pointers, height);

    // Write the complete PNG
    png_write_png(write_ptr, write_info, PNG_TRANSFORM_IDENTITY, nullptr);

    // Save an integer using png_save_int_32
    png_byte int_buffer[4];
    png_save_int_32(int_buffer, 123456);

    // Cleanup write structures
    png_destroy_write_struct(&write_ptr, &write_info);
    assert_file_closed(&out_file);;

    // Cleanup read structures and memory
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);
    
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    assert_file_closed(&in_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}