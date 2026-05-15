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
//<ID> 69
//<Prompt> []
/*<Combination>: [
*/
//<score> 23, nr_unique_branch: 0
//<Quality> {"density":23,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_register_put_slice_cb","vpx_codec_register_put_frame_cb","vpx_codec_enc_config_set","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_register_put_frame_cb","vpx_codec_enc_config_set","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback functions
static void  id_000029_put_slice_callback(void* user_priv, const vpx_image_t* img,
                               const vpx_image_rect_t* rect, 
                               const vpx_image_rect_t* rect2) {
    // Empty callback implementation
}

static void  id_000029_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // Empty callback implementation
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data for stream info
    
    vpx_codec_stream_info_t stream_info = {0};
    stream_info.sz = sizeof(stream_info);
    
    // Try both VP8 and VP9 decoders
    vpx_codec_iface_t* codec_iface = nullptr;
    vpx_codec_err_t err = VPX_CODEC_OK;
    
    // First try VP9
    codec_iface = vpx_codec_vp9_dx();
    err = vpx_codec_peek_stream_info(codec_iface, data, size, &stream_info);
    
    if (err != VPX_CODEC_OK) {
        // Try VP8
        codec_iface = vpx_codec_vp8_dx();
        err = vpx_codec_peek_stream_info(codec_iface, data, size, &stream_info);
        if (err != VPX_CODEC_OK) {
            return 0;  // Not a valid VP8/VP9 stream
        }
    }
    
    // Get codec interface name
    const char* iface_name = vpx_codec_iface_name(codec_iface);
    
    // Create decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (vpx_codec_dec_init(&decoder, codec_iface, &dec_cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Create encoder context using the same codec type
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    
    // Determine encoder interface based on decoder interface
    vpx_codec_iface_t* enc_iface = nullptr;
    if (codec_iface == vpx_codec_vp9_dx()) {
        enc_iface = vpx_codec_vp9_cx();
    } else {
        enc_iface = vpx_codec_vp8_cx();
    }
    
    // Get default encoder configuration
    if (vpx_codec_enc_config_default(enc_iface, &enc_cfg, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Set encoder configuration based on stream info
    enc_cfg.g_w = stream_info.w;
    enc_cfg.g_h = stream_info.h;
    enc_cfg.rc_target_bitrate = 1000;  // 1000 kbps
    enc_cfg.g_threads = 1;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.g_lag_in_frames = 0;
    
    // Initialize encoder
    if (vpx_codec_enc_init(&encoder, enc_iface, &enc_cfg, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Register callbacks (using the required APIs)
    vpx_codec_register_put_slice_cb(&encoder,  id_000029_put_slice_callback, nullptr);
    vpx_codec_register_put_frame_cb(&encoder,  id_000029_put_frame_callback, nullptr);
    
    // Apply encoder configuration (using the required API)
    vpx_codec_enc_config_set(&encoder, &enc_cfg);
    
    // Decode the input data
    if (vpx_codec_decode(&decoder, data, size, nullptr, 0) == VPX_CODEC_OK) {
        vpx_codec_iter_t iter = nullptr;
        vpx_image_t* decoded_frame = nullptr;
        
        // Get decoded frame
        while ((decoded_frame = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
            // Create a wrapped image for encoding (using the required API)
            vpx_image_t wrapped_img;
            // Use actual image data from decoded frame
            unsigned char* img_data = decoded_frame->img_data;
            vpx_img_wrap(&wrapped_img, decoded_frame->fmt, 
                        decoded_frame->d_w, decoded_frame->d_h, 
                        1, img_data);
            
            // Encode the frame
            vpx_codec_encode(&encoder, &wrapped_img, 0, 1, 0, VPX_DL_REALTIME);
            
            // Get encoded data
            vpx_codec_iter_t enc_iter = nullptr;
            const vpx_codec_cx_pkt_t* pkt = nullptr;
            
            while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    // Frame packet - could write to file here
                }
            }
            
            // Don't free the wrapped image since it uses the decoder's data
        }
    }
    
    // Get global headers from encoder (using the required API)
    vpx_fixed_buf_t* global_headers = vpx_codec_get_global_headers(&encoder);
    
    // Clean up
    vpx_codec_destroy(&decoder);
    vpx_codec_destroy(&encoder);
    
    return 0;
}