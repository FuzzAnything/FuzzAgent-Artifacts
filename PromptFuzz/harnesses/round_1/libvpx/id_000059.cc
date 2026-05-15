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
//<ID> 219
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_img_alloc","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_img_alloc","vpx_img_set_rect","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback functions that will be registered
static int  id_000059_get_frame_buffer_cb(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t *)malloc(min_size);
    fb->size = min_size;
    return fb->data ? 0 : -1;
}

static int  id_000059_release_frame_buffer_cb(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

static void  id_000059_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    // Callback for frame output - could be used for visualization
}

static void  id_000059_put_slice_cb(void *user_priv, const vpx_image_t *img, 
                         const vpx_image_rect_t *rect, const vpx_image_rect_t *clipping) {
    // Callback for slice output
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    vpx_rational_t dsf = {1, 1};
    vpx_fixed_buf_t output_buf = {nullptr, 0};
    
    // Initialize encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get default configuration
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set reasonable dimensions based on input data
    cfg.g_w = 640;
    cfg.g_h = 480;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    
    // Initialize encoder with multiple instances
    if (vpx_codec_enc_init_multi_ver(&codec, iface, &cfg, 1, 0, &dsf, 4) != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Register callbacks
    vpx_codec_register_put_frame_cb(&codec,  id_000059_put_frame_cb, nullptr);
    vpx_codec_register_put_slice_cb(&codec,  id_000059_put_slice_cb, nullptr);
    
    // Set frame buffer functions
    vpx_codec_set_frame_buffer_functions(&codec,  id_000059_get_frame_buffer_cb, 
                                          id_000059_release_frame_buffer_cb, nullptr);
    
    // Allocate image for encoding
    vpx_image_t *img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img) {
        vpx_codec_destroy(&codec);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set rectangle on image (using part of the fuzzer data for coordinates)
    if (size >= 16) {
        unsigned int x = data[0] % (cfg.g_w - 8);
        unsigned int y = data[1] % (cfg.g_h - 8);
        unsigned int w = 8 + (data[2] % 64);
        unsigned int h = 8 + (data[3] % 64);
        vpx_img_set_rect(img, x, y, w, h);
    }
    
    // Prepare output buffer for encoded data
    output_buf.sz = 1024 * 1024;  // 1MB buffer
    output_buf.buf = malloc(output_buf.sz);
    
    if (output_buf.buf) {
        // Set output buffer for encoder
        vpx_codec_set_cx_data_buf(&codec, &output_buf, 0, 0);
        
        // Encode a frame (use fuzzer data to fill image planes)
        size_t plane_size = cfg.g_w * cfg.g_h;
        if (size >= plane_size * 3 / 2) {
            // Fill YUV planes with fuzzer data
            memcpy(img->planes[0], data, std::min(plane_size, size));
            if (size >= plane_size * 3 / 2) {
                memcpy(img->planes[1], data + plane_size, plane_size / 4);
                memcpy(img->planes[2], data + plane_size * 5 / 4, plane_size / 4);
            }
            
            // Encode the frame
            vpx_codec_encode(&codec, img, 0, 1, 0, VPX_DL_REALTIME);
            
            // Get encoded data
            vpx_codec_iter_t iter = nullptr;
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(&codec, &iter))) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
        }
        
        free(output_buf.buf);
    }
    
    // Cleanup
    vpx_img_free(img);
    vpx_codec_destroy(&codec);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}