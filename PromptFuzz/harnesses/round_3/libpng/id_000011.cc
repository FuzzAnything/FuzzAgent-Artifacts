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
//<ID> 131
//<Prompt> []
/*<Combination>: [
*/
//<score> 165, nr_unique_branch: 116
//<Quality> {"density":45,"unique_branches":{"png_set_expand_gray_1_2_4_to_8":[[975,8,975,37,0,0,4,1]],"png_do_gray_to_rgb":[[2993,8,2993,32,0,0,4,0],[2994,8,2994,58,0,0,4,0],[2996,11,2996,54,0,0,4,0],[2996,11,2996,54,0,0,4,1],[2998,14,2998,38,0,0,4,0],[2998,14,2998,38,0,0,4,1],[3003,25,3003,38,0,0,4,0],[3003,25,3003,38,0,0,4,1],[3016,25,3016,38,0,0,4,0],[3016,25,3016,38,0,0,4,1],[3028,16,3028,65,0,0,4,0],[3030,14,3030,38,0,0,4,0],[3030,14,3030,38,0,0,4,1],[3035,25,3035,38,0,0,4,0],[3035,25,3035,38,0,0,4,1],[3049,25,3049,38,0,0,4,0],[3049,25,3049,38,0,0,4,1],[534,6,534,23,6,0,4,0]],"png_set_gray_to_rgb":[[1016,8,1016,37,0,0,4,1]],"png_image_write_to_file":[[2418,8,2418,21,0,0,4,0],[2418,25,2418,60,0,0,4,0],[2420,11,2420,28,0,0,4,0],[2420,32,2420,46,0,0,4,0],[2424,14,2424,24,0,0,4,0],[2426,17,2427,43,0,0,4,0],[2432,20,2432,35,0,0,4,0],[2432,39,2432,54,0,0,4,0],[2434,23,2434,38,0,0,4,0]],"png_read_start_row":[[4645,11,4645,56,0,0,4,0],[4647,14,4647,37,0,0,4,0],[4647,14,4647,37,0,0,4,1],[4717,8,4717,57,0,0,4,0],[4721,12,4721,35,0,0,4,0],[4721,12,4721,35,0,0,4,1],[4722,11,4722,55,0,0,4,0],[4725,11,4725,57,0,0,4,0],[4725,11,4725,57,0,0,4,1],[4727,11,4727,59,0,0,4,0],[4729,14,4729,35,0,0,4,0],[4729,14,4729,35,0,0,4,1]],"png_read_image":[[717,11,717,58,0,0,4,0]],"png_image_write_main":[[2048,14,2048,38,0,0,4,1]],"png_init_read_transformations":[[1605,8,1605,73,0,0,4,0],[1606,8,1606,53,0,0,4,1]],"png_image_write_to_stdio":[[2368,8,2368,21,0,0,4,0],[2368,25,2368,60,0,0,4,0],[2370,11,2370,23,0,0,4,0],[2370,27,2370,41,0,0,4,0],[2372,14,2372,46,0,0,4,0]],"png_get_valid":[[29,11,29,32,0,0,4,0],[29,36,29,59,0,0,4,0],[29,36,29,59,0,0,4,1]],"png_read_end":[[807,16,807,32,0,0,4,0]],"png_do_chop":[[2580,8,2580,33,0,0,4,0],[2586,14,2586,21,0,0,4,0],[2586,14,2586,21,0,0,4,1]],"png_do_read_filler":[[2813,8,2813,51,0,0,4,1],[2890,13,2890,55,0,0,4,0],[2890,13,2890,55,0,0,4,1],[2892,11,2892,35,0,0,4,0],[2894,14,2894,50,0,0,4,0],[2899,25,2899,38,0,0,4,0],[2899,25,2899,38,0,0,4,1]],"png_do_expand_palette":[[4345,8,4345,54,0,0,4,0],[4347,11,4347,34,0,0,4,0],[4347,11,4347,34,0,0,4,1],[4351,13,4351,19,0,0,4,0],[4351,13,4351,19,0,0,4,1],[4356,28,4356,41,0,0,4,0],[4356,28,4356,41,0,0,4,1],[4358,23,4358,44,0,0,4,0],[4358,23,4358,44,0,0,4,1],[4364,23,4364,33,0,0,4,0],[4364,23,4364,33,0,0,4,1],[4378,13,4378,19,0,0,4,0],[4378,13,4378,19,0,0,4,1],[4383,28,4383,41,0,0,4,0],[4383,28,4383,41,0,0,4,1],[4387,23,4387,33,0,0,4,0],[4387,23,4387,33,0,0,4,1],[4401,13,4401,19,0,0,4,0],[4401,13,4401,19,0,0,4,1],[4406,28,4406,41,0,0,4,0],[4406,28,4406,41,0,0,4,1],[4410,23,4410,33,0,0,4,0],[4410,23,4410,33,0,0,4,1],[4424,13,4424,20,0,0,4,1],[4432,11,4432,35,0,0,4,0],[4435,17,4435,30,0,0,4,0],[4435,17,4435,30,0,0,4,1],[4441,28,4441,41,0,0,4,0],[4441,28,4441,41,0,0,4,1],[4443,23,4443,46,0,0,4,0],[4443,23,4443,46,0,0,4,1],[4463,28,4463,41,0,0,4,0],[4463,28,4463,41,0,0,4,1]],"png_set_tRNS_to_alpha":[[987,8,987,37,0,0,4,1]],"png_read_transform_info":[[2073,11,2073,57,0,0,4,0],[2079,14,2079,36,0,0,4,0],[2079,14,2079,36,0,0,4,1],[2088,14,2088,38,0,0,4,1],[2137,14,2137,59,0,0,4,0],[2167,8,2167,57,0,0,4,0],[2234,11,2234,58,0,0,4,1]],"png_set_palette_to_rgb":[[963,8,963,37,0,0,4,1]],"png_do_write_transformations":[[509,11,509,51,0,0,4,0]],"png_write_info_before_PLTE":[[178,8,178,46,0,0,4,0]],"png_set_strip_16":[[189,8,189,37,0,0,4,1]],"png_do_read_transformations":[[4875,11,4875,57,0,0,4,0],[4966,8,4966,57,0,0,4,0],[4967,8,4967,53,0,0,4,0],[5024,8,5024,53,0,0,4,0],[5046,8,5046,57,0,0,4,0],[5047,8,5047,53,0,0,4,1],[5090,8,5090,52,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_progressive_frame_fn","png_read_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_destroy_read_struct","png_set_palette_to_rgb","png_set_expand_gray_1_2_4_to_8","png_get_valid","png_set_tRNS_to_alpha","png_set_strip_16","png_set_gray_to_rgb","png_set_filler","png_read_update_info","png_destroy_read_struct","png_get_rowbytes","png_destroy_read_struct","png_read_image","png_read_end","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_set_IHDR","png_set_gAMA_fixed","png_set_mDCV","png_set_write_user_transform_fn","png_write_info","png_write_image","png_write_end","png_image_write_to_file","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_progressive_frame_fn","png_read_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_palette_to_rgb","png_set_expand_gray_1_2_4_to_8","png_get_valid","png_set_tRNS_to_alpha","png_set_strip_16","png_set_gray_to_rgb","png_set_filler","png_read_update_info","png_get_rowbytes","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_set_gAMA_fixed","png_set_mDCV","png_set_write_user_transform_fn","png_write_info","png_write_image","png_write_end","png_image_write_to_file","png_destroy_write_struct","png_destroy_read_struct"],"visited":5}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to process PNG data:

