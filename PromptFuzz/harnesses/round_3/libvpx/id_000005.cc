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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["vpx_codec_version_extra_str","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_version_extra_str","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_get_stream_info","vpx_codec_decode","vpx_codec_get_frame","vpx_img_set_rect","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_encode","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/**/


// Include libvpx headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<76) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
	//fuzzer shim end}




    // Early exit if data is too small for basic operations
    if (size < 1) {
        return 0;
    }

    // 1. Get version extra string (API 1)
    const char *version_extra = vpx_codec_version_extra_str();
    (void)version_extra; // Use variable to avoid unused warning

    // 2. Create decoder context
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    
    // Initialize decoder with default configuration
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 
                                                  fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        // Try VP8 if VP9 fails
        decoder_iface = vpx_codec_vp8_dx();
        err = vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 
                                      fuzz_int64_t_2, VPX_DECODER_ABI_VERSION);
        if (err != VPX_CODEC_OK) {
            return 0;
        }
    }

    // 3. Get stream information (API 2)
    vpx_codec_stream_info_t stream_info;
    stream_info.sz = sizeof(stream_info);
    
    // Use peek_stream_info first to avoid modifying decoder state
    err = vpx_codec_peek_stream_info(decoder_iface, data, 
                                      (unsigned int)(size > 1000 ? 1000 : size), 
                                      &stream_info);
    
    // Then get stream info from decoder context
    if (err == VPX_CODEC_OK) {
        err = vpx_codec_get_stream_info(&decoder, &stream_info);
    }

    // 4. Decode the input data
    err = vpx_codec_decode(&decoder, data, (unsigned int)size, NULL, fuzz_int64_t_3);
    
    // 5. Get decoded frames (API 5)
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img = NULL;
    
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != NULL) {
        // 6. Set rectangle on image (API 4)
        // Use safe rectangle dimensions based on image size
        unsigned int x = 0;
        unsigned int y = 0;
        unsigned int w = img->d_w > 0 ? img->d_w / 2 : 1;
        unsigned int h = img->d_h > 0 ? img->d_h / 2 : 1;
        
        // Ensure rectangle is within bounds
        if (w > 0 && h > 0 && x + w <= img->d_w && y + h <= img->d_h) {
            vpx_img_set_rect(img, fuzz_uint32_t_4, fuzz_uint32_t_5, w, h);
        }
        
        // 7. Create encoder context for multi-encoder setup (API 3)
        vpx_codec_ctx_t encoder;
        vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
        
        // Create encoder configuration
        vpx_codec_enc_cfg_t enc_cfg;
        vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_6);
        
        // Set dimensions from stream info or image
        enc_cfg.g_w = stream_info.w > 0 ? stream_info.w : img->d_w;
        enc_cfg.g_h = stream_info.h > 0 ? stream_info.h : img->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000;
        
        // Create downsampling factors
        vpx_rational_t dsf[2] = {{1, 1}, {2, 1}};
        
        // Initialize multi-encoder (2 encoders)
        err = vpx_codec_enc_init_multi_ver(&encoder, encoder_iface, &enc_cfg, 
                                           2, fuzz_int64_t_7, dsf, VPX_ENCODER_ABI_VERSION);
        
        if (err == VPX_CODEC_OK) {
            // Encode the image (simplified - just to use the encoder)
            vpx_codec_encode(&encoder, img, fuzz_int64_t_8, fuzz_uint64_t_9, fuzz_int64_t_10, fuzz_uint64_t_11);
            
            // Clean up encoder
            vpx_codec_destroy(&encoder);
        }
        
        // Break after first frame to avoid infinite loops with corrupted data
        break;
    }

    // 8. Clean up decoder
    vpx_codec_destroy(&decoder);

    return 0;
}