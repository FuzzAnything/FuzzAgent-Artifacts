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
//<ID> 522
//<Prompt> []
/*<Combination>: [
*/
//<score> 58, nr_unique_branch: 1
//<Quality> {"density":58,"unique_branches":{"png_get_error_ptr":[[732,8,732,23,0,0,4,1]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_progressive_frame_fn","png_set_progressive_read_fn","png_read_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_palette_to_rgb","png_set_expand_gray_1_2_4_to_8","png_get_valid","png_set_tRNS_to_alpha","png_set_strip_16","png_set_gray_to_rgb","png_set_filler","png_read_update_info","png_get_color_type","png_get_bit_depth","png_get_rowbytes","png_malloc_default","png_malloc_default","png_read_image","png_read_end","png_free","png_free","png_destroy_read_struct","png_create_write_struct","png_free","png_free","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct","png_init_io","png_set_IHDR","png_set_gAMA_fixed","png_set_tRNS","png_write_info","png_write_image","png_write_end","png_malloc_default","png_image_write_to_file","png_free","png_destroy_write_struct","png_destroy_read_struct","png_free","png_free"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_progressive_frame_fn","png_set_progressive_read_fn","png_read_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_palette_to_rgb","png_set_expand_gray_1_2_4_to_8","png_get_valid","png_set_tRNS_to_alpha","png_set_strip_16","png_set_gray_to_rgb","png_set_filler","png_read_update_info","png_get_color_type","png_get_bit_depth","png_get_rowbytes","png_malloc_default","png_malloc_default","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_init_io","png_set_IHDR","png_set_gAMA_fixed","png_set_tRNS","png_write_info","png_write_image","png_write_end","png_malloc_default","png_image_write_to_file","png_free","png_destroy_write_struct","png_destroy_read_struct","png_free","png_free"],"visited":1}
/**/


// Custom error handling structure
struct PngErrorInfo {
    jmp_buf jmpbuf;
    bool error_occurred;
};

// Progressive reading callbacks
void  id_000027_progressive_info_callback(png_structp png_ptr, png_infop info_ptr) {
    // Do nothing in fuzzing context
}

void  id_000027_progressive_row_callback(png_structp png_ptr, png_bytep new_row, 
                              png_uint_32 row_num, int pass) {
    // Do nothing in fuzzing context
}

void  id_000027_progressive_end_callback(png_structp png_ptr, png_infop info_ptr) {
    // Do nothing in fuzzing context
}

// Frame callbacks for APNG
void  id_000027_frame_info_callback(png_structp png_ptr, png_uint_32 frame_num) {
    // Do nothing in fuzzing context
}

void  id_000027_frame_end_callback(png_structp png_ptr, png_uint_32 frame_num) {
    // Do nothing in fuzzing context
}

// Error handling callback
void  id_000027_png_error_handler(png_structp png_ptr, const char* error_message) {
    PngErrorInfo* error_info = (PngErrorInfo*)png_get_error_ptr(png_ptr);
    error_info->error_occurred = true;
    longjmp(error_info->jmpbuf, 1);
}

