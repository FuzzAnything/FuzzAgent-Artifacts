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
//<ID> 359
//<Prompt> []
/*<Combination>: [
*/
//<score> 47, nr_unique_branch: 1
//<Quality> {"density":47,"unique_branches":{"png_permit_mng_features":[[1649,8,1649,23,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_read_info","png_permit_mng_features","png_read_frame_head","png_convert_from_time_t","png_set_sPLT","png_get_next_frame_width","png_reset_zstream","png_set_swap","png_get_text","png_convert_to_rfc1123_buffer","png_get_image_width","png_get_image_height","png_image_begin_read_from_file","png_get_color_type","png_get_bit_depth","png_set_palette_to_rgb","png_set_expand_gray_1_2_4_to_8","png_get_valid","png_set_tRNS_to_alpha","png_set_strip_16","png_set_gray_to_rgb","png_read_update_info","png_get_rowbytes","png_read_image","png_read_end","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_set_IHDR","png_set_quantize","png_write_info","png_write_image","png_write_end","png_destroy_info_struct","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_permit_mng_features","png_read_frame_head","png_convert_from_time_t","png_set_sPLT","png_get_next_frame_width","png_reset_zstream","png_set_swap","png_get_text","png_convert_to_rfc1123_buffer","png_get_image_width","png_get_image_height","png_image_begin_read_from_file","png_get_color_type","png_get_bit_depth","png_set_palette_to_rgb","png_set_expand_gray_1_2_4_to_8","png_get_valid","png_set_tRNS_to_alpha","png_set_strip_16","png_set_gray_to_rgb","png_read_update_info","png_get_rowbytes","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_set_quantize","png_write_info","png_write_image","png_write_end","png_destroy_info_struct","png_destroy_write_struct","png_destroy_read_struct"],"visited":1}
/*Here's a C++ fuzzer that uses the specified libpng APIs to read, process, and write PNG data:

*/


// Custom error handlers
static void  id_000028_png_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    // Silent error handler for fuzzing
    png_longjmp(png_ptr, 1);
}

