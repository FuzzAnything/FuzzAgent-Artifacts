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
//<ID> 258
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 0
//<Quality> {"density":24,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_destroy","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000067_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Do nothing - just consume the callback
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create FILE* for input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create FILE* for output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    vpx_codec_ctx_t decoder_ctx = {0};
    vpx_codec_ctx_t encoder_ctx = {0};
    vpx_codec_stream_info_t stream_info = {0};
    vpx_codec_dec_cfg_t dec_cfg = {0};
    vpx_codec_enc_cfg_t enc_cfg = {0};
    vpx_image_t *img = nullptr;
    vpx_codec_iter_t iter = nullptr;
    vpx_fixed_buf_t output_buf = {0};
    
    // Get codec interfaces
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    
    // Get codec capabilities
    vpx_codec_caps_t dec_caps = vpx_codec_get_caps(decoder_iface);
    vpx_codec_caps_t enc_caps = vpx_codec_get_caps(encoder_iface);
    (void)dec_caps;
    (void)enc_caps;
    
    // Get interface names
    const char *dec_name = vpx_codec_iface_name(decoder_iface);
    const char *enc_name = vpx_codec_iface_name(encoder_iface);
    (void)dec_name;
    (void)enc_name;
    
    // Peek stream info to get dimensions
    stream_info.sz = sizeof(stream_info);
    vpx_codec_peek_stream_info(decoder_iface, data, (unsigned int)size, &stream_info);
    
    // Initialize decoder
    dec_cfg.w = stream_info.w ? stream_info.w : 640;
    dec_cfg.h = stream_info.h ? stream_info.h : 480;
    dec_cfg.threads = 1;
    
    vpx_codec_err_t dec_init_result = vpx_codec_dec_init_ver(
        &decoder_ctx, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (dec_init_result != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Register put frame callback
    vpx_codec_register_put_frame_cb(&decoder_ctx,  id_000067_put_frame_callback, nullptr);
    
    // Allocate image for encoder
    img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                       stream_info.w ? stream_info.w : 640,
                       stream_info.h ? stream_info.h : 480,
                       1);
    
    if (!img) {
        vpx_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Decode the input data
    vpx_codec_decode(&decoder_ctx, data, (unsigned int)size, nullptr, 0);
    
    // Get stream info from decoder context
    vpx_codec_get_stream_info(&decoder_ctx, &stream_info);
    
    // Try to get frames from decoder
    const vpx_image_t *decoded_frame;
    while ((decoded_frame = vpx_codec_get_frame(&decoder_ctx, &iter))) {
        // Copy first frame to our allocated image
        if (img && decoded_frame->fmt == VPX_IMG_FMT_I420) {
            for (int plane = 0; plane < 3; plane++) {
                int stride = decoded_frame->stride[plane];
                int height = (plane == 0) ? decoded_frame->d_h : decoded_frame->d_h / 2;
                for (int y = 0; y < height; y++) {
                    memcpy(img->planes[plane] + y * img->stride[plane],
                           decoded_frame->planes[plane] + y * stride,
                           (plane == 0) ? decoded_frame->d_w : decoded_frame->d_w / 2);
                }
            }
            break; // Just use first frame
        }
    }
    
    // Initialize encoder configuration
    vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    enc_cfg.g_w = stream_info.w ? stream_info.w : 640;
    enc_cfg.g_h = stream_info.h ? stream_info.h : 480;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    
    // Initialize encoder
    vpx_codec_err_t enc_init_result = vpx_codec_enc_init_ver(
        &encoder_ctx, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    
    if (enc_init_result != VPX_CODEC_OK) {
        vpx_img_free(img);
        vpx_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Allocate output buffer for encoder
    output_buf.buf = malloc(1024 * 1024); // 1MB buffer
    output_buf.sz = 1024 * 1024;
    
    if (output_buf.buf) {
        vpx_codec_set_cx_data_buf(&encoder_ctx, &output_buf, 0, 0);
    }
    
    // Encode the frame
    if (img) {
        vpx_codec_encode(&encoder_ctx, img, 0, 1, 0, VPX_DL_REALTIME);
    }
    
    // Try to get preview frame from encoder
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&encoder_ctx);
    (void)preview_frame;
    
    // Write output buffer to file
    if (output_buf.buf && output_buf.sz > 0) {
        fwrite(output_buf.buf, 1, output_buf.sz, out_file);
    }
    
    // Clean up
    if (output_buf.buf) {
        free(output_buf.buf);
    }
    
    vpx_img_free(img);
    vpx_codec_destroy(&encoder_ctx);
    vpx_codec_destroy(&decoder_ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}