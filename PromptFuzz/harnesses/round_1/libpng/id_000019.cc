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
//<ID> 308
//<Prompt> []
/*<Combination>: [
*/
//<score> 126, nr_unique_branch: 229
//<Quality> {"density":28,"unique_branches":{"png_do_read_transformations":[[4873,8,4873,52,0,0,4,0],[4875,11,4875,57,0,0,4,0],[4875,11,4875,57,0,0,4,1],[4890,14,4890,37,0,0,4,0],[4890,14,4890,37,0,0,4,1],[4891,14,4891,63,0,0,4,0],[5024,8,5024,53,0,0,4,0]],"png_do_expand":[[4496,8,4496,51,0,0,4,0],[4496,8,4496,51,0,0,4,1],[4498,27,4498,46,0,0,4,0],[4498,27,4498,46,0,0,4,1],[4500,11,4500,34,0,0,4,0],[4500,11,4500,34,0,0,4,1],[4504,13,4504,19,0,0,4,0],[4504,13,4504,19,0,0,4,1],[4510,28,4510,41,0,0,4,0],[4510,28,4510,41,0,0,4,1],[4512,23,4512,44,0,0,4,0],[4512,23,4512,44,0,0,4,1],[4518,23,4518,33,0,0,4,0],[4518,23,4518,33,0,0,4,1],[4532,13,4532,19,0,0,4,0],[4532,13,4532,19,0,0,4,1],[4538,28,4538,41,0,0,4,0],[4538,28,4538,41,0,0,4,1],[4543,23,4543,33,0,0,4,0],[4543,23,4543,33,0,0,4,1],[4557,13,4557,19,0,0,4,0],[4557,13,4557,19,0,0,4,1],[4563,28,4563,41,0,0,4,0],[4563,28,4563,41,0,0,4,1],[4567,23,4567,33,0,0,4,0],[4567,23,4567,33,0,0,4,1],[4581,13,4581,20,0,0,4,1],[4590,11,4590,30,0,0,4,0],[4590,11,4590,30,0,0,4,1],[4592,14,4592,38,0,0,4,0],[4592,14,4592,38,0,0,4,1],[4598,25,4598,38,0,0,4,0],[4598,25,4598,38,0,0,4,1],[4600,20,4600,41,0,0,4,0],[4600,20,4600,41,0,0,4,1],[4610,19,4610,44,0,0,4,0],[4616,25,4616,38,0,0,4,0],[4616,25,4616,38,0,0,4,1],[4618,20,4618,52,0,0,4,0],[4618,20,4618,52,0,0,4,1],[4619,20,4619,47,0,0,4,0],[4619,20,4619,47,0,0,4,1],[4643,13,4643,55,0,0,4,0],[4643,13,4643,55,0,0,4,1],[4644,8,4644,27,0,0,4,0],[4644,8,4644,27,0,0,4,1],[4646,11,4646,35,0,0,4,0],[4646,11,4646,35,0,0,4,1],[4653,22,4653,35,0,0,4,0],[4653,22,4653,35,0,0,4,1],[4655,17,4655,33,0,0,4,0],[4655,17,4655,33,0,0,4,1],[4655,37,4655,55,0,0,4,0],[4655,37,4655,55,0,0,4,1],[4655,59,4655,72,0,0,4,0],[4655,59,4655,72,0,0,4,1],[4666,16,4666,41,0,0,4,0],[4676,22,4676,35,0,0,4,0],[4676,22,4676,35,0,0,4,1],[4678,17,4678,38,0,0,4,0],[4678,17,4678,38,0,0,4,1],[4679,17,4679,37,0,0,4,0],[4679,17,4679,37,0,0,4,1],[4680,17,4680,40,0,0,4,0],[4680,17,4680,40,0,0,4,1],[4681,17,4681,39,0,0,4,0],[4681,17,4681,39,0,0,4,1],[4682,17,4682,39,0,0,4,0],[4682,17,4682,39,0,0,4,1],[4683,17,4683,38,0,0,4,0],[4683,17,4683,38,0,0,4,1],[534,6,534,23,4,0,4,0],[534,6,534,23,7,0,4,0]],"png_check_keyword":[[2093,12,2093,19,0,0,4,1],[2093,37,2093,65,0,0,4,1],[2098,16,2098,26,0,0,4,0],[2106,14,2106,22,0,0,4,1]],"png_set_user_transform_info":[[842,8,842,49,0,0,4,1]],"png_write_pHYs":[[1911,8,1911,40,0,0,4,1]],"png_write_sCAL_s":[[1885,8,1885,22,0,0,4,1]],"png_text_compress":[[522,8,522,19,0,0,4,1],[548,14,548,34,0,0,4,0],[555,14,555,45,0,0,4,1],[598,14,598,27,0,0,4,1],[606,14,606,25,0,0,4,1],[619,11,619,53,0,0,4,1],[634,11,634,30,0,0,4,0],[634,34,634,48,0,0,4,0]],"png_read_png":[[993,8,993,50,0,0,4,0],[1035,8,1035,48,0,0,4,0]],"png_write_tRNS":[[1352,8,1352,44,0,0,4,1],[1366,13,1366,46,0,0,4,1],[1381,13,1381,45,0,0,4,1]],"png_write_info":[[241,8,241,46,0,0,4,0],[249,8,249,45,0,0,4,0],[253,11,253,61,0,0,4,1],[272,8,272,46,0,0,4,0],[277,8,277,46,0,0,4,0],[296,8,296,46,0,0,4,0],[303,8,303,46,0,0,4,0],[309,8,309,46,0,0,4,0],[315,8,315,46,0,0,4,0],[335,11,335,44,0,0,4,0],[346,14,346,72,0,0,4,1],[356,16,356,74,0,0,4,0]],"png_write_zTXt":[[1646,8,1646,48,0,0,4,1],[1652,8,1652,48,0,0,4,1],[1657,8,1657,20,0,0,4,1],[1666,8,1666,20,0,0,4,1],[1668,8,1668,68,0,0,4,1]],"png_write_info_before_PLTE":[[157,14,157,52,0,0,4,0],[172,8,172,46,0,0,4,0],[178,8,178,46,0,0,4,0],[190,8,190,46,0,0,4,0],[209,14,209,52,0,0,4,0],[214,11,214,49,0,0,4,0],[219,14,219,52,0,0,4,0]],"png_deflate_claim":[[351,11,351,28,0,0,4,1],[405,11,405,63,0,0,4,0],[406,11,406,43,0,0,4,0],[412,14,412,51,0,0,4,1]],"png_write_sBIT":[[1268,8,1268,48,0,0,4,0],[1272,28,1272,62,0,0,4,1],[1275,11,1275,25,0,0,4,1],[1275,29,1275,48,0,0,4,1],[1276,11,1276,27,0,0,4,1],[1276,31,1276,52,0,0,4,1],[1277,11,1277,26,0,0,4,1],[1277,30,1277,50,0,0,4,1],[1301,8,1301,48,0,0,4,0],[1301,8,1301,48,0,0,4,1],[1303,11,1303,27,0,0,4,1],[1303,31,1303,67,0,0,4,1]],"png_do_expand_palette":[[4345,8,4345,54,0,0,4,0],[4347,11,4347,34,0,0,4,0],[4347,11,4347,34,0,0,4,1],[4351,13,4351,19,0,0,4,0],[4351,13,4351,19,0,0,4,1],[4356,28,4356,41,0,0,4,0],[4356,28,4356,41,0,0,4,1],[4358,23,4358,44,0,0,4,0],[4358,23,4358,44,0,0,4,1],[4364,23,4364,33,0,0,4,0],[4364,23,4364,33,0,0,4,1],[4378,13,4378,19,0,0,4,0],[4378,13,4378,19,0,0,4,1],[4383,28,4383,41,0,0,4,0],[4383,28,4383,41,0,0,4,1],[4387,23,4387,33,0,0,4,0],[4387,23,4387,33,0,0,4,1],[4401,13,4401,19,0,0,4,0],[4401,13,4401,19,0,0,4,1],[4406,28,4406,41,0,0,4,0],[4406,28,4406,41,0,0,4,1],[4410,23,4410,33,0,0,4,0],[4410,23,4410,33,0,0,4,1],[4424,13,4424,20,0,0,4,1],[4432,11,4432,35,0,0,4,0],[4435,17,4435,30,0,0,4,0],[4435,17,4435,30,0,0,4,1],[4441,28,4441,41,0,0,4,0],[4441,28,4441,41,0,0,4,1],[4443,23,4443,46,0,0,4,0],[4443,23,4443,46,0,0,4,1],[4463,28,4463,41,0,0,4,0],[4463,28,4463,41,0,0,4,1]],"png_write_sRGB":[[1131,8,1131,43,0,0,4,1]],"png_write_end":[[419,8,419,24,0,0,4,1]],"png_write_bKGD":[[1417,8,1417,44,0,0,4,1],[1434,13,1434,53,0,0,4,0],[1434,13,1434,53,0,0,4,1],[1440,11,1440,34,0,0,4,0],[1440,38,1440,69,0,0,4,0],[1440,38,1440,69,0,0,4,1],[1457,11,1457,50,0,0,4,1]],"png_write_pCAL":[[1821,8,1821,33,0,0,4,1],[1826,8,1826,24,0,0,4,1],[1832,33,1832,45,0,0,4,1],[1842,16,1842,27,0,0,4,0],[1842,16,1842,27,0,0,4,1],[1844,44,1844,60,0,0,4,0],[1844,44,1844,60,0,0,4,1],[1860,16,1860,27,0,0,4,0],[1860,16,1860,27,0,0,4,1]],"png_write_tIME":[[1933,8,1933,29,0,0,4,1],[1933,33,1933,53,0,0,4,1],[1934,8,1934,29,0,0,4,1],[1934,33,1934,53,0,0,4,1],[1935,8,1935,29,0,0,4,1],[1935,33,1935,54,0,0,4,1]],"png_write_PLTE":[[873,25,873,72,0,0,4,1],[878,8,878,72,0,0,4,0],[880,8,880,20,0,0,4,1],[880,25,880,53,0,0,4,1],[894,8,894,57,0,0,4,1],[907,35,907,46,0,0,4,0],[907,35,907,46,0,0,4,1]],"png_do_chop":[[2580,8,2580,33,0,0,4,0],[2580,8,2580,33,0,0,4,1],[2586,14,2586,21,0,0,4,0],[2586,14,2586,21,0,0,4,1]],"png_write_eXIf":[[1550,16,1550,28,0,0,4,0],[1550,16,1550,28,0,0,4,1]],"png_write_iTXt":[[1700,8,1700,20,0,0,4,1],[1706,7,1706,37,0,0,4,1],[1707,7,1707,37,0,0,4,1],[1711,7,1711,37,0,0,4,1],[1712,7,1712,37,0,0,4,0],[1716,7,1716,14,0,0,4,1],[1733,8,1733,20,0,0,4,1],[1735,8,1735,24,0,0,4,1],[1737,8,1737,20,0,0,4,1],[1740,8,1740,45,0,0,4,1],[1745,8,1745,49,0,0,4,1],[1752,8,1752,24,0,0,4,0],[1754,11,1754,74,0,0,4,1],[1775,8,1775,24,0,0,4,0]],"png_write_compressed_data_out":[[663,11,663,29,0,0,4,0],[670,11,670,26,0,0,4,0],[679,8,679,22,0,0,4,1]],"png_read_start_row":[[4663,16,4663,57,0,0,4,1],[4665,14,4665,37,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_destroy_read_struct","png_destroy_write_struct","png_sig_cmp","png_set_sig_bytes","png_set_read_user_transform_fn","png_set_user_transform_info","png_read_png","png_get_gAMA","png_init_io","png_write_info","png_set_write_user_transform_fn","png_set_user_transform_info","png_get_rows","png_write_rows","png_get_image_height","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_sig_cmp","png_set_sig_bytes","png_set_read_user_transform_fn","png_set_user_transform_info","png_read_png","png_get_gAMA","png_init_io","png_write_info","png_set_write_user_transform_fn","png_set_user_transform_info","png_get_rows","png_write_rows","png_get_image_height","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"visited":5}
/*Here's a C++ fuzz driver that implements PNG image processing with user transformations:

*/


