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
//<ID> 392
//<Prompt> []
/*<Combination>: [
*/
//<score> 114, nr_unique_branch: 1
//<Quality> {"density":57,"unique_branches":{"png_get_error_ptr":[[732,8,732,23,0,0,4,1]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_free","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_read_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_invert_mono","png_read_update_info","png_get_x_pixels_per_inch","png_get_valid","png_get_next_frame_delay_den","png_get_next_frame_blend_op","png_get_text","png_get_tIME","png_convert_to_rfc1123_buffer","png_save_uint_16","png_get_rowbytes","png_malloc","png_malloc","png_read_image","png_free","png_free","png_destroy_read_struct","png_create_write_struct","png_free","png_free","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct","png_init_io","png_set_IHDR","png_write_info","png_write_row","png_write_end","png_malloc","png_image_write_to_file","png_free","png_destroy_write_struct","png_free","png_free","png_read_end","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_read_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_invert_mono","png_read_update_info","png_get_x_pixels_per_inch","png_get_valid","png_get_next_frame_delay_den","png_get_next_frame_blend_op","png_get_text","png_get_tIME","png_convert_to_rfc1123_buffer","png_save_uint_16","png_get_rowbytes","png_malloc","png_malloc","png_read_image","png_create_write_struct","png_create_info_struct","png_init_io","png_set_IHDR","png_write_info","png_write_row","png_write_end","png_malloc","png_image_write_to_file","png_free","png_destroy_write_struct","png_free","png_free","png_read_end","png_destroy_read_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to read a PNG file from input data, extract information from it, and write a modified version:

*/


extern "C" {
}

// Error handling structure
struct FuzzErrorData {
    jmp_buf jmpbuf;
    bool error_occurred;
};

static void  id_000024_FuzzErrorFn(png_structp png_ptr, png_const_charp error_msg) {
    FuzzErrorData* fuzz_data = (FuzzErrorData*)png_get_error_ptr(png_ptr);
    fuzz_data->error_occurred = true;
    longjmp(fuzz_data->jmpbuf, 1);
}

