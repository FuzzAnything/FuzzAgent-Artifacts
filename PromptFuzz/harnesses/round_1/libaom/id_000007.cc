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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 0
//<Quality> {"density":25,"unique_branches":{},"library_calls":["aom_codec_build_config","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error_detail","aom_codec_destroy","aom_codec_get_frame","aom_codec_destroy","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_destroy","aom_codec_enc_config_set","aom_codec_control","aom_codec_control","aom_codec_destroy","aom_codec_destroy","aom_img_wrap","aom_codec_destroy","aom_codec_destroy","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_build_config","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_control","aom_codec_control","aom_img_wrap","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


// libaom headers
extern "C" {
}

// Event: Decode an AV1 bitstream, then re-encode the decoded frame
extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Use aom_codec_build_config() - informational API
    const char* build_config = aom_codec_build_config();
    (void)build_config; // Prevent unused variable warning

    // 2. Create decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, 
                               AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }

    // 3. Decode the input bitstream using aom_codec_decode()
    aom_codec_err_t decode_result = aom_codec_decode(&decoder, data, size, nullptr);
    
    // 4. Use aom_codec_error_detail() if decoding failed
    if (decode_result != AOM_CODEC_OK) {
        const char* error_detail = aom_codec_error_detail(&decoder);
        (void)error_detail; // Prevent unused variable warning
        aom_codec_destroy(&decoder);
        return 0;
    }

    // 5. Get the decoded frame
    aom_codec_iter_t iter = nullptr;
    aom_image_t* decoded_img = aom_codec_get_frame(&decoder, &iter);
    if (!decoded_img) {
        aom_codec_destroy(&decoder);
        return 0;
    }

    // 6. Use aom_img_plane_height() - get plane height
    int plane_height = aom_img_plane_height(decoded_img, 0);
    (void)plane_height; // Prevent unused variable warning

    // 7. Create encoder context
    aom_codec_ctx_t encoder;
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    // Get default encoder configuration
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }

    // Set encoder parameters
    enc_cfg.g_w = decoded_img->d_w;
    enc_cfg.g_h = decoded_img->d_h;
    enc_cfg.g_threads = 1;
    enc_cfg.rc_target_bitrate = 1000; // Arbitrary bitrate
    enc_cfg.g_error_resilient = 0;
    enc_cfg.g_pass = AOM_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    enc_cfg.rc_end_usage = AOM_CBR;

    // 8. Initialize encoder with aom_codec_enc_init_ver()
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, 
                                AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }

    // 9. Use aom_codec_enc_config_set() to update configuration
    // Modify a setting and update
    enc_cfg.rc_target_bitrate = 2000;
    aom_codec_enc_config_set(&encoder, &enc_cfg);

    // 10. Use aom_codec_control() to set encoder controls
    aom_codec_control(&encoder, AOME_SET_CPUUSED, 6); // Set CPU usage
    aom_codec_control(&encoder, AOME_SET_ENABLEAUTOALTREF, 0); // Disable altref

    // 11. Prepare image for encoding using aom_img_wrap()
    // We'll create a copy of the decoded image
    unsigned int d_w = decoded_img->d_w;
    unsigned int d_h = decoded_img->d_h;
    
    // Allocate buffer for image data
    size_t buffer_size = d_w * d_h * 3 / 2; // Conservative estimate for YUV420
    unsigned char* img_buffer = (unsigned char*)malloc(buffer_size);
    if (!img_buffer) {
        aom_codec_destroy(&encoder);
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Copy image data (simplified - actual copying would need plane-by-plane)
    memcpy(img_buffer, decoded_img->img_data, 
           std::min(buffer_size, decoded_img->sz));
    
    // 12. Use aom_img_wrap() to create image for encoder
    aom_image_t wrapped_img;
    aom_image_t* wrapped_image = aom_img_wrap(&wrapped_img, decoded_img->fmt, 
                                             d_w, d_h, 1, img_buffer);
    if (!wrapped_image) {
        free(img_buffer);
        aom_codec_destroy(&encoder);
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Copy other image properties
    wrapped_image->range = decoded_img->range;
    wrapped_image->monochrome = decoded_img->monochrome;

    // 13. Encode the frame
    aom_codec_encode(&encoder, wrapped_image, 0, 1, 0);

    // 14. Get encoded data
    aom_codec_iter_t enc_iter = nullptr;
    const aom_codec_cx_pkt_t* pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Successfully encoded frame
            break;
        }
    }

    // 15. Clean up
    free(img_buffer);
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);

    return 0;
}