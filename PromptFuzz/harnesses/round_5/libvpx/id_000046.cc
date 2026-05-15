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
//<ID> 159
//<Prompt> []
/*<Combination>: [
*/
//<score> 324, nr_unique_branch: 48
//<Quality> {"density":36,"unique_branches":{"scaledconvolve_vert_w4":[[925,15,925,20,0,0,4,0],[925,15,925,20,0,0,4,1],[929,9,929,27,0,0,4,0]],"vpx_scaled_2d_ssse3":[[1054,7,1054,13,0,0,4,0],[1054,7,1054,13,0,0,4,1],[1064,7,1064,14,0,0,4,0],[1064,7,1064,14,0,0,4,1],[1067,14,1067,20,0,0,4,0],[1067,14,1067,20,0,0,4,1]],"scaledconvolve_vert_w8":[[958,15,958,20,0,0,4,0],[958,15,958,20,0,0,4,1],[961,9,961,27,0,0,4,0]],"scaledconvolve_horiz_w8":[[786,17,786,22,0,0,4,0],[786,17,786,22,0,0,4,1],[788,19,788,24,0,0,4,0],[788,19,788,24,0,0,4,1],[791,13,791,31,0,0,4,0],[791,13,791,31,0,0,4,1],[795,23,795,28,0,0,4,0],[795,23,795,28,0,0,4,1],[808,12,808,18,0,0,4,0],[808,12,808,18,0,0,4,1]],"scaledconvolve_horiz_w4":[[859,15,859,20,0,0,4,0],[859,15,859,20,0,0,4,1],[861,17,861,22,0,0,4,0],[861,17,861,22,0,0,4,1],[863,19,863,24,0,0,4,0],[863,19,863,24,0,0,4,1],[866,13,866,31,0,0,4,0],[866,13,866,31,0,0,4,1],[870,23,870,28,0,0,4,0],[870,23,870,28,0,0,4,1]],"dec_find_mv_refs":[[548,16,548,57,0,0,4,0],[583,9,583,51,0,0,4,0],[584,9,584,50,0,0,4,0],[585,9,585,69,0,0,4,0],[587,11,588,35,0,0,4,1],[454,9,454,20,9,0,4,1],[462,11,462,22,9,0,4,0],[462,11,462,22,9,0,4,1],[462,11,462,22,13,0,4,1],[454,9,454,20,15,0,4,0],[455,11,455,49,15,0,4,0]],"filter_vert_w16_ssse3":[[978,15,978,20,0,0,4,0],[978,15,978,20,0,0,4,1]],"scaledconvolve_vert_w16":[[1012,15,1012,20,0,0,4,0],[1012,15,1012,20,0,0,4,1],[1015,9,1015,27,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_alloc","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_register_put_frame_cb","vpx_codec_control_","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_get_cx_data","vpx_codec_get_global_headers","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_control_","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_global_headers","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Open input as file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Get decoder interface
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    if (!decoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Get decoder capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);

    // Peek stream info
    vpx_codec_stream_info_t stream_info;
    stream_info.sz = sizeof(stream_info);
    vpx_codec_err_t err = vpx_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = stream_info.w;
    dec_cfg.h = stream_info.h;
    
    err = vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Decode the input
    err = vpx_codec_decode(&decoder_ctx, data, size, nullptr, 0);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Get decoded frame
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder_ctx, &iter);
    
    if (!decoded_img) {
        vpx_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Allocate new image for encoding
    vpx_image_t *enc_img = vpx_img_alloc(nullptr, decoded_img->fmt, 
                                         decoded_img->d_w, decoded_img->d_h, 16);
    if (!enc_img) {
        vpx_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Copy decoded image to encoder image
    for (int plane = 0; plane < 3; ++plane) {
        size_t plane_size = (plane == 0) ? 
            decoded_img->d_w * decoded_img->d_h : 
            (decoded_img->d_w >> decoded_img->x_chroma_shift) * 
            (decoded_img->d_h >> decoded_img->y_chroma_shift);
        
        if (decoded_img->planes[plane] && enc_img->planes[plane]) {
            memcpy(enc_img->planes[plane], decoded_img->planes[plane], plane_size);
        }
    }

    // Set a rectangle on the image
    unsigned int rect_w = decoded_img->d_w / 2;
    unsigned int rect_h = decoded_img->d_h / 2;
    vpx_img_set_rect(enc_img, 0, 0, rect_w, rect_h);

    // Get encoder interface
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    if (!encoder_iface) {
        vpx_img_free(enc_img);
        vpx_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize encoder
    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_enc_cfg_t enc_cfg;
    
    err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(enc_img);
        vpx_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    enc_cfg.g_w = decoded_img->d_w;
    enc_cfg.g_h = decoded_img->d_h;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_pass = VPX_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;

    // Try both encoder init functions
    vpx_codec_ctx_t multi_encoder_ctx;
    vpx_rational_t dsf = {1, 1};
    
    // Use multi encoder init (num_enc = 1)
    err = vpx_codec_enc_init_multi_ver(&multi_encoder_ctx, encoder_iface, &enc_cfg, 
                                       1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
    
    // Also use single encoder init
    err = vpx_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&multi_encoder_ctx);
        vpx_img_free(enc_img);
        vpx_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Register put frame callback
    auto dummy_callback = [](void*, const vpx_image_t*) {};
    vpx_codec_register_put_frame_cb(&encoder_ctx, dummy_callback, nullptr);

    // Try to use control API
    vpx_codec_control_(&encoder_ctx, VP8E_SET_CPUUSED); // Using a known control ID

    // Encode the image
    err = vpx_codec_encode(&encoder_ctx, enc_img, 0, 1, 0, VPX_DL_REALTIME);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&multi_encoder_ctx);
        vpx_codec_destroy(&encoder_ctx);
        vpx_img_free(enc_img);
        vpx_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Get encoded data
    vpx_codec_iter_t enc_iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Packet contains encoded frame data
        }
    }

    // Get global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder_ctx);
    if (global_headers) {
        // Global headers available
    }

    // Get preview frame
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&encoder_ctx);
    if (preview_frame) {
        // Preview frame available
    }

    // Clean up
    vpx_codec_destroy(&multi_encoder_ctx);
    vpx_codec_destroy(&encoder_ctx);
    vpx_img_free(enc_img);
    vpx_codec_destroy(&decoder_ctx);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}