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
//<ID> 75
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_set_cx_data_buf","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_set_cx_data_buf","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callbacks
static int  id_000026_get_frame_buffer(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    uint8_t *buf = (uint8_t *)malloc(min_size);
    if (!buf) return -1;
    fb->data = buf;
    fb->size = min_size;
    return 0;
}

static int  id_000026_release_frame_buffer(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

// Put frame callback
static void  id_000026_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Do nothing for fuzzing
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing
    
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get VP8 encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get interface name
    const char *iface_name = vpx_codec_iface_name(iface);
    (void)iface_name; // Use variable to avoid unused warning
    
    // Setup encoder configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Configure encoder with some values from input
    cfg.g_w = 320;
    cfg.g_h = 240;
    cfg.rc_target_bitrate = 1000;
    
    vpx_rational_t dsf = {1, 1};
    
    // Initialize first encoder with vpx_codec_enc_init_ver
    vpx_codec_ctx_t encoder1;
    err = vpx_codec_enc_init_ver(&encoder1, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize second encoder with vpx_codec_enc_init_multi_ver
    vpx_codec_ctx_t encoder2;
    err = vpx_codec_enc_init_multi_ver(&encoder2, iface, &cfg, 1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder1);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set frame buffer functions for encoder1
    err = vpx_codec_set_frame_buffer_functions(&encoder1,  id_000026_get_frame_buffer,  id_000026_release_frame_buffer, nullptr);
    (void)err; // Use variable to avoid unused warning
    
    // Register put frame callback for encoder1
    err = vpx_codec_register_put_frame_cb(&encoder1,  id_000026_put_frame_callback, nullptr);
    (void)err;
    
    // Allocate image for raw video data
    vpx_image_t *raw_img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!raw_img) {
        vpx_codec_destroy(&encoder2);
        vpx_codec_destroy(&encoder1);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Read some data from input to fill image
    size_t img_size = cfg.g_w * cfg.g_h * 3 / 2;
    size_t bytes_to_read = img_size < size ? img_size : size;
    fread(raw_img->planes[0], 1, bytes_to_read, in_file);
    
    // Setup output buffer for encoded data
    std::vector<uint8_t> output_buffer(size * 2);
    vpx_fixed_buf_t fb;
    fb.buf = output_buffer.data();
    fb.sz = output_buffer.size();
    
    // Set cx data buffer
    err = vpx_codec_set_cx_data_buf(&encoder1, &fb, 0, 0);
    (void)err;
    
    // Get global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder1);
    if (global_headers && global_headers->buf) {
        fwrite(global_headers->buf, 1, global_headers->sz, out_file);
    }
    
    // Encode frame with encoder1
    err = vpx_codec_encode(&encoder1, raw_img, 0, 1, 0, VPX_DL_REALTIME);
    if (err == VPX_CODEC_OK) {
        vpx_codec_iter_t iter = nullptr;
        const vpx_codec_cx_pkt_t *pkt;
        
        while ((pkt = vpx_codec_get_cx_data(&encoder1, &iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
        
        // Get preview frame
        const vpx_image_t *preview = vpx_codec_get_preview_frame(&encoder1);
        (void)preview; // Use variable to avoid unused warning
    }
    
    // Encode frame with encoder2
    err = vpx_codec_encode(&encoder2, raw_img, 1, 1, 0, VPX_DL_REALTIME);
    if (err == VPX_CODEC_OK) {
        vpx_codec_iter_t iter = nullptr;
        const vpx_codec_cx_pkt_t *pkt;
        
        while ((pkt = vpx_codec_get_cx_data(&encoder2, &iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
        
        // Get preview frame
        const vpx_image_t *preview = vpx_codec_get_preview_frame(&encoder2);
        (void)preview;
    }
    
    // Cleanup
    vpx_img_free(raw_img);
    vpx_codec_destroy(&encoder2);
    vpx_codec_destroy(&encoder1);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}