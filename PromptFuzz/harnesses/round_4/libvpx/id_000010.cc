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
//<ID> 28
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_vp9_cx","vpx_codec_peek_stream_info","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_destroy","vpx_codec_get_stream_info","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_error","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_peek_stream_info","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_destroy","vpx_codec_get_stream_info","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_error","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000010_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Do nothing in fuzzing context
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Try both VP8 and VP9 codec interfaces
    vpx_codec_iface_t* decoder_ifaces[] = {
        vpx_codec_vp8_dx(),
        vpx_codec_vp9_dx()
    };
    
    vpx_codec_iface_t* encoder_ifaces[] = {
        vpx_codec_vp8_cx(),
        vpx_codec_vp9_cx()
    };
    
    const int num_codecs = sizeof(decoder_ifaces) / sizeof(decoder_ifaces[0]);
    
    for (int i = 0; i < num_codecs; i++) {
        vpx_codec_iface_t* decoder_iface = decoder_ifaces[i];
        vpx_codec_iface_t* encoder_iface = encoder_ifaces[i];
        
        // 1. Peek stream info to check if this codec can handle the data
        vpx_codec_stream_info_t stream_info;
        memset(&stream_info, 0, sizeof(stream_info));
        stream_info.sz = sizeof(stream_info);
        
        vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
            decoder_iface, data, size, &stream_info);
        
        if (peek_result != VPX_CODEC_OK) {
            // Try next codec
            continue;
        }
        
        // 2. Get codec interface name
        const char* iface_name = vpx_codec_iface_name(decoder_iface);
        (void)iface_name; // Use in debug if needed
        
        // 3. Initialize decoder context
        vpx_codec_ctx_t decoder;
        vpx_codec_dec_cfg_t dec_cfg;
        memset(&dec_cfg, 0, sizeof(dec_cfg));
        dec_cfg.threads = 1;
        
        vpx_codec_err_t init_result = vpx_codec_dec_init_ver(
            &decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
        
        if (init_result != VPX_CODEC_OK) {
            // Cleanup and try next codec
            vpx_codec_destroy(&decoder);
            continue;
        }
        
        // 4. Get stream info from decoder context
        vpx_codec_stream_info_t decoder_stream_info;
        memset(&decoder_stream_info, 0, sizeof(decoder_stream_info));
        decoder_stream_info.sz = sizeof(decoder_stream_info);
        
        vpx_codec_err_t get_info_result = vpx_codec_get_stream_info(
            &decoder, &decoder_stream_info);
        
        // 5. Set up encoder configuration (simulate re-encoding scenario)
        vpx_codec_ctx_t encoder;
        vpx_codec_enc_cfg_t enc_cfg;
        
        // Initialize encoder with default config
        if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_2) == VPX_CODEC_OK) {
            // Set encoder parameters based on stream info
            enc_cfg.g_w = stream_info.w;
            enc_cfg.g_h = stream_info.h;
            enc_cfg.g_threads = 1;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.rc_target_bitrate = 1000;
            enc_cfg.g_error_resilient = 0;
            
            // Initialize encoder
            if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_3, 
                                      VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
                
                // Try to set encoder configuration (may fail but we try)
                vpx_codec_enc_config_set(&encoder, &enc_cfg);
                
                // Clean up encoder
                vpx_codec_destroy(&encoder);
            }
        }
        
        // 6. Register put frame callback
        vpx_codec_register_put_frame_cb(&decoder,  id_000010_put_frame_callback, nullptr);
        
        // 7. Decode the input data
        vpx_codec_decode(&decoder, data, size, nullptr, fuzz_int64_t_4);
        
        // 8. Get error information if any
        const char* error_str = vpx_codec_error(&decoder);
        const char* error_detail = vpx_codec_error_detail(&decoder);
        (void)error_str; // Use in debug if needed
        (void)error_detail; // Use in debug if needed
        
        // 9. Try to get decoded frames
        vpx_codec_iter_t iter = nullptr;
        while (const vpx_image_t* img = vpx_codec_get_frame(&decoder, &iter)) {
            // Frame available, but we don't process it in fuzzing
            (void)img;
        }
        
        // 10. Cleanup decoder
        vpx_codec_destroy(&decoder);
        
        // We found a working codec, break the loop
        break;
    }
    
    return 0;
}