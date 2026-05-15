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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"aom_codec_dec_init_ver":[[31,7,31,37,0,0,4,0]]},"library_calls":["aom_codec_build_config","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_metadata_alloc","aom_codec_decode","aom_codec_get_preview_frame","aom_img_metadata_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_build_config","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_metadata_alloc","aom_codec_decode","aom_codec_get_preview_frame","aom_img_metadata_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Use aom_codec_build_config() - just call it for coverage
    const char *build_config = aom_codec_build_config();
    (void)build_config; // Prevent unused variable warning

    // Create a decoder context
    aom_codec_ctx_t decoder_ctx;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // Initialize decoder with default configuration
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    aom_codec_err_t dec_init_res = aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, fuzz_int64_t_1, fuzz_int32_t_2);
    
    // Create an encoder context (even though we're primarily decoding)
    aom_codec_ctx_t encoder_ctx;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    // Initialize encoder with default configuration
    aom_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_3);
    enc_cfg.g_w = 64;
    enc_cfg.g_h = 64;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_threads = 1;
    
    // Use aom_codec_enc_init_ver
    aom_codec_err_t enc_init_res = aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, fuzz_int64_t_4, fuzz_int32_t_5);
    
    // Create metadata for testing aom_img_metadata_free
    aom_metadata_t *metadata = nullptr;
    if (size > 4) {
        uint32_t metadata_type = *reinterpret_cast<const uint32_t*>(data) % 256;
        size_t metadata_size = size > 100 ? 50 : size / 2;
        metadata = aom_img_metadata_alloc(metadata_type, data, metadata_size, AOM_MIF_ANY_FRAME);
    }
    
    // Try to decode the input data
    if (dec_init_res == AOM_CODEC_OK) {
        aom_codec_decode(&decoder_ctx, data, size, nullptr);
        
        // Try to get preview frame
        const aom_image_t *preview_frame = aom_codec_get_preview_frame(&decoder_ctx);
        (void)preview_frame; // Use result to prevent unused variable warning
    }
    
    // Cleanup
    if (metadata) {
        aom_img_metadata_free(metadata);
    }
    
    if (enc_init_res == AOM_CODEC_OK) {
        aom_codec_destroy(&encoder_ctx);
    }
    
    if (dec_init_res == AOM_CODEC_OK) {
        aom_codec_destroy(&decoder_ctx);
    }
    
    return 0;
}