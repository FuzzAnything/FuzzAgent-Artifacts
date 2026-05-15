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
//<ID> 52
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_destroy","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_img_set_rect","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_get_preview_frame","vpx_img_set_rect","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// Frame buffer callback structures
typedef struct fb_context {
    uint8_t* frame_buffers[4];
    size_t frame_buffer_sizes[4];
    int buffer_used;
} fb_context_t;

// Frame buffer allocation callback
int  id_000024_get_frame_buffer_cb(void* cb_priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
    fb_context_t* ctx = static_cast<fb_context_t*>(cb_priv);
    
    if (ctx->buffer_used >= 4) {
        return -1; // No more buffers available
    }
    
    // Allocate buffer
    ctx->frame_buffers[ctx->buffer_used] = static_cast<uint8_t*>(malloc(min_size));
    if (!ctx->frame_buffers[ctx->buffer_used]) {
        return -1;
    }
    
    ctx->frame_buffer_sizes[ctx->buffer_used] = min_size;
    
    fb->data = ctx->frame_buffers[ctx->buffer_used];
    fb->size = min_size;
    fb->priv = reinterpret_cast<void*>(static_cast<intptr_t>(ctx->buffer_used));
    
    ctx->buffer_used++;
    return 0;
}

// Frame buffer release callback
int  id_000024_release_frame_buffer_cb(void* cb_priv, vpx_codec_frame_buffer_t* fb) {
    fb_context_t* ctx = static_cast<fb_context_t*>(cb_priv);
    int buffer_index = static_cast<int>(reinterpret_cast<intptr_t>(fb->priv));
    
    if (buffer_index >= 0 && buffer_index < 4 && ctx->frame_buffers[buffer_index]) {
        free(ctx->frame_buffers[buffer_index]);
        ctx->frame_buffers[buffer_index] = nullptr;
        ctx->frame_buffer_sizes[buffer_index] = 0;
    }
    return 0;
}

// Put slice callback
void  id_000024_put_slice_cb(void* user_priv, const vpx_image_t* img,
                  const vpx_image_rect_t* slice_rect,
                  const vpx_image_rect_t* border) {
    // This callback can be used for progressive rendering or processing
    // For fuzzing, we just validate that we received valid parameters
    if (img && slice_rect && border) {
        // Basic parameter validation
        if (slice_rect->x + slice_rect->w <= img->d_w &&
            slice_rect->y + slice_rect->h <= img->d_h) {
            // Valid slice received
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }
    
    // Initialize decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    
    if (!decoder_iface) {
        return 0;
    }
    
    // Initialize decoder with default configuration
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, decoder_iface, 
                                                  &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        return 0;
    }
    
    // Register put slice callback
    void* user_priv = nullptr;
    res = vpx_codec_register_put_slice_cb(&decoder,  id_000024_put_slice_cb, user_priv);
    // Note: This might fail if decoder doesn't support slice callbacks
    // Continue even if it fails
    
    // Set up frame buffer callbacks
    fb_context_t fb_ctx = {0};
    res = vpx_codec_set_frame_buffer_functions(&decoder,  id_000024_get_frame_buffer_cb, 
                                                id_000024_release_frame_buffer_cb, &fb_ctx);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Get stream information
    vpx_codec_stream_info_t si;
    si.sz = sizeof(si);
    
    // First try to peek stream info
    res = vpx_codec_peek_stream_info(decoder_iface, data, 
                                     static_cast<unsigned int>(size), &si);
    
    if (res == VPX_CODEC_OK) {
        // Now decode the frame
        res = vpx_codec_decode(&decoder, data, static_cast<unsigned int>(size), 
                               nullptr, 0);
        
        if (res == VPX_CODEC_OK) {
            // Try to get stream info after decoding
            vpx_codec_stream_info_t si_after;
            si_after.sz = sizeof(si_after);
            vpx_codec_get_stream_info(&decoder, &si_after);
            
            // Get decoded frames
            vpx_codec_iter_t iter = nullptr;
            const vpx_image_t* img;
            
            while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
                // Try to get preview frame (though typically for encoders)
                // In decoder context, this might fail or return null
                const vpx_image_t* preview = vpx_codec_get_preview_frame(&decoder);
                
                // If we have a valid image, test vpx_img_set_rect
                if (img->fmt != VPX_IMG_FMT_NONE) {
                    // Create a copy of the image to test set_rect
                    vpx_image_t img_copy;
                    memcpy(&img_copy, img, sizeof(vpx_image_t));
                    
                    // Calculate safe rectangle bounds
                    unsigned int x = 0;
                    unsigned int y = 0;
                    unsigned int w = img->d_w > 0 ? img->d_w : 1;
                    unsigned int h = img->d_h > 0 ? img->d_h : 1;
                    
                    // Ensure rectangle is within bounds
                    if (x < w && y < h) {
                        vpx_img_set_rect(&img_copy, x, y, w, h);
                    }
                }
            }
        }
    }
    
    // Also test with VP8 decoder
    vpx_codec_ctx_t decoder_vp8;
    vpx_codec_iface_t* decoder_iface_vp8 = vpx_codec_vp8_dx();
    
    if (decoder_iface_vp8) {
        if (vpx_codec_dec_init_ver(&decoder_vp8, decoder_iface_vp8, 
                                    &dec_cfg, 0, VPX_DECODER_ABI_VERSION) == VPX_CODEC_OK) {
            // Try to decode with VP8 (input might be VP9, but decoder will handle it)
            vpx_codec_decode(&decoder_vp8, data, static_cast<unsigned int>(size), 
                            nullptr, 0);
            
            // Try to get global headers (though typically for encoders)
            // This might fail or return null in decoder context
            vpx_fixed_buf_t* headers = vpx_codec_get_global_headers(&decoder_vp8);
            if (headers) {
                free(headers->buf);
            }
            
            vpx_codec_destroy(&decoder_vp8);
        }
    }
    
    // Clean up VP9 decoder
    vpx_codec_destroy(&decoder);
    
    // Clean up frame buffers if any weren't released
    for (int i = 0; i < 4; i++) {
        if (fb_ctx.frame_buffers[i]) {
            free(fb_ctx.frame_buffers[i]);
            fb_ctx.frame_buffers[i] = nullptr;
        }
    }
    
    return 0;
}