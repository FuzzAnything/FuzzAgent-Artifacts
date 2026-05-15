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
//<ID> 882
//<Prompt> []
/*<Combination>: [
*/
//<score> 64, nr_unique_branch: 1
//<Quality> {"density":32,"unique_branches":{"png_set_crc_action":[[84,7,84,27,0,0,4,0]]},"library_calls":["png_sig_cmp","png_image_begin_read_from_memory","png_create_read_struct","png_image_free","png_create_info_struct","png_destroy_read_struct","png_image_free","png_set_longjmp_fn","png_destroy_read_struct","png_image_free","png_set_crc_action","png_set_read_fn","png_read_info","png_get_compression_buffer_size","png_get_progressive_ptr","png_set_bKGD","png_get_next_frame_x_offset","png_get_next_frame_width","png_calloc","png_destroy_read_struct","png_image_free","png_calloc","png_image_finish_read","png_free","png_free","png_destroy_read_struct","png_image_free","png_image_write_to_memory","png_malloc","png_image_write_to_memory","png_free","png_free","png_free","png_destroy_read_struct","png_image_free"],"critical_calls":["png_sig_cmp","png_image_begin_read_from_memory","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_crc_action","png_set_read_fn","png_read_info","png_get_compression_buffer_size","png_get_progressive_ptr","png_set_bKGD","png_get_next_frame_x_offset","png_get_next_frame_width","png_calloc","png_calloc","png_image_finish_read","png_image_write_to_memory","png_malloc","png_image_write_to_memory","png_free","png_free","png_free","png_destroy_read_struct","png_image_free"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to achieve reading a PNG image from memory, processing it with traditional API, and writing it back to memory:

*/


extern "C" {
}

// Custom read function for memory buffer
struct MemoryReaderState {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void  id_000046_ReadDataFromMemory(png_structp png_ptr, png_bytep data, size_t length) {
    MemoryReaderState* state = static_cast<MemoryReaderState*>(png_get_io_ptr(png_ptr));
    
    if (state->offset + length > state->size) {
        png_error(png_ptr, "Read beyond buffer");
    }
    
    memcpy(data, state->data + state->offset, length);
    state->offset += length;
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<52) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    if (size < 8) {
        return 0;
    }

    // 1. Check PNG signature
    if (png_sig_cmp(data, fuzz_uint64_t_1, 8) != 0) {
        return 0;
    }

    // 2. Begin reading image from memory using simplified API
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    if (png_image_begin_read_from_memory(&image, data, size) == 0) {
        return 0;
    }

    // 3. Create read structure for traditional API
    png_structp png_ptr = png_create_read_struct(fuzz_str_2, 
                                                  nullptr, nullptr, nullptr);
    if (!png_ptr) {
        png_image_free(&image);
        return 0;
    }

    // 4. Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        png_image_free(&image);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        return 0;
    }

    // 5. Set CRC action
    png_set_crc_action(png_ptr, fuzz_int32_t_3, fuzz_int32_t_4);

    // 6. Set up memory reader
    MemoryReaderState reader_state = {data, size, 0};
    png_set_read_fn(png_ptr, &reader_state,  id_000046_ReadDataFromMemory);
    
    // Skip the signature (already checked)
    reader_state.offset = 8;

    // 7. Read info
    png_read_info(png_ptr, info_ptr);

    // 8. Get compression buffer size
    size_t comp_buffer_size = png_get_compression_buffer_size(png_ptr);

    // 9. Get progressive pointer (even if not using progressive reading)
    void* prog_ptr = png_get_progressive_ptr(png_ptr);

    // 10. Set background color
    png_color_16 background;
    background.index = 0;
    background.red = 65535;
    background.green = 65535;
    background.blue = 65535;
    background.gray = 65535;
    png_set_bKGD(png_ptr, info_ptr, &background);

    // 11. Get next frame info (for APNG)
    png_uint_32 next_frame_x_offset = png_get_next_frame_x_offset(png_ptr, info_ptr);
    png_uint_32 next_frame_width = png_get_next_frame_width(png_ptr, info_ptr);

    // 12. Allocate buffer with png_calloc
    png_alloc_size_t alloc_size = 1024;
    void* calloc_buffer = png_calloc(png_ptr, fuzz_uint64_t_5);
    if (!calloc_buffer) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        return 0;
    }

    // 13. Finish reading with simplified API
    image.format = PNG_FORMAT_RGBA;
    png_bytep buffer = static_cast<png_bytep>(png_calloc(png_ptr, 
        PNG_IMAGE_SIZE(image)));
    
    png_color background_color;
    background_color.red = 255;
    background_color.green = 255;
    background_color.blue = 255;
    
    if (png_image_finish_read(&image, &background_color, buffer, 
                               PNG_IMAGE_ROW_STRIDE(image), nullptr) == 0) {
        png_free(png_ptr, buffer);
        png_free(png_ptr, calloc_buffer);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        return 0;
    }

    // 14. Write image back to memory
    png_alloc_size_t output_size = 0;
    png_bytep output_buffer = nullptr;
    
    // First call to get required size
    if (png_image_write_to_memory(&image, nullptr, &output_size, fuzz_int32_t_6, 
                                   buffer, PNG_IMAGE_ROW_STRIDE(image), 
                                   nullptr) == 0) {
        output_buffer = static_cast<png_bytep>(png_malloc(png_ptr, fuzz_uint64_t_7));
        
        // Second call to actually write
        png_image_write_to_memory(&image, output_buffer, &output_size, fuzz_int32_t_8,
                                   buffer, PNG_IMAGE_ROW_STRIDE(image), 
                                   nullptr);
    }

    // 15. Clean up
    if (output_buffer) {
        png_free(png_ptr, output_buffer);
    }
    png_free(png_ptr, buffer);
    png_free(png_ptr, calloc_buffer);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    png_image_free(&image);

    return 0;
}