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
//<ID> 291
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_error_detail","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_destroy","vpx_img_wrap","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_set_frame_buffer_functions","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_error_detail","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_img_wrap","vpx_img_set_rect","vpx_codec_set_frame_buffer_functions","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_get_preview_frame","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to achieve encoding/decoding operations:

*/


extern "C" {
}

// Frame buffer callbacks for decoder
static int  id_000072_get_frame_buffer(void *user_priv, size_t min_size, 
                            vpx_codec_frame_buffer_t *fb) {
    fb->data = (uint8_t *)malloc(min_size);
    fb->size = min_size;
    return 0;
}

static int  id_000072_release_frame_buffer(void *user_priv, 
                                vpx_codec_frame_buffer_t *fb) {
    free(fb->data);
    return 0;
}

// Callback for encoded data
static void  id_000072_handle_encoded_data(const uint8_t *data, size_t size) {
    // Can be used for additional processing
}

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_image_t img;
    vpx_codec_stream_info_t si;
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    vpx_fixed_buf_t cx_buf;
    uint8_t *img_data = nullptr;
    int width = 320, height = 240;
    int ret = 0;
    
    // Initialize stream info
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    // Get VP8 encoder interface
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp8_cx();
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp8_dx();
    
    if (!encoder_iface || !decoder_iface) {
	return 0;
	}
	
    
    // Get default encoder configuration
    if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Set encoder configuration
    enc_cfg.g_w = width;
    enc_cfg.g_h = height;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 256;
    enc_cfg.g_error_resilient = 1;
    
    // Initialize encoder
    if (vpx_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, 
                               0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&encoder_ctx);
        (void)error_detail; // Use error detail
        return 0;
    }
    
    // Also try multi-encoder init (though we won't use it fully)
    vpx_codec_ctx_t multi_ctx;
    vpx_rational_t dsf = {1, 1};
    if (vpx_codec_enc_init_multi_ver(&multi_ctx, encoder_iface, &enc_cfg, 
                                      1, 0, &dsf, VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
        vpx_codec_destroy(&multi_ctx);
    }
    
    // Allocate image data
    size_t img_size = width * height * 3 / 2; // I420 format
    img_data = (uint8_t *)malloc(img_size);
    if (!img_data) {
        vpx_codec_destroy(&encoder_ctx);
        return 0;
    }
    
    // Wrap input data as image
    vpx_image_t *wrapped_img = vpx_img_wrap(&img, VPX_IMG_FMT_I420, 
                                            width, height, 1, img_data);
    if (!wrapped_img) {
        free(img_data);
        vpx_codec_destroy(&encoder_ctx);
        return 0;
    }
    
    // Fill image with pattern from input data
    size_t copy_size = (size < img_size) ? size : img_size;
    memcpy(img_data, data, copy_size);
    
    // Set rectangle on image
    unsigned int rect_x = (data[0] % (width - 32));
    unsigned int rect_y = (data[1] % (height - 32));
    unsigned int rect_w = 32;
    unsigned int rect_h = 32;
    vpx_img_set_rect(&img, rect_x, rect_y, rect_w, rect_h);
    
    // Set frame buffer functions for encoder
    vpx_codec_set_frame_buffer_functions(&encoder_ctx, 
                                          id_000072_get_frame_buffer, 
                                          id_000072_release_frame_buffer, 
                                         nullptr);
    
    // Set up output buffer
    std::vector<uint8_t> output_buffer(size * 2);
    cx_buf.buf = output_buffer.data();
    cx_buf.sz = output_buffer.size();
    
    // Set encoded data buffer
    vpx_codec_set_cx_data_buf(&encoder_ctx, &cx_buf, 0, 0);
    
    // Encode the image
    if (vpx_codec_encode(&encoder_ctx, &img, 0, 1, 0, VPX_DL_REALTIME) != VPX_CODEC_OK) {
        const char *error_detail = vpx_codec_error_detail(&encoder_ctx);
        (void)error_detail; // Use error detail
    } else {
        // Get encoded data
        while ((pkt = vpx_codec_get_cx_data(&encoder_ctx, &iter)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                 id_000072_handle_encoded_data((const uint8_t *)pkt->data.frame.buf, 
                                    pkt->data.frame.sz);
            }
        }
        
        // Try to get preview frame
        const vpx_image_t *preview = vpx_codec_get_preview_frame(&encoder_ctx);
        (void)preview;
    }
    
    // Initialize decoder
    if (vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, NULL, 
                               0, VPX_DECODER_ABI_VERSION) == VPX_CODEC_OK) {
        
        // Get stream info from encoded data if available
        if (output_buffer.size() > 0 && output_buffer[0] != 0) {
            // Use part of the output as input to decoder
            size_t decode_size = (size < 1000) ? size : 1000;
            if (decode_size > output_buffer.size()) {
                decode_size = output_buffer.size();
            }
            
            vpx_codec_decode(&decoder_ctx, output_buffer.data(), 
                            decode_size, NULL, 0);
            
            // Get stream info
            vpx_codec_get_stream_info(&decoder_ctx, &si);
            
            // Get decoded frames
            vpx_codec_iter_t dec_iter = NULL;
            while (vpx_codec_get_frame(&decoder_ctx, &dec_iter)) {
                // Process decoded frames
            }
        }
        
        vpx_codec_destroy(&decoder_ctx);
    }
    
    // Cleanup
    vpx_img_free(&img);
    free(img_data);
    vpx_codec_destroy(&encoder_ctx);
    
    return 0;
}