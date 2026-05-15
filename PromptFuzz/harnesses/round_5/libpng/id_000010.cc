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
//<ID> 105
//<Prompt> []
/*<Combination>: [
*/
//<score> 49, nr_unique_branch: 0
//<Quality> {"density":49,"unique_branches":{},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_read_info","png_get_IHDR","png_get_rowbytes","png_malloc","png_malloc","png_read_image","png_read_end","png_get_x_offset_pixels","png_get_sCAL","png_get_acTL","png_get_mem_ptr","png_create_write_struct","png_free","png_free","png_destroy_read_struct","png_create_info_struct","png_free","png_free","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct","png_init_io","png_set_IHDR","png_set_cHRM_XYZ","png_set_iCCP","png_write_info","png_write_image","png_write_end","png_write_chunk_end","png_chunk_benign_error","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_read_info","png_get_IHDR","png_get_rowbytes","png_malloc","png_malloc","png_read_image","png_read_end","png_get_x_offset_pixels","png_get_sCAL","png_get_acTL","png_get_mem_ptr","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_set_cHRM_XYZ","png_set_iCCP","png_write_info","png_write_image","png_write_end","png_write_chunk_end","png_chunk_benign_error","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct"],"visited":0}
/*Here's a step-by-step C++ fuzz driver that uses the specified libpng APIs to read a PNG from memory, modify its metadata, and write a new PNG file:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	
    
    // Check PNG signature
    if (png_sig_cmp((png_const_bytep)data, 0, 8) != 0) {
        return 0;
    }

    // Create read structures
    png_structp png_ptr_r = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                    nullptr, 
                                                    nullptr, 
                                                    nullptr);
    if (!png_ptr_r) {
	return 0;
	}
	

    png_infop info_ptr_r = png_create_info_struct(png_ptr_r);
    if (!info_ptr_r) {
        png_destroy_read_struct(&png_ptr_r, nullptr, nullptr);
        return 0;
    }

    // Set up error handling with longjmp
    if (setjmp(png_jmpbuf(png_ptr_r))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        return 0;
    }

    // Set longjmp function
    png_set_longjmp_fn(png_ptr_r, longjmp, sizeof(jmp_buf));

    // Create FILE* for reading from memory
    FILE* in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize IO and read PNG info
    png_init_io(png_ptr_r, in_file);
    png_set_sig_bytes(png_ptr_r, 8);
    png_read_info(png_ptr_r, info_ptr_r);

    // Get basic image information
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(png_ptr_r, info_ptr_r, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);

    // Get rowbytes for memory allocation
    size_t rowbytes = png_get_rowbytes(png_ptr_r, info_ptr_r);
    
    // Allocate memory for image data
    png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr_r, 
                                                     sizeof(png_bytep) * height);
    for (png_uint_32 y = 0; y < height; ++y) {
        row_pointers[y] = (png_bytep)png_malloc(png_ptr_r, rowbytes);
    }

    // Read the image data
    png_read_image(png_ptr_r, row_pointers);
    png_read_end(png_ptr_r, nullptr);

    // Get x offset pixels
    png_int_32 x_offset = png_get_x_offset_pixels(png_ptr_r, info_ptr_r);

    // Get sCAL information
    int unit;
    double width_s, height_s;
    png_get_sCAL(png_ptr_r, info_ptr_r, &unit, &width_s, &height_s);

    // Get acTL information
    png_uint_32 num_frames, num_plays;
    png_get_acTL(png_ptr_r, info_ptr_r, &num_frames, &num_plays);

    // Get memory pointer
    void* mem_ptr = png_get_mem_ptr(png_ptr_r);

    // Create write structures
    png_structp png_ptr_w = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                    nullptr,
                                                    nullptr,
                                                    nullptr);
    if (!png_ptr_w) {
        // Cleanup read structures
        for (png_uint_32 y = 0; y < height; ++y) {
            png_free(png_ptr_r, row_pointers[y]);
        }
        png_free(png_ptr_r, row_pointers);
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }

    png_infop info_ptr_w = png_create_info_struct(png_ptr_w);
    if (!info_ptr_w) {
        // Cleanup read structures
        for (png_uint_32 y = 0; y < height; ++y) {
            png_free(png_ptr_r, row_pointers[y]);
        }
        png_free(png_ptr_r, row_pointers);
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        png_destroy_write_struct(&png_ptr_w, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr_w))) {
	FUZZ_FILE_CLOSE();
        // Cleanup on write error
        png_destroy_write_struct(&png_ptr_w, &info_ptr_w);
        for (png_uint_32 y = 0; y < height; ++y) {
            png_free(png_ptr_r, row_pointers[y]);
        }
        png_free(png_ptr_r, row_pointers);
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    // Create output file
    FILE* out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        png_destroy_write_struct(&png_ptr_w, &info_ptr_w);
        for (png_uint_32 y = 0; y < height; ++y) {
            png_free(png_ptr_r, row_pointers[y]);
        }
        png_free(png_ptr_r, row_pointers);
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize write IO
    png_init_io(png_ptr_w, out_file);

    // Set IHDR for output
    png_set_IHDR(png_ptr_w, info_ptr_w, width, height, bit_depth, color_type,
                 interlace_method, compression_method, filter_method);

    // Set cHRM_XYZ (example values)
    png_set_cHRM_XYZ(png_ptr_w, info_ptr_w,
                     0.6400, 0.3300, 0.2126,  // red
                     0.3000, 0.6000, 0.7152,  // green
                     0.1500, 0.0600, 0.0722); // blue

    // Set iCCP profile (dummy data)
    const char* profile_name = "sRGB IEC61966-2.1";
    png_byte icc_profile[] = {0x00, 0x01, 0x02, 0x03, 0x04};
    png_set_iCCP(png_ptr_w, info_ptr_w, profile_name, PNG_COMPRESSION_TYPE_BASE,
                 icc_profile, sizeof(icc_profile));

    // Write info
    png_write_info(png_ptr_w, info_ptr_w);

    // Write image data
    png_write_image(png_ptr_w, row_pointers);
    png_write_end(png_ptr_w, nullptr);

    // Write chunk end
    png_write_chunk_end(png_ptr_w);

    // Generate benign error (for testing)
    png_chunk_benign_error(png_ptr_w, "Test benign error");

    // Cleanup write structures
    png_destroy_write_struct(&png_ptr_w, &info_ptr_w);
    
    // Cleanup read structures and memory
    for (png_uint_32 y = 0; y < height; ++y) {
        png_free(png_ptr_r, row_pointers[y]);
    }
    png_free(png_ptr_r, row_pointers);
    
    png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}