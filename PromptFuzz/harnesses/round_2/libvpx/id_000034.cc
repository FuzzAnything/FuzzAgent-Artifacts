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
//<ID> 113
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"vp8_pack_tokens":[[252,28,252,48,0,0,4,0]]},"library_calls":["vpx_codec_version_str","vpx_codec_build_config","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_slice_cb","vpx_codec_register_put_frame_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_destroy","vpx_img_wrap","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_error","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_str","vpx_codec_build_config","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_slice_cb","vpx_codec_register_put_frame_cb","vpx_codec_set_frame_buffer_functions","vpx_img_wrap","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_error","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback functions
static int  id_000034_get_frame_buffer_cb(void *cb_priv, size_t min_size, 
                               vpx_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t*)calloc(1, min_size);
    fb->size = min_size;
    return 0;
}

static int  id_000034_release_frame_buffer_cb(void *cb_priv, 
                                   vpx_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

static void  id_000034_put_slice_cb(void *user_priv, const vpx_image_t *img,
                         const vpx_image_rect_t *slice,
                         const vpx_image_rect_t *total) {
    // Empty callback
}

static void  id_000034_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    // Empty callback
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Too small for meaningful processing
    
    // Get version info (API requirement)
    const char *version_str = vpx_codec_version_str();
    const char *build_config = vpx_codec_build_config();
    
    // Initialize encoder
    vpx_codec_ctx_t codec;
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_enc_config_default(iface, &cfg, 0);  // Usage = 0 (good quality)
    
    // Configure basic settings
    cfg.g_w = 320;
    cfg.g_h = 240;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    
    // Initialize encoder
    vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    
    // Set config again (API requirement)
    vpx_codec_enc_config_set(&codec, &cfg);
    
    // Register callbacks
    vpx_codec_register_put_slice_cb(&codec,  id_000034_put_slice_cb, nullptr);
    vpx_codec_register_put_frame_cb(&codec,  id_000034_put_frame_cb, nullptr);
    
    // Set frame buffer functions
    vpx_codec_set_frame_buffer_functions(&codec,  id_000034_get_frame_buffer_cb, 
                                         id_000034_release_frame_buffer_cb, nullptr);
    
    // Create input image from fuzz data
    vpx_image_t img;
    unsigned int width = 320;
    unsigned int height = 240;
    
    // Calculate required buffer size for I420 format
    size_t y_size = width * height;
    size_t uv_size = (width / 2) * (height / 2);
    size_t total_size = y_size + 2 * uv_size;
    
    // Allocate image buffer
    uint8_t *img_buffer = (uint8_t*)malloc(total_size);
    if (!img_buffer) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Fill buffer with fuzz data (wrap around if needed)
    for (size_t i = 0; i < total_size; i++) {
        img_buffer[i] = data[i % size];
    }
    
    // Wrap the image
    vpx_img_wrap(&img, VPX_IMG_FMT_I420, width, height, 1, img_buffer);
    
    // Set image rectangle (API requirement)
    vpx_img_set_rect(&img, 0, 0, width, height);
    
    // Encode frame
    vpx_codec_encode(&codec, &img, 0, 1, 0, VPX_DL_REALTIME);
    
    // Get global headers (API requirement)
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&codec);
    
    // Get compressed data
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        // Process packet if needed
    }
    
    // Get preview frame (API requirement)
    const vpx_image_t *preview = vpx_codec_get_preview_frame(&codec);
    
    // Check for errors
    const char *error_str = vpx_codec_error(&codec);
    const char *error_detail = vpx_codec_error_detail(&codec);
    
    // Cleanup
    vpx_img_free(&img);
    free(img_buffer);
    
    // Destroy codec
    vpx_codec_destroy(&codec);
    
    return 0;
}