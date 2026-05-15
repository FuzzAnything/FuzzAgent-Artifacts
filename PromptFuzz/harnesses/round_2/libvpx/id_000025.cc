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
//<ID> 67
//<Prompt> []
/*<Combination>: [
*/
//<score> 100, nr_unique_branch: 26
//<Quality> {"density":25,"unique_branches":{"vp9_enc_alloc_mi":[[886,7,886,15,0,0,4,0]],"alloc_util_frame_buffers":[[1305,7,1311,49,0,0,4,0]],"update_initial_width":[[2368,9,2368,39,0,0,4,0]],"vpx_realloc_frame_buffer":[[162,7,162,33,0,0,4,0],[162,37,162,65,0,0,4,0]],"alloc_compressor_data":[[1386,7,1386,59,0,0,4,0],[440,9,440,16,1,0,4,0]],"set_good_speed_feature_framesize_dependent":[[89,7,89,24,0,0,4,0],[93,7,93,26,0,0,4,1],[95,9,95,26,0,0,4,0],[106,7,106,25,0,0,4,1]],"vp9_set_rc_buffer_sizes":[[24,23,24,34,1,0,4,1],[24,23,24,34,2,0,4,1]],"encoder_encode":[[1483,27,1483,70,0,0,4,0]],"vp9_alloc_context_buffers":[[136,9,136,38,0,0,4,0]],"encoder_init":[[1209,11,1209,28,0,0,4,0]],"vp9_noise_estimate_init":[[35,15,35,44,0,0,4,1],[41,7,41,36,0,0,4,0],[43,14,43,42,0,0,4,0]],"vp9_cyclic_refresh_free":[[56,7,56,17,0,0,4,1]],"vp9_rc_update_framerate":[[25,25,25,26,4,0,4,0],[25,38,25,39,4,0,4,0]],"vp9_free_pc_tree":[[147,7,147,28,0,0,4,1],[154,7,154,26,0,0,4,1]],"vp9_rc_get_default_min_gf_interval":[[356,7,356,28,0,0,4,1],[25,23,25,34,3,0,4,1]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_destroy","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_decode","vpx_codec_get_frame","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":3}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to achieve a complete video processing pipeline:

*/


// Callback function for slice processing
static void  id_000025_put_slice_callback(void* user_priv, const vpx_image_t* img,
                              const vpx_image_rect_t* slice, const vpx_image_rect_t* area) {
    // Simple callback that does nothing for fuzzing
    (void)user_priv;
    (void)img;
    (void)slice;
    (void)area;
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    vpx_codec_ctx_t encoder = {0};
    vpx_codec_ctx_t decoder = {0};
    vpx_codec_iter_t iter = nullptr;
    vpx_codec_stream_info_t si = {0};
    vpx_codec_enc_cfg_t cfg = {0};
    vpx_image_t* img = nullptr;
    vpx_fixed_buf_t* global_headers = nullptr;
    const vpx_codec_cx_pkt_t* pkt = nullptr;
    
    // Choose codec interface based on input data
    vpx_codec_iface_t* codec_iface = nullptr;
    if (data[0] & 1) {
        codec_iface = vpx_codec_vp8_cx();  // VP8 encoder
    } else {
        codec_iface = vpx_codec_vp9_cx();  // VP9 encoder
    }
    
    // API 1: Get codec interface name
    const char* iface_name = vpx_codec_iface_name(codec_iface);
    (void)iface_name;  // Prevent unused variable warning
    
    // API 2: Get codec capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(codec_iface);
    (void)caps;  // Prevent unused variable warning
    
    // Initialize stream info
    si.sz = sizeof(si);
    
    // Try to decode the input as if it's encoded video data
    vpx_codec_iface_t* decoder_iface = nullptr;
    if (codec_iface == vpx_codec_vp8_cx()) {
        decoder_iface = vpx_codec_vp8_dx();
    } else {
        decoder_iface = vpx_codec_vp9_dx();
    }
    
    // Initialize decoder
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, 0, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        // Cleanup and exit if decoder init fails
        if (encoder.priv) {
            vpx_codec_destroy(&encoder);
        }
        if (decoder.priv) {
            vpx_codec_destroy(&decoder);
        }
        return 0;
    }
    
    // API 3: Get stream info from encoded data
    vpx_codec_err_t stream_info_result = vpx_codec_peek_stream_info(decoder_iface, data, size, &si);
    (void)stream_info_result;  // Result may be invalid for fuzzing input
    
    // Decode the input data
    if (vpx_codec_decode(&decoder, data, size, nullptr, 0) == VPX_CODEC_OK) {
        // API 4: Get decoded frame
        vpx_image_t* decoded_frame = vpx_codec_get_frame(&decoder, &iter);
        (void)decoded_frame;  // Frame may be null for invalid data
    }
    
    // Reset iterator for encoder phase
    iter = nullptr;
    
    // Now setup encoder configuration
    unsigned int width = 320;
    unsigned int height = 240;
    if (si.w > 0 && si.h > 0) {
        width = si.w;
        height = si.h;
    }
    
    // Get default encoder configuration
    if (vpx_codec_enc_config_default(codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256;
    
    // API 5: Initialize encoder
    if (vpx_codec_enc_init_ver(&encoder, codec_iface, &cfg, 0, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // API 6: Register slice callback
    vpx_codec_register_put_slice_cb(&encoder,  id_000025_put_slice_callback, nullptr);
    
    // Create a test image for encoding
    unsigned int align = 32;
    img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, width, height, align);
    if (!img) {
        vpx_codec_destroy(&encoder);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Fill image with some data (using fuzzer input)
    for (int plane = 0; plane < 3; ++plane) {
        unsigned char* plane_ptr = img->planes[plane];
        int stride = img->stride[plane];
        int plane_height = (plane == 0) ? height : height / 2;
        int plane_width = (plane == 0) ? width : width / 2;
        
        for (int y = 0; y < plane_height; ++y) {
            for (int x = 0; x < plane_width; ++x) {
                if (size > 0) {
                    plane_ptr[y * stride + x] = data[(y * plane_width + x) % size];
                } else {
                    plane_ptr[y * stride + x] = 128;
                }
            }
        }
    }
    
    // Encode the frame
    vpx_codec_encode(&encoder, img, 0, 1, 0, VPX_DL_REALTIME);
    
    // Get compressed data
    while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
        // Process the packet if needed
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Try to decode what we just encoded
            if (decoder_iface) {
                vpx_codec_decode(&decoder, 
                                reinterpret_cast<const uint8_t*>(pkt->data.frame.buf),
                                pkt->data.frame.sz, nullptr, 0);
                
                // Try to get the frame again
                vpx_image_t* re_decoded = vpx_codec_get_frame(&decoder, &iter);
                (void)re_decoded;
            }
        }
    }
    
    // Cleanup
    if (img) {
        vpx_img_free(img);
    }
    
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }
    
    if (encoder.priv) {
        vpx_codec_destroy(&encoder);
    }
    
    if (decoder.priv) {
        vpx_codec_destroy(&decoder);
    }
    
    return 0;
}