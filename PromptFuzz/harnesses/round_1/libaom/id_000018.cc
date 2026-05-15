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
//<ID> 29
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_peek_stream_info","aom_uleb_decode","aom_uleb_encode_fixed_size","aom_img_alloc_with_border","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_img_free","aom_codec_set_option","aom_codec_decode","aom_codec_get_stream_info","aom_img_alloc","aom_codec_get_frame","aom_img_add_metadata","aom_img_flip","aom_img_add_metadata","aom_img_free","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_peek_stream_info","aom_uleb_decode","aom_uleb_encode_fixed_size","aom_img_alloc_with_border","aom_codec_dec_init_ver","aom_codec_set_option","aom_codec_decode","aom_codec_get_stream_info","aom_img_alloc","aom_codec_get_frame","aom_img_add_metadata","aom_img_flip","aom_img_add_metadata","aom_img_free","aom_img_free","aom_codec_destroy"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data
    
    // Initialize variables
    aom_codec_ctx_t decoder;
    aom_codec_stream_info_t stream_info;
    aom_image_t *image = nullptr;
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t *pkt = nullptr;
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    uint8_t coded_value[16];
    size_t coded_size = 0;
    const char *option_name = "threads";
    char option_value[16];
    
    // Get decoder interface
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
	return 0;
	}
	
    
    // Get codec capabilities
    aom_codec_caps_t caps = aom_codec_get_caps(iface);
    (void)caps; // Mark as used
    
    // Peek stream info
    memset(&stream_info, 0, sizeof(stream_info));
    aom_codec_err_t err = aom_codec_peek_stream_info(iface, data, size, &stream_info);
    
    // Decode ULEB value from input
    if (size > 0) {
        // Use first part for ULEB decoding
        size_t uleb_available = (size > 16) ? 16 : size;
        int decode_result = aom_uleb_decode(data, uleb_available, &uleb_value, &uleb_length);
        (void)decode_result; // Mark as used
    }
    
    // Encode ULEB with fixed size
    if (uleb_value > 0) {
        int encode_result = aom_uleb_encode_fixed_size(
            uleb_value, sizeof(coded_value), 8, coded_value, &coded_size);
        (void)encode_result; // Mark as used
    }
    
    // Allocate image with border (for potential use)
    aom_image_t img_with_border;
    memset(&img_with_border, 0, sizeof(img_with_border));
    aom_image_t *border_img = aom_img_alloc_with_border(
        &img_with_border, AOM_IMG_FMT_I420, 64, 64, 16, 16, 32);
    
    // Initialize decoder
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 640;
    cfg.h = 480;
    cfg.allow_lowbitdepth = 1;
    
    err = aom_codec_dec_init_ver(&decoder, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(err);
        (void)err_str; // Mark as used
        if (border_img) aom_img_free(border_img);
        return 0;
    }
    
    // Set decoder option
    snprintf(option_value, sizeof(option_value), "%d", (int)(uleb_value % 4 + 1));
    err = aom_codec_set_option(&decoder, option_name, option_value);
    
    // Decode the input data
    err = aom_codec_decode(&decoder, data, size, nullptr);
    
    // Get stream info after decoding
    aom_codec_get_stream_info(&decoder, &stream_info);
    
    // Allocate an image (for metadata testing)
    aom_image_t allocated_img;
    memset(&allocated_img, 0, sizeof(allocated_img));
    aom_image_t *test_img = aom_img_alloc(
        &allocated_img, AOM_IMG_FMT_I420, 32, 32, 16);
    
    // Process decoded frames
    while ((image = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Add metadata if we have encoded ULEB data
        if (coded_size > 0 && test_img) {
            aom_img_add_metadata(test_img, 1, coded_value, 
                                (coded_size > 8) ? 8 : coded_size, 
                                AOM_MIF_ANY_FRAME);
        }
        
        // Flip the image
        aom_img_flip(image);
        
        // Try to add metadata to decoded frame as well
        if (coded_size > 0) {
            aom_img_add_metadata(image, 2, coded_value, 
                                (coded_size > 4) ? 4 : coded_size, 
                                AOM_MIF_KEY_FRAME);
        }
    }
    
    // Cleanup
    if (test_img) aom_img_free(test_img);
    if (border_img) aom_img_free(border_img);
    
    err = aom_codec_destroy(&decoder);
    
    return 0;
}