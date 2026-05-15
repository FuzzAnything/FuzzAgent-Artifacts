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
//<ID> 120
//<Prompt> []
/*<Combination>: [
*/
//<score> 40.5, nr_unique_branch: 2
//<Quality> {"density":27,"unique_branches":{"vp9_left_block_mode":[[16,21,16,44,0,0,4,0]],"decode_tiles_mt":[[2509,12,2509,23,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_alloc","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_get_cx_data","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_enc_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to decode VP9 video from input data, then re-encode it:

*/


// Include libvpx headers

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful processing

    // Part 1: Decode VP9 stream from input data
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    
    // Check decoder capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    (void)caps; // Use caps to satisfy requirement
    
    // Peek stream info to get dimensions
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    if (vpx_codec_peek_stream_info(decoder_iface, data, size, &stream_info) != VPX_CODEC_OK) {
        return 0; // Invalid VP9 stream
    }
    
    // Initialize decoder
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 2;
    dec_cfg.w = stream_info.w;
    dec_cfg.h = stream_info.h;
    
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Decode the frame
    if (vpx_codec_decode(&decoder, data, size, nullptr, 0) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Get decoded frame
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t *decoded_img = vpx_codec_get_frame(&decoder, &iter);
    
    if (!decoded_img) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Create a copy of the decoded image for manipulation
    vpx_image_t *img_copy = vpx_img_alloc(nullptr, decoded_img->fmt, 
                                         decoded_img->d_w, decoded_img->d_h, 16);
    if (!img_copy) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Copy image data
    for (int plane = 0; plane < 3; ++plane) {
        uint8_t *src = decoded_img->planes[plane];
        uint8_t *dst = img_copy->planes[plane];
        int stride = std::min(decoded_img->stride[plane], img_copy->stride[plane]);
        int height = (plane == 0) ? decoded_img->d_h : (decoded_img->d_h >> decoded_img->y_chroma_shift);
        
        for (int y = 0; y < height; ++y) {
            memcpy(dst, src, stride);
            src += decoded_img->stride[plane];
            dst += img_copy->stride[plane];
        }
    }
    
    // Set a rectangle on the image (crop to smaller region)
    if (img_copy->d_w > 16 && img_copy->d_h > 16) {
        unsigned int crop_w = std::min(img_copy->d_w / 2, 64u);
        unsigned int crop_h = std::min(img_copy->d_h / 2, 64u);
        vpx_img_set_rect(img_copy, 0, 0, crop_w, crop_h);
    }
    
    // Part 2: Re-encode the decoded frame
    vpx_codec_ctx_t encoder;
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    
    // Get default encoder configuration
    vpx_codec_enc_cfg_t enc_cfg;
    if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0) != VPX_CODEC_OK) {
        vpx_img_free(img_copy);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Configure encoder
    enc_cfg.g_w = img_copy->d_w;
    enc_cfg.g_h = img_copy->d_h;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_pass = VPX_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    enc_cfg.g_error_resilient = 0;
    
    // Initialize encoder (try multi_ver first, then fall back to single)
    vpx_rational_t dsf = {1, 1};
    vpx_codec_err_t enc_init_result = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, 
                                                                  &enc_cfg, 1, 0, &dsf, 
                                                                  VPX_ENCODER_ABI_VERSION);
    
    if (enc_init_result != VPX_CODEC_OK) {
        // Fall back to single encoder initialization
        enc_init_result = vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0,
                                                VPX_ENCODER_ABI_VERSION);
    }
    
    if (enc_init_result != VPX_CODEC_OK) {
        vpx_img_free(img_copy);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Register frame callback (dummy callback)
    auto frame_cb = [](void *user_priv, const vpx_image_t *img) {
        (void)user_priv;
        (void)img;
    };
    vpx_codec_register_put_frame_cb(&encoder, frame_cb, nullptr);
    
    // Encode the frame
    if (vpx_codec_encode(&encoder, img_copy, 0, 1, 0, VPX_DL_REALTIME) != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
        vpx_img_free(img_copy);
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Get encoded data
    vpx_codec_iter_t enc_iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt;
    std::vector<uint8_t> encoded_data;
    
    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            encoded_data.insert(encoded_data.end(),
                              (uint8_t*)pkt->data.frame.buf,
                              (uint8_t*)pkt->data.frame.buf + pkt->data.frame.sz);
        }
    }
    
    // Part 3: Decode the re-encoded data
    if (!encoded_data.empty()) {
        vpx_codec_ctx_t redecoder;
        if (vpx_codec_dec_init_ver(&redecoder, decoder_iface, &dec_cfg, 0, 
                                   VPX_DECODER_ABI_VERSION) == VPX_CODEC_OK) {
            vpx_codec_decode(&redecoder, encoded_data.data(), encoded_data.size(), 
                           nullptr, 0);
            
            vpx_codec_destroy(&redecoder);
        }
    }
    
    // Cleanup
    vpx_codec_destroy(&encoder);
    vpx_img_free(img_copy);
    vpx_codec_destroy(&decoder);
    
    return 0;
}