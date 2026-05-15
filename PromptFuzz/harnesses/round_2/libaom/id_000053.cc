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
//<ID> 173
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 3
//<Quality> {"density":14,"unique_branches":{"estimate_screen_content":[[2098,35,2098,68,0,0,4,0]],"av1_change_config":[[34,23,34,34,10,0,4,1],[34,23,34,34,11,0,4,1]]},"library_calls":["aom_codec_av1_cx","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_error","aom_codec_set_option","aom_codec_destroy","aom_img_wrap","aom_codec_destroy","aom_img_remove_metadata","aom_codec_encode","aom_uleb_encode","aom_codec_enc_config_set","aom_codec_get_preview_frame","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_wrap","aom_img_remove_metadata","aom_codec_encode","aom_uleb_encode","aom_codec_enc_config_set","aom_codec_get_preview_frame","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure minimum input size for basic operations
    if (size < 64) {
	return 0;
	}
	

    // Step 1: Get AV1 encoder interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
	return 0;
	}
	

    // Step 2: Get codec capabilities
    aom_codec_caps_t caps = aom_codec_get_caps(iface);
    (void)caps; // Use caps to avoid unused variable warning

    // Step 3: Create encoder configuration
    aom_codec_enc_cfg_t cfg;
    unsigned int usage = 0; // Good quality mode
    aom_codec_err_t err = aom_codec_enc_config_default(iface, &cfg, usage);
    if (err != AOM_CODEC_OK) {
	return 0;
	}
	

    // Step 4: Initialize encoder context
    aom_codec_ctx_t ctx;
    cfg.g_w = 64; // Set reasonable dimensions for fuzzing
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;

    err = aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        // Get error message
        const char *error_str = aom_codec_error(&ctx);
        (void)error_str; // Use error_str to avoid unused variable warning
        return 0;
    }

    // Step 5: Set encoder options from input data
    char opt_name[32] = "cpu-used";
    char opt_value[32];
    snprintf(opt_value, sizeof(opt_value), "%u", data[0] % 10); // Use first byte for cpu-used
    err = aom_codec_set_option(&ctx, opt_name, opt_value);
    (void)err; // We don't fail on option errors

    // Step 6: Create test image
    aom_image_t img;
    const unsigned int width = 64;
    const unsigned int height = 64;
    const unsigned int align = 32;
    
    // Allocate image buffer (I420 format requires width*height*1.5 bytes)
    size_t img_size = width * height * 3 / 2;
    unsigned char *img_data = (unsigned char *)malloc(img_size);
    if (!img_data) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fill image data with fuzzer input (cyclic)
    for (size_t i = 0; i < img_size; i++) {
        img_data[i] = data[i % size];
    }

    aom_image_t *wrapped_img = aom_img_wrap(&img, AOM_IMG_FMT_I420, width, height, align, img_data);
    if (!wrapped_img) {
        free(img_data);
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Step 7: Remove metadata from image
    aom_img_remove_metadata(wrapped_img);

    // Step 8: Encode the image
    aom_codec_encode(&ctx, wrapped_img, 0, 1, 0);

    // Step 9: Test ULEB encoding with data from input
    uint64_t uleb_value = 0;
    for (size_t i = 0; i < 8 && i < size; i++) {
        uleb_value |= (uint64_t)data[i] << (i * 8);
    }
    uint8_t coded_buffer[16];
    size_t coded_size = 0;
    int uleb_result = aom_uleb_encode(uleb_value, sizeof(coded_buffer), coded_buffer, &coded_size);
    (void)uleb_result; // Result checked for demonstration

    // Step 10: Reconfigure encoder (testing reconfiguration)
    cfg.rc_target_bitrate = 500;
    err = aom_codec_enc_config_set(&ctx, &cfg);
    (void)err; // Don't fail on reconfig errors

    // Step 11: Clean up resources
    // Free image data (aom_img_wrap doesn't own the buffer, so we must free it)
    free(img_data);
    
    // Get preview frame (if any)
    const aom_image_t *preview = aom_codec_get_preview_frame(&ctx);
    (void)preview; // Use preview to avoid unused variable warning

    // Step 12: Destroy encoder
    aom_codec_destroy(&ctx);

    // Step 13: Free image structure (aom_img_wrap doesn't allocate the struct, just fills it)
    // No need to call aom_img_free as we used stack-allocated struct
    
    return 0;
}