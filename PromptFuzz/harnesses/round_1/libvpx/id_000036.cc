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
//<ID> 94
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_multi_ver","vpx_codec_vp9_cx","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_img_alloc","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_multi_ver","vpx_codec_vp9_cx","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_multi_ver","vpx_codec_vp9_cx","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // First, try to use the input as a compressed VP9 bitstream
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    // Peek at stream info using the input data (assuming it's compressed video)
    vpx_codec_err_t peek_res = vpx_codec_peek_stream_info(
        vpx_codec_vp9_dx(),  // Try VP9 decoder
        data, 
        static_cast<unsigned int>(size), 
        &si
    );
    
    // Initialize decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    
    vpx_codec_err_t dec_init_res = vpx_codec_dec_init_ver(
        &decoder,
        vpx_codec_vp9_dx(),
        &dec_cfg,
        0,
        VPX_DECODER_ABI_VERSION
    );
    
    if (dec_init_res == VPX_CODEC_OK) {
        // Decode the input data
        vpx_codec_decode(
            &decoder,
            data,
            static_cast<unsigned int>(size),
            nullptr,
            0
        );
        
        // Try to get decoded frames
        vpx_codec_iter_t iter = nullptr;
        const vpx_image_t *decoded_frame;
        while ((decoded_frame = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
            // Create a new image for encoding
            vpx_image_t *img = vpx_img_alloc(
                nullptr,
                VPX_IMG_FMT_I420,
                decoded_frame->d_w > 0 ? decoded_frame->d_w : 64,
                decoded_frame->d_h > 0 ? decoded_frame->d_h : 64,
                1
            );
            
            if (img) {
                // Initialize encoder context for VP9
                vpx_codec_ctx_t encoder;
                vpx_codec_enc_cfg_t cfg;
                
                // Get default encoder configuration
                vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &cfg, 0);
                
                // Set reasonable dimensions
                cfg.g_w = img->d_w;
                cfg.g_h = img->d_h;
                cfg.g_threads = 1;
                
                // Initialize encoder with multi_ver API
                vpx_rational_t dsf = {1, 1};  // No downsampling
                vpx_codec_err_t enc_init_res = vpx_codec_enc_init_multi_ver(
                    &encoder,
                    vpx_codec_vp9_cx(),
                    &cfg,
                    1,          // num_enc = 1 encoder
                    0,          // flags
                    &dsf,       // downsampling factor
                    VPX_ENCODER_ABI_VERSION
                );
                
                if (enc_init_res == VPX_CODEC_OK) {
                    // Encode the image
                    vpx_codec_encode(
                        &encoder,
                        img,
                        0,      // pts
                        1,      // duration
                        0,      // flags
                        VPX_DL_REALTIME  // deadline
                    );
                    
                    // Get encoded data
                    vpx_codec_iter_t enc_iter = nullptr;
                    const vpx_codec_cx_pkt_t *pkt;
                    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                        // Process packet if needed
                    }
                    
                    // Clean up encoder
                    vpx_codec_destroy(&encoder);
                }
                
                // Clean up image
                vpx_img_free(img);
            }
        }
        
        // Clean up decoder
        vpx_codec_destroy(&decoder);
    }
    
    // If decoder initialization failed, still try to use the input with encoder
    // by creating a synthetic image
    else {
        // Create a small test image
        vpx_image_t *img = vpx_img_alloc(
            nullptr,
            VPX_IMG_FMT_I420,
            64,
            64,
            1
        );
        
        if (img && size > 0) {
            // Fill image planes with input data (truncated to fit)
            size_t bytes_to_copy = size;
            if (bytes_to_copy > img->w * img->h * 3 / 2) {
                bytes_to_copy = img->w * img->h * 3 / 2;
            }
            
            if (bytes_to_copy > 0 && img->planes[0]) {
                memcpy(img->planes[0], data, bytes_to_copy > img->w * img->h ? 
                       img->w * img->h : bytes_to_copy);
            }
            
            // Initialize encoder
            vpx_codec_ctx_t encoder;
            vpx_codec_enc_cfg_t cfg;
            vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &cfg, 0);
            
            cfg.g_w = img->d_w;
            cfg.g_h = img->d_h;
            cfg.g_threads = 1;
            
            vpx_rational_t dsf = {1, 1};
            vpx_codec_err_t enc_init_res = vpx_codec_enc_init_multi_ver(
                &encoder,
                vpx_codec_vp9_cx(),
                &cfg,
                1,
                0,
                &dsf,
                VPX_ENCODER_ABI_VERSION
            );
            
            if (enc_init_res == VPX_CODEC_OK) {
                // Encode the image
                vpx_codec_encode(
                    &encoder,
                    img,
                    0,
                    1,
                    0,
                    VPX_DL_REALTIME
                );
                
                // Get encoded data
                vpx_codec_iter_t enc_iter = nullptr;
                const vpx_codec_cx_pkt_t *pkt;
                while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                    // Process packet
                }
                
                vpx_codec_destroy(&encoder);
            }
            
            vpx_img_free(img);
        }
    }
    
    return 0;
}