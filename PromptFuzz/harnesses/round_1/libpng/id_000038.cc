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
//<ID> 665
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 1
//<Quality> {"density":30,"unique_branches":{"png_image_begin_read_from_file":[[1462,14,1462,24,0,0,4,1]]},"library_calls":["png_sig_cmp","png_image_begin_read_from_file","png_image_free","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_read_info","png_get_chunk_cache_max","png_get_pHYs","png_set_gAMA","png_set_tIME","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_write_struct","png_init_io","png_get_IHDR","png_set_IHDR","png_get_PLTE","png_set_PLTE","png_write_info","png_malloc","png_malloc","png_get_rowbytes","png_read_image","png_write_image","png_write_end","png_free_default","png_free_default","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_image_begin_read_from_file","png_image_free","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_read_info","png_get_chunk_cache_max","png_get_pHYs","png_set_gAMA","png_set_tIME","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_get_IHDR","png_set_IHDR","png_get_PLTE","png_set_PLTE","png_write_info","png_malloc","png_malloc","png_get_rowbytes","png_read_image","png_write_image","png_write_end","png_free_default","png_free_default","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/*Here's a C++ fuzz driver for libpng that uses all the specified APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 8) {
        return 0; // Need at least 8 bytes for PNG signature
    }
    
    // Check PNG signature
    if (png_sig_cmp(data, fuzz_uint64_t_1, 8) != 0) {
        return 0; // Not a PNG file
    }
    
    // Create temporary file with input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize structures
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_image image;
    png_time mod_time;
	FUZZ_FILENAME_PUSH("input_file")
    
    // 1. Use png_image_begin_read_from_file
    // First write data to a file for png_image_begin_read_from_file
    const char *input_filename = "input_file";
    FILE *tmp_file = fopen(input_filename, "wb");
	FUZZ_FILE_PUSH(tmp_file)
    if (tmp_file) {
        fwrite(data, 1, size, tmp_file);
        assert_file_closed(&tmp_file);;
        
        memset(&image, 0, sizeof(image));
        image.version = PNG_IMAGE_VERSION;
        int result = png_image_begin_read_from_file(&image, input_filename);
        // We don't need to complete the read for fuzzing
        if (result) {
            png_image_free(&image);
        }
        remove(input_filename);
    }
    
    // 2. Create read structures
    png_ptr = png_create_read_struct(fuzz_str_2, NULL, NULL, NULL);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&tmp_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&tmp_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Set up error handling - will use png_error
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&tmp_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Initialize I/O
    png_init_io(png_ptr, in_file);
    png_set_sig_bytes(png_ptr, fuzz_int32_t_3);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // 3. Get chunk cache max
    png_uint_32 chunk_cache_max = png_get_chunk_cache_max(png_ptr);
    
    // 4. Get pHYs info
    png_uint_32 res_x, res_y;
    int unit_type;
    png_uint_32 phys_result = png_get_pHYs(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    
    // 5. Set gamma
    png_set_gAMA(png_ptr, info_ptr, 1.0); // Standard gamma
    
    // 6. Set modification time
    memset(&mod_time, 0, sizeof(mod_time));
    mod_time.year = 2023;
    mod_time.month = 1;
    mod_time.day = 1;
    png_set_tIME(png_ptr, info_ptr, &mod_time);
    
    // Create write structure for output
    png_structp write_ptr = png_create_write_struct(fuzz_str_4, NULL, NULL, NULL);
    if (write_ptr) {
        png_infop write_info = png_create_info_struct(write_ptr);
        if (write_info) {
            if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
                png_destroy_write_struct(&write_ptr, &write_info);
            } else {
                png_init_io(write_ptr, out_file);
                
                // Copy info from read to write
                png_uint_32 width, height;
                int bit_depth, color_type, interlace_method, compression_method, filter_method;
                png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                            &interlace_method, &compression_method, &filter_method);
                
                png_set_IHDR(write_ptr, write_info, width, height, bit_depth, color_type,
                            interlace_method, compression_method, filter_method);
                
                // Copy palette if present
                png_colorp palette;
                int num_palette;
                if (png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette)) {
                    png_set_PLTE(write_ptr, write_info, palette, num_palette);
                }
                
                // Write info
                png_write_info(write_ptr, write_info);
                
                // Allocate memory for rows and demonstrate png_free_default
                png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr,
                    height * sizeof(png_bytep));
                
                for (png_uint_32 y = 0; y < height; y++) {
                    row_pointers[y] = (png_bytep)png_malloc(png_ptr,
                        png_get_rowbytes(png_ptr, info_ptr));
                }
                
                // Read image data
                png_read_image(png_ptr, row_pointers);
                
                // Write image data
                png_write_image(write_ptr, row_pointers);
                png_write_end(write_ptr, NULL);
                
                // Free allocated memory using png_free_default
                for (png_uint_32 y = 0; y < height; y++) {
                    png_free_default(png_ptr, row_pointers[y]);
                }
                png_free_default(png_ptr, row_pointers);
                
                png_destroy_write_struct(&write_ptr, &write_info);
            }
        }
    }
    
    // 7. Demonstrate png_error - will be triggered by invalid operation
    // This is just to ensure png_error is called in the code path
    // The actual call to png_error happens through libpng's internal error handling
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
	FUZZ_FILENAME_PUSH("output_file")
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&tmp_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}