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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_err_to_string","vpx_codec_destroy","vpx_codec_err_to_string","vpx_codec_destroy","vpx_codec_peek_stream_info","vpx_codec_err_to_string","vpx_codec_enc_config_default","vpx_codec_enc_config_default"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_err_to_string","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_peek_stream_info","vpx_codec_err_to_string","vpx_codec_enc_config_default","vpx_codec_enc_config_default"],"visited":0}
/*Looking at the provided APIs and the instructions, I'll create a fuzz driver that decodes encoded VP8/VP9 video data from the input and then attempts to re-encode it.

*/


// Include libvpx headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a file handle from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Variables for decoding
    vpx_codec_ctx_t decoder;
    vpx_codec_ctx_t encoder;
    vpx_codec_iter_t iter = NULL;
    vpx_codec_enc_cfg_t cfg;
    vpx_rational_t dsf = {1, 1};  // Default downsampling factor
    const vpx_image_t *img;
    vpx_codec_err_t res;
    
    // Try both VP8 and VP9 decoders
    vpx_codec_iface_t *decoder_ifaces[] = {
        vpx_codec_vp8_dx(),
        vpx_codec_vp9_dx()
    };
    
    // Get decoder interface names
    const char *decoder_names[] = {
        vpx_codec_iface_name(decoder_ifaces[0]),
        vpx_codec_iface_name(decoder_ifaces[1])
    };
    
    // Try to decode with each decoder interface
    for (int i = 0; i < 2; i++) {
        // Initialize decoder
        res = vpx_codec_dec_init_ver(&decoder, decoder_ifaces[i], NULL, 0, VPX_DECODER_ABI_VERSION);
        if (res != VPX_CODEC_OK) {
            const char *error_str = vpx_codec_err_to_string(res);
            continue;  // Try next decoder
        }
        
        // Decode the input data
        res = vpx_codec_decode(&decoder, data, size, NULL, 0);
        if (res == VPX_CODEC_OK) {
            // Try to get decoded frame
            img = vpx_codec_get_frame(&decoder, &iter);
            if (img) {
                // Now try to initialize an encoder with the same codec type
                // Note: We use the decoder interface name to determine encoder interface
                // In real usage, we'd use vpx_codec_vp8_cx() or vpx_codec_vp9_cx() based on decoder
                
                // For fuzzing purposes, we'll use the decoder interface to get default config
                // This is not typical usage but fits the requirement to use vpx_codec_enc_config_default
                vpx_codec_enc_config_default(decoder_ifaces[i], &cfg, 0);
                
                // Initialize encoder (multi-ver) - using 1 encoder instance
                res = vpx_codec_enc_init_multi_ver(&encoder, decoder_ifaces[i], &cfg, 
                                                   1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
                
                if (res != VPX_CODEC_OK) {
                    const char *error_str = vpx_codec_err_to_string(res);
                }
                
                // Clean up encoder
                vpx_codec_destroy(&encoder);
            }
        } else {
            const char *error_str = vpx_codec_err_to_string(res);
        }
        
        // Clean up decoder
        vpx_codec_destroy(&decoder);
        
        // Reset iterator for next attempt
        iter = NULL;
    }
    
    // Try to peek stream info with VP8 decoder
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    for (int i = 0; i < 2; i++) {
        res = vpx_codec_peek_stream_info(decoder_ifaces[i], data, 
                                         size > 100 ? 100 : (unsigned int)size, &si);
        if (res != VPX_CODEC_OK) {
            const char *error_str = vpx_codec_err_to_string(res);
        }
    }
    
    // Additional attempt: Try to get default encoder config for both codecs
    vpx_codec_enc_cfg_t cfg_vp8, cfg_vp9;
    vpx_codec_enc_config_default(decoder_ifaces[0], &cfg_vp8, 0);
    vpx_codec_enc_config_default(decoder_ifaces[1], &cfg_vp9, 0);
    
    // Clean up
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}