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
//<ID> 95
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 2
//<Quality> {"density":34,"unique_branches":{"decode_tiles_mt":[[2509,12,2509,23,0,0,4,0],[24,23,24,34,1,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy","vpx_img_wrap","vpx_codec_destroy","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_img_wrap","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_set_cx_data_buf","vpx_codec_encode","vpx_codec_get_preview_frame","vpx_codec_destroy","vpx_img_free","vpx_codec_destroy"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified libvpx APIs to decode and re-encode video data:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<92) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_13, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_14, fdp);
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < 10) {
	return 0;
	}
	
    
    vpx_codec_ctx_t decoder_ctx = {0};
    vpx_codec_ctx_t encoder_ctx = {0};
    vpx_codec_stream_info_t stream_info = {0};
    vpx_fixed_buf_t cx_buf = {0};
    vpx_image_t *wrapped_img = nullptr;
    const vpx_image_t *preview_frame = nullptr;
    vpx_codec_iter_t iter = nullptr;
    
    // Try VP9 decoder first
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    if (!decoder_iface) {
	return 0;
	}
	
    
    // Check decoder capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    (void)caps; // Use caps to avoid unused variable warning
    
    // Initialize decoder with default config
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 2;
    
    if (vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
        // Try VP8 decoder if VP9 fails
        decoder_iface = vpx_codec_vp8_dx();
        if (vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, fuzz_int64_t_2, VPX_DECODER_ABI_VERSION) != VPX_CODEC_OK) {
            return 0;
        }
    }
    
    // Try to get stream info from the input data
    stream_info.sz = sizeof(stream_info);
    vpx_codec_err_t stream_info_err = vpx_codec_get_stream_info(&decoder_ctx, &stream_info);
    if (stream_info_err != VPX_CODEC_OK) {
        // If we can't get stream info, try to decode first
        if (vpx_codec_decode(&decoder_ctx, data, size, nullptr, fuzz_int64_t_3) != VPX_CODEC_OK) {
            vpx_codec_destroy(&decoder_ctx);
            return 0;
        }
        
        // Try getting stream info again after decoding
        vpx_codec_get_stream_info(&decoder_ctx, &stream_info);
    }
    
    // Decode the input data
    if (vpx_codec_decode(&decoder_ctx, data, size, nullptr, fuzz_int64_t_4) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    // Get the first decoded frame
    vpx_image_t *decoded_frame = vpx_codec_get_frame(&decoder_ctx, &iter);
    if (!decoded_frame) {
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    // Allocate memory for wrapped image
    size_t img_data_size = decoded_frame->stride[0] * decoded_frame->d_h;
    if (decoded_frame->fmt != VPX_IMG_FMT_I420) {
        // For simplicity, we only handle I420 format in this fuzzer
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    std::vector<unsigned char> img_data(img_data_size * 3 / 2); // YUV420 needs 1.5x Y plane size
    wrapped_img = vpx_img_wrap(nullptr, VPX_IMG_FMT_I420, 
                               decoded_frame->d_w, decoded_frame->d_h, 
                               1, img_data.data());
    
    if (!wrapped_img) {
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    // Copy decoded frame to wrapped image
    for (int plane = 0; plane < 3; plane++) {
        size_t plane_size = decoded_frame->stride[plane] * 
                           (plane == 0 ? decoded_frame->d_h : decoded_frame->d_h / 2);
        if (plane_size > 0 && decoded_frame->planes[plane] && wrapped_img->planes[plane]) {
            memcpy(wrapped_img->planes[plane], decoded_frame->planes[plane], plane_size);
        }
    }
    
    // Set a rectangle on the image (use valid coordinates within image bounds)
    unsigned int rect_x = 0;
    unsigned int rect_y = 0;
    unsigned int rect_w = decoded_frame->d_w > 16 ? 16 : decoded_frame->d_w;
    unsigned int rect_h = decoded_frame->d_h > 16 ? 16 : decoded_frame->d_h;
    
    vpx_img_set_rect(wrapped_img, fuzz_uint32_t_5, fuzz_uint32_t_6, rect_w, rect_h);
    
    // Initialize encoder
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    if (!encoder_iface) {
        encoder_iface = vpx_codec_vp8_cx();
    }
    
    vpx_codec_enc_cfg_t enc_cfg = {0};
    if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_7) != VPX_CODEC_OK) {
        vpx_img_free(wrapped_img);
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    // Set encoder configuration
    enc_cfg.g_w = rect_w;
    enc_cfg.g_h = rect_h;
    enc_cfg.rc_target_bitrate = 256; // Low bitrate for fuzzing
    
    if (vpx_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, fuzz_int64_t_8, VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
        vpx_img_free(wrapped_img);
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    // Allocate buffer for encoded data
    cx_buf.sz = size * 2; // Allocate double the input size
    cx_buf.buf = malloc(cx_buf.sz);
    if (!cx_buf.buf) {
        vpx_codec_destroy(&encoder_ctx);
        vpx_img_free(wrapped_img);
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    // Set the buffer for encoded data
    if (vpx_codec_set_cx_data_buf(&encoder_ctx, &cx_buf, fuzz_uint32_t_9, fuzz_uint32_t_10) != VPX_CODEC_OK) {
        // Continue anyway - some codecs might not support this
    }
    
    // Encode the frame
    vpx_codec_encode(&encoder_ctx, wrapped_img, fuzz_int64_t_11, fuzz_uint64_t_12, fuzz_int64_t_13, fuzz_uint64_t_14);
    
    // Try to get preview frame
    preview_frame = vpx_codec_get_preview_frame(&encoder_ctx);
    (void)preview_frame; // Use preview_frame to avoid unused variable warning
    
    // Cleanup
    free(cx_buf.buf);
    vpx_codec_destroy(&encoder_ctx);
    vpx_img_free(wrapped_img);
    vpx_codec_destroy(&decoder_ctx);
    
    return 0;
}