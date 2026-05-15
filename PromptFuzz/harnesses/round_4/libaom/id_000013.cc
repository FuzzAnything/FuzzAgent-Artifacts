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
#include <aom/aom.h>
}
extern "C" {
#include <aom/aom_codec.h>
}
extern "C" {
#include <aom/aom_decoder.h>
}
extern "C" {
#include <aom/aom_encoder.h>
}
extern "C" {
#include <aom/aom_ext_ratectrl.h>
}
extern "C" {
#include <aom/aom_external_partition.h>
}
extern "C" {
#include <aom/aom_frame_buffer.h>
}
extern "C" {
#include <aom/aom_image.h>
}
extern "C" {
#include <aom/aomcx.h>
}
extern "C" {
#include <aom/aomdx.h>
}
//<ID> 36
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 2
//<Quality> {"density":10,"unique_branches":{"clamp_pframe_target_size":[[35,23,35,34,1,0,4,0]],"aom_img_metadata_alloc":[[340,7,340,12,0,0,4,0]]},"library_calls":["aom_codec_build_config","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_destroy","aom_codec_decode","aom_codec_get_preview_frame","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_codec_encode","aom_codec_get_preview_frame","aom_img_free","aom_codec_destroy","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_destroy"],"critical_calls":["aom_codec_build_config","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_preview_frame","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_codec_encode","aom_codec_get_preview_frame","aom_img_free","aom_codec_destroy","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<68) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // 1. Get build config string (API 4)
    const char *build_config = aom_codec_build_config();
    (void)build_config; // Prevent unused variable warning
    
    // 2. Create decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 1;
    
    // Initialize decoder
    aom_codec_err_t dec_init_result = aom_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, fuzz_int64_t_1, AOM_DECODER_ABI_VERSION);
    
    if (dec_init_result != AOM_CODEC_OK) {
        // Cleanup and return if decoder init fails
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // 3. Decode the input data (API 5)
    aom_codec_decode(&decoder, data, size, nullptr);
    
    // 4. Get preview frame from decoder (API 2)
    // Note: This is typically for encoders, but we'll call it anyway
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&decoder);
    (void)preview_frame; // Prevent unused variable warning
    
    // 5. Create encoder context
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    // Get default encoder config
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_2) == AOM_CODEC_OK) {
        // Set basic configuration
        enc_cfg.g_w = 64;
        enc_cfg.g_h = 64;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 100;
        enc_cfg.g_threads = 1;
        enc_cfg.g_lag_in_frames = 0;
        
        // 6. Initialize encoder (API 1)
        aom_codec_err_t enc_init_result = aom_codec_enc_init_ver(
            &encoder, encoder_iface, &enc_cfg, fuzz_int64_t_3, AOM_ENCODER_ABI_VERSION);
        
        if (enc_init_result == AOM_CODEC_OK) {
            // Create a test image
            aom_image_t img;
            aom_img_alloc(&img, AOM_IMG_FMT_I420, fuzz_uint32_t_4, fuzz_uint32_t_5, fuzz_uint32_t_6);
            
            // Fill image with some data
            for (int plane = 0; plane < 3; plane++) {
                uint8_t *plane_ptr = img.planes[plane];
                int stride = img.stride[plane];
                int height = aom_img_plane_height(&img, plane);
                int width = aom_img_plane_width(&img, plane);
                
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        plane_ptr[y * stride + x] = (uint8_t)((x + y) % 256);
                    }
                }
            }
            
            // Encode the image
            aom_codec_encode(&encoder, &img, fuzz_int64_t_7, fuzz_uint64_t_8, fuzz_int64_t_9);
            
            // Get preview frame from encoder
            const aom_image_t *encoder_preview = aom_codec_get_preview_frame(&encoder);
            (void)encoder_preview;
            
            // Cleanup image
            aom_img_free(&img);
            
            // Destroy encoder
            aom_codec_destroy(&encoder);
        }
    }
    
    // 7. Create and free metadata (API 3)
    aom_metadata_t *metadata = aom_img_metadata_alloc(fuzz_uint32_t_10, nullptr, fuzz_uint64_t_11, AOM_MIF_ANY_FRAME);
    if (metadata) {
        aom_img_metadata_free(metadata);
    }
    
    // 8. Cleanup decoder
    aom_codec_destroy(&decoder);
    
    return 0;
}