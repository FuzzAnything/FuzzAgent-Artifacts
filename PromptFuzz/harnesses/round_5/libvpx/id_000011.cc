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
//<ID> 23
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["vpx_codec_version","vpx_codec_version_extra_str","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp8_dx","vpx_codec_err_to_string","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_register_put_frame_cb","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_get_stream_info","vpx_codec_err_to_string","vpx_codec_get_frame","vpx_img_wrap","vpx_img_free","vpx_codec_vp9_dx","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_codec_enc_config_set","vpx_codec_control_","vpx_codec_control_","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_version","vpx_codec_version_extra_str","vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_get_stream_info","vpx_codec_err_to_string","vpx_codec_get_frame","vpx_img_wrap","vpx_img_free","vpx_codec_vp9_dx","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_codec_enc_config_set","vpx_codec_control_","vpx_codec_control_","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame callback function for vpx_codec_register_put_frame_cb
static void  id_000011_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // This callback is invoked when a frame is decoded
    // We can process the frame here if needed
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get version information
    int version = vpx_codec_version();
    const char *extra_str = vpx_codec_version_extra_str();
    (void)version;
    (void)extra_str;
    
    // 2. Try to peek stream info to determine if this is VP8 or VP9 data
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // Try VP9 first, then VP8
    vpx_codec_err_t peek_err = vpx_codec_peek_stream_info(vpx_codec_vp9_dx(), data, size, &stream_info);
    vpx_codec_iface_t *codec_iface = nullptr;
    
    if (peek_err == VPX_CODEC_OK) {
        codec_iface = vpx_codec_vp9_dx();
    } else {
        // Try VP8
        peek_err = vpx_codec_peek_stream_info(vpx_codec_vp8_dx(), data, size, &stream_info);
        if (peek_err == VPX_CODEC_OK) {
            codec_iface = vpx_codec_vp8_dx();
        }
    }
    
    if (!codec_iface) {
        // Can't determine codec, use error string API
        const char *err_str = vpx_codec_err_to_string(peek_err);
        (void)err_str;
        return 0;
    }
    
    // 3. Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    
    vpx_codec_err_t init_err = vpx_codec_dec_init_ver(&decoder, codec_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (init_err != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(init_err);
        (void)err_str;
        return 0;
    }
    
    // 4. Register frame callback
    vpx_codec_err_t reg_err = vpx_codec_register_put_frame_cb(&decoder,  id_000011_put_frame_callback, nullptr);
    if (reg_err != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(reg_err);
        (void)err_str;
    }
    
    // 5. Decode the input data
    vpx_codec_err_t decode_err = vpx_codec_decode(&decoder, data, size, nullptr, 0);
    if (decode_err != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(decode_err);
        (void)err_str;
    }
    
    // 6. Get stream info from decoder (different from peek)
    vpx_codec_stream_info_t decoder_stream_info;
    memset(&decoder_stream_info, 0, sizeof(decoder_stream_info));
    decoder_stream_info.sz = sizeof(decoder_stream_info);
    
    vpx_codec_err_t get_info_err = vpx_codec_get_stream_info(&decoder, &decoder_stream_info);
    if (get_info_err != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(get_info_err);
        (void)err_str;
    }
    
    // 7. Try to get decoded frames
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        // 8. Wrap the image data using vpx_img_wrap
        // We need to allocate buffer for wrapped image
        unsigned int buffer_size = img->d_w * img->d_h * 3 / 2; // For I420 format
        unsigned char *img_buffer = (unsigned char *)malloc(buffer_size);
        if (!img_buffer) {
            break;
        }
        
        vpx_image_t wrapped_img;
        vpx_image_t *wrap_result = vpx_img_wrap(&wrapped_img, VPX_IMG_FMT_I420, 
                                               img->d_w, img->d_h, 1, img_buffer);
        if (wrap_result) {
            // Copy image data from decoded frame to our wrapped buffer
            // This is a simplified copy - in reality we'd need to handle planes properly
            for (int plane = 0; plane < 3; plane++) {
                if (img->planes[plane]) {
                    int plane_size = (plane == 0) ? img->d_w * img->d_h : 
                                     (img->d_w * img->d_h) / 4;
                    memcpy(wrapped_img.planes[plane], img->planes[plane], plane_size);
                }
            }
            
            // Free the image buffer
            vpx_img_free(&wrapped_img);
        }
        free(img_buffer);
    }
    
    // 9. Try to initialize encoder and set config (even though we're decoding)
    // This is to use vpx_codec_enc_config_set API
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    
    // Try to get default encoder config
    if (codec_iface == vpx_codec_vp9_dx()) {
        vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &enc_cfg, 0);
        
        // Initialize encoder
        if (vpx_codec_enc_init_ver(&encoder, vpx_codec_vp9_cx(), &enc_cfg, 0, VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
            // Set encoder config (even though we won't actually encode)
            vpx_codec_enc_config_set(&encoder, &enc_cfg);
            
            // Use vpx_codec_control_ API
            // Try to set some control parameters
            vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 6);
            vpx_codec_control_(&encoder, VP8E_SET_STATIC_THRESHOLD, 1);
            
            vpx_codec_destroy(&encoder);
        }
    }
    
    // 10. Cleanup decoder
    vpx_codec_destroy(&decoder);
    
    return 0;
}