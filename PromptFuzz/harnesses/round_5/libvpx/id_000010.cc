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
//<ID> 19
//<Prompt> []
/*<Combination>: [
*/
//<score> 46, nr_unique_branch: 2
//<Quality> {"density":23,"unique_branches":{"dec_build_inter_predictors":[[662,54,662,73,0,0,4,1],[663,7,663,27,0,0,4,0]]},"library_calls":["vpx_codec_version_str","vpx_codec_build_config","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_error_detail","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_destroy","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_get_global_headers","vpx_codec_register_put_slice_cb","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_str","vpx_codec_build_config","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_codec_register_put_slice_cb","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


// Dummy callback function for vpx_codec_register_put_slice_cb
static void  id_000010_dummy_put_slice_cb(void *user_priv, 
                               const vpx_image_t *img,
                               const vpx_image_rect_t *slice,
                               const vpx_image_rect_t *clipped) {
    // Do nothing - just a placeholder to use the API
    (void)user_priv;
    (void)img;
    (void)slice;
    (void)clipped;
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Return early if input is too small
    if (size < 10) {
        return 0;
    }

    // Use the input data as a VP9 encoded bitstream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    int in_fd = fuzz_fileno(in_file);

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    int out_fd = fuzz_fileno(out_file);

    // Get version strings
    const char *version_str = vpx_codec_version_str();
    const char *build_config = vpx_codec_build_config();
    (void)version_str;
    (void)build_config;

    // DECODING PHASE
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    
    // Get decoder capabilities
    vpx_codec_caps_t decoder_caps = vpx_codec_get_caps(decoder_iface);
    (void)decoder_caps;
    
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 640;
    dec_cfg.h = 480;
    
    // Initialize decoder
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, 
                                                &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&decoder);
        (void)error_detail;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Decode the input data
    err = vpx_codec_decode(&decoder, data, size, NULL, 0);
    if (err != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&decoder);
        (void)error_detail;
    }
    
    // Get decoded frame
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *decoded_frame = vpx_codec_get_frame(&decoder, &iter);
    
    // ENCODING PHASE (if we got a decoded frame)
    if (decoded_frame) {
        // Allocate image for encoder
        vpx_image_t img;
        if (vpx_img_alloc(&img, VPX_IMG_FMT_I420, 
                         decoded_frame->d_w, decoded_frame->d_h, 16) == NULL) {
            vpx_codec_destroy(&decoder);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
        }
        
        // Initialize encoder
        vpx_codec_ctx_t encoder;
        vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
        
        vpx_codec_enc_cfg_t enc_cfg;
        err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        if (err != VPX_CODEC_OK) {
            vpx_img_free(&img);
            vpx_codec_destroy(&decoder);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
        }
        
        enc_cfg.g_w = decoded_frame->d_w;
        enc_cfg.g_h = decoded_frame->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 256;
        
        err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 
                                    0, VPX_ENCODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            vpx_img_free(&img);
            vpx_codec_destroy(&decoder);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
        }
        
        // Get global headers
        vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
        if (global_headers && global_headers->buf && global_headers->sz > 0) {
            fwrite(global_headers->buf, 1, global_headers->sz, out_file);
        }
        
        // Register put slice callback
        vpx_codec_register_put_slice_cb(&encoder,  id_000010_dummy_put_slice_cb, NULL);
        
        // Encode the frame
        err = vpx_codec_encode(&encoder, decoded_frame, 0, 1, 0, 0);
        if (err == VPX_CODEC_OK) {
            // Get encoded data
            vpx_codec_iter_t enc_iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
        }
        
        // Cleanup encoder
        vpx_codec_destroy(&encoder);
        vpx_img_free(&img);
    }
    
    // Cleanup decoder
    vpx_codec_destroy(&decoder);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}