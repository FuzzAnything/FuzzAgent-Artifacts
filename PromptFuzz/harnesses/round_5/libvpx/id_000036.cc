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
//<ID> 121
//<Prompt> []
/*<Combination>: [
*/
//<score> 18.5, nr_unique_branch: 0
//<Quality> {"density":37,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_alloc","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000036_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // Empty callback - just to satisfy API requirement
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Step 1: Create decoder interface for VP9
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    if (!decoder_iface) {
        return 0;
    }

    // Step 2: Get decoder capabilities
    vpx_codec_caps_t decoder_caps = vpx_codec_get_caps(decoder_iface);
    (void)decoder_caps; // Use variable to avoid unused warning

    // Step 3: Peek at stream info to get dimensions
    vpx_codec_stream_info_t stream_info;
    stream_info.sz = sizeof(stream_info);
    
    vpx_codec_err_t err = vpx_codec_peek_stream_info(decoder_iface, data, 
                                                     (unsigned int)size, &stream_info);
    if (err != VPX_CODEC_OK) {
        // Input might not be valid VP9 data, but we should still try to proceed
        stream_info.w = 320;
        stream_info.h = 240;
    }

    // Step 4: Initialize decoder
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 4;
    dec_cfg.w = stream_info.w;
    dec_cfg.h = stream_info.h;
    
    err = vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, 
                                0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    // Step 5: Decode the input data
    err = vpx_codec_decode(&decoder_ctx, data, (unsigned int)size, NULL, 0);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }

    // Step 6: Get decoded frame
    vpx_codec_iter_t iter = NULL;
    vpx_image_t* decoded_img = vpx_codec_get_frame(&decoder_ctx, &iter);
    
    if (!decoded_img) {
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }

    // Step 7: Allocate an image for encoding
    vpx_image_t* img_to_encode = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 
                                               decoded_img->d_w, 
                                               decoded_img->d_h, 1);
    if (!img_to_encode) {
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }

    // Step 8: Set rectangle on the image (using part of the image)
    unsigned int rect_w = decoded_img->d_w > 100 ? 100 : decoded_img->d_w;
    unsigned int rect_h = decoded_img->d_h > 100 ? 100 : decoded_img->d_h;
    if (vpx_img_set_rect(img_to_encode, 0, 0, rect_w, rect_h) != 0) {
        // If setting rectangle fails, continue anyway
    }

    // Step 9: Create encoder interface for VP9
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    if (!encoder_iface) {
        vpx_img_free(img_to_encode);
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }

    // Step 10: Initialize first encoder (single encoder version)
    vpx_codec_ctx_t encoder_ctx1;
    vpx_codec_enc_cfg_t enc_cfg1;
    
    err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg1, 0);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img_to_encode);
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    enc_cfg1.g_w = img_to_encode->d_w;
    enc_cfg1.g_h = img_to_encode->d_h;
    enc_cfg1.g_timebase.num = 1;
    enc_cfg1.g_timebase.den = 30;
    enc_cfg1.rc_target_bitrate = 1000;
    
    err = vpx_codec_enc_init_ver(&encoder_ctx1, encoder_iface, &enc_cfg1, 
                                0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img_to_encode);
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }

    // Step 11: Initialize second encoder (multi encoder version)
    vpx_codec_ctx_t encoder_ctx2;
    vpx_codec_enc_cfg_t enc_cfg2;
    
    err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg2, 0);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder_ctx1);
        vpx_img_free(img_to_encode);
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    enc_cfg2.g_w = img_to_encode->d_w;
    enc_cfg2.g_h = img_to_encode->d_h;
    enc_cfg2.g_timebase.num = 1;
    enc_cfg2.g_timebase.den = 30;
    enc_cfg2.rc_target_bitrate = 1000;
    
    vpx_rational_t dsf = {1, 1}; // Default scaling factor
    err = vpx_codec_enc_init_multi_ver(&encoder_ctx2, encoder_iface, &enc_cfg2, 
                                      1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder_ctx1);
        vpx_img_free(img_to_encode);
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }

    // Step 12: Register put frame callback
    err = vpx_codec_register_put_frame_cb(&encoder_ctx1,  id_000036_put_frame_callback, NULL);
    (void)err; // Ignore error for fuzzing

    // Step 13: Encode with first encoder
    err = vpx_codec_encode(&encoder_ctx1, img_to_encode, 0, 1, 0, 0);
    if (err == VPX_CODEC_OK) {
        // Get encoded data
        vpx_codec_iter_t enc_iter1 = NULL;
        const vpx_codec_cx_pkt_t* pkt1;
        while ((pkt1 = vpx_codec_get_cx_data(&encoder_ctx1, &enc_iter1)) != NULL) {
            // Process packet if needed
            (void)pkt1;
        }
    }

    // Step 14: Encode with second encoder
    err = vpx_codec_encode(&encoder_ctx2, img_to_encode, 0, 1, 0, 0);
    if (err == VPX_CODEC_OK) {
        // Get encoded data
        vpx_codec_iter_t enc_iter2 = NULL;
        const vpx_codec_cx_pkt_t* pkt2;
        while ((pkt2 = vpx_codec_get_cx_data(&encoder_ctx2, &enc_iter2)) != NULL) {
            // Process packet if needed
            (void)pkt2;
        }
    }

    // Step 15: Clean up all resources
    vpx_codec_destroy(&encoder_ctx2);
    vpx_codec_destroy(&encoder_ctx1);
    vpx_img_free(img_to_encode);
    vpx_codec_destroy(&decoder_ctx);

    return 0;
}