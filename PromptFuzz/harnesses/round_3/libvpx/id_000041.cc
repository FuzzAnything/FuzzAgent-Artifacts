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
//<ID> 134
//<Prompt> []
/*<Combination>: [
*/
//<score> 17.5, nr_unique_branch: 0
//<Quality> {"density":35,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_error_detail","vpx_codec_err_to_string","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_stream_info","vpx_codec_vp9_cx","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_control_","vpx_codec_control_","vpx_codec_get_frame","vpx_img_wrap","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_set_cx_data_buf","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_stream_info","vpx_codec_vp9_cx","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_control_","vpx_codec_control_","vpx_codec_get_frame","vpx_img_wrap","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_set_cx_data_buf","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

// Callback for decoded frames
static void  id_000041_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    // Do nothing in fuzzing context
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<156) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_12, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_13, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_14, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_15, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_16, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_17, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_18, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_19, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_20, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_21, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_22, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_23, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create FILE* for input using fmemopen as required
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
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Suppress unused warning
    (void)out_fd; // Suppress unused warning
    
    // Initialize decoder
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    const char *iface_name = vpx_codec_iface_name(decoder_iface);
    (void)iface_name; // Suppress unused warning
    
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 0;
    dec_cfg.h = 0;
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&decoder);
        (void)error_detail; // Suppress unused warning
        const char *error_str = vpx_codec_err_to_string(res);
        (void)error_str; // Suppress unused warning
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Register callback
    res = vpx_codec_register_put_frame_cb(&decoder,  id_000041_put_frame_cb, nullptr);
    (void)res; // Suppress unused warning
    
    // Peek stream info
    vpx_codec_stream_info_t si;
    si.sz = sizeof(si);
    res = vpx_codec_peek_stream_info(decoder_iface, data, (unsigned int)size, &si);
    
    // Decode the input data
    res = vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, fuzz_int64_t_2);
    if (res != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&decoder);
        (void)error_detail; // Suppress unused warning
    }
    
    // Get stream info after decoding
    res = vpx_codec_get_stream_info(&decoder, &si);
    
    // Get decoded frames
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t *decoded_img = nullptr;
    
    // Initialize encoder
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_caps_t caps = vpx_codec_get_caps(encoder_iface);
    (void)caps; // Suppress unused warning
    
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    
    // Default encoder config
    res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_3);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set reasonable dimensions based on input or defaults
    if (si.w > 0 && si.h > 0) {
        enc_cfg.g_w = si.w;
        enc_cfg.g_h = si.h;
    } else {
        enc_cfg.g_w = 640;
        enc_cfg.g_h = 480;
    }
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.g_threads = 1;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_error_resilient = 0;
    
    // Initialize encoder
    res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_4, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Try multi-encoder init (though we won't use it fully)
    vpx_codec_ctx_t multi_encoder;
    vpx_rational_t dsf = {1, 1};
    res = vpx_codec_enc_init_multi_ver(&multi_encoder, encoder_iface, &enc_cfg, 1, fuzz_int64_t_5, &dsf, VPX_ENCODER_ABI_VERSION);
    if (res == VPX_CODEC_OK) {
        vpx_codec_destroy(&multi_encoder);
    }
    
    // Set encoder config
    res = vpx_codec_enc_config_set(&encoder, &enc_cfg);
    
    // Use control API
    vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 8);
    vpx_codec_control_(&encoder, VP8E_SET_ENABLEAUTOALTREF, 0);
    
    // Process decoded frames
    while ((decoded_img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Create image wrapper
        vpx_image_t img_wrapper;
        vpx_img_wrap(&img_wrapper, decoded_img->fmt, decoded_img->d_w, decoded_img->d_h, 
                    fuzz_uint32_t_6, decoded_img->img_data);
        
        // Create allocated image
        vpx_image_t *allocated_img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                                  decoded_img->d_w, decoded_img->d_h, fuzz_uint32_t_7);
        if (allocated_img) {
            // Set rectangle on allocated image
            unsigned int rect_w = decoded_img->d_w > 10 ? 10 : decoded_img->d_w;
            unsigned int rect_h = decoded_img->d_h > 10 ? 10 : decoded_img->d_h;
            vpx_img_set_rect(allocated_img, fuzz_uint32_t_8, fuzz_uint32_t_9, rect_w, rect_h);
            
            // Encode both images
            res = vpx_codec_encode(&encoder, &img_wrapper, fuzz_int64_t_10, fuzz_uint64_t_11, fuzz_int64_t_12, fuzz_uint64_t_13);
            res = vpx_codec_encode(&encoder, allocated_img, fuzz_int64_t_14, fuzz_uint64_t_15, fuzz_int64_t_16, fuzz_uint64_t_17);
            
            // Get encoded data
            vpx_codec_iter_t enc_iter = nullptr;
            const vpx_codec_cx_pkt_t *pkt = nullptr;
            while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    // Write to output file
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
            
            // Get preview frame
            const vpx_image_t *preview = vpx_codec_get_preview_frame(&encoder);
            (void)preview; // Suppress unused warning
            
            // Free allocated image
            vpx_img_free(allocated_img);
        }
    }
    
    // Flush encoder
    res = vpx_codec_encode(&encoder, nullptr, fuzz_int64_t_18, fuzz_uint64_t_19, fuzz_int64_t_20, fuzz_uint64_t_21);
    
    // Get remaining encoded data
    vpx_codec_iter_t enc_iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
        }
    }
    
    // Set CX data buffer (dummy usage)
    vpx_fixed_buf_t buf;
    buf.buf = malloc(100);
    buf.sz = 100;
    if (buf.buf) {
        vpx_codec_set_cx_data_buf(&encoder, &buf, fuzz_uint32_t_22, fuzz_uint32_t_23);
        free(buf.buf);
    }
    
    // Cleanup
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}