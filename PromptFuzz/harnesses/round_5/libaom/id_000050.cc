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
//<ID> 104
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"aom_uleb_encode_fixed_size":[[83,7,83,30,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error","aom_codec_get_stream_info","aom_img_alloc","aom_img_set_rect","aom_img_remove_metadata","aom_img_free","aom_uleb_encode_fixed_size","aom_codec_error","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_img_alloc","aom_img_set_rect","aom_img_remove_metadata","aom_img_free","aom_uleb_encode_fixed_size","aom_codec_error","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	
    
    // Create a decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    // Initialize decoder configuration
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    cfg.w = 256;
    cfg.h = 256;
    cfg.allow_lowbitdepth = 1;
    
    // Initialize decoder
    aom_codec_err_t init_result = aom_codec_dec_init_ver(
        &decoder, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (init_result != AOM_CODEC_OK) {
        // Get error message
        const char *error_msg = aom_codec_error(&decoder);
        (void)error_msg; // Prevent unused variable warning
        return 0;
    }
    
    // Get stream information
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // Try to get stream info from the input data
    if (size > 0) {
        // Use a portion of the data for stream info
        size_t stream_info_size = size > 100 ? 100 : size;
        aom_codec_err_t info_result = aom_codec_get_stream_info(
            &decoder, &stream_info);
        (void)info_result; // Result may fail, but we continue anyway
    }
    
    // Create an image
    aom_image_t image;
    memset(&image, 0, sizeof(image));
    
    // Allocate image with some reasonable dimensions
    aom_image_t *img_ptr = aom_img_alloc(
        &image, AOM_IMG_FMT_I420, 64, 64, 1);
    
    if (img_ptr != nullptr) {
        // Set a rectangle in the image
        // Use some data from input to determine rectangle parameters
        unsigned int x = data[0] % 32;
        unsigned int y = data[1] % 32;
        unsigned int w = 8 + (data[2] % 24);
        unsigned int h = 8 + (data[3] % 24);
        
        if (x + w <= 64 && y + h <= 64) {
            int rect_result = aom_img_set_rect(img_ptr, x, y, w, h, 0);
            (void)rect_result; // Result may be ignored
        }
        
        // Remove metadata from the image
        aom_img_remove_metadata(img_ptr);
        
        // Free the image
        aom_img_free(img_ptr);
    }
    
    // Test ULEB encoding
    if (size >= 20) {
        uint64_t value = 0;
        // Extract a value from input data
        for (int i = 0; i < 8 && i + 10 < (int)size; i++) {
            value = (value << 8) | data[10 + i];
        }
        
        size_t available = size > 30 ? 30 : size - 20;
        if (available > 0) {
            uint8_t coded_buffer[30];
            size_t coded_size = 0;
            size_t pad_to_size = 5 + (data[4] % 10);
            
            int encode_result = aom_uleb_encode_fixed_size(
                value, available, pad_to_size, coded_buffer, &coded_size);
            (void)encode_result; // Result may be ignored
        }
    }
    
    // Get error message (even if no error, to use the API)
    const char *final_error = aom_codec_error(&decoder);
    (void)final_error; // Prevent unused variable warning
    
    // Destroy decoder
    aom_codec_destroy(&decoder);
    
    return 0;
}