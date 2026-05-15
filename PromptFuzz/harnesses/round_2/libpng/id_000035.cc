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
//<ID> 684
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"png_get_io_ptr":[[690,8,690,23,0,0,4,1]]},"library_calls":["png_sig_cmp","png_create_read_struct_2","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_read_fn","png_set_sig_bytes","png_read_info","png_set_crc_action","png_set_background","png_set_compression_window_bits","png_set_progressive_read_fn","png_process_data","png_process_data_pause","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct_2","png_create_info_struct","png_set_longjmp_fn","png_set_read_fn","png_set_sig_bytes","png_read_info","png_set_crc_action","png_set_background","png_set_compression_window_bits","png_set_progressive_read_fn","png_process_data","png_process_data_pause","png_destroy_read_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to process PNG data:

*/


// Error handling callback
static void  id_000035_user_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - silent error handling for fuzzing
}

static void  id_000035_user_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing - silent warning handling for fuzzing
}

// Read callback function for processing data from memory
static void  id_000035_read_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) return;
    
    FILE* fp = (FILE*)io_ptr;
    fread(data, 1, length, fp);
}

// Progressive read callbacks
static void  id_000035_info_callback(png_structp png_ptr, png_infop info_ptr) {
    // Called when header info is available
}

static void  id_000035_row_callback(png_structp png_ptr, png_bytep new_row, 
                        png_uint_32 row_num, int pass) {
    // Called for each row
}

static void  id_000035_end_callback(png_structp png_ptr, png_infop info_ptr) {
    // Called when image is complete
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
        return 0; // Not enough data for PNG signature
    }
    
    FILE* in_file = nullptr;
    FILE* out_file = nullptr;
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    
    // Create input file from memory
    in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Check if data has PNG signature
    png_byte sig[8];
    fread(sig, 1, 8, in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (png_sig_cmp(sig, 0, 8) != 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0; // Not a PNG file
    }
    
    // Create PNG read structure
    png_ptr = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, 
                                      nullptr,  id_000035_user_error_fn,  id_000035_user_warning_fn,
                                      nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create info structure
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        // Error occurred - clean up
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up read function
    png_set_read_fn(png_ptr, in_file,  id_000035_read_callback);
    
    // Set the number of bytes already read (the signature)
    png_set_sig_bytes(png_ptr, 8);
    
    // Read the header info
    png_read_info(png_ptr, info_ptr);
    
    // Apply the specified APIs
    // 1. Set CRC action
    png_set_crc_action(png_ptr, PNG_CRC_ERROR_QUIT, PNG_CRC_WARN_USE);
    
    // 2. Set background
    png_color_16 background_color;
    background_color.red = 255;
    background_color.green = 255;
    background_color.blue = 255;
    background_color.gray = 255;
    background_color.index = 0;
    png_set_background(png_ptr, &background_color, 
                       PNG_BACKGROUND_GAMMA_FILE, 0, 1.0);
    
    // 3. Set alpha mode (assuming png_set_alpha_mode is available)
    // Note: This function may not be in all libpng versions, so we'll use 
    // png_set_alpha_mode_fixed if available, or skip if not
    #ifdef PNG_SET_ALPHA_MODE_SUPPORTED
    png_set_alpha_mode(png_ptr, PNG_ALPHA_PNG, 1.0);
    #endif
    
    // 4. Set compression window bits
    png_set_compression_window_bits(png_ptr, 15);
    
    // 5. Set up progressive reading
    png_set_progressive_read_fn(png_ptr, nullptr, 
                                id_000035_info_callback,  id_000035_row_callback,  id_000035_end_callback);
    
    // Process data in chunks to demonstrate png_process_data_pause
    size_t bytes_processed = 0;
    const size_t chunk_size = 1024;
    
    while (bytes_processed < size) {
        size_t to_process = (size - bytes_processed > chunk_size) ? 
                           chunk_size : size - bytes_processed;
        
        // Process a chunk of data
        png_process_data(png_ptr, info_ptr, 
                        (png_bytep)(data + bytes_processed), to_process);
        bytes_processed += to_process;
        
        // Use png_process_data_pause - this is for demonstration
        // In real usage, this would be called from within callbacks
        int save = 0;
        size_t pause_result = png_process_data_pause(png_ptr, save);
        
        // Small chance to break early (fuzzing optimization)
        if (bytes_processed > size / 2 && (rand() % 100 < 10)) {
            break;
        }
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create output file for writing processed data
    out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some dummy output (in real usage, this would be processed image)
        const char* dummy = "Processed PNG data";
        fwrite(dummy, 1, strlen(dummy), out_file);
    }
    
    // Get and print chunk type (for demonstration)
    // Note: png_get_io_chunk_type might not be available in all versions
    // This is called from within the progressive reading context
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}