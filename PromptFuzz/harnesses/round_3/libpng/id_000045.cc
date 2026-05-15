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
//<ID> 880
//<Prompt> []
/*<Combination>: [
*/
//<score> 550, nr_unique_branch: 61
//<Quality> {"density":22,"unique_branches":{"png_set_pCAL":[[521,11,521,61,0,0,4,0]],"png_image_write_to_memory":[[2336,38,2336,74,0,0,4,0]],"png_set_tIME":[[1138,8,1138,28,0,0,4,0],[1138,34,1138,54,0,0,4,0],[1139,34,1139,54,0,0,4,0],[1140,8,1140,28,0,0,4,0],[1140,34,1140,55,0,0,4,0],[1141,8,1141,29,0,0,4,0]],"png_crc_error":[[273,11,274,71,0,0,4,0],[280,11,280,63,0,0,4,0]],"png_handle_eXIf":[[2013,35,2013,55,0,0,4,0]],"png_handle_sCAL":[[2276,8,2276,22,0,0,4,0],[2276,26,2276,40,0,0,4,0],[2292,13,2292,43,0,0,4,0]],"png_calculate_crc":[[155,11,156,71,0,0,4,0],[162,11,162,63,0,0,4,0],[171,8,171,21,0,0,4,1]],"png_set_text_2":[[1049,14,1049,41,0,0,4,0]],"png_handle_iTXt":[[2596,30,2596,47,0,0,4,0],[2607,8,2607,36,0,0,4,1],[2608,7,2608,59,0,0,4,1],[2618,14,2618,36,0,0,4,1],[2625,14,2625,36,0,0,4,1],[2636,30,2636,53,0,0,4,1],[2639,16,2639,31,0,0,4,1],[2639,35,2639,57,0,0,4,1]],"png_set_mDCV_fixed":[[261,8,261,27,0,0,4,0],[261,31,261,50,0,0,4,0]],"png_set_cLLI_fixed":[[174,8,174,28,0,0,4,0],[174,32,174,53,0,0,4,0]],"png_get_int_32_checked":[[1202,8,1202,32,0,0,4,1]],"png_get_next_frame_width":[[1433,8,1433,23,0,0,4,0],[1433,27,1433,43,0,0,4,0]],"png_handle_sBIT":[[1162,11,1162,22,0,0,4,0]],"png_handle_sRGB":[[1284,8,1284,18,0,0,4,0]],"png_check_fp_number":[[2118,7,2118,14,0,0,4,0],[2137,14,2137,43,0,0,4,1],[2148,19,2148,50,0,0,4,1],[2165,14,2165,45,0,0,4,0],[2195,14,2195,43,0,0,4,0],[2213,7,2213,14,0,0,4,0]],"png_handle_cHRM":[[1235,8,1235,13,0,0,4,0]],"png_get_compression_buffer_size":[[1281,8,1281,23,0,0,4,1],[1285,8,1285,49,0,0,4,0]],"png_crc_finish_critical":[[337,8,337,60,0,0,4,0]],"png_check_fp_string":[[2238,8,2238,67,0,0,4,1],[2239,8,2239,26,0,0,4,1],[2239,30,2239,53,0,0,4,1]],"png_handle_bKGD":[[1828,29,1828,78,0,0,4,0]],"png_set_crc_action":[[42,7,42,29,0,0,4,0],[52,7,52,30,0,0,4,1],[71,7,71,29,0,0,4,0]],"png_handle_pCAL":[[2167,8,2167,26,0,0,4,0],[2184,40,2184,52,0,0,4,0],[2185,9,2185,36,0,0,4,0],[2185,40,2185,52,0,0,4,0],[2186,9,2186,39,0,0,4,0],[2186,43,2186,55,0,0,4,0],[2748,18,2748,33,16,0,4,0],[2748,18,2748,33,17,0,4,0]],"png_set_tRNS":[[1213,14,1213,45,0,0,4,0]]},"library_calls":["png_sig_cmp","png_image_begin_read_from_memory","png_create_read_struct","png_image_free","png_create_info_struct","png_destroy_read_struct","png_image_free","png_set_longjmp_fn","png_destroy_read_struct","png_image_free","png_set_crc_action","png_set_read_fn","png_read_info","png_set_bKGD","png_get_progressive_ptr","png_get_compression_buffer_size","png_get_next_frame_x_offset","png_calloc","png_destroy_read_struct","png_image_free","png_get_next_frame_width","png_free","png_destroy_read_struct","png_image_free","png_image_finish_read","png_free","png_destroy_read_struct","png_image_free","png_free","png_destroy_read_struct","png_image_free","png_image_write_to_memory","png_free","png_destroy_read_struct","png_image_free"],"critical_calls":["png_sig_cmp","png_image_begin_read_from_memory","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_crc_action","png_set_read_fn","png_read_info","png_set_bKGD","png_get_progressive_ptr","png_get_compression_buffer_size","png_get_next_frame_x_offset","png_calloc","png_get_next_frame_width","png_image_finish_read","png_image_write_to_memory","png_free","png_destroy_read_struct","png_image_free"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to achieve the event of reading a PNG image from memory, processing it with both classic and png_image APIs, and writing it back to memory:

