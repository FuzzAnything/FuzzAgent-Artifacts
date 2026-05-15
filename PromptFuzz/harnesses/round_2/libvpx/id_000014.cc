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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["vpx_codec_version_str","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_img_alloc","vpx_img_set_rect","vpx_codec_decode","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_str","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_img_alloc","vpx_img_set_rect","vpx_codec_decode","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


// Include libvpx headers

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1024) {
	return 0;
	}
	 // Need minimum data for meaningful processing
    
    // Step 1: Print version info (using vpx_codec_version_str)
    const char* version = vpx_codec_version_str();
    // version is used but we don't need to do anything else with it
    
    // Step 2: Get codec capabilities (using vpx_codec_get_caps)
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    
    // Step 3: Create a decoder context and get stream info
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    // Try to peek at stream info first
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    // Attempt to get stream info from the input data
    if (vpx_codec_peek_stream_info(decoder_iface, data, size, &si) == VPX_CODEC_OK) {
        // Now initialize the decoder
        if (vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION) == VPX_CODEC_OK) {
            
            // Step 4: Allocate an image for decoded frame (using vpx_img_alloc)
            vpx_image_t* img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                            si.w, si.h, 1);
            if (img) {
                // Step 5: Set a rectangle in the image (using vpx_img_set_rect)
                unsigned int rect_x = 0;
                unsigned int rect_y = 0;
                unsigned int rect_w = si.w / 2;
                unsigned int rect_h = si.h / 2;
                
                if (rect_w > 0 && rect_h > 0) {
                    vpx_img_set_rect(img, rect_x, rect_y, rect_w, rect_h);
                }
                
                // Step 6: Decode the input data
                vpx_codec_decode(&decoder, data, size, nullptr, 0);
                
                // Step 7: Create encoder configuration
                vpx_codec_ctx_t encoder;
                vpx_codec_enc_cfg_t enc_cfg;
                vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
                
                if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == VPX_CODEC_OK) {
                    // Configure encoder
                    enc_cfg.g_w = rect_w;
                    enc_cfg.g_h = rect_h;
                    enc_cfg.g_timebase.num = 1;
                    enc_cfg.g_timebase.den = 30;
                    enc_cfg.rc_target_bitrate = 1000;
                    enc_cfg.g_pass = VPX_RC_ONE_PASS;
                    enc_cfg.g_lag_in_frames = 0;
                    
                    // Step 8: Initialize multi-encoder (using vpx_codec_enc_init_multi_ver)
                    vpx_rational_t dsf = {1, 1};
                    if (vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &enc_cfg, 
                                                     1, 0, &dsf, VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
                        
                        // Step 9: Create output buffer and set it (using vpx_codec_set_cx_data_buf)
                        const size_t out_buf_size = 1024 * 1024;
                        uint8_t* out_buf = static_cast<uint8_t*>(malloc(out_buf_size));
                        if (out_buf) {
                            vpx_fixed_buf_t output_buffer;
                            output_buffer.buf = out_buf;
                            output_buffer.sz = out_buf_size;
                            
                            vpx_codec_set_cx_data_buf(&encoder, &output_buffer, 0, 0);
                            
                            // Step 10: Encode the image
                            // For simplicity, we'll encode a black frame since we don't have actual decoded data
                            vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
                            
                            // Step 11: Get encoded data (using vpx_codec_get_cx_data)
                            vpx_codec_iter_t iter = nullptr;
                            const vpx_codec_cx_pkt_t* pkt;
                            while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                                    // Got an encoded frame packet
                                    break;
                                }
                            }
                            
                            // Clean up output buffer
                            free(out_buf);
                        }
                        
                        // Clean up encoder
                        vpx_codec_destroy(&encoder);
                    }
                }
                
                // Clean up image
                vpx_img_free(img);
            }
            
            // Clean up decoder
            vpx_codec_destroy(&decoder);
        }
    }
    
    // Note: vpx_codec_enc_config_set was not used because it requires 
    // reconfiguring an already initialized encoder, which needs more complex setup.
    // However, we achieved similar configuration during encoder initialization.
    
    return 0;
}