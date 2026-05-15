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
//<ID> 17
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["vpx_codec_version_extra_str","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp8_dx","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_error","vpx_codec_destroy","vpx_codec_error","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_extra_str","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp8_dx","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_error","vpx_codec_destroy","vpx_codec_error","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

// Dummy callback function for put_frame
static void  id_000011_dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
    // Do nothing - just a placeholder callback
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get extra version string (API 1)
    const char *extra_version = vpx_codec_version_extra_str();
    (void)extra_version; // Use it to avoid unused variable warning
    
    // 2. Try to determine if this is VP8 or VP9 stream
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // Try VP9 first
    vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
        vpx_codec_vp9_dx(), data, size, &stream_info);
    
    vpx_codec_iface_t *decoder_iface = nullptr;
    if (peek_result == VPX_CODEC_OK) {
        decoder_iface = vpx_codec_vp9_dx();
    } else {
        // Try VP8
        memset(&stream_info, 0, sizeof(stream_info));
        stream_info.sz = sizeof(stream_info);
        peek_result = vpx_codec_peek_stream_info(
            vpx_codec_vp8_dx(), data, size, &stream_info);
        if (peek_result == VPX_CODEC_OK) {
            decoder_iface = vpx_codec_vp8_dx();
        }
    }
    
    if (!decoder_iface) {
        // Cannot determine codec, but we still need to use other APIs
        // Create a minimal decoder context anyway
        decoder_iface = vpx_codec_vp8_dx();
    }
    
    // 3. Create decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    
    vpx_codec_err_t init_result = vpx_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (init_result != VPX_CODEC_OK) {
        // Get error message (API 2)
        const char *error_msg = vpx_codec_error(&decoder);
        (void)error_msg;
        return 0;
    }
    
    // 4. Register put_frame callback (API 5)
    vpx_codec_register_put_frame_cb(&decoder,  id_000011_dummy_put_frame_cb, nullptr);
    
    // 5. Decode the input data
    vpx_codec_decode(&decoder, data, size, nullptr, 0);
    
    // 6. Try to get preview frame from decoder (API 3)
    // Note: This API is actually for encoders, but we call it anyway
    // to satisfy the requirement. It will likely return nullptr.
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&decoder);
    (void)preview_frame;
    
    // 7. Create an encoder context to use vpx_codec_get_global_headers
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    
    // Use VP8 encoder interface
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    
    // Get default encoder configuration
    if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == VPX_CODEC_OK) {
        // Set some minimal parameters
        enc_cfg.g_w = 320;
        enc_cfg.g_h = 240;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 256;
        enc_cfg.g_pass = VPX_RC_ONE_PASS;
        
        // Initialize encoder
        if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0,
                                   VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
            
            // 8. Get global headers from encoder (API 6)
            vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
            if (global_headers) {
                // Free the global headers buffer
                free(global_headers->buf);
                // Note: vpx_codec_get_global_headers returns a pointer to a
                // vpx_fixed_buf_t that should be freed by calling free() on buf
                // The structure itself is managed by the codec
            }
            
            // 9. Create a dummy image and encode one frame
            vpx_image_t *img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                            enc_cfg.g_w, enc_cfg.g_h, 1);
            if (img) {
                // Fill with zeros
                memset(img->planes[0], 0, img->stride[0] * enc_cfg.g_h);
                if (img->fmt == VPX_IMG_FMT_I420) {
                    memset(img->planes[1], 128, img->stride[1] * enc_cfg.g_h / 2);
                    memset(img->planes[2], 128, img->stride[2] * enc_cfg.g_h / 2);
                }
                
                // Encode one frame
                vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
                
                // Try to get preview frame from encoder (API 3 - actual intended use)
                const vpx_image_t *encoder_preview = vpx_codec_get_preview_frame(&encoder);
                (void)encoder_preview;
                
                // Clean up image
                vpx_img_free(img);
            }
            
            // 10. Get error from encoder context (API 2)
            const char *encoder_error = vpx_codec_error(&encoder);
            (void)encoder_error;
            
            // Destroy encoder
            vpx_codec_destroy(&encoder);
        }
    }
    
    // 11. Get error from decoder context (API 2)
    const char *decoder_error = vpx_codec_error(&decoder);
    (void)decoder_error;
    
    // 12. Destroy decoder
    vpx_codec_destroy(&decoder);
    
    return 0;
}