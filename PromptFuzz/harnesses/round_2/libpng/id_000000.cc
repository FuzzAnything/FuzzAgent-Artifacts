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
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 5
//<Quality> {"density":33,"unique_branches":{"png_create_read_struct_2":[[50,8,50,23,0,0,4,1]],"png_create_png_struct":[[326,14,326,71,0,0,4,1]],"png_user_version_check":[[218,14,218,57,0,0,4,0],[223,13,223,42,0,0,4,1],[229,8,229,57,0,0,4,0]]},"library_calls":["png_get_header_version","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_read_info","png_read_update_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_y_pixels_per_inch","png_set_strip_16","png_get_rowbytes","png_read_image","png_create_write_struct","png_create_info_struct","png_init_io","png_set_longjmp_fn","png_build_grayscale_palette","png_set_IHDR","png_set_PLTE","png_convert_from_struct_tm","png_set_tIME","png_write_info","png_write_frame_head","png_write_image","png_write_frame_tail","png_write_end","png_destroy_write_struct","png_destroy_write_struct","png_read_end","png_destroy_read_struct"],"critical_calls":["png_get_header_version","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_read_info","png_read_update_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_y_pixels_per_inch","png_set_strip_16","png_get_rowbytes","png_read_image","png_create_write_struct","png_create_info_struct","png_init_io","png_set_longjmp_fn","png_build_grayscale_palette","png_set_IHDR","png_set_PLTE","png_convert_from_struct_tm","png_set_tIME","png_write_info","png_write_frame_head","png_write_image","png_write_frame_tail","png_write_end","png_destroy_write_struct","png_read_end","png_destroy_read_struct"],"visited":2}
/*Looking at the APIs provided and the specific APIs mentioned, I'll create a fuzz driver that demonstrates reading and processing a PNG image from memory data, then potentially writing it out with some modifications. The event I'll focus on is reading a PNG image, updating its info, and optionally creating a grayscale palette version.

*/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
        return 0;
    }

    // Check if data starts with PNG signature
    png_byte png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    if (memcmp(data, png_sig, 8) != 0) {
        return 0;
    }

    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get PNG library version
    const char *version = png_get_header_version(NULL);
    
    // Create read structure
    png_structp png_ptr_read = png_create_read_struct(version, NULL, NULL, NULL);
    if (!png_ptr_read) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr_read))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_init_io(png_ptr_read, in_file);
    png_set_sig_bytes(png_ptr_read, 8);
    
    png_read_info(png_ptr_read, info_ptr_read);
    
    // Use png_read_update_info to update info after potential transformations
    png_read_update_info(png_ptr_read, info_ptr_read);
    
    // Get image information
    png_uint_32 width = png_get_image_width(png_ptr_read, info_ptr_read);
    png_uint_32 height = png_get_image_height(png_ptr_read, info_ptr_read);
    png_byte color_type = png_get_color_type(png_ptr_read, info_ptr_read);
    png_byte bit_depth = png_get_bit_depth(png_ptr_read, info_ptr_read);
    
    // Use png_get_y_pixels_per_inch
    png_uint_32 y_pixels_per_inch = png_get_y_pixels_per_inch(png_ptr_read, info_ptr_read);
    
    // Use png_set_strip_16 - strip 16-bit to 8-bit if needed
    if (bit_depth == 16) {
        png_set_strip_16(png_ptr_read);
    }
    
    // Prepare row pointers for reading
    png_bytep *row_pointers = new png_bytep[height];
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = new png_byte[png_get_rowbytes(png_ptr_read, info_ptr_read)];
    }
    
    png_read_image(png_ptr_read, row_pointers);
    
    // Create write structure for potential output
    png_structp png_ptr_write = png_create_write_struct(version, NULL, NULL, NULL);
    if (png_ptr_write) {
        png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
        
        if (info_ptr_write) {
	FUZZ_FILENAME_PUSH("output_file")
            // Create output file
            FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
            if (out_file) {
                png_init_io(png_ptr_write, out_file);
                
                // Set up error handling for write
                if (!setjmp(png_jmpbuf(png_ptr_write))) {
                    // Set image parameters for output
                    int output_bit_depth = (bit_depth == 16) ? 8 : bit_depth;
                    int output_color_type = color_type;
                    
                    // If grayscale image, build grayscale palette
                    png_color palette[256];
                    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
                        png_build_grayscale_palette(output_bit_depth, palette);
                        output_color_type = PNG_COLOR_TYPE_PALETTE;
                    }
                    
                    png_set_IHDR(png_ptr_write, info_ptr_write, width, height,
                                output_bit_depth, output_color_type, PNG_INTERLACE_NONE,
                                PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
                    
                    // If we built a palette, set it
                    if (output_color_type == PNG_COLOR_TYPE_PALETTE) {
                        png_set_PLTE(png_ptr_write, info_ptr_write, palette, 
                                    (1 << output_bit_depth));
                    }
                    
                    // Use png_convert_from_struct_tm for time stamp
                    time_t now = time(NULL);
                    struct tm *time_struct = localtime(&now);
                    png_time ptime;
                    png_convert_from_struct_tm(&ptime, time_struct);
                    png_set_tIME(png_ptr_write, info_ptr_write, &ptime);
                    
                    png_write_info(png_ptr_write, info_ptr_write);
                    
                    // Use png_write_frame_head (for animation, though we have single frame)
                    // This is a bit of a stretch since we don't have animation,
                    // but we call it with our single frame
                    png_write_frame_head(png_ptr_write, info_ptr_write, row_pointers,
                                        width, height, 0, 0, 100, 100, 0, 0);
                    
                    png_write_image(png_ptr_write, row_pointers);
                    png_write_frame_tail(png_ptr_write, info_ptr_write);
                    png_write_end(png_ptr_write, NULL);
                }
                
                assert_file_closed(&out_file);;
            }
            
            png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        } else {
            png_destroy_write_struct(&png_ptr_write, NULL);
        }
    }
    
    // Clean up read resources
    png_read_end(png_ptr_read, NULL);
    
    // Clean up row pointers
    for (png_uint_32 y = 0; y < height; y++) {
        delete[] row_pointers[y];
    }
    delete[] row_pointers;
    
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);
    assert_file_closed(&in_file);;
    
    // Remove output file if created
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}