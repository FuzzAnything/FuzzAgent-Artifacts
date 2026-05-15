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
//<ID> 93
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 3
//<Quality> {"density":30,"unique_branches":{"png_set_strip_16":[[189,8,189,37,0,0,4,1]],"png_set_expand_gray_1_2_4_to_8":[[975,8,975,37,0,0,4,1]],"png_set_gray_to_rgb":[[1016,8,1016,37,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_expand","png_set_strip_16","png_set_gray_to_rgb","png_set_rgb_to_gray_fixed","png_set_oFFs","png_set_flush","png_read_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_malloc","png_get_rowbytes","png_malloc","png_read_image","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_image_write_to_memory","png_malloc","png_image_write_to_memory","png_free","png_reset_zstream","png_destroy_info_struct","png_destroy_write_struct","png_read_end","png_free","png_free","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_expand","png_set_strip_16","png_set_gray_to_rgb","png_set_rgb_to_gray_fixed","png_set_oFFs","png_set_flush","png_read_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_malloc","png_get_rowbytes","png_malloc","png_read_image","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_image_write_to_memory","png_malloc","png_image_write_to_memory","png_free","png_reset_zstream","png_destroy_info_struct","png_destroy_write_struct","png_read_end","png_free","png_free","png_destroy_read_struct"],"visited":1}
/**/


// Include necessary libpng headers

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need enough data for meaningful PNG operations
    
    // Create input FILE* from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize structures
    png_struct *png_ptr = NULL;
    png_info *info_ptr = NULL;
    png_info *end_info = NULL;
    
    // Create read structure with default error handling
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O
    png_init_io(png_ptr, in_file);
    
    // Set transformations
    png_set_expand(png_ptr);  // Expand paletted/grayscale images to RGB
    png_set_strip_16(png_ptr); // Reduce 16-bit to 8-bit
    png_set_gray_to_rgb(png_ptr); // Convert grayscale to RGB
    
    // Call required APIs
    
    // 1. png_set_rgb_to_gray_fixed - set RGB to grayscale conversion
    // Using PNG_ERROR_ACTION_NONE as error action
    png_set_rgb_to_gray_fixed(png_ptr, 1, 21265, 71517);
    
    // 2. png_set_oFFs - set image offsets
    png_set_oFFs(png_ptr, info_ptr, 10, 10, PNG_OFFSET_PIXEL);
    
    // 3. png_set_flush - set flush behavior
    png_set_flush(png_ptr, 10); // Flush every 10 rows
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Get image dimensions
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Read image data
    png_bytep *row_pointers = (png_bytep*)png_malloc(png_ptr, height * sizeof(png_bytep));
    png_size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)png_malloc(png_ptr, row_bytes);
    }
    
    png_read_image(png_ptr, row_pointers);
    
    // 4. png_reset_zstream - reset compression stream
    // This is typically used for writing, so we'll create a write structure
    png_struct *write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (write_ptr) {
        png_info *write_info = png_create_info_struct(write_ptr);
        if (write_info) {
            if (!setjmp(png_jmpbuf(write_ptr))) {
                // 5. png_image_write_to_memory - write image to memory buffer
                // First, create a png_image structure
                png_image image;
                memset(&image, 0, sizeof(image));
                image.version = PNG_IMAGE_VERSION;
                image.width = width;
                image.height = height;
                image.format = PNG_FORMAT_RGB;
                
                // Prepare memory buffer
                png_alloc_size_t memory_bytes = 0;
                // First call to get required buffer size
                int result = png_image_write_to_memory(&image, NULL, &memory_bytes, 
                                                      1,  // convert_to_8_bit
                                                      row_pointers[0], 
                                                      row_bytes,
                                                      NULL);
                
                if (result && memory_bytes > 0) {
                    // Allocate buffer and write image
                    void *memory = png_malloc(png_ptr, memory_bytes);
                    png_image_write_to_memory(&image, memory, &memory_bytes,
                                             1, row_pointers[0], row_bytes, NULL);
                    png_free(png_ptr, memory);
                }
                
                // Reset zstream on write structure
                png_reset_zstream(write_ptr);
            }
            png_destroy_info_struct(write_ptr, &write_info);
        }
        png_destroy_write_struct(&write_ptr, NULL);
    }
    
    // Clean up
    png_read_end(png_ptr, end_info);
    
    // Free row pointers
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);
    
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}