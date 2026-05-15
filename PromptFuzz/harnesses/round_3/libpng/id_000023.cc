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
//<ID> 350
//<Prompt> []
/*<Combination>: [
*/
//<score> 114, nr_unique_branch: 68
//<Quality> {"density":24,"unique_branches":{"ppi_from_ppm":[[347,8,347,30,0,0,4,0],[347,34,348,18,0,0,4,0]],"png_formatted_warning":[[254,11,254,27,0,0,4,0],[254,31,254,47,0,0,4,0],[254,31,254,47,0,0,4,1],[259,11,259,20,0,0,4,0],[259,24,259,39,0,0,4,0],[259,24,259,39,0,0,4,1],[259,43,259,61,0,0,4,0],[264,14,264,35,0,0,4,0],[264,39,264,60,0,0,4,0],[276,20,276,36,0,0,4,0],[276,40,276,53,0,0,4,0],[276,40,276,53,0,0,4,1],[276,57,276,68,0,0,4,0]],"png_warning_parameter":[[204,8,204,18,0,0,4,0],[204,22,204,59,0,0,4,0]],"png_write_tRNS":[[1369,11,1369,50,0,0,4,0],[1381,13,1381,45,0,0,4,1],[1388,38,1388,69,0,0,4,0]],"png_get_next_frame_delay_den":[[1483,8,1483,23,0,0,4,0],[1483,27,1483,43,0,0,4,0]],"png_malloc_default":[[196,8,196,23,0,0,4,1],[202,8,202,19,0,0,4,1]],"png_set_text":[[929,8,929,16,0,0,4,1]],"png_app_warning":[[340,8,340,58,0,0,4,1]],"png_warning_parameter_signed":[[226,8,226,17,0,0,4,1],[231,8,231,17,0,0,4,1]],"png_data_freer":[[466,8,466,23,0,0,4,1],[466,27,466,43,0,0,4,1],[469,8,469,43,0,0,4,0]],"png_format_number":[[143,10,143,36,0,0,4,0]],"png_get_IHDR":[[950,8,950,32,0,0,4,1],[953,8,953,27,0,0,4,1],[956,8,956,30,0,0,4,1]],"png_get_pixels_per_meter":[[172,8,172,23,0,0,4,0],[172,27,172,43,0,0,4,0],[173,8,173,46,0,0,4,0],[173,8,173,46,0,0,4,1],[175,11,175,59,0,0,4,0],[176,11,176,69,0,0,4,0]],"png_malloc_base":[[91,11,91,26,0,0,4,1]],"png_write_tEXt":[[1604,8,1604,20,0,0,4,1],[1607,8,1607,20,0,0,4,1],[1607,24,1607,37,0,0,4,0],[1607,24,1607,37,0,0,4,1],[1613,8,1613,48,0,0,4,1],[1627,8,1627,21,0,0,4,0],[1627,8,1627,21,0,0,4,1]],"png_write_info":[[330,16,330,38,0,0,4,0],[335,11,335,44,0,0,4,1],[356,16,356,74,0,0,4,1],[369,16,369,74,0,0,4,0]],"png_check_keyword":[[2093,12,2093,19,0,0,4,1],[2093,23,2093,32,0,0,4,1],[2093,37,2093,65,0,0,4,1],[2098,16,2098,26,0,0,4,0],[2098,16,2098,26,0,0,4,1],[2106,14,2106,22,0,0,4,0],[2110,16,2110,34,0,0,4,0],[2110,16,2110,34,0,0,4,1],[2114,23,2114,33,0,0,4,0],[2117,11,2117,29,0,0,4,1],[2132,13,2132,31,0,0,4,0]],"png_free_data":[[546,11,546,40,0,0,4,1],[631,11,631,25,0,0,4,1],[642,8,642,57,0,0,4,0],[663,11,663,41,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_set_write_fn","png_init_io","png_read_info","png_get_IHDR","png_set_IHDR","png_malloc_default","png_set_tRNS","png_free","png_get_pixels_per_inch","png_get_next_frame_delay_den","png_set_text","png_data_freer","png_write_info","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_set_write_fn","png_init_io","png_read_info","png_get_IHDR","png_set_IHDR","png_malloc_default","png_set_tRNS","png_free","png_get_pixels_per_inch","png_get_next_frame_delay_den","png_set_text","png_data_freer","png_write_info","png_destroy_read_struct","png_destroy_write_struct"],"visited":3}
/**/


extern "C" {
}

