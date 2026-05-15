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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 38.5, nr_unique_branch: 76
//<Quality> {"density":11,"unique_branches":{"vp9_update_buffer_level_preencode":[[24,23,24,34,1,0,4,1]],"block_yrd":[[749,7,749,35,0,0,4,0],[749,39,749,74,0,0,4,1],[804,11,804,22,0,0,4,0]],"vp9_rc_regulate_q":[[863,9,863,52,0,0,4,1],[873,12,873,39,0,0,4,0],[873,12,873,39,0,0,4,1]],"vp9_initialize_rd_consts":[[428,9,428,61,0,0,4,1],[429,9,429,36,0,0,4,0]],"init_motion_estimation":[[3826,7,3826,40,0,0,4,1],[3828,14,3828,49,0,0,4,1]],"set_rt_speed_feature_framesize_dependent":[[446,7,446,17,0,0,4,0],[448,9,448,47,0,0,4,1],[24,23,24,34,1,0,4,0],[24,23,24,34,2,0,4,0],[24,23,24,34,3,0,4,0],[24,23,24,34,4,0,4,0],[24,23,24,34,4,0,4,1]],"rc_pick_q_and_bounds_one_pass_vbr":[[1330,9,1330,23,0,0,4,0],[1332,11,1332,59,0,0,4,1]],"vp9_rc_update_rate_correction_factors":[[779,7,779,54,0,0,4,1]],"encoder_init":[[1200,9,1200,28,0,0,4,1]],"vp9_set_variance_partition_thresholds":[[642,7,642,55,0,0,4,1]],"vp9_pick_intra_mode":[[24,23,24,34,2,0,4,0]],"encode_without_recode_loop":[[4184,7,4184,34,0,0,4,0]],"encode_nonrd_sb_row":[[5304,7,5304,31,0,0,4,0]],"vp9_set_gf_update_one_pass_vbr":[[2090,9,2090,36,0,0,4,1]],"choose_partitioning":[[1552,18,1552,46,0,0,4,1]],"alloc_copy_partition_data":[[2029,7,2029,34,0,0,4,0],[2034,7,2034,35,0,0,4,0],[2040,7,2040,37,0,0,4,0],[2046,7,2046,36,0,0,4,0],[440,9,440,16,1,0,4,1],[440,9,440,16,2,0,4,1],[440,9,440,16,3,0,4,1],[440,9,440,16,4,0,4,1]],"vp9_estimate_bits_at_q":[[25,23,25,34,1,0,4,0]],"estimate_block_intra":[[24,23,24,34,1,0,4,0]],"vp9_setup_mask":[[966,19,966,47,0,0,4,0],[966,51,966,79,0,0,4,0],[996,25,996,52,0,0,4,0],[997,25,997,52,0,0,4,0]],"set_rt_speed_feature_framesize_independent":[[662,7,662,17,0,0,4,0],[663,9,663,37,0,0,4,0],[663,41,663,68,0,0,4,1],[672,9,672,27,0,0,4,0],[675,11,675,48,0,0,4,0],[676,11,676,43,0,0,4,0],[677,12,677,25,0,0,4,0],[685,9,685,37,0,0,4,1],[689,9,689,35,0,0,4,1],[694,9,694,36,0,0,4,1],[699,7,699,17,0,0,4,0],[706,9,706,40,0,0,4,1],[712,9,712,35,0,0,4,1],[712,39,712,64,0,0,4,1],[717,9,717,21,0,0,4,1],[722,9,722,33,0,0,4,0],[722,37,722,62,0,0,4,0],[723,9,723,30,0,0,4,0],[724,10,724,23,0,0,4,0],[731,11,731,42,0,0,4,1],[738,9,738,21,0,0,4,1],[745,9,745,21,0,0,4,1],[748,9,748,43,0,0,4,1],[751,7,751,17,0,0,4,0],[754,9,754,43,0,0,4,1],[758,9,758,22,0,0,4,0],[759,9,759,20,0,0,4,1],[762,9,762,33,0,0,4,1],[766,9,766,52,0,0,4,1],[769,9,769,39,0,0,4,1],[771,9,771,22,0,0,4,0],[771,26,771,54,0,0,4,1],[792,9,792,43,0,0,4,1],[440,9,440,16,24,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_enc_config_set","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_register_put_frame_cb","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_enc_config_set","vpx_img_alloc","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_destroy"],"visited":5}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000011_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // This callback is called when a frame is decoded
    // We can process the frame here if needed
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<72) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_dec_cfg_t dec_cfg = {0, 0, 0};
    
    // Initialize decoder
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    
    // Use error APIs
    if (err != VPX_CODEC_OK) {
        const char* error_str = vpx_codec_err_to_string(err);
        const char* error_detail = vpx_codec_error_detail(&decoder);
        (void)error_str;
        (void)error_detail;
    }
    
    // Register put frame callback
    err = vpx_codec_register_put_frame_cb(&decoder,  id_000011_put_frame_callback, nullptr);
    
    // Peek stream info
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    err = vpx_codec_peek_stream_info(decoder_iface, data, (unsigned int)size, &si);
    
    // Decode the input data
    err = vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, fuzz_int64_t_2);
    
    // Get stream info after decoding
    if (err == VPX_CODEC_OK) {
        err = vpx_codec_get_stream_info(&decoder, &si);
    }
    
    // Create an encoder context for re-encoding
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_enc_cfg_t cfg;
    
    if (vpx_codec_enc_config_default(encoder_iface, &cfg, fuzz_uint32_t_3) == VPX_CODEC_OK) {
        // Configure encoder settings based on decoded stream info
        cfg.g_w = si.w;
        cfg.g_h = si.h;
        cfg.rc_target_bitrate = 1000;
        cfg.g_pass = VPX_RC_ONE_PASS;
        cfg.g_lag_in_frames = 0;
        
        // Initialize encoder
        if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, fuzz_int64_t_4, VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
            
            // Use control API (example: set CPU used)
            vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 8);
            
            // Set encoder configuration
            vpx_codec_enc_config_set(&encoder, &cfg);
            
            // Create an image for encoding
            vpx_image_t* img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                            cfg.g_w, cfg.g_h, fuzz_uint32_t_5);
            
            if (img) {
                // Set a rectangle on the image (use safe values)
                unsigned int rect_x = 0;
                unsigned int rect_y = 0;
                unsigned int rect_w = cfg.g_w > 10 ? cfg.g_w - 10 : cfg.g_w;
                unsigned int rect_h = cfg.g_h > 10 ? cfg.g_h - 10 : cfg.g_h;
                
                vpx_img_set_rect(img, fuzz_uint32_t_6, fuzz_uint32_t_7, rect_w, rect_h);
                
                // Encode the image
                vpx_codec_encode(&encoder, img, fuzz_int64_t_8, fuzz_uint64_t_9, fuzz_int64_t_10, fuzz_uint64_t_11);
                
                // Get encoded data
                vpx_codec_iter_t iter = nullptr;
                const vpx_codec_cx_pkt_t* pkt;
                
                while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
                
                vpx_img_free(img);
            }
            
            vpx_codec_destroy(&encoder);
        }
    }
    
    // Get frames from decoder
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t* decoded_img;
    while ((decoded_img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Process decoded frame if needed
    }
    
    // Cleanup
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}