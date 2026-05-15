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
//<ID> 89
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_control_","vpx_codec_error","vpx_codec_destroy","vpx_codec_decode","vpx_codec_error","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_encode","vpx_codec_error","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_control_","vpx_codec_error","vpx_codec_decode","vpx_codec_error","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_encode","vpx_codec_error","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure minimum data for basic decoding
    if (size < 10) {
        return 0;
    }

    vpx_codec_ctx_t decoder;
    vpx_codec_ctx_t encoder;
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *decoded_img = NULL;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_err_t res;
    FILE *in_file = NULL;
    FILE *out_file = NULL;

    // Read input data from memory using fmemopen
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize VP9 decoder
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    res = vpx_codec_dec_init(&decoder, decoder_iface, NULL, 0);
    if (res != VPX_CODEC_OK) {
        const char *error_msg = vpx_codec_error(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Use vpx_codec_control_ to set decoder properties
    // Set post-processing level
    int ctrl_id = VP8_SET_POSTPROC;
    int postproc_level = 3;
    res = vpx_codec_control_(&decoder, ctrl_id, postproc_level);
    if (res != VPX_CODEC_OK) {
        const char *error_msg = vpx_codec_error(&decoder);
    }

    // Decode the input data
    uint8_t *enc_data = (uint8_t *)malloc(size);
    if (!enc_data) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    size_t bytes_read = fread(enc_data, 1, size, in_file);
    res = vpx_codec_decode(&decoder, enc_data, bytes_read, NULL, 0);
    if (res != VPX_CODEC_OK) {
        const char *error_msg = vpx_codec_error(&decoder);
    }

    // Try to get decoded frames
    while ((decoded_img = vpx_codec_get_frame(&decoder, &iter)) != NULL) {
        // Use vpx_codec_get_preview_frame (though it's typically for encoder)
        // We'll call it but it may return NULL for decoder
        const vpx_image_t *preview = vpx_codec_get_preview_frame(&decoder);
        
        // Write frame info to output
        fwrite(&decoded_img->w, sizeof(decoded_img->w), 1, out_file);
        fwrite(&decoded_img->h, sizeof(decoded_img->h), 1, out_file);
        fwrite(&decoded_img->fmt, sizeof(decoded_img->fmt), 1, out_file);
        
        // Now let's try to encode the decoded frame
        vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
        
        // Get default encoder config
        res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        if (res == VPX_CODEC_OK) {
            // Set basic encoder parameters
            enc_cfg.g_w = decoded_img->w;
            enc_cfg.g_h = decoded_img->h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.rc_target_bitrate = 1000;
            enc_cfg.g_pass = VPX_RC_ONE_PASS;
            enc_cfg.g_lag_in_frames = 0;
            
            // Initialize encoder
            res = vpx_codec_enc_init(&encoder, encoder_iface, &enc_cfg, 0);
            if (res == VPX_CODEC_OK) {
                // Set encoder control
                int cpu_used = 4;
                res = vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, cpu_used);
                
                // Encode the frame
                vpx_codec_pts_t pts = 0;
                unsigned long duration = 1;
                vpx_enc_frame_flags_t flags = 0;
                vpx_enc_deadline_t deadline = VPX_DL_REALTIME;
                
                res = vpx_codec_encode(&encoder, decoded_img, pts, duration, 
                                      flags, deadline);
                
                if (res != VPX_CODEC_OK) {
                    const char *error_msg = vpx_codec_error(&encoder);
                }
                
                // Get preview frame from encoder (if available)
                const vpx_image_t *encoder_preview = vpx_codec_get_preview_frame(&encoder);
                if (encoder_preview) {
                    // Write preview frame info
                    fwrite(&encoder_preview->w, sizeof(encoder_preview->w), 1, out_file);
                }
                
                // Get encoded data
                vpx_codec_iter_t enc_iter = NULL;
                const vpx_codec_cx_pkt_t *pkt;
                while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                        // Write encoded packet to output
                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    }
                }
                
                vpx_codec_destroy(&encoder);
            }
        }
    }

    // Cleanup
    free(enc_data);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}