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
//<ID> 50
//<Prompt> []
/*<Combination>: [
*/
//<score> 33, nr_unique_branch: 2
//<Quality> {"density":11,"unique_branches":{"vpx_codec_decode":[[112,46,112,54,0,0,4,0]],"vpx_codec_peek_stream_info":[[71,26,71,34,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_slice_cb","vpx_codec_register_put_frame_cb","vpx_codec_set_frame_buffer_functions","vpx_img_alloc","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_preview_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_register_put_slice_cb","vpx_codec_register_put_frame_cb","vpx_codec_set_frame_buffer_functions","vpx_img_alloc","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_slice
static void  id_000019_put_slice_callback(void *user_priv, const vpx_image_t *img, 
                               const vpx_image_rect_t *slice, const vpx_image_rect_t *recon) {
    (void)user_priv; (void)img; (void)slice; (void)recon;
    // Do nothing - just a placeholder callback
}

// Callback function for put_frame
static void  id_000019_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    (void)user_priv; (void)img;
    // Do nothing - just a placeholder callback
}

// Frame buffer get callback
static int  id_000019_get_frame_buffer_cb(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    (void)cb_priv; (void)min_size; (void)fb;
    return -1; // Indicate failure
}

// Frame buffer release callback
static int  id_000019_release_frame_buffer_cb(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    (void)cb_priv; (void)fb;
    return -1; // Indicate failure
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Use the input data as both compressed video data and raw image data
    // We'll split the input into two parts for different purposes
    size_t half_size = size / 2;
    const uint8_t *compressed_data = data;
    size_t compressed_size = half_size;
    const uint8_t *raw_image_data = data + half_size;
    size_t raw_image_size = size - half_size;
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 2;
    dec_cfg.w = 128;
    dec_cfg.h = 128;
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 
                                                 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        // Failed to initialize decoder, but continue with other APIs
    }
    
    // Peek at stream info using the compressed data
    vpx_codec_stream_info_t stream_info = {0};
    stream_info.sz = sizeof(stream_info);
    vpx_codec_peek_stream_info(decoder_iface, compressed_data, 
                               (unsigned int)compressed_size, &stream_info);
    
    // Try to decode the compressed data
    if (res == VPX_CODEC_OK) {
        vpx_codec_decode(&decoder, compressed_data, (unsigned int)compressed_size, 
                        NULL, 0);
        
        // Try to get a preview frame
        const vpx_image_t *preview = vpx_codec_get_preview_frame(&decoder);
        (void)preview; // Use preview to avoid unused variable warning
    }
    
    // Initialize encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_enc_cfg_t enc_cfg;
    
    res = vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (res == VPX_CODEC_OK) {
        enc_cfg.g_w = 128;
        enc_cfg.g_h = 128;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 256;
        
        res = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, 
                                     VPX_ENCODER_ABI_VERSION);
        if (res == VPX_CODEC_OK) {
            // Set encoder configuration
            vpx_codec_enc_config_set(&encoder, &enc_cfg);
            
            // Register callbacks
            vpx_codec_register_put_slice_cb(&encoder,  id_000019_put_slice_callback, NULL);
            vpx_codec_register_put_frame_cb(&encoder,  id_000019_put_frame_callback, NULL);
            
            // Set frame buffer functions
            vpx_codec_set_frame_buffer_functions(&encoder,  id_000019_get_frame_buffer_cb, 
                                                 id_000019_release_frame_buffer_cb, NULL);
            
            // Create an image from raw data if we have enough
            if (raw_image_size >= 128 * 128 * 3 / 2) { // Minimum for I420
                vpx_image_t img;
                vpx_img_alloc(&img, VPX_IMG_FMT_I420, 128, 128, 1);
                
                // Fill image planes with raw data
                size_t offset = 0;
                for (int plane = 0; plane < 3; plane++) {
                    int plane_w = (plane == 0) ? 128 : 64;
                    int plane_h = (plane == 0) ? 128 : 64;
                    size_t plane_size = plane_w * plane_h;
                    
                    if (offset + plane_size <= raw_image_size) {
                        memcpy(img.planes[plane], raw_image_data + offset, plane_size);
                        offset += plane_size;
                    }
                }
                
                // Encode the image
                vpx_codec_encode(&encoder, &img, 0, 1, 0, VPX_DL_REALTIME);
                
                // Clean up image
                vpx_img_free(&img);
            }
            
            // Destroy encoder
            vpx_codec_destroy(&encoder);
        }
    }
    
    // Destroy decoder if it was initialized
    if (res == VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
    }
    
    return 0;
}