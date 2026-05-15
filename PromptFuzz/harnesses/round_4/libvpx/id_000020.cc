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
//<ID> 287
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_destroy","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_img_wrap","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_get_stream_info","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_img_wrap","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_get_stream_info","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for decoded frames
static void  id_000020_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // This callback receives decoded frames from the decoder
    // For fuzzing purposes, we just record that a frame was received
    static int frame_count = 0;
    frame_count++;
    
    // We could process the frame here, but for fuzzing we just validate it
    if (img && img->fmt != VPX_IMG_FMT_NONE) {
        // Frame is valid
    }
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    vpx_codec_ctx_t decoder;
    vpx_codec_ctx_t encoder;
    vpx_codec_stream_info_t si;
    vpx_codec_enc_cfg_t cfg;
    vpx_rational_t dsf = {1, 1};  // Downsampling factor (no downsampling)
    vpx_image_t *img = nullptr;
    vpx_codec_err_t res;
    
    // Initialize stream info structure
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    // First, try to get stream information from the input data
    // This tells us if the input looks like valid VP8/VP9 data
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    res = vpx_codec_peek_stream_info(decoder_iface, data, size, &si);
    
    // Skip if we can't parse the stream info
    if (res != VPX_CODEC_OK) {
        // Try VP8 instead
        decoder_iface = vpx_codec_vp8_dx();
        res = vpx_codec_peek_stream_info(decoder_iface, data, size, &si);
        if (res != VPX_CODEC_OK) {
            return 0;
        }
    }
    
    // Initialize decoder configuration
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;  // Single thread for fuzzing
    dec_cfg.w = si.w;
    dec_cfg.h = si.h;
    
    // Initialize decoder
    res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        return 0;
    }
    
    // Register put frame callback
    res = vpx_codec_register_put_frame_cb(&decoder,  id_000020_put_frame_callback, nullptr);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Try to decode the input data
    // We use a long deadline for maximum quality/performance
    res = vpx_codec_decode(&decoder, data, size, nullptr, fuzz_int64_t_2);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Get decoded frame to use for encoding
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    if (decoded_img) {
        // Get encoder interface
        vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
        
        // Get default encoder configuration
        res = vpx_codec_enc_config_default(encoder_iface, &cfg, fuzz_uint32_t_3);
        if (res != VPX_CODEC_OK) {
            vpx_codec_destroy(&decoder);
            return 0;
        }
        
        // Configure encoder based on decoded image
        cfg.g_w = decoded_img->d_w;
        cfg.g_h = decoded_img->d_h;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 256;
        cfg.g_threads = 1;
        
        // Initialize encoder with multiple encoders (2 in this case)
        // This simulates a multi-encoder scenario
        res = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &cfg, 2, fuzz_int64_t_4, &dsf, VPX_ENCODER_ABI_VERSION);
        if (res != VPX_CODEC_OK) {
            vpx_codec_destroy(&decoder);
            return 0;
        }
        
        // Create a wrapped image for encoding
        // We use the decoded image's data directly
        unsigned char *img_data = decoded_img->img_data;
        if (!img_data) {
            // Allocate new image data if needed
            size_t img_size = decoded_img->d_w * decoded_img->d_h * 3 / 2;  // I420 format
            img_data = (unsigned char *)malloc(img_size);
            if (img_data) {
                // Fill with some pattern for fuzzing
                for (size_t i = 0; i < img_size; i++) {
                    img_data[i] = (unsigned char)(i % 256);
                }
            }
        }
        
        if (img_data) {
            // Wrap the image data
            img = vpx_img_wrap(nullptr, VPX_IMG_FMT_I420, 
                               decoded_img->d_w, decoded_img->d_h, 
                               fuzz_uint32_t_5, img_data);
            
            if (img) {
                // Try to encode the wrapped image
                res = vpx_codec_encode(&encoder, img, fuzz_int64_t_6, fuzz_uint64_t_7, fuzz_int64_t_8, fuzz_uint64_t_9);
                
                // Get encoded data
                const vpx_codec_cx_pkt_t *pkt;
                vpx_codec_iter_t enc_iter = nullptr;
                while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                    // Process encoded packets if needed
                    if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                        // Encoded frame available
                    }
                }
                
                vpx_img_free(img);
            }
            
            // Free allocated image data if we allocated it
            if (img_data && img_data != decoded_img->img_data) {
                free(img_data);
            }
        }
        
        // Get stream info from encoder context
        // This would typically be done before encoding starts
        vpx_codec_stream_info_t enc_si;
        memset(&enc_si, 0, sizeof(enc_si));
        enc_si.sz = sizeof(enc_si);
        vpx_codec_get_stream_info(&encoder, &enc_si);
        
        vpx_codec_destroy(&encoder);
    }
    
    vpx_codec_destroy(&decoder);
    return 0;
}