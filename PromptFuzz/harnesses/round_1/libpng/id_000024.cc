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
//<ID> 379
//<Prompt> []
/*<Combination>: [
*/
//<score> 42.666668, nr_unique_branch: 7
//<Quality> {"density":32,"unique_branches":{"png_get_IHDR":[[950,8,950,32,0,0,4,1],[953,8,953,27,0,0,4,1],[956,8,956,30,0,0,4,1]],"png_ascii_from_fixed":[[2654,20,2654,31,0,0,4,0],[2659,17,2659,27,0,0,4,1]],"png_get_int_32_checked":[[1198,8,1198,32,0,0,4,1],[1202,8,1202,32,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_read_info","png_get_IHDR","png_create_write_struct_2","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_set_IHDR","png_set_sCAL_fixed","png_save_uint_16","png_set_alpha_mode_fixed","png_set_shift","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_free","png_write_end","png_read_end","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_IHDR","png_create_write_struct_2","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_set_sCAL_fixed","png_save_uint_16","png_set_alpha_mode_fixed","png_set_shift","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_free","png_write_end","png_read_end","png_destroy_read_struct","png_destroy_write_struct"],"visited":2}
/**/


// Include libpng headers (adjust path if needed)

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need enough data for meaningful PNG operations
    
    // Create read structure
    png_structp png_ptr_r = png_create_read_struct(fuzz_str_1, 
                                                    nullptr, nullptr, nullptr);
    if (!png_ptr_r) {
	return 0;
	}
	
    
    png_infop info_ptr_r = png_create_info_struct(png_ptr_r);
    if (!info_ptr_r) {
        png_destroy_read_struct(&png_ptr_r, nullptr, nullptr);
        return 0;
    }
    
    // Setup error handling
    if (setjmp(png_jmpbuf(png_ptr_r))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        return 0;
    }
    
    // Read input PNG data from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_init_io(png_ptr_r, in_file);
    png_read_info(png_ptr_r, info_ptr_r);
    
    // Get image info
    png_uint_32 width, height;
    int bit_depth, color_type;
    png_get_IHDR(png_ptr_r, info_ptr_r, &width, &height, &bit_depth, 
                 &color_type, nullptr, nullptr, nullptr);
    
    // Create write structure using png_create_write_struct_2 (as specified)
    png_structp png_ptr_w = png_create_write_struct_2(
        fuzz_str_2,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr);
    if (!png_ptr_w) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop info_ptr_w = png_create_info_struct(png_ptr_w);
    if (!info_ptr_w) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        png_destroy_write_struct(&png_ptr_w, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    if (setjmp(png_jmpbuf(png_ptr_w))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        png_destroy_write_struct(&png_ptr_w, &info_ptr_w);
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Write to output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        png_destroy_write_struct(&png_ptr_w, &info_ptr_w);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_init_io(png_ptr_w, out_file);
    
    // Copy basic image info to write structure
    png_set_IHDR(png_ptr_w, info_ptr_w, width, height, bit_depth, 
                 color_type, fuzz_int32_t_3, 
                 fuzz_int32_t_4, fuzz_int32_t_5);
    
    // 1. Use png_set_sCAL_fixed to set physical dimensions
    png_set_sCAL_fixed(png_ptr_w, info_ptr_w, fuzz_int32_t_6, 
                       png_fixed_point(100000), png_fixed_point(100000));
    
    // 2. Use png_save_uint_16 to prepare buffer
    png_byte buf[2];
    png_save_uint_16(buf, fuzz_uint32_t_7); // Example value
    
    // 3. Use png_set_alpha_mode_fixed to set alpha handling
    png_set_alpha_mode_fixed(png_ptr_w, fuzz_int32_t_8, 
                             png_fixed_point(45455)); // Gamma ~1/2.2
    
    // 4. Use png_set_shift to adjust bit depth
    png_color_8 true_bits;
    true_bits.red = 8;
    true_bits.green = 8;
    true_bits.blue = 8;
    true_bits.gray = 8;
    true_bits.alpha = 8;
    png_set_shift(png_ptr_w, &true_bits);
    
    // Write info
    png_write_info(png_ptr_w, info_ptr_w);
    
    // Read and write image data
    png_bytep row = (png_bytep)png_malloc(png_ptr_r, 
                                          png_get_rowbytes(png_ptr_r, info_ptr_r));
    
    for (png_uint_32 y = 0; y < height; y++) {
        png_read_row(png_ptr_r, row, nullptr);
        png_write_row(png_ptr_w, row);
    }
    
    png_free(png_ptr_r, row);
    
    // Cleanup
    png_write_end(png_ptr_w, nullptr);
    png_read_end(png_ptr_r, info_ptr_r);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
    png_destroy_write_struct(&png_ptr_w, &info_ptr_w);
    
    // Remove output file to clean up
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}