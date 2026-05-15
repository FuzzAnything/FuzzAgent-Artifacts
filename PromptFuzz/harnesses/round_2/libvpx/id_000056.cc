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
//<ID> 246
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_error_detail","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_error_detail","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_error_detail","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to decode and re-encode video data:

*/


// libvpx headers

// Callback for put_frame
static void  id_000056_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Simple callback that does nothing - required for API usage
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	 // Too small to be valid VP8/VP9 data
    
    // Create file pointer for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    int in_fd = fuzz_fileno(in_file);
    (void)in_fd; // File descriptor might be used in real scenarios
    
    // Initialize variables
    vpx_codec_ctx_t decoder;
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_dec_cfg_t dec_cfg;
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *img = nullptr;
    vpx_rational_t dsf = {1, 1}; // Default scaling factor
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    
    // Get interface names for debugging
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    
    const char *iface_name = vpx_codec_iface_name(decoder_iface);
    (void)iface_name; // Use the interface name
    
    // Initialize decoder configuration
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    
    // Initialize decoder
    vpx_codec_err_t dec_result = vpx_codec_dec_init_ver(
        &decoder, 
        decoder_iface, 
        &dec_cfg, 
        0, 
        VPX_DECODER_ABI_VERSION
    );
    
    if (dec_result != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&decoder);
        (void)error_detail; // Get error details
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Decode the input data
    vpx_codec_err_t decode_result = vpx_codec_decode(
        &decoder, 
        data, 
        static_cast<unsigned int>(size), 
        nullptr, 
        0
    );
    
    if (decode_result != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&decoder);
        (void)error_detail;
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Get decoded frames
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Initialize encoder configuration
        vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        
        // Set encoder configuration
        enc_cfg.g_w = img->d_w;
        enc_cfg.g_h = img->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_profile = 0;
        enc_cfg.g_threads = 1;
        enc_cfg.g_lag_in_frames = 0;
        enc_cfg.rc_end_usage = VPX_VBR;
        
        // Initialize encoder with multi-encoder API (using single encoder)
        vpx_codec_err_t enc_result = vpx_codec_enc_init_multi_ver(
            &encoder,
            encoder_iface,
            &enc_cfg,
            1,  // Single encoder
            0,  // No flags
            &dsf,  // Default scaling
            VPX_ENCODER_ABI_VERSION
        );
        
        if (enc_result != VPX_CODEC_OK) {
            const char *error_detail = vpx_codec_error_detail(&encoder);
            (void)error_detail;
            continue;
        }
        
        // Register put frame callback
        vpx_codec_register_put_frame_cb(&encoder,  id_000056_put_frame_callback, nullptr);
        
        // Encode the image
        vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
        
        // Get preview frame
        const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&encoder);
        (void)preview_frame; // Preview frame might be nullptr
        
        // Set encoder configuration again (simulating reconfiguration)
        vpx_codec_enc_config_set(&encoder, &enc_cfg);
        
        // Clean up encoder
        vpx_codec_destroy(&encoder);
    }
    
    // Clean up decoder
    vpx_codec_destroy(&decoder);
    
    // Clean up resources
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}