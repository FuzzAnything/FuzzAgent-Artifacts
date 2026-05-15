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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_destroy","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_stream_info","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_destroy","vpx_codec_register_put_slice_cb","vpx_codec_register_put_slice_cb","vpx_codec_get_global_headers","vpx_codec_get_global_headers","vpx_img_wrap","vpx_codec_encode","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_destroy","vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_slice_cb","vpx_codec_register_put_slice_cb","vpx_codec_get_global_headers","vpx_codec_get_global_headers","vpx_img_wrap","vpx_codec_encode","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Define the put_slice callback function
static void  id_000016_put_slice_callback(void* user_priv, 
                              const vpx_image_t* img,
                              const vpx_image_rect_t* slice_rect,
                              const vpx_image_rect_t* image_rect) {
    // For fuzzing purposes, we don't need to do anything with the slices
    (void)user_priv;
    (void)img;
    (void)slice_rect;
    (void)image_rect;
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // We need to initialize a decoder first to understand the input stream
    vpx_codec_ctx_t decoder_ctx;
    vpx_codec_iface_t* decoder_iface = nullptr;
    
    // Try to determine if this is VP8 or VP9 data
    // First try VP9
    decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, 
                                                 nullptr, 0, VPX_DECODER_ABI_VERSION);
    
    if (err != VPX_CODEC_OK) {
        // Try VP8 instead
        decoder_iface = vpx_codec_vp8_dx();
        err = vpx_codec_dec_init_ver(&decoder_ctx, decoder_iface, 
                                     nullptr, 0, VPX_DECODER_ABI_VERSION);
        
        if (err != VPX_CODEC_OK) {
            // Can't initialize decoder with either codec
            return 0;
        }
    }
    
    // Now peek at the stream info using the input data
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    err = vpx_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    // Decode the input to get frame information
    err = vpx_codec_decode(&decoder_ctx, data, size, nullptr, 0);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    // Get stream info from decoder context
    err = vpx_codec_get_stream_info(&decoder_ctx, &stream_info);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    // Clean up decoder
    vpx_codec_destroy(&decoder_ctx);
    
    // Now set up encoders based on the stream info we got
    vpx_codec_iface_t* encoder_iface = nullptr;
    if (decoder_iface == vpx_codec_vp9_dx()) {
        encoder_iface = vpx_codec_vp9_cx();
    } else {
        encoder_iface = vpx_codec_vp8_cx();
    }
    
    // Set up encoder configuration
    vpx_codec_enc_cfg_t cfg;
    err = vpx_codec_enc_config_default(encoder_iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }
    
    // Use dimensions from the decoded stream
    cfg.g_w = stream_info.w;
    cfg.g_h = stream_info.h;
    cfg.g_threads = 1;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 0;
    
    // Initialize first encoder using vpx_codec_enc_init_ver
    vpx_codec_ctx_t encoder_ctx1;
    err = vpx_codec_enc_init_ver(&encoder_ctx1, encoder_iface, &cfg, 
                                 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return 0;
    }
    
    // Initialize second encoder using vpx_codec_enc_init_multi_ver
    vpx_codec_ctx_t encoder_ctx2;
    vpx_rational_t dsf = {1, 1};  // No downscaling
    err = vpx_codec_enc_init_multi_ver(&encoder_ctx2, encoder_iface, &cfg, 
                                       1, 0, &dsf, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder_ctx1);
        return 0;
    }
    
    // Register put_slice callback for both encoders
    vpx_codec_register_put_slice_cb(&encoder_ctx1,  id_000016_put_slice_callback, nullptr);
    vpx_codec_register_put_slice_cb(&encoder_ctx2,  id_000016_put_slice_callback, nullptr);
    
    // Get global headers from both encoders
    vpx_fixed_buf_t* global_headers1 = vpx_codec_get_global_headers(&encoder_ctx1);
    vpx_fixed_buf_t* global_headers2 = vpx_codec_get_global_headers(&encoder_ctx2);
    
    // We could use the global headers here if needed, but for fuzzing we just
    // want to exercise the API calls
    
    // Create a test image for encoding
    const unsigned int width = cfg.g_w;
    const unsigned int height = cfg.g_h;
    const unsigned int stride = (width + 31) & ~31;  // 32-byte aligned
    
    std::vector<unsigned char> img_data(stride * height * 3 / 2);  // YUV420
    
    vpx_image_t img;
    vpx_img_wrap(&img, VPX_IMG_FMT_I420, width, height, 1, img_data.data());
    
    // Fill with some test pattern using the fuzzer input
    for (size_t i = 0; i < img_data.size() && i < size; i++) {
        img_data[i] = data[i % size];
    }
    
    // Encode a frame with both encoders
    err = vpx_codec_encode(&encoder_ctx1, &img, 0, 1, 0, VPX_DL_REALTIME);
    if (err != VPX_CODEC_OK) {
        // Continue anyway
    }
    
    err = vpx_codec_encode(&encoder_ctx2, &img, 0, 1, 0, VPX_DL_REALTIME);
    if (err != VPX_CODEC_OK) {
        // Continue anyway
    }
    
    // Clean up
    vpx_codec_destroy(&encoder_ctx2);
    vpx_codec_destroy(&encoder_ctx1);
    
    return 0;
}