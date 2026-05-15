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
//<ID> 829
//<Prompt> []
/*<Combination>: [
*/
//<score> 3, nr_unique_branch: 2
//<Quality> {"density":1,"unique_branches":{"png_set_check_for_invalid_index":[[2051,8,2051,19,0,0,4,0]],"png_set_text_compression_level":[[1287,8,1287,23,0,0,4,1]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_create_write_struct_2","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_init_io","png_set_text_compression_level","png_set_mDCV","png_set_check_for_invalid_index","png_set_sig_bytes","png_read_info","png_start_read_image","png_get_acTL","png_get_IHDR","png_get_rowbytes","png_calloc","png_read_row","png_get_IHDR","png_set_IHDR","png_write_info","png_write_row","png_free","png_read_end","png_write_end","png_destroy_read_struct","png_destroy_read_struct","png_destroy_write_struct","png_destroy_write_struct"],"critical_calls":["png_sig_cmp","png_write_end","png_read_end","png_free","png_write_row","png_write_info","png_set_IHDR","png_get_IHDR","png_read_row","png_calloc","png_get_rowbytes","png_get_IHDR","png_get_acTL","png_start_read_image","png_read_info","png_set_sig_bytes","png_set_check_for_invalid_index","png_set_mDCV","png_set_text_compression_level","png_init_io","png_init_io","png_set_longjmp_fn","png_set_longjmp_fn","png_create_info_struct","png_create_write_struct_2","png_create_info_struct","png_create_read_struct","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/*I'll create a fuzz driver that uses the specified libpng APIs to process PNG data with animation control and mDCV (Mastering Display Color Volume) information. Here's the implementation:

*/


// Include necessary libpng headers

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for too small input
    if (size < 8 || !png_sig_cmp((png_bytep)data, 0, 8)) {
        return 0; // Not PNG data
    }

    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    png_struct *read_ptr = nullptr;
    png_struct *write_ptr = nullptr;
    png_info *read_info = nullptr;
    png_info *write_info = nullptr;
    png_byte *row = nullptr;
    
    try {
        // 1. Create FILE* for input using fmemopen
        in_file = fmemopen((void *)data, size, "rb");
        if (!in_file) {
            assert_file_closed(&in_file);
	return 0;
        }
        
        // 2. Create write FILE* (though we won't actually write in fuzzing)
        out_file = fopen("/tmp/libpng_fuzz_output.png", "wb");
        if (!out_file) {
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
        
        // 3. Create read structure
        read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                          nullptr, nullptr, nullptr);
        if (!read_ptr) {
            throw "Failed to create read struct";
        }
        
        // 4. Create info structure for reading
        read_info = png_create_info_struct(read_ptr);
        if (!read_info) {
            throw "Failed to create read info";
        }
        
        // 5. Create write structure
        write_ptr = png_create_write_struct_2(PNG_LIBPNG_VER_STRING,
                                              nullptr, nullptr, nullptr,
                                              nullptr, nullptr, nullptr);
        if (!write_ptr) {
            throw "Failed to create write struct";
        }
        
        // 6. Create info structure for writing
        write_info = png_create_info_struct(write_ptr);
        if (!write_info) {
            throw "Failed to create write info";
        }
        
        // Set up error handling
        if (setjmp(png_jmpbuf(read_ptr)) || setjmp(png_jmpbuf(write_ptr))) {
            throw "Error in PNG processing";
        }
        
        // 7. Set up read and write I/O
        png_init_io(read_ptr, in_file);
        png_init_io(write_ptr, out_file);
        
        // 8. Set the text compression level (for writing)
        png_set_text_compression_level(write_ptr, 6); // Medium compression
        
        // 9. Set mDCV (Mastering Display Color Volume) information
        // Using DCI-P3 color space values
        png_set_mDCV(write_ptr, write_info,
                     0.314, 0.351,  // white point (D65)
                     0.680, 0.320,  // red primary
                     0.265, 0.690,  // green primary
                     0.150, 0.060,  // blue primary
                     1000.0, 0.001); // max/min luminance
        
        // 10. Set check for invalid index
        png_set_check_for_invalid_index(read_ptr, 1); // Check and warn
        
        // 11. Read PNG signature
        png_set_sig_bytes(read_ptr, 8);
        
        // 12. Read PNG info
        png_read_info(read_ptr, read_info);
        
        // 13. Call png_start_read_image
        png_start_read_image(read_ptr);
        
        // 14. Get animation control (acTL chunk)
        png_uint_32 num_frames = 0, num_plays = 0;
        if (png_get_acTL(read_ptr, read_info, &num_frames, &num_plays)) {
            // Process animation if present
            if (num_frames > 0) {
                // Allocate buffer for frame processing
                png_uint_32 width = 0, height = 0;
                png_get_IHDR(read_ptr, read_info, &width, &height,
                             nullptr, nullptr, nullptr, nullptr, nullptr);
                
                // 15. Use png_calloc to allocate memory for a row buffer
                if (width > 0 && height > 0) {
                    size_t rowbytes = png_get_rowbytes(read_ptr, read_info);
                    if (rowbytes > 0 && rowbytes < 1000000) { // Sanity check
                        row = (png_byte*)png_calloc(read_ptr, rowbytes);
                        
                        if (row) {
                            // Read a single row to exercise the reading path
                            png_read_row(read_ptr, row, nullptr);
                            
                            // Copy info to write struct
                            png_uint_32 width = 0, height = 0;
                            int bit_depth = 0, color_type = 0;
                            png_get_IHDR(read_ptr, read_info, &width, &height,
                                         &bit_depth, &color_type,
                                         nullptr, nullptr, nullptr);
                            
                            // Set write info
                            png_set_IHDR(write_ptr, write_info, width, height,
                                         bit_depth, color_type,
                                         PNG_INTERLACE_NONE,
                                         PNG_COMPRESSION_TYPE_DEFAULT,
                                         PNG_FILTER_TYPE_DEFAULT);
                            
                            // Write info
                            png_write_info(write_ptr, write_info);
                            
                            // Write the row we read
                            png_write_row(write_ptr, row);
                        }
                    }
                }
            }
        }
        
        // 16. Clean up and finish
        if (row) {
            png_free(read_ptr, row);
        }
        
        png_read_end(read_ptr, read_info);
        png_write_end(write_ptr, write_info);
        
    } catch (const char* msg) {
        // Error occurred, clean up
    }
    
    // Cleanup all resources
    if (read_ptr && read_info) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    } else if (read_ptr) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
    }
    
    if (write_ptr && write_info) {
        png_destroy_write_struct(&write_ptr, &write_info);
    } else if (write_ptr) {
        png_destroy_write_struct(&write_ptr, nullptr);
    }
    
    if (in_file) assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    // Remove temporary output file
    remove("/tmp/libpng_fuzz_output.png");
    
    return 0;
}