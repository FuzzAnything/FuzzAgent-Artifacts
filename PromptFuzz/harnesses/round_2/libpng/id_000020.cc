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
//<ID> 333
//<Prompt> []
/*<Combination>: [
*/
//<score> 120, nr_unique_branch: 3
//<Quality> {"density":30,"unique_branches":{"png_set_swap":[[41,8,41,32,0,0,4,1]],"png_set_read_user_chunk_fn":[[1858,8,1858,23,0,0,4,1]],"png_set_expand_16":[[1003,8,1003,37,0,0,4,1]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_set_expand_16","png_set_swap","png_set_read_user_chunk_fn","png_read_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_interlace_handling","png_read_update_info","png_get_rowbytes","png_malloc","png_set_progressive_read_fn","png_process_data","png_free","png_read_end","png_image_begin_read_from_stdio","png_malloc","png_image_finish_read","png_free","png_image_free","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_set_expand_16","png_set_swap","png_set_read_user_chunk_fn","png_read_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_interlace_handling","png_read_update_info","png_get_rowbytes","png_malloc","png_set_progressive_read_fn","png_process_data","png_free","png_read_end","png_image_begin_read_from_stdio","png_malloc","png_image_finish_read","png_free","png_image_free","png_destroy_read_struct"],"visited":0}
/**/


// Callback structure for progressive reading
struct FuzzProgressiveData {
    png_bytep row_buffer;
    png_uint_32 rowbytes;
    png_uint_32 pass;
};

// Progressive read callbacks
void  id_000020_info_callback(png_structp png_ptr, png_infop info_ptr) {
    // Initialize for interlaced handling
    int num_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);
}

void  id_000020_row_callback(png_structp png_ptr, png_bytep new_row, 
                  png_uint_32 row_num, int pass) {
    FuzzProgressiveData* data = (FuzzProgressiveData*)png_get_progressive_ptr(png_ptr);
    
    if (data && data->row_buffer) {
        // For progressive/interlaced images, combine rows
        if (row_num == 0 && pass == 0) {
            // First row of first pass
            memcpy(data->row_buffer, new_row, data->rowbytes);
        } else {
            // Combine with previous row data
            png_progressive_combine_row(png_ptr, data->row_buffer, new_row);
        }
    }
}

void  id_000020_end_callback(png_structp png_ptr, png_infop info_ptr) {
    // Reading complete
}

// User chunk callback to use png_get_int_32
int  id_000020_chunk_callback(png_structp png_ptr, png_unknown_chunkp chunk) {
    // Use png_get_int_32 on chunk data if it's at least 4 bytes
    if (chunk->size >= 4) {
        png_int_32 value = png_get_int_32(chunk->data);
        (void)value; // Prevent unused variable warning
    }
    return 0; // Keep the chunk
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	 // Too small for PNG header
    
    // Check PNG signature
    if (png_sig_cmp((png_bytep)data, 0, 8) != 0) {
        return 0; // Not a PNG file
    }
    
    FILE* in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Traditional API: Progressive reading with transformations
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    png_infop end_info = nullptr;
    
    // Create read structures
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    end_info = png_create_info_struct(png_ptr);
    
    if (!info_ptr || !end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up file reading
    png_init_io(png_ptr, in_file);
    png_set_sig_bytes(png_ptr, 8);
    
    // Apply transformations
    png_set_expand_16(png_ptr);    // Expand 16-bit samples
    png_set_swap(png_ptr);         // Swap byte order for 16-bit
    png_set_read_user_chunk_fn(png_ptr, nullptr,  id_000020_chunk_callback); // For chunk reading
    
    // Read image info
    png_read_info(png_ptr, info_ptr);
    
    // Get image dimensions
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Handle interlacing
    int num_passes = png_set_interlace_handling(png_ptr);
    (void)num_passes; // Mark as used
    
    // Update transformations
    png_read_update_info(png_ptr, info_ptr);
    
    // Prepare for progressive reading
    png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    FuzzProgressiveData prog_data;
    prog_data.row_buffer = (png_bytep)png_malloc(png_ptr, rowbytes);
    prog_data.rowbytes = rowbytes;
    prog_data.pass = 0;
    
    // Set up progressive reading
    png_set_progressive_read_fn(png_ptr, &prog_data,  id_000020_info_callback, 
                                id_000020_row_callback,  id_000020_end_callback);
    
    // Process the image in chunks
    size_t processed = 8; // Skip signature
    const size_t chunk_size = 4096;
    
    while (processed < size) {
        size_t to_process = chunk_size;
        if (processed + to_process > size) {
            to_process = size - processed;
        }
        
        png_process_data(png_ptr, info_ptr, 
                        (png_bytep)(data + processed), to_process);
        processed += to_process;
    }
    
    // Clean up traditional API
    if (prog_data.row_buffer) {
        png_free(png_ptr, prog_data.row_buffer);
    }
    
    // Read the rest of the image
    png_read_end(png_ptr, end_info);
    
    // Simplified API: Read the same image using png_image
    rewind(in_file); // Reset file pointer
    
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    if (png_image_begin_read_from_stdio(&image, in_file)) {
        // Allocate buffer
        png_bytep buffer = (png_bytep)png_malloc(png_ptr, 
            PNG_IMAGE_SIZE(image));
        
        if (buffer) {
            // Use png_image_finish_read
            png_image_finish_read(&image, nullptr, buffer, 0, nullptr);
            
            // Free the buffer
            png_free(png_ptr, buffer);
        }
        
        png_image_free(&image);
    }
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}