static void  id_000028_png_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Silent warning handler for fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
        return 0;
    }
	FUZZ_FILENAME_PUSH("input_file")

    // Step 1: Write input data to a temporary file for reading
    FILE *in_file = fopen("input_file", "wb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // Step 2: Create read structures
    png_structp png_read_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
         id_000028_png_error_handler, 
         id_000028_png_warning_handler
    );
    if (!png_read_ptr) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    png_infop read_info_ptr = png_create_info_struct(png_read_ptr);
    if (!read_info_ptr) {
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Set up longjmp for error handling
    jmp_buf *jmpbuf = png_set_longjmp_fn(png_read_ptr, longjmp, sizeof(jmp_buf));
    if (setjmp(*jmpbuf)) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        remove("input_file");
	FUZZ_FILENAME_PUSH("output_file")
        remove("output_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }
	FUZZ_FILENAME_PUSH("input_file")

    // Step 3: Open input file and initialize IO
    in_file = fopen("input_file", "rb");
    if (!in_file) {
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    png_init_io(png_read_ptr, in_file);

    // Step 4: Read PNG info
    png_read_info(png_read_ptr, read_info_ptr);

    // Step 5: Permit MNG features
    png_permit_mng_features(png_read_ptr, 0);

    // Step 6: Try to read frame head (for APNG)
    png_read_frame_head(png_read_ptr, read_info_ptr);

    // Step 7: Create dummy time data
    png_time ptime;
    time_t current_time = time(nullptr);
    png_convert_from_time_t(&ptime, current_time);

    // Step 8: Create dummy sPLT data
    png_sPLT_t splt_entry;
    splt_entry.name = (char*)"test";
    splt_entry.depth = 8;
    splt_entry.nentries = 0;
    splt_entry.entries = nullptr;
    png_set_sPLT(png_read_ptr, read_info_ptr, &splt_entry, 1);

    // Step 9: Get next frame width (for APNG)
    png_uint_32 next_frame_width = png_get_next_frame_width(png_read_ptr, read_info_ptr);

    // Step 10: Reset zstream
    png_reset_zstream(png_read_ptr);

    // Step 11: Set swap for 16-bit images
    png_set_swap(png_read_ptr);

    // Step 12: Get text chunks
    png_textp text_ptr = nullptr;
    int num_text = 0;
    png_get_text(png_read_ptr, read_info_ptr, &text_ptr, &num_text);

    // Step 13: Convert time to RFC1123 format
    char rfc1123_time[29];
    png_convert_to_rfc1123_buffer(rfc1123_time, &ptime);

    // Step 14: Get image dimensions
    png_uint_32 width = png_get_image_width(png_read_ptr, read_info_ptr);
    png_uint_32 height = png_get_image_height(png_read_ptr, read_info_ptr);

    // Step 15: Use png_image API for reading
    png_image image_info;
    memset(&image_info, 0, sizeof(image_info));
    image_info.version = PNG_IMAGE_VERSION;
    png_image_begin_read_from_file(&image_info, "input_file");

    // Step 16: Get color type and bit depth
    png_byte color_type = png_get_color_type(png_read_ptr, read_info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_read_ptr, read_info_ptr);

    // Step 17: Set up transformations for reading
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_read_ptr);
    }
    
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png_read_ptr);
    }
    
    png_uint_32 valid = png_get_valid(png_read_ptr, read_info_ptr, PNG_INFO_tRNS);
    if (valid & PNG_INFO_tRNS) {
        png_set_tRNS_to_alpha(png_read_ptr);
    }
    
    if (bit_depth == 16) {
        png_set_strip_16(png_read_ptr);
    }
    
    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png_read_ptr);
    }

    // Step 18: Update info after transformations
    png_read_update_info(png_read_ptr, read_info_ptr);

    // Step 19: Get rowbytes
    size_t rowbytes = png_get_rowbytes(png_read_ptr, read_info_ptr);

    // Step 20: Read image data
    std::vector<png_bytep> row_pointers(height);
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)malloc(rowbytes);
    }
    png_read_image(png_read_ptr, row_pointers.data());

    // Step 21: Read end of PNG
    png_read_end(png_read_ptr, read_info_ptr);

    // Close input file
    assert_file_closed(&in_file);;

    // Step 22: Create write structures
    png_structp png_write_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        nullptr,
         id_000028_png_error_handler,
         id_000028_png_warning_handler
    );
    if (!png_write_ptr) {
        // Cleanup for read
        for (png_uint_32 y = 0; y < height; y++) {
            free(row_pointers[y]);
        }
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    png_infop write_info_ptr = png_create_info_struct(png_write_ptr);
    if (!write_info_ptr) {
        png_destroy_write_struct(&png_write_ptr, nullptr);
        for (png_uint_32 y = 0; y < height; y++) {
            free(row_pointers[y]);
        }
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Set up longjmp for write error handling
    if (setjmp(png_jmpbuf(png_write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
        for (png_uint_32 y = 0; y < height; y++) {
            free(row_pointers[y]);
        }
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        remove("input_file");
	FUZZ_FILENAME_PUSH("output_file")
        remove("output_file");
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    // Step 23: Open output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
        for (png_uint_32 y = 0; y < height; y++) {
            free(row_pointers[y]);
        }
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
	FUZZ_FILENAME_PUSH("input_file")
        remove("input_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    png_init_io(png_write_ptr, out_file);

    // Step 24: Set IHDR for output
    png_set_IHDR(png_write_ptr, write_info_ptr,
                 width, height, 8,  // Using 8-bit depth for output
                 PNG_COLOR_TYPE_RGB,  // Convert to RGB for output
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);

    // Step 25: Create dummy palette for quantization
    png_color palette[256];
    png_uint_16 histogram[256] = {0};
    for (int i = 0; i < 256; i++) {
        palette[i].red = i;
        palette[i].green = i;
        palette[i].blue = i;
        histogram[i] = 1;
    }
    png_set_quantize(png_write_ptr, palette, 256, 256, histogram, 0);

    // Step 26: Write info
    png_write_info(png_write_ptr, write_info_ptr);

    // Step 27: Write image data (using the data we read)
    png_write_image(png_write_ptr, row_pointers.data());

    // Step 28: Write end
    png_write_end(png_write_ptr, write_info_ptr);

    // Step 29: Cleanup
    assert_file_closed(&out_file);;
    
    // Destroy write structures
    png_destroy_info_struct(png_write_ptr, &write_info_ptr);
    png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
    
    // Free row pointers
    for (png_uint_32 y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    
    // Destroy read structures
    png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
    
    // Clean up temporary files
    remove("input_file");
	FUZZ_FILENAME_PUSH("output_file")
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}