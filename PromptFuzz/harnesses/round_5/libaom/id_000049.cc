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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_alloc","aom_img_set_rect","aom_img_remove_metadata","aom_img_free","aom_codec_error","aom_uleb_encode_fixed_size","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_alloc","aom_img_set_rect","aom_img_remove_metadata","aom_img_free","aom_codec_error","aom_uleb_encode_fixed_size","aom_codec_destroy"],"visited":0}
/**/


// Include necessary libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    // Call aom_codec_dec_init_ver
    aom_codec_err_t init_result = aom_codec_dec_init_ver(&decoder, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    
    // Get stream information
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // Try to get stream info from the input data
    if (init_result == AOM_CODEC_OK) {
        // Call aom_codec_get_stream_info
        aom_codec_err_t info_result = aom_codec_get_stream_info(&decoder, &stream_info);
        
        // Decode the input data
        aom_codec_decode(&decoder, data, size, NULL);
        
        // Get decoded frame
        aom_codec_iter_t iter = NULL;
        aom_image_t *img = aom_codec_get_frame(&decoder, &iter);
        
        if (img) {
            // Create a new image for testing aom_img_set_rect
            aom_image_t test_img;
            unsigned int align = 32;
            unsigned int test_width = 64;
            unsigned int test_height = 64;
            
            // Allocate test image
            aom_img_alloc(&test_img, AOM_IMG_FMT_I420, test_width, test_height, align);
            
            // Call aom_img_set_rect - set a rectangle in the image
            // Use values from the input data to determine rectangle parameters
            unsigned int x = data[0] % test_width;
            unsigned int y = (data[0] + 1) % test_height;
            unsigned int w = (data[0] + 2) % (test_width - x);
            unsigned int h = (data[0] + 3) % (test_height - y);
            w = w > 0 ? w : 1;
            h = h > 0 ? h : 1;
            
            aom_img_set_rect(&test_img, x, y, w, h, 0);
            
            // Call aom_img_remove_metadata
            aom_img_remove_metadata(&test_img);
            
            // Free the test image
            aom_img_free(&test_img);
        }
        
        // Call aom_codec_error to get error string
        const char *error_str = aom_codec_error(&decoder);
        if (error_str) {
            // Write error string to output file
            fwrite(error_str, strlen(error_str), 1, out_file);
        }
    }
    
    // Test aom_uleb_encode_fixed_size
    // Use a value derived from input data
    uint64_t uleb_value = 0;
    for (size_t i = 0; i < (size < 8 ? size : 8); i++) {
        uleb_value = (uleb_value << 8) | data[i];
    }
    
    uint8_t coded_buffer[16];
    size_t coded_size = 0;
    size_t available = sizeof(coded_buffer);
    size_t pad_to_size = 8; // Pad to 8 bytes
    
    // Call aom_uleb_encode_fixed_size
    int encode_result = aom_uleb_encode_fixed_size(uleb_value, available, pad_to_size, 
                                                    coded_buffer, &coded_size);
    
    if (encode_result == 0 && coded_size > 0) {
        // Write encoded data to output file
        fwrite(coded_buffer, coded_size, 1, out_file);
    }
    
    // Cleanup
    if (init_result == AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
    }
    
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}