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
//<ID> 354
//<Prompt> []
/*<Combination>: [
*/
//<score> 210, nr_unique_branch: 17
//<Quality> {"density":42,"unique_branches":{"png_set_hIST":[[383,8,383,23,0,0,4,1],[383,27,383,43,0,0,4,1],[383,47,383,59,0,0,4,1],[386,8,386,34,0,0,4,1],[386,38,387,32,0,0,4,1],[403,8,403,30,0,0,4,1],[409,16,409,41,0,0,4,0],[409,16,409,41,0,0,4,1]],"png_set_tRNS":[[1164,8,1164,27,0,0,4,1],[1202,8,1202,27,0,0,4,1],[1222,11,1222,25,0,0,4,0]],"png_free_data":[[642,8,642,57,0,0,4,0]],"png_get_PLTE":[[1126,8,1126,23,0,0,4,0],[1126,27,1126,43,0,0,4,0],[1127,8,1127,46,0,0,4,0],[1127,8,1127,46,0,0,4,1],[1127,50,1127,65,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_read_info","png_access_version_number","png_get_tRNS","png_get_color_type","png_get_palette_max","png_get_x_pixels_per_meter","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_get_IHDR","png_set_IHDR","png_get_PLTE","png_set_PLTE","png_malloc","png_set_hIST","png_free","png_set_tRNS","png_set_tRNS","png_set_unknown_chunks","png_set_unknown_chunk_location","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_free","png_write_frame_tail","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_read_info","png_access_version_number","png_get_tRNS","png_get_color_type","png_get_palette_max","png_get_x_pixels_per_meter","png_init_io","png_get_IHDR","png_set_IHDR","png_get_PLTE","png_set_PLTE","png_malloc","png_set_hIST","png_free","png_set_tRNS","png_set_unknown_chunks","png_set_unknown_chunk_location","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_free","png_write_frame_tail","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/**/


// Include libpng headers

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	  // Need at least PNG signature
    
    // Create read structure
    png_structp png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                     nullptr, nullptr, nullptr);
    if (!png_ptr_read) {
	return 0;
	}
	
    
    // Create info structure for reading
    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        return 0;
    }
    
    // Create write structure
    png_structp png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                       nullptr, nullptr, nullptr);
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    // Create info structure for writing
    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_write_struct(&png_ptr_write, nullptr);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr_read)) || setjmp(png_jmpbuf(png_ptr_write))) {
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    // Open input from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize IO for reading
    png_init_io(png_ptr_read, in_file);
    
    // Read PNG info
    png_read_info(png_ptr_read, info_ptr_read);
    
    // Call png_access_version_number() - get libpng version
    png_uint_32 version = png_access_version_number();
    
    // Get transparency information
    png_bytep trans_alpha = nullptr;
    int num_trans = 0;
    png_color_16p trans_color = nullptr;
    png_uint_32 tRNS_result = png_get_tRNS(png_ptr_read, info_ptr_read, 
                                          &trans_alpha, &num_trans, &trans_color);
    
    // Get palette information
    int palette_max = 0;
    if (png_get_color_type(png_ptr_read, info_ptr_read) == PNG_COLOR_TYPE_PALETTE) {
        palette_max = png_get_palette_max(png_ptr_read, info_ptr_read);
    }
    
    // Get physical dimensions
    png_uint_32 x_pixels_per_meter = png_get_x_pixels_per_meter(png_ptr_read, 
                                                               info_ptr_read);
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize IO for writing
    png_init_io(png_ptr_write, out_file);
    
    // Copy basic image info from read to write
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(png_ptr_read, info_ptr_read, &width, &height, &bit_depth,
                &color_type, &interlace_method, &compression_method, &filter_method);
    
    png_set_IHDR(png_ptr_write, info_ptr_write, width, height, bit_depth,
                color_type, interlace_method, compression_method, filter_method);
    
    // Copy palette if exists
    png_colorp palette = nullptr;
    int num_palette = 0;
    if (png_get_PLTE(png_ptr_read, info_ptr_read, &palette, &num_palette)) {
        png_set_PLTE(png_ptr_write, info_ptr_write, palette, num_palette);
    }
    
    // Set histogram (hIST) - create dummy histogram
    if (color_type == PNG_COLOR_TYPE_PALETTE && num_palette > 0) {
        png_uint_16 *hist = (png_uint_16 *)png_malloc(png_ptr_write, 
                                                     num_palette * sizeof(png_uint_16));
        for (int i = 0; i < num_palette; i++) {
            hist[i] = (png_uint_16)(i * 100);
        }
        png_set_hIST(png_ptr_write, info_ptr_write, hist);
        png_free(png_ptr_write, hist);
    }
    
    // Copy transparency if exists
    if (tRNS_result) {
        if (trans_alpha) {
            png_set_tRNS(png_ptr_write, info_ptr_write, trans_alpha, num_trans, nullptr);
        } else if (trans_color) {
            png_set_tRNS(png_ptr_write, info_ptr_write, nullptr, 0, trans_color);
        }
    }
    
    // Set unknown chunk location (dummy call with minimal data)
    png_unknown_chunk unknown_chunks[1];
    memset(unknown_chunks, 0, sizeof(unknown_chunks));
    strncpy((char *)unknown_chunks[0].name, "teSt", 5);
    unknown_chunks[0].data = (png_byte *)"test";
    unknown_chunks[0].size = 4;
    unknown_chunks[0].location = 0;
    
    png_set_unknown_chunks(png_ptr_write, info_ptr_write, unknown_chunks, 1);
    png_set_unknown_chunk_location(png_ptr_write, info_ptr_write, 0, 
                                   PNG_HAVE_IHDR | PNG_HAVE_PLTE);
    
    // Write image header
    png_write_info(png_ptr_write, info_ptr_write);
    
    // Allocate row buffer
    png_bytep row = (png_bytep)png_malloc(png_ptr_write, 
                                         png_get_rowbytes(png_ptr_read, info_ptr_read));
    
    // Read and write image data
    for (png_uint_32 y = 0; y < height; y++) {
        png_read_row(png_ptr_read, row, nullptr);
        png_write_row(png_ptr_write, row);
    }
    
    // Free row buffer
    png_free(png_ptr_write, row);
    
    // Write frame tail (for APNG compatibility)
    png_write_frame_tail(png_ptr_write, info_ptr_write);
    
    // Write end of PNG
    png_write_end(png_ptr_write, info_ptr_write);
    
    // Clean up
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}