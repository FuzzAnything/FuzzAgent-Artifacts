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
//<ID> 280
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 0
//<Quality> {"density":30,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_register_put_frame_cb","vpx_codec_destroy","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_stream_info","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_codec_control_","vpx_codec_control_","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_codec_control_","vpx_codec_control_","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000064_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Do nothing in fuzzing context
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* f_data, size_t f_size) {
	if(f_size<80) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize decoder interface
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    if (!decoder_iface) {
	return 0;
	}
	
    
    // Get decoder interface name
    const char *iface_name = vpx_codec_iface_name(decoder_iface);
    if (!iface_name) {
	return 0;
	}
	
    
    // Peek stream info
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    vpx_codec_err_t err = vpx_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    if (err != VPX_CODEC_OK) {
	return 0;
	}
	
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    
    err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(err);
        const char *err_detail = vpx_codec_error_detail(&decoder);
        (void)err_str;
        (void)err_detail;
        return 0;
    }
    
    // Register put frame callback
    err = vpx_codec_register_put_frame_cb(&decoder,  id_000064_put_frame_callback, nullptr);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Decode the input data
    err = vpx_codec_decode(&decoder, data, size, nullptr, fuzz_int64_t_2);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Get stream info after decoding
    err = vpx_codec_get_stream_info(&decoder, &stream_info);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Get decoded frames
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *decoded_img = nullptr;
    int frame_count = 0;
    
    while ((decoded_img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        frame_count++;
        
        // Now encode the decoded frame
        vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
        if (!encoder_iface) break;
        
        // Get default encoder config
        vpx_codec_enc_cfg_t enc_cfg;
        err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_3);
        if (err != VPX_CODEC_OK) break;
        
        // Set encoder dimensions from decoded image
        enc_cfg.g_w = decoded_img->d_w;
        enc_cfg.g_h = decoded_img->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_pass = VPX_RC_ONE_PASS;
        enc_cfg.g_lag_in_frames = 0;
        
        // Initialize encoder
        vpx_codec_ctx_t encoder;
        err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_4, VPX_ENCODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            const char *err_str = vpx_codec_err_to_string(err);
            const char *err_detail = vpx_codec_error_detail(&encoder);
            (void)err_str;
            (void)err_detail;
            break;
        }
        
        // Also try multi-encoder init (with single encoder)
        vpx_codec_ctx_t encoder_multi;
        vpx_rational_t dsf = {1, 1};
        err = vpx_codec_enc_init_multi_ver(&encoder_multi, encoder_iface, &enc_cfg, 1, fuzz_int64_t_5, &dsf, VPX_ENCODER_ABI_VERSION);
        if (err == VPX_CODEC_OK) {
            vpx_codec_destroy(&encoder_multi);
        }
        
        // Set encoder config
        err = vpx_codec_enc_config_set(&encoder, &enc_cfg);
        if (err != VPX_CODEC_OK) {
            vpx_codec_destroy(&encoder);
            break;
        }
        
        // Try some control commands
        vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 8);
        vpx_codec_control_(&encoder, VP8E_SET_ENABLEAUTOALTREF, 0);
        
        // Allocate an image for encoding
        vpx_image_t *enc_img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                            decoded_img->d_w, decoded_img->d_h, fuzz_uint32_t_6);
        if (enc_img) {
            // Set a rectangle in the image
            vpx_img_set_rect(enc_img, fuzz_uint32_t_7, fuzz_uint32_t_8, 
                           decoded_img->d_w / 2, decoded_img->d_h / 2);
            
            // Encode the image
            err = vpx_codec_encode(&encoder, enc_img, fuzz_int64_t_9, fuzz_uint64_t_10, fuzz_int64_t_11, fuzz_uint64_t_12);
            
            vpx_img_free(enc_img);
        }
        
        // Destroy encoder
        vpx_codec_destroy(&encoder);
        
        // Limit to processing 2 frames to avoid timeouts
        if (frame_count >= 2) break;
    }
    
    // Destroy decoder
    vpx_codec_destroy(&decoder);
    
    return 0;
}