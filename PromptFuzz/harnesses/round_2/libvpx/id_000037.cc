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
//<ID> 129
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"vp8e_encode":[[910,7,910,34,0,0,4,0]]},"library_calls":["vpx_codec_version_str","vpx_codec_build_config","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_destroy","vpx_codec_destroy","vpx_img_wrap","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_get_cx_data","vpx_codec_error","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_str","vpx_codec_build_config","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_img_wrap","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_global_headers","vpx_codec_get_cx_data","vpx_codec_error","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function prototypes
extern "C" {
    void put_frame_callback(void *user_priv, const vpx_image_t *img) {
        // Dummy callback
        (void)user_priv;
        (void)img;
    }
    
    void put_slice_callback(void *user_priv, const vpx_image_t *img,
                           const vpx_image_rect_t *rect, const vpx_image_rect_t *rect2) {
        // Dummy callback
        (void)user_priv;
        (void)img;
        (void)rect;
        (void)rect2;
    }
    
    int get_frame_buffer_cb(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
        // Dummy callback - fail to allocate
        (void)cb_priv;
        (void)min_size;
        (void)fb;
        return -1;
    }
    
    int release_frame_buffer_cb(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
        // Dummy callback
        (void)cb_priv;
        (void)fb;
        return 0;
    }
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if data is too small
    if (size < 100) {
	return 0;
	}
	
    
    // Step 1: Use version and build config APIs
    const char *version_str = vpx_codec_version_str();
    const char *build_config = vpx_codec_build_config();
    (void)version_str;  // Mark as used
    (void)build_config; // Mark as used
    
    // Step 2: Create encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
	return 0;
	}
	
    
    // Step 3: Get default encoder configuration
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Modify configuration with fuzz data
    cfg.g_w = (data[0] % 64) + 16;  // Width between 16-79
    cfg.g_h = (data[1] % 64) + 16;  // Height between 16-79
    cfg.rc_target_bitrate = (data[2] * 1000) % 1000000;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;
    
    // Step 4: Initialize encoder context
    vpx_codec_ctx_t codec;
    int ver = VPX_ENCODER_ABI_VERSION;
    
    // Try single encoder initialization
    vpx_codec_err_t init_result = vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, ver);
    if (init_result != VPX_CODEC_OK) {
        // Try multi-encoder initialization (single encoder case)
        vpx_rational_t dsf = {1, 1};
        init_result = vpx_codec_enc_init_multi_ver(&codec, iface, &cfg, 1, 0, &dsf, ver);
        if (init_result != VPX_CODEC_OK) {
            return 0;
        }
    }
    
    // Step 5: Set encoder configuration (even if same)
    vpx_codec_enc_config_set(&codec, &cfg);
    
    // Step 6: Register callbacks
    vpx_codec_register_put_frame_cb(&codec, put_frame_callback, nullptr);
    vpx_codec_register_put_slice_cb(&codec, put_slice_callback, nullptr);
    
    // Step 7: Set frame buffer functions
    vpx_codec_set_frame_buffer_functions(&codec, get_frame_buffer_cb, 
                                         release_frame_buffer_cb, nullptr);
    
    // Step 8: Create image from fuzz data
    vpx_image_t img;
    unsigned int width = cfg.g_w;
    unsigned int height = cfg.g_h;
    
    // Calculate buffer size for I420 format
    size_t y_size = width * height;
    size_t uv_size = ((width + 1) / 2) * ((height + 1) / 2);
    size_t total_size = y_size + 2 * uv_size;
    
    if (size < total_size + 100) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Allocate image buffer
    unsigned char *img_data = new unsigned char[total_size];
    if (!img_data) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Copy fuzz data to image buffer (wrapping around if needed)
    for (size_t i = 0; i < total_size; i++) {
        img_data[i] = data[(i + 100) % size];
    }
    
    // Wrap the image
    vpx_image_t *wrapped_img = vpx_img_wrap(&img, VPX_IMG_FMT_I420, width, height, 1, img_data);
    if (!wrapped_img) {
        delete[] img_data;
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Step 9: Set image rectangle (using fuzz data for coordinates)
    unsigned int x = data[3] % (width / 2);
    unsigned int y = data[4] % (height / 2);
    unsigned int w = (data[5] % (width - x)) + 1;
    unsigned int h = (data[6] % (height - y)) + 1;
    vpx_img_set_rect(wrapped_img, x, y, w, h);
    
    // Step 10: Encode the image
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    vpx_enc_deadline_t deadline = 0;
    
    vpx_codec_encode(&codec, wrapped_img, pts, duration, flags, deadline);
    
    // Step 11: Get global headers
    vpx_fixed_buf_t *headers = vpx_codec_get_global_headers(&codec);
    (void)headers;  // Mark as used
    
    // Step 12: Get encoded data
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        // Process packet if needed
        (void)pkt;
    }
    
    // Step 13: Check for errors
    const char *error = vpx_codec_error(&codec);
    const char *error_detail = vpx_codec_error_detail(&codec);
    (void)error;
    (void)error_detail;
    
    // Step 14: Cleanup
    vpx_img_free(wrapped_img);
    delete[] img_data;
    vpx_codec_destroy(&codec);
    
    return 0;
}