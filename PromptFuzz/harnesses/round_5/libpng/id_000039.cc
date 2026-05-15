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
//<ID> 545
//<Prompt> []
/*<Combination>: [
*/
//<score> 176, nr_unique_branch: 3
//<Quality> {"density":44,"unique_branches":{"png_get_rgb_to_gray_status":[[1266,22,1266,29,0,0,4,0]],"png_destroy_write_struct":[[1055,11,1055,26,0,0,4,1]],"png_benign_error":[[314,13,314,37,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_chunk_malloc_max","png_get_next_frame_delay_den","png_get_rgb_to_gray_status","png_get_uint_31","png_get_sCAL_fixed","png_get_image_width","png_get_image_height","png_get_rowbytes","png_read_update_info","png_get_gAMA_fixed","png_malloc","png_malloc","png_get_rowbytes","png_set_rows","png_read_image","png_read_end","png_create_write_struct","png_free","png_free","png_destroy_read_struct","png_create_info_struct","png_free","png_free","png_destroy_write_struct","png_destroy_read_struct","png_free","png_free","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_write_info_before_PLTE","png_save_uint_16","png_get_iCCP","png_free","png_free","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_chunk_malloc_max","png_get_next_frame_delay_den","png_get_rgb_to_gray_status","png_get_uint_31","png_get_sCAL_fixed","png_get_image_width","png_get_image_height","png_get_rowbytes","png_read_update_info","png_get_gAMA_fixed","png_malloc","png_malloc","png_get_rowbytes","png_set_rows","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_init_io","png_write_info_before_PLTE","png_save_uint_16","png_get_iCCP","png_free","png_free","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/**/


// Error handler functions
static void  id_000039_libpng_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    // This function is called when libpng encounters an error
    // We'll use longjmp to return to our error recovery point
    jmp_buf* buf = (jmp_buf*)png_get_error_ptr(png_ptr);
    if (buf) {
        png_benign_error(png_ptr, error_msg); // Use provided API
        longjmp(*buf, 1);
    }
}

static void  id_000039_libpng_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Optional: handle warnings
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	 // PNG header is 8 bytes
    
    png_structp png_read_ptr = nullptr;
    png_structp png_write_ptr = nullptr;
    png_infop info_read_ptr = nullptr;
    png_infop info_write_ptr = nullptr;
    FILE* in_file = nullptr;
    FILE* out_file = nullptr;
    jmp_buf jmpbuf;
    
    // Create read structure
    png_read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                          &jmpbuf,
                                           id_000039_libpng_error_handler,
                                           id_000039_libpng_warning_handler);
    if (!png_read_ptr) {
	return 0;
	}
	
    
    // Create info structure for reading
    info_read_ptr = png_create_info_struct(png_read_ptr);
    if (!info_read_ptr) {
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up longjmp error handling
    if (setjmp(jmpbuf)) {
	FUZZ_FILE_CLOSE();
        // Error recovery - clean up and exit
        if (out_file) assert_file_closed(&out_file);;
        if (in_file) assert_file_closed(&in_file);;
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
        return 0;
    }
    
    // Set longjmp function
    png_set_longjmp_fn(png_read_ptr, longjmp, sizeof(jmp_buf));
    
    // Create FILE* from input data
    in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O for reading
    png_init_io(png_read_ptr, in_file);
    
    // Read PNG info
    png_read_info(png_read_ptr, info_read_ptr);
    
    // Use various getter functions
    png_alloc_size_t malloc_max = png_get_chunk_malloc_max(png_read_ptr);
    (void)malloc_max;
    
    png_uint_16 delay_den = png_get_next_frame_delay_den(png_read_ptr, info_read_ptr);
    (void)delay_den;
    
    png_byte rgb_to_gray_status = png_get_rgb_to_gray_status(png_read_ptr);
    (void)rgb_to_gray_status;
    
    // Use png_get_uint_31 - need at least 4 bytes of data
    if (size >= 4) {
        png_uint_32 uint31_val = png_get_uint_31(png_read_ptr, (png_bytep)data);
        (void)uint31_val;
    }
    
    // Get sCAL fixed point values
    int unit;
    png_fixed_point width_fixed, height_fixed;
    png_uint_32 scal_result = png_get_sCAL_fixed(png_read_ptr, info_read_ptr, 
                                                 &unit, &width_fixed, &height_fixed);
    (void)scal_result;
    
    // Get image dimensions
    png_uint_32 width = png_get_image_width(png_read_ptr, info_read_ptr);
    png_uint_32 height = png_get_image_height(png_read_ptr, info_read_ptr);
    
    // Get row bytes
    size_t rowbytes = png_get_rowbytes(png_read_ptr, info_read_ptr);
    (void)rowbytes;
    
    // Update info
    png_read_update_info(png_read_ptr, info_read_ptr);
    
    // Get gamma fixed point value
    png_fixed_point gamma_fixed;
    png_uint_32 gamma_result = png_get_gAMA_fixed(png_read_ptr, info_read_ptr, &gamma_fixed);
    (void)gamma_result;
    
    // Allocate memory for image data
    png_bytep* row_pointers = (png_bytep*)png_malloc(png_read_ptr, 
                                                     height * sizeof(png_bytep));
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)png_malloc(png_read_ptr, 
                                                png_get_rowbytes(png_read_ptr, info_read_ptr));
    }
    
    // Set rows
    png_set_rows(png_read_ptr, info_read_ptr, row_pointers);
    
    // Read image
    png_read_image(png_read_ptr, row_pointers);
    
    // Read end
    png_read_end(png_read_ptr, info_read_ptr);
    
    // Create write structure
    png_write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                            &jmpbuf,
                                             id_000039_libpng_error_handler,
                                             id_000039_libpng_warning_handler);
    if (!png_write_ptr) {
        // Clean up read structures
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_read_ptr, row_pointers[y]);
        }
        png_free(png_read_ptr, row_pointers);
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create info structure for writing
    info_write_ptr = png_create_info_struct(png_write_ptr);
    if (!info_write_ptr) {
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_read_ptr, row_pointers[y]);
        }
        png_free(png_read_ptr, row_pointers);
        png_destroy_write_struct(&png_write_ptr, nullptr);
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Open output file
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_read_ptr, row_pointers[y]);
        }
        png_free(png_read_ptr, row_pointers);
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O for writing
    png_init_io(png_write_ptr, out_file);
    
    // Write info before PLTE
    png_write_info_before_PLTE(png_write_ptr, info_read_ptr);
    
    // Use png_save_uint_16
    png_byte buf[2];
    png_save_uint_16(buf, 0x1234);
    (void)buf;
    
    // Get iCCP profile if present
    png_charp name;
    int compression_type;
    png_bytep profile;
    png_uint_32 proflen;
    png_uint_32 iccp_result = png_get_iCCP(png_read_ptr, info_read_ptr, 
                                           &name, &compression_type, 
                                           &profile, &proflen);
    (void)iccp_result;
    
    // Clean up
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Free row pointers
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_read_ptr, row_pointers[y]);
    }
    png_free(png_read_ptr, row_pointers);
    
    // Destroy write structures
    png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
    
    // Destroy read structures
    png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}