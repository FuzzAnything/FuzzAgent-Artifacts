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
//<ID> 160
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_next_frame_delay_den","png_get_uint_31","png_benign_error","png_get_sCAL_fixed","png_save_uint_16","png_read_update_info","png_get_image_width","png_get_image_height","png_malloc","png_malloc","png_get_rowbytes","png_read_image","png_read_end","png_free","png_free","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_next_frame_delay_den","png_get_uint_31","png_benign_error","png_get_sCAL_fixed","png_save_uint_16","png_read_update_info","png_get_image_width","png_get_image_height","png_malloc","png_malloc","png_get_rowbytes","png_read_image","png_read_end","png_free","png_free","png_destroy_read_struct"],"visited":1}
/**/


// Include libpng headers

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations

    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Initialize libpng structures
    png_struct *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling - use setjmp for libpng error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize PNG reading
    png_init_io(png_ptr, in_file);
    png_read_info(png_ptr, info_ptr);

    // Event: Process PNG data to extract frame delay denominator and sCAL information,
    // while handling warnings and converting data between formats
    
    // 1. Get next frame delay denominator (for APNG animation)
    png_uint_16 delay_den = png_get_next_frame_delay_den(png_ptr, info_ptr);
    
    // 2. Read a 31-bit unsigned integer from buffer (safe version of png_get_uint_32)
    png_uint_32 uint31_value = 0;
    if (size >= 4) {
        uint31_value = png_get_uint_31(png_ptr, data);
    }
    
    // 3. Report a benign warning (non-fatal error)
    png_benign_error(png_ptr, "Fuzzing: benign warning triggered");
    
    // 4. Get sCAL chunk information (physical scale of pixels)
    int unit_type = 0;
    png_fixed_point width_scaled, height_scaled;
    png_uint_32 scal_present = png_get_sCAL_fixed(png_ptr, info_ptr, 
                                                 &unit_type, 
                                                 &width_scaled, 
                                                 &height_scaled);
    
    // 5. Save a 16-bit integer to buffer (demonstrating data conversion)
    png_byte output_buffer[2];
    png_save_uint_16(output_buffer, delay_den);  // Save the delay denominator
    
    // Additional processing: Read the rest of the PNG
    png_read_update_info(png_ptr, info_ptr);
    
    // Read image data (if present)
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    
    if (width > 0 && height > 0 && width < 10000 && height < 10000) {
        png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr, 
                                                        height * sizeof(png_bytep));
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_byte*)png_malloc(png_ptr, 
                                                   png_get_rowbytes(png_ptr, info_ptr));
        }
        
        png_read_image(png_ptr, row_pointers);
        png_read_end(png_ptr, info_ptr);
        
        // Free row pointers
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
        png_free(png_ptr, row_pointers);
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    
    // Additional data processing using extracted values
    // This ensures all APIs' return values are used meaningfully
    uint32_t processed_value = uint31_value + delay_den;
	FUZZ_FILENAME_PUSH("output_file")
    
    // Write some output to a file (optional, for demonstration)
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file) {
        fwrite(output_buffer, 1, sizeof(output_buffer), out_file);
        fwrite(&processed_value, 1, sizeof(processed_value), out_file);
        assert_file_closed(&out_file);;
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}