// Define custom user transform structure
struct UserTransformData {
    double file_gamma;
    png_int_32 custom_int;
    png_byte signature[8];
};

// User transform function for reading
static void  id_000019_read_user_transform_fn(png_struct* png_ptr, png_row_info* row_info, png_byte* data) {
    // Simple transform: invert bits for demonstration
    for (png_uint_32 i = 0; i < row_info->rowbytes; ++i) {
        data[i] = ~data[i];
    }
}

// User transform function for writing  
static void  id_000019_write_user_transform_fn(png_struct* png_ptr, png_row_info* row_info, png_byte* data) {
    // Another simple transform: swap adjacent bytes for demonstration
    for (png_uint_32 i = 0; i + 1 < row_info->rowbytes; i += 2) {
        png_byte temp = data[i];
        data[i] = data[i + 1];
        data[i + 1] = temp;
    }
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<52) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need enough data for PNG operations
    
    // Create input file from memory buffer
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize read structures
    png_struct* read_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (!read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_info* read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize write structures
    png_struct* write_ptr = png_create_write_struct(fuzz_str_2, nullptr, nullptr, nullptr);
    if (!write_ptr) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_info* write_info = png_create_info_struct(write_ptr);
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
    
    // Set up read from input file
    png_init_io(read_ptr, in_file);
    
    // Check PNG signature from input data
    png_byte sig[8];
    if (fread(sig, 1, 8, in_file) != 8) {
        // Not enough data for signature
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Check if it's a PNG file
    if (png_sig_cmp(sig, fuzz_uint64_t_3, 8)) {
        // Not a PNG file, but we'll continue to test other APIs
        rewind(in_file);
    } else {
        // Set PNG signature
        png_set_sig_bytes(read_ptr, fuzz_int32_t_4);
    }
    
    // Prepare user transform data
    UserTransformData transform_data;
    transform_data.custom_int = 0;
    memset(transform_data.signature, 0, sizeof(transform_data.signature));
    
    // Get an integer value from the input buffer using png_get_int_32
    if (size >= 12) {
        // Use bytes 4-7 of the input as a 32-bit integer
        transform_data.custom_int = png_get_int_32(reinterpret_cast<const png_byte*>(data + 4));
    }
    
    // Set read user transform function
    png_set_read_user_transform_fn(read_ptr,  id_000019_read_user_transform_fn);
    
    // Set user transform info for reading
    png_set_user_transform_info(read_ptr, &transform_data, 
                               fuzz_int32_t_5,  // depth
                               fuzz_int32_t_6); // channels (assume RGBA)
    
    // Read the PNG image with transformations
    png_read_png(read_ptr, read_info, 
                 PNG_TRANSFORM_IDENTITY |  // Basic identity transform
                 PNG_TRANSFORM_STRIP_16 |  // Strip 16-bit to 8-bit
                 PNG_TRANSFORM_EXPAND,     // Expand paletted images
                 nullptr);
    
    // Try to get gamma information from the read image
    double file_gamma = 0.0;
    if (png_get_gAMA(read_ptr, read_info, &file_gamma)) {
        transform_data.file_gamma = file_gamma;
    }
    
    // Try to access signature (though not directly in API list, we simulate)
    if (size >= 8) {
        memcpy(transform_data.signature, data, 8);
    }
    
    // Set up write to output file
    png_init_io(write_ptr, out_file);
    
    // Copy information from read to write
    png_write_info(write_ptr, read_info);
    
    // Set write user transform function
    png_set_write_user_transform_fn(write_ptr,  id_000019_write_user_transform_fn);
    
    // Set user transform info for writing (reusing same data)
    png_set_user_transform_info(write_ptr, &transform_data,
                                fuzz_int32_t_7,  // depth
                                fuzz_int32_t_8); // channels
    
    // Write image data
    png_bytepp row_pointers = png_get_rows(read_ptr, read_info);
    png_write_rows(write_ptr, row_pointers, png_get_image_height(read_ptr, read_info));
    
    // Finish writing
    png_write_end(write_ptr, nullptr);
    
    // Clean up
    png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    png_destroy_write_struct(&write_ptr, &write_info);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}