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
//<ID> 518
//<Prompt> []
/*<Combination>: [
*/
//<score> 168, nr_unique_branch: 31
//<Quality> {"density":28,"unique_branches":{"png_get_rowbytes":[[42,8,42,23,0,0,4,0],[42,27,42,43,0,0,4,0]],"png_get_y_pixels_per_meter":[[152,8,152,23,0,0,4,0],[152,27,152,43,0,0,4,0],[153,8,153,46,0,0,4,0],[153,8,153,46,0,0,4,1],[155,11,155,59,0,0,4,0]],"png_get_cHRM":[[533,8,533,23,0,0,4,0],[533,27,533,43,0,0,4,0],[534,8,534,46,0,0,4,0],[534,8,534,46,0,0,4,1],[536,11,536,25,0,0,4,0],[538,11,538,25,0,0,4,0],[540,11,540,25,0,0,4,0],[542,11,542,25,0,0,4,0],[544,11,544,25,0,0,4,0],[546,11,546,25,0,0,4,0],[548,11,548,25,0,0,4,0],[550,11,550,25,0,0,4,0]],"png_handle_sPLT":[[1585,8,1585,42,0,0,4,1],[1590,31,1590,43,0,0,4,1],[1596,8,1596,19,0,0,4,0],[1603,18,1603,40,0,0,4,1],[1610,8,1610,53,0,0,4,1],[1619,8,1619,19,0,0,4,1],[1630,8,1630,35,0,0,4,1],[1636,16,1636,40,0,0,4,1]],"png_build_grayscale_palette":[[866,7,866,13,0,0,4,0],[871,7,871,13,0,0,4,0],[876,7,876,13,0,0,4,0],[881,7,881,13,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_read_fn","png_get_io_ptr","png_destroy_read_struct","png_read_info","png_get_image_width","png_get_image_height","png_get_y_pixels_per_meter","png_get_color_type","png_get_bit_depth","png_get_interlace_type","png_get_filter_type","png_get_rowbytes","png_malloc_warn","png_free","png_get_PLTE","png_get_oFFs","png_get_cHRM","png_malloc_warn","png_build_grayscale_palette","png_free","png_set_swap","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_set_read_fn","png_get_io_ptr","png_read_info","png_get_image_width","png_get_image_height","png_get_y_pixels_per_meter","png_get_color_type","png_get_bit_depth","png_get_interlace_type","png_get_filter_type","png_get_rowbytes","png_malloc_warn","png_free","png_get_PLTE","png_get_oFFs","png_get_cHRM","png_malloc_warn","png_build_grayscale_palette","png_free","png_set_swap","png_destroy_read_struct"],"visited":0}
/**/


// Custom read function that reads from memory buffer
static void  id_000016_read_from_memory(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) {
        png_error(png_ptr, "No IO pointer");
        return;
    }
    
    struct mem_buffer {
        const uint8_t *data;
        size_t size;
        size_t offset;
    };
    
    mem_buffer *buffer = (mem_buffer *)io_ptr;
    
    if (buffer->offset + length > buffer->size) {
        png_error(png_ptr, "Read beyond buffer");
        return;
    }
    
    memcpy(data, buffer->data + buffer->offset, length);
    buffer->offset += length;
}

// Custom error handler
static void  id_000016_libpng_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    // Longjmp will be called from here
    longjmp(png_jmpbuf(png_ptr), 1);
}

// Custom warning handler
static void  id_000016_libpng_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Just ignore warnings
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // Skip if input is too small (minimum PNG header size)
    if (size < 8) {
        return 0;
    }
    
    // Set up memory buffer for reading
    struct mem_buffer {
        const uint8_t *data;
        size_t size;
        size_t offset;
    };
    
    mem_buffer buffer = {data, size, 0};
    
    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, 
                                                 NULL, 
                                                  id_000016_libpng_error_handler, 
                                                  id_000016_libpng_warning_handler);
    if (!png_ptr) {
        return 0;
    }
    
    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    // Set up longjmp for error handling
    jmp_buf *jmp_buf_ptr = png_set_longjmp_fn(png_ptr, longjmp, sizeof(jmp_buf));
    if (!jmp_buf_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Set error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Set custom read function
    png_set_read_fn(png_ptr, &buffer,  id_000016_read_from_memory);
    
    // Get IO pointer to verify
    void *io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr != &buffer) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Get various image properties
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_uint_32 y_pixels_per_meter = png_get_y_pixels_per_meter(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    png_byte filter_type = png_get_filter_type(png_ptr, info_ptr);
    
    // Allocate memory using png_malloc_warn
    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    png_bytep row_buffer = (png_bytep)png_malloc_warn(png_ptr, row_bytes);
    if (row_buffer) {
        // Initialize with zeros
        memset(row_buffer, 0, row_bytes);
        // Free the allocated memory
        png_free(png_ptr, row_buffer);
    }
    
    // Get PLTE palette if present
    png_colorp palette = NULL;
    int num_palette = 0;
    png_uint_32 plte_status = png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);
    
    // Get oFFs offsets if present
    png_int_32 offset_x = 0, offset_y = 0;
    int unit_type = 0;
    png_uint_32 offs_status = png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, &unit_type);
    
    // Get cHRM chromaticities if present
    double white_x = 0, white_y = 0;
    double red_x = 0, red_y = 0;
    double green_x = 0, green_y = 0;
    double blue_x = 0, blue_y = 0;
    png_uint_32 chrm_status = png_get_cHRM(png_ptr, info_ptr, 
                                           &white_x, &white_y,
                                           &red_x, &red_y,
                                           &green_x, &green_y,
                                           &blue_x, &blue_y);
    
    // Build grayscale palette if needed
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        int max_palette = 1 << bit_depth;
        png_colorp gray_palette = (png_colorp)png_malloc_warn(png_ptr, 
                                                            max_palette * sizeof(png_color));
        if (gray_palette) {
            png_build_grayscale_palette(bit_depth, gray_palette);
            png_free(png_ptr, gray_palette);
        }
    }
    
    // Set swap for 16-bit images
    if (bit_depth == 16) {
        png_set_swap(png_ptr);
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    return 0;
}