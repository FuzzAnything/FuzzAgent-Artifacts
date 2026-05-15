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
//<ID> 178
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 1
//<Quality> {"density":15,"unique_branches":{"vpx_codec_dec_init_ver":[[30,7,30,37,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_error","vpx_codec_destroy","vpx_codec_get_stream_info","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_error","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_control_","vpx_codec_destroy","vpx_codec_destroy","vpx_img_wrap","vpx_codec_encode","vpx_codec_error","vpx_codec_get_global_headers","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_control_","vpx_img_wrap","vpx_codec_encode","vpx_codec_error","vpx_codec_get_global_headers","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// Callback structure for storing decoded frame info
typedef struct {
    unsigned int width;
    unsigned int height;
    bool got_frame;
} DecoderCallbackData;

// Callback function for decoded frames
static void  id_000048_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    DecoderCallbackData *data = static_cast<DecoderCallbackData*>(user_priv);
    if (!data->got_frame) {
        data->width = img->d_w;
        data->height = img->d_h;
        data->got_frame = true;
    }
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Initialize callback data
    DecoderCallbackData cb_data = {0, 0, false};
    
    // Step 1: Try to peek stream info for VP8 and VP9
    vpx_codec_stream_info_t si;
    si.sz = sizeof(si);
    
    vpx_codec_iface_t *vpx_dec_interface = nullptr;
    
    // Try VP9 first
    vpx_codec_iface_t *vpx_codec_vp9_dx_ptr = vpx_codec_vp9_dx();
    if (vpx_codec_peek_stream_info(vpx_codec_vp9_dx_ptr, data, size, &si) == 0) {
        vpx_dec_interface = vpx_codec_vp9_dx_ptr;
    } 
    // Try VP8 if VP9 fails
    else {
        vpx_codec_iface_t *vpx_codec_vp8_dx_ptr = vpx_codec_vp8_dx();
        if (vpx_codec_peek_stream_info(vpx_codec_vp8_dx_ptr, data, size, &si) == 0) {
            vpx_dec_interface = vpx_codec_vp8_dx_ptr;
        } else {
            // Cannot determine codec, skip processing
            return 0;
        }
    }
    
    // Get interface name
    const char *iface_name = vpx_codec_iface_name(vpx_dec_interface);
    
    // Step 2: Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0, 0, 0};
    dec_cfg.threads = 1;
    
    if (vpx_codec_dec_init_ver(&decoder, vpx_dec_interface, &dec_cfg, 0, 4) != 0) {
        const char *error = vpx_codec_error(&decoder);
        return 0;
    }
    
    // Register put frame callback
    vpx_codec_register_put_frame_cb(&decoder,  id_000048_put_frame_cb, &cb_data);
    
    // Step 3: Decode the input data
    if (vpx_codec_decode(&decoder, data, size, nullptr, 0) != 0) {
        const char *error = vpx_codec_error(&decoder);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Get stream info from decoder
    vpx_codec_stream_info_t dec_si;
    dec_si.sz = sizeof(dec_si);
    vpx_codec_get_stream_info(&decoder, &dec_si);
    
    // Step 4: Initialize encoder (always use VP8 encoder)
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    
    // Get default encoder configuration
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    
    // Set encoder dimensions (use decoded frame dimensions if available, otherwise from stream info)
    if (cb_data.got_frame) {
        enc_cfg.g_w = cb_data.width;
        enc_cfg.g_h = cb_data.height;
    } else if (dec_si.w > 0 && dec_si.h > 0) {
        enc_cfg.g_w = dec_si.w;
        enc_cfg.g_h = dec_si.h;
    } else {
        enc_cfg.g_w = 320;
        enc_cfg.g_h = 240;
    }
    enc_cfg.rc_target_bitrate = 256;
    
    // Initialize encoder
    if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, 4) != 0) {
        const char *error = vpx_codec_error(&encoder);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Set encoder configuration
    vpx_codec_enc_config_set(&encoder, &enc_cfg);
    
    // Use codec control (set CPU used to 1 for faster encoding)
    vpx_codec_control_(&encoder, 1, 1); // VP8E_SET_CPUUSED = 1
    
    // Step 5: Create a dummy image for encoding
    const unsigned int width = enc_cfg.g_w;
    const unsigned int height = enc_cfg.g_h;
    const size_t y_size = width * height;
    const size_t uv_size = y_size / 4;
    unsigned char *img_data = static_cast<unsigned char*>(malloc(y_size + 2 * uv_size));
    if (!img_data) {
        vpx_codec_destroy(&encoder);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Fill with zeroes (gray image)
    memset(img_data, 0, y_size + 2 * uv_size);
    
    vpx_image_t raw_img;
    vpx_img_wrap(&raw_img, VPX_IMG_FMT_I420, width, height, 1, img_data);
    
    // Step 6: Encode the dummy frame
    if (vpx_codec_encode(&encoder, &raw_img, 0, 1, 0, 0) != 0) {
        const char *error = vpx_codec_error(&encoder);
    }
    
    // Get global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
    
    // Get encoded data
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        // Process packet if needed
    }
    
    // Get preview frame
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&encoder);
    
    // Step 7: Cleanup
    free(img_data);
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    
    return 0;
}