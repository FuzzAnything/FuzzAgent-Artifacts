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
//<ID> 293
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 0
//<Quality> {"density":44,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_longjmp_fn","png_read_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_cHRM","png_set_sRGB_gAMA_and_cHRM","png_set_cLLI_fixed","png_set_hIST","png_process_data_skip","png_get_rowbytes","png_malloc","png_malloc","png_read_image","png_read_end","png_create_write_struct","png_free","png_free","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct","png_init_io","png_set_IHDR","png_write_info","png_write_rows","png_write_png","png_save_int_32","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_longjmp_fn","png_read_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_cHRM","png_set_sRGB_gAMA_and_cHRM","png_set_cLLI_fixed","png_set_hIST","png_process_data_skip","png_get_rowbytes","png_malloc","png_malloc","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_write_info","png_write_rows","png_write_png","png_save_int_32","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	
    
    // Create a memory buffer for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	FUZZ_FILENAME_PUSH("output_file")
	
    
    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Create read struct
    png_struct *png_read_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        nullptr, 
        nullptr
    );
    if (!png_read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Create read info struct
    png_info *read_info_ptr = png_create_info_struct(png_read_ptr);
    if (!read_info_ptr) {
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create end info struct (second call to png_create_info_struct)
    png_info *read_end_info_ptr = png_create_info_struct(png_read_ptr);
    
    // 4. Set up longjmp error handling
    if (setjmp(png_jmpbuf(png_read_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, &read_end_info_ptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Initialize IO for reading
    png_init_io(png_read_ptr, in_file);
    
    // 6. Set longjmp function (another API call)
    jmp_buf jmp_buffer;
    png_set_longjmp_fn(png_read_ptr, longjmp, sizeof(jmp_buf));
    
    // 7. Read PNG info
    png_read_info(png_read_ptr, read_info_ptr);
    
    // 8. Get image dimensions and properties
    png_uint_32 width = png_get_image_width(png_read_ptr, read_info_ptr);
    png_uint_32 height = png_get_image_height(png_read_ptr, read_info_ptr);
    png_byte color_type = png_get_color_type(png_read_ptr, read_info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_read_ptr, read_info_ptr);
    
    // 9. Get cHRM data if available
    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    if (png_get_cHRM(png_read_ptr, read_info_ptr, &white_x, &white_y, 
                     &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y)) {
        // 10. Set sRGB and related chunks
        png_set_sRGB_gAMA_and_cHRM(png_read_ptr, read_info_ptr, PNG_sRGB_INTENT_PERCEPTUAL);
    }
    
    // 11. Set cLLI (Content Light Level Information)
    png_set_cLLI_fixed(png_read_ptr, read_info_ptr, 10000, 5000);
    
    // 12. Set hIST (histogram) - create dummy histogram data
    png_uint_16 hist[256];
    for (int i = 0; i < 256; i++) hist[i] = 100;
    png_set_hIST(png_read_ptr, read_info_ptr, hist);
    
    // 13. Process data skip (though we're not using progressive reading)
    // This is called but won't have effect in non-progressive mode
    png_process_data_skip(png_read_ptr);
    
    // 14. Get row bytes and allocate memory for image data
    size_t rowbytes = png_get_rowbytes(png_read_ptr, read_info_ptr);
    png_byte **row_pointers = (png_byte **)png_malloc(
        png_read_ptr, 
        height * sizeof(png_byte *)
    );
    
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte *)png_malloc(png_read_ptr, rowbytes);
    }
    
    // 15. Read image
    png_read_image(png_read_ptr, row_pointers);
    
    // 16. Read end
    png_read_end(png_read_ptr, read_end_info_ptr);
    
    // 17. Create write struct
    png_struct *png_write_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        nullptr, 
        nullptr
    );
    if (!png_write_ptr) {
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_read_ptr, row_pointers[y]);
        }
        png_free(png_read_ptr, row_pointers);
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, &read_end_info_ptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 18. Create write info struct
    png_info *write_info_ptr = png_create_info_struct(png_write_ptr);
    if (!write_info_ptr) {
        png_destroy_write_struct(&png_write_ptr, nullptr);
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_read_ptr, row_pointers[y]);
        }
        png_free(png_read_ptr, row_pointers);
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, &read_end_info_ptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 19. Set up error handling for write
    if (setjmp(png_jmpbuf(png_write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_read_ptr, row_pointers[y]);
        }
        png_free(png_read_ptr, row_pointers);
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, &read_end_info_ptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 20. Initialize IO for writing
    png_init_io(png_write_ptr, out_file);
    
    // 21. Set IHDR for output
    png_set_IHDR(png_write_ptr, write_info_ptr, width, height, bit_depth,
                 color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    
    // 22. Write info
    png_write_info(png_write_ptr, write_info_ptr);
    
    // 23. Write rows
    png_write_rows(png_write_ptr, row_pointers, height);
    
    // 24. Write PNG (this will write end info)
    png_write_png(png_write_ptr, write_info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);
    
    // 25. Demonstrate png_save_int_32
    png_byte int_buffer[4];
    png_save_int_32(int_buffer, 123456789);
    
    // 26. Clean up write structures
    png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
    
    // 27. Free allocated memory for rows
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_read_ptr, row_pointers[y]);
    }
    png_free(png_read_ptr, row_pointers);
    
    // 28. Clean up read structures
    png_destroy_read_struct(&png_read_ptr, &read_info_ptr, &read_end_info_ptr);
    
    // 29. Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}