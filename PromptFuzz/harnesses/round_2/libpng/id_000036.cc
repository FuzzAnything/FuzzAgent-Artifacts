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
//<ID> 686
//<Prompt> []
/*<Combination>: [
*/
//<score> 51, nr_unique_branch: 17
//<Quality> {"density":17,"unique_branches":{"png_set_crc_action":[[28,8,28,23,0,0,4,1],[34,7,34,29,0,0,4,1],[37,7,37,28,0,0,4,0],[42,7,42,29,0,0,4,1],[48,7,48,32,0,0,4,1],[52,7,52,30,0,0,4,1],[54,7,54,27,0,0,4,1],[55,7,55,14,0,0,4,1],[63,7,63,29,0,0,4,1],[66,7,66,28,0,0,4,1],[71,7,71,29,0,0,4,0],[77,7,77,30,0,0,4,1],[82,7,82,32,0,0,4,1],[84,7,84,27,0,0,4,1],[85,7,85,14,0,0,4,1]],"png_calculate_crc":[[155,11,156,71,0,0,4,0],[171,8,171,21,0,0,4,1]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_error_fn","png_set_crc_action","png_set_read_fn","png_get_io_ptr","png_get_io_ptr","png_set_sig_bytes","png_set_progressive_read_fn","png_read_info","png_get_IHDR","png_destroy_read_struct","png_set_background","png_process_data","png_process_data_pause","png_read_end","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_error_fn","png_set_crc_action","png_set_read_fn","png_get_io_ptr","png_get_io_ptr","png_set_sig_bytes","png_set_progressive_read_fn","png_read_info","png_get_IHDR","png_set_background","png_process_data","png_process_data_pause","png_read_end","png_destroy_read_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to process PNG data with progressive reading:

*/


extern "C" {
}

// Define callback functions for progressive reading
void  id_000036_info_callback(png_structp png_ptr, png_infop info_ptr) {
    // Called when header information is read
}

void  id_000036_row_callback(png_structp png_ptr, png_bytep new_row, 
                  png_uint_32 row_num, int pass) {
    // Called when a row is read
}

void  id_000036_end_callback(png_structp png_ptr, png_infop info_ptr) {
    // Called when image is complete
}

// Error handling function
void  id_000036_user_error_fn(png_structp png_ptr, const char *error_msg) {
    // Longjmp back to setjmp point
    longjmp(png_jmpbuf(png_ptr), 1);
}

// Warning handling function  
void  id_000036_user_warning_fn(png_structp png_ptr, const char *warning_msg) {
    // Do nothing for warnings in fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	 // Need at least PNG signature
    
    // Create FILE pointer for reading input
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Check PNG signature
    png_byte header[8];
    if (fread(header, 1, 8, in_file) != 8) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    if (png_sig_cmp(header, 0, 8) != 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set error handling functions
    png_set_error_fn(png_ptr, nullptr,  id_000036_user_error_fn,  id_000036_user_warning_fn);
    
    // Set CRC handling: warn on critical chunks, no action on ancillary
    png_set_crc_action(png_ptr, PNG_CRC_WARN_USE, PNG_CRC_QUIET_USE);
    
    // Set up read function
    png_set_read_fn(png_ptr, in_file, [](png_structp png_ptr, 
                                         png_bytep outBytes, 
                                         png_size_t byteCountToRead) {
        FILE *fp = (FILE*)png_get_io_ptr(png_ptr);
        fread(outBytes, 1, byteCountToRead, fp);
    });
    
    // Let libpng know we already read some bytes
    png_set_sig_bytes(png_ptr, 8);
    
    // Set up progressive reading
    png_set_progressive_read_fn(png_ptr, nullptr, 
                                 id_000036_info_callback,  id_000036_row_callback,  id_000036_end_callback);
    
    // Read info
    png_read_info(png_ptr, info_ptr);
    
    // Get image information
    png_uint_32 width, height;
    int bit_depth, color_type;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, 
                 &color_type, nullptr, nullptr, nullptr);
    
    if (width == 0 || height == 0 || width > 10000 || height > 10000) {
        // Skip suspiciously large or small images
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set background color (white background)
    png_color_16 background_color;
    memset(&background_color, 0, sizeof(background_color));
    background_color.red = 65535;
    background_color.green = 65535;
    background_color.blue = 65535;
    background_color.gray = 65535;
    png_set_background(png_ptr, &background_color, 
                       PNG_BACKGROUND_GAMMA_SCREEN, 0, 1.0);
    
    // Process data in chunks to demonstrate png_process_data_pause
    const size_t CHUNK_SIZE = 4096;
    size_t processed_bytes = 0;
    int save = 0;
    
    // Read remaining data in chunks
    fseek(in_file, 0, SEEK_SET); // Reset to start
    fread(header, 1, 8, in_file); // Skip signature again
    
    while (processed_bytes < size - 8) {
        size_t bytes_to_process = size - 8 - processed_bytes;
        if (bytes_to_process > CHUNK_SIZE) {
            bytes_to_process = CHUNK_SIZE;
        }
        
        // Read chunk
        png_byte chunk[CHUNK_SIZE];
        size_t bytes_read = fread(chunk, 1, bytes_to_process, in_file);
        
        if (bytes_read > 0) {
            // Process the chunk
            png_process_data(png_ptr, info_ptr, chunk, bytes_read);
            
            // Pause processing to demonstrate the API
            // Note: We need to process data in a way that makes sense for png_process_data_pause
            // In a real scenario, this would be called between chunks
            size_t remaining = png_process_data_pause(png_ptr, save);
            
            processed_bytes += bytes_read;
            
            // For demonstration, we'll save state after first chunk
            if (processed_bytes >= CHUNK_SIZE && save == 0) {
                save = 1; // Enable saving state
            }
        } else {
            break;
        }
    }
    
    // Complete reading
    png_read_end(png_ptr, info_ptr);
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}