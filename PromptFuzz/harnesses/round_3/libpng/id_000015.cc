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
//<ID> 144
//<Prompt> []
/*<Combination>: [
*/
//<score> 33, nr_unique_branch: 4
//<Quality> {"density":33,"unique_branches":{"image_memory_write":[[2262,14,2262,46,0,0,4,0]],"png_image_write_to_memory":[[2309,14,2309,28,0,0,4,0],[2336,20,2336,34,0,0,4,1],[2336,38,2336,74,0,0,4,1]]},"library_calls":["png_sig_cmp","png_image_begin_read_from_memory","png_image_free","png_create_read_struct","png_image_free","png_create_info_struct","png_destroy_read_struct","png_image_free","png_set_crc_action","png_get_progressive_ptr","png_set_read_fn","png_read_info","png_get_next_frame_x_offset","png_calloc","png_destroy_read_struct","png_image_free","png_image_finish_read","png_free","png_destroy_read_struct","png_image_free","png_image_write_to_memory","png_free","png_destroy_read_struct","png_image_free","png_calloc","png_free","png_destroy_read_struct","png_image_free","png_image_write_to_memory","png_free","png_free","png_destroy_read_struct","png_image_free"],"critical_calls":["png_sig_cmp","png_image_begin_read_from_memory","png_create_read_struct","png_create_info_struct","png_set_crc_action","png_get_progressive_ptr","png_set_read_fn","png_read_info","png_get_next_frame_x_offset","png_calloc","png_image_finish_read","png_image_write_to_memory","png_calloc","png_image_write_to_memory","png_free","png_free","png_destroy_read_struct","png_image_free"],"visited":2}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to read a PNG image from memory, process it, and write it back to memory:

*/


// Custom read function for memory-based I/O
struct MemoryReaderState {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void  id_000015_read_memory_data(png_structp png_ptr, png_bytep out, png_size_t length) {
    MemoryReaderState* state = static_cast<MemoryReaderState*>(png_get_io_ptr(png_ptr));
    
    if (state->offset + length > state->size) {
        png_error(png_ptr, "Read beyond buffer boundary");
        return;
    }
    
    memcpy(out, state->data + state->offset, length);
    state->offset += length;
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    if (size < 8) {
        return 0;
    }
    
    // Check if the data starts with a PNG signature
    if (png_sig_cmp(data, fuzz_uint64_t_1, 8) != 0) {
        return 0;
    }
    
    // Initialize structures
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    MemoryReaderState reader_state = {data, size, 0};
    void* buffer = nullptr;
    png_alloc_size_t memory_bytes = 0;
    int result = 0;
    
    // 1. Begin reading PNG image from memory using high-level API
    result = png_image_begin_read_from_memory(&image, data, size);
    if (result == 0) {
        // Failed to read image, clean up and exit
        png_image_free(&image);
        return 0;
    }
    
    // 2. Allocate memory for image data using png_calloc
    // Note: We need to create a png_struct for allocation
    png_ptr = png_create_read_struct(fuzz_str_2, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        png_image_free(&image);
        return 0;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        png_image_free(&image);
        return 0;
    }
    
    // 3. Set CRC action
    png_set_crc_action(png_ptr, fuzz_int32_t_3, fuzz_int32_t_4);
    
    // 4. Get progressive pointer (set up for progressive reading)
    void* progressive_ptr = png_get_progressive_ptr(png_ptr);
    // We don't use progressive reading in this example, but we have the pointer
    
    // 5. Set up custom read function for low-level reading
    png_set_read_fn(png_ptr, &reader_state,  id_000015_read_memory_data);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // 6. Get next frame X offset (for APNG images)
    png_uint_32 x_offset = png_get_next_frame_x_offset(png_ptr, info_ptr);
    // This will be 0 for non-APNG images
    
    // Allocate buffer for final image output
    buffer = png_calloc(png_ptr, PNG_IMAGE_SIZE(image));
    if (!buffer) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        return 0;
    }
    
    // Finish reading the image using high-level API
    result = png_image_finish_read(&image, nullptr, buffer, 0, nullptr);
    if (result == 0) {
        png_free(png_ptr, buffer);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        return 0;
    }
    
    // 7. Write image to memory using high-level API
    // First, allocate memory for the output
    png_alloc_size_t required_bytes = 0;
    
    // Calculate required buffer size
    result = png_image_write_to_memory(&image, nullptr, &required_bytes, 
                                       fuzz_int32_t_5, buffer, 0, nullptr);
    
    if (result == 0) {
        png_free(png_ptr, buffer);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        return 0;
    }
    
    // Allocate output buffer
    void* output_buffer = png_calloc(png_ptr, required_bytes);
    if (!output_buffer) {
        png_free(png_ptr, buffer);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        return 0;
    }
    
    // Actually write the image
    memory_bytes = required_bytes;
    result = png_image_write_to_memory(&image, output_buffer, &memory_bytes, 
                                       fuzz_int32_t_6, buffer, 0, nullptr);
    
    // Clean up all allocated resources
    if (output_buffer) {
        png_free(png_ptr, output_buffer);
    }
    
    if (buffer) {
        png_free(png_ptr, buffer);
    }
    
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
    
    png_image_free(&image);
    
    return 0;
}