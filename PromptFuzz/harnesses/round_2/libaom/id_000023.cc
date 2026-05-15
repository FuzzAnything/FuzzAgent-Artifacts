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
//<ID> 48
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 5
//<Quality> {"density":9,"unique_branches":{"aom_codec_enc_init_ver":[[87,9,87,12,0,0,4,0],[92,25,92,34,0,0,4,0]],"encoder_destroy":[[3173,7,3173,15,0,0,4,1]],"validate_config":[[667,11,667,28,39,0,4,1]],"encoder_init":[[3046,9,3046,28,0,0,4,1]]},"library_calls":["aom_uleb_decode","aom_img_wrap","aom_img_metadata_alloc","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_enc_init_ver","aom_img_metadata_free","aom_codec_set_option","aom_codec_encode","aom_img_remove_metadata","aom_codec_get_cx_data","aom_codec_destroy","aom_img_metadata_free"],"critical_calls":["aom_uleb_decode","aom_img_wrap","aom_img_metadata_alloc","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_encode","aom_img_remove_metadata","aom_codec_get_cx_data","aom_codec_destroy","aom_img_metadata_free"],"visited":0}
/*Looking at the provided APIs, I can create a fuzz driver that simulates encoding an image with metadata using the AV1 encoder. The event could be: "Process an image with metadata, encode it using AV1 encoder, and handle ULEB-encoded values from the input."

Here's a step-by-step implementation:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful fuzzing

    // 1. Decode a ULEB value from the beginning of input
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    if (aom_uleb_decode(data, size, &uleb_value, &uleb_length) != 0) {
        // If ULEB decode fails, try with a fixed size
        uleb_value = 0;
        uleb_length = 1;
    }

    // Skip the ULEB part for image data
    const uint8_t* image_data = data + uleb_length;
    size_t image_data_size = size - uleb_length;
    
    if (image_data_size < 64 * 64 * 3 / 2) {
	return 0;
	}
	  // Need enough data for a small I420 image

    // 2. Create an image using aom_img_wrap
    // Use a small fixed size for fuzzing
    const unsigned int width = 64;
    const unsigned int height = 64;
    
    // Allocate buffer for image data
    unsigned char* img_buffer = (unsigned char*)malloc(image_data_size);
    if (!img_buffer) {
	return 0;
	}
	
    memcpy(img_buffer, image_data, image_data_size);
    
    aom_image_t img;
    aom_image_t* wrapped_img = aom_img_wrap(&img, AOM_IMG_FMT_I420, 
                                          width, height, 
                                          1,  // stride_align
                                          img_buffer);
    if (!wrapped_img) {
        free(img_buffer);
        return 0;
    }

    // 3. Allocate metadata for the image
    const uint32_t metadata_type = 1;  // Example metadata type
    const uint8_t metadata_content[] = {0x01, 0x02, 0x03, 0x04};
    aom_metadata_t* metadata = aom_img_metadata_alloc(
        metadata_type,
        metadata_content,
        sizeof(metadata_content),
        AOM_MIF_ANY_FRAME
    );
    
    // Note: We can't actually attach this metadata to the image with provided APIs,
    // but we can still allocate and free it as required

    // 4. Initialize encoder
    aom_codec_iface_t* codec_iface = aom_codec_av1_cx();
    
    // Get codec interface name (for logging/demo)
    const char* iface_name = aom_codec_iface_name(codec_iface);
    (void)iface_name;  // Use iface_name to avoid unused variable warning
    
    aom_codec_ctx_t codec_ctx;
    aom_codec_enc_cfg_t cfg;
    
    // Set minimal encoder configuration for fuzzing
    cfg.g_usage = 0;  // Good quality
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = AOM_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_error_resilient = 0;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    cfg.rc_end_usage = AOM_VBR;
    cfg.rc_target_bitrate = 256;
    cfg.rc_min_quantizer = 2;
    cfg.rc_max_quantizer = 56;
    cfg.rc_undershoot_pct = 100;
    cfg.rc_overshoot_pct = 15;
    cfg.rc_buf_sz = 6000;
    cfg.rc_buf_initial_sz = 4000;
    cfg.rc_buf_optimal_sz = 5000;
    cfg.kf_mode = AOM_KF_AUTO;
    cfg.kf_min_dist = 0;
    cfg.kf_max_dist = 9999;

    aom_codec_err_t res = aom_codec_enc_init_ver(&codec_ctx, codec_iface, &cfg, 
                                                0,  // flags
                                                AOM_ENCODER_ABI_VERSION);
    
    if (res != AOM_CODEC_OK) {
        aom_img_metadata_free(metadata);
        free(img_buffer);
        return 0;
    }

    // 5. Set an encoder option using ULEB value
    char option_value[64];
    snprintf(option_value, sizeof(option_value), "%lu", (unsigned long)uleb_value % 100);
    aom_codec_set_option(&codec_ctx, "cpu-used", option_value);

    // 6. Encode the image
    res = aom_codec_encode(&codec_ctx, wrapped_img, 
                          0,  // pts
                          1,  // duration (in timebase units)
                          0); // flags
    
    // 7. Remove metadata from image (even though we didn't actually add it with provided APIs)
    aom_img_remove_metadata(wrapped_img);

    // 8. Try to get encoded data (optional, to exercise more code paths)
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&codec_ctx, &iter)) != NULL) {
        // Process packets if needed
        (void)pkt;
    }

    // 9. Clean up
    aom_codec_destroy(&codec_ctx);
    aom_img_metadata_free(metadata);
    
    // Note: We shouldn't call aom_img_free on wrapped_img since we used aom_img_wrap
    // and allocated the buffer ourselves
    free(img_buffer);

    return 0;
}