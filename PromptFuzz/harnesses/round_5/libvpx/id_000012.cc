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
//<ID> 24
//<Prompt> []
/*<Combination>: [
*/
//<score> 37.5, nr_unique_branch: 11
//<Quality> {"density":15,"unique_branches":{"vp8dx_receive_compressed_data":[[324,7,324,18,0,0,4,0],[325,9,325,47,0,0,4,0],[331,9,331,43,0,0,4,1]],"vp8_decode":[[307,7,307,25,0,0,4,1],[308,15,308,32,0,0,4,0],[308,36,308,42,0,0,4,1],[342,15,342,33,0,0,4,1],[386,9,386,26,0,0,4,1],[511,9,511,43,0,0,4,0]],"vp8_decode_frame":[[965,35,965,62,0,0,4,0]],"update_fragments":[[244,7,244,32,0,0,4,1]]},"library_calls":["vpx_codec_version","vpx_codec_version_extra_str","vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_err_to_string","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_control_","vpx_codec_destroy","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_img_wrap","vpx_codec_destroy"],"critical_calls":["vpx_codec_version","vpx_codec_version_extra_str","vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_err_to_string","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_control_","vpx_codec_destroy","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_img_wrap","vpx_codec_destroy"],"visited":1}
/**/


// Include libvpx headers
extern "C" {
}

// Callback function for put_frame
static void  id_000012_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // This is just a placeholder callback for fuzzing
    (void)user_priv;
    (void)img;
}

// Custom structure to hold input data and position
struct input_data_context {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

// Function to read from our custom buffer
static size_t  id_000012_read_callback(void* user_priv, uint8_t* buffer, size_t size) {
    input_data_context* ctx = reinterpret_cast<input_data_context*>(user_priv);
    size_t remaining = ctx->size - ctx->pos;
    size_t to_read = (size < remaining) ? size : remaining;
    
    if (to_read > 0) {
        memcpy(buffer, ctx->data + ctx->pos, to_read);
        ctx->pos += to_read;
    }
    
    return to_read;
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if data is too small
    if (size < 100) {
	return 0;
	}
	
    
    // Create a custom input context
    input_data_context input_ctx = {data, size, 0};
    
    // Call version APIs
    int version = vpx_codec_version();
    (void)version; // Mark as used
    
    const char* version_extra = vpx_codec_version_extra_str();
    (void)version_extra; // Mark as used
    
    // Initialize codec interfaces
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp8_dx();
    if (!decoder_iface) {
        decoder_iface = vpx_codec_vp9_dx();
    }
    
    if (!decoder_iface) {
	return 0;
	}
	
    
    // Try to peek stream info using the input data
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // Use a portion of input data for stream info
    size_t peek_size = (size > 1000) ? 1000 : size;
    vpx_codec_err_t peek_err = vpx_codec_peek_stream_info(
        decoder_iface, data, peek_size, &stream_info);
    
    const char* err_str = vpx_codec_err_to_string(peek_err);
    (void)err_str; // Mark as used
    
    // Create decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_err_t dec_init_err = vpx_codec_dec_init_ver(
        &decoder, decoder_iface, NULL, 0, VPX_DECODER_ABI_VERSION);
    
    if (dec_init_err != VPX_CODEC_OK) {
        return 0;
    }
    
    // Register put frame callback
    vpx_codec_err_t reg_err = vpx_codec_register_put_frame_cb(
        &decoder,  id_000012_put_frame_callback, nullptr);
    (void)reg_err; // Mark as used
    
    // Create encoder context if we have enough data
    if (size > 500) {
        vpx_codec_ctx_t encoder;
        vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx();
        
        vpx_codec_enc_cfg_t enc_cfg;
        memset(&enc_cfg, 0, sizeof(enc_cfg));
        
        // Try to get default encoder configuration
        vpx_codec_err_t cfg_err = vpx_codec_enc_config_default(
            encoder_iface, &enc_cfg, 0);
        
        if (cfg_err == VPX_CODEC_OK) {
            // Set some reasonable values from input data
            enc_cfg.g_w = (data[0] % 640) + 1;
            enc_cfg.g_h = (data[1] % 480) + 1;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            
            // Initialize encoder
            vpx_codec_err_t enc_init_err = vpx_codec_enc_init_ver(
                &encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
            
            if (enc_init_err == VPX_CODEC_OK) {
                // Try to set encoder configuration
                vpx_codec_err_t set_cfg_err = vpx_codec_enc_config_set(
                    &encoder, &enc_cfg);
                (void)set_cfg_err; // Mark as used
                
                // Try to call control API
                vpx_codec_err_t ctrl_err = vpx_codec_control_(
                    &encoder, VP8E_SET_CPUUSED, data[2] % 10);
                (void)ctrl_err; // Mark as used
                
                // Clean up encoder
                vpx_codec_destroy(&encoder);
            }
        }
    }
    
    // Try to decode the input data in chunks
    size_t decode_pos = 0;
    size_t chunk_size = 1024;
    
    while (decode_pos < size && (size - decode_pos) > 10) {
        size_t current_chunk = (size - decode_pos < chunk_size) ? 
                              size - decode_pos : chunk_size;
        
        vpx_codec_err_t decode_err = vpx_codec_decode(
            &decoder, data + decode_pos, current_chunk, nullptr, 0);
        
        decode_pos += current_chunk;
        
        // Try to get stream info from decoder context
        vpx_codec_stream_info_t dec_stream_info;
        memset(&dec_stream_info, 0, sizeof(dec_stream_info));
        dec_stream_info.sz = sizeof(dec_stream_info);
        
        vpx_codec_err_t get_info_err = vpx_codec_get_stream_info(
            &decoder, &dec_stream_info);
        (void)get_info_err; // Mark as used
        
        // Get decoded frames
        vpx_codec_iter_t iter = nullptr;
        vpx_image_t* img;
        while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
            // Test vpx_img_wrap with a small buffer
            if (size > 2000) {
                vpx_image_t wrapped_img;
                unsigned char img_buffer[100];
                
                // Use some input data as image buffer
                size_t buffer_size = (size - 1000 > 100) ? 100 : size - 1000;
                if (buffer_size > 0) {
                    vpx_img_wrap(&wrapped_img, VPX_IMG_FMT_I420, 
                                16, 16, 1, const_cast<unsigned char*>(data + 1000));
                }
            }
        }
    }
    
    // Clean up decoder
    vpx_codec_destroy(&decoder);
    
    return 0;
}