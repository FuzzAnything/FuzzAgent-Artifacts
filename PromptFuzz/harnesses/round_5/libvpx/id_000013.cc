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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 0
//<Quality> {"density":34,"unique_branches":{},"library_calls":["vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy","vpx_img_wrap","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_register_put_slice_cb","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_img_set_rect","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_slice_cb","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_slice
static void  id_000013_put_slice_callback(void *user_priv, 
                              const vpx_image_t *img,
                              const vpx_image_rect_t *slice,
                              const vpx_image_rect_t *update) {
    // Empty callback for fuzzing
    (void)user_priv;
    (void)img;
    (void)slice;
    (void)update;
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        return 0;
    }

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

    vpx_codec_ctx_t decoder;
    vpx_codec_ctx_t encoder;
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt = NULL;
    
    // Initialize decoder for VP8
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, 
                                                 vpx_codec_vp8_dx(), 
                                                 &dec_cfg, 
                                                 0, 
                                                 VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        // Try VP9 if VP8 fails
        res = vpx_codec_dec_init_ver(&decoder, 
                                     vpx_codec_vp9_dx(), 
                                     &dec_cfg, 
                                     0, 
                                     VPX_DECODER_ABI_VERSION);
        if (res != VPX_CODEC_OK) {
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
    }

    // Get codec interface name
    const char *iface_name = vpx_codec_iface_name(decoder.iface);
    (void)iface_name; // Use variable to avoid unused warning

    // Decode the input data
    res = vpx_codec_decode(&decoder, data, size, NULL, 0);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get decoded frame
    vpx_image_t *img = vpx_codec_get_frame(&decoder, &iter);
    if (!img) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create a new image by wrapping existing data
    vpx_image_t wrapped_img;
    unsigned char *img_data = (unsigned char *)malloc(img->d_w * img->d_h * 3 / 2);
    if (!img_data) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Copy data from original image
    memcpy(img_data, img->planes[0], img->d_w * img->d_h);
    memcpy(img_data + img->d_w * img->d_h, img->planes[1], img->d_w * img->d_h / 4);
    memcpy(img_data + img->d_w * img->d_h * 5 / 4, img->planes[2], img->d_w * img->d_h / 4);

    vpx_image_t *wrapped = vpx_img_wrap(&wrapped_img, 
                                       VPX_IMG_FMT_I420, 
                                       img->d_w, 
                                       img->d_h, 
                                       1, 
                                       img_data);
    if (!wrapped) {
        free(img_data);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set a rectangle on the image (use first 100x100 pixels if possible)
    unsigned int rect_w = (img->d_w > 100) ? 100 : img->d_w;
    unsigned int rect_h = (img->d_h > 100) ? 100 : img->d_h;
    int rect_res = vpx_img_set_rect(wrapped, 0, 0, rect_w, rect_h);
    (void)rect_res; // Use variable to avoid unused warning

    // Initialize encoder configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    
    res = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        vpx_img_free(wrapped);
        free(img_data);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    cfg.g_w = rect_w;
    cfg.g_h = rect_h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256;
    cfg.g_threads = 1;

    // Initialize multi-encoder (using 1 encoder for simplicity)
    vpx_rational_t dsf = {1, 1};
    res = vpx_codec_enc_init_multi_ver(&encoder, 
                                       encoder_iface, 
                                       &cfg, 
                                       1,  // num_enc
                                       0,  // flags
                                       &dsf, 
                                       VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        vpx_img_free(wrapped);
        free(img_data);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Register put_slice callback
    res = vpx_codec_register_put_slice_cb(&encoder,  id_000013_put_slice_callback, NULL);
    (void)res; // Use variable to avoid unused warning

    // Create buffer for encoded data
    vpx_fixed_buf_t buf;
    buf.sz = 1024 * 1024; // 1MB buffer
    buf.buf = malloc(buf.sz);
    if (!buf.buf) {
        vpx_codec_destroy(&encoder);
        vpx_img_free(wrapped);
        free(img_data);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set buffer for encoded data
    res = vpx_codec_set_cx_data_buf(&encoder, &buf, 0, 0);
    (void)res; // Use variable to avoid unused warning

    // Encode the image
    res = vpx_codec_encode(&encoder, wrapped, 0, 1, 0, VPX_DL_REALTIME);
    if (res != VPX_CODEC_OK) {
        free(buf.buf);
        vpx_codec_destroy(&encoder);
        vpx_img_free(wrapped);
        free(img_data);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get encoded data
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Write encoded data to output file
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }

    // Get preview frame
    const vpx_image_t *preview = vpx_codec_get_preview_frame(&encoder);
    if (preview) {
        // Could write preview frame data if needed
        (void)preview;
    }

    // Cleanup
    free(buf.buf);
    vpx_codec_destroy(&encoder);
    vpx_img_free(wrapped);
    free(img_data);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}