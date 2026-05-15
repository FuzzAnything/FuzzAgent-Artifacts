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
//<ID> 201
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 0
//<Quality> {"density":34,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_peek_stream_info","vpx_codec_err_to_string","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_destroy","vpx_codec_register_put_frame_cb","vpx_codec_error_detail","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_get_frame","vpx_codec_get_caps","vpx_codec_enc_init_multi_ver","vpx_codec_err_to_string","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_err_to_string","vpx_img_alloc","vpx_codec_destroy","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_set_cx_data_buf","vpx_codec_err_to_string","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_control_","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_peek_stream_info","vpx_codec_err_to_string","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_error_detail","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_get_frame","vpx_codec_get_caps","vpx_codec_enc_init_multi_ver","vpx_codec_enc_config_set","vpx_codec_err_to_string","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_set_cx_data_buf","vpx_codec_err_to_string","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_control_","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback for put_frame
static void  id_000048_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Do nothing in fuzzing context
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<100) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_11, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_12, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_13, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_14, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_15, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize variables
    vpx_codec_ctx_t decoder = {0}, encoder = {0};
    vpx_codec_stream_info_t stream_info = {0};
    vpx_codec_enc_cfg_t enc_cfg = {0};
    vpx_codec_dec_cfg_t dec_cfg = {0};
    vpx_image_t *img = nullptr;
    vpx_fixed_buf_t output_buf = {0};
    vpx_codec_iter_t iter = nullptr;
    vpx_codec_caps_t caps;
    vpx_rational_t dsf = {1, 1};
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    vpx_codec_err_t err = VPX_CODEC_OK;
    
    // 1. Get decoder and encoder interfaces
    vpx_codec_iface_t *dec_iface = vpx_codec_vp9_dx();
    vpx_codec_iface_t *enc_iface = vpx_codec_vp9_cx();
    
    if (!dec_iface || !enc_iface) {
	return 0;
	}
	
    
    // 2. Peek stream info from input data
    stream_info.sz = sizeof(stream_info);
    err = vpx_codec_peek_stream_info(dec_iface, data, size, &stream_info);
    if (err != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(err);
        const char *err_detail = nullptr; // Will get from context later
        (void)err_str;
        (void)err_detail;
        // Continue anyway
    }
    
    // 3. Initialize decoder
    dec_cfg.threads = 1;
    dec_cfg.w = stream_info.w;
    dec_cfg.h = stream_info.h;
    
    err = vpx_codec_dec_init_ver(&decoder, dec_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(err);
        (void)err_str;
        // Clean up and return
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // 4. Register put frame callback
    err = vpx_codec_register_put_frame_cb(&decoder,  id_000048_put_frame_callback, nullptr);
    if (err != VPX_CODEC_OK) {
        const char *err_detail = vpx_codec_error_detail(&decoder);
        (void)err_detail;
    }
    
    // 5. Decode the input data
    err = vpx_codec_decode(&decoder, data, size, nullptr, fuzz_int64_t_2);
    if (err != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(err);
        (void)err_str;
    }
    
    // 6. Get decoded frame
    img = vpx_codec_get_frame(&decoder, &iter);
    
    // 7. Get encoder capabilities
    caps = vpx_codec_get_caps(enc_iface);
    (void)caps;
    
    // 8. Set up encoder configuration
    enc_cfg.g_w = stream_info.w > 0 ? stream_info.w : 64;
    enc_cfg.g_h = stream_info.h > 0 ? stream_info.h : 64;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 256;
    enc_cfg.g_threads = 1;
    enc_cfg.g_pass = VPX_RC_ONE_PASS;
    enc_cfg.rc_end_usage = VPX_VBR;
    enc_cfg.g_error_resilient = 0;
    
    // 9. Initialize encoder with multi version
    err = vpx_codec_enc_init_multi_ver(&encoder, enc_iface, &enc_cfg, 1, fuzz_int64_t_3, &dsf, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(err);
        (void)err_str;
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // 10. Set encoder configuration
    err = vpx_codec_enc_config_set(&encoder, &enc_cfg);
    if (err != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(err);
        (void)err_str;
    }
    
    // 11. Allocate image if no decoded frame available
    if (!img) {
        img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, enc_cfg.g_w, enc_cfg.g_h, fuzz_uint32_t_4);
        if (!img) {
            vpx_codec_destroy(&encoder);
            vpx_codec_destroy(&decoder);
            return 0;
        }
        
        // Set rectangle on image
        vpx_img_set_rect(img, fuzz_uint32_t_5, fuzz_uint32_t_6, enc_cfg.g_w, enc_cfg.g_h);
        
        // Fill with some data for encoding
        if (img->planes[0]) {
            size_t y_plane_size = img->stride[0] * img->d_h;
            size_t fill_size = size < y_plane_size ? size : y_plane_size;
            if (fill_size > 0) {
                memcpy(img->planes[0], data, fill_size);
            }
        }
    }
    
    // 12. Encode the image
    err = vpx_codec_encode(&encoder, img, fuzz_int64_t_7, fuzz_uint64_t_8, fuzz_int64_t_9, fuzz_uint64_t_10);
    if (err != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(err);
        (void)err_str;
    }
    
    // 13. Allocate output buffer for encoded data
    output_buf.buf = malloc(4096);
    output_buf.sz = 4096;
    
    if (output_buf.buf) {
        // 14. Set output buffer
        err = vpx_codec_set_cx_data_buf(&encoder, &output_buf, fuzz_uint32_t_11, fuzz_uint32_t_12);
        if (err != VPX_CODEC_OK) {
            const char *err_str = vpx_codec_err_to_string(err);
            (void)err_str;
        }
        
        // 15. Get encoded data
        iter = nullptr;
        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
            // Process packet if needed
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Do something with encoded frame
                break;
            }
        }
        
        free(output_buf.buf);
    }
    
    // 16. Try to encode again with different flags
    if (img) {
        err = vpx_codec_encode(&encoder, img, fuzz_int64_t_13, fuzz_uint64_t_14, VPX_EFLAG_FORCE_KF, fuzz_uint64_t_15);
        (void)err;
        
        // Get encoded data again
        iter = nullptr;
        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
            // Just iterate through packets
        }
    }
    
    // 17. Use codec control (example: set CPU used)
    err = vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 8);
    if (err != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(err);
        (void)err_str;
    }
    
    // 18. Get error detail
    const char *err_detail = vpx_codec_error_detail(&encoder);
    (void)err_detail;
    
    // 19. Clean up
    if (img && img->self_allocd) {
        vpx_img_free(img);
    }
    
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    
    return 0;
}