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
//<ID> 91
//<Prompt> []
/*<Combination>: [
*/
//<score> 125, nr_unique_branch: 32
//<Quality> {"density":25,"unique_branches":{"png_set_rgb_to_gray_fixed":[[1039,7,1039,33,0,0,4,0],[1055,8,1055,53,0,0,4,0],[1055,8,1055,53,0,0,4,1],[1070,11,1070,19,0,0,4,0],[1070,23,1070,33,0,0,4,0],[1070,37,1070,60,0,0,4,0]],"png_handle_IEND":[[1077,8,1077,19,0,0,4,0]],"png_rtran_ok":[[106,29,106,65,0,0,4,1]],"png_handle_pCAL":[[2184,9,2184,36,0,0,4,1]],"png_reset_zstream":[[953,8,953,23,0,0,4,1]],"png_read_png":[[960,8,960,23,0,0,4,1],[960,27,960,43,0,0,4,1],[967,8,967,64,0,0,4,1],[979,8,979,50,0,0,4,1],[993,8,993,50,0,0,4,1],[1003,8,1003,53,0,0,4,1],[1013,8,1013,49,0,0,4,1],[1023,8,1023,50,0,0,4,1],[1035,8,1035,48,0,0,4,1],[1047,8,1047,53,0,0,4,1],[1058,8,1058,47,0,0,4,1],[1067,8,1067,45,0,0,4,1],[1075,8,1075,52,0,0,4,1],[1083,8,1083,53,0,0,4,1],[1092,8,1092,54,0,0,4,1],[1101,8,1101,53,0,0,4,1],[1109,8,1109,51,0,0,4,1],[1132,8,1132,38,0,0,4,1]],"png_read_info":[[139,16,139,52,0,0,4,0]],"png_handle_PLTE":[[981,13,981,49,0,0,4,0]],"png_set_flush":[[970,8,970,23,0,0,4,1],[973,27,973,36,0,0,4,1]]},"library_calls":["png_create_read_struct_2","png_create_info_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_set_oFFs","png_set_rgb_to_gray_fixed","png_set_flush","png_reset_zstream","png_read_png","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_rows","png_malloc","png_image_write_to_memory","png_malloc","png_image_write_to_memory","png_free","png_free","png_destroy_read_struct"],"critical_calls":["png_create_read_struct_2","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_set_oFFs","png_set_rgb_to_gray_fixed","png_set_flush","png_reset_zstream","png_read_png","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_rows","png_malloc","png_image_write_to_memory","png_malloc","png_image_write_to_memory","png_free","png_free","png_destroy_read_struct"],"visited":1}
/*Looking at the provided APIs, I'll create a fuzz driver that reads a PNG image from memory, applies various transformations using the specified APIs, and then writes it back to memory.

*/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE pointer from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize libpng structures
    png_struct *png_ptr = nullptr;
    png_info *info_ptr = nullptr;
    png_info *end_info = nullptr;

    // Set up error handling
    png_ptr = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling using setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, in_file);

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // Apply transformations using the specified APIs
    
    // 1. Use png_set_oFFs to set image offsets
    png_set_oFFs(png_ptr, info_ptr, 10, 20, PNG_OFFSET_PIXEL);
    
    // 2. Use png_set_rgb_to_gray_fixed to convert RGB to grayscale
    // Using standard coefficients for RGB to grayscale conversion
    png_set_rgb_to_gray_fixed(png_ptr, PNG_ERROR_ACTION_NONE, 29900, 58700);
    
    // 3. Use png_set_flush to set flush behavior
    png_set_flush(png_ptr, 10);  // Flush every 10 rows
    
    // 4. Use png_reset_zstream to reset compression stream
    png_reset_zstream(png_ptr);
    
    // Read the entire PNG image
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);
    
    // Get image information
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Get row pointers
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
    
    // Prepare for writing the image back to memory
    png_image image_write;
    memset(&image_write, 0, sizeof(image_write));
    image_write.version = PNG_IMAGE_VERSION;
    image_write.width = width;
    image_write.height = height;
    
    // Set format based on color type and bit depth
    if (color_type == PNG_COLOR_TYPE_GRAY) {
        if (bit_depth == 16) {
            image_write.format = PNG_FORMAT_GRAY;
        } else {
            image_write.format = PNG_FORMAT_GRAY;
        }
    } else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        image_write.format = PNG_FORMAT_GA;
    } else if (color_type == PNG_COLOR_TYPE_RGB) {
        if (bit_depth == 16) {
            image_write.format = PNG_FORMAT_RGB;
        } else {
            image_write.format = PNG_FORMAT_RGB;
        }
    } else if (color_type == PNG_COLOR_TYPE_RGBA) {
        image_write.format = PNG_FORMAT_RGBA;
    } else if (color_type == PNG_COLOR_TYPE_PALETTE) {
        image_write.format = PNG_FORMAT_FLAG_COLORMAP | PNG_FORMAT_FLAG_ALPHA;
    }
    
    // Calculate row stride
    png_int_32 row_stride = 0;
    if (color_type == PNG_COLOR_TYPE_GRAY) {
        row_stride = (bit_depth == 16) ? width * 2 : width;
    } else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        row_stride = width * 2 * ((bit_depth == 16) ? 2 : 1);
    } else if (color_type == PNG_COLOR_TYPE_RGB) {
        row_stride = width * 3 * ((bit_depth == 16) ? 2 : 1);
    } else if (color_type == PNG_COLOR_TYPE_RGBA) {
        row_stride = width * 4 * ((bit_depth == 16) ? 2 : 1);
    } else if (color_type == PNG_COLOR_TYPE_PALETTE) {
        row_stride = width;
    }
    
    // Create a contiguous buffer from row pointers
    size_t buffer_size = height * row_stride;
    png_byte *buffer = nullptr;
    
    if (buffer_size > 0) {
        buffer = (png_byte *)png_malloc(png_ptr, buffer_size);
        if (buffer) {
            for (png_uint_32 y = 0; y < height; y++) {
                memcpy(buffer + y * row_stride, row_pointers[y], row_stride);
            }
            
            // 5. Use png_image_write_to_memory to write image to memory
            png_alloc_size_t memory_bytes = 0;
            int convert_to_8_bit = 0;
            
            // First call to get required buffer size
            if (png_image_write_to_memory(&image_write, nullptr, &memory_bytes, 
                                          convert_to_8_bit, buffer, row_stride, nullptr)) {
                // Allocate memory for output
                void *output_memory = png_malloc(png_ptr, memory_bytes);
                if (output_memory) {
                    // Write image to memory
                    png_image_write_to_memory(&image_write, output_memory, &memory_bytes,
                                              convert_to_8_bit, buffer, row_stride, nullptr);
                    
                    // Free output memory
                    png_free(png_ptr, output_memory);
                }
            }
            
            // Free the buffer
            png_free(png_ptr, buffer);
        }
    }

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}