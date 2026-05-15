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
//<ID> 316
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 1
//<Quality> {"density":39,"unique_branches":{"png_read_info":[[139,16,139,52,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_init_io","png_destroy_read_struct","png_destroy_write_struct","png_sig_cmp","png_destroy_read_struct","png_destroy_write_struct","png_set_sig_bytes","png_set_read_user_transform_fn","png_set_user_transform_info","png_read_info","png_get_image_height","png_get_gAMA","png_read_png","png_get_IHDR","png_set_IHDR","png_get_rows","png_write_info","png_write_rows","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_init_io","png_sig_cmp","png_set_sig_bytes","png_set_read_user_transform_fn","png_set_user_transform_info","png_read_info","png_get_image_height","png_get_gAMA","png_read_png","png_get_IHDR","png_set_IHDR","png_get_rows","png_write_info","png_write_rows","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to read a PNG from memory, apply transformations, and write it to a file:

*/


// Include necessary headers for libpng

// Dummy user transform function
void  id_000020_dummy_user_transform(png_structp png_ptr, png_row_infop row_info, png_bytep data) {
    // This is a dummy transform function that does nothing
    // It's required by png_set_read_user_transform_fn
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    // Early exit if data is too small for PNG signature
    if (size < 8) {
        return 0;
    }

    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    // Create FILE* for output data
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create read struct
    png_structp read_ptr = png_create_read_struct(fuzz_str_1, 
                                                  nullptr, nullptr, nullptr);
    if (!read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create info struct for reading
    png_infop read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create write struct
    png_structp write_ptr = png_create_write_struct(fuzz_str_2,
                                                   nullptr, nullptr, nullptr);
    if (!write_ptr) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create info struct for writing
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling with setjmp/longjmp
    if (setjmp(png_jmpbuf(read_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Setup longjmp function for read struct
    jmp_buf *read_jmp = png_set_longjmp_fn(read_ptr, longjmp, sizeof(jmp_buf));
    if (!read_jmp) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Setup longjmp function for write struct
    jmp_buf *write_jmp = png_set_longjmp_fn(write_ptr, longjmp, sizeof(jmp_buf));
    if (!write_jmp) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize IO for reading
    png_init_io(read_ptr, in_file);
    
    // Initialize IO for writing
    png_init_io(write_ptr, out_file);

    // Check PNG signature
    png_byte sig[8];
    if (fread(sig, 1, 8, in_file) != 8) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    if (png_sig_cmp(sig, fuzz_uint64_t_3, 8)) {
        // Not a PNG file
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set the number of bytes we've already read
    png_set_sig_bytes(read_ptr, fuzz_int32_t_4);

    // Setup user transform functions for reading
    png_set_read_user_transform_fn(read_ptr,  id_000020_dummy_user_transform);
    
    // Setup user transform info
    png_bytep user_transform_ptr = nullptr;
    int user_transform_depth = 8;
    int user_transform_channels = 4;
    png_set_user_transform_info(read_ptr, user_transform_ptr, 
                                fuzz_int32_t_5, fuzz_int32_t_6);

    // Read PNG info
    png_read_info(read_ptr, read_info);

    // Get image height
    png_uint_32 height = png_get_image_height(read_ptr, read_info);
    
    // Get gamma value if available
    double file_gamma;
    if (png_get_gAMA(read_ptr, read_info, &file_gamma)) {
        // Gamma value retrieved successfully
    }

    // Transform the image (apply default transformations)
    png_read_png(read_ptr, read_info, fuzz_int32_t_7, nullptr);

    // Setup for writing - copy relevant info from read struct
    // Get image width and other parameters
    png_uint_32 width;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(read_ptr, read_info, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);

    // Set IHDR for writing
    png_set_IHDR(write_ptr, write_info, width, height, bit_depth, color_type,
                 interlace_method, compression_method, filter_method);

    // Get rows from read struct
    png_bytepp row_pointers = png_get_rows(read_ptr, read_info);

    // Write PNG info
    png_write_info(write_ptr, write_info);

    // Write rows
    png_write_rows(write_ptr, row_pointers, height);

    // Write end
    png_write_end(write_ptr, write_info);

    // Clean up
    png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    png_destroy_write_struct(&write_ptr, &write_info);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Remove output file to avoid accumulating files during fuzzing
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}