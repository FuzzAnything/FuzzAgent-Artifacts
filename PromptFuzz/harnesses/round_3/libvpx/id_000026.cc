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
//<ID> 72
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":24,"unique_branches":{"vpx_codec_enc_init_ver":[[37,30,37,34,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_enc_init_ver","vpx_codec_get_caps","vpx_codec_enc_config_set","vpx_codec_enc_init_multi_ver","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_set_cx_data_buf","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_control_","vpx_codec_destroy","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_enc_init_ver","vpx_codec_get_caps","vpx_codec_enc_config_set","vpx_codec_enc_init_multi_ver","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_set_cx_data_buf","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_control_","vpx_codec_destroy","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_destroy"],"visited":2}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000026_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    // Do nothing in fuzzing context
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<88) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_12, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
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
	
    
    // Create FILE* for output
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
    (void)in_fd; // Mark as used
    (void)out_fd; // Mark as used
    
    // Initialize decoder interface
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    
    // Initialize decoder context
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 0;
    dec_cfg.h = 0;
    
    vpx_codec_err_t dec_init_result = vpx_codec_dec_init_ver(
        &decoder_ctx, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    
    // Use error handling APIs
    const char *err_str = vpx_codec_err_to_string(dec_init_result);
    (void)err_str;
    
    // Register put_frame callback
    vpx_codec_err_t reg_result = vpx_codec_register_put_frame_cb(
        &decoder_ctx,  id_000026_put_frame_cb, nullptr);
    (void)reg_result;
    
    // Peek stream info
    vpx_codec_stream_info_t stream_info = {0};
    stream_info.sz = sizeof(stream_info);
    vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
        decoder_iface, data, (unsigned int)size, &stream_info);
    
    if (peek_result == VPX_CODEC_OK) {
        // Decode the input data
        vpx_codec_err_t decode_result = vpx_codec_decode(
            &decoder_ctx, data, (unsigned int)size, nullptr, fuzz_int64_t_2);
        (void)decode_result;
        
        // Get stream info after decoding
        vpx_codec_err_t get_stream_result = vpx_codec_get_stream_info(
            &decoder_ctx, &stream_info);
        (void)get_stream_result;
        
        // Initialize encoder context
        vpx_codec_ctx_t encoder_ctx;
        vpx_codec_enc_cfg_t enc_cfg;
        
        // Initialize encoder with default config
        vpx_codec_err_t enc_init_result = vpx_codec_enc_init_ver(
            &encoder_ctx, encoder_iface, nullptr, fuzz_int64_t_3, VPX_ENCODER_ABI_VERSION);
        
        if (enc_init_result == VPX_CODEC_OK) {
            // Get encoder caps
            vpx_codec_caps_t caps = vpx_codec_get_caps(encoder_iface);
            (void)caps;
            
            // Try to set encoder config
            vpx_codec_err_t config_set_result = vpx_codec_enc_config_set(
                &encoder_ctx, nullptr);
            (void)config_set_result;
            
            // Try multi-encoder initialization
            vpx_codec_ctx_t multi_encoder_ctx;
            vpx_rational_t dsf = {1, 1};
            vpx_codec_err_t multi_init_result = vpx_codec_enc_init_multi_ver(
                &multi_encoder_ctx, encoder_iface, nullptr, 1, fuzz_int64_t_4, &dsf,
                VPX_ENCODER_ABI_VERSION);
            
            if (multi_init_result == VPX_CODEC_OK) {
                // Get frames from decoder
                vpx_codec_iter_t iter = nullptr;
                const vpx_image_t *img = vpx_codec_get_frame(&decoder_ctx, &iter);
                
                if (img) {
                    // Create a copy of the image
                    vpx_image_t *img_copy = vpx_img_alloc(nullptr, img->fmt, 
                                                         img->d_w, img->d_h, fuzz_uint32_t_5);
                    
                    if (img_copy) {
                        // Set rectangle on image
                        int set_rect_result = vpx_img_set_rect(
                            img_copy, fuzz_uint32_t_6, fuzz_uint32_t_7, 
                            img->d_w > 10 ? 10 : img->d_w,
                            img->d_h > 10 ? 10 : img->d_h);
                        (void)set_rect_result;
                        
                        // Encode the image
                        vpx_codec_err_t encode_result = vpx_codec_encode(
                            &encoder_ctx, img_copy, fuzz_int64_t_8, fuzz_uint64_t_9, fuzz_int64_t_10, fuzz_uint64_t_11);
                        
                        if (encode_result == VPX_CODEC_OK) {
                            // Set output buffer for encoded data
                            vpx_fixed_buf_t output_buf = {nullptr, 0};
                            vpx_codec_err_t set_buf_result = 
                                vpx_codec_set_cx_data_buf(&encoder_ctx, 
                                                         &output_buf, fuzz_uint32_t_12, fuzz_uint32_t_13);
                            (void)set_buf_result;
                            
                            // Get encoded data
                            vpx_codec_iter_t enc_iter = nullptr;
                            const vpx_codec_cx_pkt_t *pkt;
                            
                            while ((pkt = vpx_codec_get_cx_data(&encoder_ctx, 
                                                               &enc_iter)) != nullptr) {
                                // Write to output file
                                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                                    fwrite(pkt->data.frame.buf, 1, 
                                           pkt->data.frame.sz, out_file);
                                }
                            }
                        }
                        
                        // Free the image copy
                        vpx_img_free(img_copy);
                    }
                }
                
                // Try control API
                vpx_codec_err_t control_result = vpx_codec_control_(
                    &multi_encoder_ctx, fuzz_int32_t_14);
                (void)control_result;
                
                // Destroy multi encoder
                vpx_codec_err_t multi_destroy_result = 
                    vpx_codec_destroy(&multi_encoder_ctx);
                (void)multi_destroy_result;
            }
            
            // Get error detail
            const char *error_detail = vpx_codec_error_detail(&encoder_ctx);
            (void)error_detail;
            
            // Destroy encoder
            vpx_codec_err_t enc_destroy_result = vpx_codec_destroy(&encoder_ctx);
            (void)enc_destroy_result;
        }
    }
    
    // Clean up
    vpx_codec_err_t dec_destroy_result = vpx_codec_destroy(&decoder_ctx);
    (void)dec_destroy_result;
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}