*/


// Global variables for progressive reading
static png_structp  id_000011_read_ptr = nullptr;
static png_infop  id_000011_read_info = nullptr;
static png_bytep*  id_000011_row_pointers = nullptr;
static png_uint_32  id_000011_width = 0;
static png_uint_32  id_000011_height = 0;
static int  id_000011_color_type = 0;
static int  id_000011_bit_depth = 0;

// Progressive frame callbacks
static void  id_000011_frame_info_callback(png_structp png_ptr, png_uint_32 frame_num) {
    // Frame information callback
}

static void  id_000011_frame_end_callback(png_structp png_ptr, png_uint_32 frame_num) {
    // Frame end callback
}

// User transform function
static void  id_000011_write_user_transform_fn(png_structp png_ptr, png_row_infop row_info, png_bytep data) {
    // Simple user transform: just pass through
    // Could be used for custom processing
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<76) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	
    
    FILE* in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create read structure
     id_000011_read_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (! id_000011_read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create info structure
     id_000011_read_info = png_create_info_struct( id_000011_read_ptr);
    if (! id_000011_read_info) {
        png_destroy_read_struct(& id_000011_read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf( id_000011_read_ptr))) {
	FUZZ_FILE_CLOSE();
        // Clean up on error
        if ( id_000011_row_pointers) {
            for (png_uint_32 y = 0; y <  id_000011_height; y++) {
                free( id_000011_row_pointers[y]);
            }
            free( id_000011_row_pointers);
             id_000011_row_pointers = nullptr;
        }
        png_destroy_read_struct(& id_000011_read_ptr, & id_000011_read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O
    png_init_io( id_000011_read_ptr, in_file);
    
    // Set up progressive frame functions
    png_set_progressive_frame_fn( id_000011_read_ptr,  id_000011_frame_info_callback,  id_000011_frame_end_callback);
    
    // Read PNG info
    png_read_info( id_000011_read_ptr,  id_000011_read_info);
    
    // Get image information
     id_000011_width = png_get_image_width( id_000011_read_ptr,  id_000011_read_info);
     id_000011_height = png_get_image_height( id_000011_read_ptr,  id_000011_read_info);
     id_000011_color_type = png_get_color_type( id_000011_read_ptr,  id_000011_read_info);
     id_000011_bit_depth = png_get_bit_depth( id_000011_read_ptr,  id_000011_read_info);
    
    if ( id_000011_width == 0 ||  id_000011_height == 0 ||  id_000011_width > 10000 ||  id_000011_height > 10000) {
        png_destroy_read_struct(& id_000011_read_ptr, & id_000011_read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Transformations
    if ( id_000011_color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb( id_000011_read_ptr);
    if ( id_000011_color_type == PNG_COLOR_TYPE_GRAY &&  id_000011_bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8( id_000011_read_ptr);
    if (png_get_valid( id_000011_read_ptr,  id_000011_read_info, fuzz_uint32_t_2))
        png_set_tRNS_to_alpha( id_000011_read_ptr);
    if ( id_000011_bit_depth == 16)
        png_set_strip_16( id_000011_read_ptr);
    if ( id_000011_color_type == PNG_COLOR_TYPE_GRAY ||  id_000011_color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb( id_000011_read_ptr);
    
    // Apply filler transformation
    if (!( id_000011_color_type & PNG_COLOR_MASK_ALPHA)) {
        png_set_filler( id_000011_read_ptr, fuzz_uint32_t_3, fuzz_int32_t_4);
    }
    
    // Update info after transformations
    png_read_update_info( id_000011_read_ptr,  id_000011_read_info);
    
    // Allocate memory for image
     id_000011_row_pointers = (png_bytep*)malloc(sizeof(png_bytep) *  id_000011_height);
    if (! id_000011_row_pointers) {
        png_destroy_read_struct(& id_000011_read_ptr, & id_000011_read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_size_t rowbytes = png_get_rowbytes( id_000011_read_ptr,  id_000011_read_info);
    for (png_uint_32 y = 0; y <  id_000011_height; y++) {
         id_000011_row_pointers[y] = (png_byte*)malloc(rowbytes);
        if (! id_000011_row_pointers[y]) {
            for (png_uint_32 i = 0; i < y; i++) {
                free( id_000011_row_pointers[i]);
            }
            free( id_000011_row_pointers);
            png_destroy_read_struct(& id_000011_read_ptr, & id_000011_read_info, nullptr);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // Read image data
    png_read_image( id_000011_read_ptr,  id_000011_row_pointers);
    
    // Read end
    png_read_end( id_000011_read_ptr, nullptr);
    
    // Now create write structures
    png_structp write_ptr = png_create_write_struct(fuzz_str_5, nullptr, nullptr, nullptr);
    if (!write_ptr) {
        for (png_uint_32 y = 0; y <  id_000011_height; y++) {
            free( id_000011_row_pointers[y]);
        }
        free( id_000011_row_pointers);
        png_destroy_read_struct(& id_000011_read_ptr, & id_000011_read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        for (png_uint_32 y = 0; y <  id_000011_height; y++) {
            free( id_000011_row_pointers[y]);
        }
        free( id_000011_row_pointers);
        png_destroy_read_struct(& id_000011_read_ptr, & id_000011_read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&write_ptr, &write_info);
        for (png_uint_32 y = 0; y <  id_000011_height; y++) {
            free( id_000011_row_pointers[y]);
        }
        free( id_000011_row_pointers);
        png_destroy_read_struct(& id_000011_read_ptr, & id_000011_read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    FILE* out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        png_destroy_write_struct(&write_ptr, &write_info);
        for (png_uint_32 y = 0; y <  id_000011_height; y++) {
            free( id_000011_row_pointers[y]);
        }
        free( id_000011_row_pointers);
        png_destroy_read_struct(& id_000011_read_ptr, & id_000011_read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_init_io(write_ptr, out_file);
    
    // Set up write info
    int write_color_type = ( id_000011_color_type & PNG_COLOR_MASK_ALPHA) ? PNG_COLOR_TYPE_RGB_ALPHA : PNG_COLOR_TYPE_RGB;
    png_set_IHDR(write_ptr, write_info,  id_000011_width,  id_000011_height, fuzz_int32_t_6, write_color_type,
                 fuzz_int32_t_7, fuzz_int32_t_8, fuzz_int32_t_9);
    
    // Set gamma (1.0 = linear)
    png_set_gAMA_fixed(write_ptr, write_info, fuzz_int32_t_10);  // 1.0 in fixed point
    
    // Set mastering display color volume (example values)
    png_set_mDCV(write_ptr, write_info,
                 0.3127, 0.3290,  // White point
                 0.640, 0.330,    // Red primary
                 0.300, 0.600,    // Green primary
                 0.150, 0.060,    // Blue primary
                 1000.0, 0.001);  // Max and min luminance
    
    // Set write user transform function
    png_set_write_user_transform_fn(write_ptr,  id_000011_write_user_transform_fn);
    
    // Write info
    png_write_info(write_ptr, write_info);
    
    // Write image data
    png_write_image(write_ptr,  id_000011_row_pointers);
    
    // Write end
    png_write_end(write_ptr, nullptr);
    
    // Now use png_image_write_to_file
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.width =  id_000011_width;
    image.height =  id_000011_height;
    image.format = PNG_FORMAT_RGB;
    
    // Prepare buffer for png_image_write_to_file
    size_t buffer_size = PNG_IMAGE_SIZE(image);
    void* buffer = malloc(buffer_size);
    if (buffer) {
        // Copy data from  id_000011_row_pointers to buffer
        png_bytep dst = (png_bytep)buffer;
        int channels = 3;  // RGB
        for (png_uint_32 y = 0; y <  id_000011_height; y++) {
            png_bytep src =  id_000011_row_pointers[y];
            for (png_uint_32 x = 0; x <  id_000011_width; x++) {
                for (int c = 0; c < channels; c++) {
                    dst[y *  id_000011_width * channels + x * channels + c] = src[x * 4 + c];
                }
            }
        }
        
        // Write using png_image_write_to_file
        png_image_write_to_file(&image, fuzz_str_11, fuzz_int32_t_12, buffer,  id_000011_width * channels, nullptr);
        
        free(buffer);
    }
    
    // Clean up
    assert_file_closed(&out_file);;
    png_destroy_write_struct(&write_ptr, &write_info);
    
    for (png_uint_32 y = 0; y <  id_000011_height; y++) {
        free( id_000011_row_pointers[y]);
    }
    free( id_000011_row_pointers);
     id_000011_row_pointers = nullptr;
    
    png_destroy_read_struct(& id_000011_read_ptr, & id_000011_read_info, nullptr);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}