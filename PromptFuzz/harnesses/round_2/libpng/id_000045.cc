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
//<ID> 798
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"png_read_data":[[35,8,35,37,0,0,4,1]]},"library_calls":["png_create_write_struct_2","png_create_info_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_write_struct","png_init_io","png_read_info","png_get_acTL","png_set_gAMA_fixed","png_get_mem_ptr","png_chunk_benign_error","png_init_io","png_write_info","png_write_flush","png_destroy_write_struct"],"critical_calls":["png_create_write_struct_2","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_acTL","png_set_gAMA_fixed","png_get_mem_ptr","png_chunk_benign_error","png_init_io","png_write_info","png_write_flush","png_destroy_write_struct"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        return 0; // Too small to be a valid PNG
    }

    // Create file pointer for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create write structure
    png_struct *png_write_ptr = png_create_write_struct_2(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );
    if (!png_write_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Create info structure
    png_info *info_ptr = png_create_info_struct(png_write_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_write_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Setup error handling
    if (setjmp(png_jmpbuf(png_write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&png_write_ptr, &info_ptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize IO for reading
    png_init_io(png_write_ptr, in_file);

    // Read PNG info
    png_read_info(png_write_ptr, info_ptr);

    // 1. Use png_get_uint_32 - convert first 4 bytes of data
    png_uint_32 converted_value = png_get_uint_32(data);

    // 2. Use png_get_acTL - get animation control info
    png_uint_32 num_frames = 0, num_plays = 0;
    png_uint_32 actl_result = png_get_acTL(png_write_ptr, info_ptr, &num_frames, &num_plays);

    // 3. Use png_set_gAMA_fixed - set gamma to fixed value
    // Using a reasonable gamma value (1.0 in fixed point format)
    png_set_gAMA_fixed(png_write_ptr, info_ptr, 1 * PNG_FP_1);

    // 4. Use png_get_mem_ptr - get memory pointer
    void *mem_ptr = png_get_mem_ptr(png_write_ptr);

    // 5. Use png_chunk_benign_error - report benign error
    // Report a benign error for demonstration
    png_chunk_benign_error(png_write_ptr, "Fuzzer: benign chunk error for testing");
	FUZZ_FILENAME_PUSH("output_file")

    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file) {
        // Re-initialize IO for writing to output file
        png_init_io(png_write_ptr, out_file);
        
        // Write PNG info
        png_write_info(png_write_ptr, info_ptr);
        
        // 6. Use png_write_flush
        png_write_flush(png_write_ptr);
        
        assert_file_closed(&out_file);;
        remove("output_file"); // Clean up temporary file
    }

    // Cleanup
    png_destroy_write_struct(&png_write_ptr, &info_ptr);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}