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
//<ID> 224
//<Prompt> []
/*<Combination>: [
*/
//<score> 86.666664, nr_unique_branch: 64
//<Quality> {"density":26,"unique_branches":{"png_set_first_frame_is_hidden":[[1466,8,1466,17,0,0,4,0]],"png_rtran_ok":[[106,29,106,65,0,0,4,1]],"png_write_image_8bit":[[1785,28,1785,72,0,0,4,0],[1785,28,1785,72,0,0,4,1],[1788,8,1788,52,0,0,4,0],[1788,8,1788,52,0,0,4,1],[1794,11,1794,56,0,0,4,1],[1808,14,1808,19,0,0,4,0],[1808,14,1808,19,0,0,4,1],[1813,17,1813,34,0,0,4,0],[1813,17,1813,34,0,0,4,1],[1823,17,1823,30,0,0,4,0],[1823,17,1823,30,0,0,4,1],[1823,34,1823,49,0,0,4,0],[1823,34,1823,49,0,0,4,1],[1829,20,1829,27,0,0,4,0],[1829,20,1829,27,0,0,4,1],[1849,14,1849,19,0,0,4,0],[1849,14,1849,19,0,0,4,1],[1854,17,1854,34,0,0,4,0],[1854,17,1854,34,0,0,4,1]],"png_do_expand":[[4619,20,4619,47,0,0,4,1],[4655,59,4655,72,0,0,4,1],[4679,17,4679,37,0,0,4,1],[4680,17,4680,40,0,0,4,1],[4681,17,4681,39,0,0,4,1],[4682,17,4682,39,0,0,4,1],[4683,17,4683,38,0,0,4,1]],"png_unpremultiply":[[1742,8,1742,26,0,0,4,0],[1742,8,1742,26,0,0,4,1],[1742,30,1742,41,0,0,4,0],[1742,30,1742,41,0,0,4,1],[1748,13,1748,26,0,0,4,0],[1748,13,1748,26,0,0,4,1],[1755,11,1755,24,0,0,4,0],[1755,11,1755,24,0,0,4,1]],"png_image_write_main":[[2030,32,2030,63,0,0,4,1],[2209,38,2209,67,0,0,4,0],[2216,11,2216,27,0,0,4,1]],"png_image_finish_read":[[4231,20,4231,67,0,0,4,1],[4232,20,4232,47,0,0,4,0],[4232,51,4232,67,0,0,4,1],[3042,5,3042,37,7,0,4,0]],"png_set_rgb_to_gray_fixed":[[1039,7,1039,33,0,0,4,0],[1055,8,1055,53,0,0,4,0],[1055,8,1055,53,0,0,4,1],[1070,11,1070,19,0,0,4,0],[1070,23,1070,33,0,0,4,0],[1070,37,1070,60,0,0,4,0]],"png_set_crc_action":[[28,8,28,23,0,0,4,1],[34,7,34,29,0,0,4,1],[37,7,37,28,0,0,4,1],[42,7,42,29,0,0,4,1],[48,7,48,32,0,0,4,1],[52,7,52,30,0,0,4,0],[54,7,54,27,0,0,4,1],[55,7,55,14,0,0,4,1],[63,7,63,29,0,0,4,1],[66,7,66,28,0,0,4,0],[71,7,71,29,0,0,4,1],[77,7,77,30,0,0,4,1],[82,7,82,32,0,0,4,1],[84,7,84,27,0,0,4,1],[85,7,85,14,0,0,4,1]]},"library_calls":["png_image_begin_read_from_memory","png_image_free","png_create_read_struct","png_image_free","png_create_info_struct","png_destroy_read_struct","png_image_free","png_set_longjmp_fn","png_destroy_read_struct","png_image_free","png_init_io","png_read_info","png_set_crc_action","png_set_rgb_to_gray_fixed","png_set_first_frame_is_hidden","png_calloc","png_destroy_read_struct","png_image_free","png_image_finish_read","png_free","png_destroy_read_struct","png_image_free","png_image_write_to_stdio","png_free","png_destroy_read_struct","png_image_free","png_image_free"],"critical_calls":["png_image_begin_read_from_memory","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_set_crc_action","png_set_rgb_to_gray_fixed","png_set_first_frame_is_hidden","png_calloc","png_image_finish_read","png_image_write_to_stdio","png_free","png_destroy_read_struct","png_image_free","png_image_free"],"visited":2}
/*Here's a C++ fuzz driver that uses all the specified libpng APIs:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        return 0;
    }

    // Create a FILE pointer for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    // Create a FILE pointer for writing output data
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize png_image structure for reading
    png_image image_read;
    memset(&image_read, 0, sizeof(image_read));
    image_read.version = PNG_IMAGE_VERSION;

    // Read image from memory
    int read_result = png_image_begin_read_from_memory(&image_read, data, size);
    if (!read_result) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_image_free(&image_read);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Allocate buffer for image data using png_calloc
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_image_free(&image_read);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_image_free(&image_read);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_image_free(&image_read);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, in_file);

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // Set CRC action
    png_set_crc_action(png_ptr, PNG_CRC_ERROR_QUIT, PNG_CRC_WARN_USE);

    // Set RGB to gray conversion with fixed point parameters
    png_set_rgb_to_gray_fixed(png_ptr, PNG_ERROR_ACTION_NONE, 29900, 58700);

    // Set first frame as hidden (for APNG)
    png_byte is_hidden = 0;
    if (size > 200) {
        is_hidden = data[100] % 2;
    }
    png_set_first_frame_is_hidden(png_ptr, info_ptr, is_hidden);

    // Allocate memory for image buffer using png_calloc
    png_alloc_size_t buffer_size = PNG_IMAGE_SIZE(image_read);
    void *buffer = png_calloc(png_ptr, buffer_size);
    if (!buffer) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_image_free(&image_read);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Finish reading the image
    png_int_32 row_stride = 0;
    void *colormap = NULL;
    int finish_result = png_image_finish_read(&image_read, NULL, buffer, row_stride, colormap);
    
    if (!finish_result) {
        png_free(png_ptr, buffer);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_image_free(&image_read);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create png_image structure for writing
    png_image image_write;
    memset(&image_write, 0, sizeof(image_write));
    image_write.version = PNG_IMAGE_VERSION;
    image_write.width = image_read.width;
    image_write.height = image_read.height;
    image_write.format = image_read.format;

    // Write image to stdio
    int convert_to_8_bit = 0;
    if (size > 150) {
        convert_to_8_bit = data[120] % 2;
    }
    
    int write_result = png_image_write_to_stdio(&image_write, out_file, convert_to_8_bit, 
                                                buffer, row_stride, colormap);

    // Clean up
    png_free(png_ptr, buffer);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    png_image_free(&image_read);
    png_image_free(&image_write);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}