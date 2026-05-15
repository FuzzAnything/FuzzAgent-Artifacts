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
//<ID> 30
//<Prompt> []
/*<Combination>: [
*/
//<score> 9.5, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_destroy","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_encode","vpx_codec_error","vpx_codec_get_preview_frame","vpx_codec_control_","vpx_codec_get_cx_data","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_error","vpx_codec_get_preview_frame","vpx_codec_control_","vpx_codec_get_cx_data","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/*Here's a C++ fuzz driver that achieves the described event using the specified libvpx APIs:

*/


extern "C" {
}

// Callback function for frame output
static void  id_000015_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Empty callback - just to register
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data
    
    // Create input file from the data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize codec
    vpx_codec_ctx_t codec;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    
    // Set up encoder configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_enc_config_default(iface, &cfg, 0);
    
    // Set dimensions from fuzz data (ensure valid)
    cfg.g_w = (data[0] % 64) + 16;  // 16-79 pixels
    cfg.g_h = (data[1] % 64) + 16;  // 16-79 pixels
    
    // Initialize encoder with vpx_codec_enc_init_ver
    vpx_codec_err_t err = vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize multi-encoder context (even though we only use one)
    vpx_codec_ctx_t multi_codec;
    vpx_rational_t dsf = {1, 1};  // No downsampling
    
    err = vpx_codec_enc_init_multi_ver(&multi_codec, iface, &cfg, 1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        // Clean up and continue with single codec
        vpx_codec_destroy(&codec);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Register put frame callback
    vpx_codec_register_put_frame_cb(&codec,  id_000015_put_frame_callback, nullptr);
    
    // Create input image
    vpx_image_t *img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img) {
        vpx_codec_destroy(&multi_codec);
        vpx_codec_destroy(&codec);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set rectangle on image using fuzz data
    unsigned int x = data[2] % cfg.g_w;
    unsigned int y = data[3] % cfg.g_h;
    unsigned int w = (data[4] % (cfg.g_w - x)) + 1;
    unsigned int h = (data[5] % (cfg.g_h - y)) + 1;
    
    int rect_result = vpx_img_set_rect(img, x, y, w, h);
    
    // Fill image with some data from input
    size_t offset = 6;
    for (int i = 0; i < 3; i++) {
        if (img->stride[i] > 0 && img->planes[i]) {
            size_t plane_size = img->stride[i] * (i == 0 ? cfg.g_h : cfg.g_h / 2);
            size_t copy_size = (size - offset) < plane_size ? (size - offset) : plane_size;
            if (copy_size > 0) {
                memcpy(img->planes[i], data + offset, copy_size);
                offset += copy_size;
            }
        }
    }
    
    // Encode a frame
    err = vpx_codec_encode(&codec, img, 0, 1, 0, VPX_DL_REALTIME);
    
    // Check error using vpx_codec_error
    const char* error_str = vpx_codec_error(&codec);
    
    // Try to get preview frame
    const vpx_image_t *preview = vpx_codec_get_preview_frame(&codec);
    
    // Use vpx_codec_control_ with some control IDs from fuzz data
    int control_id = data[offset % size] % 256;  // Random control ID
    vpx_codec_control_(&codec, control_id);
    
    // Get encoded data
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        // Process packet if needed
    }
    
    // Also get frame from the multi encoder context
    iter = nullptr;
    const vpx_image_t *decoded_frame = vpx_codec_get_frame(&multi_codec, &iter);
    
    // Clean up
    vpx_img_free(img);
    vpx_codec_destroy(&multi_codec);
    vpx_codec_destroy(&codec);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}