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
//<ID> 376
//<Prompt> []
/*<Combination>: [
*/
//<score> 16.5, nr_unique_branch: 0
//<Quality> {"density":33,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_chunk_malloc_max","png_get_next_frame_delay_den","png_get_uint_31","png_benign_error","png_get_sCAL_fixed","png_get_image_width","png_get_image_height","png_get_rowbytes","png_read_update_info","png_malloc","png_malloc","png_set_rows","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_write_info_before_PLTE","png_save_uint_16","png_destroy_write_struct","png_destroy_write_struct","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_chunk_malloc_max","png_get_next_frame_delay_den","png_get_uint_31","png_benign_error","png_get_sCAL_fixed","png_get_image_width","png_get_image_height","png_get_rowbytes","png_read_update_info","png_malloc","png_malloc","png_set_rows","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_write_info_before_PLTE","png_save_uint_16","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct"],"visited":1}
/**/


// Include necessary libpng headers
extern "C" {
}

// Custom error handling function
static void  id_000029_libpng_error_handler(png_structp png_ptr, const char* error_msg) {
    // Jump back to the setjmp point
    png_longjmp(png_ptr, 1);
}

// Custom warning handler
static void  id_000029_libpng_warning_handler(png_structp png_ptr, const char* warning_msg) {
    // Could log warnings here if needed
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE pointer from the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create read structure
    png_structp png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
         id_000029_libpng_error_handler, 
         id_000029_libpng_warning_handler
    );
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Create end info structure
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set up longjmp for error handling
    jmp_buf* jmpbuf = png_set_longjmp_fn(png_ptr, longjmp, sizeof(jmp_buf));
    if (!jmpbuf || setjmp(*jmpbuf)) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, in_file);

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // Call additional required APIs
    // Get chunk malloc max
    png_alloc_size_t chunk_malloc_max = png_get_chunk_malloc_max(png_ptr);
    
    // Try to get next frame delay denominator (for APNG)
    png_uint_16 delay_den = png_get_next_frame_delay_den(png_ptr, info_ptr);
    
    // Get a 31-bit unsigned integer from the beginning of data
    png_uint_32 uint31_val = 0;
    if (size >= 4) {
        uint31_val = png_get_uint_31(png_ptr, data);
    }
    
    // Report a benign error (just for demonstration)
    png_benign_error(png_ptr, "Fuzzing in progress");
    
    // Try to get sCAL chunk data
    int unit = 0;
    png_fixed_point width = 0, height = 0;
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width, &height);
    
    // Get image dimensions
    png_uint_32 width_val = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height_val = png_get_image_height(png_ptr, info_ptr);
    
    // Get row bytes
    size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    // Update info (apply transformations)
    png_read_update_info(png_ptr, info_ptr);
    
    // Allocate memory for row pointers
    png_bytep* row_pointers = (png_bytep*)png_malloc(png_ptr, 
        sizeof(png_bytep) * height_val);
    
    // Allocate memory for each row
    for (png_uint_32 y = 0; y < height_val; y++) {
        row_pointers[y] = (png_byte*)png_malloc(png_ptr, rowbytes);
    }
    
    // Set rows in info structure
    png_set_rows(png_ptr, info_ptr, row_pointers);
    
    // Read the image data
    png_read_image(png_ptr, row_pointers);
    
    // Read end of PNG
    png_read_end(png_ptr, end_info);
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Create write structure
        png_structp write_ptr = png_create_write_struct(
            PNG_LIBPNG_VER_STRING, 
            nullptr, 
             id_000029_libpng_error_handler, 
             id_000029_libpng_warning_handler
        );
        
        if (write_ptr) {
            png_infop write_info = png_create_info_struct(write_ptr);
            
            if (write_info && !setjmp(png_jmpbuf(write_ptr))) {
                png_init_io(write_ptr, out_file);
                
                // Write info before PLTE
                png_write_info_before_PLTE(write_ptr, info_ptr);
                
                // Demonstrate png_save_uint_16
                png_byte buf[2];
                png_save_uint_16(buf, 1234);
                
                // Clean up write structures
                png_destroy_write_struct(&write_ptr, &write_info);
            } else {
                if (write_info) {
                    png_destroy_write_struct(&write_ptr, &write_info);
                } else {
                    png_destroy_write_struct(&write_ptr, nullptr);
                }
            }
        }
        
        assert_file_closed(&out_file);;
    }
    
    // Free allocated memory for rows
    for (png_uint_32 y = 0; y < height_val; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}