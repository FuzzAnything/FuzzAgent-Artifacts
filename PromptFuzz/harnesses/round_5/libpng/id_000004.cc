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
//<ID> 76
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":21,"unique_branches":{"png_chunk_benign_error":[[456,8,456,59,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_x_offset_pixels","png_get_sCAL","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_set_iCCP","png_chunk_benign_error","png_get_mem_ptr","png_destroy_write_struct","png_get_IHDR","png_malloc","png_get_rowbytes","png_malloc","png_read_image","png_read_end","png_free","png_free","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_x_offset_pixels","png_get_sCAL","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_iCCP","png_chunk_benign_error","png_get_mem_ptr","png_destroy_write_struct","png_get_IHDR","png_malloc","png_get_rowbytes","png_malloc","png_read_image","png_read_end","png_free","png_free","png_destroy_read_struct"],"visited":2}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to process PNG data:

*/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE pointer from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize libpng structures for reading
    png_struct *png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr_read) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_info *info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr_read))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize I/O for reading
    png_init_io(png_ptr_read, in_file);
    
    // Read PNG info
    png_read_info(png_ptr_read, info_ptr_read);

    // 1. Get x offset in pixels
    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr_read, info_ptr_read);
    
    // 2. Get sCAL chunk if present
    int unit = 0;
    double width = 0.0, height = 0.0;
    png_uint_32 scal_result = png_get_sCAL(png_ptr_read, info_ptr_read, &unit, &width, &height);

    // Create structures for writing to set iCCP
    png_struct *png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_info *info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_write_struct(&png_ptr_write, nullptr);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling for write
    if (setjmp(png_jmpbuf(png_ptr_write))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Set iCCP profile using part of input data as profile
    // Use a portion of the input data as ICC profile (ensure we don't overflow)
    size_t profile_size = size > 100 ? 100 : size;
    const char *profile_name = "TestProfile";
    int compression_type = 0; // PNG_COMPRESSION_TYPE_BASE
    
    png_set_iCCP(png_ptr_write, info_ptr_write, profile_name, 
                 compression_type, data, (png_uint_32)profile_size);

    // 4. Report a benign error (simulating a non-critical chunk error)
    png_chunk_benign_error(png_ptr_write, "Simulated benign chunk error during fuzzing");

    // 5. Get memory pointer from read structure
    void *mem_ptr = png_get_mem_ptr(png_ptr_read);

    // Clean up write structures
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);

    // Read the image data to process the PNG fully
    png_bytepp row_pointers = nullptr;
    png_uint_32 width_val, height_val;
    int bit_depth, color_type, interlace_type;
    
    png_get_IHDR(png_ptr_read, info_ptr_read, &width_val, &height_val, 
                 &bit_depth, &color_type, &interlace_type, nullptr, nullptr);

    // Allocate memory for row pointers
    row_pointers = (png_bytepp)png_malloc(png_ptr_read, height_val * sizeof(png_bytep));
    
    // Calculate row size
    png_size_t rowbytes = png_get_rowbytes(png_ptr_read, info_ptr_read);
    
    for (png_uint_32 row = 0; row < height_val; row++) {
        row_pointers[row] = (png_bytep)png_malloc(png_ptr_read, rowbytes);
    }
    
    // Read the image
    png_read_image(png_ptr_read, row_pointers);
    
    // Read the end of PNG
    png_read_end(png_ptr_read, nullptr);

    // Clean up row pointers
    for (png_uint_32 row = 0; row < height_val; row++) {
        png_free(png_ptr_read, row_pointers[row]);
    }
    png_free(png_ptr_read, row_pointers);

    // Clean up read structures
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    
    // Close the input file
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}