static void  id_000024_FuzzWarningFn(png_structp png_ptr, png_const_charp warning_msg) {
    // Just call png_warning as required
    png_warning(png_ptr, warning_msg);
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	 // Need at least PNG signature
    
    // Check PNG signature
    if (png_sig_cmp((png_const_bytep)data, 0, 8) != 0) {
        return 0; // Not a PNG file
    }
    
    FILE* in_file = nullptr;
    FILE* out_file = nullptr;
    png_structp read_ptr = nullptr;
    png_structp write_ptr = nullptr;
    png_infop read_info = nullptr;
    png_infop write_info = nullptr;
    png_infop end_info = nullptr;
    png_bytepp row_pointers = nullptr;
    FuzzErrorData read_error_data = {0};
    FuzzErrorData write_error_data = {0};
    
    // Create input FILE from memory
    in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Set up read error handling
    read_error_data.error_occurred = false;
    
    // Create read structure
    read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                     &read_error_data, 
                                      id_000024_FuzzErrorFn, 
                                      id_000024_FuzzWarningFn);
    if (!read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create info structure
    read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    end_info = png_create_info_struct(read_ptr);
    if (!end_info) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set longjmp function for error handling
    jmp_buf* jmpbuf_ptr = png_set_longjmp_fn(read_ptr, longjmp, sizeof(jmp_buf));
    
    // Set up error handling with setjmp
    if (setjmp(read_error_data.jmpbuf)) {
	FUZZ_FILE_CLOSE();
        // Clean up on error
        if (row_pointers) png_free(read_ptr, row_pointers);
        png_destroy_read_struct(&read_ptr, &read_info, &end_info);
        assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O
    png_init_io(read_ptr, in_file);
    
    // Set PNG signature (already checked)
    png_set_sig_bytes(read_ptr, 8);
    
    // Read info
    png_read_info(read_ptr, read_info);
    
    // Get various image properties
    png_uint_32 width = png_get_image_width(read_ptr, read_info);
    png_uint_32 height = png_get_image_height(read_ptr, read_info);
    png_byte color_type = png_get_color_type(read_ptr, read_info);
    png_byte bit_depth = png_get_bit_depth(read_ptr, read_info);
    
    // Transformations
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_invert_mono(read_ptr);
    }
    
    // Update info after transformations
    png_read_update_info(read_ptr, read_info);
    
    // Get various metadata
    png_uint_32 x_pixels_per_inch = png_get_x_pixels_per_inch(read_ptr, read_info);
    png_uint_32 valid = png_get_valid(read_ptr, read_info, PNG_INFO_tRNS);
    
    // Animation-related (if present)
    png_uint_16 delay_den = png_get_next_frame_delay_den(read_ptr, read_info);
    png_byte blend_op = png_get_next_frame_blend_op(read_ptr, read_info);
    
    // Text handling
    int num_text = 0;
    png_textp text_ptr = nullptr;
    png_get_text(read_ptr, read_info, &text_ptr, &num_text);
    
    // Convert time to RFC1123 format if tIME chunk exists
    png_timep mod_time;
    if (png_get_tIME(read_ptr, read_info, &mod_time)) {
        char time_buf[29];
        int result = png_convert_to_rfc1123_buffer(time_buf, mod_time);
        // Use the buffer to avoid compiler warning
        if (result == 0) {
            // Conversion successful
        }
    }
    
    // Use png_save_uint_16
    png_byte buf[2];
    png_save_uint_16(buf, 1234);
    
    // Read image data
    png_uint_32 rowbytes = png_get_rowbytes(read_ptr, read_info);
    row_pointers = (png_bytepp)png_malloc(read_ptr, height * sizeof(png_bytep));
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)png_malloc(read_ptr, rowbytes);
    }
    
    png_read_image(read_ptr, row_pointers);
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create output file
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        // Clean up
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(read_ptr, row_pointers[y]);
        }
        png_free(read_ptr, row_pointers);
        png_destroy_read_struct(&read_ptr, &read_info, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up write error handling
    write_error_data.error_occurred = false;
    
    // Create write structure
    write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                       &write_error_data,
                                        id_000024_FuzzErrorFn,
                                        id_000024_FuzzWarningFn);
    if (!write_ptr) {
        assert_file_closed(&out_file);;
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(read_ptr, row_pointers[y]);
        }
        png_free(read_ptr, row_pointers);
        png_destroy_read_struct(&read_ptr, &read_info, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create write info structure
    write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        assert_file_closed(&out_file);;
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(read_ptr, row_pointers[y]);
        }
        png_free(read_ptr, row_pointers);
        png_destroy_read_struct(&read_ptr, &read_info, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set write error handling
    if (setjmp(write_error_data.jmpbuf)) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&out_file);;
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(read_ptr, row_pointers[y]);
        }
        png_free(read_ptr, row_pointers);
        png_destroy_read_struct(&read_ptr, &read_info, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize write I/O
    png_init_io(write_ptr, out_file);
    
    // Set IHDR for output
    png_set_IHDR(write_ptr, write_info, width, height, bit_depth,
                color_type, PNG_INTERLACE_NONE,
                PNG_COMPRESSION_TYPE_DEFAULT,
                PNG_FILTER_TYPE_DEFAULT);
    
    // Write info
    png_write_info(write_ptr, write_info);
    
    // Write rows
    for (png_uint_32 y = 0; y < height; y++) {
        png_write_row(write_ptr, row_pointers[y]);
    }
    
    // Write end
    png_write_end(write_ptr, write_info);
    
    // Also try png_image_write_to_file
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.width = width;
    image.height = height;
    image.format = PNG_FORMAT_RGBA;
    
    png_int_32 row_stride = PNG_IMAGE_ROW_STRIDE(image);
    void* buffer = png_malloc(read_ptr, PNG_IMAGE_SIZE(image));
    
    if (buffer) {
        // Fill buffer with data (simplified - in reality would need proper conversion)
        png_image_write_to_file(&image, "output_file2", 0, buffer, row_stride, nullptr);
        png_free(read_ptr, buffer);
    }
    
    // Clean up everything
    png_destroy_write_struct(&write_ptr, &write_info);
    assert_file_closed(&out_file);;
    
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(read_ptr, row_pointers[y]);
    }
    png_free(read_ptr, row_pointers);
    
    png_read_end(read_ptr, end_info);
    png_destroy_read_struct(&read_ptr, &read_info, &end_info);
    
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}