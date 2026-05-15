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
//<ID> 361
//<Prompt> []
/*<Combination>: [
*/
//<score> 84, nr_unique_branch: 2
//<Quality> {"density":28,"unique_branches":{"png_malloc":[[178,8,178,23,0,0,4,0]],"png_calloc":[[56,8,56,19,0,0,4,1]]},"library_calls":["png_sig_cmp","png_calloc","png_image_begin_read_from_memory","png_calloc","png_image_finish_read","png_calloc","png_image_write_to_memory","png_free","png_free","png_image_free","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_set_read_fn","png_get_io_ptr","png_get_io_ptr","png_set_sig_bytes","png_read_info","png_get_palette_max","png_reset_zstream","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_write_fn","png_get_io_ptr","png_get_io_ptr","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_IHDR","png_write_info","png_calloc","png_get_rowbytes","png_read_row","png_write_row","png_free","png_write_end","png_destroy_info_struct","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_calloc","png_image_begin_read_from_memory","png_calloc","png_image_finish_read","png_calloc","png_image_write_to_memory","png_free","png_free","png_image_free","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_read_fn","png_get_io_ptr","png_get_io_ptr","png_set_sig_bytes","png_read_info","png_get_palette_max","png_reset_zstream","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_write_fn","png_get_io_ptr","png_get_io_ptr","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_IHDR","png_write_info","png_calloc","png_get_rowbytes","png_read_row","png_write_row","png_free","png_write_end","png_destroy_info_struct","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/*Looking at the provided libpng APIs, I can create a fuzzer that tests PNG image reading and writing functionality. Here's a comprehensive fuzzer that uses all the specified APIs:

*/


// Include libpng headers

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<56) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    if (size < 8) {
        return 0; // Too small for PNG signature
    }

    // 1. Check if data has PNG signature
    if (png_sig_cmp(data, fuzz_uint64_t_1, size > 8 ? 8 : size) != 0) {
        return 0; // Not a PNG file
    }

    // Variables to track resource cleanup
    png_struct *png_ptr = nullptr;
    png_info *info_ptr = nullptr;
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    png_image *image = nullptr;
    void *buffer = nullptr;
    png_alloc_size_t memory_bytes = 0;
    
    // Try to use png_image API first
    image = (png_image *)png_calloc(nullptr, sizeof(png_image));
    if (image) {
        memset(image, 0, sizeof(png_image));
        image->version = PNG_IMAGE_VERSION;
        
        // Begin reading image from memory
        if (png_image_begin_read_from_memory(image, data, size)) {
            // Allocate buffer for image data
            buffer = png_calloc(nullptr, PNG_IMAGE_SIZE(*image));
            if (buffer) {
                // Finish reading the image
                png_color background = {0, 0, 0};
                if (png_image_finish_read(image, &background, buffer, fuzz_int32_t_2, nullptr)) {
                    // Try to write image to memory
                    void *output_memory = png_calloc(nullptr, PNG_IMAGE_SIZE(*image) * 2);
                    if (output_memory) {
                        png_image_write_to_memory(image, output_memory, &memory_bytes, 
                                                 fuzz_int32_t_3, buffer, fuzz_int32_t_4, nullptr);
                        png_free(nullptr, output_memory);
                    }
                }
                png_free(nullptr, buffer);
            }
        }
        png_image_free(image);
    }

    // Now test the traditional PNG reading/writing API
    // Create read structure
    png_ptr = png_create_read_struct(fuzz_str_5, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    // Create info structure
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Create in-memory file for reading
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }

    // Set up read function
    png_set_read_fn(png_ptr, in_file, [](png_structp png_ptr, png_bytep data, png_size_t length) {
        FILE *fp = (FILE *)png_get_io_ptr(png_ptr);
        if (fp) {
            fread(data, 1, length, fp);
        }
    });

    // Set signature bytes
    png_set_sig_bytes(png_ptr, 8);

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // Try to get palette max if available
    int palette_max = png_get_palette_max(png_ptr, info_ptr);

    // Try to reset zstream (may fail if not needed)
    png_reset_zstream(png_ptr);
	FUZZ_FILENAME_PUSH("output_file")

    // Create output file for writing
    out_file = fopen("output_file", "wb");
    if (out_file) {
        // Create write structure
        png_struct *write_ptr = png_create_write_struct(fuzz_str_6, nullptr, nullptr, nullptr);
        if (write_ptr) {
            png_info *write_info = png_create_info_struct(write_ptr);
            if (write_info) {
                if (!setjmp(png_jmpbuf(write_ptr))) {
                    png_set_write_fn(write_ptr, out_file, [](png_structp png_ptr, png_bytep data, png_size_t length) {
                        FILE *fp = (FILE *)png_get_io_ptr(png_ptr);
                        if (fp) {
                            fwrite(data, 1, length, fp);
                        }
                    }, nullptr);

                    // Copy info from read to write
                    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
                    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
                    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
                    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

                    png_set_IHDR(write_ptr, write_info, width, height, bit_depth, color_type,
                                fuzz_int32_t_7, fuzz_int32_t_8, fuzz_int32_t_9);

                    png_write_info(write_ptr, write_info);

                    // Read and write image data
                    png_bytep row = (png_bytep)png_calloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
                    for (png_uint_32 y = 0; y < height; y++) {
                        png_read_row(png_ptr, row, nullptr);
                        png_write_row(write_ptr, row);
                    }
                    png_free(png_ptr, row);

                    png_write_end(write_ptr, nullptr);
                }
                png_destroy_info_struct(write_ptr, &write_info);
            }
            png_destroy_write_struct(&write_ptr, nullptr);
        }
        assert_file_closed(&out_file);;
        remove("output_file");
    }

    // Cleanup
    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}