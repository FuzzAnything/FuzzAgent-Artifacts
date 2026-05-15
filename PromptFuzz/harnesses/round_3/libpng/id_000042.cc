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
//<ID> 861
//<Prompt> []
/*<Combination>: [
*/
//<score> 874, nr_unique_branch: 176
//<Quality> {"density":38,"unique_branches":{"png_write_end":[[408,8,408,53,0,0,4,0],[409,8,409,56,0,0,4,0],[409,8,409,56,0,0,4,1],[487,11,487,49,0,0,4,0],[488,11,488,48,0,0,4,1]],"png_write_info":[[277,8,277,46,0,0,4,0]],"png_write_finish_row":[[2143,8,2143,32,0,0,4,0],[2146,11,2146,58,0,0,4,1],[2158,17,2158,35,0,0,4,1],[2171,17,2171,64,0,0,4,1],[2174,19,2174,42,0,0,4,1],[2174,46,2174,68,0,0,4,1],[2179,11,2179,28,0,0,4,0],[2181,14,2181,39,0,0,4,0],[2181,14,2181,39,0,0,4,1],[534,6,534,23,4,0,4,0]],"png_do_read_interlace":[[3887,10,3887,16,0,0,4,0],[3900,17,3900,54,0,0,4,1],[3919,25,3919,44,0,0,4,0],[3919,25,3919,44,0,0,4,1],[3922,28,3922,37,0,0,4,0],[3922,28,3922,37,0,0,4,1],[3928,23,3928,38,0,0,4,0],[3928,23,3928,38,0,0,4,1],[3938,20,3938,35,0,0,4,0],[3938,20,3938,35,0,0,4,1],[3950,10,3950,16,0,0,4,0],[3961,17,3961,54,0,0,4,1],[3980,25,3980,44,0,0,4,0],[3980,25,3980,44,0,0,4,1],[3986,28,3986,37,0,0,4,0],[3986,28,3986,37,0,0,4,1],[3992,23,3992,38,0,0,4,0],[3992,23,3992,38,0,0,4,1],[4002,20,4002,35,0,0,4,0],[4002,20,4002,35,0,0,4,1],[4014,10,4014,16,0,0,4,0],[4025,17,4025,54,0,0,4,1],[4044,25,4044,44,0,0,4,0],[4044,25,4044,44,0,0,4,1],[4049,28,4049,37,0,0,4,0],[4049,28,4049,37,0,0,4,1],[4055,23,4055,38,0,0,4,0],[4055,23,4055,38,0,0,4,1],[4065,20,4065,35,0,0,4,0],[4065,20,4065,35,0,0,4,1],[4077,10,4077,17,0,0,4,1],[534,6,534,23,5,0,4,1]],"png_set_tRNS":[[1164,8,1164,27,0,0,4,1]],"png_benign_error":[[310,8,310,59,0,0,4,0],[310,8,310,59,0,0,4,1],[313,14,313,55,0,0,4,0],[314,13,314,37,0,0,4,0],[324,14,324,55,0,0,4,1]],"png_get_PLTE":[[1126,8,1126,23,0,0,4,0],[1126,27,1126,43,0,0,4,0],[1127,8,1127,46,0,0,4,0],[1127,8,1127,46,0,0,4,1],[1127,50,1127,65,0,0,4,0]],"png_do_read_transformations":[[5074,8,5074,54,0,0,4,0],[5075,8,5075,37,0,0,4,0]],"png_get_tRNS":[[1209,8,1209,23,0,0,4,0],[1209,27,1209,43,0,0,4,0],[1210,8,1210,46,0,0,4,0],[1210,8,1210,46,0,0,4,1],[1212,11,1212,57,0,0,4,0],[1212,11,1212,57,0,0,4,1],[1214,14,1214,33,0,0,4,0],[1220,14,1220,33,0,0,4,0],[1226,14,1226,33,0,0,4,0],[1232,14,1232,33,0,0,4,0],[1236,11,1236,28,0,0,4,0]],"png_set_text_compression_window_bits":[[1323,8,1323,23,0,0,4,1],[1326,8,1326,24,0,0,4,1],[1332,13,1332,28,0,0,4,1]],"png_fixed_inches_from_microns":[[385,8,385,51,0,0,4,0]],"png_combine_row":[[3426,8,3426,21,0,0,4,0],[3432,11,3432,57,0,0,4,1],[3461,11,3461,26,0,0,4,0],[3596,14,3596,60,0,0,4,1],[3615,17,3615,23,0,0,4,0],[3615,17,3615,23,0,0,4,1],[3617,20,3617,29,0,0,4,0],[3627,17,3627,45,0,0,4,0],[3627,17,3627,45,0,0,4,1],[3869,8,3869,23,0,0,4,0],[534,6,534,23,2,0,4,1],[534,6,534,23,3,0,4,1],[3576,14,3576,23,21,0,4,1],[534,6,534,23,32,0,4,1],[3551,33,3551,39,90,0,4,0],[3551,33,3551,39,90,0,4,1],[3551,43,3551,49,90,0,4,0],[3551,43,3551,49,90,0,4,1]],"png_write_eXIf":[[1550,16,1550,28,0,0,4,0],[1550,16,1550,28,0,0,4,1]],"png_write_row":[[768,36,768,54,0,0,4,1],[817,8,817,32,0,0,4,0],[818,8,818,55,0,0,4,1],[904,8,904,27,0,0,4,0],[904,31,904,48,0,0,4,0],[905,8,905,55,0,0,4,1],[951,8,951,53,0,0,4,0],[952,8,952,37,0,0,4,0],[534,6,534,23,4,0,4,1]],"png_get_x_offset_microns":[[249,8,249,23,0,0,4,0],[249,27,249,43,0,0,4,0],[250,8,250,46,0,0,4,0],[250,8,250,46,0,0,4,1],[252,11,252,62,0,0,4,0]],"png_write_tRNS":[[1352,8,1352,44,0,0,4,0],[1354,11,1354,25,0,0,4,1],[1354,29,1354,66,0,0,4,1],[1388,38,1388,69,0,0,4,1]],"png_get_compression_type":[[118,8,118,23,0,0,4,0],[118,27,118,43,0,0,4,0]],"png_write_start_row":[[2102,8,2102,32,0,0,4,0],[2104,11,2104,58,0,0,4,0],[534,6,534,23,2,0,4,1]],"png_read_end":[[784,8,784,56,0,0,4,0],[825,16,825,38,0,0,4,0],[833,15,833,25,0,0,4,0],[833,29,833,71,0,0,4,1],[834,17,834,65,0,0,4,1]],"png_get_filter_type":[[100,8,100,23,0,0,4,0],[100,27,100,43,0,0,4,0]],"png_image_size":[[230,11,230,35,0,0,4,0],[240,34,240,41,0,0,4,0],[240,34,240,41,0,0,4,1],[244,17,244,23,0,0,4,0],[534,6,534,23,2,0,4,0],[534,6,534,23,2,0,4,1],[2611,35,2611,43,4,0,4,0],[2611,35,2611,43,4,0,4,1],[2611,35,2611,43,6,0,4,0],[2611,35,2611,43,6,0,4,1],[2610,35,2610,43,7,0,4,0],[2610,35,2610,43,7,0,4,1],[2610,35,2610,43,9,0,4,0],[2610,35,2610,43,9,0,4,1]],"png_do_check_palette_indexes":[[732,8,732,57,0,0,4,0],[732,8,732,57,0,0,4,1],[733,7,733,31,0,0,4,0],[746,10,746,16,0,0,4,0],[746,10,746,16,0,0,4,1],[751,20,751,41,0,0,4,0],[751,20,751,41,0,0,4,1],[753,19,753,40,0,0,4,0],[753,19,753,40,0,0,4,1],[761,10,761,16,0,0,4,0],[761,10,761,16,0,0,4,1],[763,20,763,41,0,0,4,0],[763,20,763,41,0,0,4,1],[767,19,767,47,0,0,4,0],[767,19,767,47,0,0,4,1],[772,19,772,47,0,0,4,0],[772,19,772,47,0,0,4,1],[777,19,777,47,0,0,4,0],[777,19,777,47,0,0,4,1],[782,19,782,47,0,0,4,0],[782,19,782,47,0,0,4,1],[791,10,791,16,0,0,4,0],[791,10,791,16,0,0,4,1],[793,20,793,41,0,0,4,0],[793,20,793,41,0,0,4,1],[797,19,797,47,0,0,4,0],[797,19,797,47,0,0,4,1],[802,19,802,47,0,0,4,0],[802,19,802,47,0,0,4,1],[811,10,811,16,0,0,4,0],[811,10,811,16,0,0,4,1],[813,20,813,41,0,0,4,0],[813,20,813,41,0,0,4,1],[815,20,815,50,0,0,4,0],[815,20,815,50,0,0,4,1],[822,10,822,17,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_read_png","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_set_text_compression_window_bits","png_get_x_offset_inches_fixed","png_set_eXIf_1","png_set_cHRM_fixed","png_set_chunk_cache_max","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_set_IHDR","png_get_PLTE","png_set_PLTE","png_get_tRNS","png_set_tRNS","png_write_info","png_get_rows","png_write_row","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_read_png","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_text_compression_window_bits","png_get_x_offset_inches_fixed","png_set_eXIf_1","png_set_cHRM_fixed","png_set_chunk_cache_max","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_set_IHDR","png_get_PLTE","png_set_PLTE","png_get_tRNS","png_set_tRNS","png_write_info","png_get_rows","png_write_row","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to achieve reading and writing PNG images with specific metadata operations:

