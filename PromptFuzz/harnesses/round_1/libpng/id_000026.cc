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
//<ID> 381
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 5
//<Quality> {"density":30,"unique_branches":{"png_create_read_struct_2":[[50,8,50,23,0,0,4,1]],"png_create_png_struct":[[326,14,326,71,0,0,4,1]],"png_user_version_check":[[218,14,218,57,0,0,4,0],[223,13,223,42,0,0,4,1],[229,8,229,57,0,0,4,0]]},"library_calls":["png_sig_cmp","png_get_header_version","png_create_read_struct","png_destroy_read_struct","png_set_longjmp_fn","png_create_info_struct","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_set_gamma","png_set_read_user_transform_fn","png_set_user_transform_info","png_read_png","png_get_gAMA","png_create_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_get_image_height","png_get_rows","png_write_info","png_set_write_user_transform_fn","png_write_rows","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_get_header_version","png_create_read_struct","png_set_longjmp_fn","png_create_info_struct","png_init_io","png_set_sig_bytes","png_set_gamma","png_set_read_user_transform_fn","png_set_user_transform_info","png_read_png","png_get_gAMA","png_create_write_struct","png_set_longjmp_fn","png_init_io","png_get_image_height","png_get_rows","png_write_info","png_set_write_user_transform_fn","png_write_rows","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"visited":1}
/**/


// Dummy user transform functions
void  id_000026_dummy_read_transform(png_structp png_ptr, png_row_infop row_info, png_bytep data) {}
void  id_000026_dummy_write_transform(png_structp png_ptr, png_row_infop row_info, png_bytep data) {}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    // Check minimum size for PNG signature
    if (size < 8) {
	return 0;
	}
	
    
    // Check PNG signature
    if (png_sig_cmp((png_bytep)data, fuzz_uint64_t_1, 8) != 0) {
        return 0; // Not a PNG file
    }
    
    // Get libpng version
    const char *version = png_get_header_version(NULL);
    if (version == NULL) {
	return 0;
	}
	
    
    // Create read structure
    png_structp png_read_ptr = png_create_read_struct(
        version, NULL, NULL, NULL);
    if (png_read_ptr == NULL) {
	return 0;
	}
	
    
    // Set longjmp for error handling
    jmp_buf jmpbuf;
    if (setjmp(jmpbuf)) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_read_ptr, NULL, NULL);
        return 0;
    }
    png_set_longjmp_fn(png_read_ptr, longjmp, sizeof(jmp_buf));
    
    // Create info structure for reading
    png_infop read_info_ptr = png_create_info_struct(png_read_ptr);
    if (read_info_ptr == NULL) {
        png_destroy_read_struct(&png_read_ptr, NULL, NULL);
        return 0;
    }
    
    // Open input from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (in_file == NULL) {
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, NULL);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O
    png_init_io(png_read_ptr, in_file);
    
    // Set signature bytes we already read
    png_set_sig_bytes(png_read_ptr, fuzz_int32_t_2);
    
    // Set gamma correction
    png_set_gamma(png_read_ptr, 2.2, 0.45455);
    
    // Set user transform functions
    png_set_read_user_transform_fn(png_read_ptr,  id_000026_dummy_read_transform);
    void *user_transform_ptr = NULL;
    png_set_user_transform_info(png_read_ptr, user_transform_ptr, fuzz_int32_t_3, fuzz_int32_t_4);
    
    // Read PNG info and data
    png_read_png(png_read_ptr, read_info_ptr, fuzz_int32_t_5, NULL);
    
    // Get gamma value if present
    double file_gamma = 0.0;
    if (png_get_gAMA(png_read_ptr, read_info_ptr, &file_gamma)) {
        // Gamma retrieved successfully
    }
    
    // Create write structure
    png_structp png_write_ptr = png_create_write_struct(
        version, NULL, NULL, NULL);
    if (png_write_ptr == NULL) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, NULL);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set longjmp for write error handling
    if (setjmp(jmpbuf)) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&png_write_ptr, NULL);
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, NULL);
        assert_file_closed(&in_file);
	return 0;
    }
    png_set_longjmp_fn(png_write_ptr, longjmp, sizeof(jmp_buf));
	FUZZ_FILENAME_PUSH("output_file")
    
    // Open output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file == NULL) {
        png_destroy_write_struct(&png_write_ptr, NULL);
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, NULL);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize write I/O
    png_init_io(png_write_ptr, out_file);
    
    // Get image dimensions
    png_uint_32 height = png_get_image_height(png_read_ptr, read_info_ptr);
    
    // Get rows for writing
    png_bytepp rows = png_get_rows(png_read_ptr, read_info_ptr);
    
    // Write PNG info
    png_write_info(png_write_ptr, read_info_ptr);
    
    // Set write user transform
    png_set_write_user_transform_fn(png_write_ptr,  id_000026_dummy_write_transform);
    
    // Write rows
    png_write_rows(png_write_ptr, rows, height);
    
    // Test png_get_uint_16 with first 2 bytes of data
    if (size >= 2) {
        png_uint_16 test_value = png_get_uint_16((png_bytep)data);
        (void)test_value; // Suppress unused variable warning
    }
    
    // Write end
    png_write_end(png_write_ptr, read_info_ptr);
    
    // Cleanup
    png_destroy_write_struct(&png_write_ptr, NULL);
    png_destroy_read_struct(&png_read_ptr, &read_info_ptr, NULL);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}