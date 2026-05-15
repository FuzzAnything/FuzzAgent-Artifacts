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
//<ID> 235
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_err_to_string","vpx_codec_dec_init_ver","vpx_codec_err_to_string","vpx_codec_set_frame_buffer_functions","vpx_codec_destroy","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_err_to_string","vpx_codec_destroy","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_get_global_headers","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


// Include necessary libvpx headers

// Custom frame buffer callback structure
struct FrameBufferData {
    uint8_t* data;
    size_t size;
};

// Frame buffer allocation callback
int  id_000063_get_frame_buffer_cb(void* cb_priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
    FrameBufferData* fbd = static_cast<FrameBufferData*>(cb_priv);
    if (min_size > fbd->size) {
        fbd->data = static_cast<uint8_t*>(realloc(fbd->data, min_size));
        fbd->size = min_size;
    }
    fb->data = fbd->data;
    fb->size = fbd->size;
    return 0;
}

// Frame buffer release callback
int  id_000063_release_frame_buffer_cb(void* cb_priv, vpx_codec_frame_buffer_t* fb) {
    // Nothing to do here - we manage our own memory
    return 0;
}

// Put frame callback
void  id_000063_put_frame_cb(void* user_priv, const vpx_image_t* img) {
    // Callback for when a frame is decoded
    // We don't need to do anything here for fuzzing
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data
    
    // 1. Create VP9 decoder interface
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    if (!decoder_iface) {
	return 0;
	}
	
    
    // 2. Get decoder capabilities
    vpx_codec_caps_t decoder_caps = vpx_codec_get_caps(decoder_iface);
    (void)decoder_caps; // Use capabilities as needed
    
    // 3. Peek at stream information
    vpx_codec_stream_info_t stream_info;
    stream_info.sz = sizeof(stream_info);
    vpx_codec_err_t peek_err = vpx_codec_peek_stream_info(
        decoder_iface, data, static_cast<unsigned int>(size), &stream_info);
    
    if (peek_err != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(peek_err);
        (void)err_str; // Error string for debugging
        return 0;
    }
    
    // 4. Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = stream_info.w;
    dec_cfg.h = stream_info.h;
    
    vpx_codec_err_t dec_init_err = vpx_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (dec_init_err != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(dec_init_err);
        (void)err_str;
        return 0;
    }
    
    // 5. Set frame buffer functions for decoder
    FrameBufferData frame_buffer_data = {nullptr, 0};
    vpx_codec_err_t set_buf_err = vpx_codec_set_frame_buffer_functions(
        &decoder,  id_000063_get_frame_buffer_cb,  id_000063_release_frame_buffer_cb, &frame_buffer_data);
    
    if (set_buf_err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        free(frame_buffer_data.data);
        return 0;
    }
    
    // 6. Register put frame callback
    vpx_codec_err_t reg_err = vpx_codec_register_put_frame_cb(
        &decoder,  id_000063_put_frame_cb, nullptr);
    (void)reg_err; // Continue even if registration fails
    
    // 7. Decode the input data
    vpx_codec_err_t decode_err = vpx_codec_decode(
        &decoder, data, static_cast<unsigned int>(size), nullptr, 0);
    
    if (decode_err != VPX_CODEC_OK) {
        const char* err_str = vpx_codec_err_to_string(decode_err);
        (void)err_str;
        vpx_codec_destroy(&decoder);
        free(frame_buffer_data.data);
        return 0;
    }
    
    // 8. Get decoded frames
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t* decoded_img = nullptr;
    
    // Try to get first frame
    decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    // If we got a frame, try to encode it
    if (decoded_img) {
        // 9. Allocate an image for encoding
        vpx_image_t enc_img;
        vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
        
        // Use dimensions from decoded image or stream info
        unsigned int width = decoded_img->d_w ? decoded_img->d_w : stream_info.w;
        unsigned int height = decoded_img->d_h ? decoded_img->d_h : stream_info.h;
        
        if (width == 0 || height == 0) {
            width = 128;
            height = 128;
        }
        
        vpx_image_t* allocated_img = vpx_img_alloc(&enc_img, fmt, width, height, 1);
        if (!allocated_img) {
            vpx_codec_destroy(&decoder);
            free(frame_buffer_data.data);
            return 0;
        }
        
        // 10. Set a rectangle on the image (use safe values)
        unsigned int rect_x = 0;
        unsigned int rect_y = 0;
        unsigned int rect_w = width > 16 ? width / 2 : width;
        unsigned int rect_h = height > 16 ? height / 2 : height;
        
        vpx_img_set_rect(allocated_img, rect_x, rect_y, rect_w, rect_h);
        
        // 11. Create VP9 encoder interface
        vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
        if (!encoder_iface) {
            vpx_img_free(allocated_img);
            vpx_codec_destroy(&decoder);
            free(frame_buffer_data.data);
            return 0;
        }
        
        // 12. Get default encoder configuration
        vpx_codec_enc_cfg_t enc_cfg;
        unsigned int usage = 0; // Good quality
        vpx_codec_err_t cfg_err = vpx_codec_enc_config_default(
            encoder_iface, &enc_cfg, usage);
        
        if (cfg_err != VPX_CODEC_OK) {
            vpx_img_free(allocated_img);
            vpx_codec_destroy(&decoder);
            free(frame_buffer_data.data);
            return 0;
        }
        
        // Set encoder dimensions
        enc_cfg.g_w = width;
        enc_cfg.g_h = height;
        enc_cfg.g_threads = 1;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30; // 30fps
        enc_cfg.rc_target_bitrate = 256; // 256 kbps
        
        // 13. Initialize first encoder (single instance)
        vpx_codec_ctx_t encoder1;
        vpx_codec_err_t enc_init_err1 = vpx_codec_enc_init_ver(
            &encoder1, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
        
        if (enc_init_err1 != VPX_CODEC_OK) {
            vpx_img_free(allocated_img);
            vpx_codec_destroy(&decoder);
            free(frame_buffer_data.data);
            return 0;
        }
        
        // 14. Initialize second encoder (multi-instance, but only 1 instance)
        vpx_codec_ctx_t encoder2;
        vpx_codec_err_t enc_init_err2 = vpx_codec_enc_init_multi_ver(
            &encoder2, encoder_iface, &enc_cfg, 1, 0, nullptr, VPX_ENCODER_ABI_VERSION);
        
        if (enc_init_err2 != VPX_CODEC_OK) {
            vpx_codec_destroy(&encoder1);
            vpx_img_free(allocated_img);
            vpx_codec_destroy(&decoder);
            free(frame_buffer_data.data);
            return 0;
        }
        
        // 15. Get global headers from first encoder
        vpx_fixed_buf_t* global_headers = vpx_codec_get_global_headers(&encoder1);
        (void)global_headers; // Use or free as needed
        
        // 16. Encode with first encoder
        vpx_codec_err_t encode_err1 = vpx_codec_encode(
            &encoder1, allocated_img, 0, 1, 0, VPX_DL_REALTIME);
        
        if (encode_err1 == VPX_CODEC_OK) {
            // Get encoded data from first encoder
            vpx_codec_iter_t enc_iter1 = nullptr;
            const vpx_codec_cx_pkt_t* pkt1 = nullptr;
            while ((pkt1 = vpx_codec_get_cx_data(&encoder1, &enc_iter1)) != nullptr) {
                // Process packet if needed
                (void)pkt1;
            }
            
            // Try to get preview frame from first encoder
            const vpx_image_t* preview1 = vpx_codec_get_preview_frame(&encoder1);
            (void)preview1;
        }
        
        // 17. Encode with second encoder
        vpx_codec_err_t encode_err2 = vpx_codec_encode(
            &encoder2, allocated_img, 1, 1, 0, VPX_DL_REALTIME);
        
        if (encode_err2 == VPX_CODEC_OK) {
            // Get encoded data from second encoder
            vpx_codec_iter_t enc_iter2 = nullptr;
            const vpx_codec_cx_pkt_t* pkt2 = nullptr;
            while ((pkt2 = vpx_codec_get_cx_data(&encoder2, &enc_iter2)) != nullptr) {
                // Process packet if needed
                (void)pkt2;
            }
        }
        
        // 18. Get preview frame from decoder (if available)
        const vpx_image_t* preview_frame = vpx_codec_get_preview_frame(&decoder);
        (void)preview_frame;
        
        // 19. Clean up encoders
        vpx_codec_destroy(&encoder1);
        vpx_codec_destroy(&encoder2);
        
        // 20. Clean up allocated image
        vpx_img_free(allocated_img);
    }
    
    // 21. Clean up decoder
    vpx_codec_destroy(&decoder);
    
    // 22. Clean up frame buffer data
    free(frame_buffer_data.data);
    
    return 0;
}