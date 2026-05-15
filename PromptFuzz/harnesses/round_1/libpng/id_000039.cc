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
//<ID> 666
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"png_get_chunk_cache_max":[[1320,12,1320,19,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_read_fn","png_get_io_ptr","png_get_io_ptr","png_sig_cmp","png_destroy_read_struct","png_set_sig_bytes","png_read_info","png_get_chunk_cache_max","png_malloc","png_free_default","png_set_gAMA","png_set_error_fn","png_get_pHYs","png_set_tIME","png_image_begin_read_from_file","png_image_free","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_read_fn","png_get_io_ptr","png_get_io_ptr","png_sig_cmp","png_set_sig_bytes","png_read_info","png_get_chunk_cache_max","png_malloc","png_free_default","png_set_gAMA","png_set_error_fn","png_get_pHYs","png_set_tIME","png_image_begin_read_from_file","png_image_free","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 8) {
        return 0; // Not enough data for PNG signature
    }

    // Create input file from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize PNG structures for reading
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
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

    png_set_read_fn(png_ptr, in_file, [](png_structp png_ptr, png_bytep out, png_size_t length) {
        FILE *fp = (FILE*)png_get_io_ptr(png_ptr);
        fread(out, 1, length, fp);
    });

    // Check PNG signature
    png_byte sig[8];
    fread(sig, 1, 8, in_file);
    if (png_sig_cmp(sig, fuzz_uint64_t_2, 8) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_set_sig_bytes(png_ptr, fuzz_int32_t_3);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // 1. Get chunk cache max
    png_uint_32 chunk_cache_max = png_get_chunk_cache_max(png_ptr);

    // 2. Allocate and free memory using default allocator
    void *temp_buffer = png_malloc(png_ptr, 1024);
    png_free_default(png_ptr, temp_buffer);

    // 3. Set gamma value (2.2 is typical for computer graphics)
    png_set_gAMA(png_ptr, info_ptr, 2.2);

    // 4. Prepare for potential error
    const char *error_msg = "Test error";
    // Note: png_error would longjmp, so we don't call it directly here
    // but we demonstrate its availability by setting error function
    png_set_error_fn(png_ptr, nullptr, 
        [](png_structp png_ptr, png_const_charp msg) {
            // Error handler - would normally longjmp
        },
        [](png_structp png_ptr, png_const_charp msg) {
            // Warning handler
        });

    // 5. Get physical dimensions (pHYs chunk)
    png_uint_32 res_x = 0, res_y = 0;
    int unit_type = 0;
    png_get_pHYs(png_ptr, info_ptr, &res_x, &res_y, &unit_type);

    // 6. Set modification time
    png_time mod_time;
    time_t current_time;
    time(&current_time);
    struct tm *timeinfo = localtime(&current_time);
    
    mod_time.year = (png_uint_16)(timeinfo->tm_year + 1900);
    mod_time.month = (png_byte)(timeinfo->tm_mon + 1);
    mod_time.day = (png_byte)timeinfo->tm_mday;
    mod_time.hour = (png_byte)timeinfo->tm_hour;
    mod_time.minute = (png_byte)timeinfo->tm_min;
    mod_time.second = (png_byte)timeinfo->tm_sec;
    
    png_set_tIME(png_ptr, info_ptr, &mod_time);

    // 7. Use high-level image reading API
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
	FUZZ_FILENAME_PUSH("input_file")
    
    // Try to read from file (will likely fail with fuzzed data, but API is called)
    int result = png_image_begin_read_from_file(&image, "input_file");

    // Cleanup image if it was partially initialized
    if (result != 0) {
        png_image_free(&image);
    }

    // Clean up resources
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}