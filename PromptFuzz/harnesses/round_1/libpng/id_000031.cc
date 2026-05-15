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
//<ID> 456
//<Prompt> []
/*<Combination>: [
*/
//<score> 74, nr_unique_branch: 14
//<Quality> {"density":37,"unique_branches":{"png_get_mDCV_fixed":[[838,8,838,23,0,0,4,0],[838,27,838,43,0,0,4,0],[839,8,839,46,0,0,4,0],[839,8,839,46,0,0,4,1],[841,11,841,26,0,0,4,0],[842,11,842,26,0,0,4,0],[843,11,843,24,0,0,4,0],[844,11,844,24,0,0,4,0],[845,11,845,26,0,0,4,0],[846,11,846,26,0,0,4,0],[847,11,847,25,0,0,4,0],[848,11,848,25,0,0,4,0],[849,11,849,34,0,0,4,0],[850,11,850,34,0,0,4,0]]},"library_calls":["png_create_read_struct","png_set_user_limits","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_free","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_read_info","png_get_IHDR","png_create_write_struct_2","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_free","png_destroy_read_struct","png_destroy_write_struct","png_set_IHDR","png_set_sCAL_fixed","png_save_uint_16","png_set_alpha_mode_fixed","png_get_mDCV_fixed","png_set_shift","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_write_info","png_get_rowbytes","png_malloc","png_read_row","png_write_row","png_free","png_read_end","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_set_user_limits","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_IHDR","png_create_write_struct_2","png_create_info_struct","png_set_longjmp_fn","png_set_IHDR","png_set_sCAL_fixed","png_save_uint_16","png_set_alpha_mode_fixed","png_get_mDCV_fixed","png_set_shift","png_init_io","png_write_info","png_get_rowbytes","png_malloc","png_read_row","png_write_row","png_free","png_read_end","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/


// Include libpng headers

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<44) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    // Initialize libpng structures
    png_structp png_ptr_read = nullptr;
    png_structp png_ptr_write = nullptr;
    png_infop info_ptr_read = nullptr;
    png_infop info_ptr_write = nullptr;
    png_infop end_info_read = nullptr;
    FILE* in_file = nullptr;
    FILE* out_file = nullptr;
    png_bytep row_buffer = nullptr;
    jmp_buf* read_jmpbuf = nullptr;
    jmp_buf* write_jmpbuf = nullptr;
    
    // Custom memory functions (using default)
    png_malloc_ptr malloc_fn = nullptr;
    png_free_ptr free_fn = nullptr;
    void* mem_ptr = nullptr;
    
    // Error handling setup
    if (size == 0) {
	return 0;
	}
	
    
    // 1. Create read structure
    png_ptr_read = png_create_read_struct(
        fuzz_str_1,
        nullptr,  // error_ptr
        nullptr,  // error_fn
        nullptr   // warn_fn
    );
    if (!png_ptr_read) {
	return 0;
	}
	
    
    // 2. Set user limits for safety
    png_set_user_limits(png_ptr_read, fuzz_uint32_t_2, fuzz_uint32_t_3);
    
    // 3. Create info structure for reading
    info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        return 0;
    }
    
    // 4. Set longjmp function for read struct
    read_jmpbuf = png_set_longjmp_fn(png_ptr_read, longjmp, sizeof(jmp_buf));
    if (setjmp(*read_jmpbuf)) {
	FUZZ_FILE_CLOSE();
        // Error handling for read operations
        if (row_buffer) png_free(png_ptr_read, row_buffer);
        if (in_file) assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, &end_info_read);
        return 0;
    }
    
    // 5. Create input file from memory buffer
    in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Initialize I/O for reading
    png_init_io(png_ptr_read, in_file);
    
    // 7. Read PNG info
    png_read_info(png_ptr_read, info_ptr_read);
    
    // 8. Get IHDR information
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(png_ptr_read, info_ptr_read, &width, &height, &bit_depth,
                 &color_type, &interlace_method, &compression_method, &filter_method);
    
    // 9. Create write structure with version 2 API
    png_ptr_write = png_create_write_struct_2(
        fuzz_str_4,
        nullptr,  // error_ptr
        nullptr,  // error_fn
        nullptr,  // warn_fn
        mem_ptr,  // mem_ptr
        malloc_fn,
        free_fn
    );
    if (!png_ptr_write) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 10. Create info structure for writing
    info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 11. Set longjmp function for write struct
    write_jmpbuf = png_set_longjmp_fn(png_ptr_write, longjmp, sizeof(jmp_buf));
    if (setjmp(*write_jmpbuf)) {
	FUZZ_FILE_CLOSE();
        // Error handling for write operations
        if (row_buffer) png_free(png_ptr_read, row_buffer);
        if (in_file) assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, &end_info_read);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 12. Set IHDR for output PNG
    png_set_IHDR(png_ptr_write, info_ptr_write, width, height, bit_depth,
                 color_type, interlace_method, compression_method, filter_method);
    
    // 13. Set sCAL (fixed point) - using arbitrary values
    png_set_sCAL_fixed(png_ptr_write, info_ptr_write, fuzz_int32_t_5,
                       png_fixed_point(1000), png_fixed_point(1000));
    
    // 14. Demonstrate png_save_uint_16
    png_byte saved_uint[2];
    png_save_uint_16(saved_uint, fuzz_uint32_t_6);
    
    // 15. Set alpha mode
    png_set_alpha_mode_fixed(png_ptr_write, fuzz_int32_t_7, png_fixed_point(1000));
    
    // 16. Try to get mDCV data (may not be present)
    png_fixed_point int_white_x, int_white_y, int_red_x, int_red_y;
    png_fixed_point int_green_x, int_green_y, int_blue_x, int_blue_y;
    png_uint_32 max_lum, min_lum;
    png_uint_32 mdcv_result = png_get_mDCV_fixed(png_ptr_read, info_ptr_read,
        &int_white_x, &int_white_y, &int_red_x, &int_red_y,
        &int_green_x, &int_green_y, &int_blue_x, &int_blue_y,
        &max_lum, &min_lum);
    
    // 17. Set shift (using arbitrary values)
    png_color_8 true_bits;
    true_bits.red = 8;
    true_bits.green = 8;
    true_bits.blue = 8;
    true_bits.gray = 8;
    true_bits.alpha = 8;
    png_set_shift(png_ptr_write, &true_bits);
	FUZZ_FILENAME_PUSH("output_file")
    
    // 18. Set unknown chunk location (if any unknown chunks exist)
    // This is a no-op since we don't have unknown chunks
    
    // 19. Create output file
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 20. Initialize I/O for writing
    png_init_io(png_ptr_write, out_file);
    
    // 21. Write PNG info
    png_write_info(png_ptr_write, info_ptr_write);
    
    // 22. Allocate row buffer using png_malloc
    size_t rowbytes = png_get_rowbytes(png_ptr_read, info_ptr_read);
    row_buffer = static_cast<png_bytep>(png_malloc(png_ptr_read, rowbytes));
    
    // 23. Read and write image rows
    for (png_uint_32 y = 0; y < height; ++y) {
        png_read_row(png_ptr_read, row_buffer, nullptr);
        png_write_row(png_ptr_write, row_buffer);
    }
    
    // 24. Free row buffer
    png_free(png_ptr_read, row_buffer);
    row_buffer = nullptr;
    
    // 25. Read end of input PNG
    png_read_end(png_ptr_read, end_info_read);
    
    // 26. Write end of output PNG
    png_write_end(png_ptr_write, info_ptr_write);
    
    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, &end_info_read);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}