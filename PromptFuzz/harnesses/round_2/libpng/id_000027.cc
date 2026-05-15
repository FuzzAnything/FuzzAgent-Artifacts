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
//<ID> 467
//<Prompt> []
/*<Combination>: [
*/
//<score> 79.333336, nr_unique_branch: 29
//<Quality> {"density":34,"unique_branches":{"png_write_png":[[1391,8,1391,23,0,0,4,1],[1391,27,1391,43,0,0,4,1],[1394,8,1394,46,0,0,4,0]],"png_set_unknown_chunks":[[1521,8,1521,23,0,0,4,1],[1521,27,1521,43,0,0,4,1],[1521,47,1521,64,0,0,4,1],[1522,8,1522,24,0,0,4,1],[1559,8,1559,18,0,0,4,1],[1576,11,1576,27,0,0,4,0],[1576,11,1576,27,0,0,4,1],[1582,11,1582,30,0,0,4,1],[1593,14,1593,30,0,0,4,1]],"check_location":[[1487,8,1487,21,0,0,4,1],[1500,8,1500,21,0,0,4,1],[1506,11,1506,45,0,0,4,1]],"png_free_data":[[604,8,604,40,0,0,4,0],[605,8,605,57,0,0,4,0],[607,11,607,20,0,0,4,1],[617,22,617,54,0,0,4,0],[617,22,617,54,0,0,4,1]],"write_unknown_chunks":[[26,8,26,41,0,0,4,0],[33,12,33,72,0,0,4,0],[33,12,33,72,0,0,4,1],[35,14,35,41,0,0,4,0],[35,14,35,41,0,0,4,1],[55,14,55,44,0,0,4,0],[56,15,56,35,0,0,4,0],[63,17,63,30,0,0,4,1]],"png_ascii_from_fp":[[2357,17,2357,24,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_read_info","png_get_IHDR","png_get_x_pixels_per_meter","png_get_current_pass_number","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_set_IHDR","png_set_pHYs","png_set_sCAL","png_set_unknown_chunks","png_write_info","png_write_chunk_start","png_write_chunk_data","png_write_chunk_end","png_write_png","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_IHDR","png_get_x_pixels_per_meter","png_get_current_pass_number","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_set_pHYs","png_set_sCAL","png_set_unknown_chunks","png_write_info","png_write_chunk_start","png_write_chunk_data","png_write_chunk_end","png_write_png","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Too small to be meaningful
    
    // Create read structure
    png_structp png_ptr_read = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr_read) {
	return 0;
	}
	
    
    // Create info structure for reading
    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        return 0;
    }
    
    // Create end info structure
    png_infop end_info_read = png_create_info_struct(png_ptr_read);
    if (!end_info_read) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    // Set up error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr_read))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, &end_info_read);
        return 0;
    }
    
    // Create input FILE from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, &end_info_read);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O and read PNG info
    png_init_io(png_ptr_read, in_file);
    png_read_info(png_ptr_read, info_ptr_read);
    
    // Get IHDR information
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(png_ptr_read, info_ptr_read, &width, &height, &bit_depth, 
                 &color_type, &interlace_method, &compression_method, &filter_method);
    
    // Get x pixels per meter
    png_uint_32 x_pixels_per_meter = png_get_x_pixels_per_meter(png_ptr_read, info_ptr_read);
    
    // Get current pass number (for interlaced images)
    int pass_number = png_get_current_pass_number(png_ptr_read);
    
    // Create write structure
    png_structp png_ptr_write = png_create_write_struct(
        PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr_write) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, &end_info_read);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create info structure for writing
    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, &end_info_read);
        png_destroy_write_struct(&png_ptr_write, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling for writing
    if (setjmp(png_jmpbuf(png_ptr_write))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, &end_info_read);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create output FILE
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, &end_info_read);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O for writing
    png_init_io(png_ptr_write, out_file);
    
    // Set IHDR for output
    png_set_IHDR(png_ptr_write, info_ptr_write, width, height, bit_depth,
                 color_type, interlace_method, compression_method, filter_method);
    
    // Set pHYs (physical pixel dimensions)
    png_set_pHYs(png_ptr_write, info_ptr_write, x_pixels_per_meter, 
                 x_pixels_per_meter, PNG_RESOLUTION_METER);
    
    // Set sCAL (physical scale of subject)
    png_set_sCAL(png_ptr_write, info_ptr_write, PNG_SCALE_METER, 1.0, 1.0);
    
    // Prepare unknown chunk data
    png_unknown_chunk unknown_chunks[1];
    const char chunk_name[] = "tEXt";
    const char chunk_data[] = "FuzzTest:test";
    
    strncpy((char*)unknown_chunks[0].name, chunk_name, 5);
    unknown_chunks[0].data = (png_byte*)chunk_data;
    unknown_chunks[0].size = sizeof(chunk_data) - 1;  // Exclude null terminator
    unknown_chunks[0].location = PNG_HAVE_IHDR;
    
    // Set unknown chunks
    png_set_unknown_chunks(png_ptr_write, info_ptr_write, unknown_chunks, 1);
    
    // Write info
    png_write_info(png_ptr_write, info_ptr_write);
    
    // Write a custom chunk using chunk writing functions
    const png_byte custom_chunk_name[] = {0x74, 0x45, 0x58, 0x74, 0x00}; // "tEXt"
    const char custom_data[] = "CustomChunk";
    
    png_write_chunk_start(png_ptr_write, custom_chunk_name, sizeof(custom_data) - 1);
    png_write_chunk_data(png_ptr_write, (const png_byte*)custom_data, sizeof(custom_data) - 1);
    png_write_chunk_end(png_ptr_write);
    
    // Write PNG (with no transforms)
    png_write_png(png_ptr_write, info_ptr_write, PNG_TRANSFORM_IDENTITY, nullptr);
    
    // Write end
    png_write_end(png_ptr_write, info_ptr_write);
    
    // Cleanup
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Destroy write structure
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    // Destroy read structure
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, &end_info_read);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}