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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["aom_codec_build_config","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_decode","aom_codec_error_detail","aom_codec_get_frame","aom_img_plane_height","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_enc_config_set","aom_codec_error_detail","aom_codec_control","aom_codec_control","aom_img_wrap","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_build_config","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_decode","aom_codec_error_detail","aom_codec_get_frame","aom_img_plane_height","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_enc_config_set","aom_codec_error_detail","aom_codec_control","aom_codec_control","aom_img_wrap","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    // 1. Create decoder context
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    // Get build config (just for info)
    const char *build_config = aom_codec_build_config();
    (void)build_config; // Prevent unused variable warning
    
    // Initialize decoder
    if (aom_codec_dec_init_ver(&decoder, aom_codec_av1_dx(), &dec_cfg, 0, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // 2. Decode the input data
    aom_codec_err_t decode_result = aom_codec_decode(&decoder, data, size, nullptr);
    
    // Get error details if decoding failed
    if (decode_result != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&decoder);
        (void)error_detail; // Prevent unused variable warning
    }
    
    // 3. Get decoded frame
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_img = aom_codec_get_frame(&decoder, &iter);
    
    if (decoded_img) {
        // 4. Get plane height (just to use the API)
        int plane_height = aom_img_plane_height(decoded_img, 0);
        (void)plane_height; // Prevent unused variable warning
        
        // 5. Create encoder context
        aom_codec_ctx_t encoder;
        aom_codec_enc_cfg_t enc_cfg;
        
        // Get default encoder config
        if (aom_codec_enc_config_default(aom_codec_av1_cx(), &enc_cfg, 0) != AOM_CODEC_OK) {
            aom_codec_destroy(&decoder);
            return 0;
        }
        
        // Set encoder configuration
        enc_cfg.g_w = decoded_img->d_w;
        enc_cfg.g_h = decoded_img->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_pass = AOM_RC_ONE_PASS;
        enc_cfg.g_lag_in_frames = 0;
        
        // Initialize encoder
        if (aom_codec_enc_init_ver(&encoder, aom_codec_av1_cx(), &enc_cfg, 0, AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
            
            // 6. Set encoder config
            if (aom_codec_enc_config_set(&encoder, &enc_cfg) != AOM_CODEC_OK) {
                // Get error details
                const char *enc_error_detail = aom_codec_error_detail(&encoder);
                (void)enc_error_detail; // Prevent unused variable warning
            }
            
            // 7. Use codec control API (set quality)
            aom_codec_control(&encoder, AOME_SET_CPUUSED, 6);
            aom_codec_control(&encoder, AOME_SET_CQ_LEVEL, 32);
            
            // 8. Create output image wrapper for encoding
            size_t img_data_size = decoded_img->stride[0] * decoded_img->d_h;
            if (decoded_img->fmt & AOM_IMG_FMT_HIGHBITDEPTH) {
                img_data_size *= 2;
            }
            
            unsigned char *img_data = (unsigned char *)malloc(img_data_size);
            if (img_data) {
                // Copy Y plane data
                memcpy(img_data, decoded_img->planes[0], decoded_img->stride[0] * decoded_img->d_h);
                
                // Wrap image using aom_img_wrap
                aom_image_t wrapped_img;
                aom_image_t *wrapped = aom_img_wrap(&wrapped_img, decoded_img->fmt, 
                                                    decoded_img->d_w, decoded_img->d_h,
                                                    1, img_data);
                
                if (wrapped) {
                    // Copy other parameters
                    wrapped->range = decoded_img->range;
                    wrapped->bit_depth = decoded_img->bit_depth;
                    
                    // Encode the wrapped image
                    aom_codec_encode(&encoder, wrapped, 0, 1, 0);
                    
                    // Get encoded data
                    aom_codec_iter_t enc_iter = nullptr;
                    const aom_codec_cx_pkt_t *pkt;
                    while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                            // Frame data available
                            break;
                        }
                    }
                }
                
                free(img_data);
            }
            
            aom_codec_destroy(&encoder);
        }
    }
    
    // Cleanup decoder
    aom_codec_destroy(&decoder);
    
    return 0;
}