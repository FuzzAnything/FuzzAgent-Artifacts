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
//<ID> 383
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 2
//<Quality> {"density":18,"unique_branches":{"read_uncompressed_header":[[2779,7,2779,23,0,0,4,0]],"check_resync":[[300,32,300,53,0,0,4,1]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_err_to_string","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_err_to_string","vpx_codec_register_put_frame_cb","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_err_to_string","vpx_codec_register_put_frame_cb","vpx_codec_err_to_string","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to achieve decoding and re-encoding of video frames:

*/


extern "C" {
}

// Global callback counter
static int  id_000089_frame_callback_count = 0;

// Callback function for decoded frames
static void  id_000089_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    if (img && user_priv) {
         id_000089_frame_callback_count++;
        
        // Get encoder context from user_priv
        vpx_codec_ctx_t* encoder_ctx = static_cast<vpx_codec_ctx_t*>(user_priv);
        
        // Try to encode the decoded frame
        if (encoder_ctx) {
            vpx_codec_err_t encode_res = vpx_codec_encode(
                encoder_ctx, 
                img, 
                0,  // pts
                1,  // duration
                0,  // flags
                0   // deadline
            );
            
            // Check for encoding errors
            if (encode_res != VPX_CODEC_OK) {
                const char* error_str = vpx_codec_err_to_string(encode_res);
                // Error string is consumed
            }
            
            // Try to get preview frame from encoder
            const vpx_image_t* preview = vpx_codec_get_preview_frame(encoder_ctx);
            if (preview) {
                // Preview frame available - we just need to call the API
            }
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Reset callback counter
     id_000089_frame_callback_count = 0;
    
    // Create input file from memory buffer
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize decoder
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    
    vpx_codec_dec_cfg_t dec_cfg;
    dec_cfg.threads = 1;
    dec_cfg.w = 0;
    dec_cfg.h = 0;
    
    vpx_codec_err_t dec_init_res = vpx_codec_dec_init_ver(
        &decoder_ctx, 
        decoder_iface, 
        &dec_cfg, 
        0,  // flags
        VPX_DECODER_ABI_VERSION
    );
    
    if (dec_init_res != VPX_CODEC_OK) {
        const char* error_str = vpx_codec_err_to_string(dec_init_res);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize encoder
    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    
    // Set some reasonable values for fuzzing
    enc_cfg.g_w = 320;
    enc_cfg.g_h = 240;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.g_threads = 1;
    enc_cfg.g_pass = VPX_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    
    vpx_codec_err_t enc_init_res = vpx_codec_enc_init_ver(
        &encoder_ctx, 
        encoder_iface, 
        &enc_cfg, 
        0,  // flags
        VPX_ENCODER_ABI_VERSION
    );
    
    if (enc_init_res != VPX_CODEC_OK) {
        const char* error_str = vpx_codec_err_to_string(enc_init_res);
        vpx_codec_destroy(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set encoder configuration (even if same as initial)
    vpx_codec_err_t config_set_res = vpx_codec_enc_config_set(&encoder_ctx, &enc_cfg);
    if (config_set_res != VPX_CODEC_OK) {
        const char* error_str = vpx_codec_err_to_string(config_set_res);
    }
    
    // Register frame callback with encoder context as user_priv
    vpx_codec_err_t register_res = vpx_codec_register_put_frame_cb(
        &decoder_ctx, 
         id_000089_put_frame_callback, 
        &encoder_ctx
    );
    
    if (register_res != VPX_CODEC_OK) {
        const char* error_str = vpx_codec_err_to_string(register_res);
    }
    
    // Read and decode the input data
    // Use a buffer to simulate streaming
    const size_t chunk_size = 4096;
    uint8_t* buffer = new uint8_t[chunk_size];
    
    size_t total_read = 0;
    while (total_read < size && !feof(in_file)) {
        size_t to_read = (size - total_read) > chunk_size ? chunk_size : (size - total_read);
        size_t read = fread(buffer, 1, to_read, in_file);
        
        if (read > 0) {
            // Decode the chunk
            vpx_codec_err_t decode_res = vpx_codec_decode(
                &decoder_ctx, 
                buffer, 
                read, 
                nullptr,  // user_priv
                0         // deadline
            );
            
            if (decode_res != VPX_CODEC_OK) {
                const char* error_str = vpx_codec_err_to_string(decode_res);
                // Continue despite errors for fuzzing
            }
            
            // Get decoded frames
            vpx_codec_iter_t iter = nullptr;
            const vpx_image_t* img;
            while ((img = vpx_codec_get_frame(&decoder_ctx, &iter)) != nullptr) {
                // Frame will be processed in callback
            }
            
            total_read += read;
        }
    }
    
    // Try to get preview frame one more time
    const vpx_image_t* preview = vpx_codec_get_preview_frame(&encoder_ctx);
    if (preview) {
        // Preview frame exists
    }
    
    // Clean up
    delete[] buffer;
    vpx_codec_destroy(&decoder_ctx);
    vpx_codec_destroy(&encoder_ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}