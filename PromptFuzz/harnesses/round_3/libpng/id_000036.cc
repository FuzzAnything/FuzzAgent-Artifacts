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
//<ID> 740
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_acTL","png_get_next_frame_height","png_get_next_frame_fcTL","png_set_quantize","png_set_acTL","png_set_sCAL_s","png_read_frame_head","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_acTL","png_get_next_frame_height","png_get_next_frame_fcTL","png_set_quantize","png_set_acTL","png_set_sCAL_s","png_read_frame_head","png_destroy_read_struct"],"visited":0}
/*Here's a C++ fuzzer that uses the specified libpng APIs to process APNG (Animated PNG) files:

*/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<40) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Too small to be a valid PNG

    // Create input FILE from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Initialize PNG structures
    png_struct *png_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
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
    png_read_info(png_ptr, info_ptr);

    // Check if it's an APNG by trying to get animation control
    png_uint_32 num_frames = 0, num_plays = 0;
    png_get_acTL(png_ptr, info_ptr, &num_frames, &num_plays);

    // If it's an APNG, process frame information
    if (num_frames > 0) {
        // Get next frame height
        png_uint_32 next_frame_height = png_get_next_frame_height(png_ptr, info_ptr);
        
        // Get next frame control data
        png_uint_32 width = 0, height = 0, x_offset = 0, y_offset = 0;
        png_uint_16 delay_num = 0, delay_den = 0;
        png_byte dispose_op = 0, blend_op = 0;
        
        png_get_next_frame_fcTL(png_ptr, info_ptr, &width, &height, &x_offset, 
                               &y_offset, &delay_num, &delay_den, &dispose_op, &blend_op);

        // Set up quantization parameters
        png_color palette[256];
        png_uint_16 histogram[256] = {0};
        int num_palette = 256;
        int maximum_colors = 256;
        
        // Initialize palette with some values
        for (int i = 0; i < 256; i++) {
            palette[i].red = i;
            palette[i].green = i;
            palette[i].blue = i;
            histogram[i] = i;
        }
        
        png_set_quantize(png_ptr, palette, num_palette, maximum_colors, 
                        histogram, fuzz_int32_t_2);  // full_quantize = 1

        // Set animation control
        png_set_acTL(png_ptr, info_ptr, fuzz_uint32_t_3, fuzz_uint32_t_4);

        // Set physical scale (sCAL chunk)
        const char *swidth = "1.0";
        const char *sheight = "1.0";
        png_set_sCAL_s(png_ptr, info_ptr, fuzz_int32_t_7, fuzz_str_5, fuzz_str_6);

        // Read frame header for animated PNG
        png_read_frame_head(png_ptr, info_ptr);
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}