// Warning handling callback
void  id_000027_png_warning_handler(png_structp png_ptr, const char* warning_message) {
    // Ignore warnings during fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<79) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
	//fuzzer shim end}




    if (size < 8) {
        return 0; // Too small for PNG signature
    }
    
    // Check PNG signature
    if (png_sig_cmp((png_bytep)data, fuzz_uint64_t_1, 8) != 0) {
        return 0; // Not a PNG file
    }
    
    FILE* in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    PngErrorInfo error_info;
    error_info.error_occurred = false;
    
    // Create read structure
    png_structp read_ptr = png_create_read_struct(
        fuzz_str_2, 
        &error_info, 
         id_000027_png_error_handler, 
         id_000027_png_warning_handler
    );
    
    if (!read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create info structure
    png_infop read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up longjmp for error handling
    png_set_longjmp_fn(read_ptr, longjmp, sizeof(jmp_buf));
    
    if (setjmp(error_info.jmpbuf)) {
	FUZZ_FILE_CLOSE();
        // Error occurred during PNG processing
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize IO
    png_init_io(read_ptr, in_file);
    
    // Set progressive frame functions
    png_set_progressive_frame_fn(read_ptr,  id_000027_frame_info_callback,  id_000027_frame_end_callback);
    
    // Set progressive read functions
    png_set_progressive_read_fn(read_ptr, nullptr, 
                                id_000027_progressive_info_callback,
                                id_000027_progressive_row_callback,
                                id_000027_progressive_end_callback);
    
    // Read PNG info
    png_read_info(read_ptr, read_info);
    
    // Get basic image information
    png_uint_32 width = png_get_image_width(read_ptr, read_info);
    png_uint_32 height = png_get_image_height(read_ptr, read_info);
    png_byte color_type = png_get_color_type(read_ptr, read_info);
    png_byte bit_depth = png_get_bit_depth(read_ptr, read_info);
    
    // Apply various transformations
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(read_ptr);
    }
    
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(read_ptr);
    }
    
    // Check for transparency
    if (png_get_valid(read_ptr, read_info, fuzz_uint32_t_3)) {
        png_set_tRNS_to_alpha(read_ptr);
    }
    
    // Strip 16-bit to 8-bit
    if (bit_depth == 16) {
        png_set_strip_16(read_ptr);
    }
    
    // Convert grayscale to RGB
    if (color_type == PNG_COLOR_TYPE_GRAY || 
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(read_ptr);
    }
    
    // Add filler channel if needed
    if (!(color_type & PNG_COLOR_MASK_ALPHA)) {
        png_set_filler(read_ptr, fuzz_uint32_t_4, fuzz_int32_t_5);
    }
    
    // Update info after transformations
    png_read_update_info(read_ptr, read_info);
    
    // Get updated info
    color_type = png_get_color_type(read_ptr, read_info);
    bit_depth = png_get_bit_depth(read_ptr, read_info);
    size_t rowbytes = png_get_rowbytes(read_ptr, read_info);
    
    // Allocate memory for image data
    png_bytepp row_pointers = (png_bytepp)png_malloc_default(
        read_ptr, 
        sizeof(png_bytep) * height
    );
    
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)png_malloc_default(read_ptr, rowbytes);
    }
    
    // Read the image
    png_read_image(read_ptr, row_pointers);
    
    // Read end of PNG
    png_read_end(read_ptr, read_info);
	FUZZ_FILENAME_PUSH("output_file")
    
    // Now write the image back out
    FILE* out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        // Clean up read resources
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(read_ptr, row_pointers[y]);
        }
        png_free(read_ptr, row_pointers);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create write structure
    png_structp write_ptr = png_create_write_struct(
        fuzz_str_6,
        &error_info,
         id_000027_png_error_handler,
         id_000027_png_warning_handler
    );
    
    if (!write_ptr) {
        assert_file_closed(&out_file);;
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(read_ptr, row_pointers[y]);
        }
        png_free(read_ptr, row_pointers);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create write info structure
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        assert_file_closed(&out_file);;
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(read_ptr, row_pointers[y]);
        }
        png_free(read_ptr, row_pointers);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    if (setjmp(error_info.jmpbuf)) {
	FUZZ_FILE_CLOSE();
        // Error occurred during writing
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&out_file);;
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(read_ptr, row_pointers[y]);
        }
        png_free(read_ptr, row_pointers);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize write IO
    png_init_io(write_ptr, out_file);
    
    // Set IHDR for output
    png_set_IHDR(write_ptr, write_info, width, height,
                 bit_depth, color_type, fuzz_int32_t_7,
                 fuzz_int32_t_8, fuzz_int32_t_9);
    
    // Set gamma (using fixed point value)
    png_set_gAMA_fixed(write_ptr, write_info, fuzz_int32_t_10); // gamma = 1.0/2.2 * 100000
    
    // Set tRNS if needed (simplified - just using a dummy)
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_color_16 trans_color;
        trans_color.index = 0;
        png_set_tRNS(write_ptr, write_info, nullptr, 0, &trans_color);
    }
    
    // Write info
    png_write_info(write_ptr, write_info);
    
    // Write image
    png_write_image(write_ptr, row_pointers);
    
    // Write end
    png_write_end(write_ptr, write_info);
    
    // Also try the high-level write function
    png_image image_desc;
    memset(&image_desc, 0, sizeof(image_desc));
    image_desc.version = PNG_IMAGE_VERSION;
    image_desc.width = width;
    image_desc.height = height;
    
    // Determine format based on color type
    if (color_type == PNG_COLOR_TYPE_RGB) {
        image_desc.format = PNG_FORMAT_RGB;
    } else if (color_type == PNG_COLOR_TYPE_RGBA) {
        image_desc.format = PNG_FORMAT_RGBA;
    } else if (color_type == PNG_COLOR_TYPE_GRAY) {
        image_desc.format = PNG_FORMAT_GRAY;
    } else if (color_type == PNG_COLOR_TYPE_GA) {
        image_desc.format = PNG_FORMAT_GA;
    }
    
    // Flatten row pointers into a contiguous buffer
    png_byte* flat_buffer = (png_byte*)png_malloc_default(
        write_ptr,
        PNG_IMAGE_SIZE(image_desc)
    );
    
    if (flat_buffer) {
        png_uint_32 row_stride = PNG_IMAGE_ROW_STRIDE(image_desc);
        for (png_uint_32 y = 0; y < height; y++) {
            memcpy(flat_buffer + y * row_stride, row_pointers[y], row_stride);
        }
        
        // Write using high-level API
        png_image_write_to_file(&image_desc, fuzz_str_11, fuzz_int32_t_12, 
                                flat_buffer, row_stride, nullptr);
        
        png_free(write_ptr, flat_buffer);
    }
    
    // Clean up
    png_destroy_write_struct(&write_ptr, &write_info);
    png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(read_ptr, row_pointers[y]);
    }
    png_free(read_ptr, row_pointers);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}