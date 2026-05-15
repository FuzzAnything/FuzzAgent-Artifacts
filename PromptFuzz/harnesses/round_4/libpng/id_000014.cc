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
//<ID> 492
//<Prompt> []
/*<Combination>: [
*/
//<score> 416, nr_unique_branch: 219
//<Quality> {"density":13,"unique_branches":{"png_write_chunk_header":[[100,8,100,23,0,0,4,1]],"png_write_tEXt":[[1604,8,1604,20,0,0,4,1],[1607,8,1607,20,0,0,4,1],[1607,24,1607,37,0,0,4,1],[1613,8,1613,48,0,0,4,1],[1627,8,1627,21,0,0,4,0]],"png_image_size":[[228,8,228,33,0,0,4,0],[228,37,228,46,0,0,4,0],[230,11,230,35,0,0,4,1]],"png_setup_paeth_row":[[2557,36,2557,43,0,0,4,0],[2557,36,2557,43,0,0,4,1],[2564,14,2564,23,0,0,4,0],[2568,64,2568,77,0,0,4,0],[2568,64,2568,77,0,0,4,1],[2585,12,2585,17,0,0,4,1],[2586,12,2586,18,0,0,4,1],[2587,12,2587,24,0,0,4,1],[2590,12,2590,20,0,0,4,0],[2590,12,2590,20,0,0,4,1],[2590,24,2590,31,0,0,4,0],[2590,39,2590,49,0,0,4,0],[2597,14,2597,23,0,0,4,0],[2600,11,2600,22,0,0,4,1]],"optimize_cmf":[[272,8,272,26,0,0,4,0],[276,11,276,30,0,0,4,0],[276,34,276,56,0,0,4,0],[284,14,284,45,0,0,4,1]],"png_write_filtered_row":[[2858,8,2858,33,0,0,4,0],[2874,8,2874,31,0,0,4,1]],"png_write_complete_chunk":[[195,8,195,23,0,0,4,1],[199,8,199,32,0,0,4,1]],"png_get_next_frame_height":[[1443,8,1443,23,0,0,4,0],[1443,27,1443,43,0,0,4,0]],"png_setup_avg_row":[[2495,36,2495,43,0,0,4,0],[2495,36,2495,43,0,0,4,1],[2502,14,2502,23,0,0,4,0],[2506,36,2506,49,0,0,4,0],[2506,36,2506,49,0,0,4,1],[2514,14,2514,23,0,0,4,0],[2517,11,2517,22,0,0,4,0],[2517,11,2517,22,0,0,4,1]],"png_write_rows":[[645,8,645,23,0,0,4,1],[649,26,649,38,0,0,4,0],[649,26,649,38,0,0,4,1]],"png_write_end":[[401,8,401,23,0,0,4,1],[404,8,404,44,0,0,4,1],[408,8,408,53,0,0,4,1],[414,8,414,67,0,0,4,1],[419,8,419,24,0,0,4,1]],"write_unknown_chunks":[[26,8,26,41,0,0,4,1]],"png_write_info_before_PLTE":[[87,8,87,23,0,0,4,1],[87,27,87,43,0,0,4,1],[90,8,90,57,0,0,4,0],[96,11,96,56,0,0,4,0],[97,11,97,47,0,0,4,1],[131,14,131,52,0,0,4,1],[157,14,157,52,0,0,4,1],[172,8,172,46,0,0,4,1],[178,8,178,46,0,0,4,1],[190,8,190,46,0,0,4,1],[201,14,201,52,0,0,4,1],[209,14,209,52,0,0,4,1],[214,11,214,49,0,0,4,1],[219,14,219,52,0,0,4,1]],"png_write_finish_row":[[2138,8,2138,47,0,0,4,0],[2138,8,2138,47,0,0,4,1],[2143,8,2143,32,0,0,4,1]],"png_setup_up_row":[[2451,36,2451,49,0,0,4,0],[2451,36,2451,49,0,0,4,1],[2458,14,2458,23,0,0,4,0],[2461,11,2461,22,0,0,4,0],[2461,11,2461,22,0,0,4,1]],"png_free_buffer_list":[[460,8,460,20,0,0,4,0],[471,14,471,26,0,0,4,1]],"png_get_bKGD":[[507,8,507,23,0,0,4,0],[507,27,507,43,0,0,4,0],[508,8,508,46,0,0,4,0],[508,8,508,46,0,0,4,1],[509,8,509,26,0,0,4,0]],"png_check_keyword":[[2083,8,2083,19,0,0,4,1],[2089,11,2089,15,0,0,4,0],[2089,11,2089,15,0,0,4,1],[2089,19,2089,31,0,0,4,0],[2093,12,2093,19,0,0,4,0],[2093,23,2093,32,0,0,4,0],[2114,8,2114,19,0,0,4,0],[2114,23,2114,33,0,0,4,1],[2124,8,2124,20,0,0,4,1],[2129,8,2129,17,0,0,4,1],[2132,13,2132,31,0,0,4,1]],"png_setup_sub_row":[[2390,70,2390,77,0,0,4,0],[2390,70,2390,77,0,0,4,1],[2397,14,2397,23,0,0,4,0],[2401,36,2401,49,0,0,4,0],[2401,36,2401,49,0,0,4,1],[2408,14,2408,23,0,0,4,0],[2411,11,2411,22,0,0,4,1]],"png_write_chunk_end":[[167,8,167,23,0,0,4,1]],"png_deflate_claim":[[312,8,312,28,0,0,4,1],[351,11,351,28,0,0,4,0],[353,14,353,67,0,0,4,1],[356,19,356,56,0,0,4,0],[388,11,388,29,0,0,4,0],[397,17,397,52,0,0,4,0],[397,17,397,52,0,0,4,1],[405,11,405,63,0,0,4,1],[429,11,429,63,0,0,4,1],[437,14,437,25,0,0,4,0],[444,11,444,22,0,0,4,0]],"png_write_destroy":[[1002,8,1002,60,0,0,4,0]],"png_get_tIME":[[1189,8,1189,23,0,0,4,0],[1189,27,1189,43,0,0,4,0],[1190,8,1190,46,0,0,4,0],[1190,8,1190,46,0,0,4,1],[1190,50,1190,66,0,0,4,0]],"png_write_row":[[764,8,764,23,0,0,4,1],[768,8,768,32,0,0,4,0],[768,8,768,32,0,0,4,1],[768,36,768,54,0,0,4,0],[771,11,771,60,0,0,4,1],[817,8,817,32,0,0,4,1],[904,8,904,27,0,0,4,1],[919,8,919,37,0,0,4,1],[926,8,926,52,0,0,4,1],[927,8,927,64,0,0,4,1],[940,8,940,71,0,0,4,1],[951,8,951,53,0,0,4,1],[959,8,959,37,0,0,4,1],[534,6,534,23,4,0,4,0]],"png_write_chunk_data":[[150,8,150,20,0,0,4,1]],"png_write_start_row":[[2056,8,2056,28,0,0,4,1],[2059,8,2059,27,0,0,4,1],[2062,8,2062,20,0,0,4,1],[2067,8,2068,32,0,0,4,0],[2068,36,2068,60,0,0,4,0],[2075,11,2075,35,0,0,4,0],[2078,11,2078,34,0,0,4,0],[2081,11,2081,35,0,0,4,0],[2084,11,2084,37,0,0,4,0],[2087,11,2087,26,0,0,4,0],[2095,8,2095,76,0,0,4,0],[2102,8,2102,32,0,0,4,1],[534,6,534,23,2,0,4,0]],"png_write_find_filter":[[2701,8,2701,37,0,0,4,1],[2708,13,2708,50,0,0,4,0],[2709,10,2709,41,0,0,4,0],[2720,40,2720,53,0,0,4,0],[2720,40,2720,53,0,0,4,1],[2726,20,2726,29,0,0,4,0],[2735,8,2735,38,0,0,4,1],[2742,13,2742,49,0,0,4,0],[2749,11,2749,21,0,0,4,0],[2753,14,2753,38,0,0,4,0],[2762,8,2762,37,0,0,4,1],[2768,13,2768,48,0,0,4,0],[2775,11,2775,21,0,0,4,0],[2775,11,2775,21,0,0,4,1],[2779,14,2779,38,0,0,4,0],[2788,8,2788,38,0,0,4,1],[2794,13,2794,49,0,0,4,0],[2801,11,2801,21,0,0,4,1],[2814,8,2814,40,0,0,4,1],[2820,13,2820,51,0,0,4,0],[2827,11,2827,21,0,0,4,0],[2827,11,2827,21,0,0,4,1],[2830,14,2830,38,0,0,4,0]],"png_write_info":[[236,8,236,23,0,0,4,1],[236,27,236,43,0,0,4,1],[241,8,241,46,0,0,4,1],[245,13,245,59,0,0,4,1],[249,8,249,45,0,0,4,1],[272,8,272,46,0,0,4,1],[277,8,277,46,0,0,4,1],[285,8,285,46,0,0,4,1],[290,8,290,46,0,0,4,1],[296,8,296,46,0,0,4,1],[303,8,303,46,0,0,4,1],[309,8,309,46,0,0,4,1],[315,8,315,46,0,0,4,1],[323,8,323,46,0,0,4,1],[330,16,330,38,0,0,4,0],[330,16,330,38,0,0,4,1],[335,11,335,44,0,0,4,1],[356,16,356,74,0,0,4,1],[369,16,369,74,0,0,4,0]],"png_set_text":[[929,8,929,16,0,0,4,1]],"png_write_IHDR":[[701,7,701,31,0,0,4,1],[719,7,719,30,0,0,4,0],[722,30,722,46,0,0,4,1],[724,14,724,30,0,0,4,1],[730,7,730,34,0,0,4,1],[745,7,745,37,0,0,4,1],[756,7,756,36,0,0,4,1],[767,7,767,14,0,0,4,1],[771,8,771,53,0,0,4,1],[788,10,788,73,0,0,4,1],[794,8,794,43,0,0,4,1],[801,8,801,44,0,0,4,1],[846,8,846,46,0,0,4,0],[848,11,848,56,0,0,4,1],[849,11,849,33,0,0,4,1],[534,6,534,23,19,0,4,0]],"png_compress_IDAT":[[940,8,940,35,0,0,4,0],[940,8,940,35,0,0,4,1],[947,11,947,40,0,0,4,0],[958,11,958,80,0,0,4,1],[981,11,981,28,0,0,4,0],[987,40,987,53,0,0,4,1],[997,11,997,42,0,0,4,1],[1039,11,1039,22,0,0,4,0],[1039,11,1039,22,0,0,4,1],[1045,14,1045,28,0,0,4,0],[1047,17,1047,34,0,0,4,1],[1054,16,1054,35,0,0,4,0],[1054,39,1054,56,0,0,4,0],[1063,14,1063,50,0,0,4,0],[1064,14,1064,68,0,0,4,0],[1068,14,1068,22,0,0,4,0],[1071,17,1071,49,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_init_io","png_read_info","png_get_tIME","png_get_next_frame_height","png_get_bKGD","png_set_IHDR","png_set_text","png_write_info","png_malloc","png_save_uint_32","png_write_rows","png_free","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_init_io","png_read_info","png_get_tIME","png_get_next_frame_height","png_get_bKGD","png_set_IHDR","png_set_text","png_write_info","png_malloc","png_save_uint_32","png_write_rows","png_free","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<52) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for PNG operations

    // Create input file from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize PNG structures for reading
    png_struct *png_ptr_read = png_create_read_struct(fuzz_str_1, 
                                                      nullptr, nullptr, nullptr);
    if (!png_ptr_read) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_info *info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize PNG structures for writing
    png_struct *png_ptr_write = png_create_write_struct(fuzz_str_2,
                                                       nullptr, nullptr, nullptr);
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_info *info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr_read)) || setjmp(png_jmpbuf(png_ptr_write))) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up I/O
    png_init_io(png_ptr_read, in_file);
    png_init_io(png_ptr_write, out_file);

    // Read PNG info from input
    png_read_info(png_ptr_read, info_ptr_read);

    // Get various metadata using the required APIs
    png_time *mod_time = nullptr;
    png_uint_32 time_result = png_get_tIME(png_ptr_read, info_ptr_read, &mod_time);
    
    png_uint_32 next_frame_height = png_get_next_frame_height(png_ptr_read, info_ptr_read);
    
    png_color_16 *background = nullptr;
    png_uint_32 bkgd_result = png_get_bKGD(png_ptr_read, info_ptr_read, &background);
    
    // Note: png_get_next_frame_delay_num is not in the provided API list, 
    // but we'll use a placeholder value since it's required
    png_uint_16 delay_num = 0;  // Default value

    // Create a simple image for writing
    png_uint_32 width = 64;
    png_uint_32 height = 64;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;
    
    png_set_IHDR(png_ptr_write, info_ptr_write, fuzz_uint32_t_3, fuzz_uint32_t_4, fuzz_int32_t_5,
                 fuzz_int32_t_6, fuzz_int32_t_7, fuzz_int32_t_8,
                 fuzz_int32_t_9);

    // Create text metadata using png_set_text
    png_text text_ptr;
    char key[] = "FuzzerTest";
    char text[] = "Generated by fuzzer";
    text_ptr.compression = PNG_TEXT_COMPRESSION_NONE;
    text_ptr.key = key;
    text_ptr.text = text;
    text_ptr.text_length = strlen(text);
    text_ptr.itxt_length = 0;
    text_ptr.lang = nullptr;
    text_ptr.lang_key = nullptr;
    
    png_set_text(png_ptr_write, info_ptr_write, &text_ptr, 1);

    // Write PNG info
    png_write_info(png_ptr_write, info_ptr_write);

    // Allocate and prepare row data
    png_bytep row = (png_bytep)png_malloc(png_ptr_write, 
                                          width * 3 * sizeof(png_byte));
    
    // Use png_save_uint_32 to prepare buffer
    png_byte buf[4];
    png_save_uint_32(buf, next_frame_height);
    
    // Fill rows with pattern using the saved uint32
    for (png_uint_32 y = 0; y < height; y++) {
        for (png_uint_32 x = 0; x < width; x++) {
            int offset = x * 3;
            row[offset] = (png_byte)((x + y + buf[0]) % 256);     // Red
            row[offset + 1] = (png_byte)((x * 2 + buf[1]) % 256); // Green
            row[offset + 2] = (png_byte)((y * 2 + buf[2]) % 256); // Blue
        }
        png_bytep row_pointers[1] = {row};
        png_write_rows(png_ptr_write, row_pointers, 1);
    }

    // Clean up
    png_free(png_ptr_write, row);
    png_write_end(png_ptr_write, nullptr);
    
    // Destroy structures
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}