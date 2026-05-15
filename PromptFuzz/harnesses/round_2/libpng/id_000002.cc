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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 0
//<Quality> {"density":40,"unique_branches":{},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_read_info","png_get_sCAL_s","png_chunk_warning","png_get_interlace_type","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_palette_to_rgb","png_set_expand_gray_1_2_4_to_8","png_get_valid","png_set_tRNS_to_alpha","png_read_update_info","png_destroy_read_struct","png_get_rowbytes","png_destroy_read_struct","png_read_image","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_set_IHDR","png_write_info","png_write_rows","png_write_end","png_data_freer","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_read_info","png_get_sCAL_s","png_chunk_warning","png_get_interlace_type","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_palette_to_rgb","png_set_expand_gray_1_2_4_to_8","png_get_valid","png_set_tRNS_to_alpha","png_read_update_info","png_get_rowbytes","png_read_image","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_write_info","png_write_rows","png_write_end","png_data_freer","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/**/


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
	
    
    // Check PNG signature
    if (png_sig_cmp((png_byte*)data, 0, 8) != 0) {
        return 0;
    }
    
    // Create read structures
    png_struct *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                  nullptr, nullptr, nullptr);
    if (!png_ptr) {
	return 0;
	}
	
    
    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Setup error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Read from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_init_io(png_ptr, in_file);
    png_set_sig_bytes(png_ptr, 8);
    
    // Read image info
    png_read_info(png_ptr, info_ptr);
    
    // Get sCAL chunk if present
    int unit = 0;
    char *swidth = nullptr;
    char *sheight = nullptr;
    png_uint_32 scal_result = png_get_sCAL_s(png_ptr, info_ptr, &unit, &swidth, &sheight);
    
    if (scal_result & PNG_INFO_sCAL) {
        // We have sCAL data
        if (swidth) free(swidth);
        if (sheight) free(sheight);
    } else {
        // Issue a warning about missing sCAL
        png_chunk_warning(png_ptr, "Missing sCAL chunk");
    }
    
    // Get interlace type
    png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    
    // Prepare for reading image data
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Setup transformations if needed
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);
    
    png_read_update_info(png_ptr, info_ptr);
    
    // Allocate memory for row pointers
    png_bytep *row_pointers = (png_bytep*)malloc(height * sizeof(png_bytep));
    if (!row_pointers) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    // Allocate memory for image data
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(rowbytes);
        if (!row_pointers[y]) {
            for (png_uint_32 j = 0; j < y; j++) free(row_pointers[j]);
            free(row_pointers);
            assert_file_closed(&in_file);;
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // Read the image data
    png_read_image(png_ptr, row_pointers);
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create write structures
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        for (png_uint_32 y = 0; y < height; y++) free(row_pointers[y]);
        free(row_pointers);
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_struct *write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                    nullptr, nullptr, nullptr);
    if (!write_ptr) {
        for (png_uint_32 y = 0; y < height; y++) free(row_pointers[y]);
        free(row_pointers);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_info *write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        for (png_uint_32 y = 0; y < height; y++) free(row_pointers[y]);
        free(row_pointers);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_destroy_write_struct(&write_ptr, nullptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        for (png_uint_32 y = 0; y < height; y++) free(row_pointers[y]);
        free(row_pointers);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_init_io(write_ptr, out_file);
    
    // Setup write info from read info
    png_set_IHDR(write_ptr, write_info, width, height,
                 bit_depth, color_type, interlace_type,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    
    png_write_info(write_ptr, write_info);
    
    // Write rows using png_write_rows
    if (height > 0) {
        png_write_rows(write_ptr, row_pointers, 1);
    }
    
    // Finish writing
    png_write_end(write_ptr, nullptr);
    
    // Free data using png_data_freer
    png_data_freer(png_ptr, info_ptr, PNG_DESTROY_WILL_FREE_DATA, PNG_FREE_ALL);
    
    // Cleanup
    for (png_uint_32 y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    png_destroy_write_struct(&write_ptr, &write_info);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}