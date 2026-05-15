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
//<ID> 23
//<Prompt> []
/*<Combination>: [
*/
//<score> 19.5, nr_unique_branch: 0
//<Quality> {"density":39,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_read_info","png_get_sCAL_s","png_chunk_warning","png_get_interlace_type","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_palette_to_rgb","png_set_expand_gray_1_2_4_to_8","png_get_valid","png_set_tRNS_to_alpha","png_set_strip_16","png_set_gray_to_rgb","png_read_update_info","png_get_rowbytes","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_set_sCAL_s","png_write_info","png_write_image","png_write_end","png_destroy_write_struct","png_destroy_write_struct","png_destroy_write_struct","png_free","png_free","png_data_freer","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_read_info","png_get_sCAL_s","png_chunk_warning","png_get_interlace_type","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_palette_to_rgb","png_set_expand_gray_1_2_4_to_8","png_get_valid","png_set_tRNS_to_alpha","png_set_strip_16","png_set_gray_to_rgb","png_read_update_info","png_get_rowbytes","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_set_sCAL_s","png_write_info","png_write_image","png_write_end","png_destroy_write_struct","png_free","png_free","png_data_freer","png_destroy_read_struct"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
        return 0; // Need at least PNG signature
    }

    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create PNG read structure
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
    png_set_sig_bytes(png_ptr, 8);

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // Get sCAL chunk data if present
    int unit = 0;
    char *swidth = nullptr;
    char *sheight = nullptr;
    png_uint_32 scal_ret = png_get_sCAL_s(png_ptr, info_ptr, &unit, &swidth, &sheight);
    
    if (scal_ret == 0) {
        // No sCAL chunk found - emit a warning
        png_chunk_warning(png_ptr, "No sCAL chunk found in PNG file");
    }

    // Get interlace type
    png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);

    // Prepare for reading image data
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    // Transformations to ensure we can read all PNG types
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
    }
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    }
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr);
    }
    if (bit_depth == 16) {
        png_set_strip_16(png_ptr);
    }
    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png_ptr);
    }

    png_read_update_info(png_ptr, info_ptr);

    // Read image rows
    png_bytep *row_pointers = new png_bytep[height];
    png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = new png_byte[rowbytes];
    }

    png_read_image(png_ptr, row_pointers);
    png_read_end(png_ptr, nullptr);
	FUZZ_FILENAME_PUSH("output_file")

    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file) {
        png_struct *write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (write_ptr) {
            png_info *write_info = png_create_info_struct(write_ptr);
            if (write_info && !setjmp(png_jmpbuf(write_ptr))) {
                png_init_io(write_ptr, out_file);
                
                // Write the same image back out
                png_set_IHDR(write_ptr, write_info, width, height,
                            bit_depth, color_type, interlace_type,
                            PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
                
                // Copy relevant chunks from original
                if (scal_ret) {
                    png_set_sCAL_s(write_ptr, write_info, unit, swidth, sheight);
                }
                
                png_write_info(write_ptr, write_info);
                png_write_image(write_ptr, row_pointers);
                png_write_end(write_ptr, nullptr);
                
                png_destroy_write_struct(&write_ptr, &write_info);
            } else {
                if (write_info) {
                    png_destroy_write_struct(&write_ptr, &write_info);
                } else {
                    png_destroy_write_struct(&write_ptr, nullptr);
                }
            }
        }
        assert_file_closed(&out_file);;
    }

    // Free row data
    for (png_uint_32 y = 0; y < height; y++) {
        delete[] row_pointers[y];
    }
    delete[] row_pointers;

    // Free sCAL strings if they were allocated
    if (swidth) {
        png_free(png_ptr, swidth);
    }
    if (sheight) {
        png_free(png_ptr, sheight);
    }

    // Free PNG data using png_data_freer
    png_data_freer(png_ptr, info_ptr, PNG_DESTROY_WILL_FREE_DATA, PNG_FREE_ALL);

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}