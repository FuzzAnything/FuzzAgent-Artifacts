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
//<ID> 290
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 0
//<Quality> {"density":24,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_vp9_dx","vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_destroy","vpx_codec_decode","vpx_codec_destroy","vpx_codec_get_stream_info","vpx_codec_destroy","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to decode and re-encode video data:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<76) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_11, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    vpx_codec_ctx_t decoder, encoder;
    vpx_codec_iter_t iter = nullptr;
    vpx_codec_stream_info_t stream_info;
    vpx_codec_enc_cfg_t enc_cfg;
    vpx_codec_iface_t *decoder_iface = nullptr;
    vpx_codec_iface_t *encoder_iface = nullptr;
    
    // Choose codec based on input data
    uint8_t codec_selector = data[0] % 2;
    if (codec_selector == 0) {
        decoder_iface = vpx_codec_vp8_dx();
        encoder_iface = vpx_codec_vp8_cx();
    } else {
        decoder_iface = vpx_codec_vp9_dx();
        encoder_iface = vpx_codec_vp9_cx();
    }
    
    // Get interface name
    const char *iface_name = vpx_codec_iface_name(decoder_iface);
    (void)iface_name; // Use the value to satisfy compiler
    
    // Initialize decoder
    vpx_codec_dec_cfg_t dec_cfg = {0};
    if (vpx_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION) 
        != VPX_CODEC_OK) {
        return 0;
    }
    
    // Get stream info from input data
    stream_info.sz = sizeof(stream_info);
    if (vpx_codec_peek_stream_info(decoder_iface, data, size, &stream_info) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Decode the input data
    if (vpx_codec_decode(&decoder, data, size, nullptr, fuzz_int64_t_2) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Get stream info from decoder context
    if (vpx_codec_get_stream_info(&decoder, &stream_info) != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Get decoded frames
    const vpx_image_t *img = nullptr;
    vpx_image_t *reencoded_img = nullptr;
    bool frame_found = false;
    
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
        frame_found = true;
        
        // Initialize encoder with default config
        if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_3) != VPX_CODEC_OK) {
            break;
        }
        
        // Set encoder configuration based on decoded image
        enc_cfg.g_w = img->d_w;
        enc_cfg.g_h = img->d_h;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.g_threads = 1;
        enc_cfg.rc_target_bitrate = 1000;
        
        // Initialize encoder
        if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_4, 
                                   VPX_ENCODER_ABI_VERSION) != VPX_CODEC_OK) {
            continue;
        }
        
        // Create a copy of the image for re-encoding
        reencoded_img = vpx_img_alloc(nullptr, img->fmt, img->d_w, img->d_h, fuzz_uint32_t_5);
        if (!reencoded_img) {
            vpx_codec_destroy(&encoder);
            continue;
        }
        
        // Copy image data
        for (int plane = 0; plane < 3; ++plane) {
            if (img->planes[plane]) {
                size_t plane_size = (plane == 0) ? 
                    (img->d_w * img->d_h) : 
                    ((img->d_w >> img->x_chroma_shift) * (img->d_h >> img->y_chroma_shift));
                memcpy(reencoded_img->planes[plane], img->planes[plane], plane_size);
            }
        }
        
        // Set encoder configuration
        vpx_codec_enc_config_set(&encoder, &enc_cfg);
        
        // Encode the frame
        vpx_codec_encode(&encoder, reencoded_img, fuzz_int64_t_6, fuzz_uint64_t_7, fuzz_int64_t_8, fuzz_uint64_t_9);
        
        // Clean up reencoded image
        vpx_img_free(reencoded_img);
        
        // Clean up encoder
        vpx_codec_destroy(&encoder);
    }
    
    // Try to initialize encoder even if no frames were decoded
    if (!frame_found) {
        if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_10) == VPX_CODEC_OK) {
            enc_cfg.g_w = 320;
            enc_cfg.g_h = 240;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.g_threads = 1;
            enc_cfg.rc_target_bitrate = 1000;
            
            if (vpx_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, fuzz_int64_t_11, 
                                       VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
                vpx_codec_enc_config_set(&encoder, &enc_cfg);
                vpx_codec_destroy(&encoder);
            }
        }
    }
    
    // Clean up decoder
    vpx_codec_destroy(&decoder);
    
    return 0;
}