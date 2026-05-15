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
//<ID> 459
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 3
//<Quality> {"density":14,"unique_branches":{"png_push_read_sig":[[164,11,164,26,0,0,4,0],[165,11,165,79,0,0,4,0],[165,11,165,79,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_process_data","png_get_signature","png_sig_cmp","png_create_write_struct","png_create_info_struct","png_init_io","png_set_IHDR","png_write_info","png_write_row","png_write_end","png_destroy_write_struct","png_image_begin_read_from_memory","png_image_finish_read","png_image_free","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_process_data","png_get_signature","png_sig_cmp","png_create_write_struct","png_create_info_struct","png_init_io","png_set_IHDR","png_write_info","png_write_row","png_write_end","png_destroy_write_struct","png_image_begin_read_from_memory","png_image_finish_read","png_image_free","png_destroy_read_struct"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	  // Need at least PNG signature bytes

    // Create read structure
    png_struct *png_ptr = png_create_read_struct(fuzz_str_1, 
                                                 nullptr, nullptr, nullptr);
    if (!png_ptr) {
	return 0;
	}
	

    // Create info structure
    png_info *info_ptr = png_create_info_struct(png_ptr);
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

    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize PNG reading
    png_init_io(png_ptr, in_file);
    png_set_sig_bytes(png_ptr, fuzz_int32_t_2);  // We'll check signature manually

    // Process the data
    png_process_data(png_ptr, info_ptr, (png_byte *)data, size);

    // Get signature (should match PNG signature)
    const png_byte *signature = png_get_signature(png_ptr, info_ptr);
    if (signature && png_sig_cmp(signature, fuzz_uint64_t_3, 8) != 0) {
        // Valid PNG signature found
        // For this fuzzing driver, we'll simulate writing a row
        // by creating a simple row buffer and calling png_write_row
        
        // Create write structure
        png_struct *write_ptr = png_create_write_struct(fuzz_str_4,
                                                       nullptr, nullptr, nullptr);
        if (write_ptr) {
            png_info *write_info = png_create_info_struct(write_ptr);
            if (write_info) {
	FUZZ_FILENAME_PUSH("output_file")
                // Create output file
                FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
                if (out_file) {
                    png_init_io(write_ptr, out_file);
                    
                    // Set up image parameters (using reasonable defaults)
                    png_set_IHDR(write_ptr, write_info, 
                                fuzz_uint32_t_5, 100,  // width, height
                                fuzz_int32_t_6,         // bit depth
                                fuzz_int32_t_7,
                                fuzz_int32_t_8,
                                fuzz_int32_t_9,
                                fuzz_int32_t_10);
                    
                    png_write_info(write_ptr, write_info);
                    
                    // Create a sample row and write it
                    png_byte row[300];  // 100 pixels * 3 bytes
                    memset(row, 0, sizeof(row));
                    png_write_row(write_ptr, row);
                    
                    png_write_end(write_ptr, nullptr);
                    assert_file_closed(&out_file);;
                }
                png_destroy_write_struct(&write_ptr, &write_info);
            }
        }
    }

    // Prepare for simplified API read
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    // Try to read the image using simplified API
    if (png_image_begin_read_from_memory(&image, data, size)) {
        // Allocate buffer for image data
        png_alloc_size_t buffer_size = PNG_IMAGE_SIZE(image);
        if (buffer_size > 0 && buffer_size < 1000000) {  // Sanity check
            void *buffer = malloc(buffer_size);
            if (buffer) {
                // Finish reading (simulate - we don't have background or colormap)
                png_image_finish_read(&image, nullptr, buffer, fuzz_int32_t_11, nullptr);
                free(buffer);
            }
        }
        png_image_free(&image);
    }

    // Cleanup
    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    assert_file_closed(&in_file);
	return 0;
}