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
//<ID> 1131
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 15
//<Quality> {"density":1,"unique_branches":{"png_set_sPLT":[[1253,8,1253,23,0,0,4,1],[1253,27,1253,43,0,0,4,1],[1253,47,1253,60,0,0,4,1],[1253,64,1253,79,0,0,4,1],[1263,8,1263,18,0,0,4,1],[1282,11,1282,32,0,0,4,1],[1282,36,1282,60,0,0,4,0],[1332,11,1332,21,0,0,4,1],[1334,8,1334,20,0,0,4,1]],"png_set_text_compression_mem_level":[[1298,8,1298,23,0,0,4,1]],"png_set_bgr":[[24,8,24,23,0,0,4,1]],"png_free_data":[[574,8,574,39,0,0,4,0],[575,8,575,57,0,0,4,0],[577,11,577,20,0,0,4,1],[589,22,589,53,0,0,4,1]]},"library_calls":["png_create_read_struct_2","png_create_info_struct","png_set_mem_fn","png_set_longjmp_fn","png_init_io","png_read_info","png_set_scale_16","png_set_bgr","png_set_invert_mono","png_set_text_compression_mem_level","png_set_sPLT","png_read_update_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_num_plays","png_get_rowbytes","png_malloc","png_read_image","png_create_write_struct_2","png_create_info_struct","png_init_io","png_set_IHDR","png_write_info","png_write_row","png_write_end","png_destroy_write_struct","png_free","png_read_end","png_destroy_read_struct"],"critical_calls":["png_read_end","png_free","png_destroy_write_struct","png_write_end","png_write_row","png_write_info","png_set_IHDR","png_init_io","png_create_info_struct","png_create_write_struct_2","png_read_image","png_malloc","png_get_rowbytes","png_get_num_plays","png_get_bit_depth","png_get_color_type","png_get_image_height","png_get_image_width","png_read_update_info","png_set_sPLT","png_set_text_compression_mem_level","png_set_invert_mono","png_set_bgr","png_set_scale_16","png_read_info","png_init_io","png_set_longjmp_fn","png_set_mem_fn","png_create_info_struct","png_create_read_struct_2","png_destroy_read_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to achieve the event of reading and transforming a PNG image from memory:

*/


// Custom memory functions to use with png_set_mem_fn
static void*  id_000050_custom_png_malloc(png_struct* png_ptr, png_alloc_size_t size) {
    return malloc(size);
}

static void  id_000050_custom_png_free(png_struct* png_ptr, void* ptr) {
    free(ptr);
}

// Custom write function for libpng
static void  id_000050_custom_write_data(png_struct* png_ptr, png_byte* data, png_size_t length) {
    FILE* fp = (FILE*)png_get_io_ptr(png_ptr);
    if (fp && data && length > 0) {
        fwrite(data, 1, length, fp);
    }
}

