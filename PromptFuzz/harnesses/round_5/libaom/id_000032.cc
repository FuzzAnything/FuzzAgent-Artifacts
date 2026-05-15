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
//<ID> 55
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 8
//<Quality> {"density":13,"unique_branches":{"img_alloc_helper":[[91,5,91,26,0,0,4,1],[99,5,99,28,0,0,4,0],[103,15,103,47,0,0,4,0],[107,5,107,26,0,0,4,1],[114,5,114,12,0,0,4,0],[118,5,118,26,0,0,4,1],[123,5,123,12,0,0,4,0]],"is_valid_img_fmt":[[44,5,44,28,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_alloc","aom_img_alloc_with_border","aom_codec_get_preview_frame","aom_img_set_rect","aom_img_set_rect","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_img_free","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_alloc","aom_img_alloc_with_border","aom_codec_get_preview_frame","aom_img_set_rect","aom_img_set_rect","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_img_free","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    // 1. Create and initialize decoder context
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    
    // Get codec interface name (required API)
    const char* iface_name = aom_codec_iface_name(decoder_iface);
    (void)iface_name; // Mark as used
    
    // Get codec capabilities (required API)
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    (void)caps; // Mark as used
    
    // Initialize decoder
    if (aom_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    // 2. Peek stream info first
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    aom_codec_peek_stream_info(decoder_iface, data, size, &si);
    
    // 3. Decode the input data
    aom_codec_decode(&decoder, data, size, nullptr);
    
    // 4. Get frames from decoder
    aom_codec_iter_t iter = nullptr;
    const aom_image_t* decoded_frame = nullptr;
    
    // Allocate images using different methods (required APIs)
    aom_image_t* img1 = nullptr;
    aom_image_t* img2 = nullptr;
    aom_image_t img3;
    
    // Try to get first frame
    decoded_frame = aom_codec_get_frame(&decoder, &iter);
    
    if (decoded_frame) {
        // Allocate image with aom_img_alloc (required API)
        img1 = aom_img_alloc(nullptr, decoded_frame->fmt, 
                            decoded_frame->d_w, decoded_frame->d_h, 16);
        
        // Allocate image with aom_img_alloc_with_border (required API)
        img2 = aom_img_alloc_with_border(nullptr, decoded_frame->fmt,
                                        decoded_frame->d_w, decoded_frame->d_h,
                                        16, 16, 32);
        
        // Try to get preview frame (required API)
        const aom_image_t* preview_frame = aom_codec_get_preview_frame(&decoder);
        (void)preview_frame; // Mark as used
        
        // Copy some data from decoded frame if images were allocated successfully
        if (img1 && img2) {
            // Use aom_img_set_rect (required API)
            unsigned int rect_w = decoded_frame->d_w > 16 ? 16 : decoded_frame->d_w;
            unsigned int rect_h = decoded_frame->d_h > 16 ? 16 : decoded_frame->d_h;
            unsigned int border = 8;
            
            // Set rectangle on img1
            aom_img_set_rect(img1, 0, 0, rect_w, rect_h, border);
            
            // Set rectangle on img2
            if (rect_w > border * 2 && rect_h > border * 2) {
                aom_img_set_rect(img2, border, border, 
                                rect_w - border * 2, rect_h - border * 2, border);
            }
        }
        
        // Create metadata (required API)
        uint32_t metadata_type = 1;
        uint8_t metadata_data[] = {0x01, 0x02, 0x03, 0x04};
        size_t metadata_size = sizeof(metadata_data);
        
        aom_metadata_t* metadata = aom_img_metadata_alloc(
            metadata_type, metadata_data, metadata_size, AOM_MIF_ANY_FRAME);
        
        if (metadata) {
            // Try to add metadata to img1 if it has metadata support
            if (img1) {
                // Note: aom_img_add_metadata is in the provided API list
                aom_img_add_metadata(img1, metadata_type, metadata_data, 
                                    metadata_size, AOM_MIF_ANY_FRAME);
            }
            aom_img_metadata_free(metadata);
        }
    }
    
    // 5. Initialize encoder for testing encoder config
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    
    // Get default encoder configuration (required API)
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
        // Set reasonable values for fuzzing
        enc_cfg.g_w = 64;
        enc_cfg.g_h = 64;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_pass = AOM_RC_ONE_PASS;
        enc_cfg.g_lag_in_frames = 0;
        
        // Initialize encoder (but don't actually encode to avoid timeouts)
        // aom_codec_enc_init(&encoder, encoder_iface, &enc_cfg, 0);
    }
    
    // 6. Cleanup
    if (img1) {
        aom_img_free(img1);
    }
    if (img2) {
        aom_img_free(img2);
    }
    
    aom_codec_destroy(&decoder);
    // aom_codec_destroy(&encoder); // If encoder was initialized
    
    return 0;
}