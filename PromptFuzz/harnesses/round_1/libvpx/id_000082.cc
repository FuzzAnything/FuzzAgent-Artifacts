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
//<ID> 315
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_peek_stream_info","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_img_alloc","vpx_codec_encode","vpx_codec_destroy","vpx_img_free","vpx_codec_get_frame","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_peek_stream_info","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_img_alloc","vpx_codec_encode","vpx_codec_destroy","vpx_img_free","vpx_codec_get_frame","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses libvpx to decode and potentially re-encode video data:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small for any meaningful processing
    if (size < 128) {
        return 0;
    }

    // Create a memory file for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize variables
    vpx_codec_ctx_t decoder = {0};
    vpx_codec_ctx_t encoder = {0};
    vpx_codec_enc_cfg_t enc_cfg = {0};
    vpx_codec_stream_info_t stream_info = {0};
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *image = nullptr;
    vpx_image_t *allocated_img = nullptr;
    vpx_codec_iface_t *decoder_iface = nullptr;
    vpx_codec_iface_t *encoder_iface = nullptr;
    int fuzz_ret = 0;
    
    // Determine which codec to use by trying to peek stream info
    // Try VP9 first
    decoder_iface = vpx_codec_vp9_dx();
    encoder_iface = vpx_codec_vp9_cx();
    
    stream_info.sz = sizeof(stream_info);
    vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
        decoder_iface, data, size, &stream_info);
    
    // If VP9 doesn't work, try VP8
    if (peek_result != VPX_CODEC_OK) {
        decoder_iface = vpx_codec_vp8_dx();
        encoder_iface = vpx_codec_vp8_cx();
        
        stream_info.sz = sizeof(stream_info);
        peek_result = vpx_codec_peek_stream_info(
            decoder_iface, data, size, &stream_info);
    }
    
    // Get codec capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(encoder_iface);
    (void)caps; // Use caps to avoid unused variable warning
    
    // Initialize decoder
    vpx_codec_err_t dec_init_result = vpx_codec_dec_init_ver(
        &decoder, decoder_iface, nullptr, 0, VPX_DECODER_ABI_VERSION);
    
    if (dec_init_result != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Decode the input data
    vpx_codec_err_t decode_result = vpx_codec_decode(
        &decoder, data, size, nullptr, 0);
    
    // Get stream info from decoder context
    stream_info.sz = sizeof(stream_info);
    vpx_codec_err_t stream_info_result = vpx_codec_get_stream_info(
        &decoder, &stream_info);
    
    // Try to get decoded frame
    image = vpx_codec_get_frame(&decoder, &iter);
    
    // If we got a frame, try encoding it
    if (image) {
        // Get default encoder configuration
        vpx_codec_err_t cfg_default_result = vpx_codec_enc_config_default(
            encoder_iface, &enc_cfg, 0);
        
        if (cfg_default_result == VPX_CODEC_OK) {
            // Configure encoder based on decoded image
            enc_cfg.g_w = image->d_w;
            enc_cfg.g_h = image->d_h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.rc_target_bitrate = 256; // 256 kbps
            
            // Initialize encoder
            vpx_codec_err_t enc_init_result = vpx_codec_enc_init_ver(
                &encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
            
            if (enc_init_result == VPX_CODEC_OK) {
                // Update encoder configuration with the same settings
                vpx_codec_err_t enc_cfg_set_result = vpx_codec_enc_config_set(
                    &encoder, &enc_cfg);
                
                // Allocate an image using vpx_img_alloc
                allocated_img = vpx_img_alloc(
                    nullptr, VPX_IMG_FMT_I420, image->d_w, image->d_h, 16);
                
                if (allocated_img) {
                    // Copy image data (simplified - in reality would need proper conversion)
                    // Here we just demonstrate the API usage
                    (void)allocated_img;
                    
                    // Encode the frame (deadline of 0 = use default)
                    vpx_codec_encode(&encoder, image, 0, 1, 0, 0);
                }
                
                // Clean up encoder
                vpx_codec_destroy(&encoder);
            }
            
            // Free allocated image
            if (allocated_img) {
                vpx_img_free(allocated_img);
            }
        }
    }
    
    // Try to get another frame (if multiple frames were decoded)
    while (vpx_codec_get_frame(&decoder, &iter)) {
        // Just iterate through remaining frames
    }
    
    // Clean up decoder
    vpx_codec_destroy(&decoder);
    
    // Close input file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}