// Custom flush function for libpng
static void  id_000050_custom_flush_data(png_struct* png_ptr) {
    FILE* fp = (FILE*)png_get_io_ptr(png_ptr);
    if (fp) {
        fflush(fp);
    }
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<44) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    // Create input file from memory buffer
    FILE* in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_struct* png_ptr = nullptr;
    png_info* info_ptr = nullptr;
    png_image image = {0};
    png_sPLT_t splt_entry = {0};
    
    try {
        // Create read structure with custom memory functions
        png_ptr = png_create_read_struct_2(
            fuzz_str_1,  // Use default version
            nullptr,                 // Error pointer
            nullptr,                 // Error function
            nullptr,                 // Warning function
            nullptr,                 // Memory pointer
             id_000050_custom_png_malloc,       // Custom malloc
             id_000050_custom_png_free          // Custom free
        );
        
        if (!png_ptr) {
            throw std::runtime_error("Failed to create PNG read structure");
        }
        
        // Initialize info structure
        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            throw std::runtime_error("Failed to create PNG info structure");
        }
        
        // Set memory function (already done in create, but calling as required)
        png_set_mem_fn(png_ptr, nullptr,  id_000050_custom_png_malloc,  id_000050_custom_png_free);
        
        // Set error handling
        if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
            throw std::runtime_error("PNG error during processing");
        }
        
        // Initialize I/O
        png_init_io(png_ptr, in_file);
        
        // Read PNG header
        png_read_info(png_ptr, info_ptr);
        
        // Apply various transformations
        // Scale 16-bit data to 8-bit
        png_set_scale_16(png_ptr);
        
        // Convert to BGR format
        png_set_bgr(png_ptr);
        
        // Invert monochrome images
        png_set_invert_mono(png_ptr);
        
        // Set text compression memory level
        png_set_text_compression_mem_level(png_ptr, fuzz_int32_t_2);  // Default is 8
        
        // Initialize a simple sPLT palette entry
        splt_entry.name = strdup("Fuzzer Palette");
        splt_entry.depth = 8;
        splt_entry.nentries = 0;  // No entries for simplicity
        splt_entry.entries = nullptr;
        
        // Set sPLT chunk
        png_set_sPLT(png_ptr, info_ptr, &splt_entry, 1);
        
        // Update transformations
        png_read_update_info(png_ptr, info_ptr);
        
        // Get image dimensions
        png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
        png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
        int color_type = png_get_color_type(png_ptr, info_ptr);
        int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        
        // Get number of plays (for APNG)
        png_uint_32 num_plays = png_get_num_plays(png_ptr, info_ptr);
        
        // Prepare for reading image data
        png_bytep row_pointers[height];
        size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
        
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_byte*)png_malloc(png_ptr, rowbytes);
        }
        
        // Read image
        png_read_image(png_ptr, row_pointers);
        
        // Setup png_image structure for png_image_finish_read
        image.version = PNG_IMAGE_VERSION;
        image.width = width;
        image.height = height;
        image.format = PNG_FORMAT_RGBA;  // Target format
        
        // Create buffer for final image
        size_t buffer_size = PNG_IMAGE_SIZE(image);
        void* buffer = malloc(buffer_size);
        
        if (buffer) {
            // Call png_image_finish_read (simulating a read operation)
            // Note: In a real scenario, we'd use png_image_begin_read first
            png_color background = {255, 255, 255};  // White background
            png_int_32 row_stride = PNG_IMAGE_ROW_STRIDE(image);
            
            // For demonstration, we'll create a write structure instead
            // since png_image_finish_read needs a png_image started with png_image_begin_read
            
            // Create write structure for output
            png_struct* write_ptr = png_create_write_struct_2(
                fuzz_str_3,
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                 id_000050_custom_png_malloc,
                 id_000050_custom_png_free
            );
            
            if (write_ptr) {
                png_info* write_info = png_create_info_struct(write_ptr);
                
                if (write_info) {
                    png_init_io(write_ptr, out_file);
                    
                    // Set IHDR parameters
                    png_set_IHDR(write_ptr, write_info, width, height,
                                fuzz_int32_t_4,  // bit_depth
                                PNG_COLOR_TYPE_RGBA,  // color_type
                                fuzz_int32_t_5,
                                fuzz_int32_t_6,
                                fuzz_int32_t_7);
                    
                    // Write headers
                    png_write_info(write_ptr, write_info);
                    
                    // Write image data
                    for (png_uint_32 y = 0; y < height; y++) {
                        png_write_row(write_ptr, row_pointers[y]);
                    }
                    
                    png_write_end(write_ptr, nullptr);
                    png_destroy_write_struct(&write_ptr, &write_info);
                }
            }
            
            free(buffer);
        }
        
        // Cleanup row pointers
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
        
        // End reading
        png_read_end(png_ptr, info_ptr);
        
    } catch (const std::exception& e) {
        // Error handling
    }
    
    // Cleanup
    if (splt_entry.name) {
        free(splt_entry.name);
    }
    
    if (png_ptr && info_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}