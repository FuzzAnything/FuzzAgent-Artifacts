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
//<ID> 215
//<Prompt> []
/*<Combination>: [
*/
//<score> 87.5, nr_unique_branch: 7
//<Quality> {"density":35,"unique_branches":{"png_set_read_status_fn":[[945,8,945,23,0,0,4,1]],"png_set_packing":[[53,8,53,23,0,0,4,1],[56,8,56,30,0,0,4,0]],"png_set_user_transform_info":[[838,8,838,23,0,0,4,1],[842,8,842,49,0,0,4,0],[843,7,843,48,0,0,4,1]],"png_set_compression_level":[[1199,8,1199,23,0,0,4,1]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_set_packing","png_set_compression_level","png_set_user_transform_info","png_set_read_status_fn","png_read_info","png_get_chunk_malloc_max","png_get_y_pixels_per_meter","png_get_error_ptr","png_get_cICP","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_malloc","png_malloc","png_get_rowbytes","png_read_image","png_read_end","png_free","png_free","png_destroy_read_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_write_info","png_write_end","png_destroy_write_struct","png_destroy_write_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_set_packing","png_set_compression_level","png_set_user_transform_info","png_set_read_status_fn","png_read_info","png_get_chunk_malloc_max","png_get_y_pixels_per_meter","png_get_error_ptr","png_get_cICP","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_malloc","png_malloc","png_get_rowbytes","png_read_image","png_read_end","png_free","png_free","png_destroy_read_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_write_info","png_write_end","png_destroy_write_struct"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	  // Need at least PNG signature
    
    // Check PNG signature
    if (png_sig_cmp((png_byte*)data, 0, 8) != 0) {
        return 0;  // Not a PNG file
    }

    // Create FILE* from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
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
    
    // Set that we already read the signature
    png_set_sig_bytes(png_ptr, 8);
    
    // Configure PNG reading with target APIs
    png_set_packing(png_ptr);  // Pack pixels into bytes
    
    // Set compression level (6 is default)
    png_set_compression_level(png_ptr, 6);
    
    // Create user transform info structure
    struct TransformInfo {
        int dummy;
    };
    TransformInfo transform_info;
    png_set_user_transform_info(png_ptr, &transform_info, 8, 3);
    
    // Set read status callback (empty function)
    png_set_read_status_fn(png_ptr, [](png_structp, png_uint_32, int) {});
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Get chunk malloc max
    png_alloc_size_t chunk_malloc_max = png_get_chunk_malloc_max(png_ptr);
    
    // Get y pixels per meter
    png_uint_32 y_pixels_per_meter = png_get_y_pixels_per_meter(png_ptr, info_ptr);
    
    // Get error pointer
    void* error_ptr = png_get_error_ptr(png_ptr);
    
    // Get cICP information
    png_byte colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag;
    png_uint_32 cicp_result = png_get_cICP(png_ptr, info_ptr, 
                                           &colour_primaries, 
                                           &transfer_function, 
                                           &matrix_coefficients, 
                                           &video_full_range_flag);
    
    // Read image data
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Setup row pointers
    png_bytep* row_pointers = (png_bytep*)png_malloc(png_ptr, 
                                                     height * sizeof(png_bytep));
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)png_malloc(png_ptr, 
                                                png_get_rowbytes(png_ptr, info_ptr));
    }
    
    // Read image
    png_read_image(png_ptr, row_pointers);
    
    // Read end
    png_read_end(png_ptr, info_ptr);
    
    // Clean up row pointers
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    
    // Create write structure for additional testing
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                    nullptr, nullptr, nullptr);
    if (write_ptr) {
        png_infop write_info = png_create_info_struct(write_ptr);
        if (write_info) {
            if (!setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILENAME_PUSH("output_file")
                FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
                if (out_file) {
                    png_init_io(write_ptr, out_file);
                    
                    // Use some of the retrieved information
                    png_set_IHDR(write_ptr, write_info, 
                                 width > 0 ? width : 1, 
                                 height > 0 ? height : 1,
                                 bit_depth, color_type, 
                                 PNG_INTERLACE_NONE,
                                 PNG_COMPRESSION_TYPE_DEFAULT,
                                 PNG_FILTER_TYPE_DEFAULT);
                    
                    png_write_info(write_ptr, write_info);
                    png_write_end(write_ptr, nullptr);
                    
                    assert_file_closed(&out_file);;
                }
            }
            png_destroy_write_struct(&write_ptr, &write_info);
        } else {
            png_destroy_write_struct(&write_ptr, nullptr);
        }
    }
    
    assert_file_closed(&in_file);
	return 0;
}