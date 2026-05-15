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
//<ID> 524
//<Prompt> []
/*<Combination>: [
*/
//<score> 352, nr_unique_branch: 96
//<Quality> {"density":22,"unique_branches":{"png_init_palette_transformations":[[1170,8,1170,30,0,0,4,0],[1170,8,1170,30,0,0,4,1],[1175,17,1175,37,0,0,4,0],[1175,17,1175,37,0,0,4,1],[1177,14,1177,44,0,0,4,0],[1177,14,1177,44,0,0,4,1],[1179,19,1179,47,0,0,4,0],[1179,19,1179,47,0,0,4,1],[1191,8,1191,28,0,0,4,0],[1191,8,1191,28,0,0,4,1],[1200,11,1200,38,0,0,4,0],[1200,11,1200,38,0,0,4,1],[1213,8,1213,63,0,0,4,1]],"png_init_read_transformations":[[1498,8,1498,57,0,0,4,1],[1570,8,1570,53,0,0,4,0]],"png_app_warning":[[340,8,340,58,0,0,4,1]],"png_get_sPLT":[[750,8,750,23,0,0,4,0],[750,27,750,43,0,0,4,0],[750,47,750,64,0,0,4,0]],"png_init_rgb_transformations":[[1256,8,1256,28,0,0,4,1],[1267,11,1267,38,0,0,4,1]],"png_get_cLLI":[[813,8,813,23,0,0,4,0],[813,27,813,43,0,0,4,0],[814,8,814,46,0,0,4,0],[814,8,814,46,0,0,4,1],[816,11,816,25,0,0,4,0],[817,11,817,26,0,0,4,0]],"png_gamma_significant":[[2896,8,2896,56,0,0,4,0]],"png_image_begin_read_from_memory":[[1525,8,1525,21,0,0,4,0],[1525,25,1525,60,0,0,4,0],[1527,11,1527,25,0,0,4,0],[1527,29,1527,37,0,0,4,0],[1529,14,1529,45,0,0,4,0]],"png_image_memory_read":[[1495,8,1495,23,0,0,4,0],[1498,11,1498,24,0,0,4,0],[1501,14,1501,24,0,0,4,0],[1506,17,1506,31,0,0,4,0],[1506,35,1506,47,0,0,4,0]],"png_read_transform_info":[[2055,8,2055,37,0,0,4,0],[2057,11,2057,57,0,0,4,0],[2057,11,2057,57,0,0,4,1],[2058,11,2058,36,0,0,4,0],[2058,40,2058,64,0,0,4,0],[2071,8,2071,52,0,0,4,1],[2111,8,2111,53,0,0,4,1],[2128,8,2128,33,0,0,4,0],[2128,8,2128,33,0,0,4,1],[2132,14,2132,65,0,0,4,1],[2137,14,2137,59,0,0,4,1],[2167,8,2167,57,0,0,4,1],[2173,8,2173,57,0,0,4,1],[2179,8,2179,54,0,0,4,1],[2191,8,2191,55,0,0,4,1],[2200,8,2200,50,0,0,4,1],[2205,8,2205,54,0,0,4,0],[2205,8,2205,54,0,0,4,1],[2208,13,2208,63,0,0,4,0],[2208,13,2208,63,0,0,4,1],[2215,8,2215,57,0,0,4,1],[2223,8,2223,58,0,0,4,0],[2223,8,2223,58,0,0,4,1],[2228,8,2228,52,0,0,4,0],[2229,9,2229,51,0,0,4,0],[2229,9,2229,51,0,0,4,1],[2230,8,2230,51,0,0,4,0],[2230,8,2230,51,0,0,4,1],[2234,11,2234,58,0,0,4,1],[2241,8,2241,60,0,0,4,0],[2243,11,2243,45,0,0,4,1],[2246,11,2246,48,0,0,4,1],[534,6,534,23,35,0,4,0],[534,6,534,23,35,0,4,1]],"png_resolve_file_gamma":[[1350,8,1350,23,0,0,4,0]],"png_init_gamma_values":[[1382,8,1382,22,0,0,4,0],[1384,11,1384,27,0,0,4,1]],"png_check_IHDR":[[2077,11,2077,56,0,0,4,1]],"png_read_update_info":[[264,8,264,23,0,0,4,0],[266,11,266,52,0,0,4,0]],"png_read_start_row":[[4604,8,4604,32,0,0,4,0],[4606,11,4606,58,0,0,4,0],[4693,8,4693,54,0,0,4,0],[4695,11,4695,53,0,0,4,0],[4695,11,4695,53,0,0,4,1],[4697,14,4697,34,0,0,4,0],[4697,14,4697,34,0,0,4,1],[4704,16,4704,57,0,0,4,0],[4704,16,4704,57,0,0,4,1],[4705,10,4705,55,0,0,4,0],[4705,10,4705,55,0,0,4,1],[4707,14,4707,35,0,0,4,0],[4707,14,4707,35,0,0,4,1],[4758,8,4758,60,0,0,4,0],[4763,11,4763,45,0,0,4,1],[4794,11,4794,35,0,0,4,0],[4850,8,4850,36,0,0,4,0],[534,6,534,23,22,0,4,0]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_set_read_user_transform_fn","png_set_filler","png_read_info","png_get_sPLT","png_get_cLLI","png_read_update_info","png_process_data_skip","png_get_progressive_ptr","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_rowbytes","png_read_image","png_read_end","png_image_begin_read_from_memory","png_image_write_to_memory","png_image_write_to_memory","png_image_free","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_set_read_user_transform_fn","png_set_filler","png_read_info","png_get_sPLT","png_get_cLLI","png_read_update_info","png_process_data_skip","png_get_progressive_ptr","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_rowbytes","png_read_image","png_read_end","png_image_begin_read_from_memory","png_image_write_to_memory","png_image_write_to_memory","png_image_free","png_destroy_read_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to achieve the event of reading a PNG image from memory, processing it with transformations, and writing it to memory:

