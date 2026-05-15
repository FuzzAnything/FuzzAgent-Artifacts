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
//<ID> 54
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 5
//<Quality> {"density":8,"unique_branches":{"vpx_codec_enc_config_set":[[352,15,352,26,0,0,4,0]],"vp9_rc_get_default_max_gf_interval":[[25,23,25,34,2,0,4,1]],"vpx_codec_get_preview_frame":[[318,9,318,20,0,0,4,0]],"vp9_rc_set_gf_interval_range":[[24,23,24,34,5,0,4,1]],"vpx_realloc_frame_buffer":[[220,11,220,59,0,0,4,0]]},"library_calls":["vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// Callback function for put_slice
static void  id_000021_put_slice_callback(void *user_priv, const vpx_image_t *img,
                               const vpx_image_rect_t *rect, 
                               const vpx_image_rect_t *rect2) {
    // Empty callback for fuzzing
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

// Callback function for put_frame
static void  id_000021_put_frame_callback(void *user_priv, const vpx_image_t *img) {
    // Empty callback for fuzzing
    (void)user_priv;
    (void)img;
}

// Frame buffer callbacks
static int  id_000021_get_frame_buffer_callback(void *user_priv, size_t min_size,
                                     vpx_codec_frame_buffer_t *fb) {
    (void)user_priv;
    (void)min_size;
    (void)fb;
    return -1; // Indicate failure
}

static int  id_000021_release_frame_buffer_callback(void *user_priv,
                                         vpx_codec_frame_buffer_t *fb) {
    (void)user_priv;
    (void)fb;
    return -1; // Indicate failure
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    vpx_codec_ctx_t decoder;
    vpx_codec_ctx_t encoder;
    vpx_codec_stream_info_t stream_info;
    vpx_codec_enc_cfg_t encoder_config;
    vpx_image_t *preview_frame = nullptr;
    vpx_codec_iter_t iter = nullptr;
    
    // Initialize stream info
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // 1. Peek at stream info to get basic information
    vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
        vpx_codec_vp9_dx(), data, static_cast<unsigned int>(size), &stream_info);
    
    if (peek_result != VPX_CODEC_OK) {
        // Try VP8 if VP9 fails
        peek_result = vpx_codec_peek_stream_info(
            vpx_codec_vp8_dx(), data, static_cast<unsigned int>(size), &stream_info);
    }
    
    // 2. Initialize decoder
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_err_t dec_init_result = vpx_codec_dec_init_ver(
        &decoder, decoder_iface, nullptr, 0, VPX_DECODER_ABI_VERSION);
    
    if (dec_init_result != VPX_CODEC_OK) {
        // Try VP8 if VP9 fails
        decoder_iface = vpx_codec_vp8_dx();
        dec_init_result = vpx_codec_dec_init_ver(
            &decoder, decoder_iface, nullptr, 0, VPX_DECODER_ABI_VERSION);
    }
    
    if (dec_init_result == VPX_CODEC_OK) {
        // 3. Register put_frame callback for decoder
        vpx_codec_register_put_frame_cb(&decoder,  id_000021_put_frame_callback, nullptr);
        
        // 4. Register put_slice callback for decoder
        vpx_codec_register_put_slice_cb(&decoder,  id_000021_put_slice_callback, nullptr);
        
        // 5. Set frame buffer functions
        vpx_codec_set_frame_buffer_functions(&decoder,
             id_000021_get_frame_buffer_callback,
             id_000021_release_frame_buffer_callback,
            nullptr);
        
        // 6. Initialize encoder with default configuration
        vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
        vpx_codec_enc_config_default(encoder_iface, &encoder_config, 0);
        
        // Adjust encoder config based on stream info if available
        if (stream_info.w > 0 && stream_info.h > 0) {
            encoder_config.g_w = stream_info.w;
            encoder_config.g_h = stream_info.h;
        } else {
            // Use small default values for fuzzing
            encoder_config.g_w = 64;
            encoder_config.g_h = 64;
        }
        
        // 7. Initialize encoder
        vpx_codec_enc_init_ver(&encoder, encoder_iface, &encoder_config, 0,
                               VPX_ENCODER_ABI_VERSION);
        
        // 8. Set encoder configuration (redundant but for API usage)
        vpx_codec_enc_config_set(&encoder, &encoder_config);
        
        // 9. Get preview frame from encoder (may be null)
        preview_frame = (vpx_image_t*)vpx_codec_get_preview_frame(&encoder);
        
        // 10. Encode the preview frame if we have one
        if (preview_frame) {
            vpx_codec_encode(&encoder, preview_frame, 0, 1, 0, 0);
        }
        
        // 11. Decode the input data
        vpx_codec_decode(&decoder, data, static_cast<unsigned int>(size),
                         nullptr, 0);
        
        // 12. Get decoded frames
        const vpx_image_t *img;
        while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
            // If we have a decoded frame, try to encode it
            if (img) {
                vpx_codec_encode(&encoder, img, 0, 1, 0, 0);
                
                // Get encoded data
                const vpx_codec_cx_pkt_t *pkt;
                vpx_codec_iter_t enc_iter = nullptr;
                while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                    // Process encoded packet if needed
                    (void)pkt;
                }
            }
        }
        
        // Cleanup encoder
        vpx_codec_destroy(&encoder);
        
        // Cleanup decoder
        vpx_codec_destroy(&decoder);
    }
    
    return 0;
}