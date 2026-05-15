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
//<ID> 202
//<Prompt> []
/*<Combination>: [
*/
//<score> 41, nr_unique_branch: 2
//<Quality> {"density":41,"unique_branches":{"vp9_rc_set_gf_interval_range":[[24,23,24,34,5,0,4,1]],"vp9_rc_get_default_max_gf_interval":[[25,23,25,34,2,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_peek_stream_info","vpx_codec_destroy","vpx_img_alloc","vpx_codec_destroy","vpx_img_set_rect","vpx_img_free","vpx_codec_destroy","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_err_to_string","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_err_to_string","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_err_to_string","vpx_codec_get_global_headers","vpx_codec_set_cx_data_buf","vpx_codec_err_to_string","vpx_codec_register_put_frame_cb","vpx_codec_err_to_string","vpx_codec_register_put_slice_cb","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_get_frame","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_img_alloc","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_err_to_string","vpx_codec_get_global_headers","vpx_codec_set_cx_data_buf","vpx_codec_err_to_string","vpx_codec_register_put_frame_cb","vpx_codec_err_to_string","vpx_codec_register_put_slice_cb","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_get_frame","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":2}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create input FILE* using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Create output FILE*
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 3. Initialize decoder for VP9 (input is libvpx's output data)
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    
    // Check codec capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, NULL, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(err);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 4. Peek stream info to get frame dimensions
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    err = vpx_codec_peek_stream_info(decoder_iface, data, size, &si);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 5. Allocate image for decoded frame
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, si.w, si.h, 16);
    if (!img) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 6. Set rectangle on image (entire image area)
    int rect_err = vpx_img_set_rect(img, 0, 0, si.w, si.h);
    if (rect_err) {
        vpx_img_free(img);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 7. Initialize encoder for re-encoding
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    
    vpx_codec_enc_cfg_t cfg;
    err = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(err);
        vpx_img_free(img);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    cfg.g_w = si.w;
    cfg.g_h = si.h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_pass = VPX_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;

    err = vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(err);
        vpx_img_free(img);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 8. Set encoder configuration (modify a parameter)
    cfg.rc_target_bitrate = 2000;
    err = vpx_codec_enc_config_set(&encoder, &cfg);
    if (err != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(err);
    }

    // 9. Get global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
    if (global_headers && global_headers->buf && global_headers->sz > 0) {
        fwrite(global_headers->buf, 1, global_headers->sz, out_file);
    }

    // 10. Set up output buffer for compressed data
    const size_t output_buf_size = 1024 * 1024; // 1MB buffer
    std::vector<uint8_t> output_buffer(output_buf_size);
    vpx_fixed_buf_t output_buf;
    output_buf.buf = output_buffer.data();
    output_buf.sz = output_buf_size;
    
    err = vpx_codec_set_cx_data_buf(&encoder, &output_buf, 0, 16);
    if (err != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(err);
    }

    // 11. Register callbacks (dummy implementations)
    auto dummy_put_frame_cb = [](void *user_priv, const vpx_image_t *img) {};
    auto dummy_put_slice_cb = [](void *user_priv, const vpx_image_t *img, 
                                 const vpx_image_rect_t *rect, 
                                 const vpx_image_rect_t *rect2) {};
    
    err = vpx_codec_register_put_frame_cb(&encoder, dummy_put_frame_cb, NULL);
    if (err != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(err);
    }
    
    err = vpx_codec_register_put_slice_cb(&encoder, dummy_put_slice_cb, NULL);
    if (err != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(err);
    }

    // 12. Decode input data
    err = vpx_codec_decode(&decoder, data, size, NULL, 0);
    if (err != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(err);
    }

    // 13. Get decoded frame
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    if (decoded_img) {
        // 14. Copy decoded data to our image
        for (int plane = 0; plane < 3; ++plane) {
            const int stride = decoded_img->stride[plane];
            const int copy_stride = img->stride[plane];
            const int height = (plane == 0) ? decoded_img->d_h : decoded_img->d_h / 2;
            const uint8_t *src = decoded_img->planes[plane];
            uint8_t *dst = img->planes[plane];
            
            for (int y = 0; y < height; ++y) {
                memcpy(dst, src, std::min(stride, copy_stride));
                src += stride;
                dst += copy_stride;
            }
        }

        // 15. Encode the frame
        err = vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
        if (err != VPX_CODEC_OK) {
            const char *error_str = vpx_codec_err_to_string(err);
        }

        // 16. Get encoded data
        vpx_codec_iter_t enc_iter = NULL;
        const vpx_codec_cx_pkt_t *pkt;
        
        while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                fflush(out_file);
            }
        }
    }

    // 17. Flush encoder
    err = vpx_codec_encode(&encoder, NULL, -1, 1, 0, VPX_DL_REALTIME);
    if (err != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(err);
    }

    // Get remaining packets
    vpx_codec_iter_t flush_iter = NULL;
    const vpx_codec_cx_pkt_t *flush_pkt;
    while ((flush_pkt = vpx_codec_get_cx_data(&encoder, &flush_iter)) != NULL) {
        if (flush_pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            fwrite(flush_pkt->data.frame.buf, 1, flush_pkt->data.frame.sz, out_file);
        }
    }

    // 18. Cleanup
    vpx_img_free(img);
    vpx_codec_destroy(&decoder);
    vpx_codec_destroy(&encoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}