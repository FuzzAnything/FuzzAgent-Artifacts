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
//<ID> 87
//<Prompt> []
/*<Combination>: [
*/
//<score> 7.5, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_decode","vpx_codec_error","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_error","vpx_codec_encode","vpx_codec_error","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_control_","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_error","vpx_codec_encode","vpx_codec_error","vpx_codec_get_cx_data","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_control_","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/*Here's a fuzzer that uses the specified libvpx APIs to decode and re-encode video data:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create input file from memory buffer
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
    
    vpx_codec_ctx_t decoder = {0};
    vpx_codec_ctx_t encoder = {0};
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *input_image = nullptr;
    
    // Try to decode the input data
    vpx_codec_iface_t *decoder_interface = vpx_codec_vp9_dx();
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    // Initialize decoder
    vpx_codec_err_t ret = vpx_codec_dec_init(&decoder, decoder_interface, &dec_cfg, 0);
    if (ret != VPX_CODEC_OK) {
        // Use vpx_codec_error to get error description
        const char *error_msg = vpx_codec_error(&decoder);
        (void)error_msg; // Prevent unused variable warning
        goto cleanup;
    }
    
    // Decode the input data
    ret = vpx_codec_decode(&decoder, data, size, nullptr, 0);
    if (ret != VPX_CODEC_OK) {
        const char *error_msg = vpx_codec_error(&decoder);
        (void)error_msg;
        goto cleanup;
    }
    
    // Try to get decoded frames
    while ((input_image = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Set some encoder control parameters using vpx_codec_control_
        // Try to set a simple control (e.g., VP8E_SET_CPUUSED)
        if (encoder.iface) {
            // This is just an example - actual control IDs depend on encoder type
            // vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 4);
        }
        
        // Try to get preview frame if available
        const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&encoder);
        if (preview_frame) {
            // Preview frame available - could process it here
            (void)preview_frame;
        }
        
        // Initialize encoder if not already initialized
        if (!encoder.iface) {
            vpx_codec_iface_t *encoder_interface = vpx_codec_vp9_cx();
            vpx_codec_enc_cfg_t enc_cfg = {0};
            
            // Set basic encoder configuration
            enc_cfg.g_w = input_image->d_w;
            enc_cfg.g_h = input_image->d_h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.rc_target_bitrate = 1000;
            enc_cfg.g_pass = VPX_RC_ONE_PASS;
            enc_cfg.g_lag_in_frames = 0;
            enc_cfg.kf_mode = VPX_KF_AUTO;
            
            ret = vpx_codec_enc_init(&encoder, encoder_interface, &enc_cfg, 0);
            if (ret != VPX_CODEC_OK) {
                const char *error_msg = vpx_codec_error(&encoder);
                (void)error_msg;
                break;
            }
        }
        
        // Encode the frame using vpx_codec_encode
        ret = vpx_codec_encode(&encoder, input_image, 0, 1, 0, VPX_DL_REALTIME);
        if (ret != VPX_CODEC_OK) {
            const char *error_msg = vpx_codec_error(&encoder);
            (void)error_msg;
            continue;
        }
        
        // Try to get encoded data
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Write encoded data to output file
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
        iter = NULL; // Reset iterator for next iteration
    }
    
    // Flush encoder
    if (encoder.iface) {
        ret = vpx_codec_encode(&encoder, nullptr, 0, 1, 0, VPX_DL_REALTIME);
        if (ret == VPX_CODEC_OK) {
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
        }
    }
    
    // Set another control parameter just to use vpx_codec_control_ API
    if (encoder.iface) {
        // Try to set a control that should exist in most encoders
        vpx_codec_control_(&encoder, VP8E_SET_ENABLEAUTOALTREF, 0);
    }

cleanup:
    // Clean up resources
    if (encoder.iface) {
        vpx_codec_destroy(&encoder);
    }
    if (decoder.iface) {
        vpx_codec_destroy(&decoder);
    }
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    if (out_file) {
        assert_file_closed(&out_file);;
        // Remove output file
        remove("output_file");
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}