*/


// Include necessary headers for libpng

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<72) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
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
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_14, fdp);
	//fuzzer shim end}




    // Early exit if input is too small
    if (size < 100) {
	return 0;
	}
	
    
    // Create input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	FUZZ_FILENAME_PUSH("output_file")
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Read structures
    png_struct *png_ptr_read = nullptr;
    png_info *info_ptr_read = nullptr;
    
    // Write structures
    png_struct *png_ptr_write = nullptr;
    png_info *info_ptr_write = nullptr;
    
    // Initialize error handling for read
    png_ptr_read = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (!png_ptr_read) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr_read))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O for reading
    png_init_io(png_ptr_read, in_file);
    
    // Read the PNG signature
    png_set_sig_bytes(png_ptr_read, fuzz_int32_t_2);
    
    // Read the entire PNG file
    png_read_png(png_ptr_read, info_ptr_read, fuzz_int32_t_3, nullptr);
    
    // Initialize write structures
    png_ptr_write = png_create_write_struct(fuzz_str_4, nullptr, nullptr, nullptr);
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling for write
    if (setjmp(png_jmpbuf(png_ptr_write))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O for writing
    png_init_io(png_ptr_write, out_file);
    
    // 1. Set text compression window bits (using a value within valid range)
    png_set_text_compression_window_bits(png_ptr_write, fuzz_int32_t_5);
    
    // 2. Get x offset in inches (fixed point) from the read image
    png_fixed_point x_offset_inches = 0;
    if (png_get_x_offset_inches_fixed(png_ptr_read, info_ptr_read)) {
        // Just call the function, we don't need to use the value
    }
    
    // 3. Set eXIf data - extract some EXIF data from input buffer
    // We'll use a portion of the input buffer as EXIF data
    size_t exif_size = size > 200 ? 100 : size / 2;
    if (exif_size > 0) {
        png_byte *exif_data = (png_byte *)data + (size - exif_size);
        png_set_eXIf_1(png_ptr_write, info_ptr_write, exif_size, exif_data);
    }
    
    // 4. Get a uint32 value from the buffer
    // Use the first 4 bytes of the input buffer
    if (size >= 4) {
        png_uint_32 uint_val = png_get_uint_32(data);
        // We can use this value for something, like setting chunk cache max
        uint_val = uint_val % 1000 + 100; // Keep it in reasonable range
    }
    
    // 5. Set cHRM fixed values (using reasonable default values for chromaticities)
    // These are typical sRGB chromaticity values converted to fixed point
    png_set_cHRM_fixed(png_ptr_write, info_ptr_write,
        fuzz_int32_t_6,  // white x
        fuzz_int32_t_7,  // white y
        fuzz_int32_t_8,  // red x
        fuzz_int32_t_9,  // red y
        fuzz_int32_t_10,  // green x
        fuzz_int32_t_11,  // green y
        fuzz_int32_t_12,  // blue x
        fuzz_int32_t_13);  // blue y
    
    // 6. Set chunk cache max (using a reasonable value)
    png_set_chunk_cache_max(png_ptr_write, fuzz_uint32_t_14);
    
    // Copy necessary information from read to write info struct
    png_uint_32 width = png_get_image_width(png_ptr_read, info_ptr_read);
    png_uint_32 height = png_get_image_height(png_ptr_read, info_ptr_read);
    int bit_depth = png_get_bit_depth(png_ptr_read, info_ptr_read);
    int color_type = png_get_color_type(png_ptr_read, info_ptr_read);
    int interlace_type = png_get_interlace_type(png_ptr_read, info_ptr_read);
    int compression_type = png_get_compression_type(png_ptr_read, info_ptr_read);
    int filter_type = png_get_filter_type(png_ptr_read, info_ptr_read);
    
    // Set IHDR for output
    png_set_IHDR(png_ptr_write, info_ptr_write, width, height, bit_depth,
                 color_type, interlace_type, compression_type, filter_type);
    
    // Copy palette if present
    png_colorp palette;
    int num_palette;
    if (png_get_PLTE(png_ptr_read, info_ptr_read, &palette, &num_palette)) {
        png_set_PLTE(png_ptr_write, info_ptr_write, palette, num_palette);
    }
    
    // Copy transparency if present
    png_bytep trans_alpha;
    int num_trans;
    png_color_16p trans_color;
    if (png_get_tRNS(png_ptr_read, info_ptr_read, &trans_alpha, &num_trans, &trans_color)) {
        png_set_tRNS(png_ptr_write, info_ptr_write, trans_alpha, num_trans, trans_color);
    }
    
    // Write the PNG info
    png_write_info(png_ptr_write, info_ptr_write);
    
    // Get row pointers from read image
    png_bytepp row_pointers = png_get_rows(png_ptr_read, info_ptr_read);
    
    // Write image data
    for (png_uint_32 row = 0; row < height; row++) {
        png_write_row(png_ptr_write, row_pointers[row]);
    }
    
    // Write end of PNG
    png_write_end(png_ptr_write, info_ptr_write);
    
    // Clean up
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean up output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}