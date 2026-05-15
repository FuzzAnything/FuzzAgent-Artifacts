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
//<ID> 1152
//<Prompt> []
/*<Combination>: [
*/
//<score> 330, nr_unique_branch: 133
//<Quality> {"density":22,"unique_branches":{"png_set_expand_16":[[1003,8,1003,37,0,0,4,1]],"png_push_read_IDAT":[[533,8,533,52,0,0,4,0],[533,8,533,52,0,0,4,1],[552,11,552,42,0,0,4,0],[552,46,552,74,0,0,4,1],[580,11,580,42,0,0,4,0],[580,11,580,42,0,0,4,1],[580,46,580,75,0,0,4,0],[587,14,587,60,0,0,4,1],[591,14,591,43,0,0,4,1],[602,11,602,39,0,0,4,1],[610,8,610,31,0,0,4,0],[610,8,610,31,0,0,4,1],[610,35,610,65,0,0,4,1],[637,8,637,31,0,0,4,0],[637,8,637,31,0,0,4,1],[637,35,637,68,0,0,4,0],[647,11,647,32,0,0,4,0],[647,11,647,32,0,0,4,1],[663,8,663,31,0,0,4,0],[663,8,663,31,0,0,4,1],[31,5,31,29,2,0,4,0],[31,5,31,29,2,0,4,1],[31,5,31,29,13,0,4,0],[31,5,31,29,13,0,4,1]],"png_push_have_end":[[1079,8,1079,31,0,0,4,1]],"png_process_some_data":[[115,7,115,29,0,0,4,1],[121,7,121,31,0,0,4,0],[127,7,127,30,0,0,4,0],[133,7,133,14,0,0,4,0]],"png_get_cHRM_XYZ_fixed":[[608,8,608,23,0,0,4,0],[608,27,608,43,0,0,4,0],[609,8,609,47,0,0,4,0],[609,8,609,47,0,0,4,1],[610,8,610,51,0,0,4,0],[612,11,612,28,0,0,4,0],[613,11,613,28,0,0,4,0],[614,11,614,28,0,0,4,0],[615,11,615,30,0,0,4,0],[616,11,616,30,0,0,4,0],[617,11,617,30,0,0,4,0],[618,11,618,29,0,0,4,0],[619,11,619,29,0,0,4,0],[620,11,620,29,0,0,4,0]],"png_set_rows":[[1873,8,1873,23,0,0,4,1],[1873,27,1873,43,0,0,4,1],[1876,8,1876,38,0,0,4,1],[1882,8,1882,28,0,0,4,0]],"png_push_have_info":[[1072,8,1072,32,0,0,4,1]],"png_process_data_pause":[[67,8,67,23,0,0,4,0],[72,11,72,20,0,0,4,1],[83,14,83,51,0,0,4,1]],"png_process_IDAT_data":[[678,8,678,28,0,0,4,1],[678,32,678,46,0,0,4,1],[683,8,683,45,0,0,4,0],[683,49,683,77,0,0,4,1],[701,11,701,40,0,0,4,0],[702,7,702,53,0,0,4,0],[702,7,702,53,0,0,4,1],[711,11,711,44,0,0,4,1],[730,11,730,22,0,0,4,0],[730,26,730,45,0,0,4,0],[739,14,739,54,0,0,4,0],[790,8,790,37,0,0,4,0]],"png_push_read_sig":[[162,8,162,72,0,0,4,1],[164,11,164,26,0,0,4,0],[165,11,165,79,0,0,4,0],[165,11,165,79,0,0,4,1],[173,11,173,34,0,0,4,0]],"png_push_read_chunk":[[194,8,194,52,0,0,4,0],[194,8,194,52,0,0,4,1],[204,8,204,36,0,0,4,0],[204,8,204,36,0,0,4,1],[205,8,205,55,0,0,4,1],[302,8,302,30,0,0,4,0],[302,8,302,30,0,0,4,1],[304,11,304,48,0,0,4,1],[311,11,311,47,0,0,4,0],[311,11,311,47,0,0,4,1],[314,16,314,61,0,0,4,0],[314,16,314,61,0,0,4,1],[315,11,315,47,0,0,4,0],[315,11,315,47,0,0,4,1],[320,11,320,47,0,0,4,1],[327,11,327,48,0,0,4,1],[331,13,331,49,0,0,4,0],[331,13,331,49,0,0,4,1],[339,8,339,30,0,0,4,0],[339,8,339,30,0,0,4,1],[341,11,341,37,0,0,4,0],[341,11,341,37,0,0,4,1],[348,13,348,35,0,0,4,0],[348,13,348,35,0,0,4,1],[358,13,358,74,0,0,4,1],[368,13,368,35,0,0,4,0],[368,13,368,35,0,0,4,1],[384,13,384,35,0,0,4,0],[384,13,384,35,0,0,4,1],[386,11,386,58,0,0,4,0],[386,11,386,58,0,0,4,1],[395,13,395,35,0,0,4,0],[395,13,395,35,0,0,4,1],[397,11,397,58,0,0,4,0],[397,11,397,58,0,0,4,1],[31,5,31,29,2,0,4,0],[31,5,31,29,2,0,4,1],[28,5,28,52,32,0,4,0],[28,5,28,52,32,0,4,1],[28,5,28,52,34,0,4,0],[28,5,28,52,34,0,4,1],[534,6,534,23,41,0,4,0],[534,6,534,23,41,0,4,1],[28,5,28,52,44,0,4,0],[28,5,28,52,44,0,4,1]],"png_push_save_buffer":[[463,8,463,38,0,0,4,0],[463,8,463,38,0,0,4,1],[465,11,465,59,0,0,4,1],[479,8,480,32,0,0,4,0],[479,8,480,32,0,0,4,1],[485,11,486,47,0,0,4,1],[496,11,496,39,0,0,4,1],[502,11,502,21,0,0,4,0],[502,11,502,21,0,0,4,1],[504,16,504,41,0,0,4,1],[509,8,509,36,0,0,4,0],[509,8,509,36,0,0,4,1]],"png_push_fill_buffer":[[426,8,426,38,0,0,4,0],[430,11,430,45,0,0,4,0],[430,11,430,45,0,0,4,1],[443,23,443,56,0,0,4,0],[447,11,447,48,0,0,4,0],[447,11,447,48,0,0,4,1]],"png_get_chunk_malloc_max":[[1327,12,1327,19,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_get_chunk_malloc_max","png_set_expand_16","png_set_progressive_read_fn","png_process_data","png_process_data_pause","png_process_data","png_get_cHRM_XYZ_fixed","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_rowbytes","png_malloc","png_malloc","png_set_rows","png_free","png_free","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_get_chunk_malloc_max","png_set_expand_16","png_set_progressive_read_fn","png_process_data","png_process_data_pause","png_process_data","png_get_cHRM_XYZ_fixed","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_rowbytes","png_malloc","png_malloc","png_set_rows","png_free","png_free","png_destroy_read_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to achieve progressive PNG data processing with cHRM extraction:

