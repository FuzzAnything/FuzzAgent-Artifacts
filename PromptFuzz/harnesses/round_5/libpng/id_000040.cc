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
//<ID> 560
//<Prompt> []
/*<Combination>: [
*/
//<score> 33, nr_unique_branch: 3
//<Quality> {"density":11,"unique_branches":{"png_sig_cmp":[[84,8,84,24,0,0,4,0]],"png_push_read_sig":[[162,8,162,72,0,0,4,0],[164,11,164,26,0,0,4,1]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_set_progressive_read_fn","png_get_progressive_ptr","png_set_strip_alpha","png_get_sBIT","png_warning","png_get_pixels_per_inch","png_set_text_compression_level","png_set_interlace_handling","png_get_progressive_ptr","png_set_strip_alpha","png_get_sBIT","png_warning","png_get_pixels_per_inch","png_set_text_compression_level","png_set_interlace_handling","png_get_progressive_ptr","png_get_current_row_number","png_process_data_skip","png_get_progressive_ptr","png_get_current_row_number","png_process_data_skip","png_process_data","png_warning","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_info_struct","png_destroy_write_struct","png_init_io","png_set_text_compression_level","png_set_strip_alpha","png_destroy_info_struct","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_set_progressive_read_fn","png_process_data_skip","png_get_current_row_number","png_get_progressive_ptr","png_process_data_skip","png_get_current_row_number","png_get_progressive_ptr","png_set_interlace_handling","png_set_text_compression_level","png_get_pixels_per_inch","png_warning","png_get_sBIT","png_set_strip_alpha","png_get_progressive_ptr","png_set_interlace_handling","png_set_text_compression_level","png_get_pixels_per_inch","png_warning","png_get_sBIT","png_set_strip_alpha","png_get_progressive_ptr","png_process_data","png_warning","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_text_compression_level","png_set_strip_alpha","png_destroy_info_struct","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/**/


// Include libpng headers

// Custom types from libpng
typedef unsigned int png_uint_32;
typedef unsigned char png_byte;

// Event: Process a PNG image with progressive reading, strip alpha channel,
// set text compression level, handle interlacing, and extract metadata
extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	  // Too small to be a valid PNG
    
    // Check PNG signature
    if (png_sig_cmp((png_const_bytep)data, 0, 8) != 0) {
        return 0;  // Not a PNG file
    }
    
    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize structures
    png_struct *png_ptr = nullptr;
    png_info *info_ptr = nullptr;
    
    // Set up error handling
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up longjmp error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O
    png_init_io(png_ptr, in_file);
    png_set_sig_bytes(png_ptr, 8);
    
    // Set up progressive reading callbacks (dummy implementations)
    struct ProgressiveData {
        png_struct *png_ptr;
        png_info *info_ptr;
    } prog_data = {png_ptr, info_ptr};
    
    png_set_progressive_read_fn(png_ptr, &prog_data,
        [](png_structp png_ptr, png_infop info_ptr) {
            // info_callback
            auto data = static_cast<ProgressiveData*>(png_get_progressive_ptr(png_ptr));
            
            // Use required APIs
            png_set_strip_alpha(png_ptr);
            
            // Get and print warning about sBIT
            png_color_8 *sig_bit = nullptr;
            if (png_get_sBIT(png_ptr, info_ptr, &sig_bit)) {
                // We got sBIT info
            } else {
                png_warning(png_ptr, "No sBIT chunk found");
            }
            
            // Get pixels per inch
            png_uint_32 ppi = png_get_pixels_per_inch(png_ptr, info_ptr);
            
            // Set text compression level (for any text chunks)
            png_set_text_compression_level(png_ptr, 6);  // Medium compression
            
            // Set interlace handling
            int num_passes = png_set_interlace_handling(png_ptr);
        },
        [](png_structp png_ptr, png_bytep new_row, png_uint_32 row_num, int pass) {
            // row_callback
            auto data = static_cast<ProgressiveData*>(png_get_progressive_ptr(png_ptr));
            
            // Get current row number
            png_uint_32 current_row = png_get_current_row_number(png_ptr);
            
            // Skip some rows (every other row as an example)
            if (current_row % 2 == 0) {
                png_process_data_skip(png_ptr);
            }
        },
        [](png_structp png_ptr, png_infop info_ptr) {
            // end_callback
        });
    
    // Process the PNG data in chunks
    const size_t CHUNK_SIZE = 4096;
    size_t bytes_processed = 0;
    
    while (bytes_processed < size) {
        size_t chunk_size = (size - bytes_processed) > CHUNK_SIZE ? 
                           CHUNK_SIZE : (size - bytes_processed);
        
        png_process_data(png_ptr, info_ptr, 
                        (png_bytep)(data + bytes_processed), chunk_size);
        
        bytes_processed += chunk_size;
        
        // Break early if we've processed enough to test the APIs
        if (bytes_processed >= 1024 && bytes_processed < size) {
            // Force a warning to be called
            png_warning(png_ptr, "Partial processing complete");
            break;
        }
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create output file for writing processed data
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file) {
        // Initialize write structures
        png_struct *write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                       nullptr, nullptr, nullptr);
        if (write_ptr) {
            png_info *write_info = png_create_info_struct(write_ptr);
            if (write_info) {
                if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
                    png_destroy_info_struct(write_ptr, &write_info);
                    png_destroy_write_struct(&write_ptr, nullptr);
                    assert_file_closed(&out_file);;
                } else {
                    png_init_io(write_ptr, out_file);
                    
                    // Re-use some APIs on write structure
                    png_set_text_compression_level(write_ptr, 9);  // Maximum compression
                    png_set_strip_alpha(write_ptr);
                    
                    png_destroy_info_struct(write_ptr, &write_info);
                }
            }
            png_destroy_write_struct(&write_ptr, nullptr);
        }
        assert_file_closed(&out_file);;
        
        // Clean up output file
        remove("output_file");
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}