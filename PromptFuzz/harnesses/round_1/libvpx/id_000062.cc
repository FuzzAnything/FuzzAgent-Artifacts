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
//<ID> 232
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_register_put_frame_cb","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_error_detail","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_register_put_frame_cb","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_error_detail","vpx_codec_error_detail","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// libvpx headers
extern "C" {
}

// Callback function for decoded frames
static void  id_000062_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // Store frame or process it (in real scenario)
    (void)user_priv;
    (void)img;
    // Just acknowledge frame reception in fuzzing context
}

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE* for reading the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder context
    vpx_codec_ctx_t decoder;
    memset(&decoder, 0, sizeof(decoder));
    
    // Get decoder interface (try VP9 first, then VP8)
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    if (!decoder_iface) {
        decoder_iface = vpx_codec_vp8_dx();
    }
    
    if (!decoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Get codec interface name for logging/debugging
    const char* codec_name = vpx_codec_iface_name(decoder_iface);
    (void)codec_name; // In fuzzing, we might not use this

    // Initialize decoder configuration
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1; // Single thread for fuzzing

    // Initialize decoder with version check
    vpx_codec_err_t dec_init_result = vpx_codec_dec_init_ver(
        &decoder, 
        decoder_iface, 
        &dec_cfg, 
        0, // flags
        VPX_DECODER_ABI_VERSION
    );

    // Convert error code to string for debugging
    const char* err_str = vpx_codec_err_to_string(dec_init_result);
    (void)err_str; // In fuzzing, we might not use this

    if (dec_init_result != VPX_CODEC_OK) {
        // Get detailed error information
        const char* err_detail = vpx_codec_error_detail(&decoder);
        (void)err_detail; // In fuzzing, we might not use this
        
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Register frame callback
    vpx_codec_err_t reg_result = vpx_codec_register_put_frame_cb(
        &decoder, 
         id_000062_put_frame_callback, 
        nullptr // user_priv
    );

    if (reg_result != VPX_CODEC_OK) {
        // Get error string for the registration failure
        const char* reg_err_str = vpx_codec_err_to_string(reg_result);
        (void)reg_err_str; // In fuzzing, we might not use this
    }

    // Read data from file and decode
    std::vector<uint8_t> buffer(size);
    size_t bytes_read = fread(buffer.data(), 1, size, in_file);
    
    if (bytes_read > 0) {
        // Decode the input data
        vpx_codec_decode(&decoder, buffer.data(), buffer.size(), nullptr, 0);
        
        // Get decoded frames (drain the decoder)
        vpx_codec_iter_t iter = nullptr;
        while (vpx_codec_get_frame(&decoder, &iter)) {
            // Frames are processed in callback
        }
    }

    // Now test encoder initialization with default config
    vpx_codec_ctx_t encoder;
    memset(&encoder, 0, sizeof(encoder));
    
    // Get encoder interface
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    if (!encoder_iface) {
        encoder_iface = vpx_codec_vp8_cx();
    }
    
    if (encoder_iface) {
        vpx_codec_enc_cfg_t enc_cfg;
        memset(&enc_cfg, 0, sizeof(enc_cfg));
        
        // Get default encoder configuration
        vpx_codec_err_t cfg_result = vpx_codec_enc_config_default(
            encoder_iface, 
            &enc_cfg, 
            0 // usage: good quality
        );
        
        if (cfg_result == VPX_CODEC_OK) {
            // Could initialize encoder here, but for fuzzing we'll just
            // demonstrate the API was called successfully
            (void)cfg_result;
        }
        
        // Get encoder error detail if initialization fails
        if (cfg_result != VPX_CODEC_OK) {
            const char* enc_err_detail = vpx_codec_error_detail(&encoder);
            (void)enc_err_detail; // In fuzzing, we might not use this
        }
    }

    // Get decoder error detail before destroying
    const char* final_err_detail = vpx_codec_error_detail(&decoder);
    (void)final_err_detail; // In fuzzing, we might not use this

    // Cleanup
    vpx_codec_destroy(&decoder);
    
    if (encoder_iface) {
        vpx_codec_destroy(&encoder);
    }
    
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}