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
//<ID> 27
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"encoder_init":[[1198,9,1198,28,0,0,4,1]]},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_vp9_cx","vpx_codec_peek_stream_info","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_get_stream_info","vpx_codec_error","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_peek_stream_info","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_get_stream_info","vpx_codec_error","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000009_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // This is just a dummy callback for the fuzzer
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Try both VP8 and VP9 codec interfaces
    vpx_codec_iface_t *decoder_ifaces[] = {
        vpx_codec_vp8_dx(),
        vpx_codec_vp9_dx()
    };
    
    vpx_codec_iface_t *encoder_ifaces[] = {
        vpx_codec_vp8_cx(),
        vpx_codec_vp9_cx()
    };
    
    const int num_codecs = 2;
    
    for (int i = 0; i < num_codecs; i++) {
        vpx_codec_iface_t *dec_iface = decoder_ifaces[i];
        vpx_codec_iface_t *enc_iface = encoder_ifaces[i];
        
        // 1. Use vpx_codec_peek_stream_info to get stream info without full decode
        vpx_codec_stream_info_t stream_info;
        memset(&stream_info, 0, sizeof(stream_info));
        stream_info.sz = sizeof(stream_info);
        
        vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
            dec_iface, data, (unsigned int)size, &stream_info);
        
        // 2. Get codec interface name
        const char *iface_name = vpx_codec_iface_name(dec_iface);
        (void)iface_name; // Use the name to avoid unused variable warning
        
        // Try to initialize decoder if peek was successful
        if (peek_result == VPX_CODEC_OK) {
            vpx_codec_ctx_t decoder;
            vpx_codec_dec_cfg_t dec_cfg;
            memset(&dec_cfg, 0, sizeof(dec_cfg));
            dec_cfg.threads = 1;
            
            // Initialize decoder
            vpx_codec_err_t init_result = vpx_codec_dec_init_ver(
                &decoder, dec_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
            
            if (init_result == VPX_CODEC_OK) {
                // 3. Register put frame callback
                vpx_codec_register_put_frame_cb(
                    &decoder,  id_000009_put_frame_callback, nullptr);
                
                // 4. Get stream info from initialized decoder context
                vpx_codec_stream_info_t stream_info2;
                memset(&stream_info2, 0, sizeof(stream_info2));
                stream_info2.sz = sizeof(stream_info2);
                
                vpx_codec_err_t get_info_result = vpx_codec_get_stream_info(
                    &decoder, &stream_info2);
                (void)get_info_result; // Check result if needed
                
                // 5. Get error string (check if any error occurred)
                const char *error_str = vpx_codec_error(&decoder);
                (void)error_str; // Use error string to avoid unused variable warning
                
                // Try to decode the data
                vpx_codec_decode(&decoder, data, (unsigned int)size, 
                                nullptr, fuzz_int64_t_2);
                
                // Try to get decoded frames
                vpx_codec_iter_t iter = nullptr;
                while (vpx_codec_get_frame(&decoder, &iter) != nullptr) {
                    // Frames are processed in the callback
                }
                
                // Try to set up encoder with configuration based on stream info
                vpx_codec_ctx_t encoder;
                vpx_codec_enc_cfg_t enc_cfg;
                
                // Get default encoder configuration
                if (vpx_codec_enc_config_default(enc_iface, &enc_cfg, fuzz_uint32_t_3) 
                    == VPX_CODEC_OK) {
                    
                    // Set encoder configuration based on stream info
                    enc_cfg.g_w = stream_info.w;
                    enc_cfg.g_h = stream_info.h;
                    enc_cfg.rc_target_bitrate = 1000; // Default bitrate
                    
                    // Initialize encoder
                    if (vpx_codec_enc_init_ver(&encoder, enc_iface, &enc_cfg, 
                                              fuzz_int64_t_4, VPX_ENCODER_ABI_VERSION) 
                        == VPX_CODEC_OK) {
                        
                        // 6. Set encoder configuration
                        vpx_codec_enc_config_set(&encoder, &enc_cfg);
                        
                        // Clean up encoder
                        vpx_codec_destroy(&encoder);
                    }
                }
                
                // Clean up decoder
                vpx_codec_destroy(&decoder);
            }
        }
    }
    
    return 0;
}