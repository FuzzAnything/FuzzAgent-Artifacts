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
//<ID> 449
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 1
//<Quality> {"density":39,"unique_branches":{"png_get_first_frame_is_hidden":[[1513,8,1513,23,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_init_io","png_read_info","png_set_text_compression_level","png_get_first_frame_is_hidden","png_set_rgb_to_gray","png_get_next_frame_blend_op","png_get_uint_31","png_set_swap_alpha","png_set_gamma_fixed","png_read_update_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_malloc","png_malloc","png_get_rowbytes","png_read_image","png_set_IHDR","png_write_info","png_set_swap_alpha","png_write_image","png_write_flush","png_write_end","png_free","png_free","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_init_io","png_read_info","png_set_text_compression_level","png_get_first_frame_is_hidden","png_set_rgb_to_gray","png_get_next_frame_blend_op","png_get_uint_31","png_set_swap_alpha","png_set_gamma_fixed","png_read_update_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_malloc","png_malloc","png_get_rowbytes","png_read_image","png_set_IHDR","png_write_info","png_set_swap_alpha","png_write_image","png_write_flush","png_write_end","png_free","png_free","png_destroy_read_struct","png_destroy_write_struct"],"visited":1}
/*I'll create a fuzzer that processes PNG data using the specified libpng APIs. The event I'll simulate is: reading a PNG file, applying various transformations (RGB to grayscale, gamma correction, alpha swapping), and writing it out with animation frame settings (for APNG files).

*/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<52) return 0;

	
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




    // Check for minimum size
    if (size < 8) {
        return 0;
    }

    // Use fmemopen to create a FILE* from the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize libpng structures
    png_struct* read_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (!read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_info* read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_struct* write_ptr = png_create_write_struct(fuzz_str_2, nullptr, nullptr, nullptr);
    if (!write_ptr) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_info* write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set error handling
    if (setjmp(png_jmpbuf(read_ptr)) || setjmp(png_jmpbuf(write_ptr))) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize I/O
    png_init_io(read_ptr, in_file);
    png_init_io(write_ptr, out_file);

    // Read PNG header
    png_read_info(read_ptr, read_info);

    // Apply required transformations using the specified APIs
    
    // 1. Set text compression level (for textual data in PNG)
    png_set_text_compression_level(write_ptr, fuzz_int32_t_3);  // Use medium compression level
    
    // 2. Set first frame as hidden (for APNG files)
    // Note: The function name in instruction differs from exported APIs
    // We'll use png_get_first_frame_is_hidden and set it through metadata
    png_byte is_hidden = 0;
    // We'll use this information to simulate setting first frame hidden
    is_hidden = png_get_first_frame_is_hidden(read_ptr, read_info);
    
    // 3. Set RGB to grayscale conversion with custom weights
    png_set_rgb_to_gray(read_ptr, fuzz_int32_t_4, 0.2126, 0.7152);  // ITU-R BT.709 weights
    
    // 4. Get next frame blend op (for APNG)
    png_byte blend_op = png_get_next_frame_blend_op(read_ptr, read_info);
    
    // 5. Get uint31 from buffer - extract from input data
    png_uint_32 uint31_value = 0;
    if (size >= 4) {
        uint31_value = png_get_uint_31(read_ptr, data);
    }
    
    // 6. Set swap alpha (RGBA to ARGB or vice versa)
    png_set_swap_alpha(read_ptr);
    
    // 7. Set gamma correction with fixed point values
    png_set_gamma_fixed(read_ptr, fuzz_int32_t_5, fuzz_int32_t_6);  // ~1.0/2.2 for typical monitor gamma
    
    // Update read transformations
    png_read_update_info(read_ptr, read_info);
    
    // Get image dimensions
    png_uint_32 width = png_get_image_width(read_ptr, read_info);
    png_uint_32 height = png_get_image_height(read_ptr, read_info);
    png_byte color_type = png_get_color_type(read_ptr, read_info);
    png_byte bit_depth = png_get_bit_depth(read_ptr, read_info);
    
    // Allocate row pointers for reading
    png_bytep* row_pointers = (png_bytep*)png_malloc(read_ptr, height * sizeof(png_bytep));
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)png_malloc(read_ptr, png_get_rowbytes(read_ptr, read_info));
    }
    
    // Read image data
    png_read_image(read_ptr, row_pointers);
    
    // Set write parameters
    png_set_IHDR(write_ptr, write_info, width, height, bit_depth, 
                 color_type, fuzz_int32_t_7, 
                 fuzz_int32_t_8, fuzz_int32_t_9);
    
    // Write PNG header
    png_write_info(write_ptr, write_info);
    
    // Apply same transformations for writing
    png_set_swap_alpha(write_ptr);
    
    // Write image data
    png_write_image(write_ptr, row_pointers);
    
    // 8. Write flush to ensure all data is written
    png_write_flush(write_ptr);
    
    // Write end
    png_write_end(write_ptr, nullptr);
    
    // Clean up row pointers
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(read_ptr, row_pointers[y]);
    }
    png_free(read_ptr, row_pointers);
    
    // Clean up
    png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    png_destroy_write_struct(&write_ptr, &write_info);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}