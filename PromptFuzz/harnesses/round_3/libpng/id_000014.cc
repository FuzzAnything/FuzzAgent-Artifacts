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
//<ID> 143
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 1
//<Quality> {"density":26,"unique_branches":{"png_set_crc_action":[[66,7,66,28,0,0,4,0]]},"library_calls":["png_sig_cmp","png_image_begin_read_from_memory","png_image_free","png_create_read_struct","png_image_free","png_create_info_struct","png_destroy_read_struct","png_image_free","png_set_longjmp_fn","png_destroy_read_struct","png_image_free","png_set_crc_action","png_set_read_fn","png_read_info","png_get_acTL","png_get_next_frame_x_offset","png_get_progressive_ptr","png_set_progressive_read_fn","png_process_data","png_get_image_height","png_image_write_to_memory","png_calloc","png_image_write_to_memory","png_free","png_process_data_skip","png_destroy_read_struct","png_image_free"],"critical_calls":["png_sig_cmp","png_image_begin_read_from_memory","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_crc_action","png_set_read_fn","png_read_info","png_get_acTL","png_get_next_frame_x_offset","png_get_progressive_ptr","png_set_progressive_read_fn","png_process_data","png_get_image_height","png_image_write_to_memory","png_calloc","png_image_write_to_memory","png_free","png_process_data_skip","png_destroy_read_struct","png_image_free"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to achieve progressive PNG image reading from memory:

*/


// Custom memory reader callback for progressive reading
struct mem_read_data {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

void  id_000014_mem_read_fn(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead) {
    mem_read_data* mem = (mem_read_data*)png_get_io_ptr(png_ptr);
    
    if (mem->offset + byteCountToRead > mem->size) {
        // Not enough data, signal error
        png_error(png_ptr, "Read error: insufficient data");
    }
    
    memcpy(outBytes, mem->data + mem->offset, byteCountToRead);
    mem->offset += byteCountToRead;
}

// Progressive row callback
void  id_000014_row_callback(png_structp png_ptr, png_bytep new_row, png_uint_32 row_num, int pass) {
    // Do nothing for now, just consume the row data
}

// Progressive end callback  
void  id_000014_end_callback(png_structp png_ptr, png_infop info_ptr) {
    // End of progressive reading
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<52) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	  // Need at least PNG signature
    
    // Check if data has PNG signature
    if (png_sig_cmp((png_bytep)data, fuzz_uint64_t_1, 8) != 0) {
        return 0;  // Not a PNG file
    }
    
    // Initialize png_image structure for high-level API
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    // 1. Use png_image_begin_read_from_memory
    int begin_result = png_image_begin_read_from_memory(&image, data, size);
    if (begin_result == 0) {
        // Failed to begin reading, clean up and return
        png_image_free(&image);
        return 0;
    }
    
    // Create PNG structures for low-level API
    png_structp png_ptr = png_create_read_struct(fuzz_str_2, NULL, NULL, NULL);
    if (!png_ptr) {
        png_image_free(&image);
        return 0;
    }
    
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
    
    // 2. Use png_set_crc_action to control CRC handling
    png_set_crc_action(png_ptr, fuzz_int32_t_3, fuzz_int32_t_4);
    
    // Set up memory reader for progressive reading
    mem_read_data mem_data = {data, size, 0};
    png_set_read_fn(png_ptr, &mem_data,  id_000014_mem_read_fn);
    
    // Read PNG header
    png_read_info(png_ptr, info_ptr);
    
    // 3. Use png_get_next_frame_x_offset for APNG support
    png_uint_32 x_offset = 0;
    // Check if this is an APNG
    png_uint_32 num_frames = 0;
    png_uint_32 num_plays = 0;
    if (png_get_acTL(png_ptr, info_ptr, &num_frames, &num_plays)) {
        if (num_frames > 1) {
            // Get x offset for next frame
            x_offset = png_get_next_frame_x_offset(png_ptr, info_ptr);
        }
    }
    
    // 4. Use png_get_progressive_ptr
    void* progressive_ptr = png_get_progressive_ptr(png_ptr);
    // We can use this pointer to pass user data if needed
    
    // Set up progressive callbacks
    png_set_progressive_read_fn(png_ptr, NULL, NULL,  id_000014_row_callback,  id_000014_end_callback);
    
    // Process PNG data in chunks (simulate progressive loading)
    size_t chunk_size = 4096;
    size_t processed = 0;
    
    while (processed < size) {
        size_t to_process = (size - processed) > chunk_size ? chunk_size : (size - processed);
        png_process_data(png_ptr, info_ptr, (png_bytep)(data + processed), to_process);
        processed += to_process;
        
        // Break early if we've processed the entire image
        if (png_get_image_height(png_ptr, info_ptr) > 0) {
            // Check if all rows have been processed
            break;
        }
    }
    
    // 5. Prepare for writing the image back to memory
    // Allocate buffer for output
    png_alloc_size_t output_size = 0;
    
    // First, get required buffer size
    if (png_image_write_to_memory(&image, NULL, &output_size, fuzz_int32_t_5, NULL, fuzz_int32_t_6, NULL)) {
        // 6. Use png_calloc to allocate memory for output
        void* output_buffer = png_calloc(png_ptr, fuzz_uint64_t_7);
        
        if (output_buffer) {
            // Actually write the image to memory
            png_image_write_to_memory(&image, output_buffer, &output_size, fuzz_int32_t_8, NULL, fuzz_int32_t_9, NULL);
            
            // Clean up allocated buffer
            png_free(png_ptr, output_buffer);
        }
    }
    
    // 7. Use png_process_data_skip to skip remaining data if needed
    png_process_data_skip(png_ptr);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    png_image_free(&image);
    
    return 0;
}