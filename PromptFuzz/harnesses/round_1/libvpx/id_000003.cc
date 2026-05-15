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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["vpx_codec_build_config","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_build_config","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for slice output
static void  id_000003_put_slice_callback(void* user_priv,
                              const vpx_image_t* img,
                              const vpx_image_rect_t* slice_rect,
                              const vpx_image_rect_t* full_rect) {
    // We don't need to do anything in this fuzzing callback
    (void)user_priv;
    (void)img;
    (void)slice_rect;
    (void)full_rect;
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // 1. First, use vpx_codec_build_config() - just call it
    const char* build_config = vpx_codec_build_config();
    (void)build_config; // Mark as used to avoid warning

    // 2. Create a FILE* to read input data (as specified in instruction 4)
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Try to determine if this is VP8 or VP9 data by peeking at stream info
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);

    vpx_codec_err_t peek_result = vpx_codec_err_t(0);
    
    // Try VP9 first
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    peek_result = vpx_codec_peek_stream_info(decoder_iface, data, (unsigned int)size, &stream_info);
    
    // If VP9 fails, try VP8
    if (peek_result != VPX_CODEC_OK) {
        decoder_iface = vpx_codec_vp8_dx();
        memset(&stream_info, 0, sizeof(stream_info));
        stream_info.sz = sizeof(stream_info);
        peek_result = vpx_codec_peek_stream_info(decoder_iface, data, (unsigned int)size, &stream_info);
    }

    // 4. Set up encoder configuration using vpx_codec_enc_config_default
    vpx_codec_enc_cfg_t encoder_cfg;
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    
    // Try to get default encoder config
    vpx_codec_err_t cfg_result = vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, 0);
    
    // If VP9 fails, try VP8
    if (cfg_result != VPX_CODEC_OK) {
        encoder_iface = vpx_codec_vp8_cx();
        cfg_result = vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, 0);
    }

    // 5. Initialize encoder with vpx_codec_enc_init_ver
    vpx_codec_ctx_t encoder_ctx;
    if (cfg_result == VPX_CODEC_OK) {
        // Use stream info if available, otherwise use reasonable defaults
        if (stream_info.w > 0 && stream_info.h > 0) {
            encoder_cfg.g_w = stream_info.w;
            encoder_cfg.g_h = stream_info.h;
        } else {
            encoder_cfg.g_w = 320;
            encoder_cfg.g_h = 240;
        }
        encoder_cfg.rc_target_bitrate = 1000;
        encoder_cfg.g_timebase.num = 1;
        encoder_cfg.g_timebase.den = 30;
        
        vpx_codec_err_t init_result = vpx_codec_enc_init_ver(&encoder_ctx, 
                                                             encoder_iface, 
                                                             &encoder_cfg, 
                                                             0, 
                                                             VPX_ENCODER_ABI_VERSION);
        (void)init_result; // We don't care about the result for fuzzing
    }

    // 6. Set up decoder context
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_err_t dec_init_result = vpx_codec_dec_init_ver(&decoder_ctx, 
                                                            decoder_iface, 
                                                            NULL, 
                                                            0, 
                                                            VPX_DECODER_ABI_VERSION);
    
    // 7. Register put_slice callback using vpx_codec_register_put_slice_cb
    if (dec_init_result == VPX_CODEC_OK) {
        vpx_codec_err_t cb_result = vpx_codec_register_put_slice_cb(&decoder_ctx, 
                                                                    id_000003_put_slice_callback, 
                                                                   NULL);
        (void)cb_result; // Don't care about result for fuzzing
    }

    // 8. Try to decode the input data if decoder initialized successfully
    if (dec_init_result == VPX_CODEC_OK) {
        vpx_codec_decode(&decoder_ctx, data, (unsigned int)size, NULL, 0);
        
        // Get the decoded frame (if any)
        vpx_codec_iter_t iter = NULL;
        vpx_image_t* img;
        while ((img = vpx_codec_get_frame(&decoder_ctx, &iter)) != NULL) {
            // We got a frame - do nothing with it for fuzzing
            (void)img;
        }
    }

    // 9. Clean up resources
    if (cfg_result == VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder_ctx);
    }
    if (dec_init_result == VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder_ctx);
    }
    
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}