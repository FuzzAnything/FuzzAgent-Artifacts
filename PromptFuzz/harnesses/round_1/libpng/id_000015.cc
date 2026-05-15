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
//<ID> 208
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":38,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_read_info","png_set_progressive_read_fn","png_process_data","png_get_oFFs","png_get_x_offset_microns","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_set_compression_level","png_get_IHDR","png_set_IHDR","png_get_PLTE","png_set_PLTE","png_get_tRNS","png_set_tRNS","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_write_end","png_free_data","png_free","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_set_progressive_read_fn","png_process_data","png_get_oFFs","png_get_x_offset_microns","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_compression_level","png_get_IHDR","png_set_IHDR","png_get_PLTE","png_set_PLTE","png_get_tRNS","png_set_tRNS","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_write_end","png_free_data","png_free","png_destroy_write_struct","png_destroy_read_struct"],"visited":1}
/*Looking at this task, I need to create a fuzzer that processes PNG data using the specified libpng APIs. The key is to use all the mentioned APIs while handling PNG data properly.

*/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<32) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    if (size < 50) {
        return 0;  // Too small to be meaningful PNG data
    }

    // Create read structure
    png_struct *png_ptr_read = png_create_read_struct(fuzz_str_1, 
                                                      nullptr, nullptr, nullptr);
    if (!png_ptr_read) {
        return 0;
    }

    // Create info structure for reading
    png_info *info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        return 0;
    }

    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr_read))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }

    // Create memory file from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize IO for reading
    png_init_io(png_ptr_read, in_file);
    
    // Read PNG info
    png_read_info(png_ptr_read, info_ptr_read);

    // Process the PNG data using png_process_data
    // First, we need to set up progressive reading
    png_set_progressive_read_fn(png_ptr_read, nullptr, nullptr, nullptr, nullptr);
    
    // Process the data in chunks
    size_t bytes_processed = 0;
    const size_t chunk_size = 4096;
    while (bytes_processed < size) {
        size_t to_process = (size - bytes_processed < chunk_size) ? 
                           size - bytes_processed : chunk_size;
        png_process_data(png_ptr_read, info_ptr_read, 
                        (png_byte *)(data + bytes_processed), to_process);
        bytes_processed += to_process;
    }

    // Get offset information from the PNG
    png_int_32 offset_x, offset_y;
    int unit_type;
    png_uint_32 offs_valid = png_get_oFFs(png_ptr_read, info_ptr_read, 
                                         &offset_x, &offset_y, &unit_type);
    
    // Get x offset in microns
    png_int_32 x_offset_microns = png_get_x_offset_microns(png_ptr_read, info_ptr_read);
    
    // Parse a 32-bit value from the input data using png_get_uint_32
    // Let's read from a safe position in the data
    if (size >= sizeof(png_uint_32)) {
        png_uint_32 value = png_get_uint_32(data);
        // Can use this value for something if needed
    }

    // Now let's create a write structure for output
    png_struct *png_ptr_write = png_create_write_struct(fuzz_str_2,
                                                       nullptr, nullptr, nullptr);
    if (!png_ptr_write) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create info structure for writing
    png_info *info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        assert_file_closed(&in_file);;
        png_destroy_write_struct(&png_ptr_write, nullptr);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }

    // Set error handling for writing
    if (setjmp(png_jmpbuf(png_ptr_write))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize IO for writing
    png_init_io(png_ptr_write, out_file);

    // Set compression level using png_set_compression_level
    // Use a level derived from input data (mod 10 to keep it valid)
    int compression_level = data[0] % 10;  // Valid levels are 0-9
    png_set_compression_level(png_ptr_write, compression_level);

    // Copy image information from read to write structures
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type, compression_method, filter_method;
    
    png_get_IHDR(png_ptr_read, info_ptr_read, &width, &height, &bit_depth,
                 &color_type, &interlace_type, &compression_method, &filter_method);
    
    png_set_IHDR(png_ptr_write, info_ptr_write, width, height, bit_depth,
                 color_type, interlace_type, compression_method, filter_method);

    // Copy other chunks if they exist
    // Copy palette if present
    png_color *palette;
    int num_palette;
    if (png_get_PLTE(png_ptr_read, info_ptr_read, &palette, &num_palette)) {
        png_set_PLTE(png_ptr_write, info_ptr_write, palette, num_palette);
    }

    // Copy transparency if present
    png_byte *trans_alpha;
    int num_trans;
    png_color_16 *trans_color;
    if (png_get_tRNS(png_ptr_read, info_ptr_read, &trans_alpha, &num_trans, &trans_color)) {
        png_set_tRNS(png_ptr_write, info_ptr_write, trans_alpha, num_trans, trans_color);
    }

    // Write the info
    png_write_info(png_ptr_write, info_ptr_write);

    // Allocate row buffers
    png_bytep row = (png_bytep)png_malloc(png_ptr_write, 
                                         png_get_rowbytes(png_ptr_write, info_ptr_write));
    
    // Read rows one by one and write them out
    for (png_uint_32 y = 0; y < height; y++) {
        png_read_row(png_ptr_read, row, nullptr);
        png_write_row(png_ptr_write, row);
    }

    // Finish writing
    png_write_end(png_ptr_write, info_ptr_write);

    // Free data using png_free_data on the read structure
    // Free all data except for the basic info
    png_free_data(png_ptr_read, info_ptr_read, fuzz_uint32_t_3, fuzz_int32_t_4);

    // Cleanup
    png_free(png_ptr_write, row);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Destroy structures
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}