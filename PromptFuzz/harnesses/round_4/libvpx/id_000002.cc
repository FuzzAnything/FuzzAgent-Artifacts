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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["vpx_codec_version_str","vpx_codec_build_config","vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_get_frame","vpx_img_flip","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_preview_frame","vpx_img_alloc","vpx_codec_encode","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy","vpx_codec_err_to_string","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_str","vpx_codec_build_config","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_img_flip","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_preview_frame","vpx_img_alloc","vpx_codec_encode","vpx_codec_error_detail","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Use informational APIs
    const char *version_str = vpx_codec_version_str();
    const char *build_config = vpx_codec_build_config();
    (void)version_str; // Avoid unused variable warnings
    (void)build_config;
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    
    vpx_codec_iface_t *decoder_iface = nullptr;
    
    // Try VP9 first, then VP8 based on input hint
    if (data[0] & 0x01) {
        decoder_iface = vpx_codec_vp9_dx();
    } else {
        decoder_iface = vpx_codec_vp8_dx();
    }
    
    vpx_codec_err_t dec_init_result = vpx_codec_dec_init_ver(
        &decoder, 
        decoder_iface, 
        &dec_cfg, 
        fuzz_int64_t_1, 
        VPX_DECODER_ABI_VERSION
    );
    
    if (dec_init_result != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(dec_init_result);
        (void)error_str;
        return 0;
    }
    
    // Decode the input data
    vpx_codec_err_t decode_result = vpx_codec_decode(
        &decoder, 
        data, 
        (unsigned int)size, 
        nullptr, 
        fuzz_int64_t_2
    );
    
    if (decode_result != VPX_CODEC_OK) {
        const char *error_str = vpx_codec_err_to_string(decode_result);
        const char *error_detail = vpx_codec_error_detail(&decoder);
        (void)error_str;
        (void)error_detail;
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Get decoded frames
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Flip the image if it exists
        vpx_img_flip(img);
    }
    
    // Initialize encoder for re-encoding
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    
    vpx_codec_iface_t *encoder_iface = nullptr;
    if (data[0] & 0x01) {
        encoder_iface = vpx_codec_vp9_cx();
    } else {
        encoder_iface = vpx_codec_vp8_cx();
    }
    
    // Get default encoder configuration
    vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_3);
    
    // Set basic configuration
    enc_cfg.g_w = 640;
    enc_cfg.g_h = 480;
    enc_cfg.rc_target_bitrate = 1000;
    
    vpx_codec_err_t enc_init_result = vpx_codec_enc_init_ver(
        &encoder,
        encoder_iface,
        &enc_cfg,
        fuzz_int64_t_4,
        VPX_ENCODER_ABI_VERSION
    );
    
    if (enc_init_result == VPX_CODEC_OK) {
        // Try to get preview frame (may not exist for decoder)
        const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&encoder);
        (void)preview_frame; // May be nullptr
        
        // Create a dummy image for encoding test
        vpx_image_t dummy_img;
        vpx_img_alloc(&dummy_img, VPX_IMG_FMT_I420, 640, 480, fuzz_uint32_t_5);
        
        // Fill with some data (simplified)
        if (dummy_img.planes[0]) {
            size_t fill_size = std::min(size, (size_t)(dummy_img.stride[0] * 480));
            if (fill_size > 0) {
                memcpy(dummy_img.planes[0], data, fill_size);
            }
        }
        
        // Try to encode one frame
        vpx_codec_encode(&encoder, &dummy_img, fuzz_int64_t_6, fuzz_uint64_t_7, fuzz_int64_t_8, fuzz_uint64_t_9);
        
        // Get encoder error detail if any
        const char *enc_error_detail = vpx_codec_error_detail(&encoder);
        (void)enc_error_detail;
        
        vpx_img_free(&dummy_img);
        vpx_codec_destroy(&encoder);
    } else {
        const char *error_str = vpx_codec_err_to_string(enc_init_result);
        (void)error_str;
    }
    
    // Clean up decoder
    vpx_codec_destroy(&decoder);
    
    return 0;
}