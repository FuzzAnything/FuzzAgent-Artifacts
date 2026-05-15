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
//<ID> 471
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 1
//<Quality> {"density":32,"unique_branches":{"png_get_current_pass_number":[[889,8,889,23,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_longjmp_fn","png_set_read_fn","png_init_io","png_read_info","png_get_signature","png_chunk_warning","png_get_sCAL_s","png_get_sCAL","png_get_IHDR","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_write_info","png_get_rowbytes","png_write_row","png_write_chunk","png_get_pHYs_dpi","png_get_current_pass_number","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_set_read_fn","png_init_io","png_read_info","png_get_signature","png_chunk_warning","png_get_sCAL_s","png_get_sCAL","png_get_IHDR","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_write_info","png_get_rowbytes","png_write_row","png_write_chunk","png_get_pHYs_dpi","png_get_current_pass_number","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"visited":1}
/**/


// Using provided typedefs and function prototypes
typedef unsigned char png_byte;
typedef unsigned int png_uint_32;

// External C functions from libpng that we need
extern "C" {
    png_struct *png_create_read_struct(const char * user_png_ver, void * error_ptr, 
                                      png_error_ptr error_fn, png_error_ptr warn_fn);
    png_info *png_create_info_struct(const png_struct * png_ptr);
    jmp_buf *png_set_longjmp_fn(png_struct * png_ptr, png_longjmp_ptr longjmp_fn, 
                               size_t jmp_buf_size);
    void png_set_read_fn(png_struct * png_ptr, void * io_ptr, png_rw_ptr read_data_fn);
    void png_read_info(png_struct * png_ptr, png_info * info_ptr);
    const png_byte *png_get_signature(const png_struct * png_ptr, const png_info * info_ptr);
    void png_chunk_warning(const png_struct * png_ptr, const char * warning_message);
    png_uint_32 png_get_sCAL_s(const png_struct * png_ptr, const png_info * info_ptr, 
                              int * unit, char ** swidth, char ** sheight);
    png_uint_32 png_get_sCAL(const png_struct * png_ptr, const png_info * info_ptr, 
                            int * unit, double * width, double * height);
    void png_init_io(png_struct * png_ptr, FILE * fp);
    png_uint_32 png_get_IHDR(const png_struct * png_ptr, const png_info * info_ptr,
                            png_uint_32 * width, png_uint_32 * height, int * bit_depth,
                            int * color_type, int * interlace_method, int * compression_method,
                            int * filter_method);
    void png_set_IHDR(const png_struct * png_ptr, png_info * info_ptr,
                     png_uint_32 width, png_uint_32 height, int bit_depth,
                     int color_type, int interlace_method, int compression_method,
                     int filter_method);
    void png_write_info(png_struct * png_ptr, const png_info * info_ptr);
    size_t png_get_rowbytes(const png_struct * png_ptr, const png_info * info_ptr);
    void png_write_row(png_struct * png_ptr, const png_byte * row);
    void png_write_chunk(png_struct * png_ptr, const png_byte * chunk_name,
                        const png_byte * data, size_t length);
    png_uint_32 png_get_pHYs_dpi(const png_struct * png_ptr, const png_info * info_ptr,
                                png_uint_32 * res_x, png_uint_32 * res_y, int * unit_type);
    void png_write_end(png_struct * png_ptr, png_info * info_ptr);
    void png_destroy_read_struct(png_struct ** png_ptr_ptr, png_info ** info_ptr_ptr,
                                png_info ** end_info_ptr_ptr);
    png_byte png_get_current_pass_number(const png_struct * png_ptr);
    void png_destroy_write_struct(png_struct ** png_ptr_ptr, png_info ** info_ptr_ptr);
}

