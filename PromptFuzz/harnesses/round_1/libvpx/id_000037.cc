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
//<ID> 98
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_set_cx_data_buf","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_vp8_cx","vpx_codec_vp9_cx","vpx_codec_vp9_dx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Check if input is too small for meaningful processing
    if (size < 10) {
	return 0;
	}
	

    // Step 1: Try to identify the codec type from the input data
    vpx_codec_iface_t *codec_iface = nullptr;
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // First try VP9
    codec_iface = vpx_codec_vp9_dx();
    if (vpx_codec_peek_stream_info(codec_iface, data, size, &stream_info) == VPX_CODEC_OK) {
        // Input appears to be VP9
    } else {
        // Try VP8
        codec_iface = vpx_codec_vp8_dx();
        if (vpx_codec_peek_stream_info(codec_iface, data, size, &stream_info) != VPX_CODEC_OK) {
            // Not a valid VP8 or VP9 stream
            return 0;
        }
    }
    
    // Step 2: Get decoder capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(codec_iface);
    // Note: caps value depends on the codec implementation
    
    // Step 3: Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    dec_cfg.w = stream_info.w;
    dec_cfg.h = stream_info.h;
    
    if (vpx_codec_dec_init_ver(&decoder, codec_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        // Failed to initialize decoder
        return 0;
    }
    
    // Step 4: Set up an encoder context as well for vpx_codec_set_cx_data_buf
    // We need an encoder to use vpx_codec_set_cx_data_buf
    vpx_codec_iface_t *encoder_iface = (codec_iface == vpx_codec_vp9_dx()) ? 
                                        vpx_codec_vp9_cx() : vpx_codec_vp8_cx();
    
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    memset(&enc_cfg, 0, sizeof(enc_cfg));
    
    // Initialize encoder with default configuration
    if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Set encoder dimensions based on stream info
    enc_cfg.g_w = stream_info.w;
    enc_cfg.g_h = stream_info.h;
    
    if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Step 5: Create a fixed buffer and use vpx_codec_set_cx_data_buf
    vpx_fixed_buf_t buffer;
    // Allocate a buffer for encoded data
    size_t buf_size = size * 2; // Double the input size for safety
    buffer.buf = malloc(buf_size);
    if (!buffer.buf) {
        vpx_codec_destroy(&encoder);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    buffer.sz = buf_size;
    
    // Try to set the buffer (this may fail if not supported by encoder)
    vpx_codec_set_cx_data_buf(&encoder, &buffer, 0, 0);
    
    // Step 6: Decode the input data
    if (vpx_codec_decode(&decoder, data, size, nullptr, 0) == VPX_CODEC_OK) {
        // Get decoded frames
        vpx_codec_iter_t iter = nullptr;
        const vpx_image_t *img;
        while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
            // We have a decoded frame, but for this fuzzing scenario,
            // we're just testing the API calls
        }
    }
    
    // Step 7: Get global headers from encoder
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
    if (global_headers) {
        // Global headers retrieved (could be null)
    }
    
    // Step 8: Clean up
    free(buffer.buf);
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    
    return 0;
}