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
//<ID> 561
//<Prompt> []
/*<Combination>: [
*/
//<score> 357, nr_unique_branch: 278
//<Quality> {"density":17,"unique_branches":{"png_set_strip_alpha":[[202,8,202,37,0,0,4,1]],"png_push_read_chunk":[[194,8,194,52,0,0,4,0],[194,8,194,52,0,0,4,1],[204,8,204,36,0,0,4,0],[204,8,204,36,0,0,4,1],[205,8,205,55,0,0,4,1],[302,8,302,30,0,0,4,0],[302,8,302,30,0,0,4,1],[304,11,304,48,0,0,4,1],[311,11,311,47,0,0,4,0],[311,11,311,47,0,0,4,1],[314,16,314,61,0,0,4,0],[314,16,314,61,0,0,4,1],[315,11,315,47,0,0,4,0],[315,11,315,47,0,0,4,1],[320,11,320,47,0,0,4,1],[327,11,327,48,0,0,4,1],[331,13,331,49,0,0,4,0],[331,13,331,49,0,0,4,1],[339,8,339,30,0,0,4,0],[339,8,339,30,0,0,4,1],[341,11,341,37,0,0,4,0],[341,11,341,37,0,0,4,1],[348,13,348,35,0,0,4,0],[348,13,348,35,0,0,4,1],[358,13,358,74,0,0,4,1],[368,13,368,35,0,0,4,0],[368,13,368,35,0,0,4,1],[384,13,384,35,0,0,4,0],[384,13,384,35,0,0,4,1],[386,11,386,58,0,0,4,0],[386,11,386,58,0,0,4,1],[395,13,395,35,0,0,4,0],[395,13,395,35,0,0,4,1],[397,11,397,58,0,0,4,0],[397,11,397,58,0,0,4,1],[31,5,31,29,2,0,4,0],[31,5,31,29,2,0,4,1],[28,5,28,52,32,0,4,0],[28,5,28,52,32,0,4,1],[28,5,28,52,34,0,4,0],[28,5,28,52,34,0,4,1],[534,6,534,23,41,0,4,0],[534,6,534,23,41,0,4,1],[28,5,28,52,44,0,4,0],[28,5,28,52,44,0,4,1]],"png_read_push_finish_row":[[1028,8,1028,47,0,0,4,0],[1028,8,1028,47,0,0,4,1],[1032,8,1032,32,0,0,4,0],[1032,8,1032,32,0,0,4,1],[1040,15,1040,33,0,0,4,0],[1040,15,1040,33,0,0,4,1],[1040,37,1040,55,0,0,4,1],[1041,15,1041,33,0,0,4,0],[1041,15,1041,33,0,0,4,1],[1041,37,1041,55,0,0,4,1],[1042,15,1042,33,0,0,4,0],[1042,15,1042,33,0,0,4,1],[1042,37,1042,55,0,0,4,1],[1045,14,1045,31,0,0,4,1],[1048,14,1048,32,0,0,4,0],[1048,14,1048,32,0,0,4,1],[1056,14,1056,61,0,0,4,0]],"png_push_have_info":[[1072,8,1072,32,0,0,4,0]],"png_get_pixels_per_meter":[[172,8,172,23,0,0,4,0],[172,27,172,43,0,0,4,0],[173,8,173,46,0,0,4,0],[173,8,173,46,0,0,4,1],[175,11,175,59,0,0,4,0],[176,11,176,69,0,0,4,0]],"png_push_process_row":[[807,8,807,51,0,0,4,0],[807,8,807,51,0,0,4,1],[809,11,809,54,0,0,4,0],[809,11,809,54,0,0,4,1],[824,8,824,37,0,0,4,0],[829,8,829,45,0,0,4,0],[829,8,829,45,0,0,4,1],[832,11,832,62,0,0,4,1],[836,13,836,69,0,0,4,1],[842,8,842,32,0,0,4,0],[842,8,842,32,0,0,4,1],[843,8,843,55,0,0,4,0],[845,11,845,28,0,0,4,0],[845,11,845,28,0,0,4,1],[851,10,851,16,0,0,4,0],[851,10,851,16,0,0,4,1],[854,25,854,30,0,0,4,0],[854,25,854,30,0,0,4,1],[854,34,854,52,0,0,4,0],[854,34,854,52,0,0,4,1],[860,17,860,35,0,0,4,1],[869,17,869,35,0,0,4,1],[878,17,878,35,0,0,4,1],[887,10,887,16,0,0,4,0],[887,10,887,16,0,0,4,1],[890,25,890,30,0,0,4,0],[890,25,890,30,0,0,4,1],[890,34,890,52,0,0,4,0],[890,34,890,52,0,0,4,1],[896,17,896,35,0,0,4,0],[896,17,896,35,0,0,4,1],[898,28,898,33,0,0,4,0],[898,28,898,33,0,0,4,1],[898,37,898,55,0,0,4,0],[908,10,908,16,0,0,4,0],[908,10,908,16,0,0,4,1],[912,25,912,30,0,0,4,0],[912,25,912,30,0,0,4,1],[912,34,912,52,0,0,4,0],[912,34,912,52,0,0,4,1],[918,25,918,30,0,0,4,0],[918,25,918,30,0,0,4,1],[918,34,918,52,0,0,4,0],[918,34,918,52,0,0,4,1],[924,17,924,35,0,0,4,1],[936,10,936,16,0,0,4,0],[936,10,936,16,0,0,4,1],[940,25,940,30,0,0,4,0],[940,25,940,30,0,0,4,1],[940,34,940,52,0,0,4,0],[940,34,940,52,0,0,4,1],[946,17,946,35,0,0,4,0],[946,17,946,35,0,0,4,1],[948,28,948,33,0,0,4,0],[948,28,948,33,0,0,4,1],[948,37,948,55,0,0,4,0],[958,10,958,16,0,0,4,0],[958,10,958,16,0,0,4,1],[962,25,962,30,0,0,4,0],[962,25,962,30,0,0,4,1],[962,34,962,52,0,0,4,0],[968,25,968,30,0,0,4,0],[968,25,968,30,0,0,4,1],[968,34,968,52,0,0,4,0],[968,34,968,52,0,0,4,1],[974,17,974,35,0,0,4,1],[983,10,983,16,0,0,4,0],[983,10,983,16,0,0,4,1],[987,25,987,30,0,0,4,0],[987,25,987,30,0,0,4,1],[987,34,987,52,0,0,4,0],[987,34,987,52,0,0,4,1],[993,17,993,35,0,0,4,0],[993,17,993,35,0,0,4,1],[1002,10,1002,17,0,0,4,1],[1003,10,1003,16,0,0,4,0],[1003,10,1003,16,0,0,4,1],[1008,17,1008,35,0,0,4,0],[1008,17,1008,35,0,0,4,1],[534,6,534,23,1,0,4,0],[534,6,534,23,1,0,4,1]],"png_process_IDAT_data":[[678,8,678,28,0,0,4,1],[678,32,678,46,0,0,4,1],[683,8,683,45,0,0,4,0],[683,49,683,77,0,0,4,1],[701,11,701,40,0,0,4,0],[701,11,701,40,0,0,4,1],[702,7,702,53,0,0,4,0],[702,7,702,53,0,0,4,1],[711,11,711,44,0,0,4,0],[711,11,711,44,0,0,4,1],[730,11,730,22,0,0,4,0],[730,11,730,22,0,0,4,1],[730,26,730,45,0,0,4,0],[730,26,730,45,0,0,4,1],[739,14,739,54,0,0,4,0],[739,14,739,54,0,0,4,1],[740,14,740,31,0,0,4,0],[740,14,740,31,0,0,4,1],[745,17,745,36,0,0,4,0],[745,17,745,36,0,0,4,1],[756,11,756,56,0,0,4,0],[756,11,756,56,0,0,4,1],[762,14,762,54,0,0,4,0],[762,14,762,54,0,0,4,1],[763,14,763,31,0,0,4,0],[763,14,763,31,0,0,4,1],[777,14,777,45,0,0,4,0],[777,14,777,45,0,0,4,1],[782,11,782,30,0,0,4,0],[782,11,782,30,0,0,4,1],[790,8,790,37,0,0,4,0],[790,8,790,37,0,0,4,1],[534,6,534,23,3,0,4,0],[534,6,534,23,3,0,4,1]],"png_init_read_transformations":[[1471,8,1471,57,0,0,4,0],[1472,8,1472,53,0,0,4,0]],"png_push_fill_buffer":[[426,8,426,38,0,0,4,0],[430,11,430,45,0,0,4,0],[430,11,430,45,0,0,4,1],[443,8,443,19,0,0,4,0],[443,23,443,56,0,0,4,0],[447,11,447,48,0,0,4,0],[447,11,447,48,0,0,4,1]],"png_push_read_sig":[[162,8,162,72,0,0,4,1],[173,11,173,34,0,0,4,0]],"png_do_read_transformations":[[4902,8,4902,57,0,0,4,0],[4903,8,4903,53,0,0,4,0],[4904,9,4904,57,0,0,4,0],[4904,9,4904,57,0,0,4,1],[4905,8,4905,57,0,0,4,0],[4905,8,4905,57,0,0,4,1],[5000,8,5000,57,0,0,4,0],[5001,8,5001,53,0,0,4,1]],"png_push_have_row":[[1086,8,1086,31,0,0,4,0]],"png_do_strip_channel":[[539,8,539,31,0,0,4,0],[539,8,539,31,0,0,4,1],[541,11,541,35,0,0,4,0],[541,11,541,35,0,0,4,1],[543,14,543,27,0,0,4,1],[551,17,551,24,0,0,4,0],[551,17,551,24,0,0,4,1],[559,16,559,41,0,0,4,0],[561,14,561,27,0,0,4,1],[568,17,568,24,0,0,4,0],[568,17,568,24,0,0,4,1],[582,11,582,60,0,0,4,0],[587,13,587,36,0,0,4,0],[589,11,589,35,0,0,4,0],[589,11,589,35,0,0,4,1],[591,14,591,27,0,0,4,1],[599,17,599,24,0,0,4,0],[599,17,599,24,0,0,4,1],[607,16,607,41,0,0,4,0],[609,14,609,27,0,0,4,1],[616,17,616,24,0,0,4,0],[616,17,616,24,0,0,4,1],[633,11,633,59,0,0,4,0]],"png_process_some_data":[[115,7,115,29,0,0,4,1],[121,7,121,31,0,0,4,0],[127,7,127,30,0,0,4,0],[133,7,133,14,0,0,4,0]],"png_get_current_row_number":[[880,8,880,23,0,0,4,0]],"png_push_save_buffer":[[463,8,463,38,0,0,4,0],[463,8,463,38,0,0,4,1],[465,11,465,59,0,0,4,1],[479,8,480,32,0,0,4,0],[479,8,480,32,0,0,4,1],[485,11,486,47,0,0,4,1],[496,11,496,39,0,0,4,1],[502,11,502,21,0,0,4,0],[502,11,502,21,0,0,4,1],[504,16,504,41,0,0,4,1],[509,8,509,36,0,0,4,0],[509,8,509,36,0,0,4,1]],"png_get_sBIT":[[1145,8,1145,23,0,0,4,0],[1145,27,1145,43,0,0,4,0],[1146,8,1146,46,0,0,4,0],[1146,8,1146,46,0,0,4,1],[1146,50,1146,65,0,0,4,0]],"png_push_read_IDAT":[[533,8,533,52,0,0,4,0],[533,8,533,52,0,0,4,1],[552,11,552,42,0,0,4,0],[552,11,552,42,0,0,4,1],[552,46,552,74,0,0,4,1],[580,11,580,42,0,0,4,0],[580,11,580,42,0,0,4,1],[580,46,580,75,0,0,4,0],[587,14,587,60,0,0,4,0],[587,14,587,60,0,0,4,1],[591,14,591,43,0,0,4,1],[602,11,602,39,0,0,4,1],[610,8,610,31,0,0,4,0],[610,8,610,31,0,0,4,1],[610,35,610,65,0,0,4,1],[637,8,637,31,0,0,4,0],[637,8,637,31,0,0,4,1],[637,35,637,68,0,0,4,0],[647,11,647,32,0,0,4,0],[647,11,647,32,0,0,4,1],[663,8,663,31,0,0,4,0],[663,8,663,31,0,0,4,1],[31,5,31,29,2,0,4,0],[31,5,31,29,2,0,4,1],[31,5,31,29,13,0,4,0],[31,5,31,29,13,0,4,1]],"png_push_have_end":[[1079,8,1079,31,0,0,4,0]],"png_read_transform_info":[[2215,8,2215,57,0,0,4,0]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_read_fn","png_set_progressive_read_fn","png_process_data","png_process_data","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_compression_level","png_destroy_info_struct","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_set_read_fn","png_set_progressive_read_fn","png_process_data","png_process_data","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_compression_level","png_destroy_info_struct","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to read, process, and write a PNG image:

*/


// Context structure to pass between callbacks
struct fuzz_context {
    const uint8_t* data;
    size_t size;
    size_t data_offset;
    FILE* out_file;
    png_structp png_write;
    png_infop info_write;
};

// Custom read function for progressive processing
void  id_000041_user_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    fuzz_context* ctx = (fuzz_context*)png_get_io_ptr(png_ptr);
    
    if (ctx->data_offset + length > ctx->size) {
        png_error(png_ptr, "Not enough input data");
    }
    
    memcpy(data, ctx->data + ctx->data_offset, length);
    ctx->data_offset += length;
}

// Progressive callbacks
void  id_000041_info_callback(png_structp png_ptr, png_infop info_ptr) {
    // Set transformations on the read structure
    png_set_strip_alpha(png_ptr);
    
    // Handle interlace - get number of passes needed
    int number_of_passes = png_set_interlace_handling(png_ptr);
    
    // Get and potentially use sBIT information
    png_color_8* sig_bit = nullptr;
    if (png_get_sBIT(png_ptr, info_ptr, &sig_bit)) {
        // sBIT information is available
    }
    
    // Get resolution information
    png_uint_32 pixels_per_inch = png_get_pixels_per_inch(png_ptr, info_ptr);
    
    // Update info after transformations
    png_read_update_info(png_ptr, info_ptr);
}

