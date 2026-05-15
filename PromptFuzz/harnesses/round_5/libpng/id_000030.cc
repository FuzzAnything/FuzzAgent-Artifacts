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
//<ID> 379
//<Prompt> []
/*<Combination>: [
*/
//<score> 33, nr_unique_branch: 0
//<Quality> {"density":33,"unique_branches":{},"library_calls":["png_create_read_struct","png_get_chunk_malloc_max","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_next_frame_delay_den","png_get_uint_31","png_benign_error","png_get_sCAL_fixed","png_read_update_info","png_get_image_width","png_get_image_height","png_get_rowbytes","png_malloc","png_malloc","png_set_rows","png_read_image","png_read_end","png_create_write_struct","png_free","png_free","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct","png_init_io","png_write_info_before_PLTE","png_save_uint_16","png_free","png_free","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_get_chunk_malloc_max","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_next_frame_delay_den","png_get_uint_31","png_benign_error","png_get_sCAL_fixed","png_read_update_info","png_get_image_width","png_get_image_height","png_get_rowbytes","png_malloc","png_malloc","png_set_rows","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_write_info_before_PLTE","png_save_uint_16","png_free","png_free","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    // Create FILE* for writing output data
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Create read structure
    png_struct *png_read_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Get chunk malloc max (reading)
    png_alloc_size_t chunk_malloc_max = png_get_chunk_malloc_max(png_read_ptr);

    // Step 3: Create info structure for reading
    png_info *read_info_ptr = png_create_info_struct(png_read_ptr);
    if (!read_info_ptr) {
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 4: Set up error handling with longjmp
    jmp_buf jmpbuf;
    jmp_buf *jmpbuf_ptr = png_set_longjmp_fn(png_read_ptr, longjmp, sizeof(jmp_buf));
    if (setjmp(*jmpbuf_ptr)) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Initialize IO for reading
    png_init_io(png_read_ptr, in_file);

    // Step 6: Read PNG info
    png_read_info(png_read_ptr, read_info_ptr);

    // Step 7: Get APNG frame delay denominator (if applicable)
    png_uint_16 frame_delay_den = png_get_next_frame_delay_den(png_read_ptr, read_info_ptr);

    // Step 8: Use png_get_uint_31 on a buffer (use first 4 bytes of data)
    png_uint_32 uint31_val = 0;
    if (size >= 4) {
        uint31_val = png_get_uint_31(png_read_ptr, (const png_byte*)data);
    }

    // Step 9: Set a benign error handler
    png_benign_error(png_read_ptr, "Fuzzer benign error");

    // Step 10: Get sCAL chunk data if present
    int unit = 0;
    png_fixed_point width_fixed = 0, height_fixed = 0;
    png_uint_32 scal_result = png_get_sCAL_fixed(png_read_ptr, read_info_ptr, 
                                                  &unit, &width_fixed, &height_fixed);

    // Step 11: Update info after potential transformations
    png_read_update_info(png_read_ptr, read_info_ptr);

    // Step 12: Get image dimensions
    png_uint_32 width = png_get_image_width(png_read_ptr, read_info_ptr);
    png_uint_32 height = png_get_image_height(png_read_ptr, read_info_ptr);

    // Step 13: Get rowbytes for allocation
    size_t rowbytes = png_get_rowbytes(png_read_ptr, read_info_ptr);

    // Step 14: Allocate memory for image rows
    png_bytepp row_pointers = (png_bytepp)png_malloc(png_read_ptr, 
                                                      height * sizeof(png_bytep));
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)png_malloc(png_read_ptr, rowbytes);
    }

    // Step 15: Set rows in info structure
    png_set_rows(png_read_ptr, read_info_ptr, row_pointers);

    // Step 16: Read the image data
    png_read_image(png_read_ptr, row_pointers);

    // Step 17: Read end of PNG
    png_read_end(png_read_ptr, read_info_ptr);

    // Step 18: Create write structure
    png_struct *png_write_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_write_ptr) {
        // Cleanup reading structures first
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_read_ptr, row_pointers[y]);
        }
        png_free(png_read_ptr, row_pointers);
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 19: Create info structure for writing
    png_info *write_info_ptr = png_create_info_struct(png_write_ptr);
    if (!write_info_ptr) {
        png_destroy_write_struct(&png_write_ptr, nullptr);
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_read_ptr, row_pointers[y]);
        }
        png_free(png_read_ptr, row_pointers);
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling for writing
    if (setjmp(png_jmpbuf(png_write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_read_ptr, row_pointers[y]);
        }
        png_free(png_read_ptr, row_pointers);
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize IO for writing
    png_init_io(png_write_ptr, out_file);

    // Step 20: Write info before PLTE (for writing path)
    png_write_info_before_PLTE(png_write_ptr, write_info_ptr);

    // Step 21: Use png_save_uint_16 to save a value
    png_byte saved_uint16_buffer[2];
    png_save_uint_16(saved_uint16_buffer, 1000); // Save arbitrary value

    // Cleanup - Free allocated memory
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_read_ptr, row_pointers[y]);
    }
    png_free(png_read_ptr, row_pointers);

    // Destroy read structures
    png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);

    // Destroy write structures
    png_destroy_write_struct(&png_write_ptr, &write_info_ptr);

    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}