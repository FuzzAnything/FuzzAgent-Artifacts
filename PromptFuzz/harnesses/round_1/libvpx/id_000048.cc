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
//<ID> 161
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_img_alloc","vpx_codec_destroy","vpx_codec_get_stream_info","vpx_codec_decode","vpx_img_free","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_register_put_frame_cb","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_set_cx_data_buf","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_img_alloc","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_set_cx_data_buf","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


// Dummy callback function for frame output
static void  id_000048_dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Use fmemopen to create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    vpx_codec_ctx_t decoder_ctx = {0};
    vpx_codec_ctx_t encoder_ctx = {0};
    vpx_image_t *img = nullptr;
    vpx_fixed_buf_t buf = {nullptr, 0};
    vpx_codec_stream_info_t si = {0};
    vpx_codec_iter_t iter = nullptr;
    vpx_codec_cx_pkt_t *pkt = nullptr;
    int ret = 0;
    
    // Get VP9 decoder interface
    const vpx_codec_iface_t *dec_iface = vpx_codec_vp9_dx();
    if (!dec_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get VP8 encoder interface
    const vpx_codec_iface_t *enc_iface = vpx_codec_vp8_cx();
    if (!enc_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get capabilities of decoder interface
    vpx_codec_caps_t dec_caps = vpx_codec_get_caps(dec_iface);
    (void)dec_caps;
    
    // Get capabilities of encoder interface
    vpx_codec_caps_t enc_caps = vpx_codec_get_caps(enc_iface);
    (void)enc_caps;
    
    // Get interface names
    const char *dec_name = vpx_codec_iface_name(dec_iface);
    const char *enc_name = vpx_codec_iface_name(enc_iface);
    (void)dec_name;
    (void)enc_name;
    
    // Peek stream info from input data
    si.sz = sizeof(si);
    vpx_codec_err_t err = vpx_codec_peek_stream_info(dec_iface, data, (unsigned int)size, &si);
    (void)err;
    
    // Initialize decoder with default configuration
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.w = si.w ? si.w : 128;
    dec_cfg.h = si.h ? si.h : 128;
    dec_cfg.threads = 1;
    
    err = vpx_codec_dec_init_ver(&decoder_ctx, dec_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Allocate image for dummy encoding
    img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 128, 128, 1);
    if (!img) {
        vpx_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize dummy image data with zeros
    if (img->img_data) {
        memset(img->img_data, 0, img->bps * 128 * 128 / 8);
    }
    
    // Get stream info from decoder context
    err = vpx_codec_get_stream_info(&decoder_ctx, &si);
    (void)err;
    
    // Decode the input data
    err = vpx_codec_decode(&decoder_ctx, data, (unsigned int)size, nullptr, 0);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        vpx_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get decoded frames
    const vpx_image_t *decoded_img;
    while ((decoded_img = vpx_codec_get_frame(&decoder_ctx, &iter)) != nullptr) {
        // Process decoded frame if needed
        (void)decoded_img;
    }
    
    // Reinitialize iterator for encoder data
    iter = nullptr;
    
    // Initialize encoder (using a simple configuration)
    vpx_codec_enc_cfg_t enc_cfg;
    err = vpx_codec_enc_config_default(enc_iface, &enc_cfg, 0);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        vpx_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    enc_cfg.g_w = 128;
    enc_cfg.g_h = 128;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    
    err = vpx_codec_enc_init_ver(&encoder_ctx, enc_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        vpx_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Register put frame callback
    err = vpx_codec_register_put_frame_cb(&encoder_ctx,  id_000048_dummy_put_frame_cb, nullptr);
    (void)err;
    
    // Get global headers
    vpx_fixed_buf_t *headers = vpx_codec_get_global_headers(&encoder_ctx);
    if (headers) {
        free(headers->buf);
        free(headers);
    }
    
    // Encode a dummy frame
    err = vpx_codec_encode(&encoder_ctx, img, 0, 1, 0, VPX_DL_REALTIME);
    (void)err;
    
    // Set output buffer for encoded data
    buf.sz = 4096;
    buf.buf = malloc(buf.sz);
    if (buf.buf) {
        err = vpx_codec_set_cx_data_buf(&encoder_ctx, &buf, 0, 0);
        (void)err;
    }
    
    // Get encoded data packets
    while ((pkt = (vpx_codec_cx_pkt_t *)vpx_codec_get_cx_data(&encoder_ctx, &iter)) != nullptr) {
        // Process encoded packet if needed
        (void)pkt;
    }
    
    // Cleanup
    if (buf.buf) {
        free(buf.buf);
    }
    vpx_codec_destroy(&encoder_ctx);
    vpx_img_free(img);
    vpx_codec_destroy(&decoder_ctx);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}