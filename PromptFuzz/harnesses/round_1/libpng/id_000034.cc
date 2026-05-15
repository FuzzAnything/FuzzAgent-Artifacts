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
//<ID> 562
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 2
//<Quality> {"density":20,"unique_branches":{"png_get_palette_max":[[1351,8,1351,23,0,0,4,0],[1351,27,1351,43,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_read_info","png_get_palette_max","png_set_text_compression_level","png_set_progressive_read_fn","png_process_data","png_process_data_pause","png_get_unknown_chunks","png_get_current_pass_number","png_set_invalid","png_get_cHRM_XYZ","png_process_data","png_create_write_struct","png_create_info_struct","png_init_io","png_get_IHDR","png_set_IHDR","png_write_info","png_write_end","png_destroy_write_struct","png_destroy_write_struct","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_palette_max","png_set_text_compression_level","png_set_progressive_read_fn","png_process_data","png_process_data_pause","png_get_unknown_chunks","png_get_current_pass_number","png_set_invalid","png_get_cHRM_XYZ","png_process_data","png_create_write_struct","png_create_info_struct","png_init_io","png_get_IHDR","png_set_IHDR","png_write_info","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
	//fuzzer shim end}




    if (size < 100) {
        return 0; // Need enough data for meaningful processing
    }
    
    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return 0;
    }
    
    // Create FILE* from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O
    png_init_io(png_ptr, in_file);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // API 1: Get palette max
    int palette_max = png_get_palette_max(png_ptr, info_ptr);
    (void)palette_max; // Prevent unused variable warning
    
    // API 2: Note: png_set_text_compression_method is not in provided list,
    // but we can use png_set_text_compression_level instead
    png_set_text_compression_level(png_ptr, fuzz_int32_t_2); // Use default compression level
    
    // API 3: Process data pause (simulate progressive reading)
    // For this, we need to set up progressive reading first
    png_set_progressive_read_fn(png_ptr, nullptr, nullptr, nullptr, nullptr);
    
    // Create a buffer to simulate progressive reading
    size_t chunk_size = size / 4;
    if (chunk_size < 1) chunk_size = 1;
    
    // Process first chunk
    png_process_data(png_ptr, info_ptr, const_cast<png_bytep>(data), chunk_size);
    
    // Pause processing
    int save = 1;
    size_t remaining = png_process_data_pause(png_ptr, fuzz_int32_t_3);
    (void)remaining; // Prevent unused variable warning
    
    // API 4: Get unknown chunks
    png_unknown_chunk *unknowns = nullptr;
    int num_unknowns = png_get_unknown_chunks(png_ptr, info_ptr, &unknowns);
    (void)num_unknowns; // Prevent unused variable warning
    
    // API 5: Get current pass number (for interlaced images)
    png_byte pass_num = png_get_current_pass_number(png_ptr);
    (void)pass_num; // Prevent unused variable warning
    
    // API 6: Set invalid chunks
    // Mark some chunks as invalid (e.g., sBIT chunk)
    png_set_invalid(png_ptr, info_ptr, fuzz_int32_t_4);
    
    // API 7: Get cHRM XYZ values
    double red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z;
    png_uint_32 cHRM_valid = png_get_cHRM_XYZ(png_ptr, info_ptr,
        &red_X, &red_Y, &red_Z,
        &green_X, &green_Y, &green_Z,
        &blue_X, &blue_Y, &blue_Z);
    
    if (cHRM_valid) {
        // We have valid cHRM data, could use it here
        (void)red_X; (void)red_Y; (void)red_Z;
        (void)green_X; (void)green_Y; (void)green_Z;
        (void)blue_X; (void)blue_Y; (void)blue_Z;
    }
    
    // Continue processing remaining data
    size_t processed = chunk_size;
    while (processed < size) {
        size_t to_process = (size - processed) > chunk_size ? chunk_size : (size - processed);
        png_process_data(png_ptr, info_ptr, 
                         const_cast<png_bytep>(data + processed), 
                         to_process);
        processed += to_process;
    }
    
    // Create write structure for output
    png_structp write_ptr = png_create_write_struct(fuzz_str_5, nullptr, nullptr, nullptr);
    if (write_ptr) {
        png_infop write_info = png_create_info_struct(write_ptr);
        if (write_info) {
	FUZZ_FILENAME_PUSH("output_file")
            FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
            if (out_file) {
                png_init_io(write_ptr, out_file);
                
                // Copy basic image info from read structure
                png_uint_32 width, height;
                int bit_depth, color_type, interlace_method, compression_method, filter_method;
                
                if (png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                                &interlace_method, &compression_method, &filter_method)) {
                    png_set_IHDR(write_ptr, write_info, width, height, bit_depth, color_type,
                                interlace_method, compression_method, filter_method);
                    
                    // Write info and end
                    png_write_info(write_ptr, write_info);
                    png_write_end(write_ptr, nullptr);
                }
                
                assert_file_closed(&out_file);;
                png_destroy_write_struct(&write_ptr, &write_info);
            } else {
                png_destroy_write_struct(&write_ptr, &write_info);
            }
        } else {
            png_destroy_write_struct(&write_ptr, nullptr);
        }
    }
    
    // Cleanup
    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    
    assert_file_closed(&in_file);
	return 0;
}