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
#include <vpx/vp8cx.h>
}
extern "C" {
#include <vpx/vp8dx.h>
}
extern "C" {
#include <vpx/vpx_decoder.h>
}
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"vp8_mb_init_dequantizer":[[75,31,75,47,0,0,4,1]]},"library_calls":["vpx_codec_version","vpx_codec_build_config","vpx_codec_vp8_dx","vpx_codec_get_caps","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_dec_init_ver","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_decode","vpx_codec_get_frame","vpx_img_flip","vpx_img_wrap","vpx_codec_encode","vpx_codec_control_","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_version","vpx_codec_build_config","vpx_codec_vp8_dx","vpx_codec_get_caps","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_dec_init_ver","vpx_codec_enc_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_img_flip","vpx_img_wrap","vpx_codec_encode","vpx_codec_control_","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Call these APIs at least once as required
    int version = vpx_codec_version();
    const char* build_config = vpx_codec_build_config();
    
    // Create a decoder interface and get its capabilities
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp8_dx();
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    
    // Create an encoder interface and get default config
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx();
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    
    // Create a codec context for decoder
    vpx_codec_ctx_t decoder_ctx;
    
    // Initialize decoder
    if (vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, NULL, 0, VPX_DECODER_ABI_VERSION) 
        != VPX_CODEC_OK) {
        return 0;
    }
    
    // Create a codec context for encoder
    vpx_codec_ctx_t encoder_ctx;
    
    // Initialize encoder with default config
    if (vpx_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION) 
        != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    // Try to decode the input data (encoded video stream)
    if (size > 0) {
        vpx_codec_decode(&decoder_ctx, data, size, NULL, 0);
    }
    
    // Get the decoded frame
    vpx_codec_iter_t iter = NULL;
    vpx_image_t* decoded_img = vpx_codec_get_frame(&decoder_ctx, &iter);
    
    // If we got a decoded frame, process it
    if (decoded_img) {
        // Create a flipped version of the image
        vpx_img_flip(decoded_img);
        
        // Wrap the flipped image data in a new vpx_image_t structure
        unsigned int stride_align = 1;
        vpx_image_t wrapped_img;
        
        // Only wrap if we have valid dimensions
        if (decoded_img->d_w > 0 && decoded_img->d_h > 0) {
            vpx_img_wrap(&wrapped_img, decoded_img->fmt, 
                        decoded_img->d_w, decoded_img->d_h, 
                        stride_align, decoded_img->img_data);
            
            // Try to encode the flipped image
            vpx_codec_encode(&encoder_ctx, &wrapped_img, 0, 1, 0, 0);
            
            // Try to use control API with various control IDs
            // We'll try a few common control IDs that might work
            const int control_ids[] = {VP8E_SET_CPUUSED, VP8E_SET_NOISE_SENSITIVITY, 
                                      VP8E_SET_SHARPNESS, VP8E_SET_TOKEN_PARTITIONS};
            
            for (int i = 0; i < 4; i++) {
                // Try to set the control parameter (ignore errors in fuzzing)
                vpx_codec_control_(&encoder_ctx, control_ids[i]);
            }
            
            // Get encoded data
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(&encoder_ctx, &iter)) != NULL) {
                // Do nothing with the packet in fuzzing, just consume it
            }
        }
    }
    
    // Clean up
    vpx_codec_destroy(&encoder_ctx);
    vpx_codec_destroy(&decoder_ctx);
    
    return 0;
}