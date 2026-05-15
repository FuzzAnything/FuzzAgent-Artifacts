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
//<ID> 806
//<Prompt> []
/*<Combination>: [
*/
//<score> 560, nr_unique_branch: 52
//<Quality> {"density":40,"unique_branches":{"png_get_x_pixels_per_meter":[[131,8,131,23,0,0,4,0],[131,27,131,43,0,0,4,0],[132,8,132,46,0,0,4,0],[132,8,132,46,0,0,4,1],[134,11,134,59,0,0,4,0]],"png_get_progressive_ptr":[[1138,8,1138,23,0,0,4,1]],"png_xy_from_XYZ":[[1118,8,1118,48,0,0,4,1],[1121,8,1121,62,0,0,4,1],[1123,8,1123,62,0,0,4,1],[1127,8,1127,52,0,0,4,1],[1130,8,1130,68,0,0,4,1],[1132,8,1132,68,0,0,4,1],[1136,8,1136,50,0,0,4,1],[1139,8,1139,65,0,0,4,1],[1141,8,1141,65,0,0,4,1],[1149,8,1149,38,0,0,4,1],[1157,8,1157,51,0,0,4,1],[1162,8,1162,51,0,0,4,1],[1166,8,1166,62,0,0,4,1],[1168,8,1168,62,0,0,4,1]],"png_set_cHRM_XYZ_fixed":[[74,8,74,23,0,0,4,1],[74,27,74,43,0,0,4,1],[87,8,87,39,0,0,4,0]],"png_get_cHRM_XYZ":[[567,8,567,23,0,0,4,0],[567,27,567,43,0,0,4,0],[568,8,568,46,0,0,4,0],[568,8,568,46,0,0,4,1],[569,8,569,51,0,0,4,0],[571,11,571,24,0,0,4,0],[573,11,573,24,0,0,4,0],[575,11,575,24,0,0,4,0],[577,11,577,26,0,0,4,0],[579,11,579,26,0,0,4,0],[581,11,581,26,0,0,4,0],[583,11,583,25,0,0,4,0],[585,11,585,25,0,0,4,0],[587,11,587,25,0,0,4,0]],"png_set_write_fn":[[130,8,130,29,0,0,4,0],[142,8,142,31,0,0,4,0]],"png_fp_add":[[1050,8,1050,19,0,0,4,0],[1052,11,1052,42,0,0,4,0]],"png_safe_add":[[1099,8,1099,14,0,0,4,0]],"png_set_read_fn":[[106,8,106,38,0,0,4,0]],"png_write_sig":[[81,8,81,30,0,0,4,1]],"png_get_PLTE":[[1126,8,1126,23,0,0,4,0],[1126,27,1126,43,0,0,4,0],[1127,8,1127,46,0,0,4,0],[1127,8,1127,46,0,0,4,1],[1127,50,1127,65,0,0,4,0]],"png_set_packswap":[[73,8,73,23,0,0,4,1],[76,8,76,30,0,0,4,0]],"png_write_data":[[35,8,35,38,0,0,4,1]]},"library_calls":["png_create_read_struct_2","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_read_fn","png_sig_cmp","png_destroy_read_struct","png_set_sig_bytes","png_read_info","png_create_write_struct_2","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_set_write_fn","png_get_IHDR","png_set_IHDR","png_get_PLTE","png_set_PLTE","png_set_background_fixed","png_get_x_pixels_per_meter","png_get_y_pixels_per_meter","png_set_pHYs","png_set_mDCV_fixed","png_get_progressive_ptr","png_set_progressive_read_fn","png_set_packswap","png_get_cHRM_XYZ","png_set_cHRM_XYZ","png_write_sig","png_set_quantize","png_set_text_compression_method","png_get_rows","png_set_rows","png_write_info","png_set_longjmp_fn","png_write_image","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct_2","png_create_info_struct","png_set_longjmp_fn","png_set_read_fn","png_sig_cmp","png_set_sig_bytes","png_read_info","png_create_write_struct_2","png_create_info_struct","png_set_longjmp_fn","png_set_write_fn","png_get_IHDR","png_set_IHDR","png_get_PLTE","png_set_PLTE","png_set_background_fixed","png_get_x_pixels_per_meter","png_get_y_pixels_per_meter","png_set_pHYs","png_set_mDCV_fixed","png_get_progressive_ptr","png_set_progressive_read_fn","png_set_packswap","png_get_cHRM_XYZ","png_set_cHRM_XYZ","png_write_sig","png_set_quantize","png_set_text_compression_method","png_get_rows","png_set_rows","png_write_info","png_set_longjmp_fn","png_write_image","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to process PNG data:

*/


extern "C" {
}

// Custom memory functions
void*  id_000042_custom_png_malloc(png_structp png_ptr, png_alloc_size_t size) {
    return malloc(size);
}

void  id_000042_custom_png_free(png_structp png_ptr, void* ptr) {
    free(ptr);
}

// Custom read function for memory buffer
void  id_000042_read_from_memory(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) return;
    
    FILE* in_file = static_cast<FILE*>(io_ptr);
    fread(data, 1, length, in_file);
}

// Custom write function for output
void  id_000042_write_to_file(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) return;
    
    FILE* out_file = static_cast<FILE*>(io_ptr);
    fwrite(data, 1, length, out_file);
}

