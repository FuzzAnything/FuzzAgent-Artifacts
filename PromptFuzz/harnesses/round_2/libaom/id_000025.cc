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
#include <aom/aom.h>
}
extern "C" {
#include <aom/aom_codec.h>
}
extern "C" {
#include <aom/aom_decoder.h>
}
extern "C" {
#include <aom/aom_encoder.h>
}
extern "C" {
#include <aom/aom_ext_ratectrl.h>
}
extern "C" {
#include <aom/aom_external_partition.h>
}
extern "C" {
#include <aom/aom_frame_buffer.h>
}
extern "C" {
#include <aom/aom_image.h>
}
extern "C" {
#include <aom/aomcx.h>
}
extern "C" {
#include <aom/aomdx.h>
}
//<ID> 57
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 3
//<Quality> {"density":6,"unique_branches":{"aom_codec_peek_stream_info":[[62,26,62,34,0,0,4,0]],"aom_codec_err_to_string":[[37,11,37,14,0,0,4,0]],"img_alloc_helper":[[84,7,84,18,0,0,4,0]]},"library_calls":["aom_uleb_decode","aom_codec_err_to_string","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_peek_stream_info","aom_codec_err_to_string","aom_img_alloc_with_border","aom_img_alloc_with_border","aom_img_plane_width","aom_img_plane_height","aom_img_free"],"critical_calls":["aom_uleb_decode","aom_codec_err_to_string","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_peek_stream_info","aom_codec_err_to_string","aom_img_alloc_with_border","aom_img_alloc_with_border","aom_img_plane_width","aom_img_plane_height","aom_img_free"],"visited":1}
/*To create a fuzzer that processes libaom output data, we'll design a program that:

1. Decodes ULEB values from the input to determine image parameters
2. Uses stream information to understand video properties
3. Allocates images with borders based on decoded parameters
4. Checks decoder capabilities and handles error cases

Here's the complete implementation:

*/


// Include libaom headers
extern "C" {
}

// The main fuzzer entry point
extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (!data || size == 0) {
        return 0;
    }

    // Variables to track allocated resources
    aom_image_t *img = nullptr;
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    
    // 1. Decode a ULEB value from the input data
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    const uint8_t *current_data = data;
    size_t remaining_size = size;
    
    int uleb_result = aom_uleb_decode(current_data, remaining_size, &uleb_value, &uleb_length);
    
    // Get error string for ULEB decoding result
    const char *uleb_error_str = aom_codec_err_to_string(static_cast<aom_codec_err_t>(uleb_result));
    
    // Update position in the data
    if (uleb_result == 0) { // Success
        current_data += uleb_length;
        remaining_size -= uleb_length;
    }
    
    // 2. Create a FILE* to read the input data
    in_file = fmemopen((void *)current_data, remaining_size, "rb");
    if (!in_file) {
        // Clean up and return
        if (uleb_error_str) {
            // Error string is a constant, no need to free
        }
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create an output file for writing (not used for fuzzing, just to demonstrate)
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Get AV1 decoder capabilities
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    
    // 4. Try to peek at stream information from the remaining data
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // We'll use a portion of the data for stream info peeking
    size_t peek_size = remaining_size > 100 ? 100 : remaining_size;
    int peek_result = aom_codec_peek_stream_info(decoder_iface, current_data, peek_size, &stream_info);
    
    // Get error string for peek result
    const char *peek_error_str = aom_codec_err_to_string(static_cast<aom_codec_err_t>(peek_result));
    
    // 5. Allocate an image with border using decoded ULEB value as parameters
    // Use sane defaults with the ULEB value influencing dimensions
    unsigned int width = 64 + (uleb_value % 256);  // Use ULEB value to influence width
    unsigned int height = 64 + ((uleb_value >> 8) % 256);  // Use ULEB value to influence height
    
    // Ensure minimum dimensions
    if (width < 16) width = 16;
    if (height < 16) height = 16;
    
    // Cap dimensions to reasonable values for fuzzing
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    
    // Use ULEB value to determine border (limit to reasonable size)
    unsigned int border = static_cast<unsigned int>(uleb_value % 64);
    
    // Allocate image with border
    img = aom_img_alloc_with_border(nullptr, 
                                   AOM_IMG_FMT_I420, 
                                   width, 
                                   height, 
                                   1,  // align
                                   0,  // size_align
                                   border);
    
    // If allocation failed, try without border
    if (!img) {
        img = aom_img_alloc_with_border(nullptr,
                                       AOM_IMG_FMT_I420,
                                       width,
                                       height,
                                       1,
                                       0,
                                       0);
    }
    
    // 6. If we have an image, fill it with some data from input
    if (img && remaining_size > 0) {
        // Read some data from the input file into image planes
        int file_descriptor = fuzz_fileno(in_file);
        
        // Try to fill each plane with some data
        for (int plane = 0; plane < 3; plane++) {
            int plane_width = aom_img_plane_width(img, plane);
            int plane_height = aom_img_plane_height(img, plane);
            size_t plane_size = plane_width * plane_height;
            
            if (plane_size > 0 && img->planes[plane]) {
                // Limit how much we read
                size_t read_size = plane_size > remaining_size ? remaining_size : plane_size;
                if (read_size > 0) {
                    // Move file pointer back to start for reading
                    fseek(in_file, 0, SEEK_SET);
                    fread(img->planes[plane], 1, read_size, in_file);
                }
            }
        }
        
        // Write a small amount to output file to demonstrate file operations
        if (out_file) {
            // Write image dimensions
            fwrite(&width, sizeof(width), 1, out_file);
            fwrite(&height, sizeof(height), 1, out_file);
            
            // Write a small portion of image data if available
            if (img->planes[0] && img->stride[0] > 0 && height > 0) {
                size_t write_size = img->stride[0] > 64 ? 64 : img->stride[0];
                fwrite(img->planes[0], 1, write_size, out_file);
            }
        }
    }
    
    // 7. Use error strings (even if just to prevent them from being optimized away)
    if (uleb_error_str) {
        // Do nothing, just reference to prevent optimization
        volatile const char *temp = uleb_error_str;
        (void)temp;
    }
    
    if (peek_error_str) {
        // Do nothing, just reference to prevent optimization
        volatile const char *temp = peek_error_str;
        (void)temp;
    }
    
    // 8. Clean up all allocated resources
    if (img) {
        aom_img_free(img);
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
        // Remove the output file since we don't need it after fuzzing
        remove("output_file");
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}