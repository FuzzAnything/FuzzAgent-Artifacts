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
//<ID> 231
//<Prompt> []
/*<Combination>: [
*/
//<score> 29, nr_unique_branch: 1
//<Quality> {"density":29,"unique_branches":{"png_set_compression_buffer_size":[[1899,8,1899,49,0,0,4,0]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_mem_fn","png_set_compression_buffer_size","png_set_read_fn","png_set_sig_bytes","png_read_info","png_set_unknown_chunks","png_set_keep_unknown_chunks","png_reset_zstream","png_get_valid","png_get_acTL","png_read_update_info","png_read_frame_head","png_get_next_frame_fcTL","png_get_image_height","png_get_image_width","png_get_color_type","png_get_bit_depth","png_get_rowbytes","png_malloc","png_malloc","png_read_image","png_read_end","png_free","png_free","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_mem_fn","png_set_compression_buffer_size","png_set_read_fn","png_set_sig_bytes","png_read_info","png_set_unknown_chunks","png_set_keep_unknown_chunks","png_reset_zstream","png_get_valid","png_get_acTL","png_read_update_info","png_read_frame_head","png_get_next_frame_fcTL","png_get_image_height","png_get_image_width","png_get_color_type","png_get_bit_depth","png_get_rowbytes","png_malloc","png_malloc","png_read_image","png_read_end","png_free","png_free","png_destroy_read_struct"],"visited":1}
/*To create a fuzz driver for libpng that uses the specified APIs, I'll implement a PNG frame reading function that processes animated PNG (APNG) frames. The fuzzer will read PNG data from memory, attempt to process it as an APNG, and extract frame control information.

Here's the complete implementation:

*/


// Custom memory functions for libpng
static void*  id_000017_png_custom_malloc(png_structp png_ptr, png_alloc_size_t size) {
    return malloc(size);
}

static void  id_000017_png_custom_free(png_structp png_ptr, void* ptr) {
    free(ptr);
}

// Custom read function for memory buffer
struct mem_buffer {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void  id_000017_user_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    mem_buffer* buffer = (mem_buffer*)png_get_io_ptr(png_ptr);
    
    if (buffer->offset + length > buffer->size) {
        png_error(png_ptr, "Read beyond buffer boundary");
        return;
    }
    
    memcpy(data, buffer->data + buffer->offset, length);
    buffer->offset += length;
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<51) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	 // Minimum PNG header size
    
    // Check PNG signature
    if (png_sig_cmp((png_const_bytep)data, fuzz_uint64_t_1, 8) != 0) {
        return 0; // Not a PNG file
    }
    
    // Create read structure
    png_structp png_ptr = png_create_read_struct(fuzz_str_2, 
                                                  nullptr, nullptr, nullptr);
    if (!png_ptr) {
	return 0;
	}
	
    
    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // 1. Set custom memory functions
    png_set_mem_fn(png_ptr, nullptr,  id_000017_png_custom_malloc,  id_000017_png_custom_free);
    
    // 2. Set compression memory level (though this API wasn't in the list,
    // we'll use a similar approach with available compression APIs)
    png_set_compression_buffer_size(png_ptr, fuzz_uint64_t_3); // Use available API
    
    // Set up custom read from memory buffer
    mem_buffer buffer = {data, size, 0};
    png_set_read_fn(png_ptr, &buffer,  id_000017_user_read_data);
    
    // Read PNG header
    png_set_sig_bytes(png_ptr, fuzz_int32_t_4);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // 3. Set unknown chunks (create dummy unknown chunk)
    png_unknown_chunk unknown_chunks[1];
    memset(&unknown_chunks[0], 0, sizeof(png_unknown_chunk));
    strncpy((char*)unknown_chunks[0].name, "tEXt", 5);
    unknown_chunks[0].data = (png_byte*)"Test";
    unknown_chunks[0].size = 4;
    unknown_chunks[0].location = PNG_HAVE_IHDR;
    
    png_set_unknown_chunks(png_ptr, info_ptr, unknown_chunks, 1);
    png_set_keep_unknown_chunks(png_ptr, fuzz_int32_t_6, 
                               fuzz_str_5, fuzz_int32_t_7);
    
    // 4. Reset zstream (important for reading multiple frames)
    png_reset_zstream(png_ptr);
    
    // Check if it might be an animated PNG by looking for acTL chunk
    png_uint_32 num_frames = 0;
    png_uint_32 num_plays = 0;
    
    if (png_get_valid(png_ptr, info_ptr, fuzz_uint32_t_8)) {
        png_get_acTL(png_ptr, info_ptr, &num_frames, &num_plays);
        
        // If we have multiple frames, try to read frame headers
        if (num_frames > 0) {
            // Update info after reading first frame
            png_read_update_info(png_ptr, info_ptr);
            
            // 5. Read frame head for next frame
            png_read_frame_head(png_ptr, info_ptr);
            
            // 6. Get next frame fcTL information
            png_uint_32 width = 0, height = 0;
            png_uint_32 x_offset = 0, y_offset = 0;
            png_uint_16 delay_num = 0, delay_den = 0;
            png_byte dispose_op = 0, blend_op = 0;
            
            png_uint_32 result = png_get_next_frame_fcTL(png_ptr, info_ptr, 
                                                        &width, &height,
                                                        &x_offset, &y_offset,
                                                        &delay_num, &delay_den,
                                                        &dispose_op, &blend_op);
            
            // If we got frame information, try to read the frame data
            if (result) {
                // Allocate row pointers for reading frame data
                png_uint_32 img_height = png_get_image_height(png_ptr, info_ptr);
                png_uint_32 img_width = png_get_image_width(png_ptr, info_ptr);
                int color_type = png_get_color_type(png_ptr, info_ptr);
                int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
                
                // Calculate row bytes
                size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
                
                // Allocate memory for row pointers
                png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr, 
                    img_height * sizeof(png_bytep));
                
                // Allocate memory for each row
                for (png_uint_32 y = 0; y < img_height; y++) {
                    row_pointers[y] = (png_bytep)png_malloc(png_ptr, rowbytes);
                }
                
                // Read image data
                png_read_image(png_ptr, row_pointers);
                
                // Read end of frame
                png_read_end(png_ptr, info_ptr);
                
                // Free allocated memory
                for (png_uint_32 y = 0; y < img_height; y++) {
                    png_free(png_ptr, row_pointers[y]);
                }
                png_free(png_ptr, row_pointers);
            }
        }
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}