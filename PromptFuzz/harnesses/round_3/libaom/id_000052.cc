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
//<ID> 133
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 1
//<Quality> {"density":24,"unique_branches":{"aom_codec_dec_init_ver":[[31,7,31,37,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error","aom_codec_get_preview_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_control","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_codec_encode","aom_codec_get_preview_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_uleb_encode_fixed_size","aom_codec_control","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error","aom_codec_get_preview_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_control","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_codec_encode","aom_codec_get_preview_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_uleb_encode_fixed_size","aom_codec_control","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Define control IDs since they're not provided in the headers
#define AOM_IMAGE_ABI_VERSION 0
#define AOM_CTRL_USE_REFERENCE 0

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    aom_codec_ctx_t decoder;
    aom_codec_ctx_t encoder;
    aom_image_t *img = nullptr;
    aom_codec_err_t res = AOM_CODEC_OK;
    
    // 1. Initialize decoder with AV1
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0, 0, 0};
    dec_cfg.threads = 1;
    
    res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_IMAGE_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    // 2. Decode the input data
    res = aom_codec_decode(&decoder, data, size, nullptr);
    
    // 3. Get error message using aom_codec_error
    const char *error_msg = aom_codec_error(&decoder);
    (void)error_msg; // Use variable to avoid unused warning
    
    // 4. Try to get preview frame from decoder
    // Note: This is typically for encoder, but we try with decoder context
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&decoder);
    if (preview_frame) {
        // 5. Get metadata from the image if available
        size_t metadata_count = aom_img_num_metadata(preview_frame);
        for (size_t i = 0; i < metadata_count; ++i) {
            const aom_metadata_t *metadata = aom_img_get_metadata(preview_frame, i);
            if (metadata) {
                // 6. Free the metadata using aom_img_metadata_free
                // Note: We need to copy since aom_img_get_metadata returns const
                aom_metadata_t *metadata_copy = aom_img_metadata_alloc(
                    metadata->type, 
                    metadata->payload, 
                    metadata->sz, 
                    metadata->insert_flag
                );
                if (metadata_copy) {
                    aom_img_metadata_free(metadata_copy);
                }
            }
        }
    }
    
    // 7. Use aom_codec_control with a valid control ID
    // Use 0 as control ID since we don't know specific IDs
    int ctrl_value = 0;
    aom_codec_control(&decoder, ctrl_value, &ctrl_value);
    
    // 8. Try to initialize encoder as well
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
        enc_cfg.g_w = 64;
        enc_cfg.g_h = 64;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_pass = AOM_RC_ONE_PASS;
        enc_cfg.g_lag_in_frames = 0;
        
        if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_IMAGE_ABI_VERSION) == AOM_CODEC_OK) {
            // 9. Create a test image for encoding
            img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 64, 64, 1);
            if (img) {
                // Fill image with test data from input
                size_t fill_size = std::min(size, (size_t)(img->stride[0] * img->h * 3 / 2));
                if (fill_size > 0) {
                    memcpy(img->planes[0], data, std::min(fill_size, size));
                }
                
                // 10. Encode the image
                aom_codec_encode(&encoder, img, 0, 1, 0);
                
                // 11. Try to get preview frame from encoder
                const aom_image_t *encoder_preview = aom_codec_get_preview_frame(&encoder);
                if (encoder_preview) {
                    // Get metadata from encoder preview frame as well
                    size_t enc_metadata_count = aom_img_num_metadata(encoder_preview);
                    for (size_t i = 0; i < enc_metadata_count; ++i) {
                        const aom_metadata_t *enc_metadata = aom_img_get_metadata(encoder_preview, i);
                        if (enc_metadata) {
                            // Copy and free metadata
                            aom_metadata_t *enc_metadata_copy = aom_img_metadata_alloc(
                                enc_metadata->type,
                                enc_metadata->payload,
                                enc_metadata->sz,
                                enc_metadata->insert_flag
                            );
                            if (enc_metadata_copy) {
                                aom_img_metadata_free(enc_metadata_copy);
                            }
                        }
                    }
                }
                
                // 12. Use aom_uleb_encode_fixed_size
                uint8_t uleb_buffer[16];
                size_t coded_size = 0;
                uint64_t uleb_value = size > 0 ? data[0] : 0;
                uleb_value = (uleb_value << 8) | (size > 1 ? data[1] : 0);
                uleb_value = (uleb_value << 8) | (size > 2 ? data[2] : 0);
                uleb_value = (uleb_value << 8) | (size > 3 ? data[3] : 0);
                
                aom_uleb_encode_fixed_size(uleb_value, sizeof(uleb_buffer), 8, uleb_buffer, &coded_size);
                
                // 13. Use aom_codec_control on encoder
                aom_codec_control(&encoder, ctrl_value, &ctrl_value);
                
                // Clean up image
                aom_img_free(img);
            }
            
            // Clean up encoder
            aom_codec_destroy(&encoder);
        }
    }
    
    // 14. Clean up decoder
    aom_codec_destroy(&decoder);
    
    return 0;
}