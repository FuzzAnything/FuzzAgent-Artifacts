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
//<ID> 28
//<Prompt> []
/*<Combination>: [
*/
//<score> 23.5, nr_unique_branch: 0
//<Quality> {"density":47,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_sig_cmp","png_set_sig_bytes","png_set_sig_bytes","png_read_info","png_get_IHDR","png_set_compression_mem_level","png_set_swap_alpha","png_set_strip_alpha","png_set_shift","png_set_read_user_transform_fn","png_read_update_info","png_get_pHYs_dpi","png_get_signature","png_get_num_frames","png_get_rowbytes","png_malloc","png_destroy_read_struct","png_read_row","png_free","png_read_end","png_destroy_read_struct","png_create_write_struct","png_create_info_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_write_struct","png_init_io","png_set_IHDR","png_write_info","png_write_row","png_write_end","png_destroy_write_struct","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_set_read_user_transform_fn","png_get_rowbytes","png_malloc","png_read_row","png_free","png_read_end","png_destroy_read_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_sig_cmp","png_set_sig_bytes","png_read_info","png_get_IHDR","png_set_compression_mem_level","png_set_swap_alpha","png_set_strip_alpha","png_set_shift","png_set_read_user_transform_fn","png_read_update_info","png_get_pHYs_dpi","png_get_signature","png_get_num_frames","png_get_rowbytes","png_malloc","png_read_row","png_free","png_read_end","png_destroy_read_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_write_info","png_write_row","png_write_end","png_destroy_write_struct","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_set_read_user_transform_fn","png_get_rowbytes","png_malloc","png_read_row","png_free","png_read_end","png_destroy_read_struct"],"visited":1}
/*To create a comprehensive fuzz driver for libpng using the specified APIs, I'll design a fuzzer that reads PNG data from memory, applies various transformations, and processes the image. Here's the complete implementation:

*/


// Custom user transform function that gets called during reading
static void  id_000004_user_transform_fn(png_struct* png_ptr, png_row_info* row_info, png_byte* data) {
    // Simple transformation: invert the bytes for demonstration
    for (png_uint_32 i = 0; i < row_info->rowbytes; i++) {
        data[i] = ~data[i];
    }
}

// Global variable to track user transform calls
static int  id_000004_user_transform_called = 0;

