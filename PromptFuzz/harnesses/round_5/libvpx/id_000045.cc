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
//<ID> 155
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 4
//<Quality> {"density":36,"unique_branches":{"dec_find_mv_refs":[[572,9,572,50,0,0,4,1],[455,11,455,49,8,0,4,1],[462,11,462,22,8,0,4,1],[455,11,455,49,13,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_alloc","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_register_put_frame_cb","vpx_codec_control_","vpx_codec_control_","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_frame_cb","vpx_codec_control_","vpx_codec_control_","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


// Include necessary libvpx headers
extern "C" {
}

// Callback function for put_frame
static void  id_000045_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    // This is just a placeholder callback
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create FILE* for input using fmemopen
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

    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    
    // Check decoder capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    (void)caps; // Use caps to avoid unused variable warning

    // Peek stream info
    vpx_codec_stream_info_t stream_info;
    stream_info.sz = sizeof(stream_info);
    vpx_codec_err_t err = vpx_codec_peek_stream_info(
        decoder_iface, data, (unsigned int)size, &stream_info);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder with default config
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = stream_info.w;
    dec_cfg.h = stream_info.h;
    
    err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, 
                                 VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Decode the input data
    err = vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, 0);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get decoded frame
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    if (!decoded_img) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Allocate an image using vpx_img_alloc
    vpx_image_t allocated_img;
    vpx_img_alloc(&allocated_img, VPX_IMG_FMT_I420, 
                  decoded_img->d_w, decoded_img->d_h, 16);
    
    // Set rectangle on allocated image
    vpx_img_set_rect(&allocated_img, 0, 0, 
                     decoded_img->d_w / 2, decoded_img->d_h / 2);

    // Initialize encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    
    // Get default encoder config
    vpx_codec_enc_cfg_t enc_cfg;
    err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(&allocated_img);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set encoder config based on decoded image
    enc_cfg.g_w = decoded_img->d_w;
    enc_cfg.g_h = decoded_img->d_h;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_pass = VPX_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;

    // Initialize encoder with single encoder
    err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0,
                                 VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(&allocated_img);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize another encoder with multi_ver (num_enc = 1)
    vpx_codec_ctx_t encoder_multi;
    vpx_rational_t dsf = {1, 1};
    err = vpx_codec_enc_init_multi_ver(&encoder_multi, encoder_iface, &enc_cfg,
                                       1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
        vpx_img_free(&allocated_img);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Register put frame callback
    err = vpx_codec_register_put_frame_cb(&encoder,  id_000045_put_frame_cb, nullptr);
    (void)err; // Result not critical for fuzzing

    // Use control API (set CPU used to 8 for speed)
    vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 8);
    vpx_codec_control_(&encoder_multi, VP8E_SET_CPUUSED, 8);

    // Encode the decoded frame
    err = vpx_codec_encode(&encoder, decoded_img, 0, 1, 0, VPX_DL_REALTIME);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder_multi);
        vpx_codec_destroy(&encoder);
        vpx_img_free(&allocated_img);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get encoded data
    vpx_codec_iter_t enc_iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }

    // Get preview frame (if available)
    const vpx_image_t *preview = vpx_codec_get_preview_frame(&encoder);
    (void)preview; // Use preview to avoid unused variable warning

    // Get global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }

    // Cleanup encoder_multi
    vpx_codec_destroy(&encoder_multi);
    
    // Cleanup encoder
    vpx_codec_destroy(&encoder);
    
    // Free allocated image
    vpx_img_free(&allocated_img);
    
    // Cleanup decoder
    vpx_codec_destroy(&decoder);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}