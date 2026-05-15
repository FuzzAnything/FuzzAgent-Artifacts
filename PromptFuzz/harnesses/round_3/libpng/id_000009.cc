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
//<ID> 121
//<Prompt> []
/*<Combination>: [
*/
//<score> 37, nr_unique_branch: 0
//<Quality> {"density":37,"unique_branches":{},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_read_fn","png_set_write_fn","png_set_sig_bytes","png_read_info","png_get_IHDR","png_set_background_fixed","png_set_strip_16","png_read_update_info","png_get_next_frame_y_offset","png_set_IHDR","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_write_flush","png_process_data_pause","png_write_end","png_free","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_set_read_fn","png_set_write_fn","png_set_sig_bytes","png_read_info","png_get_IHDR","png_set_background_fixed","png_set_strip_16","png_read_update_info","png_get_next_frame_y_offset","png_set_IHDR","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_write_flush","png_process_data_pause","png_write_end","png_free","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to process PNG data:

*/


extern "C" {
}

// Static callbacks for libpng
static void  id_000009_custom_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Silent error handler for fuzzing
}

static void  id_000009_custom_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Silent warning handler for fuzzing
}

static void  id_000009_custom_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    FILE* fp = (FILE*)png_get_io_ptr(png_ptr);
    if (fp) {
        fread(data, 1, length, fp);
    }
}

static void  id_000009_custom_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    FILE* fp = (FILE*)png_get_io_ptr(png_ptr);
    if (fp) {
        fwrite(data, 1, length, fp);
    }
}

static void  id_000009_custom_flush_fn(png_structp png_ptr) {
    FILE* fp = (FILE*)png_get_io_ptr(png_ptr);
    if (fp) {
        fflush(fp);
    }
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
	//fuzzer shim end}




    if (size < 8) {
        return 0;
    }

    // Check if it's a PNG file
    if (png_sig_cmp((png_bytep)data, fuzz_uint64_t_1, 8) != 0) {
        return 0;
    }

    // Create read structure
    png_structp png_ptr_r = png_create_read_struct(fuzz_str_2, nullptr, 
                                                    id_000009_custom_error_fn,  id_000009_custom_warning_fn);
    if (!png_ptr_r) {
        return 0;
    }

    // Create info structure
    png_infop info_ptr_r = png_create_info_struct(png_ptr_r);
    if (!info_ptr_r) {
        png_destroy_read_struct(&png_ptr_r, nullptr, nullptr);
        return 0;
    }

    // Create write structure
    png_structp png_ptr_w = png_create_write_struct(fuzz_str_3, nullptr,
                                                     id_000009_custom_error_fn,  id_000009_custom_warning_fn);
    if (!png_ptr_w) {
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        return 0;
    }

    // Create info structure for writing
    png_infop info_ptr_w = png_create_info_struct(png_ptr_w);
    if (!info_ptr_w) {
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        png_destroy_write_struct(&png_ptr_w, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr_r))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        png_destroy_write_struct(&png_ptr_w, &info_ptr_w);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr_w))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        png_destroy_write_struct(&png_ptr_w, &info_ptr_w);
        return 0;
    }

    // Create FILE* for input data
    FILE* in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        png_destroy_write_struct(&png_ptr_w, &info_ptr_w);
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    // Create FILE* for output
    FILE* out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        png_destroy_write_struct(&png_ptr_w, &info_ptr_w);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up read function
    png_set_read_fn(png_ptr_r, in_file,  id_000009_custom_read_fn);

    // Set up write function
    png_set_write_fn(png_ptr_w, out_file,  id_000009_custom_write_fn,  id_000009_custom_flush_fn);

    // Set that we've already read the signature
    png_set_sig_bytes(png_ptr_r, 8);

    // Read PNG info
    png_read_info(png_ptr_r, info_ptr_r);

    // Get image dimensions
    png_uint_32 width, height;
    int bit_depth, color_type;
    png_get_IHDR(png_ptr_r, info_ptr_r, &width, &height, &bit_depth, &color_type,
                 nullptr, nullptr, nullptr);

    // Set up transformations using the specified APIs
    // 1. Set background color (using fixed point API)
    png_color_16 background_color = {0, 0, 0, 0, 0};
    png_set_background_fixed(png_ptr_r, &background_color, 
                            fuzz_int32_t_4, fuzz_int32_t_5, fuzz_int32_t_6);

    // 2. Strip 16-bit to 8-bit
    png_set_strip_16(png_ptr_r);

    // 3. Set an option (use PNG_OPTION_ON for a safe option)
    #ifdef PNG_OPTION_SUPPORTED
    png_set_option(png_ptr_r, PNG_OPTION_ON, PNG_OPTION_ON);
    #endif

    // Update info after transformations
    png_read_update_info(png_ptr_r, info_ptr_r);

    // 4. Get next frame y offset (for APNG support)
    png_uint_32 y_offset = png_get_next_frame_y_offset(png_ptr_r, info_ptr_r);
    // y_offset may be 0 for non-APNG images, but we still call the API

    // Set write info based on read info
    png_set_IHDR(png_ptr_w, info_ptr_w, width, height, 
                 bit_depth <= 8 ? bit_depth : 8,  // After strip_16
                 color_type, 
                 fuzz_int32_t_7, 
                 fuzz_int32_t_8, 
                 fuzz_int32_t_9);

    // Write PNG info
    png_write_info(png_ptr_w, info_ptr_w);

    // Allocate row buffer
    png_bytep row = (png_bytep)png_malloc(png_ptr_r, 
                                         png_get_rowbytes(png_ptr_r, info_ptr_r));

    // Read and write image row by row
    for (png_uint_32 y = 0; y < height; y++) {
        png_read_row(png_ptr_r, row, nullptr);
        png_write_row(png_ptr_w, row);
        
        // 5. Flush output periodically
        if (y % 10 == 0) {
            png_write_flush(png_ptr_w);
        }
    }

    // 6. Use process data pause API (simulate progressive reading scenario)
    // Since we're not doing progressive reading in this simple example,
    // we'll create a minimal progressive-like scenario
    png_byte buffer[1024];
    size_t bytes_processed = 0;
    while (bytes_processed < size) {
        size_t chunk_size = size - bytes_processed > 1024 ? 1024 : size - bytes_processed;
        memcpy(buffer, data + bytes_processed, chunk_size);
        
        // In a real progressive scenario, we would call png_process_data here
        // For demonstration, we'll just call the pause function
        size_t save_state = 0;
        size_t pause_result = png_process_data_pause(png_ptr_r, fuzz_int32_t_10);
        
        bytes_processed += chunk_size;
        if (pause_result > 0) {
            break; // Simulate pause
        }
    }

    // Finish writing
    png_write_end(png_ptr_w, nullptr);

    // Clean up
    png_free(png_ptr_r, row);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Destroy structures
    png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
    png_destroy_write_struct(&png_ptr_w, &info_ptr_w);

    // Clean up output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}