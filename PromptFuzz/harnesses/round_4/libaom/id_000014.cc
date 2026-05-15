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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["aom_codec_build_config","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_codec_destroy","aom_codec_encode","aom_codec_get_preview_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_img_free","aom_codec_destroy","aom_codec_decode","aom_codec_get_frame","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_build_config","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_codec_encode","aom_codec_get_preview_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<56) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // 1. Get build configuration string
    const char *build_config = aom_codec_build_config();
    (void)build_config; // Use to avoid unused variable warning

    // 2. Initialize encoder context
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;
    
    // Get default encoder configuration
    if (aom_codec_enc_config_default(encoder_iface, &cfg, fuzz_uint32_t_1) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Set basic configuration
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256;
    cfg.g_threads = 1;
    
    // Initialize encoder
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, fuzz_int64_t_2, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }

    // 3. Create input image for encoder
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, fuzz_uint32_t_3);
    if (!img) {
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // Fill image with some data (using fuzzer input for partial initialization)
    size_t bytes_to_copy = size < img->sz ? size : img->sz;
    if (bytes_to_copy > 0) {
        memcpy(img->planes[0], data, bytes_to_copy > img->stride[0] * img->h ? 
               img->stride[0] * img->h : bytes_to_copy);
    }

    // 4. Encode one frame
    aom_codec_encode(&encoder, img, fuzz_int64_t_4, fuzz_uint64_t_5, fuzz_int64_t_6);
    
    // 5. Try to get preview frame (may return NULL)
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&encoder);
    if (preview_frame) {
        // Check if preview frame has metadata
        size_t num_metadata = aom_img_num_metadata(preview_frame);
        for (size_t i = 0; i < num_metadata; i++) {
            const aom_metadata_t *metadata = aom_img_get_metadata(preview_frame, fuzz_uint64_t_7);
            // Create a copy of metadata to free
            if (metadata) {
                aom_metadata_t *metadata_copy = aom_img_metadata_alloc(
                    metadata->type, 
                    metadata->payload, 
                    metadata->sz, 
                    metadata->insert_flag
                );
                if (metadata_copy) {
                    // 6. Free metadata (required API)
                    aom_img_metadata_free(metadata_copy);
                }
            }
        }
    }

    // 7. Initialize decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, NULL, fuzz_int64_t_8, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        aom_img_free(img);
        aom_codec_destroy(&encoder);
        return 0;
    }

    // 8. Decode the input data (required API)
    // Use the fuzzer input as compressed bitstream
    aom_codec_decode(&decoder, data, size, NULL);
    
    // Get decoded frames
    aom_codec_iter_t iter = NULL;
    const aom_image_t *decoded_frame;
    while ((decoded_frame = aom_codec_get_frame(&decoder, &iter)) != NULL) {
        // Process decoded frame if needed
        (void)decoded_frame;
    }

    // 9. Cleanup
    aom_img_free(img);
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);

    return 0;
}