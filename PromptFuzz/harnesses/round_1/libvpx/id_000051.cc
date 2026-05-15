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
//<ID> 175
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_wrap","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_control_","vpx_codec_control_","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_wrap","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_control_","vpx_codec_control_","vpx_codec_enc_config_set","vpx_codec_set_cx_data_buf","vpx_img_set_rect","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to process encoded VP9 data, decode it, and re-encode it:

*/


// libvpx headers

// Callback structure to track decoded frames
struct DecoderState {
    vpx_codec_ctx_t* encoder = nullptr;
    vpx_image_t* image = nullptr;
    bool frame_received = false;
};

// Callback function for decoded frames
static void  id_000051_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    DecoderState* state = static_cast<DecoderState*>(user_priv);
    
    if (state && img) {
        // If we haven't allocated an image yet, create one
        if (!state->image) {
            state->image = vpx_img_alloc(nullptr, img->fmt, img->d_w, img->d_h, 16);
            if (state->image) {
                // Copy the received image data
                for (int plane = 0; plane < 3; ++plane) {
                    unsigned int plane_width = (plane > 0) ? (img->d_w >> img->x_chroma_shift) : img->d_w;
                    unsigned int plane_height = (plane > 0) ? (img->d_h >> img->y_chroma_shift) : img->d_h;
                    
                    for (unsigned int y = 0; y < plane_height; ++y) {
                        memcpy(state->image->planes[plane] + y * state->image->stride[plane],
                               img->planes[plane] + y * img->stride[plane],
                               plane_width);
                    }
                }
                state->frame_received = true;
            }
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Too small to be valid VP9 data
    
    DecoderState decoder_state;
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    
    if (vpx_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Register frame callback
    vpx_codec_register_put_frame_cb(&decoder,  id_000051_put_frame_callback, &decoder_state);
    
    // Decode the input data
    vpx_codec_decode(&decoder, data, static_cast<unsigned int>(size), nullptr, 0);
    
    // Try to get a frame from decoder
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t* decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    if (decoded_img) {
        // If we got a frame directly (not through callback), process it
        if (!decoder_state.image) {
            decoder_state.image = vpx_img_alloc(nullptr, decoded_img->fmt, 
                                                decoded_img->d_w, decoded_img->d_h, 16);
            if (decoder_state.image) {
                // Wrap the decoded image data
                vpx_img_wrap(decoder_state.image, decoded_img->fmt,
                            decoded_img->d_w, decoded_img->d_h, 16,
                            const_cast<unsigned char*>(decoded_img->planes[0]));
                decoder_state.frame_received = true;
            }
        }
    }
    
    // If we have a decoded frame, try to re-encode it
    if (decoder_state.frame_received && decoder_state.image) {
        // Initialize encoder
        vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
        vpx_codec_ctx_t encoder;
        vpx_codec_enc_cfg_t enc_cfg;
        
        // Get default encoder configuration
        if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0) != VPX_CODEC_OK) {
            vpx_img_free(decoder_state.image);
            vpx_codec_destroy(&decoder);
            return 0;
        }
        
        // Configure encoder
        enc_cfg.g_w = decoder_state.image->d_w;
        enc_cfg.g_h = decoder_state.image->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_pass = VPX_RC_ONE_PASS;
        enc_cfg.g_lag_in_frames = 0;
        
        // Initialize encoder
        if (vpx_codec_enc_init(&encoder, encoder_iface, &enc_cfg, 0) != VPX_CODEC_OK) {
            vpx_img_free(decoder_state.image);
            vpx_codec_destroy(&decoder);
            return 0;
        }
        
        // Set some encoder controls
        vpx_codec_control_(&encoder, VP8E_SET_CPUUSED, 8);
        vpx_codec_control_(&encoder, VP9E_SET_TILE_COLUMNS, 2);
        
        // Apply configuration
        vpx_codec_enc_config_set(&encoder, &enc_cfg);
        
        // Set up output buffer for encoded data
        vpx_fixed_buf_t output_buf;
        size_t max_output_size = 1024 * 1024;  // 1MB buffer
        output_buf.buf = malloc(max_output_size);
        output_buf.sz = 0;
        
        if (output_buf.buf) {
            // Set output buffer
            vpx_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);
            
            // Set a rectangle on the image (just use the whole image)
            vpx_img_set_rect(decoder_state.image, 0, 0, 
                           decoder_state.image->d_w, decoder_state.image->d_h);
            
            // Encode the frame
            vpx_codec_encode(&encoder, decoder_state.image, 0, 1, 0, VPX_DL_REALTIME);
            
            // Get encoded data
            vpx_codec_iter_t enc_iter = nullptr;
            const vpx_codec_cx_pkt_t* pkt;
            
            while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    // Frame packet - could process further here
                    break;
                }
            }
            
            // Try to get preview frame
            const vpx_image_t* preview_frame = vpx_codec_get_preview_frame(&encoder);
            (void)preview_frame;  // Preview frame may not always be available
            
            free(output_buf.buf);
        }
        
        // Destroy encoder
        vpx_codec_destroy(&encoder);
        
        // Free the image
        vpx_img_free(decoder_state.image);
    }
    
    // Destroy decoder
    vpx_codec_destroy(&decoder);
    
    return 0;
}