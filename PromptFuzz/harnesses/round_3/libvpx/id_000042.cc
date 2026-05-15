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
//<ID> 179
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 0
//<Quality> {"density":32,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_error_detail","vpx_codec_register_put_slice_cb","vpx_codec_peek_stream_info","vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_enc_init_ver","vpx_codec_error","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_control_","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_img_alloc","vpx_img_wrap","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_set_cx_data_buf","vpx_img_free","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_peek_stream_info","vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_control_","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_img_alloc","vpx_img_wrap","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_set_cx_data_buf","vpx_img_free","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_slice
static void  id_000042_put_slice_callback(void *user_priv,
                               const vpx_image_t *img,
                               const vpx_image_rect_t *rect,
                               const vpx_image_rect_t *rect2) {
    // Empty callback for fuzzing
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<92) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
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
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_14, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_15, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file for encoded data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    (void)fd_in; // Mark as used
    (void)fd_out; // Mark as used

    // DECODER SETUP
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    const char *decoder_name = vpx_codec_iface_name(decoder_iface);
    (void)decoder_name; // Mark as used
    
    vpx_codec_caps_t decoder_caps = vpx_codec_get_caps(decoder_iface);
    (void)decoder_caps; // Mark as used

    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 320;
    dec_cfg.h = 240;
    
    vpx_codec_err_t dec_init_result = vpx_codec_dec_init_ver(
        &decoder_ctx, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    
    if (dec_init_result != VPX_CODEC_OK) {
        const char *dec_error = vpx_codec_error(&decoder_ctx);
        const char *dec_error_detail = vpx_codec_error_detail(&decoder_ctx);
        (void)dec_error; // Mark as used
        (void)dec_error_detail; // Mark as used
        
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Register put_slice callback
    vpx_codec_err_t register_result = vpx_codec_register_put_slice_cb(
        &decoder_ctx,  id_000042_put_slice_callback, nullptr);
    (void)register_result; // Mark as used

    // Peek stream info from input data
    vpx_codec_stream_info_t stream_info = {0};
    stream_info.sz = sizeof(stream_info);
    
    vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
        decoder_iface, data, (unsigned int)size, &stream_info);
    (void)peek_result; // Mark as used

    // ENCODER SETUP
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    const char *encoder_name = vpx_codec_iface_name(encoder_iface);
    (void)encoder_name; // Mark as used
    
    vpx_codec_caps_t encoder_caps = vpx_codec_get_caps(encoder_iface);
    (void)encoder_caps; // Mark as used

    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_enc_cfg_t enc_cfg;
    
    // Get default encoder configuration
    vpx_codec_err_t enc_config_default_result = vpx_codec_enc_config_default(
        encoder_iface, &enc_cfg, fuzz_uint32_t_2);
    
    if (enc_config_default_result != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Set encoder configuration based on stream info
    enc_cfg.g_w = stream_info.w > 0 ? stream_info.w : 320;
    enc_cfg.g_h = stream_info.h > 0 ? stream_info.h : 240;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 256;
    enc_cfg.g_pass = VPX_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    enc_cfg.kf_mode = VPX_KF_AUTO;
    enc_cfg.kf_min_dist = 0;
    enc_cfg.kf_max_dist = 9999;

    // Initialize encoder with multi_ver (num_enc = 1 for single encoder)
    vpx_rational_t dsf = {1, 1};
    vpx_codec_err_t enc_init_result = vpx_codec_enc_init_multi_ver(
        &encoder_ctx, encoder_iface, &enc_cfg, 1, fuzz_int64_t_3, &dsf, VPX_ENCODER_ABI_VERSION);
    
    if (enc_init_result != VPX_CODEC_OK) {
        // Try with regular init
        enc_init_result = vpx_codec_enc_init_ver(
            &encoder_ctx, encoder_iface, &enc_cfg, fuzz_int64_t_4, VPX_ENCODER_ABI_VERSION);
    }

    if (enc_init_result != VPX_CODEC_OK) {
        const char *enc_error = vpx_codec_error(&encoder_ctx);
        (void)enc_error; // Mark as used
        vpx_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Set encoder configuration after init
    vpx_codec_err_t enc_config_set_result = vpx_codec_enc_config_set(
        &encoder_ctx, &enc_cfg);
    (void)enc_config_set_result; // Mark as used

    // Use codec control
    vpx_codec_err_t control_result = vpx_codec_control_(&encoder_ctx, VP8E_SET_CPUUSED, 8);
    (void)control_result; // Mark as used

    // MAIN PROCESSING LOOP
    vpx_codec_iter_t decoder_iter = nullptr;
    vpx_codec_iter_t encoder_iter = nullptr;
    
    // Decode input data
    vpx_codec_err_t decode_result = vpx_codec_decode(
        &decoder_ctx, data, (unsigned int)size, nullptr, fuzz_int64_t_5);
    (void)decode_result; // Mark as used

    // Get stream info after decoding
    vpx_codec_err_t get_stream_info_result = vpx_codec_get_stream_info(
        &decoder_ctx, &stream_info);
    (void)get_stream_info_result; // Mark as used

    // Process decoded frames
    vpx_image_t *decoded_frame = nullptr;
    int frame_count = 0;
    const int max_frames = 10; // Limit frames for fuzzing
    
    while ((decoded_frame = vpx_codec_get_frame(&decoder_ctx, &decoder_iter)) != nullptr &&
           frame_count < max_frames) {
        
        // Create image for encoding
        vpx_image_t *img_to_encode = vpx_img_alloc(
            nullptr, VPX_IMG_FMT_I420, 
            decoded_frame->d_w, decoded_frame->d_h, fuzz_uint32_t_6);
        
        if (!img_to_encode) {
            // Try vpx_img_wrap
            unsigned char *img_data = (unsigned char *)malloc(
                decoded_frame->d_w * decoded_frame->d_h * 3 / 2);
            if (img_data) {
                img_to_encode = vpx_img_wrap(
                    nullptr, VPX_IMG_FMT_I420,
                    decoded_frame->d_w, decoded_frame->d_h, fuzz_uint32_t_7, img_data);
            }
        }
        
        if (!img_to_encode) {
            break;
        }

        // Set rectangle on image
        int set_rect_result = vpx_img_set_rect(
            img_to_encode, fuzz_uint32_t_8, fuzz_uint32_t_9, 
            decoded_frame->d_w, decoded_frame->d_h);
        (void)set_rect_result; // Mark as used

        // Copy data from decoded frame (simplified - just use random data for fuzzing)
        // In real usage, you would copy actual frame data
        
        // Encode the frame
        vpx_codec_err_t encode_result = vpx_codec_encode(
            &encoder_ctx, img_to_encode, 
            fuzz_int64_t_10, fuzz_uint64_t_11, fuzz_int64_t_12, fuzz_uint64_t_13);
        (void)encode_result; // Mark as used

        // Get encoded data
        const vpx_codec_cx_pkt_t *pkt = nullptr;
        while ((pkt = vpx_codec_get_cx_data(&encoder_ctx, &encoder_iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Write to output file
                if (out_file) {
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
        }

        // Get preview frame
        const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&encoder_ctx);
        (void)preview_frame; // Mark as used

        // Set cx data buffer
        vpx_fixed_buf_t buf = {nullptr, 0};
        vpx_codec_err_t set_buf_result = vpx_codec_set_cx_data_buf(
            &encoder_ctx, &buf, fuzz_uint32_t_14, fuzz_uint32_t_15);
        (void)set_buf_result; // Mark as used

        // Clean up
        vpx_img_free(img_to_encode);
        frame_count++;
    }

    // Get global headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder_ctx);
    (void)global_headers; // Mark as used

    // Cleanup
    vpx_codec_destroy(&decoder_ctx);
    vpx_codec_destroy(&encoder_ctx);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}