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
//<ID> 109
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"png_set_keep_unknown_chunks":[[1709,11,1709,29,0,0,4,0]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_set_keep_unknown_chunks","png_read_info","png_get_acTL","png_set_progressive_read_fn","png_process_data","png_process_data_pause","png_get_x_offset_microns","png_get_libpng_ver","png_read_image","png_read_end","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_set_keep_unknown_chunks","png_read_info","png_get_acTL","png_set_progressive_read_fn","png_process_data","png_process_data_pause","png_get_x_offset_microns","png_get_libpng_ver","png_read_image","png_read_end","png_destroy_read_struct"],"visited":1}
/**/


// Include libpng headers

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
        return 0; // Too small for PNG header
    }

    // Check PNG signature (first 8 bytes)
    if (png_sig_cmp((png_bytep)data, 0, 8) != 0) {
        return 0; // Not a valid PNG
    }

    // Create file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                  nullptr, nullptr, nullptr);
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

    // Initialize I/O
    png_init_io(png_ptr, in_file);
    png_set_sig_bytes(png_ptr, 8); // We already checked 8 bytes

    // 1. Call png_set_keep_unknown_chunks first (before reading)
    png_set_keep_unknown_chunks(png_ptr, PNG_HANDLE_CHUNK_ALWAYS, nullptr, 0);

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // 2. Call png_get_acTL to get animation info (for APNG files)
    png_uint_32 num_frames = 0, num_plays = 0;
    png_get_acTL(png_ptr, info_ptr, &num_frames, &num_plays);

    // 3. Call png_process_data_pause - using it to pause processing
    // Since we're reading from file, we need to set up progressive reader first
    png_set_progressive_read_fn(png_ptr, info_ptr, 
                                nullptr, nullptr, nullptr);
    
    // Process some data to set up for pause
    png_process_data(png_ptr, info_ptr, (png_bytep)data, 8);
    png_process_data_pause(png_ptr, 0); // Call pause with save=0

    // 4. Call png_get_x_offset_microns
    png_int_32 x_offset_microns = png_get_x_offset_microns(png_ptr, info_ptr);

    // 5. Call png_get_libpng_ver
    const char *libpng_version = png_get_libpng_ver(png_ptr);

    // Read the rest of the PNG data
    png_read_image(png_ptr, nullptr);
    png_read_end(png_ptr, nullptr);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}