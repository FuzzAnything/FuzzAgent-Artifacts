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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_error_detail","aom_img_plane_height","aom_img_metadata_alloc","aom_img_num_metadata","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_plane_height","aom_codec_encode","aom_codec_error_detail","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_img_plane_height","aom_img_metadata_alloc","aom_img_num_metadata","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_plane_height","aom_codec_encode","aom_codec_error_detail","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


// Include the necessary libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Step 1: Get decoder interface using aom_codec_av1_dx()
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
	return 0;
	}
	

    // Step 2: Get decoder capabilities using aom_codec_get_caps()
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    (void)caps; // Mark as used to prevent warning

    // Step 3: Initialize decoder context using aom_codec_dec_init_ver()
    aom_codec_ctx_t decoder_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t init_result = aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, 
                                                         0, AOM_DECODER_ABI_VERSION);
    
    // Step 4: Use error detail API if initialization failed
    if (init_result != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&decoder_ctx);
        (void)error_detail; // Mark as used
        return 0;
    }

    // Step 5: Decode the input data
    // Note: In real usage, we'd use aom_codec_decode() here, but we're required to use
    // specific APIs from the list. We'll demonstrate with metadata APIs instead.
    
    // Step 6: Create a dummy image for metadata operations
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    img.fmt = AOM_IMG_FMT_I420;
    img.w = img.d_w = 64;
    img.h = img.d_h = 64;
    img.bit_depth = 8;
    
    // Step 7: Use plane height API
    int plane_height = aom_img_plane_height(&img, 0);
    (void)plane_height; // Mark as used
    
    // Step 8: Create metadata using aom_img_metadata_alloc()
    // Use first 4 bytes of input for metadata type if available
    uint32_t metadata_type = size >= 4 ? *(reinterpret_cast<const uint32_t*>(data)) : 1234;
    size_t metadata_size = size > 100 ? 100 : size; // Limit metadata size
    
    aom_metadata_t *metadata = aom_img_metadata_alloc(
        metadata_type, 
        data, 
        metadata_size, 
        AOM_MIF_ANY_FRAME);
    
    if (metadata) {
        // Step 9: Add metadata to image
        // Note: This would normally be done on an actual allocated image
        // For demonstration, we just track that we created metadata
        
        // Step 10: Get number of metadata (would be 0 for our dummy image)
        size_t num_metadata = aom_img_num_metadata(&img);
        (void)num_metadata; // Mark as used
        
        // Step 11: Free the metadata using aom_img_metadata_free()
        aom_img_metadata_free(metadata);
    }

    // Step 12: Try to encode something using the encoder
    // First get encoder interface
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    if (encoder_iface) {
        aom_codec_ctx_t encoder_ctx;
        aom_codec_enc_cfg_t enc_cfg;
        
        // Get default configuration
        if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
            // Initialize encoder
            if (aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, 0, 
                                       AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
                
                // Create a minimal image for encoding
                aom_image_t *enc_img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 16, 16, 1);
                if (enc_img) {
                    // Fill with some data from input
                    size_t fill_size = size > 100 ? 100 : size;
                    for (int i = 0; i < 3; i++) {
                        size_t plane_size = enc_img->stride[i] * aom_img_plane_height(enc_img, i);
                        if (plane_size > 0 && fill_size > 0) {
                            size_t copy_size = fill_size > plane_size ? plane_size : fill_size;
                            memcpy(enc_img->planes[i], data, copy_size);
                        }
                    }
                    
                    // Step 13: Use aom_codec_encode() API
                    aom_codec_err_t encode_result = aom_codec_encode(&encoder_ctx, enc_img, 
                                                                     0, 1, 0);
                    (void)encode_result; // Mark as used
                    
                    // Get error detail if encoding failed
                    const char *encode_error = aom_codec_error_detail(&encoder_ctx);
                    (void)encode_error; // Mark as used
                    
                    aom_img_free(enc_img);
                }
                
                aom_codec_destroy(&encoder_ctx);
            }
        }
    }

    // Step 14: Clean up decoder
    aom_codec_destroy(&decoder_ctx);

    return 0;
}