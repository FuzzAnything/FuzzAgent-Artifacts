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
//<ID> 152
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 1
//<Quality> {"density":25,"unique_branches":{"png_get_user_width_max":[[1307,12,1307,19,0,0,4,0]]},"library_calls":["png_sig_cmp","png_image_begin_read_from_memory","png_create_read_struct","png_image_free","png_create_info_struct","png_destroy_read_struct","png_image_free","png_set_longjmp_fn","png_destroy_read_struct","png_image_free","png_get_user_width_max","png_set_read_fn","png_get_io_ptr","png_get_io_ptr","png_set_sig_bytes","png_read_info","png_set_tRNS","png_set_scale_16","png_set_cLLI","png_set_crc_action","png_read_update_info","png_malloc","png_get_rowbytes","png_read_image","png_read_end","png_free","png_destroy_read_struct","png_image_free"],"critical_calls":["png_sig_cmp","png_image_begin_read_from_memory","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_get_user_width_max","png_set_read_fn","png_get_io_ptr","png_get_io_ptr","png_set_sig_bytes","png_read_info","png_set_tRNS","png_set_scale_16","png_set_cLLI","png_set_crc_action","png_read_update_info","png_malloc","png_get_rowbytes","png_read_image","png_read_end","png_free","png_destroy_read_struct","png_image_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

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
	
		const png_byte * fuzzer_var_png_set_tRNS_16_2 = fuzz_char_array_4;
		int fuzzer_size_png_set_tRNS_16_2 = static_cast<int>(fuzz_char_array_size_4);//fuzzer shim end}




    // Early exit if input is too small to be a valid PNG
    if (size < 8 || png_sig_cmp(data, fuzz_uint64_t_1, 8) != 0) {
        return 0;
    }

    // Initialize structures
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    // Step 1: Begin reading PNG from memory
    if (!png_image_begin_read_from_memory(&image, data, size)) {
        return 0;
    }

    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(fuzz_str_2, 
                                                  NULL, NULL, NULL);
    if (!png_ptr) {
        png_image_free(&image);
        return 0;
    }

    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        png_image_free(&image);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        png_image_free(&image);
        return 0;
    }

    // Step 2: Get user width max
    png_uint_32 width_max = png_get_user_width_max(png_ptr);

    // Set up memory reading
    png_set_read_fn(png_ptr, (png_voidp)&data, 
                    [](png_structp png_ptr, png_bytep out, 
                       png_size_t length) {
        png_voidp io_ptr = png_get_io_ptr(png_ptr);
        const uint8_t** data_ptr = (const uint8_t**)io_ptr;
        memcpy(out, *data_ptr, length);
        *data_ptr += length;
    });

    // Read PNG info
    png_set_sig_bytes(png_ptr, fuzz_int32_t_3);
    png_read_info(png_ptr, info_ptr);

    // Step 3: Set tRNS if needed (dummy transparency for testing)
    png_byte trans_alpha[] = {255};
    png_color_16 trans_color;
    trans_color.index = 0;
    trans_color.red = 0;
    trans_color.green = 0;
    trans_color.blue = 0;
    trans_color.gray = 0;
    png_set_tRNS(png_ptr, info_ptr, fuzzer_var_png_set_tRNS_16_2, fuzzer_size_png_set_tRNS_16_2, &trans_color);

    // Step 4: Enable 16-bit to 8-bit scaling
    png_set_scale_16(png_ptr);

    // Step 5: Set Content Light Level Information
    png_set_cLLI(png_ptr, info_ptr, 1000.0, 400.0);

    // Step 6: Set CRC action
    png_set_crc_action(png_ptr, fuzz_int32_t_5, fuzz_int32_t_6);

    // Update info after transformations
    png_read_update_info(png_ptr, info_ptr);

    // Read image rows
    png_bytep row_pointers[image.height];
    for (png_uint_32 y = 0; y < image.height; y++) {
        row_pointers[y] = (png_byte*)png_malloc(png_ptr, 
                        png_get_rowbytes(png_ptr, info_ptr));
    }
    png_read_image(png_ptr, row_pointers);

    // Read end of PNG
    png_read_end(png_ptr, info_ptr);

    // Clean up allocated rows
    for (png_uint_32 y = 0; y < image.height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }

    // Clean up PNG structures
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    png_image_free(&image);

    return 0;
}