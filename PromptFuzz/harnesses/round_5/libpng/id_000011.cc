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
//<ID> 111
//<Prompt> []
/*<Combination>: [
*/
//<score> 160, nr_unique_branch: 17
//<Quality> {"density":40,"unique_branches":{"png_get_gAMA":[[683,8,683,23,0,0,4,0],[683,27,683,43,0,0,4,0],[684,8,684,46,0,0,4,0],[684,8,684,46,0,0,4,1],[686,11,686,29,0,0,4,0]],"png_get_cLLI_fixed":[[794,8,794,23,0,0,4,0],[794,27,794,43,0,0,4,0],[795,8,795,46,0,0,4,0],[795,8,795,46,0,0,4,1],[797,11,797,25,0,0,4,0],[798,11,798,26,0,0,4,0]],"png_set_filler":[[152,8,152,23,0,0,4,1],[158,8,158,49,0,0,4,1],[185,13,185,36,0,0,4,1],[189,13,189,37,0,0,4,0],[190,20,190,43,0,0,4,1],[208,13,208,20,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_init_io","png_set_progressive_frame_fn","png_read_info","png_get_libpng_ver","png_get_gAMA","png_get_x_pixels_per_inch","png_get_cLLI_fixed","png_set_filler","png_set_sCAL","png_set_text_compression_window_bits","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_set_IHDR","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_read_end","png_write_end","png_free","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_init_io","png_set_progressive_frame_fn","png_read_info","png_get_libpng_ver","png_get_gAMA","png_get_x_pixels_per_inch","png_get_cLLI_fixed","png_set_filler","png_set_sCAL","png_set_text_compression_window_bits","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_set_IHDR","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_read_end","png_write_end","png_free","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/


extern "C" {
}

// Progressive reading callbacks
static void  id_000011_frame_info_callback(png_structp png_ptr, png_uint_32 frame_num) {
    // Frame info callback - can be empty for fuzzing
}

static void  id_000011_frame_end_callback(png_structp png_ptr, png_uint_32 frame_num) {
    // Frame end callback - can be empty for fuzzing
}

// Error handling
static void  id_000011_png_error_handler(png_structp png_ptr, const char* msg) {
    // Error handler for libpng
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void  id_000011_png_warning_handler(png_structp png_ptr, const char* msg) {
    // Warning handler for libpng
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need enough data for PNG header
    
    png_structp png_ptr_read = nullptr;
    png_structp png_ptr_write = nullptr;
    png_infop info_ptr_read = nullptr;
    png_infop info_ptr_write = nullptr;
    FILE* in_file = nullptr;
    FILE* out_file = nullptr;
    
    // 1. Create read structure
    png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr,
                                           id_000011_png_error_handler,  id_000011_png_warning_handler);
    if (!png_ptr_read) {
	return 0;
	}
	
    
    // 2. Create info structure for reading
    info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        return 0;
    }
    
    // 3. Create write structure
    png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr,
                                             id_000011_png_error_handler,  id_000011_png_warning_handler);
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    // 4. Create info structure for writing
    info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, nullptr);
        return 0;
    }
    
    // Set up error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr_read)) || setjmp(png_jmpbuf(png_ptr_write))) {
        // Clean up on error
        if (in_file) assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        return 0;
    }
    
    // 5. Open input file from memory
    in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Open output file
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Set up input source
    png_init_io(png_ptr_read, in_file);
    
    // 8. Set up output destination
    png_init_io(png_ptr_write, out_file);
    
    // 9. Set up progressive reading with frame callbacks
    png_set_progressive_frame_fn(png_ptr_read,  id_000011_frame_info_callback,  id_000011_frame_end_callback);
    
    // 10. Read PNG info
    png_read_info(png_ptr_read, info_ptr_read);
    
    // 11. Get libpng version (API 7)
    const char* libpng_ver = png_get_libpng_ver(png_ptr_read);
    // Use the version string to avoid unused variable warning
    
    // 12. Get gamma value (API 5)
    double file_gamma = 0.0;
    png_uint_32 gamma_result = png_get_gAMA(png_ptr_read, info_ptr_read, &file_gamma);
    
    // 13. Get x pixels per inch (API 2)
    png_uint_32 x_pixels_per_inch = png_get_x_pixels_per_inch(png_ptr_read, info_ptr_read);
    
    // 14. Get cLLI fixed values (API 8)
    png_uint_32 max_content_light = 0;
    png_uint_32 max_frame_average = 0;
    png_uint_32 clli_result = png_get_cLLI_fixed(png_ptr_read, info_ptr_read, 
                                                &max_content_light, &max_frame_average);
    
    // 15. Set filler for write structure (API 1)
    png_set_filler(png_ptr_write, 0xFFFFFFFF, PNG_FILLER_AFTER);
    
    // 16. Set sCAL for write structure (API 3)
    png_set_sCAL(png_ptr_write, info_ptr_write, PNG_SCALE_METER, 1.0, 1.0);
    
    // 17. Set text compression window bits (API 6)
    png_set_text_compression_window_bits(png_ptr_write, 15);
    
    // 18. Copy image info from read to write
    png_uint_32 width = png_get_image_width(png_ptr_read, info_ptr_read);
    png_uint_32 height = png_get_image_height(png_ptr_read, info_ptr_read);
    int bit_depth = png_get_bit_depth(png_ptr_read, info_ptr_read);
    int color_type = png_get_color_type(png_ptr_read, info_ptr_read);
    
    // 19. Set IHDR for output
    png_set_IHDR(png_ptr_write, info_ptr_write, width, height, bit_depth,
                 color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);
    
    // 20. Write info to output
    png_write_info(png_ptr_write, info_ptr_write);
    
    // 21. Read image rows and write them
    png_bytep row = (png_bytep)png_malloc(png_ptr_read, png_get_rowbytes(png_ptr_read, info_ptr_read));
    
    for (png_uint_32 y = 0; y < height; y++) {
        png_read_row(png_ptr_read, row, nullptr);
        png_write_row(png_ptr_write, row);
    }
    
    // 22. Read and write end
    png_read_end(png_ptr_read, info_ptr_read);
    png_write_end(png_ptr_write, info_ptr_write);
    
    // 23. Clean up
    png_free(png_ptr_read, row);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}