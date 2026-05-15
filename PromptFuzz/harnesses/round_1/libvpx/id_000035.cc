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
//<ID> 91
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_img_alloc","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_peek_stream_info","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// Include libvpx headers

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Use fmemopen to create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Try to determine if this is VP8 or VP9 data by peeking stream info
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // Try VP9 first
    vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
        vpx_codec_vp9_dx(), data, size, &stream_info);
    
    vpx_codec_iface_t *decoder_iface = nullptr;
    vpx_codec_iface_t *encoder_iface = nullptr;
    
    if (peek_result == VPX_CODEC_OK) {
        decoder_iface = vpx_codec_vp9_dx();
        encoder_iface = vpx_codec_vp9_cx();
    } else {
        // Try VP8
        peek_result = vpx_codec_peek_stream_info(
            vpx_codec_vp8_dx(), data, size, &stream_info);
        if (peek_result == VPX_CODEC_OK) {
            decoder_iface = vpx_codec_vp8_dx();
            encoder_iface = vpx_codec_vp8_cx();
        }
    }
    
    if (!decoder_iface || peek_result != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_err_t dec_init_result = vpx_codec_dec_init_ver(
        &decoder, decoder_iface, nullptr, 0, VPX_DECODER_ABI_VERSION);
    
    if (dec_init_result != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Decode the input data
    vpx_codec_err_t decode_result = vpx_codec_decode(
        &decoder, data, size, nullptr, 0);
    
    if (decode_result != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Get decoded frame
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t *decoded_image = vpx_codec_get_frame(&decoder, &iter);
    
    if (!decoded_image) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize encoder with the same dimensions as decoded frame
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    
    // Get default encoder configuration
    vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    
    // Set encoder configuration based on decoded image
    enc_cfg.g_w = decoded_image->d_w;
    enc_cfg.g_h = decoded_image->d_h;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_pass = VPX_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    
    // Initialize encoder (multi-ver with 1 encoder instance)
    vpx_rational_t dsf = {1, 1};
    vpx_codec_err_t enc_init_result = vpx_codec_enc_init_multi_ver(
        &encoder, encoder_iface, &enc_cfg, 1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
    
    if (enc_init_result != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Allocate image for encoding using vpx_img_alloc
    vpx_image_t *allocated_image = vpx_img_alloc(
        nullptr, decoded_image->fmt, decoded_image->d_w, decoded_image->d_h, 16);
    
    if (!allocated_image) {
        vpx_codec_destroy(&encoder);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Copy decoded image to allocated image
    for (int plane = 0; plane < 3; ++plane) {
        const uint8_t *src = decoded_image->planes[plane];
        uint8_t *dst = allocated_image->planes[plane];
        const int stride = decoded_image->stride[plane];
        const int dst_stride = allocated_image->stride[plane];
        const int height = plane ? (decoded_image->d_h + 1) >> 1 : decoded_image->d_h;
        
        for (int y = 0; y < height; ++y) {
            memcpy(dst, src, stride);
            src += stride;
            dst += dst_stride;
        }
    }

    // Encode the frame
    vpx_codec_err_t encode_result = vpx_codec_encode(
        &encoder, allocated_image, 0, 1, 0, VPX_DL_REALTIME);
    
    if (encode_result != VPX_CODEC_OK) {
        vpx_img_free(allocated_image);
        vpx_codec_destroy(&encoder);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Get encoded data
    vpx_codec_iter_t enc_iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    
    while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Encoded frame available, we could write it to a file
            // but for fuzzing we just process it
            break;
        }
    }

    // Clean up
    vpx_img_free(allocated_image);
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}