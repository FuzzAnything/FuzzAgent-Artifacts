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
//<ID> 380
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 0
//<Quality> {"density":32,"unique_branches":{},"library_calls":["png_sig_cmp","png_get_libpng_ver","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_init_io","png_set_gamma","png_set_gamma","png_set_sig_bytes","png_set_read_user_transform_fn","png_set_write_user_transform_fn","png_set_user_transform_info","png_read_png","png_get_image_height","png_get_gAMA","png_get_rows","png_write_info","png_write_rows","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_get_libpng_ver","png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_init_io","png_set_gamma","png_set_gamma","png_set_sig_bytes","png_set_read_user_transform_fn","png_set_write_user_transform_fn","png_set_user_transform_info","png_read_png","png_get_image_height","png_get_gAMA","png_get_rows","png_write_info","png_write_rows","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to read and write PNG data:

*/


// User transform functions (required by png_set_read_user_transform_fn and png_set_write_user_transform_fn)
static void  id_000025_read_user_transform(png_structp png_ptr, png_row_infop row_info, png_bytep data) {
    // Simple identity transform
}

static void  id_000025_write_user_transform(png_structp png_ptr, png_row_infop row_info, png_bytep data) {
    // Simple identity transform
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    // Early return if we don't have enough data for PNG signature check
    if (size < 8) {
        return 0;
    }

    // Check PNG signature
    if (png_sig_cmp(data, fuzz_uint64_t_1, 8) != 0) {
        return 0; // Not a valid PNG signature
    }

    // Get libpng version for informational purposes
    const char *libpng_version = png_get_libpng_ver(NULL);
    // Note: png_get_header_version was listed but not in the provided APIs
    // We'll use png_get_libpng_ver which is similar
    
    // Create read structure
    png_structp png_ptr_read = png_create_read_struct(fuzz_str_2, 
                                                     NULL, NULL, NULL);
    if (!png_ptr_read) {
        return 0;
    }

    // Create info structure for reading
    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, NULL, NULL);
        return 0;
    }

    // Create write structure
    png_structp png_ptr_write = png_create_write_struct(fuzz_str_3,
                                                       NULL, NULL, NULL);
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);
        return 0;
    }

    // Create info structure for writing
    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_write_struct(&png_ptr_write, NULL);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);
        return 0;
    }

    // Set up longjmp for error handling
    jmp_buf *jmpbuf = png_set_longjmp_fn(png_ptr_read, longjmp, sizeof(jmp_buf));
    if (setjmp(*jmpbuf)) {
	FUZZ_FILE_CLOSE();
        // Error occurred during PNG processing
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);
        return 0;
    }

    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    // Create FILE* for writing output data
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize I/O for reading
    png_init_io(png_ptr_read, in_file);
    
    // Initialize I/O for writing
    png_init_io(png_ptr_write, out_file);

    // Set gamma correction for reading
    png_set_gamma(png_ptr_read, 2.2, 0.45455);
    
    // Set gamma correction for writing
    png_set_gamma(png_ptr_write, 2.2, 0.45455);

    // We've already checked 8 bytes of signature
    png_set_sig_bytes(png_ptr_read, fuzz_int32_t_4);

    // Set read user transform function
    png_set_read_user_transform_fn(png_ptr_read,  id_000025_read_user_transform);
    
    // Set write user transform function
    png_set_write_user_transform_fn(png_ptr_write,  id_000025_write_user_transform);

    // Set user transform info
    void *user_transform_ptr = NULL;
    int user_transform_depth = 0;
    int user_transform_channels = 0;
    png_set_user_transform_info(png_ptr_read, user_transform_ptr, 
                               fuzz_int32_t_5, fuzz_int32_t_6);

    // Read the entire PNG
    png_read_png(png_ptr_read, info_ptr_read, fuzz_int32_t_7, NULL);

    // Get image height
    png_uint_32 height = png_get_image_height(png_ptr_read, info_ptr_read);
    
    // Get gamma information if present
    double file_gamma;
    if (png_get_gAMA(png_ptr_read, info_ptr_read, &file_gamma)) {
        // Gamma information is available
    }

    // Get rows for writing
    png_bytepp rows = png_get_rows(png_ptr_read, info_ptr_read);
    
    // Convert a couple of bytes to demonstrate png_get_uint_16
    if (size >= 10) {
        png_uint_16 sample_value = png_get_uint_16(data + 8);
        (void)sample_value; // Prevent unused variable warning
    }

    // Write PNG info
    png_write_info(png_ptr_write, info_ptr_read);
    
    // Write rows
    png_write_rows(png_ptr_write, rows, height);
    
    // Write end
    png_write_end(png_ptr_write, info_ptr_write);

    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Destroy structures
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);

    // Remove output file since we don't need it for fuzzing
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}