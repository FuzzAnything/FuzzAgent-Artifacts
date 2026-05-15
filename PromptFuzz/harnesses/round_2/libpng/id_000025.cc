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
//<ID> 410
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 21
//<Quality> {"density":18,"unique_branches":{"png_get_y_offset_microns":[[269,8,269,23,0,0,4,0],[269,27,269,43,0,0,4,0],[270,8,270,46,0,0,4,0],[270,8,270,46,0,0,4,1],[272,11,272,62,0,0,4,0]],"png_get_next_frame_y_offset":[[1463,8,1463,23,0,0,4,0],[1463,27,1463,43,0,0,4,0]],"png_get_mDCV_fixed":[[838,8,838,23,0,0,4,0],[838,27,838,43,0,0,4,0],[839,8,839,46,0,0,4,0],[839,8,839,46,0,0,4,1],[841,11,841,26,0,0,4,0],[842,11,842,26,0,0,4,0],[843,11,843,24,0,0,4,0],[844,11,844,24,0,0,4,0],[845,11,845,26,0,0,4,0],[846,11,846,26,0,0,4,0],[847,11,847,25,0,0,4,0],[848,11,848,25,0,0,4,0],[849,11,849,34,0,0,4,0],[850,11,850,34,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_mDCV_fixed","png_get_y_offset_microns","png_get_next_frame_y_offset","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_info_struct","png_destroy_write_struct","png_set_cHRM_fixed","png_set_cHRM_fixed","png_destroy_info_struct","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_mDCV_fixed","png_get_y_offset_microns","png_get_next_frame_y_offset","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_info_struct","png_destroy_write_struct","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE* for reading input data from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize libpng structures for reading
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

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize I/O for reading
    png_init_io(png_ptr, in_file);
    
    // Read the PNG info
    png_read_info(png_ptr, info_ptr);

    // Get memory pointer (though not in provided list, we'll skip as not available)
    // void* mem_ptr = png_get_mem_ptr(png_ptr); // API not in provided list
    
    // Get mDCV fixed values
    png_fixed_point white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    png_uint_32 max_luminance, min_luminance;
    png_uint_32 mdcv_valid = png_get_mDCV_fixed(png_ptr, info_ptr, 
                                                &white_x, &white_y, 
                                                &red_x, &red_y, 
                                                &green_x, &green_y, 
                                                &blue_x, &blue_y,
                                                &max_luminance, &min_luminance);

    // Get y offset in microns
    png_int_32 y_offset_microns = png_get_y_offset_microns(png_ptr, info_ptr);
    
    // Get next frame y offset (for APNG)
    png_uint_32 next_frame_y_offset = png_get_next_frame_y_offset(png_ptr, info_ptr);

    // Create structures for writing to test setter APIs
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (write_ptr) {
        png_infop write_info = png_create_info_struct(write_ptr);
        if (write_info) {
            if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
                png_destroy_info_struct(write_ptr, &write_info);
                png_destroy_write_struct(&write_ptr, nullptr);
            } else {
                // Set compression level (though not in provided list, we'll use available API)
                // png_set_compression_level(write_ptr, 6); // API not in provided list
                
                // Set cHRM fixed values using values obtained from input (or defaults)
                if (mdcv_valid) {
                    png_set_cHRM_fixed(write_ptr, write_info, 
                                       white_x, white_y, 
                                       red_x, red_y, 
                                       green_x, green_y, 
                                       blue_x, blue_y);
                } else {
                    // Use default values if mDCV not present
                    png_set_cHRM_fixed(write_ptr, write_info, 
                                       31270, 32900,   // D65 white point
                                       64000, 33000,   // red
                                       30000, 60000,   // green
                                       15000, 6000);   // blue
                }
                
                png_destroy_info_struct(write_ptr, &write_info);
            }
            png_destroy_write_struct(&write_ptr, nullptr);
        }
    }

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}