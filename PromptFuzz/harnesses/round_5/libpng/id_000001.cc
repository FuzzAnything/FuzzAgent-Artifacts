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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 0
//<Quality> {"density":42,"unique_branches":{},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_read_info","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_set_alpha_mode","png_set_tRNS_to_alpha","png_get_progressive_ptr","png_get_y_pixels_per_meter","png_set_oFFs","png_get_y_offset_pixels","png_read_update_info","png_malloc","png_destroy_read_struct","png_get_rowbytes","png_malloc","png_free","png_free","png_destroy_read_struct","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_set_alpha_mode","png_set_tRNS_to_alpha","png_set_oFFs","png_write_info","png_write_image","png_write_end","png_destroy_info_struct","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_read_info","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_set_alpha_mode","png_set_tRNS_to_alpha","png_get_progressive_ptr","png_get_y_pixels_per_meter","png_set_oFFs","png_get_y_offset_pixels","png_read_update_info","png_malloc","png_get_rowbytes","png_malloc","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_set_alpha_mode","png_set_tRNS_to_alpha","png_set_oFFs","png_write_info","png_write_image","png_write_end","png_destroy_info_struct","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to process PNG image data:

*/


// Include libpng headers

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
        return 0;  // Not enough data for PNG signature
    }

    // Check PNG signature
    if (png_sig_cmp((png_bytep)data, 0, 8) != 0) {
        return 0;  // Not a valid PNG
    }

    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create read structure
    png_struct *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Create info structure
    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize PNG reading
    png_init_io(png_ptr, in_file);
    png_set_sig_bytes(png_ptr, 8);  // We already checked 8 bytes

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // Get image dimensions
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);

    // Apply transformations using the required APIs
    // 1. Set alpha mode
    png_set_alpha_mode(png_ptr, PNG_ALPHA_PNG, 2.2);
    
    // 2. Convert tRNS to alpha
    png_set_tRNS_to_alpha(png_ptr);
    
    // 3. Get progressive pointer (if using progressive reading)
    void *progressive_ptr = png_get_progressive_ptr(png_ptr);
    (void)progressive_ptr;  // Use to avoid unused variable warning
    
    // 4. Get y pixels per meter
    png_uint_32 y_pixels_per_meter = png_get_y_pixels_per_meter(png_ptr, info_ptr);
    (void)y_pixels_per_meter;  // Use to avoid unused variable warning
    
    // 5. Set offsets (oFFs chunk)
    png_set_oFFs(png_ptr, info_ptr, 100, 100, PNG_OFFSET_PIXEL);
    
    // 6. Get y offset in pixels
    png_int_32 y_offset_pixels = png_get_y_offset_pixels(png_ptr, info_ptr);
    (void)y_offset_pixels;  // Use to avoid unused variable warning
    
    // 7. Get x offset in inches (fixed point)
    png_fixed_point x_offset_inches_fixed = 0;
    // Note: png_get_x_offset_inches_fixed is not in the provided API list,
    // but we'll use a similar approach with available APIs
    // We'll use png_get_oFFs to get the offsets and calculate if needed
    
    // Update info after transformations
    png_read_update_info(png_ptr, info_ptr);
    
    // Allocate memory for row pointers
    png_bytep *row_pointers = (png_bytep *)png_malloc(png_ptr, 
                                                      sizeof(png_bytep) * height);
    if (!row_pointers) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Calculate row size
    png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    // Allocate memory for each row
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte *)png_malloc(png_ptr, rowbytes);
        if (!row_pointers[y]) {
            // Clean up allocated rows
            for (png_uint_32 i = 0; i < y; i++) {
                png_free(png_ptr, row_pointers[i]);
            }
            png_free(png_ptr, row_pointers);
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // Read the image data
    png_read_image(png_ptr, row_pointers);
    
    // Read the end of PNG file
    png_read_end(png_ptr, nullptr);
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create output file for writing processed image
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file) {
        // Create write structure
        png_struct *write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                       nullptr, nullptr, nullptr);
        if (write_ptr) {
            png_info *write_info = png_create_info_struct(write_ptr);
            if (write_info) {
                if (!setjmp(png_jmpbuf(write_ptr))) {
                    png_init_io(write_ptr, out_file);
                    
                    // Set image dimensions and type
                    png_set_IHDR(write_ptr, write_info, width, height,
                                bit_depth, color_type, PNG_INTERLACE_NONE,
                                PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
                    
                    // Apply the same transformations
                    png_set_alpha_mode(write_ptr, PNG_ALPHA_PNG, 2.2);
                    png_set_tRNS_to_alpha(write_ptr);
                    png_set_oFFs(write_ptr, write_info, 100, 100, PNG_OFFSET_PIXEL);
                    
                    // Write info
                    png_write_info(write_ptr, write_info);
                    
                    // Write image data
                    png_write_image(write_ptr, row_pointers);
                    
                    // Write end
                    png_write_end(write_ptr, nullptr);
                }
                png_destroy_info_struct(write_ptr, &write_info);
            }
            png_destroy_write_struct(&write_ptr, nullptr);
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up allocated memory
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);
    
    // Clean up libpng structures
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    // Close input file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}