// Alternative user transform function that tracks calls
static void  id_000004_counting_transform_fn(png_struct* png_ptr, png_row_info* row_info, png_byte* data) {
     id_000004_user_transform_called++;
    // Do nothing else to avoid affecting fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
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

    // Reset global counter
     id_000004_user_transform_called = 0;

    // Create FILE pointer from memory buffer
    FILE* in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	FUZZ_FILENAME_PUSH("output_file")
	

    // Create output file
    FILE* out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize structures
    png_struct* png_read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                      nullptr, nullptr, nullptr);
    if (!png_read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_info* info_ptr = png_create_info_struct(png_read_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_read_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_read_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize I/O
    png_init_io(png_read_ptr, in_file);

    // Check signature
    unsigned char sig[8];
    fread(sig, 1, 8, in_file);
    rewind(in_file);
    
    if (png_sig_cmp(sig, 0, 8) != 0) {
        // Not a valid PNG, but we'll still try to process it
        png_set_sig_bytes(png_read_ptr, 0);
    } else {
        png_set_sig_bytes(png_read_ptr, 8);
    }

    // Read PNG info
    png_read_info(png_read_ptr, info_ptr);

    // Get basic image information
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type;
    png_get_IHDR(png_read_ptr, info_ptr, &width, &height, &bit_depth, 
                 &color_type, &interlace_type, nullptr, nullptr);

    // Call required APIs
    // 1. png_set_compression_mem_level - set compression memory level
    png_set_compression_mem_level(png_read_ptr, 8);

    // 2. png_set_swap_alpha - swap alpha channel position
    png_set_swap_alpha(png_read_ptr);

    // 3. png_set_strip_alpha - remove alpha channel
    png_set_strip_alpha(png_read_ptr);

    // 4. png_set_shift - shift pixel values
    png_color_8 true_bits;
    true_bits.red = 8;
    true_bits.green = 8;
    true_bits.blue = 8;
    true_bits.gray = 8;
    true_bits.alpha = 8;
    png_set_shift(png_read_ptr, &true_bits);

    // 5. png_set_read_user_transform_fn - set custom transform function
    png_set_read_user_transform_fn(png_read_ptr,  id_000004_user_transform_fn);

    // Update info after transformations
    png_read_update_info(png_read_ptr, info_ptr);

    // 6. png_get_pHYs_dpi - get physical dimensions
    png_uint_32 res_x, res_y;
    int unit_type;
    png_uint_32 has_phys = png_get_pHYs_dpi(png_read_ptr, info_ptr, &res_x, &res_y, &unit_type);

    // 7. Get signature (even though we already checked it)
    const png_byte* signature = png_get_signature(png_read_ptr, info_ptr);

    // 8. Try to get number of frames (for APNG)
    png_uint_32 num_frames = 1; // Default for non-animated PNGs
    #ifdef PNG_APNG_SUPPORTED
    num_frames = png_get_num_frames(png_read_ptr, info_ptr);
    #endif

    // Allocate row buffer
    png_uint_32 rowbytes = png_get_rowbytes(png_read_ptr, info_ptr);
    png_byte* row = (png_byte*)png_malloc(png_read_ptr, rowbytes);
    if (!row) {
        png_destroy_read_struct(&png_read_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 9. png_read_row - read image rows
    for (png_uint_32 y = 0; y < height; y++) {
        png_read_row(png_read_ptr, row, nullptr);
        
        // Process a few rows for fuzzing efficiency
        if (y >= 10 && height > 20) break; // Limit rows for performance
    }

    // Clean up reading structures
    png_free(png_read_ptr, row);
    png_read_end(png_read_ptr, info_ptr);
    png_destroy_read_struct(&png_read_ptr, &info_ptr, nullptr);

    // Now test writing functionality
    png_struct* png_write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                       nullptr, nullptr, nullptr);
    if (!png_write_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_info* write_info_ptr = png_create_info_struct(png_write_ptr);
    if (!write_info_ptr) {
        png_destroy_write_struct(&png_write_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    if (setjmp(png_jmpbuf(png_write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_init_io(png_write_ptr, out_file);

    // Set IHDR for writing
    png_set_IHDR(png_write_ptr, write_info_ptr, width, height, 8,
                 PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Write info
    png_write_info(png_write_ptr, write_info_ptr);

    // Create and write a simple test row
    png_byte* test_row = (png_byte*)malloc(rowbytes);
    if (test_row) {
        memset(test_row, 0xFF, rowbytes); // White row
        for (png_uint_32 i = 0; i < 3; i++) {
            png_write_row(png_write_ptr, test_row);
        }
        free(test_row);
    }

    png_write_end(png_write_ptr, write_info_ptr);
    png_destroy_write_struct(&png_write_ptr, &write_info_ptr);

    // Alternative test with different user transform function
    if (size > 100) { // Only if we have enough data
        rewind(in_file);
        
        png_struct* png_ptr2 = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                     nullptr, nullptr, nullptr);
        if (png_ptr2) {
            png_info* info_ptr2 = png_create_info_struct(png_ptr2);
            if (info_ptr2) {
                if (!setjmp(png_jmpbuf(png_ptr2))) {
                    png_init_io(png_ptr2, in_file);
                    png_read_info(png_ptr2, info_ptr2);
                    
                    // Use the counting transform function
                    png_set_read_user_transform_fn(png_ptr2,  id_000004_counting_transform_fn);
                    
                    // Read a few rows to trigger transform
                    png_uint_32 rowbytes2 = png_get_rowbytes(png_ptr2, info_ptr2);
                    png_byte* row2 = (png_byte*)png_malloc(png_ptr2, rowbytes2);
                    if (row2) {
                        for (int i = 0; i < 3; i++) {
                            png_read_row(png_ptr2, row2, nullptr);
                        }
                        png_free(png_ptr2, row2);
                    }
                    
                    png_read_end(png_ptr2, info_ptr2);
                }
                png_destroy_read_struct(&png_ptr2, &info_ptr2, nullptr);
            } else {
                png_destroy_read_struct(&png_ptr2, nullptr, nullptr);
            }
        }
    }

    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}