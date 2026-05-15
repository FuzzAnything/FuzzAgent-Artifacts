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
//<ID> 346
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"setup_frame_size_with_refs":[[1608,7,1616,25,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_destroy","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_img_alloc","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_img_alloc","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callback context
struct FrameBufferContext {
    uint8_t* buffer;
    size_t buffer_size;
    bool buffer_used;
};

// Frame buffer allocation callback
int  id_000086_get_frame_buffer_cb(void* cb_priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
    FrameBufferContext* ctx = static_cast<FrameBufferContext*>(cb_priv);
    
    if (ctx->buffer_used || min_size > ctx->buffer_size) {
        return -1;
    }
    
    fb->data = ctx->buffer;
    fb->size = ctx->buffer_size;
    ctx->buffer_used = true;
    return 0;
}

// Frame buffer release callback
int  id_000086_release_frame_buffer_cb(void* cb_priv, vpx_codec_frame_buffer_t* fb) {
    FrameBufferContext* ctx = static_cast<FrameBufferContext*>(cb_priv);
    ctx->buffer_used = false;
    return 0;
}

// Put frame callback
void  id_000086_put_frame_cb(void* user_priv, const vpx_image_t* img) {
    // In fuzzing, we just acknowledge the callback
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }
    
    // Use the input data as a VP9 encoded stream
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx();
    
    if (!decoder_iface || !encoder_iface) {
        return 0;
    }
    
    // Get codec capabilities
    vpx_codec_caps_t decoder_caps = vpx_codec_get_caps(decoder_iface);
    vpx_codec_caps_t encoder_caps = vpx_codec_get_caps(encoder_iface);
    
    (void)decoder_caps;
    (void)encoder_caps;
    
    // Peek stream info from input data
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    vpx_codec_err_t peek_res = vpx_codec_peek_stream_info(decoder_iface, data, 
                                                          (unsigned int)(size > 1024 ? 1024 : size), &si);
    (void)peek_res;
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t dec_init_res = vpx_codec_dec_init_ver(&decoder, decoder_iface, 
                                                         &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (dec_init_res != VPX_CODEC_OK) {
        return 0;
    }
    
    // Register put frame callback for decoder
    vpx_codec_register_put_frame_cb(&decoder,  id_000086_put_frame_cb, nullptr);
    
    // Set frame buffer functions for decoder
    FrameBufferContext fb_ctx;
    const size_t buffer_size = 1024 * 1024;
    fb_ctx.buffer = static_cast<uint8_t*>(malloc(buffer_size));
    if (!fb_ctx.buffer) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    fb_ctx.buffer_size = buffer_size;
    fb_ctx.buffer_used = false;
    
    vpx_codec_set_frame_buffer_functions(&decoder,  id_000086_get_frame_buffer_cb, 
                                         id_000086_release_frame_buffer_cb, &fb_ctx);
    
    // Decode the input
    vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, 0);
    
    // Get stream info from decoder
    vpx_codec_get_stream_info(&decoder, &si);
    
    // Initialize encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    
    // Get default encoder configuration
    vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    
    // Set encoder configuration based on stream info
    if (si.w > 0 && si.h > 0) {
        enc_cfg.g_w = si.w;
        enc_cfg.g_h = si.h;
    } else {
        enc_cfg.g_w = 320;
        enc_cfg.g_h = 240;
    }
    enc_cfg.g_threads = 1;
    enc_cfg.rc_target_bitrate = 256;
    
    vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
    
    // Set encoder configuration again (as required by API usage)
    vpx_codec_enc_config_set(&encoder, &enc_cfg);
    
    // Allocate image for encoder input
    vpx_image_t* img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                     enc_cfg.g_w, enc_cfg.g_h, 16);
    if (!img) {
        free(fb_ctx.buffer);
        vpx_codec_destroy(&decoder);
        vpx_codec_destroy(&encoder);
        return 0;
    }
    
    // Initialize image data with some content
    const size_t y_size = img->d_w * img->d_h;
    const size_t uv_size = y_size / 4;
    
    if (y_size > 0 && uv_size > 0) {
        // Use some data from input for image content
        memset(img->planes[0], 128, y_size);  // Y plane
        
        // Use input data for UV planes if available
        size_t uv_offset = size / 2;
        for (size_t i = 0; i < uv_size && (uv_offset + i) < size; i++) {
            img->planes[1][i] = data[uv_offset + i] % 256;
            img->planes[2][i] = data[(uv_offset + i + 1) % size] % 256;
        }
    }
    
    // Set output buffer for encoded data
    vpx_fixed_buf_t output_buf;
    output_buf.buf = malloc(4096);
    output_buf.sz = 4096;
    
    if (output_buf.buf) {
        vpx_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);
        
        // Encode the image
        vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
        
        // Free the output buffer
        free(output_buf.buf);
    }
    
    // Cleanup
    vpx_img_free(img);
    free(fb_ctx.buffer);
    vpx_codec_destroy(&decoder);
    vpx_codec_destroy(&encoder);
    
    return 0;
}