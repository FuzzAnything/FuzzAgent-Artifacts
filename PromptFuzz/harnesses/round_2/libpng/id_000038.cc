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
//<ID> 724
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 2
//<Quality> {"density":44,"unique_branches":{"png_set_text_compression_strategy":[[1309,8,1309,23,0,0,4,1]],"png_set_compression_mem_level":[[1210,8,1210,23,0,0,4,1]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_set_text_compression_strategy","png_set_compression_mem_level","png_read_info","png_get_IHDR","png_set_sRGB","png_set_swap_alpha","png_set_strip_alpha","png_set_shift","png_set_read_user_transform_fn","png_read_update_info","png_get_pHYs_dpi","png_get_signature","png_get_num_frames","png_get_rowbytes","png_malloc","png_destroy_read_struct","png_read_row","png_free","png_read_end","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_set_IHDR","png_set_scale_16","png_write_info","png_malloc","png_write_row","png_free","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_set_text_compression_strategy","png_set_compression_mem_level","png_read_info","png_get_IHDR","png_set_sRGB","png_set_swap_alpha","png_set_strip_alpha","png_set_shift","png_set_read_user_transform_fn","png_read_update_info","png_get_pHYs_dpi","png_get_signature","png_get_num_frames","png_get_rowbytes","png_malloc","png_read_row","png_free","png_read_end","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_set_scale_16","png_write_info","png_malloc","png_write_row","png_free","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"visited":1}
/*Here's a fuzz driver that uses the specified libpng APIs to perform PNG file processing:

*/


// Include libpng headers

// Custom user transform function
void  id_000038_user_transform_fn(png_structp png_ptr, png_row_infop row_info, png_bytep data) {
    // Simple transform that does nothing - required for png_set_read_user_transform_fn
    (void)png_ptr;
    (void)row_info;
    (void)data;
}

// Custom error and warning handlers
void  id_000038_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    // Jump back to setjmp point
    longjmp(png_jmpbuf(png_ptr), 1);
}

void  id_000038_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Ignore warnings during fuzzing
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
        return 0;  // Need at least PNG signature
    }

    // Check if data starts with PNG signature
    if (png_sig_cmp((png_bytep)data, 0, 8) != 0) {
        return 0;  // Not a PNG file
    }

    // Create input FILE* from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    // Create output FILE*
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize read structures
    png_structp png_read_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
         id_000038_error_handler, 
         id_000038_warning_handler
    );
    if (!png_read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_infop read_info_ptr = png_create_info_struct(png_read_ptr);
    if (!read_info_ptr) {
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Setup longjmp for error handling
    jmp_buf *read_jmp_buf = png_set_longjmp_fn(png_read_ptr, longjmp, sizeof(jmp_buf));
    if (setjmp(*read_jmp_buf)) {
	FUZZ_FILE_CLOSE();
        // Error occurred, cleanup
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize I/O for reading
    png_init_io(png_read_ptr, in_file);
    
    // Set signature bytes
    png_set_sig_bytes(png_read_ptr, 8);
    
    // Set text compression strategy
    png_set_text_compression_strategy(png_read_ptr, 0);
    
    // Set compression memory level
    png_set_compression_mem_level(png_read_ptr, 8);
    
    // Read PNG info
    png_read_info(png_read_ptr, read_info_ptr);
    
    // Get basic image information
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type, compression_type, filter_type;
    png_get_IHDR(png_read_ptr, read_info_ptr, &width, &height, &bit_depth, 
                 &color_type, &interlace_type, &compression_type, &filter_type);
    
    // Set various transformations
    png_set_sRGB(png_read_ptr, read_info_ptr, PNG_sRGB_INTENT_PERCEPTUAL);
    
    // Setup alpha channel transformations
    png_set_swap_alpha(png_read_ptr);
    png_set_strip_alpha(png_read_ptr);
    
    // Setup bit shifting
    png_color_8 true_bits = {8, 8, 8, 8, 8};
    png_set_shift(png_read_ptr, &true_bits);
    
    // Setup user transform function
    png_set_read_user_transform_fn(png_read_ptr,  id_000038_user_transform_fn);
    
    // Update info after transformations
    png_read_update_info(png_read_ptr, read_info_ptr);
    
    // Get physical dimensions
    png_uint_32 res_x, res_y;
    int unit_type;
    png_get_pHYs_dpi(png_read_ptr, read_info_ptr, &res_x, &res_y, &unit_type);
    
    // Get signature
    const png_byte *signature = png_get_signature(png_read_ptr, read_info_ptr);
    
    // Get number of frames (for APNG)
    png_uint_32 num_frames = png_get_num_frames(png_read_ptr, read_info_ptr);
    
    // Get row bytes
    size_t rowbytes = png_get_rowbytes(png_read_ptr, read_info_ptr);
    
    // Allocate memory for rows
    png_bytep row = (png_bytep)png_malloc(png_read_ptr, rowbytes);
    if (!row) {
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Read rows
    for (png_uint_32 y = 0; y < height; y++) {
        png_read_row(png_read_ptr, row, nullptr);
    }
    
    // Free allocated row
    png_free(png_read_ptr, row);
    
    // Read end of PNG
    png_read_end(png_read_ptr, nullptr);
    
    // Now write the PNG to output file
    png_structp png_write_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        nullptr,
         id_000038_error_handler,
         id_000038_warning_handler
    );
    if (!png_write_ptr) {
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop write_info_ptr = png_create_info_struct(png_write_ptr);
    if (!write_info_ptr) {
        png_destroy_write_struct(&png_write_ptr, nullptr);
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Setup longjmp for write structure
    jmp_buf *write_jmp_buf = png_set_longjmp_fn(png_write_ptr, longjmp, sizeof(jmp_buf));
    if (setjmp(*write_jmp_buf)) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O for writing
    png_init_io(png_write_ptr, out_file);
    
    // Set IHDR for output
    png_set_IHDR(png_write_ptr, write_info_ptr, width, height, bit_depth, 
                 color_type, interlace_type, compression_type, filter_type);
    
    // Set 16-bit scaling
    png_set_scale_16(png_write_ptr);
    
    // Write info
    png_write_info(png_write_ptr, write_info_ptr);
    
    // Allocate and write rows (simplified - using dummy data for demonstration)
    png_bytep write_row = (png_bytep)png_malloc(png_write_ptr, rowbytes);
    if (write_row) {
        memset(write_row, 0, rowbytes);
        for (png_uint_32 y = 0; y < height; y++) {
            png_write_row(png_write_ptr, write_row);
        }
        png_free(png_write_ptr, write_row);
    }
    
    // Write end
    png_write_end(png_write_ptr, write_info_ptr);
    
    // Cleanup
    png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
    png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}