// Custom write function for png_set_write_fn
static void  id_000023_custom_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Write data - in this fuzzing context, we'll just discard it
    (void)png_ptr;
    (void)data;
    (void)length;
}

// Custom flush function for png_set_write_fn
static void  id_000023_custom_flush_data(png_structp png_ptr) {
    (void)png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<40) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations
    
    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create FILE* for writing output (though we won't actually use it for writing)
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize libpng structures
    png_structp read_ptr = png_create_read_struct(fuzz_str_1, 
                                                  nullptr, nullptr, nullptr);
    if (!read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_structp write_ptr = png_create_write_struct(fuzz_str_2,
                                                   nullptr, nullptr, nullptr);
    if (!write_ptr) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
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
    
    // Set up error handling
    if (setjmp(png_jmpbuf(read_ptr)) || setjmp(png_jmpbuf(write_ptr))) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Use png_set_write_fn - API #3
    png_set_write_fn(write_ptr, out_file,  id_000023_custom_write_data,  id_000023_custom_flush_data);
    
    // Read PNG header from input
    png_init_io(read_ptr, in_file);
    png_read_info(read_ptr, read_info);
    
    // Extract basic image info
    png_uint_32 width, height;
    int bit_depth, color_type;
    png_get_IHDR(read_ptr, read_info, &width, &height, &bit_depth, 
                 &color_type, nullptr, nullptr, nullptr);
    
    // Set up write info with same dimensions
    png_set_IHDR(write_ptr, write_info, width, height, bit_depth,
                 color_type, fuzz_int32_t_3, fuzz_int32_t_4,
                 fuzz_int32_t_5);
    
    // Use png_malloc_default - API #1
    // Allocate memory for transparency data
    size_t trans_size = (size < 256) ? size : 256;
    png_byte* trans_alpha = (png_byte*)png_malloc_default(write_ptr, trans_size);
    if (trans_alpha) {
        // Fill with some data from input
        for (size_t i = 0; i < trans_size && i < size; i++) {
            trans_alpha[i] = data[i] % 256;
        }
        
        // Use png_set_tRNS - API #7
        png_color_16 trans_color;
        trans_color.red = (png_uint_16)(data[0] * 256 + data[1]);
        trans_color.green = (png_uint_16)(data[2] * 256 + data[3]);
        trans_color.blue = (png_uint_16)(data[4] * 256 + data[5]);
        trans_color.gray = (png_uint_16)(data[6] * 256 + data[7]);
        
        png_set_tRNS(write_ptr, write_info, trans_alpha, 
                    (trans_size < 256) ? (int)trans_size : 256, &trans_color);
        
        // Free allocated memory
        png_free(write_ptr, trans_alpha);
    }
    
    // Use png_get_pixels_per_inch - API #5
    png_uint_32 pixels_per_inch = png_get_pixels_per_inch(read_ptr, read_info);
    (void)pixels_per_inch;  // Use the value to avoid unused variable warning
    
    // Use png_get_next_frame_delay_den - API #4
    // This is for APNG (animated PNG) - may return 0 for static PNG
    png_uint_16 delay_den = png_get_next_frame_delay_den(read_ptr, read_info);
    (void)delay_den;
    
    // Use png_set_text - API #6
    // Prepare text data from input
    if (size >= 50) {
        png_text text_ptr[1];
        char key_buffer[20];
        char text_buffer[30];
        
        // Extract key from first 19 bytes of input
        size_t key_len = (size < 19) ? size : 19;
        memcpy(key_buffer, data, key_len);
        key_buffer[key_len] = '\0';
        
        // Extract text from next 29 bytes
        size_t text_start = (key_len < size) ? key_len : 0;
        size_t text_len = (size - text_start < 29) ? size - text_start : 29;
        if (text_len > 0) {
            memcpy(text_buffer, data + text_start, text_len);
            text_buffer[text_len] = '\0';
            
            text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
            text_ptr[0].key = key_buffer;
            text_ptr[0].text = text_buffer;
            text_ptr[0].text_length = text_len;
            
            png_set_text(write_ptr, write_info, text_ptr, 1);
        }
    }
    
    // Use png_data_freer - API #2
    // Free image data from read structure
    png_data_freer(read_ptr, read_info, PNG_DESTROY_WILL_FREE_DATA, fuzz_uint32_t_6);
    
    // Write the PNG info (though we're not actually writing to file in fuzzing)
    png_write_info(write_ptr, write_info);
    
    // Cleanup
    png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    png_destroy_write_struct(&write_ptr, &write_info);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove the output file since we don't need it
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}