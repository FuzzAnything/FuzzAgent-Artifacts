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
//<ID> 78
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 6
//<Quality> {"density":16,"unique_branches":{"copy_and_extend_plane":[[41,9,41,29,0,0,4,1],[44,19,44,24,0,0,4,0],[44,19,44,24,0,0,4,1]],"vp8_copy_and_extend_frame":[[83,21,83,55,0,0,4,0]],"vp8_compute_frame_size_bounds":[[1458,9,1458,30,0,0,4,0]],"calc_iframe_target_size":[[319,9,319,52,0,0,4,1]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_get_global_headers","vpx_img_alloc","vpx_codec_destroy","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_get_global_headers","vpx_img_alloc","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

// Callback function for frame release
static int  id_000029_release_frame_buffer(void *user_priv, vpx_codec_frame_buffer_t *fb) {
    (void)user_priv;
    (void)fb;
    return 0;
}

// Callback function for getting frame buffer
static int  id_000029_get_frame_buffer(void *user_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    (void)user_priv;
    (void)min_size;
    (void)fb;
    return 0;
}

// Callback function for put frame
static void  id_000029_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for operations

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

    // Initialize encoder interface for VP8
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    if (!encoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get interface name
    const char *iface_name = vpx_codec_iface_name(encoder_iface);
    (void)iface_name; // Use to avoid unused variable warning

    // Initialize encoder configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set configuration parameters from fuzzer data
    cfg.g_w = (data[0] % 640) + 1; // Width between 1-640
    cfg.g_h = (data[1] % 480) + 1; // Height between 1-480
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256;
    cfg.g_threads = (data[2] % 4) + 1;

    // Create encoder context
    vpx_codec_ctx_t encoder;
    
    // Initialize encoder with single version
    err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, VPX_CODEC_USE_HIGHBITDEPTH);
    if (err != VPX_CODEC_OK) {
        // Try multi-encoder version as fallback
        vpx_rational_t dsf = {1, 1};
        err = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &cfg, 1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
    }

    // Set frame buffer functions
    err = vpx_codec_set_frame_buffer_functions(&encoder,  id_000029_get_frame_buffer,  id_000029_release_frame_buffer, NULL);
    (void)err; // Continue even if this fails

    // Register put frame callback
    err = vpx_codec_register_put_frame_cb(&encoder,  id_000029_put_frame_callback, NULL);
    (void)err; // Continue even if this fails

    // Get global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
    if (global_headers) {
        fwrite(global_headers->buf, 1, global_headers->sz, out_file);
    }

    // Allocate image for encoding
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img) {
        vpx_codec_destroy(&encoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Fill image with data from fuzzer input
    size_t data_offset = 3; // Skip first 3 bytes used for config
    size_t plane_size = cfg.g_w * cfg.g_h;
    
    for (int plane = 0; plane < 3; plane++) {
        size_t current_plane_size = plane == 0 ? plane_size : plane_size / 4;
        size_t copy_size = (size - data_offset) < current_plane_size ? 
                          (size - data_offset) : current_plane_size;
        
        if (copy_size > 0) {
            memcpy(img->planes[plane], data + data_offset, copy_size);
            data_offset += copy_size;
        }
    }

    // Encode frame
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    vpx_enc_deadline_t deadline = VPX_DL_REALTIME;

    // Set cx data buffer
    std::vector<uint8_t> cx_buffer(4096);
    vpx_fixed_buf_t buf;
    buf.buf = cx_buffer.data();
    buf.sz = cx_buffer.size();
    err = vpx_codec_set_cx_data_buf(&encoder, &buf, 0, 0);
    (void)err; // Continue even if this fails

    // Encode the image
    err = vpx_codec_encode(&encoder, img, pts, duration, flags, deadline);
    
    if (err == VPX_CODEC_OK) {
        // Get encoded data
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t *pkt;
        
        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }

        // Get preview frame
        const vpx_image_t *preview = vpx_codec_get_preview_frame(&encoder);
        (void)preview; // Use to avoid unused variable warning
    }

    // Cleanup
    vpx_img_free(img);
    vpx_codec_destroy(&encoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}