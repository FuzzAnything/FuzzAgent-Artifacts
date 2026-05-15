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
//<ID> 434
//<Prompt> []
/*<Combination>: [
*/
//<score> 174, nr_unique_branch: 11
//<Quality> {"density":29,"unique_branches":{"png_get_chunk_cache_max":[[1320,12,1320,19,0,0,4,0]],"png_get_x_pixels_per_meter":[[131,8,131,23,0,0,4,0],[131,27,131,43,0,0,4,0],[132,8,132,46,0,0,4,0],[132,8,132,46,0,0,4,1],[134,11,134,59,0,0,4,0]],"ppi_from_ppm":[[347,8,347,30,0,0,4,0],[347,34,348,18,0,0,4,0]],"png_set_crc_action":[[37,7,37,28,0,0,4,0],[52,7,52,30,0,0,4,1]],"png_muldiv":[[2748,11,2748,17,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_read_info","png_get_header_ver","png_get_header_version","png_get_chunk_cache_max","png_get_palette_max","png_get_x_pixels_per_inch","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_write_sig","png_save_uint_16","png_set_crc_action","png_malloc_default","png_set_hIST","png_free_data","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_header_ver","png_get_header_version","png_get_chunk_cache_max","png_get_palette_max","png_get_x_pixels_per_inch","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_write_sig","png_save_uint_16","png_set_crc_action","png_malloc_default","png_set_hIST","png_free_data","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<44) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    // Create read structure for input PNG data
    png_structp png_ptr_read = png_create_read_struct(
        fuzz_str_1, nullptr, nullptr, nullptr);
    if (!png_ptr_read) {
        return 0;
    }

    // Create info structure for reading
    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        return 0;
    }

    // Setup error handling
    if (setjmp(png_jmpbuf(png_ptr_read))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }

    // Open input data as file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize IO for reading
    png_init_io(png_ptr_read, in_file);
    
    // Read PNG header
    png_read_info(png_ptr_read, info_ptr_read);

    // Get header version information
    const char *header_ver = png_get_header_ver(png_ptr_read);
    const char *header_version = png_get_header_version(png_ptr_read);
    
    // Get chunk cache maximum
    png_uint_32 chunk_cache_max = png_get_chunk_cache_max(png_ptr_read);
    
    // Get palette maximum
    int palette_max = png_get_palette_max(png_ptr_read, info_ptr_read);
    
    // Get x pixels per inch
    png_uint_32 x_pixels_per_inch = png_get_x_pixels_per_inch(png_ptr_read, info_ptr_read);
    
    // Create write structure for output
    png_structp png_ptr_write = png_create_write_struct(
        fuzz_str_2, nullptr, nullptr, nullptr);
    if (!png_ptr_write) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create info structure for writing
    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Setup error handling for writing
    if (setjmp(png_jmpbuf(png_ptr_write))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Open output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize IO for writing
    png_init_io(png_ptr_write, out_file);
    
    // Write PNG signature
    png_write_sig(png_ptr_write);
    
    // Create buffer and save a 16-bit integer
    png_byte buf[2];
    png_uint_16 value = 0;
    if (size >= 2) {
        value = (data[0] << 8) | data[1];
    }
    png_save_uint_16(buf, fuzz_uint32_t_3);
    
    // Set CRC action (critical and ancillary chunks)
    png_set_crc_action(png_ptr_write, fuzz_int32_t_4, fuzz_int32_t_5);
    
    // Create and set histogram data if palette exists
    if (palette_max > 0) {
        png_uint_16 *hist = (png_uint_16 *)png_malloc_default(png_ptr_write, 
            palette_max * sizeof(png_uint_16));
        if (hist) {
            // Initialize histogram with data from input
            for (int i = 0; i < palette_max && i < (int)size; i++) {
                hist[i] = (png_uint_16)(data[i] * 1000);
            }
            // Set histogram
            png_set_hIST(png_ptr_write, info_ptr_write, hist);
            png_free_data(png_ptr_write, info_ptr_write, fuzz_uint32_t_6, fuzz_int32_t_7);
        }
    }
    
    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Destroy structures
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}