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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_error","vpx_codec_decode","vpx_codec_error","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_img_alloc","vpx_img_set_rect","vpx_img_flip","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_enc_config_set","vpx_codec_error","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_img_alloc","vpx_img_set_rect","vpx_img_flip","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_enc_config_set","vpx_codec_error","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1024) {
	return 0;
	}
	 // Need minimum data for meaningful processing
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx(); // Use VP9 decoder
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t res = vpx_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0);
    if (res != VPX_CODEC_OK) {
        // Try VP8 decoder if VP9 fails
        decoder_iface = vpx_codec_vp8_dx();
        res = vpx_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0);
        if (res != VPX_CODEC_OK) {
            const char* error = vpx_codec_error(&decoder);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // Decode the input data
    res = vpx_codec_decode(&decoder, data, size, NULL, 0);
    if (res != VPX_CODEC_OK) {
        const char* error = vpx_codec_error(&decoder);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get decoded frame
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    if (decoded_img) {
        // Create encoder configuration
        vpx_codec_enc_cfg_t cfg;
        vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx(); // Use VP9 encoder
        res = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
        
        if (res == VPX_CODEC_OK) {
            // Set encoder parameters
            cfg.g_w = decoded_img->d_w;
            cfg.g_h = decoded_img->d_h;
            cfg.g_timebase.num = 1;
            cfg.g_timebase.den = 30;
            cfg.rc_target_bitrate = 1000;
            cfg.g_error_resilient = VPX_ERROR_RESILIENT_DEFAULT;
            
            // Create multiple encoder instances
            vpx_codec_ctx_t encoders[2];
            vpx_rational_t dsf[2] = {{1, 1}, {1, 2}};
            int num_enc = 2;
            
            // Initialize multiple encoders - first mandatory API
            res = vpx_codec_enc_init_multi_ver(&encoders[0], encoder_iface, &cfg, 
                                             num_enc, 0, dsf, VPX_ENCODER_ABI_VERSION);
            
            if (res == VPX_CODEC_OK) {
                // Create a copy of the image for manipulation
                vpx_image_t *img = vpx_img_alloc(NULL, decoded_img->fmt, 
                                                decoded_img->d_w, decoded_img->d_h, 16);
                if (img) {
                    // Copy image data
                    for (int plane = 0; plane < 3; plane++) {
                        size_t plane_size = (plane == 0) ? 
                            img->d_h * img->stride[plane] : 
                            (img->d_h / 2) * img->stride[plane];
                        if (decoded_img->planes[plane] && img->planes[plane]) {
                            memcpy(img->planes[plane], decoded_img->planes[plane], 
                                   plane_size);
                        }
                    }
                    
                    // Set rectangle on image - second mandatory API
                    unsigned int rect_x = 0;
                    unsigned int rect_y = 0;
                    unsigned int rect_w = img->d_w / 2;
                    unsigned int rect_h = img->d_h / 2;
                    vpx_img_set_rect(img, rect_x, rect_y, rect_w, rect_h);
                    
                    // Flip image - third mandatory API
                    vpx_img_flip(img);
                    
                    // Re-encode with first encoder
                    res = vpx_codec_encode(&encoders[0], img, 0, 1, 0, VPX_DL_REALTIME);
                    
                    if (res == VPX_CODEC_OK) {
                        // Get encoded data
                        vpx_codec_iter_t enc_iter = NULL;
                        const vpx_codec_cx_pkt_t *pkt;
                        while ((pkt = vpx_codec_get_cx_data(&encoders[0], &enc_iter)) != NULL) {
                            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                            }
                        }
                    }
                    
                    // Modify encoder configuration for second encoder
                    cfg.rc_target_bitrate = 500;
                    cfg.g_w = img->d_w / 2;
                    cfg.g_h = img->d_h / 2;
                    
                    // Set encoder configuration - fourth mandatory API
                    res = vpx_codec_enc_config_set(&encoders[1], &cfg);
                    
                    if (res != VPX_CODEC_OK) {
                        // Use error API - fifth mandatory API
                        const char* error = vpx_codec_error(&encoders[1]);
                    }
                    
                    // Clean up
                    vpx_img_free(img);
                }
                
                // Destroy encoders
                for (int i = 0; i < num_enc; i++) {
                    vpx_codec_destroy(&encoders[i]);
                }
            }
        }
    }
    
    // Clean up
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}