// Custom read function for memory buffer
static void  id_000033_user_read_data(png_structp png_ptr, png_bytep data, size_t length) {
    void* io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) return;
    
    FILE* fp = (FILE*)io_ptr;
    size_t read = fread(data, 1, length, fp);
    if (read < length) {
        png_error(png_ptr, "Read Error");
    }
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<77) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeCharArray(unsigned char, fuzz_char_array_10, fuzz_char_array_size_10, fdp);
	
		const png_byte * fuzzer_var_png_write_chunk_28_2 = fuzz_char_array_10;
		size_t fuzzer_size_png_write_chunk_28_2 = static_cast<size_t>(fuzz_char_array_size_10);//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	 // Need at least PNG signature
    
    FILE* in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	FUZZ_FILENAME_PUSH("output_file")
	
    
    FILE* out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create read structures
    png_structp png_read_ptr = png_create_read_struct(
        fuzz_str_1, nullptr, nullptr, nullptr);
    if (!png_read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop read_info_ptr = png_create_info_struct(png_read_ptr);
    if (!read_info_ptr) {
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling with longjmp
    if (setjmp(png_jmpbuf(png_read_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        remove("output_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }
    
    png_set_longjmp_fn(png_read_ptr, longjmp, sizeof(jmp_buf));
    
    // Set up read function
    png_set_read_fn(png_read_ptr, in_file,  id_000033_user_read_data);
    png_init_io(png_read_ptr, in_file);
    
    // Read PNG info
    png_read_info(png_read_ptr, read_info_ptr);
    
    // Get and check signature
    const png_byte* signature = png_get_signature(png_read_ptr, read_info_ptr);
    if (signature) {
        // Signature is valid
    }
    
    // Issue a warning
    png_chunk_warning(png_read_ptr, fuzz_str_2);
    
    // Try to get sCAL data
    int unit = 0;
    char* swidth = nullptr;
    char* sheight = nullptr;
    png_get_sCAL_s(png_read_ptr, read_info_ptr, &unit, &swidth, &sheight);
    
    // Try to get sCAL as doubles
    double width_d = 0, height_d = 0;
    png_get_sCAL(png_read_ptr, read_info_ptr, &unit, &width_d, &height_d);
    
    // Get image dimensions
    png_uint_32 width = 0, height = 0;
    int bit_depth = 0, color_type = 0;
    int interlace_method = 0, compression_method = 0, filter_method = 0;
    png_get_IHDR(png_read_ptr, read_info_ptr, &width, &height, &bit_depth,
                &color_type, &interlace_method, &compression_method, &filter_method);
    
    // Create write structures
    png_structp png_write_ptr = png_create_write_struct(
        fuzz_str_3, nullptr, nullptr, nullptr);
    if (!png_write_ptr) {
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop write_info_ptr = png_create_info_struct(png_write_ptr);
    if (!write_info_ptr) {
        png_destroy_write_struct(&png_write_ptr, nullptr);
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    if (setjmp(png_jmpbuf(png_write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        remove("output_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }
    
    png_set_longjmp_fn(png_write_ptr, longjmp, sizeof(jmp_buf));
    png_init_io(png_write_ptr, out_file);
    
    // Set IHDR for output
    png_set_IHDR(png_write_ptr, write_info_ptr, width, fuzz_uint32_t_4, fuzz_int32_t_5,
                fuzz_int32_t_6, fuzz_int32_t_7, fuzz_int32_t_8, fuzz_int32_t_9);
    
    // Write info
    png_write_info(png_write_ptr, write_info_ptr);
    
    // Get rowbytes for buffer allocation
    size_t rowbytes = png_get_rowbytes(png_write_ptr, write_info_ptr);
    
    // Allocate row buffer
    png_byte* row = (png_byte*)malloc(rowbytes);
    if (row) {
        // Try to write a dummy row
        for (png_uint_32 y = 0; y < height && y < 10; y++) {
            // Fill with pattern
            for (size_t x = 0; x < rowbytes; x++) {
                row[x] = (png_byte)((x + y) % 256);
            }
            png_write_row(png_write_ptr, row);
        }
        free(row);
    }
    
    // Write a custom chunk
    png_byte chunk_data[] = {0x46, 0x55, 0x5A, 0x5A}; // "FUZZ"
    png_write_chunk(png_write_ptr, (png_byte*)"tEXt", fuzzer_var_png_write_chunk_28_2, fuzzer_size_png_write_chunk_28_2);
    
    // Get physical dimensions
    png_uint_32 res_x = 0, res_y = 0;
    int unit_type = 0;
    png_get_pHYs_dpi(png_read_ptr, read_info_ptr, &res_x, &res_y, &unit_type);
    
    // Get current pass number (for interlaced images)
    png_byte pass = png_get_current_pass_number(png_read_ptr);
    
    // Write end
    png_write_end(png_write_ptr, write_info_ptr);
    
    // Cleanup
    png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
    png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file to avoid accumulating files
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}