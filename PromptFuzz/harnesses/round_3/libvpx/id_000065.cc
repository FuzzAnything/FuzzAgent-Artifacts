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
//<ID> 286
//<Prompt> []
/*<Combination>: [
*/
//<score> 175, nr_unique_branch: 27
//<Quality> {"density":25,"unique_branches":{"high_build_mc_border":[[521,9,521,13,0,0,4,1]],"highbd_filter_selectively_horiz":[[560,13,560,34,0,0,4,1],[613,17,613,33,0,0,4,0],[616,24,616,40,0,0,4,1]],"decode_block":[[1002,26,1002,51,0,0,4,1],[1011,31,1011,56,0,0,4,1]],"dec_build_inter_predictors":[[662,54,662,73,0,0,4,0]],"is_mv_valid":[[391,10,391,26,0,0,4,1]],"dec_find_mv_refs":[[493,7,493,29,0,0,4,0],[545,7,545,21,0,0,4,0],[546,9,546,50,0,0,4,0],[546,9,546,50,0,0,4,1],[548,16,548,57,0,0,4,1],[571,7,571,21,0,0,4,0],[572,9,572,50,0,0,4,0],[573,9,573,51,0,0,4,0],[575,11,576,35,0,0,4,1],[455,11,455,49,4,0,4,1],[454,9,454,20,8,0,4,0],[454,9,454,20,8,0,4,1],[455,11,455,49,8,0,4,0],[462,11,462,22,8,0,4,0],[462,11,462,22,8,0,4,1],[454,9,454,20,13,0,4,0],[454,9,454,20,13,0,4,1],[455,11,455,49,13,0,4,0],[462,11,462,22,13,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_peek_stream_info","vpx_codec_err_to_string","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_register_put_frame_cb","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_get_stream_info","vpx_codec_err_to_string","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_vp9_cx","vpx_img_free","vpx_codec_destroy","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_err_to_string","vpx_codec_enc_init_ver","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_enc_config_default","vpx_codec_err_to_string","vpx_codec_enc_init_multi_ver","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_enc_config_set","vpx_codec_err_to_string","vpx_codec_control_","vpx_codec_err_to_string","vpx_img_set_rect","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_encode","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_peek_stream_info","vpx_codec_err_to_string","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_register_put_frame_cb","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_get_stream_info","vpx_codec_err_to_string","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_err_to_string","vpx_codec_enc_init_ver","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_enc_config_default","vpx_codec_err_to_string","vpx_codec_enc_init_multi_ver","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_enc_config_set","vpx_codec_err_to_string","vpx_codec_control_","vpx_codec_err_to_string","vpx_img_set_rect","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_encode","vpx_codec_err_to_string","vpx_codec_encode","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000065_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // Do nothing - just a dummy callback
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t* f_data, size_t f_size) {
	if(f_size<180) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
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
		FDPConsumeIntegral(int64_t, fuzz_int64_t_22, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_23, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_24, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_25, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create a FILE* from the input data
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
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Mark as used
    (void)out_fd; // Mark as used
    
    vpx_codec_err_t res = VPX_CODEC_OK;
    
    // DECODER PART
    // Get VP9 decoder interface
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    if (!decoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Get interface name
    const char* iface_name = vpx_codec_iface_name(decoder_iface);
    (void)iface_name; // Mark as used
    
    // Peek stream info
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    res = vpx_codec_peek_stream_info(decoder_iface, data, (unsigned int)size, &stream_info);
    if (res != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(res);
        (void)err_str; // Mark as used
    }
    
    // Initialize decoder
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    
    res = vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(res);
        const char* err_detail = vpx_codec_error_detail(&decoder_ctx);
        (void)err_str;
        (void)err_detail;
    }
    
    // Register put frame callback
    res = vpx_codec_register_put_frame_cb(&decoder_ctx,  id_000065_put_frame_callback, nullptr);
    if (res != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(res);
        (void)err_str;
    }
    
    // Decode the input data
    res = vpx_codec_decode(&decoder_ctx, data, (unsigned int)size, nullptr, fuzz_int64_t_2);
    if (res != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(res);
        (void)err_str;
    }
    
    // Get stream info from decoder context
    res = vpx_codec_get_stream_info(&decoder_ctx, &stream_info);
    if (res != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(res);
        (void)err_str;
    }
    
    // Get decoded frames
    vpx_codec_iter_t iter = nullptr;
    std::vector<vpx_image_t*> decoded_frames;
    
    while (const vpx_image_t* frame = vpx_codec_get_frame(&decoder_ctx, &iter)) {
        // Allocate a new image and copy the frame data
        vpx_image_t* img_copy = vpx_img_alloc(nullptr, frame->fmt, 
                                             frame->d_w, frame->d_h, fuzz_uint32_t_3);
        if (img_copy) {
            // Copy image data (simplified - in reality would need to copy planes)
            img_copy->w = frame->w;
            img_copy->h = frame->h;
            img_copy->bit_depth = frame->bit_depth;
            decoded_frames.push_back(img_copy);
        }
    }
    
    // ENCODER PART
    // Get VP9 encoder interface
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    if (!encoder_iface) {
        for (auto* img : decoded_frames) vpx_img_free(img);
        vpx_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Get encoder interface name
    iface_name = vpx_codec_iface_name(encoder_iface);
    (void)iface_name;
    
    // Single encoder configuration
    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_enc_cfg_t enc_cfg;
    
    res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_4);
    if (res != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(res);
        (void)err_str;
    }
    
    // Set encoder configuration based on decoded stream info
    if (stream_info.w > 0 && stream_info.h > 0) {
        enc_cfg.g_w = stream_info.w;
        enc_cfg.g_h = stream_info.h;
    } else {
        enc_cfg.g_w = 320;
        enc_cfg.g_h = 240;
    }
    enc_cfg.rc_target_bitrate = 1000;
    
    // Initialize single encoder
    res = vpx_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, fuzz_int64_t_5, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(res);
        const char* err_detail = vpx_codec_error_detail(&encoder_ctx);
        (void)err_str;
        (void)err_detail;
    }
    
    // Multi-encoder configuration
    vpx_codec_ctx_t multi_encoder_ctx;
    vpx_codec_enc_cfg_t multi_enc_cfg;
    
    res = vpx_codec_enc_config_default(encoder_iface, &multi_enc_cfg, fuzz_uint32_t_6);
    if (res != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(res);
        (void)err_str;
    }
    
    multi_enc_cfg.g_w = enc_cfg.g_w;
    multi_enc_cfg.g_h = enc_cfg.g_h;
    multi_enc_cfg.rc_target_bitrate = 1000;
    
    // Initialize multi-encoder
    vpx_rational_t dsf = {1, 1};
    res = vpx_codec_enc_init_multi_ver(&multi_encoder_ctx, encoder_iface, &multi_enc_cfg, 
                                       1, fuzz_int64_t_7, &dsf, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(res);
        const char* err_detail = vpx_codec_error_detail(&multi_encoder_ctx);
        (void)err_str;
        (void)err_detail;
    }
    
    // Set encoder configuration
    res = vpx_codec_enc_config_set(&multi_encoder_ctx, &multi_enc_cfg);
    if (res != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(res);
        (void)err_str;
    }
    
    // Use control API
    res = vpx_codec_control_(&encoder_ctx, VP8E_SET_CPUUSED, 8);
    if (res != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(res);
        (void)err_str;
    }
    
    // Process decoded frames
    for (size_t i = 0; i < decoded_frames.size(); i++) {
        vpx_image_t* img = decoded_frames[i];
        
        if (img) {
            // Set rectangle on image
            unsigned int rect_w = img->d_w > 10 ? img->d_w - 10 : img->d_w;
            unsigned int rect_h = img->d_h > 10 ? img->d_h - 10 : img->d_h;
            int set_rect_res = vpx_img_set_rect(img, fuzz_uint32_t_8, fuzz_uint32_t_9, rect_w, rect_h);
            (void)set_rect_res; // Mark as used
            
            // Encode with single encoder
            res = vpx_codec_encode(&encoder_ctx, img, fuzz_int64_t_10, fuzz_uint64_t_11, fuzz_int64_t_12, fuzz_uint64_t_13);
            if (res != VPX_CODEC_OK) {
                const char* err_str = vpx_codec_err_to_string(res);
                (void)err_str;
            }
            
            // Encode with multi-encoder
            res = vpx_codec_encode(&multi_encoder_ctx, img, fuzz_int64_t_14, fuzz_uint64_t_15, fuzz_int64_t_16, fuzz_uint64_t_17);
            if (res != VPX_CODEC_OK) {
                const char* err_str = vpx_codec_err_to_string(res);
                (void)err_str;
            }
        }
    }
    
    // Flush encoders
    res = vpx_codec_encode(&encoder_ctx, nullptr, fuzz_int64_t_18, fuzz_uint64_t_19, fuzz_int64_t_20, fuzz_uint64_t_21);
    res = vpx_codec_encode(&multi_encoder_ctx, nullptr, fuzz_int64_t_22, fuzz_uint64_t_23, fuzz_int64_t_24, fuzz_uint64_t_25);
    
    // Clean up
    for (auto* img : decoded_frames) {
        vpx_img_free(img);
    }
    
    // Destroy codecs
    vpx_codec_destroy(&decoder_ctx);
    vpx_codec_destroy(&encoder_ctx);
    vpx_codec_destroy(&multi_encoder_ctx);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}