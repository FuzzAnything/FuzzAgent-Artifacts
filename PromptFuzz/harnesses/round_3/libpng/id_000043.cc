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
//<ID> 866
//<Prompt> []
/*<Combination>: [
*/
//<score> 99, nr_unique_branch: 9
//<Quality> {"density":33,"unique_branches":{"png_get_cLLI_fixed":[[794,8,794,23,0,0,4,0],[794,27,794,43,0,0,4,0],[795,8,795,46,0,0,4,0],[795,8,795,46,0,0,4,1],[797,11,797,25,0,0,4,0],[798,11,798,26,0,0,4,0]],"png_set_filter":[[1072,8,1072,23,0,0,4,1],[1076,8,1076,71,0,0,4,1],[1081,8,1081,38,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_read_fn","png_get_io_ptr","png_get_io_ptr","png_read_info","png_get_cLLI_fixed","png_get_oFFs","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_set_write_fn","png_get_io_ptr","png_get_io_ptr","png_set_gamma","png_set_filter","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_IHDR","png_set_cLLI","png_set_oFFs","png_write_info","png_get_rowbytes","png_read_image","png_write_image","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_set_read_fn","png_get_io_ptr","png_get_io_ptr","png_read_info","png_get_cLLI_fixed","png_get_oFFs","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_write_fn","png_get_io_ptr","png_get_io_ptr","png_set_gamma","png_set_filter","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_IHDR","png_set_cLLI","png_set_oFFs","png_write_info","png_get_rowbytes","png_read_image","png_write_image","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to achieve the event of reading and processing a PNG file with specific metadata operations:

*/


extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<52) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for PNG operations

    // Create input file from memory
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

    png_struct *png_ptr = nullptr;
    png_info *info_ptr = nullptr;
    png_info *end_info = nullptr;

    // Initialize read structure
    png_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize info structure
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize end info structure
    end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up read function
    png_set_read_fn(png_ptr, in_file, [](png_struct *png_ptr, png_byte *data_ptr, png_size_t length) {
        FILE *f = (FILE *)png_get_io_ptr(png_ptr);
        fread(data_ptr, 1, length, f);
    });

    // Read PNG header
    png_read_info(png_ptr, info_ptr);

    // 1. Use png_get_uint_32 to read a 32-bit value from buffer
    png_uint_32 uint_value = 0;
    if (size >= 4) {
        // Use first 4 bytes of input data
        uint_value = png_get_uint_32(data);
    }

    // 2. Use png_reset_zstream (if available in this context)
    // Note: png_reset_zstream is typically called after decompression errors
    // We'll simulate a scenario where we might need it
    int zstream_result = 0;
    // This would normally be called if we had a zstream error
    // zstream_result = png_reset_zstream(png_ptr);

    // 3. Get CLLI (Content Light Level Information) metadata
    png_uint_32 max_content_light_level = 0;
    png_uint_32 max_frame_average_light_level = 0;
    png_uint_32 clli_result = png_get_cLLI_fixed(png_ptr, info_ptr, 
        &max_content_light_level, &max_frame_average_light_level);

    // 4. Get oFFs (Image Offset) metadata
    png_int_32 offset_x = 0, offset_y = 0;
    int unit_type = 0;
    png_uint_32 offs_result = png_get_oFFs(png_ptr, info_ptr, 
        &offset_x, &offset_y, &unit_type);

    // Create write structure for output
    png_struct *write_ptr = png_create_write_struct(fuzz_str_2, nullptr, nullptr, nullptr);
    if (!write_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_info *write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up write function
    png_set_write_fn(write_ptr, out_file, [](png_struct *png_ptr, png_byte *data_ptr, png_size_t length) {
        FILE *f = (FILE *)png_get_io_ptr(png_ptr);
        fwrite(data_ptr, 1, length, f);
    }, nullptr);

    // 5. Set gamma correction
    // Use the uint32 value we read earlier to create gamma values
    double screen_gamma = 2.2;
    double file_gamma = (uint_value % 1000) / 1000.0 + 1.0;  // Convert to range 1.0-2.0
    png_set_gamma(write_ptr, screen_gamma, file_gamma);

    // 6. Set filter method
    // Use the uint32 value to determine filter type
    int filter_method = PNG_FILTER_NONE;
    int filters = uint_value % 5;  // 0-4 for different filter combinations
    png_set_filter(write_ptr, fuzz_int32_t_3, filters);

    // Copy image info from read to write
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    png_set_IHDR(write_ptr, write_info, width, height, bit_depth, color_type,
                 fuzz_int32_t_4, fuzz_int32_t_5, fuzz_int32_t_6);

    // Set other metadata if available
    if (clli_result & PNG_INFO_cLLI) {
        // Convert back from fixed point if needed
        png_set_cLLI(write_ptr, write_info, 
            max_content_light_level / 10000.0, 
            max_frame_average_light_level / 10000.0);
    }

    if (offs_result & PNG_INFO_oFFs) {
        png_set_oFFs(write_ptr, write_info, fuzz_int32_t_7, fuzz_int32_t_8, fuzz_int32_t_9);
    }

    // Write header
    png_write_info(write_ptr, write_info);

    // Read image data
    png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
    }
    
    png_read_image(png_ptr, row_pointers);

    // Write image data
    png_write_image(write_ptr, row_pointers);
    png_write_end(write_ptr, nullptr);

    // Cleanup
    for (png_uint_32 y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);

    png_destroy_write_struct(&write_ptr, &write_info);
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}