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
//<ID> 147
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 2
//<Quality> {"density":11,"unique_branches":{"png_set_packing":[[53,8,53,23,0,0,4,1],[56,8,56,30,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_packing","png_set_shift","png_read_info","png_read_frame_head","png_get_eXIf_1","png_destroy_read_struct","png_create_write_struct","png_create_info_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_write_struct","png_init_io","png_set_IHDR","png_write_info","png_write_row","png_write_end","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_packing","png_set_shift","png_read_info","png_read_frame_head","png_get_eXIf_1","png_destroy_read_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_write_info","png_write_row","png_write_end","png_destroy_write_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create input FILE* from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize libpng structures for reading
    png_struct *png_ptr_read = png_create_read_struct(fuzz_str_1, 
                                                      nullptr, nullptr, nullptr);
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

    // Set up reading from FILE*
    png_init_io(png_ptr_read, in_file);

    // Set packing transformation
    png_set_packing(png_ptr_read);

    // Set shift transformation with dummy values
    png_color_8 shift_bits = {8, 8, 8, 8, 8};
    png_set_shift(png_ptr_read, &shift_bits);

    // Read PNG info
    png_read_info(png_ptr_read, info_ptr_read);

    // Try to read frame head (for APNG)
    png_read_frame_head(png_ptr_read, info_ptr_read);

    // Try to get EXIF data
    png_uint_32 num_exif = 0;
    png_byte *exif_data = nullptr;
    png_get_eXIf_1(png_ptr_read, info_ptr_read, &num_exif, &exif_data);

    // Clean up read structures
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    assert_file_closed(&in_file);;
	FUZZ_FILENAME_PUSH("output_file")

    // Create output FILE* for writing
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize libpng structures for writing
    png_struct *png_ptr_write = png_create_write_struct(fuzz_str_2,
                                                       nullptr, nullptr, nullptr);
    if (!png_ptr_write) {
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_info *info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_write_struct(&png_ptr_write, nullptr);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling for writing
    if (setjmp(png_jmpbuf(png_ptr_write))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up writing to FILE*
    png_init_io(png_ptr_write, out_file);

    // Set some basic image parameters for writing (dummy values)
    png_set_IHDR(png_ptr_write, info_ptr_write, 
                 fuzz_uint32_t_3, 100, fuzz_int32_t_4, fuzz_int32_t_5, 
                 fuzz_int32_t_6, fuzz_int32_t_7, 
                 fuzz_int32_t_8);

    // Write PNG info
    png_write_info(png_ptr_write, info_ptr_write);

    // Create dummy row for writing
    png_byte row[300]; // 100 pixels * 3 bytes per pixel
    memset(row, 0, sizeof(row));

    // Write a single row
    png_write_row(png_ptr_write, row);

    // Write end of PNG
    png_write_end(png_ptr_write, info_ptr_write);

    // Clean up write structures
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}