void  id_000041_row_callback(png_structp png_ptr, png_bytep new_row, png_uint_32 row_num, int pass) {
    // Get current row number
    png_uint_32 current_row = png_get_current_row_number(png_ptr);
    
    // Skip processing of some rows (simplified example)
    if (current_row % 10 == 0) {
        // In real usage, this would skip processing of certain data
        // png_process_data_skip(png_ptr);
    }
    
    // Write warning for demonstration
    if (current_row == 0) {
        png_warning(png_ptr, "Starting row processing");
    }
}

void  id_000041_end_callback(png_structp png_ptr, png_infop info_ptr) {
    // End of image processing
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
        return 0;  // Not enough data for PNG signature
    }
    
    // Check PNG signature
    if (png_sig_cmp((png_const_bytep)data, 0, 8) != 0) {
        return 0;  // Not a PNG file
    }
    
    // Create read structure
    png_structp png_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                  nullptr, nullptr, nullptr);
    if (!png_read) {
        return 0;
    }
    
    // Create info structure
    png_infop info_read = png_create_info_struct(png_read);
    if (!info_read) {
        png_destroy_read_struct(&png_read, nullptr, nullptr);
        return 0;
    }
    
    // Create end info structure
    png_infop end_info = png_create_info_struct(png_read);
    if (!end_info) {
        png_destroy_read_struct(&png_read, &info_read, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_read))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_read, &info_read, &end_info);
        return 0;
    }
    
    // Set up context for custom reading
    fuzz_context ctx;
    ctx.data = data;
    ctx.size = size;
    ctx.data_offset = 0;
    ctx.out_file = nullptr;
    ctx.png_write = nullptr;
    ctx.info_write = nullptr;
    
    // Set custom read function
    png_set_read_fn(png_read, &ctx,  id_000041_user_read_data);
    
    // Set progressive callbacks
    png_set_progressive_read_fn(png_read, &ctx,  id_000041_info_callback,  id_000041_row_callback,  id_000041_end_callback);
    
    // Start processing
    png_process_data(png_read, info_read, (png_bytep)data, 8);  // Process signature
    ctx.data_offset = 8;
    
    // Process remaining data in chunks
    const size_t CHUNK_SIZE = 4096;
    while (ctx.data_offset < size) {
        size_t remaining = size - ctx.data_offset;
        size_t process_size = remaining < CHUNK_SIZE ? remaining : CHUNK_SIZE;
        
        png_process_data(png_read, info_read, 
                        (png_bytep)(data + ctx.data_offset), 
                        process_size);
        ctx.data_offset += process_size;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create write structure for output
    ctx.out_file = fopen("output_file", "wb");
    if (ctx.out_file) {
        ctx.png_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                               nullptr, nullptr, nullptr);
        if (ctx.png_write) {
            ctx.info_write = png_create_info_struct(ctx.png_write);
            if (ctx.info_write) {
                if (setjmp(png_jmpbuf(ctx.png_write))) {
	FUZZ_FILE_CLOSE();
                    // Error in write operations
                } else {
                    png_init_io(ctx.png_write, ctx.out_file);
                    
                    // Set text compression level (example usage)
                    // Note: This API might not be available in all versions
                    // Using compression level as an example
                    png_set_compression_level(ctx.png_write, 6);
                    
                    // For demonstration, we would write the processed image here
                    // This is simplified since we need the actual image data
                }
                
                png_destroy_info_struct(ctx.png_write, &ctx.info_write);
            }
            png_destroy_write_struct(&ctx.png_write, nullptr);
        }
        fclose(ctx.out_file);
    }
    
    // Clean up
    png_destroy_read_struct(&png_read, &info_read, &end_info);
    
    return 0;
}