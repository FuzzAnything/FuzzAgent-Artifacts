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
//<ID> 53
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_codec_get_preview_frame","vpx_img_free","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/



// Callback function for put_slice
static void  id_000025_put_slice_callback(void *user_priv, const vpx_image_t *img,
                              const vpx_image_rect_t *slice, const vpx_image_rect_t *clip) {
    // Minimal implementation - could log or process slice info
    (void)user_priv;
    (void)img;
    (void)slice;
    (void)clip;
}

// Frame buffer callback functions
static int  id_000025_get_frame_buffer_cb(void *cb_priv, size_t min_size,
                              vpx_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)min_size;
    (void)fb;
    return -1; // Return error - we won't actually use custom frame buffers
}

static int  id_000025_release_frame_buffer_cb(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    (void)cb_priv;
    (void)fb;
    return -1; // Return error
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    vpx_codec_err_t err = vpx_codec_dec_init(&decoder, decoder_iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    // Get stream info from the input data
    vpx_codec_stream_info_t si;
    si.sz = sizeof(si);
    err = vpx_codec_peek_stream_info(decoder_iface, data, static_cast<unsigned int>(size), &si);
    // Note: We still try to decode even if peek fails

    // Set frame buffer functions (though we use minimal callbacks that return errors)
    vpx_codec_set_frame_buffer_functions(&decoder,  id_000025_get_frame_buffer_cb,
                                         id_000025_release_frame_buffer_cb, nullptr);

    // Register put slice callback (typically used for encoding, but we register anyway)
    vpx_codec_register_put_slice_cb(&decoder,  id_000025_put_slice_callback, nullptr);

    // Decode the input data
    err = vpx_codec_decode(&decoder, data, static_cast<unsigned int>(size),
                          nullptr, 0);

    // Try to get stream info after decoding
    vpx_codec_stream_info_t si2;
    si2.sz = sizeof(si2);
    vpx_codec_get_stream_info(&decoder, &si2);

    // Get decoded frames
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Create a copy of the image to manipulate
        vpx_image_t *img_copy = vpx_img_alloc(nullptr, img->fmt, 
                                             img->d_w, img->d_h, 1);
        if (img_copy) {
            // Copy image data
            for (int plane = 0; plane < 3; plane++) {
                if (img->planes[plane]) {
                    int stride = img->stride[plane];
                    int height = (plane > 0) ? (img->d_h + img->y_chroma_shift) >> img->y_chroma_shift : img->d_h;
                    for (int y = 0; y < height; y++) {
                        memcpy(img_copy->planes[plane] + y * img_copy->stride[plane],
                               img->planes[plane] + y * stride,
                               stride);
                    }
                }
            }

            // Set a rectangle on the image (using safe coordinates)
            unsigned int rect_x = 0;
            unsigned int rect_y = 0;
            unsigned int rect_w = img->d_w > 10 ? 10 : img->d_w;
            unsigned int rect_h = img->d_h > 10 ? 10 : img->d_h;
            
            vpx_img_set_rect(img_copy, rect_x, rect_y, rect_w, rect_h);

            // Try to get preview frame (though this is mainly for encoders)
            // We'll try with the decoder context anyway
            const vpx_image_t *preview = vpx_codec_get_preview_frame(&decoder);
            (void)preview; // Use variable to avoid unused warning

            vpx_img_free(img_copy);
        }
    }

    // Create an encoder context to get global headers
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_enc_cfg_t enc_cfg;
    
    if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == VPX_CODEC_OK) {
        // Set minimal dimensions to avoid allocation issues
        enc_cfg.g_w = 16;
        enc_cfg.g_h = 16;
        enc_cfg.g_threads = 1;
        
        if (vpx_codec_enc_init(&encoder, encoder_iface, &enc_cfg, 0) == VPX_CODEC_OK) {
            // Get global headers from encoder
            vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&encoder);
            (void)global_headers; // Use variable to avoid unused warning
            
            vpx_codec_destroy(&encoder);
        }
    }

    // Clean up
    vpx_codec_destroy(&decoder);

    return 0;
}