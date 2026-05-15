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
//<ID> 154
//<Prompt> []
/*<Combination>: [
*/
//<score> 35, nr_unique_branch: 22
//<Quality> {"density":14,"unique_branches":{"png_get_pHYs":[[1094,8,1094,23,0,0,4,0],[1094,27,1094,43,0,0,4,0],[1095,8,1095,46,0,0,4,0],[1095,8,1095,46,0,0,4,1],[1097,11,1097,24,0,0,4,0],[1103,11,1103,24,0,0,4,0],[1109,11,1109,28,0,0,4,0]],"png_set_sRGB_gAMA_and_cHRM":[[847,8,847,23,0,0,4,1],[847,27,847,43,0,0,4,1]],"png_get_pCAL":[[1000,8,1000,23,0,0,4,0],[1000,27,1000,43,0,0,4,0],[1001,8,1001,46,0,0,4,0],[1001,8,1001,46,0,0,4,1],[1002,8,1002,23,0,0,4,0],[1002,27,1002,37,0,0,4,0],[1002,41,1002,51,0,0,4,0],[1002,55,1002,67,0,0,4,0],[1003,8,1003,23,0,0,4,0],[1003,27,1003,40,0,0,4,0],[1003,44,1003,58,0,0,4,0]],"png_set_packswap":[[73,8,73,23,0,0,4,1],[76,8,76,30,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_set_packswap","png_set_sRGB_gAMA_and_cHRM","png_get_pHYs","png_get_pCAL","png_set_eXIf_1","png_write_chunk_start","png_write_chunk_data","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_packswap","png_set_sRGB_gAMA_and_cHRM","png_get_pHYs","png_get_pCAL","png_set_eXIf_1","png_write_chunk_start","png_write_chunk_data","png_destroy_write_struct","png_destroy_read_struct"],"visited":1}
/**/


extern "C" {
}

// Custom type declarations from libpng
struct png_struct_def;
typedef struct png_struct_def png_struct;
typedef unsigned char png_byte;
typedef FILE * png_FILE_p;
typedef int png_int_32;
typedef unsigned int png_uint_32;
struct png_info_def;
typedef struct png_info_def png_info;

// Function prototypes for the required APIs
extern "C" {
// Note: png_get_pHYs_dpi doesn't exist in provided APIs, using png_get_pHYs instead
png_uint_32 png_get_pHYs(const png_struct * png_ptr, const png_info * info_ptr, 
                         png_uint_32 * res_x, png_uint_32 * res_y, int * unit_type);
png_uint_32 png_get_pCAL(const png_struct * png_ptr, png_info * info_ptr,
                         char ** purpose, png_int_32 * X0, png_int_32 * X1,
                         int * type, int * nparams, char ** units, char *** params);
void png_write_chunk_data(png_struct * png_ptr, const png_byte * data, size_t length);
void png_set_packswap(png_struct * png_ptr);
void png_set_sRGB_gAMA_and_cHRM(const png_struct * png_ptr, png_info * info_ptr, int srgb_intent);
// Note: png_get_palette_max doesn't exist in provided APIs, skipping this call
// Note: png_get_user_width_max doesn't exist in provided APIs, skipping this call
void png_set_eXIf_1(const png_struct * png_ptr, png_info * info_ptr, 
                    png_uint_32 num_exif, png_byte * exif);
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for processing
    
    // Create FILE pointers for input and output
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	FUZZ_FILENAME_PUSH("output_file")
	
    
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_struct *png_ptr = nullptr;
    png_info *info_ptr = nullptr;
    png_info *end_info = nullptr;
    
    // Initialize PNG structures for reading
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O
    png_init_io(png_ptr, in_file);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Create write structure
    png_struct *write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!write_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_info *write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize write I/O
    png_init_io(write_ptr, out_file);
    
    // 1. Use png_set_packswap - set byte swapping for packed pixel data
    png_set_packswap(write_ptr);
    
    // 2. Use png_set_sRGB_gAMA_and_cHRM - set sRGB color space
    png_set_sRGB_gAMA_and_cHRM(write_ptr, write_info, PNG_sRGB_INTENT_PERCEPTUAL);
    
    // 3. Use png_get_pHYs to get physical dimensions
    png_uint_32 res_x = 0, res_y = 0;
    int unit_type = 0;
    png_get_pHYs(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    
    // 4. Use png_get_pCAL to get pCAL chunk data if present
    char *purpose = nullptr;
    png_int_32 X0 = 0, X1 = 0;
    int type = 0, nparams = 0;
    char *units = nullptr;
    char **params = nullptr;
    
    png_get_pCAL(png_ptr, info_ptr, &purpose, &X0, &X1, &type, &nparams, &units, &params);
    
    // 5. Use png_set_eXIf_1 to set EXIF data
    // Prepare some EXIF-like data from input
    png_uint_32 num_exif = size > 100 ? 100 : size;
    png_byte *exif_data = (png_byte *)malloc(num_exif);
    if (exif_data) {
        memcpy(exif_data, data, num_exif);
        png_set_eXIf_1(write_ptr, write_info, num_exif, exif_data);
        free(exif_data);
    }
    
    // 6. Use png_write_chunk_data to write a custom chunk
    // Write a custom tEXt chunk
    const png_byte chunk_name[] = {'t', 'E', 'X', 't'};
    const char *text_data = "FuzzerGenerated";
    png_write_chunk_start(write_ptr, chunk_name, strlen(text_data) + 1);
    png_write_chunk_data(write_ptr, (const png_byte *)text_data, strlen(text_data) + 1);
    
    // Clean up
    png_destroy_write_struct(&write_ptr, &write_info);
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}