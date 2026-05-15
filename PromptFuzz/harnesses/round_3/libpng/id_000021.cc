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
//<ID> 266
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_read_info","png_get_pHYs_dpi","png_get_valid","png_get_next_frame_blend_op","png_get_color_type","png_get_PLTE","png_get_bit_depth","png_build_grayscale_palette","png_get_hIST","png_read_image","png_read_end","png_destroy_read_struct","png_free"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_read_info","png_get_pHYs_dpi","png_get_valid","png_get_next_frame_blend_op","png_get_color_type","png_get_PLTE","png_get_bit_depth","png_build_grayscale_palette","png_get_hIST","png_read_image","png_read_end","png_destroy_read_struct","png_free"],"visited":0}
/*Looking at these APIs, I can see they're related to reading PNG metadata and handling animated PNGs (APNG). Let me create a fuzzer that reads PNG data and extracts various metadata properties.

*/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
	//fuzzer shim end}




    // Create a FILE pointer from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Check if data has PNG signature
    png_byte header[8];
    if (fread(header, 1, 8, in_file) != 8) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    if (png_sig_cmp(header, fuzz_uint64_t_1, 8) != 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;  // Not a PNG file
    }
    
    rewind(in_file);

    // Create PNG read structure
    png_struct *png_ptr = png_create_read_struct(fuzz_str_2, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Create PNG info structure
    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling with longjmp
    jmp_buf *jmp_buf_ptr = png_set_longjmp_fn(png_ptr, longjmp, sizeof(jmp_buf));
    
    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize PNG I/O
    png_init_io(png_ptr, in_file);
    png_set_sig_bytes(png_ptr, 8);

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // Extract various metadata using the specified APIs
    
    // 1. Get physical dimensions (DPI)
    png_uint_32 res_x, res_y;
    int unit_type;
    png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    
    // 2. Get next frame blend operation (for APNG)
    png_byte blend_op = 0;
    if (png_get_valid(png_ptr, info_ptr, fuzz_uint32_t_3)) {
        // This is an APNG, try to get blend operation
        blend_op = png_get_next_frame_blend_op(png_ptr, info_ptr);
    }
    
    // 3. Get palette max (if palette image)
    int palette_max = 0;
    if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_PALETTE) {
        // png_get_palette_max is not in the provided list, but we can use
        // png_get_PLTE to get palette info
        png_color *palette;
        int num_palette;
        if (png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette)) {
            palette_max = num_palette;
        }
    }
    
    // 4. Build grayscale palette (if needed)
    png_color grayscale_palette[256];
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    if (bit_depth <= 8) {
        png_build_grayscale_palette(bit_depth, grayscale_palette);
    }
    
    // 5. Get histogram (if present)
    png_uint_16 *hist = nullptr;
    png_uint_32 hist_result = png_get_hIST(png_ptr, info_ptr, &hist);
    if (hist_result && hist) {
        // Histogram data is available, we can process it
        // For fuzzing, we just verify it's accessible
    }
    
    // Read the image data
    png_read_image(png_ptr, nullptr);
    
    // Read end of PNG
    png_read_end(png_ptr, info_ptr);

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    // Free any histogram data
    if (hist) {
        png_free(png_ptr, hist);
    }
    
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}