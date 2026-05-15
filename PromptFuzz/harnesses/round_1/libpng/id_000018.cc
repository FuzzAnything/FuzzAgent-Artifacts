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
//<ID> 234
//<Prompt> []
/*<Combination>: [
*/
//<score> 110, nr_unique_branch: 60
//<Quality> {"density":22,"unique_branches":{"png_handle_hIST":[[2061,8,2061,39,0,0,4,1]],"png_set_keep_unknown_chunks":[[1694,8,1694,23,0,0,4,1],[1697,8,1697,16,0,0,4,1],[1697,20,1697,49,0,0,4,1],[1704,8,1704,26,0,0,4,1],[1713,8,1713,25,0,0,4,1],[1751,11,1751,29,0,0,4,1],[1765,8,1765,35,0,0,4,0],[1770,8,1770,48,0,0,4,1],[1781,8,1781,17,0,0,4,0],[1786,11,1786,29,0,0,4,1],[1801,8,1801,24,0,0,4,0],[1807,17,1807,29,0,0,4,0],[1807,17,1807,29,0,0,4,1],[1815,42,1815,58,0,0,4,0],[1815,42,1815,58,0,0,4,1],[1817,14,1817,23,0,0,4,0],[1819,17,1819,34,0,0,4,1],[1827,11,1827,26,0,0,4,1],[1841,8,1841,39,0,0,4,0],[1843,11,1843,38,0,0,4,1]],"check_location":[[1487,8,1487,21,0,0,4,0],[1487,8,1487,21,0,0,4,1],[1487,25,1487,66,0,0,4,1],[1500,8,1500,21,0,0,4,0],[1500,8,1500,21,0,0,4,1],[1506,11,1506,45,0,0,4,0],[1506,11,1506,45,0,0,4,1]],"png_read_info":[[152,16,152,77,0,0,4,0],[156,14,156,36,0,0,4,1],[159,19,159,41,0,0,4,1]],"png_free_data":[[604,8,604,40,0,0,4,0],[605,8,605,57,0,0,4,0],[605,8,605,57,0,0,4,1],[607,11,607,20,0,0,4,1],[617,22,617,54,0,0,4,0],[617,22,617,54,0,0,4,1]],"png_handle_unknown":[[3030,11,3030,46,0,0,4,1],[3033,11,3033,42,0,0,4,0],[3037,14,3037,59,0,0,4,1],[3074,10,3074,16,0,0,4,1],[3078,10,3078,16,0,0,4,1],[3084,10,3084,17,0,0,4,0],[3087,10,3087,16,0,0,4,1]],"png_set_unknown_chunks":[[1521,8,1521,23,0,0,4,1],[1521,27,1521,43,0,0,4,1],[1521,47,1521,64,0,0,4,1],[1522,8,1522,24,0,0,4,1],[1559,8,1559,18,0,0,4,1],[1576,11,1576,27,0,0,4,0],[1576,11,1576,27,0,0,4,1],[1582,11,1582,30,0,0,4,0],[1582,11,1582,30,0,0,4,1],[1593,14,1593,30,0,0,4,1]],"add_one_chunk":[[1667,14,1667,21,0,0,4,1],[1677,8,1677,43,0,0,4,0]],"png_handle_as_unknown":[[908,49,908,77,0,0,4,1],[922,11,922,40,0,0,4,0],[922,11,922,40,0,0,4,1],[925,11,925,20,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_mem_fn","png_set_keep_unknown_chunks","png_set_unknown_chunks","png_read_info","png_get_acTL","png_reset_zstream","png_read_frame_head","png_get_next_frame_fcTL","png_read_update_info","png_get_rowbytes","png_get_image_height","png_malloc","png_malloc","png_read_image","png_free","png_free","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_mem_fn","png_set_keep_unknown_chunks","png_set_unknown_chunks","png_read_info","png_get_acTL","png_reset_zstream","png_read_frame_head","png_get_next_frame_fcTL","png_read_update_info","png_get_rowbytes","png_get_image_height","png_malloc","png_malloc","png_read_image","png_free","png_free","png_destroy_read_struct"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to process animated PNG (APNG) frames:

*/


// Include necessary headers (not provided but needed)

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for PNG header

    // Create FILE* for reading input
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create PNG read structure
    png_struct *png_ptr = png_create_read_struct(fuzz_str_1, 
                                                 nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Create PNG info structure
    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Setup error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, in_file);

    // 1. Set custom memory functions
    png_set_mem_fn(png_ptr, nullptr, 
                   [](png_struct *p, png_alloc_size_t s) { return malloc(s); },
                   [](png_struct *p, void *ptr) { free(ptr); });

    // 2. Set compression memory level (though API not in list, we'll use similar pattern)
    // png_set_compression_mem_level(png_ptr, 8); // API not provided

    // 3. Set unknown chunks handling
    png_unknown_chunk unknown_chunks[1];
    memset(unknown_chunks, 0, sizeof(unknown_chunks));
    memcpy(unknown_chunks[0].name, "teSt", 5); // Example chunk name
    unknown_chunks[0].data = (png_byte*)"test";
    unknown_chunks[0].size = 4;
    unknown_chunks[0].location = PNG_HAVE_IHDR;
    
    png_set_keep_unknown_chunks(png_ptr, fuzz_int32_t_2, 
                                (png_byte*)"teSt", 1);
    png_set_unknown_chunks(png_ptr, info_ptr, unknown_chunks, 1);

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // Check if it's an animated PNG
    png_uint_32 num_frames = 0, num_plays = 0;
    png_uint_32 has_acTL = png_get_acTL(png_ptr, info_ptr, &num_frames, &num_plays);

    if (has_acTL && num_frames > 1) {
        // Process frames
        for (png_uint_32 frame = 0; frame < num_frames; frame++) {
            // 4. Reset zstream before reading frame
            png_reset_zstream(png_ptr);

            // 5. Read frame header
            png_read_frame_head(png_ptr, info_ptr);

            // 6. Get frame control data
            png_uint_32 width = 0, height = 0, x_offset = 0, y_offset = 0;
            png_uint_16 delay_num = 0, delay_den = 0;
            png_byte dispose_op = 0, blend_op = 0;
            
            png_uint_32 result = png_get_next_frame_fcTL(png_ptr, info_ptr,
                &width, &height, &x_offset, &y_offset,
                &delay_num, &delay_den, &dispose_op, &blend_op);

            // Use the results to prevent optimization
            if (result) {
                // Do something with the frame data
                volatile png_uint_32 dummy = width + height + x_offset + y_offset;
                (void)dummy;
            }

            // Skip frame data to move to next frame
            // Note: In real usage, we'd read the image data here
            png_read_update_info(png_ptr, info_ptr);
            
            // Allocate row pointers and read rows
            png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
            png_uint_32 img_height = png_get_image_height(png_ptr, info_ptr);
            
            png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr,
                img_height * sizeof(png_bytep));
            
            for (png_uint_32 row = 0; row < img_height; row++) {
                row_pointers[row] = (png_byte*)png_malloc(png_ptr, rowbytes);
            }
            
            png_read_image(png_ptr, row_pointers);
            
            // Clean up row pointers
            for (png_uint_32 row = 0; row < img_height; row++) {
                png_free(png_ptr, row_pointers[row]);
            }
            png_free(png_ptr, row_pointers);
            
            // Check if we've reached the end of input
            if (feof(in_file)) {
                break;
            }
        }
    }

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}