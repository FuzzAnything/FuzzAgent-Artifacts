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
//<ID> 87
//<Prompt> []
/*<Combination>: [
*/
//<score> 90, nr_unique_branch: 2
//<Quality> {"density":30,"unique_branches":{"png_write_sig":[[81,8,81,30,0,0,4,1]],"png_write_data":[[35,8,35,38,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_write_sig","png_read_info","png_get_compression_buffer_size","png_set_gamma_fixed","png_set_background_fixed","png_get_text","png_read_update_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_rowbytes","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_write_struct","png_init_io","png_set_IHDR","png_set_text","png_write_info","png_write_image","png_write_end","png_destroy_write_struct","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_write_sig","png_read_info","png_get_compression_buffer_size","png_set_gamma_fixed","png_set_background_fixed","png_get_text","png_read_update_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_rowbytes","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_set_text","png_write_info","png_write_image","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need enough data for meaningful PNG processing

    // Create input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Initialize PNG read structures
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

    // Initialize I/O
    png_init_io(png_ptr, in_file);

    // Read PNG signature
    png_write_sig(png_ptr); // Note: This is typically for writing, but we call it as required
    png_read_info(png_ptr, info_ptr);

    // Get compression buffer size
    size_t comp_buf_size = png_get_compression_buffer_size(png_ptr);

    // Set gamma correction
    png_fixed_point screen_gamma = 45000; // 0.45 in fixed-point
    png_fixed_point file_gamma = 55000;   // 0.55 in fixed-point
    png_set_gamma_fixed(png_ptr, screen_gamma, file_gamma);

    // Set background color
    png_color_16 background;
    background.index = 0;
    background.red = 255;
    background.green = 255;
    background.blue = 255;
    background.gray = 255;
    png_set_background_fixed(png_ptr, &background, PNG_BACKGROUND_GAMMA_SCREEN, 1, 0);

    // Get text chunks if present
    png_textp text_ptr = nullptr;
    int num_text = 0;
    png_get_text(png_ptr, info_ptr, &text_ptr, &num_text);

    // Read image data
    png_read_update_info(png_ptr, info_ptr);
    
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Allocate row pointers
    png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
    }
    
    // Read image
    png_read_image(png_ptr, row_pointers);
    
    // Read end
    png_read_end(png_ptr, nullptr);
	FUZZ_FILENAME_PUSH("output_file")

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file) {
        // Initialize write structure
        png_struct *write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (write_ptr) {
            png_info *write_info = png_create_info_struct(write_ptr);
            if (write_info) {
                if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
                    png_destroy_write_struct(&write_ptr, &write_info);
                } else {
                    png_init_io(write_ptr, out_file);
                    
                    // Write header
                    png_set_IHDR(write_ptr, write_info, width, height,
                                 bit_depth, color_type, PNG_INTERLACE_NONE,
                                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
                    
                    // Write text if we found any
                    if (num_text > 0 && text_ptr) {
                        png_set_text(write_ptr, write_info, text_ptr, num_text);
                    }
                    
                    png_write_info(write_ptr, write_info);
                    png_write_image(write_ptr, row_pointers);
                    png_write_end(write_ptr, nullptr);
                    
                    png_destroy_write_struct(&write_ptr, &write_info);
                }
            } else {
                png_destroy_write_struct(&write_ptr, nullptr);
            }
        }
        assert_file_closed(&out_file);;
    }

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