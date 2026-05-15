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
//<ID> 80
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_error","vpx_codec_err_to_string","vpx_codec_enc_config_set","vpx_codec_error","vpx_codec_destroy","vpx_codec_register_put_slice_cb","vpx_codec_destroy","vpx_img_wrap","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_error","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_version_str","vpx_codec_build_config"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_slice_cb","vpx_img_wrap","vpx_img_set_rect","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_error","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_version_str","vpx_codec_build_config"],"visited":1}
/*Looking at these APIs and the requirement to process libvpx output data, I can see they're designed for encoding functionality. The event these APIs achieve together is: **encoding a video frame while handling slice output callbacks**. Here's the fuzz driver:

*/


// Callback function for put_slice API
static void  id_000026_put_slice_callback(void *user_priv, 
                               const vpx_image_t *img, 
                               const vpx_image_rect_t *rect, 
                               const vpx_image_rect_t *rect2) {
    // Empty callback for fuzzing
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 100) {
	return 0;
	}
	
    
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t *img = nullptr;
    vpx_fixed_buf_t *global_headers = nullptr;
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    
    // Initialize codec interface
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
	return 0;
	}
	
    
    // Get default configuration
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Set configuration parameters from input data
    cfg.g_w = (data[0] % 64 + 64) * 4;  // Width: 256-512
    cfg.g_h = (data[1] % 64 + 64) * 4;  // Height: 256-512
    cfg.rc_target_bitrate = (data[2] % 1000 + 100) * 1000;  // Bitrate: 100k-1100k
    
    // Initialize encoder
    if (vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        // Use error reporting APIs
        const char *error_str = vpx_codec_error(&codec);
        const char *err_to_str = vpx_codec_err_to_string(codec.err);
        (void)error_str;
        (void)err_to_str;
        return 0;
    }
    
    // Set configuration (using vpx_codec_enc_config_set)
    if (vpx_codec_enc_config_set(&codec, &cfg) != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_error(&codec);
        (void)error_str;
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Register put_slice callback
    if (vpx_codec_register_put_slice_cb(&codec,  id_000026_put_slice_callback, nullptr) != VPX_CODEC_OK) {
        // Continue even if callback registration fails
    }
    
    // Create image buffer
    const size_t img_size = cfg.g_w * cfg.g_h * 3 / 2;  // YUV420 size
    unsigned char *img_data = (unsigned char *)malloc(img_size);
    if (!img_data) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Initialize with input data (wrapping around if needed)
    for (size_t i = 0; i < img_size; i++) {
        img_data[i] = data[i % size];
    }
    
    // Wrap image
    img = vpx_img_wrap(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1, img_data);
    if (!img) {
        free(img_data);
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Set rectangle on image (using vpx_img_set_rect)
    unsigned int rect_x = (data[3] % (cfg.g_w / 4)) & ~1;  // Align to even
    unsigned int rect_y = (data[4] % (cfg.g_h / 4)) & ~1;  // Align to even
    unsigned int rect_w = ((data[5] % (cfg.g_w / 2)) & ~1) + 2;
    unsigned int rect_h = ((data[6] % (cfg.g_h / 2)) & ~1) + 2;
    
    // Ensure rectangle stays within bounds
    if (rect_x + rect_w > cfg.g_w) rect_w = cfg.g_w - rect_x;
    if (rect_y + rect_h > cfg.g_h) rect_h = cfg.g_h - rect_y;
    
    if (rect_w > 0 && rect_h > 0) {
        vpx_img_set_rect(img, rect_x, rect_y, rect_w, rect_h);
    }
    
    // Get global headers
    global_headers = vpx_codec_get_global_headers(&codec);
    
    // Encode frame (using vpx_codec_encode)
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    
    if (vpx_codec_encode(&codec, img, pts, duration, flags, 0) != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_error(&codec);
        (void)error_str;
    }
    
    // Get encoded data
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        // Process packet if needed
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Frame data available in pkt->data.frame.buf
        }
    }
    
    // Get preview frame
    const vpx_image_t *preview = vpx_codec_get_preview_frame(&codec);
    (void)preview;
    
    // Clean up
    vpx_img_free(img);
    free(img_data);
    vpx_codec_destroy(&codec);
    
    // Use error string APIs one more time
    const char *version_str = vpx_codec_version_str();
    const char *build_config = vpx_codec_build_config();
    (void)version_str;
    (void)build_config;
    
    return 0;
}