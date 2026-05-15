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
//<ID> 121
//<Prompt> []
/*<Combination>: [
*/
//<score> 10.5, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_error_detail","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<104) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_13, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_14, fdp);
	//fuzzer shim end}




    if (size < 1024) {
	return 0;
	}
	 // Need minimum data for initialization

    // Create input file from fuzzing data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize decoder interface
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    const char *iface_name = vpx_codec_iface_name(decoder_iface);
    
    // Read first 1024 bytes for configuration (simplified approach)
    uint8_t header[1024];
    size_t read_size = fread(header, 1, sizeof(header), in_file);
    fseek(in_file, 0, SEEK_SET);
    
    // Initialize decoder
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, 
                                                 &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&decoder_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Decode the input data
    err = vpx_codec_decode(&decoder_ctx, data, size, nullptr, fuzz_int64_t_2);
    
    // Get error detail if decoding failed
    if (err != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&decoder_ctx);
    }
    
    // Try to get a decoded frame
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *img = vpx_codec_get_frame(&decoder_ctx, &iter);
    
    if (img) {
        // Create a copy of the image to manipulate
        vpx_image_t *img_copy = vpx_img_alloc(nullptr, img->fmt, 
                                              img->d_w, img->d_h, fuzz_uint32_t_3);
        if (img_copy) {
            // Copy image data
            for (int plane = 0; plane < 3; ++plane) {
                if (img->planes[plane]) {
                    size_t plane_size = (plane == 0) ? 
                        img->d_w * img->d_h : 
                        (img->d_w >> img->x_chroma_shift) * 
                        (img->d_h >> img->y_chroma_shift);
                    memcpy(img_copy->planes[plane], img->planes[plane], plane_size);
                }
            }
            
            // Set a rectangular region on the image (crop to center 50%)
            unsigned int x = img->d_w / 4;
            unsigned int y = img->d_h / 4;
            unsigned int w = img->d_w / 2;
            unsigned int h = img->d_h / 2;
            vpx_img_set_rect(img_copy, x, y, w, h);
            
            // Initialize encoder configuration
            vpx_codec_enc_cfg_t enc_cfg;
            vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
            
            // Get default configuration
            err = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_4);
            
            if (err == VPX_CODEC_OK) {
                // Set dimensions from cropped image
                enc_cfg.g_w = w;
                enc_cfg.g_h = h;
                enc_cfg.rc_target_bitrate = 1000;
                enc_cfg.g_threads = 1;
                enc_cfg.g_timebase.num = 1;
                enc_cfg.g_timebase.den = 30;
                enc_cfg.g_pass = VPX_RC_ONE_PASS;
                enc_cfg.rc_end_usage = VPX_VBR;
                
                // Initialize first encoder using vpx_codec_enc_init_ver
                vpx_codec_ctx_t encoder_ctx1;
                err = vpx_codec_enc_init_ver(&encoder_ctx1, encoder_iface, 
                                             &enc_cfg, fuzz_int64_t_5, VPX_ENCODER_ABI_VERSION);
                
                if (err == VPX_CODEC_OK) {
                    // Encode the cropped image
                    err = vpx_codec_encode(&encoder_ctx1, img_copy, fuzz_int64_t_6, fuzz_uint64_t_7, fuzz_int64_t_8, fuzz_uint64_t_9);
                    
                    // Get encoded data
                    vpx_codec_iter_t enc_iter = nullptr;
                    const vpx_codec_cx_pkt_t *pkt;
                    while ((pkt = vpx_codec_get_cx_data(&encoder_ctx1, &enc_iter)) != nullptr) {
                        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                            // Encoded frame available
                        }
                    }
                    
                    vpx_codec_destroy(&encoder_ctx1);
                }
                
                // Initialize second encoder using vpx_codec_enc_init_multi_ver
                // For simplicity, we initialize with 1 encoder instance
                vpx_codec_ctx_t encoder_ctx2;
                vpx_rational_t dsf = {1, 1}; // Downsampling factor
                
                err = vpx_codec_enc_init_multi_ver(&encoder_ctx2, encoder_iface, 
                                                   &enc_cfg, 1, fuzz_int64_t_10, &dsf, 
                                                   VPX_ENCODER_ABI_VERSION);
                
                if (err == VPX_CODEC_OK) {
                    // Encode with the second encoder
                    err = vpx_codec_encode(&encoder_ctx2, img_copy, fuzz_int64_t_11, fuzz_uint64_t_12, fuzz_int64_t_13, fuzz_uint64_t_14);
                    
                    // Get error detail if encoding failed
                    if (err != VPX_CODEC_OK) {
                        const char *error_detail = vpx_codec_error_detail(&encoder_ctx2);
                    }
                    
                    vpx_codec_destroy(&encoder_ctx2);
                }
            }
            
            vpx_img_free(img_copy);
        }
    }
    
    // Cleanup
    vpx_codec_destroy(&decoder_ctx);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}