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
//<ID> 351
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 0
//<Quality> {"density":36,"unique_branches":{},"library_calls":["png_access_version_number","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_read_info","png_get_tRNS","png_get_palette_max","png_get_x_pixels_per_meter","png_set_hIST","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_get_IHDR","png_set_IHDR","png_set_tRNS","png_set_unknown_chunks","png_set_unknown_chunk_location","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_free","png_write_frame_tail","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_access_version_number","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_tRNS","png_get_palette_max","png_get_x_pixels_per_meter","png_set_hIST","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_get_IHDR","png_set_IHDR","png_set_tRNS","png_set_unknown_chunks","png_set_unknown_chunk_location","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_free","png_write_frame_tail","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	  // PNG signature is 8 bytes
    
    // Get libpng version
    png_uint_32 version = png_access_version_number();
    
    // Create read structures
    png_struct *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
	return 0;
	}
	
    
    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Create input file from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize IO
    png_init_io(png_ptr, in_file);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Get transparency information
    png_byte *trans_alpha = nullptr;
    int num_trans = 0;
    png_color_16 *trans_color = nullptr;
    png_uint_32 trns_result = png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, &trans_color);
    
    // Get palette information
    int palette_max = png_get_palette_max(png_ptr, info_ptr);
    
    // Get pixels per meter
    png_uint_32 x_pixels_per_meter = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    
    // Set up histogram data for hIST chunk
    png_uint_16 hist[256] = {0};
    if (palette_max > 0 && palette_max < 256) {
        for (int i = 0; i <= palette_max; i++) {
            hist[i] = static_cast<png_uint_16>(i * 100);
        }
        png_set_hIST(png_ptr, info_ptr, hist);
    }
    
    // Create write structures for output
    png_struct *write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!write_ptr) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_info *write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_destroy_write_struct(&write_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize output IO
    png_init_io(write_ptr, out_file);
    
    // Copy basic info from read to write
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);
    
    png_set_IHDR(write_ptr, write_info, width, height, bit_depth, color_type,
                 interlace_method, compression_method, filter_method);
    
    // Copy transparency if it exists
    if (trns_result & PNG_INFO_tRNS) {
        if (trans_alpha) {
            png_set_tRNS(write_ptr, write_info, trans_alpha, num_trans, trans_color);
        }
    }
    
    // Set unknown chunk location (example: location 0 = PNG_HAVE_IHDR)
    // We'll create a dummy unknown chunk
    png_unknown_chunk unknown_chunks[1];
    strcpy((char *)unknown_chunks[0].name, "tEsT");
    unknown_chunks[0].data = (png_byte *)"test";
    unknown_chunks[0].size = 4;
    unknown_chunks[0].location = 0;
    
    png_set_unknown_chunks(write_ptr, write_info, unknown_chunks, 1);
    png_set_unknown_chunk_location(write_ptr, write_info, 0, 0);
    
    // Write PNG info
    png_write_info(write_ptr, write_info);
    
    // Read and write image rows
    png_bytep row = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
    for (png_uint_32 y = 0; y < height; y++) {
        png_read_row(png_ptr, row, nullptr);
        png_write_row(write_ptr, row);
    }
    png_free(png_ptr, row);
    
    // Write frame tail (for APNG support)
    png_write_frame_tail(write_ptr, write_info);
    
    // End writing
    png_write_end(write_ptr, write_info);
    
    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    png_destroy_write_struct(&write_ptr, &write_info);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}