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
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["vpx_codec_version_str","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_destroy","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_img_alloc","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_str","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_img_alloc","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to decode and re-encode video frames:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // 1. Get version string (required API)
    const char *version_str = vpx_codec_version_str();
    (void)version_str; // Prevent unused variable warning
    
    // 2. Create a FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    
    // Check decoder capabilities (required API)
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    (void)caps; // Prevent unused variable warning
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, NULL, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Read and decode frames
    uint8_t *buffer = (uint8_t *)malloc(size);
    if (!buffer) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(buffer, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    if (bytes_read > 0) {
        // Decode the frame
        res = vpx_codec_decode(&decoder, buffer, bytes_read, NULL, 0);
        
        // Get error detail if decoding failed (required API)
        if (res != VPX_CODEC_OK) {
            const char *error_detail = vpx_codec_error_detail(&decoder);
            (void)error_detail; // Prevent unused variable warning
        }
        
        // Get decoded frame
        vpx_codec_iter_t iter = NULL;
        vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
        
        if (decoded_img) {
            // 5. Initialize encoder
            vpx_codec_ctx_t encoder;
            vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
            
            // Get default encoder config (required API)
            vpx_codec_enc_cfg_t cfg;
            res = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
            
            if (res == VPX_CODEC_OK) {
                // Configure encoder based on decoded frame
                cfg.g_w = decoded_img->d_w;
                cfg.g_h = decoded_img->d_h;
                cfg.g_timebase.num = 1;
                cfg.g_timebase.den = 30;
                cfg.rc_target_bitrate = 1000;
                cfg.g_pass = VPX_RC_ONE_PASS;
                cfg.g_lag_in_frames = 0;
                
                // Allocate image for encoder (required API)
                vpx_image_t *input_img = vpx_img_alloc(NULL, decoded_img->fmt, 
                                                      decoded_img->d_w, decoded_img->d_h, 16);
                
                if (input_img) {
                    // Copy decoded image to input image
                    for (int plane = 0; plane < 3; plane++) {
                        uint8_t *src = decoded_img->planes[plane];
                        uint8_t *dst = input_img->planes[plane];
                        int src_stride = decoded_img->stride[plane];
                        int dst_stride = input_img->stride[plane];
                        int plane_height = plane == 0 ? decoded_img->d_h : (decoded_img->d_h + 1) >> 1;
                        
                        for (int y = 0; y < plane_height; y++) {
                            memcpy(dst, src, decoded_img->d_w >> (plane ? 1 : 0));
                            src += src_stride;
                            dst += dst_stride;
                        }
                    }
                    
                    // Initialize encoder
                    res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
                    
                    if (res == VPX_CODEC_OK) {
                        // Encode the frame
                        res = vpx_codec_encode(&encoder, input_img, 0, 1, 0, VPX_DL_REALTIME);
                        
                        if (res == VPX_CODEC_OK) {
                            // Get encoded data
                            vpx_codec_iter_t enc_iter = NULL;
                            const vpx_codec_cx_pkt_t *pkt;
                            
                            while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                                    // Frame encoded successfully
                                    break;
                                }
                            }
                        }
                        
                        // Clean up encoder
                        vpx_codec_destroy(&encoder);
                    }
                    
                    // Free allocated image
                    vpx_img_free(input_img);
                }
            }
        }
    }
    
    // 6. Cleanup
    free(buffer);
    vpx_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);
	return 0;
}