*/


// Include necessary libpng headers

// Callback function for user transform
static void  id_000018_user_transform_callback(png_structp png_ptr, png_row_infop row_info, png_bytep data) {
    // Simple transform: invert colors for demonstration
    for (png_uint_32 i = 0; i < row_info->rowbytes; i++) {
        data[i] = ~data[i];
    }
}

// Progressive read callback structure
struct progressive_read_data {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

// Progressive read callback function
static void  id_000018_progressive_read_callback(png_structp png_ptr, png_bytep out_bytes, png_size_t byte_count) {
    progressive_read_data* read_data = (progressive_read_data*)png_get_progressive_ptr(png_ptr);
    
    if (read_data->offset + byte_count > read_data->size) {
        png_error(png_ptr, "Not enough data for progressive read");
    }
    
    memcpy(out_bytes, read_data->data + read_data->offset, byte_count);
    read_data->offset += byte_count;
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<44) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for PNG operations
    
    // Initialize variables
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    png_infop end_info = nullptr;
    png_image image = {};
    void* write_buffer = nullptr;
    png_alloc_size_t write_buffer_size = 0;
    png_sPLT_t* splt_entries = nullptr;
    int num_splt_entries = 0;
    
    // Part 1: Normal PNG reading operations
    FILE* in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Check PNG signature
    png_byte sig[8];
    if (fread(sig, 1, 8, in_file) != 8) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    if (!png_sig_cmp(sig, fuzz_uint64_t_1, 8)) {
        // Create read structure
        png_ptr = png_create_read_struct(fuzz_str_2, nullptr, nullptr, nullptr);
        if (!png_ptr) {
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	return 0;
        }
        
        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, nullptr, nullptr);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	return 0;
        }
        
        end_info = png_create_info_struct(png_ptr);
        
        // Set up error handling
        if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	return 0;
        }
        
        // Initialize PNG reading
        png_init_io(png_ptr, in_file);
        png_set_sig_bytes(png_ptr, fuzz_int32_t_3);
        
        // Set up user transform
        png_set_read_user_transform_fn(png_ptr,  id_000018_user_transform_callback);
        
        // Set filler (add alpha channel if needed)
        png_set_filler(png_ptr, fuzz_uint32_t_4, fuzz_int32_t_5);
        
        // Read PNG info
        png_read_info(png_ptr, info_ptr);
        
        // Get sPLT chunks
        num_splt_entries = png_get_sPLT(png_ptr, info_ptr, &splt_entries);
        
        // Get cLLI chunk
        double max_content_light = 0, max_frame_avg_light = 0;
        png_get_cLLI(png_ptr, info_ptr, &max_content_light, &max_frame_avg_light);
        
        // Process some data then skip
        png_read_update_info(png_ptr, info_ptr);
        png_process_data_skip(png_ptr);
        
        // Get progressive pointer
        progressive_read_data read_data = {data, size, 0};
        void* progressive_ptr = png_get_progressive_ptr(png_ptr);
        
        // Read image data
        png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
        png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
        png_byte color_type = png_get_color_type(png_ptr, info_ptr);
        png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        
        // Allocate memory for row pointers
        png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
        }
        
        // Read image
        png_read_image(png_ptr, row_pointers);
        
        // Read end
        png_read_end(png_ptr, end_info);
        
        // Clean up row pointers
        for (png_uint_32 y = 0; y < height; y++) {
            free(row_pointers[y]);
        }
        free(row_pointers);
    }
    
    assert_file_closed(&in_file);;
    
    // Part 2: Simplified PNG image writing using png_image API
    // Initialize png_image structure
    image.version = PNG_IMAGE_VERSION;
    
    // Try to read the input as a PNG image
    if (png_image_begin_read_from_memory(&image, data, size)) {
        // Set image format
        image.format = PNG_FORMAT_RGBA;
        
        // Calculate buffer size for writing
        png_alloc_size_t required_size = 0;
        if (png_image_write_to_memory(&image, nullptr, &required_size, 
                                      fuzz_int32_t_6, nullptr, fuzz_int32_t_7, nullptr)) {
            // Allocate buffer for writing
            write_buffer = malloc(required_size);
            if (write_buffer) {
                write_buffer_size = required_size;
                
                // Create a simple buffer for source image data
                png_alloc_size_t row_stride = PNG_IMAGE_ROW_STRIDE(image);
                void* buffer = malloc(PNG_IMAGE_SIZE(image));
                
                if (buffer) {
                    // Fill buffer with test pattern
                    memset(buffer, 0x80, PNG_IMAGE_SIZE(image));
                    
                    // Write image to memory
                    png_image_write_to_memory(&image, write_buffer, &write_buffer_size,
                                              fuzz_int32_t_8, buffer, row_stride, nullptr);
                    
                    free(buffer);
                }
            }
        }
        
        png_image_free(&image);
    }
    
    // Part 3: Additional API calls using data from input
    // Extract a 32-bit value from input data
    if (size >= 4) {
        png_uint_32 uint32_val = png_get_uint_32(data);
        (void)uint32_val; // Use the value to avoid unused variable warning
    }
    
    // Clean up
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    }
    
    if (write_buffer) {
        free(write_buffer);
    }
    
    assert_file_closed(&in_file);
	return 0;
}