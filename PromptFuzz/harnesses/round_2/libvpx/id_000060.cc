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
//<ID> 274
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 5
//<Quality> {"density":11,"unique_branches":{"vpx_codec_get_global_headers":[[335,9,335,20,0,0,4,0]],"vp8cx_pick_filter_level":[[333,17,333,62,0,0,4,0],[365,34,365,57,0,0,4,1]],"validate_config":[[125,54,125,71,1,0,4,1],[125,54,125,71,2,0,4,1]]},"library_calls":["vpx_codec_version_extra_str","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_preview_frame","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_codec_iface_name","vpx_codec_iface_name","vpx_img_alloc","vpx_codec_encode","vpx_codec_set_cx_data_buf","vpx_img_free","vpx_codec_error","vpx_codec_error","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_extra_str","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_preview_frame","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_global_headers","vpx_codec_iface_name","vpx_codec_iface_name","vpx_img_alloc","vpx_codec_encode","vpx_codec_set_cx_data_buf","vpx_img_free","vpx_codec_error","vpx_codec_error","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000060_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // Can be used for processing decoded frames, but just a stub for fuzzing
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // 1. Print version info
    const char* extra_str = vpx_codec_version_extra_str();
    (void)extra_str; // Prevent unused warning

    // 2. Try to peek stream info to determine codec type
    vpx_codec_stream_info_t stream_info;
    stream_info.sz = sizeof(stream_info);
    
    vpx_codec_iface_t* decoder_iface = nullptr;
    vpx_codec_iface_t* encoder_iface = nullptr;
    
    // Try VP9 first
    decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_err_t peek_err = vpx_codec_peek_stream_info(
        decoder_iface, data, (unsigned int)size, &stream_info);
    
    // If VP9 fails, try VP8
    if (peek_err != VPX_CODEC_OK) {
        decoder_iface = vpx_codec_vp8_dx();
        peek_err = vpx_codec_peek_stream_info(
            decoder_iface, data, (unsigned int)size, &stream_info);
    }
    
    if (peek_err != VPX_CODEC_OK) {
        // Can't determine stream type, try with VP8 decoder anyway
        decoder_iface = vpx_codec_vp8_dx();
    }

    // 3. Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t init_err = vpx_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (init_err != VPX_CODEC_OK) {
        // Try alternative initialization
        init_err = vpx_codec_dec_init_ver(
            &decoder, vpx_codec_vp9_dx(), nullptr, 0, VPX_DECODER_ABI_VERSION);
        if (init_err != VPX_CODEC_OK) {
            return 0;
        }
    }

    // 4. Register put_frame callback
    vpx_codec_register_put_frame_cb(&decoder,  id_000060_put_frame_callback, nullptr);

    // 5. Decode the input data
    // Use some of the data to create a deadline value
    long deadline = 0;
    if (size >= sizeof(long)) {
        memcpy(&deadline, data, sizeof(long));
    }
    
    vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, deadline);

    // 6. Get stream info after decoding
    vpx_codec_stream_info_t post_stream_info;
    post_stream_info.sz = sizeof(post_stream_info);
    vpx_codec_get_stream_info(&decoder, &post_stream_info);

    // 7. Try to get preview frame (may not exist for decoder)
    const vpx_image_t* preview_frame = vpx_codec_get_preview_frame(&decoder);
    (void)preview_frame;

    // 8. Initialize encoder for re-encoding
    encoder_iface = vpx_codec_vp8_cx();
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    
    // Get default encoder configuration
    vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    
    // Set some configuration based on decoded stream info
    if (stream_info.w > 0 && stream_info.h > 0) {
        enc_cfg.g_w = stream_info.w;
        enc_cfg.g_h = stream_info.h;
    } else {
        enc_cfg.g_w = 640;
        enc_cfg.g_h = 480;
    }
    
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_error_resilient = 0;
    
    // Initialize encoder
    vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);

    // 9. Get global headers
    vpx_fixed_buf_t* global_headers = vpx_codec_get_global_headers(&encoder);
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }

    // 10. Get interface name
    const char* iface_name = vpx_codec_iface_name(decoder_iface);
    (void)iface_name;
    iface_name = vpx_codec_iface_name(encoder_iface);
    (void)iface_name;

    // 11. Create test image for encoding
    vpx_image_t* test_image = vpx_img_alloc(
        nullptr, VPX_IMG_FMT_I420, 
        enc_cfg.g_w, enc_cfg.g_h, 1);
    
    if (test_image) {
        // Fill image with some data
        size_t y_plane_size = test_image->stride[0] * test_image->d_h;
        size_t uv_plane_size = test_image->stride[1] * (test_image->d_h / 2);
        
        // Use input data to fill image planes
        size_t offset = 0;
        for (int plane = 0; plane < 3; plane++) {
            size_t plane_size = (plane == 0) ? y_plane_size : uv_plane_size;
            if (plane_size > 0 && offset < size) {
                size_t copy_size = (size - offset) < plane_size ? 
                    (size - offset) : plane_size;
                memcpy(test_image->planes[plane], data + offset, copy_size);
                offset += copy_size;
            }
        }

        // 12. Encode the image
        // Use some data to determine pts and duration
        vpx_codec_pts_t pts = 0;
        unsigned long duration = 1;
        if (size >= sizeof(vpx_codec_pts_t)) {
            memcpy(&pts, data, sizeof(vpx_codec_pts_t));
        }
        
        vpx_codec_encode(&encoder, test_image, pts, duration, 
                        0, VPX_DL_REALTIME);
        
        // 13. Prepare output buffer for encoded data
        const size_t output_buffer_size = 1024 * 1024; // 1MB
        std::vector<uint8_t> output_buffer(output_buffer_size);
        vpx_fixed_buf_t output_buf;
        output_buf.buf = output_buffer.data();
        output_buf.sz = output_buffer_size;
        
        // 14. Set the output buffer
        vpx_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);

        // Free the test image
        vpx_img_free(test_image);
    }

    // 15. Get error information (check for any errors)
    const char* error_str = vpx_codec_error(&decoder);
    (void)error_str;
    error_str = vpx_codec_error(&encoder);
    (void)error_str;

    // 16. Clean up
    vpx_codec_destroy(&decoder);
    vpx_codec_destroy(&encoder);

    return 0;
}