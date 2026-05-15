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
//<ID> 88
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_error","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_error","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create input file from fuzzed data
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
    
    vpx_codec_ctx_t codec_ctx;
    vpx_codec_iface_t *decoder = vpx_codec_vp9_dx();
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iter_t iter = NULL;
    
    // Initialize decoder with default config
    vpx_codec_dec_cfg_t dec_cfg;
    dec_cfg.threads = 1;
    dec_cfg.w = 0;
    dec_cfg.h = 0;
    
    if (vpx_codec_dec_init_ver(&codec_ctx, decoder, &dec_cfg, 0, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        // Try to peek stream info first
        vpx_codec_stream_info_t si;
        si.sz = sizeof(si);
        if (vpx_codec_peek_stream_info(decoder, data, static_cast<unsigned int>(size), &si) == VPX_CODEC_OK) {
            // Reinitialize with proper dimensions
            dec_cfg.w = si.w;
            dec_cfg.h = si.h;
            if (vpx_codec_dec_init_ver(&codec_ctx, decoder, &dec_cfg, 0, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
                assert_file_closed(&in_file);;
                assert_file_closed(&out_file);;
                assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
            }
        } else {
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // Decode the input data
    if (vpx_codec_decode(&codec_ctx, data, static_cast<unsigned int>(size), NULL, 0) != VPX_CODEC_OK) {
        // Use vpx_codec_error API
        const char *error_str = vpx_codec_error(&codec_ctx);
        (void)error_str; // Suppress unused warning in fuzzing context
    }
    
    // Get decoded frames
    const vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&codec_ctx, &iter)) != NULL) {
        // Try to get preview frame (may return NULL)
        const vpx_image_t *preview_img = vpx_codec_get_preview_frame(&codec_ctx);
        (void)preview_img; // Use preview frame if available
        
        // Initialize encoder with default config
        vpx_codec_iface_t *encoder = vpx_codec_vp9_cx();
        vpx_codec_enc_config_default(encoder, &cfg, 0);
        
        cfg.g_w = img->d_w;
        cfg.g_h = img->d_h;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        
        vpx_codec_ctx_t enc_ctx;
        if (vpx_codec_enc_init_ver(&enc_ctx, encoder, &cfg, 0, VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
            // Use vpx_codec_control_ API
            // Try to set some control parameters based on fuzzed data
            int ctrl_id = data[0] % 100; // Use first byte to determine control ID
            vpx_codec_control_(&enc_ctx, ctrl_id);
            
            // Encode the frame
            vpx_codec_pts_t pts = 0;
            unsigned long duration = 1;
            vpx_enc_frame_flags_t flags = 0;
            vpx_enc_deadline_t deadline = 0;
            
            if (vpx_codec_encode(&enc_ctx, img, pts, duration, flags, deadline) == VPX_CODEC_OK) {
                // Get encoded data
                const vpx_codec_cx_pkt_t *pkt;
                vpx_codec_iter_t enc_iter = NULL;
                
                while ((pkt = vpx_codec_get_cx_data(&enc_ctx, &enc_iter)) != NULL) {
                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
            }
            
            // Clean up encoder
            vpx_codec_destroy(&enc_ctx);
        }
    }
    
    // Clean up
    vpx_codec_destroy(&codec_ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}