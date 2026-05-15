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
//<ID> 1143
//<Prompt> []
/*<Combination>: [
*/
//<score> 114, nr_unique_branch: 5
//<Quality> {"density":38,"unique_branches":{"png_fixed_ITU":[[2721,8,2721,23,0,0,4,1],[2721,27,2721,32,0,0,4,1]],"png_set_crc_action":[[42,7,42,29,0,0,4,0],[66,7,66,28,0,0,4,1],[71,7,71,29,0,0,4,0]]},"library_calls":["png_sig_cmp","png_image_begin_read_from_memory","png_image_free","png_create_read_struct","png_image_free","png_create_info_struct","png_destroy_read_struct","png_image_free","png_create_info_struct","png_destroy_read_struct","png_image_free","png_set_longjmp_fn","png_destroy_read_struct","png_image_free","png_destroy_read_struct","png_image_free","png_init_io","png_set_sig_bytes","png_set_scale_16","png_set_tRNS","png_set_cLLI","png_set_crc_action","png_read_info","png_read_update_info","png_get_image_height","png_get_rowbytes","png_malloc","png_malloc","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_init_io","png_set_IHDR","png_get_image_width","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_write_info","png_write_rows","png_write_end","png_destroy_write_struct","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct","png_image_free"],"critical_calls":["png_sig_cmp","png_image_begin_read_from_memory","png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_set_scale_16","png_set_tRNS","png_set_cLLI","png_set_crc_action","png_read_info","png_read_update_info","png_get_image_height","png_get_rowbytes","png_malloc","png_malloc","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_init_io","png_set_IHDR","png_get_filter_type","png_get_compression_type","png_get_interlace_type","png_get_color_type","png_get_bit_depth","png_get_image_width","png_write_info","png_write_rows","png_write_end","png_destroy_write_struct","png_free","png_free","png_destroy_read_struct","png_image_free"],"visited":0}
/**/


// Error handling structure
struct FuzzError {
    jmp_buf jmpbuf;
    bool error_occurred;
};

// Error callback for libpng
static void  id_000051_png_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    FuzzError* ferr = static_cast<FuzzError*>(png_get_error_ptr(png_ptr));
    ferr->error_occurred = true;
    longjmp(ferr->jmpbuf, 1);
}

// Warning callback for libpng
static void  id_000051_png_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Suppress warnings during fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<49) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeCharArray(unsigned char, fuzz_char_array_4, fuzz_char_array_size_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
	
		const png_byte * fuzzer_var_png_set_tRNS_19_2 = fuzz_char_array_4;
		int fuzzer_size_png_set_tRNS_19_2 = static_cast<int>(fuzz_char_array_size_4);//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	 // Need at least PNG signature
    
    // Step 1: Check PNG signature
    if (png_sig_cmp((png_byte*)data, fuzz_uint64_t_1, 8) != 0) {
        return 0; // Not a PNG file
    }
    
    // Step 2: Use simplified API to read image
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    if (!png_image_begin_read_from_memory(&image, data, size)) {
        png_image_free(&image);
        return 0;
    }
    
    // Step 3: Setup traditional PNG reading structures
    FuzzError ferr;
    ferr.error_occurred = false;
    
    png_structp png_ptr = png_create_read_struct(
        fuzz_str_2, 
        &ferr, 
         id_000051_png_error_handler, 
         id_000051_png_warning_handler
    );
    
    if (!png_ptr) {
        png_image_free(&image);
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        png_image_free(&image);
        return 0;
    }
    
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        return 0;
    }
    
    // Step 4: Setup longjmp error recovery
    png_set_longjmp_fn(png_ptr, longjmp, sizeof(jmp_buf));
    
    if (setjmp(ferr.jmpbuf)) {
	FUZZ_FILE_CLOSE();
        // Error recovery
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        png_image_free(&image);
        return 0;
    }
    
    // Step 5: Setup input reading
    FILE* in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        png_image_free(&image);
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_init_io(png_ptr, in_file);
    png_set_sig_bytes(png_ptr, fuzz_int32_t_3); // We already checked 8 bytes
    
    // Step 6: Configure PNG transformations
    png_set_scale_16(png_ptr); // Scale 16-bit to 8-bit
    
    // Setup some dummy transparency data
    png_byte trans_alpha[] = {0, 255};
    png_color_16 trans_color = {0, 30000, 20000, 10000, 50000};
    png_set_tRNS(png_ptr, info_ptr, fuzzer_var_png_set_tRNS_19_2, fuzzer_size_png_set_tRNS_19_2, &trans_color);
    
    // Setup CLLI (Content Light Level Information)
    png_set_cLLI(png_ptr, info_ptr, 1000.0, 500.0);
    
    // Setup CRC handling
    png_set_crc_action(png_ptr, fuzz_int32_t_5, fuzz_int32_t_6);
    
    // Step 7: Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Update info after transformations
    png_read_update_info(png_ptr, info_ptr);
    
    // Step 8: Get image properties
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    // Step 9: Allocate memory for image rows
    png_bytepp row_pointers = static_cast<png_bytepp>(
        png_malloc(png_ptr, height * sizeof(png_bytep))
    );
    
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = static_cast<png_bytep>(png_malloc(png_ptr, rowbytes));
    }
    
    // Step 10: Read image data
    png_read_image(png_ptr, row_pointers);
    png_read_end(png_ptr, end_info);
	FUZZ_FILENAME_PUSH("output_file")
    
    // Step 11: Setup output file for writing
    FILE* out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file) {
        // Create write structure
        png_structp png_write_ptr = png_create_write_struct(
            fuzz_str_7,
            &ferr,
             id_000051_png_error_handler,
             id_000051_png_warning_handler
        );
        
        if (png_write_ptr) {
            png_infop write_info = png_create_info_struct(png_write_ptr);
            
            if (write_info && !setjmp(ferr.jmpbuf)) {
                png_init_io(png_write_ptr, out_file);
                
                // Copy info from read structure to write structure
                // This is simplified - in reality would need to copy all necessary chunks
                png_set_IHDR(
                    png_write_ptr, write_info,
                    png_get_image_width(png_ptr, info_ptr),
                    height,
                    png_get_bit_depth(png_ptr, info_ptr),
                    png_get_color_type(png_ptr, info_ptr),
                    png_get_interlace_type(png_ptr, info_ptr),
                    png_get_compression_type(png_ptr, info_ptr),
                    png_get_filter_type(png_ptr, info_ptr)
                );
                
                png_write_info(png_write_ptr, write_info);
                png_write_rows(png_write_ptr, row_pointers, height);
                png_write_end(png_write_ptr, write_info);
                
                png_destroy_write_struct(&png_write_ptr, &write_info);
            } else {
                if (png_write_ptr) png_destroy_write_struct(&png_write_ptr, write_info ? &write_info : nullptr);
            }
        }
        
        assert_file_closed(&out_file);;
        remove("output_file");
    }
    
    // Step 12: Cleanup
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);
    
    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    png_image_free(&image);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}