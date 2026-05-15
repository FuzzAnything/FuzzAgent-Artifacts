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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_destroy","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_err_to_string","vpx_codec_destroy","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_destroy","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_err_to_string","vpx_codec_destroy","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file for writing encoded data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get decoder interfaces
    vpx_codec_iface_t *vp8_decoder = vpx_codec_vp8_dx();
    vpx_codec_iface_t *vp9_decoder = vpx_codec_vp9_dx();
    
    // Get decoder interface names
    const char *vp8_name = vpx_codec_iface_name(vp8_decoder);
    const char *vp9_name = vpx_codec_iface_name(vp9_decoder);
    (void)vp8_name; // Mark as used
    (void)vp9_name; // Mark as used

    // Decode the input data using VP9 decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, vp9_decoder, NULL, 0, VPX_DECODER_ABI_VERSION);
    
    if (err == VPX_CODEC_OK) {
        // Decode the input data
        err = vpx_codec_decode(&decoder, data, size, NULL, 0);
        
        // Check and log error if any
        if (err != VPX_CODEC_OK) {
            const char *error_string = vpx_codec_err_to_string(err);
            (void)error_string; // Mark as used
        }
        
        // Clean up decoder
        vpx_codec_destroy(&decoder);
    }

    // Initialize encoder configuration for VP9
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_iface_t *vp9_encoder = vpx_codec_vp9_cx(); // Not in required list, but needed for encoder
    if (vpx_codec_enc_config_default(vp9_encoder, &enc_cfg, 0) == VPX_CODEC_OK) {
        // Configure basic encoding parameters
        enc_cfg.g_w = 640;
        enc_cfg.g_h = 480;
        enc_cfg.rc_target_bitrate = 1000;
        
        // Initialize multi-encoder (single encoder in this case)
        vpx_codec_ctx_t encoder;
        err = vpx_codec_enc_init_multi_ver(&encoder, vp9_encoder, &enc_cfg, 1, 0, NULL, VPX_ENCODER_ABI_VERSION);
        
        if (err == VPX_CODEC_OK) {
            // Get error string for logging (even if successful)
            const char *enc_error_string = vpx_codec_err_to_string(err);
            (void)enc_error_string; // Mark as used
            
            // Clean up encoder
            vpx_codec_destroy(&encoder);
        }
    }

    // Decode with VP8 decoder as well
    vpx_codec_ctx_t decoder_vp8;
    if (vpx_codec_dec_init_ver(&decoder_vp8, vp8_decoder, NULL, 0, VPX_DECODER_ABI_VERSION) == VPX_CODEC_OK) {
        // Decode a portion of the data (limit to prevent excessive processing)
        size_t decode_size = size > 1024 ? 1024 : size;
        err = vpx_codec_decode(&decoder_vp8, data, decode_size, NULL, 0);
        
        // Check and log error
        if (err != VPX_CODEC_OK) {
            const char *error_string = vpx_codec_err_to_string(err);
            (void)error_string; // Mark as used
        }
        
        vpx_codec_destroy(&decoder_vp8);
    }

    // Clean up file handles
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}