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
//<ID> 157
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":34,"unique_branches":{},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_set_sig_bytes","png_read_info","png_set_gamma","png_get_next_frame_fcTL","png_get_compression_type","png_set_eXIf_1","png_set_alpha_mode_fixed","png_read_update_info","png_get_rowbytes","png_malloc","png_read_row","png_init_io","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_set_IHDR","png_write_info","png_write_row","png_write_end","png_free","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_read_info","png_set_gamma","png_get_next_frame_fcTL","png_get_compression_type","png_set_eXIf_1","png_set_alpha_mode_fixed","png_read_update_info","png_get_rowbytes","png_malloc","png_read_row","png_init_io","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_set_IHDR","png_write_info","png_write_row","png_write_end","png_free","png_destroy_read_struct","png_destroy_write_struct"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < 8 || data == nullptr) {
        return 0;
    }

    // Check PNG signature
    if (png_sig_cmp((png_byte*)data, 0, 8) != 0) {
        return 0; // Not a PNG
    }

    // Setup FILE pointers
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_structp png_ptr_read = nullptr;
    png_structp png_ptr_write = nullptr;
    png_infop info_ptr_read = nullptr;
    png_infop info_ptr_write = nullptr;

    // Initialize read structures
    png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr_read) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize write structures
    png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr_read)) || setjmp(png_jmpbuf(png_ptr_write))) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        remove("output_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    // Setup reading
    png_init_io(png_ptr_read, in_file);
    png_set_sig_bytes(png_ptr_read, 8);
    png_read_info(png_ptr_read, info_ptr_read);

    // Required API 1: Apply gamma correction
    png_set_gamma(png_ptr_read, 2.2, 0.45455);

    // Required API 2: Get next frame fcTL data (for APNG)
    png_uint_32 width = 0, height = 0, x_offset = 0, y_offset = 0;
    png_uint_16 delay_num = 0, delay_den = 0;
    png_byte dispose_op = 0, blend_op = 0;
    png_get_next_frame_fcTL(png_ptr_read, info_ptr_read, &width, &height, 
                          &x_offset, &y_offset, &delay_num, &delay_den, 
                          &dispose_op, &blend_op);

    // Required API 3: Get compression type
    png_byte compression_type = png_get_compression_type(png_ptr_read, info_ptr_read);

    // Required API 4: Set EXIF data (create dummy EXIF)
    png_uint_32 num_exif = 4;
    png_byte exif_data[4] = {0x45, 0x78, 0x69, 0x66}; // "Exif"
    png_set_eXIf_1(png_ptr_read, info_ptr_read, num_exif, exif_data);

    // Required API 5: Set alpha mode
    png_set_alpha_mode_fixed(png_ptr_read, PNG_ALPHA_PNG, PNG_FP_1);

    // Read image data
    png_read_update_info(png_ptr_read, info_ptr_read);
    png_uint_32 rowbytes = png_get_rowbytes(png_ptr_read, info_ptr_read);
    png_bytep row_pointers = (png_bytep)png_malloc(png_ptr_read, rowbytes);
    
    // Read rows
    png_read_row(png_ptr_read, row_pointers, nullptr);

    // Setup writing
    png_init_io(png_ptr_write, out_file);
    
    // Copy info from read to write
    png_uint_32 img_width = png_get_image_width(png_ptr_read, info_ptr_read);
    png_uint_32 img_height = png_get_image_height(png_ptr_read, info_ptr_read);
    png_byte bit_depth = png_get_bit_depth(png_ptr_read, info_ptr_read);
    png_byte color_type = png_get_color_type(png_ptr_read, info_ptr_read);
    
    png_set_IHDR(png_ptr_write, info_ptr_write, img_width, img_height,
                 bit_depth, color_type, PNG_INTERLACE_NONE,
                 compression_type, PNG_FILTER_TYPE_BASE);

    // Write PNG
    png_write_info(png_ptr_write, info_ptr_write);
    png_write_row(png_ptr_write, row_pointers);
    png_write_end(png_ptr_write, nullptr);

    // Cleanup
    png_free(png_ptr_read, row_pointers);
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}