// Custom flush function
void  id_000042_flush_output(png_structp png_ptr) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) return;
    
    FILE* out_file = static_cast<FILE*>(io_ptr);
    fflush(out_file);
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<100) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_17, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_18, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_19, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_20, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need enough data for PNG operations
    
    FILE* in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	FUZZ_FILENAME_PUSH("output_file")
	
    
    FILE* out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create read structure
    png_structp read_ptr = png_create_read_struct_2(
        fuzz_str_1,
        nullptr, nullptr, nullptr,
        nullptr,
         id_000042_custom_png_malloc,
         id_000042_custom_png_free);
    
    if (!read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create read info structure
    png_infop read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(read_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up read functions
    png_set_read_fn(read_ptr, in_file,  id_000042_read_from_memory);
    
    // Read PNG signature
    png_byte sig[8];
    fread(sig, 1, 8, in_file);
    if (png_sig_cmp(sig, fuzz_uint64_t_2, 8)) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_set_sig_bytes(read_ptr, fuzz_int32_t_3);
    
    // Read PNG info
    png_read_info(read_ptr, read_info);
    
    // Create write structure
    png_structp write_ptr = png_create_write_struct_2(
        fuzz_str_4,
        nullptr, nullptr, nullptr,
        nullptr,
         id_000042_custom_png_malloc,
         id_000042_custom_png_free);
    
    if (!write_ptr) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create write info structure
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling for write
    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up write functions
    png_set_write_fn(write_ptr, out_file,  id_000042_write_to_file,  id_000042_flush_output);
    
    // Copy basic info from read to write
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    
    png_get_IHDR(read_ptr, read_info, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);
    
    png_set_IHDR(write_ptr, write_info, width, height, bit_depth, color_type,
                 interlace_method, compression_method, filter_method);
    
    // Copy other chunks if they exist
    png_colorp palette;
    int num_palette;
    if (png_get_PLTE(read_ptr, read_info, &palette, &num_palette)) {
        png_set_PLTE(write_ptr, write_info, palette, num_palette);
    }
    
    // USAGE OF SPECIFIED APIS:
    
    // 1. png_set_background_fixed
    png_color_16 background_color = {0, 0, 0, 0, 0};
    png_fixed_point background_gamma = PNG_FP_1;
    png_set_background_fixed(write_ptr, &background_color, 
                            fuzz_int32_t_5, fuzz_int32_t_6, fuzz_int32_t_7);
    
    // 2. png_get_pixels_per_meter (Note: This is actually png_get_pixels_per_inch in the provided APIs)
    png_uint_32 x_pixels_per_meter, y_pixels_per_meter;
    // Using available API that's similar
    x_pixels_per_meter = png_get_x_pixels_per_meter(read_ptr, read_info);
    y_pixels_per_meter = png_get_y_pixels_per_meter(read_ptr, read_info);
    // Set physical dimensions if they exist
    if (x_pixels_per_meter && y_pixels_per_meter) {
        png_set_pHYs(write_ptr, write_info, x_pixels_per_meter, y_pixels_per_meter,
                    fuzz_int32_t_8);
    }
    
    // 3. png_set_mDCV_fixed
    png_set_mDCV_fixed(write_ptr, write_info,
                      fuzz_int32_t_9, fuzz_int32_t_10,  // white x, y
                      fuzz_int32_t_11, fuzz_int32_t_12,  // red x, y
                      fuzz_int32_t_13, fuzz_int32_t_14,  // green x, y
                      fuzz_int32_t_15, fuzz_int32_t_16,  // blue x, y
                      fuzz_uint32_t_17, fuzz_uint32_t_18);     // max/min luminance scaled by 10000
    
    // 4. png_get_progressive_ptr
    void* progressive_ptr = png_get_progressive_ptr(read_ptr);
    // Store progressive pointer in user data if available
    if (progressive_ptr) {
        png_set_progressive_read_fn(write_ptr, progressive_ptr, nullptr, nullptr, nullptr);
    }
    
    // 5. png_set_packswap
    png_set_packswap(write_ptr);
    
    // 6. png_get_cHRM_XYZ
    double red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z;
    if (png_get_cHRM_XYZ(read_ptr, read_info, &red_X, &red_Y, &red_Z,
                         &green_X, &green_Y, &green_Z,
                         &blue_X, &blue_Y, &blue_Z)) {
        png_set_cHRM_XYZ(write_ptr, write_info,
                        red_X, red_Y, red_Z,
                        green_X, green_Y, green_Z,
                        blue_X, blue_Y, blue_Z);
    }
    
    // 7. png_write_sig
    png_write_sig(write_ptr);
    
    // 8. png_set_quantize
    if (color_type == PNG_COLOR_TYPE_PALETTE && num_palette > 0) {
        int maximum_colors = 256;
        int full_quantize = 1;
        png_set_quantize(write_ptr, palette, num_palette, maximum_colors, 
                        nullptr, fuzz_int32_t_19);
    }
    
    // 9. png_set_text_compression_method
    png_set_text_compression_method(write_ptr, fuzz_int32_t_20);
    
    // Copy image data
    png_bytepp row_pointers = png_get_rows(read_ptr, read_info);
    png_set_rows(write_ptr, write_info, row_pointers);
    
    // Write image
    png_write_info(write_ptr, write_info);
    
    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        // Handle write error
    } else {
        png_write_image(write_ptr, row_pointers);
        png_write_end(write_ptr, write_info);
    }
    
    // Cleanup
    png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    png_destroy_write_struct(&write_ptr, &write_info);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}