*/


extern "C" {
}

// Custom read function for classic API
void  id_000045_read_data_from_memory(png_structp png_ptr, png_bytep out, size_t length) {
    void** io_ptr = (void**)png_get_io_ptr(png_ptr);
    const uint8_t** data_ptr = (const uint8_t**)io_ptr[0];
    size_t* size_ptr = (size_t*)io_ptr[1];
    size_t* offset_ptr = (size_t*)io_ptr[2];
    
    if (*offset_ptr + length > *size_ptr) {
        png_error(png_ptr, "Read beyond buffer");
    }
    
    memcpy(out, *data_ptr + *offset_ptr, length);
    *offset_ptr += length;
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<32) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    if (size < 8) {
        return 0;
    }
    
    // 1. Check PNG signature
    if (png_sig_cmp(data, fuzz_uint64_t_1, 8) != 0) {
        return 0;  // Not a PNG file
    }
    
    // 2. Use png_image API to read from memory
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    if (!png_image_begin_read_from_memory(&image, data, size)) {
        return 0;
    }
    
    // 3. Create read structure for classic API
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
    
    // 5. Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        return 0;
    }
    
    // 6. Set CRC action
    png_set_crc_action(png_ptr, fuzz_int32_t_3, fuzz_int32_t_4);
    
    // 7. Set up custom read function
    size_t offset = 0;
    void* io_ptrs[3] = {(void*)&data, (void*)&size, (void*)&offset};
    png_set_read_fn(png_ptr, io_ptrs,  id_000045_read_data_from_memory);
    
    // 8. Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // 9. Set background color (dummy value)
    png_color_16 background;
    memset(&background, 0, sizeof(background));
    png_set_bKGD(png_ptr, info_ptr, &background);
    
    // 10. Get progressive pointer (returns nullptr in non-progressive mode)
    void* progressive_ptr = png_get_progressive_ptr(png_ptr);
    (void)progressive_ptr;  // Mark as used
    
    // 11. Get compression buffer size
    size_t comp_buffer_size = png_get_compression_buffer_size(png_ptr);
    (void)comp_buffer_size;  // Mark as used
    
    // 12. Get next frame info (for APNG)
    png_uint_32 next_frame_x_offset = png_get_next_frame_x_offset(png_ptr, info_ptr);
    (void)next_frame_x_offset;  // Mark as used
    
    // 13. Allocate memory using png_calloc
    png_alloc_size_t alloc_size = 1024;
    void* allocated_mem = png_calloc(png_ptr, alloc_size);
    if (!allocated_mem) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        return 0;
    }
    
    // 14. Get next frame width (for APNG)
    png_uint_32 next_frame_width = png_get_next_frame_width(png_ptr, info_ptr);
    (void)next_frame_width;  // Mark as used
    
    // 15. Finish reading with png_image API
    // Calculate buffer size needed for the image
    png_alloc_size_t buffer_size = PNG_IMAGE_SIZE(image);
    void* image_buffer = malloc(buffer_size);
    if (!image_buffer) {
        png_free(png_ptr, allocated_mem);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        return 0;
    }
    
    // Read the image data
    if (!png_image_finish_read(&image, nullptr, image_buffer, 0, nullptr)) {
        free(image_buffer);
        png_free(png_ptr, allocated_mem);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        return 0;
    }
    
    // 16. Write image back to memory using png_image API
    png_alloc_size_t output_size = 0;
    void* output_buffer = malloc(PNG_IMAGE_SIZE(image) * 2);  // Allocate extra space
    if (!output_buffer) {
        free(image_buffer);
        png_free(png_ptr, allocated_mem);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        return 0;
    }
    
    // Try to write the image to memory
    int write_result = png_image_write_to_memory(&image, output_buffer, &output_size, 
                                                 fuzz_int32_t_5, image_buffer, 0, nullptr);
    (void)write_result;  // Result may fail, but we don't care for fuzzing
    
    // 17. Clean up allocated memory
    free(output_buffer);
    free(image_buffer);
    png_free(png_ptr, allocated_mem);
    
    // 18. Destroy classic API structures
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    // 19. Free png_image structure
    png_image_free(&image);
    
    return 0;
}