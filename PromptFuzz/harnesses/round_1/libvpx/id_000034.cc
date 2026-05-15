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
//<ID> 88
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_destroy","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_alloc","vpx_codec_destroy","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_destroy","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Use input data as compressed video stream for decoding
    // We'll decode it and then re-encode it (if we have enough data)
    
    if (size < 1) {
        return 0;
    }

    // First, try to decode the input as VP8/VP9 stream
    vpx_codec_ctx_t decoder = {0};
    vpx_codec_ctx_t encoder = {0};
    vpx_image_t *raw_image = nullptr;
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    
    // Initialize decoder with VP9
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, vpx_codec_vp9_dx(), 
                                                  nullptr, 0, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        // Try VP8 if VP9 fails
        vpx_codec_destroy(&decoder);
        err = vpx_codec_dec_init_ver(&decoder, vpx_codec_vp8_dx(), 
                                      nullptr, 0, VPX_DECODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            return 0;
        }
    }

    // Decode the input data
    err = vpx_codec_decode(&decoder, data, static_cast<unsigned int>(size), 
                           nullptr, 0);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }

    // Try to get decoded frame
    vpx_image_t *decoded_frame = vpx_codec_get_frame(&decoder, &iter);
    if (!decoded_frame) {
        vpx_codec_destroy(&decoder);
        return 0;
    }

    // Now try to encode the decoded frame
    // Allocate raw image buffer for encoding
    raw_image = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                              decoded_frame->d_w, decoded_frame->d_h, 16);
    if (!raw_image) {
        vpx_codec_destroy(&decoder);
        return 0;
    }

    // Copy decoded frame data to raw image (simplified - assume same format)
    for (int plane = 0; plane < 3; ++plane) {
        unsigned char *src = decoded_frame->planes[plane];
        unsigned char *dst = raw_image->planes[plane];
        int src_stride = decoded_frame->stride[plane];
        int dst_stride = raw_image->stride[plane];
        int height = plane ? (decoded_frame->d_h + 1) >> 1 : decoded_frame->d_h;
        
        for (int y = 0; y < height; ++y) {
            memcpy(dst, src, std::min(src_stride, dst_stride));
            src += src_stride;
            dst += dst_stride;
        }
    }

    // Set up encoder configuration
    vpx_codec_enc_cfg_t cfg = {0};
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    err = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(raw_image);
        vpx_codec_destroy(&decoder);
        return 0;
    }

    cfg.g_w = decoded_frame->d_w;
    cfg.g_h = decoded_frame->d_h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_pass = VPX_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    cfg.rc_end_usage = VPX_VBR;

    // Initialize multi-encoder (even though we're using 1 encoder)
    vpx_rational_t dsf = {1, 1};
    err = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &cfg, 
                                        1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        // Try VP8 if VP9 fails
        vpx_codec_destroy(&encoder);
        encoder_iface = vpx_codec_vp8_cx();
        err = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
        if (err == VPX_CODEC_OK) {
            cfg.g_w = decoded_frame->d_w;
            cfg.g_h = decoded_frame->d_h;
            err = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &cfg, 
                                                1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
        }
    }

    if (err == VPX_CODEC_OK) {
        // Encode the frame
        err = vpx_codec_encode(&encoder, raw_image, 0, 1, 0, VPX_DL_REALTIME);
        if (err == VPX_CODEC_OK) {
            // Get encoded data
            iter = nullptr;
            while ((pkt = vpx_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    // We got encoded packet - this is just for fuzzing,
                    // so we don't need to do anything with it
                    break;
                }
            }
        }
    }

    // Cleanup
    if (encoder.err == VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
    }
    vpx_img_free(raw_image);
    vpx_codec_destroy(&decoder);

    return 0;
}