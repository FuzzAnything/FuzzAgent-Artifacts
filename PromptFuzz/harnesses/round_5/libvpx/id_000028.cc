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
//<ID> 77
//<Prompt> []
/*<Combination>: [
*/
//<score> 68, nr_unique_branch: 4
//<Quality> {"density":17,"unique_branches":{"thread_loopfilter":[[31,9,31,61,0,0,4,0]],"thread_encoding_proc":[[53,9,53,61,0,0,4,0]],"encode_mb_row":[[412,11,412,38,0,0,4,1],[416,21,416,44,0,0,4,1]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_destroy","vpx_img_alloc","vpx_codec_destroy","vpx_codec_register_put_frame_cb","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_global_headers","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_destroy","vpx_img_alloc","vpx_codec_register_put_frame_cb","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_global_headers","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Dummy callback functions
static int  id_000028_dummy_get_frame_buffer(void* cb_priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
    (void)cb_priv;
    (void)min_size;
    (void)fb;
    return -1; // Indicate failure
}

static int  id_000028_dummy_release_frame_buffer(void* cb_priv, vpx_codec_frame_buffer_t* fb) {
    (void)cb_priv;
    (void)fb;
    return 0;
}

static void  id_000028_dummy_put_frame_cb(void* user_priv, const vpx_image_t* img) {
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 32) {
        return 0; // Need minimum data
    }

    // Step 1: Get encoder interface
    vpx_codec_iface_t* iface = vpx_codec_vp8_cx();
    const char* iface_name = vpx_codec_iface_name(iface);
    (void)iface_name; // Use to avoid unused warning

    // Step 2: Setup encoder configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    // Use some fuzz data for configuration
    cfg.g_w = (data[0] % 64 + 16) * 16;  // 16-1280 pixels
    cfg.g_h = (data[1] % 64 + 16) * 16;  // 16-1280 pixels
    cfg.g_threads = (data[2] % 8) + 1;
    cfg.rc_target_bitrate = (data[3] % 1000) + 100;

    // Step 3: Initialize single encoder
    vpx_codec_ctx_t encoder;
    err = vpx_codec_enc_init_ver(&encoder, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    // Step 4: Initialize multi-encoder (with 1 instance)
    vpx_codec_ctx_t multi_encoder;
    vpx_rational_t dsf = {1, 1};
    err = vpx_codec_enc_init_multi_ver(&multi_encoder, iface, &cfg, 1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
    if (err == VPX_CODEC_OK) {
        // Set frame buffer functions
        vpx_codec_set_frame_buffer_functions(&multi_encoder, 
                                             id_000028_dummy_get_frame_buffer,
                                             id_000028_dummy_release_frame_buffer,
                                            nullptr);
        
        // Clean up multi-encoder
        vpx_codec_destroy(&multi_encoder);
    }

    // Step 5: Allocate image
    vpx_image_t* img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (!img) {
        vpx_codec_destroy(&encoder);
        return 0;
    }

    // Step 6: Fill image with fuzz data (simplified - would need proper YUV format)
    size_t y_size = cfg.g_w * cfg.g_h;
    size_t uv_size = (cfg.g_w / 2) * (cfg.g_h / 2);
    
    if (size >= y_size + 2 * uv_size) {
        // Fill Y plane
        size_t copy_y = std::min(y_size, size);
        memcpy(img->planes[0], data, copy_y);
        
        // Fill U and V planes
        if (size >= y_size + uv_size) {
            size_t offset = y_size;
            size_t copy_u = std::min(uv_size, size - offset);
            memcpy(img->planes[1], data + offset, copy_u);
            
            if (size >= y_size + 2 * uv_size) {
                offset = y_size + uv_size;
                size_t copy_v = std::min(uv_size, size - offset);
                memcpy(img->planes[2], data + offset, copy_v);
            }
        }
    }

    // Step 7: Register put frame callback
    vpx_codec_register_put_frame_cb(&encoder,  id_000028_dummy_put_frame_cb, nullptr);

    // Step 8: Set output buffer
    vpx_fixed_buf_t output_buf;
    uint8_t buffer[4096];
    output_buf.buf = buffer;
    output_buf.sz = sizeof(buffer);
    vpx_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);

    // Step 9: Encode frame
    err = vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
    
    if (err == VPX_CODEC_OK) {
        // Step 10: Get encoded data
        vpx_codec_iter_t iter = nullptr;
        const vpx_codec_cx_pkt_t* pkt;
        
        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
            // Process packet if needed
            (void)pkt;
        }
        
        // Step 11: Get global headers
        vpx_fixed_buf_t* headers = vpx_codec_get_global_headers(&encoder);
        (void)headers; // Use to avoid unused warning
        
        // Step 12: Get preview frame
        const vpx_image_t* preview = vpx_codec_get_preview_frame(&encoder);
        (void)preview; // Use to avoid unused warning
    }

    // Step 13: Clean up
    vpx_img_free(img);
    vpx_codec_destroy(&encoder);

    return 0;
}