*/


// Include libpng headers

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    if (size < 50) {
	return 0;
	}
	 // Need minimum data for PNG processing
    
    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, nullptr, 
                                                  nullptr, nullptr);
    if (!png_ptr) {
	return 0;
	}
	
    
    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // API 1: Get maximum chunk allocation size
    png_alloc_size_t chunk_malloc_max = png_get_chunk_malloc_max(png_ptr);
    // We'll use this to limit memory usage if needed
    
    // API 2: Convert two bytes to 16-bit integer
    // Use first two bytes of data for demonstration
    png_uint_16 uint16_val = 0;
    if (size >= 2) {
        uint16_val = png_get_uint_16(reinterpret_cast<const png_byte*>(data));
    }
    
    // API 3: Set expand 16 mode (for 16-bit image data)
    png_set_expand_16(png_ptr);
    
    // Set up progressive reading from memory
    png_set_progressive_read_fn(png_ptr, info_ptr, nullptr, nullptr, nullptr);
    
    // Process data in chunks to demonstrate png_process_data_pause
    size_t processed = 0;
    size_t chunk_size = 1024; // Process in 1KB chunks
    
    while (processed < size) {
        size_t to_process = (size - processed > chunk_size) ? 
                           chunk_size : (size - processed);
        
        // Process a chunk of data
        png_process_data(png_ptr, info_ptr, 
                        reinterpret_cast<png_bytep>(const_cast<uint8_t*>(data + processed)), 
                        to_process);
        
        processed += to_process;
        
        // API 4: Pause data processing (save=0 means don't save state)
        if (processed < size) {
            size_t pause_bytes = png_process_data_pause(png_ptr, fuzz_int32_t_2);
            // In real usage, we might save state or do other processing here
            // For fuzzing, we just continue
        }
    }
    
    // Finish processing
    png_process_data(png_ptr, info_ptr, nullptr, 0);
    
    // API 5: Try to get cHRM XYZ fixed-point values if available
    png_fixed_point red_X, red_Y, red_Z;
    png_fixed_point green_X, green_Y, green_Z;
    png_fixed_point blue_X, blue_Y, blue_Z;
    
    png_uint_32 has_cHRM = png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
                                                 &red_X, &red_Y, &red_Z,
                                                 &green_X, &green_Y, &green_Z,
                                                 &blue_X, &blue_Y, &blue_Z);
    
    if (has_cHRM) {
        // cHRM data is available - we could use it here
        // For fuzzing, we just ensure the API was called
    }
    
    // API 6: Set rows (for demonstration - typically used for writing)
    // Create dummy row pointers for demonstration
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    
    if (width > 0 && height > 0) {
        // Calculate row bytes
        int color_type = png_get_color_type(png_ptr, info_ptr);
        int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        
        png_bytepp row_pointers = nullptr;
        size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
        
        if (rowbytes > 0 && rowbytes < 1000000) { // Reasonable limit
            // Allocate row pointers
            row_pointers = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
            
            // Allocate rows
            for (png_uint_32 y = 0; y < height; y++) {
                row_pointers[y] = (png_bytep)png_malloc(png_ptr, rowbytes);
                // Fill with zeros for safety
                memset(row_pointers[y], 0, rowbytes);
            }
            
            // Set the rows
            png_set_rows(png_ptr, info_ptr, row_pointers);
            
            // Clean up allocated rows
            for (png_uint_32 y = 0; y < height; y++) {
                png_free(png_ptr, row_pointers[y]);
            }
            png_free(png_ptr, row_pointers);
        }
    }
    
    // Alternative approach using FILE* as mentioned in instructions
    // This demonstrates how to use fmemopen when needed
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (in_file) {
        // Could use standard libpng file reading here
        // For this fuzz driver, we've already processed the data
        assert_file_closed(&in_file);;
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    assert_file_closed(&in_file);
	return 0;
}