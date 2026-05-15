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
//<ID> 34
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 7
//<Quality> {"density":8,"unique_branches":{"aom_uleb_encode_fixed_size":[[83,34,83,66,0,0,4,1],[87,7,87,21,0,0,4,0],[87,7,87,21,0,0,4,1],[92,22,92,37,0,0,4,0],[92,22,92,37,0,0,4,1],[96,9,96,28,0,0,4,0],[96,9,96,28,0,0,4,1]]},"library_calls":["aom_codec_build_config","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_get_preview_frame","aom_codec_destroy","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_peek_stream_info","aom_codec_get_frame","aom_codec_destroy","aom_img_wrap","aom_img_set_rect","aom_uleb_decode","aom_uleb_encode","aom_uleb_encode_fixed_size"],"critical_calls":["aom_codec_build_config","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_get_preview_frame","aom_codec_destroy","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_peek_stream_info","aom_codec_get_frame","aom_codec_destroy","aom_img_wrap","aom_img_set_rect","aom_uleb_decode","aom_uleb_encode","aom_uleb_encode_fixed_size"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<52) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
	//fuzzer shim end}




    // Early exit if we don't have enough data for basic operations
    if (size < 4) {
	return 0;
	}
	
    
    // 1. Use aom_codec_build_config to get build configuration string
    const char *build_config = aom_codec_build_config();
    // Just to prevent unused variable warning - we use it in a way that won't affect execution
    if (build_config) { /* Do nothing, just call the function */ }
    
    // 2. Create and free metadata to use aom_img_metadata_free
    aom_metadata_t *metadata = aom_img_metadata_alloc(fuzz_uint32_t_1, data, size > 100 ? 100 : size, AOM_MIF_ANY_FRAME);
    if (metadata) {
        aom_img_metadata_free(metadata);
    }
    
    // 3. Initialize encoder with default configuration
    aom_codec_ctx_t encoder_ctx;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    // Initialize encoder with default configuration
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_2) == AOM_CODEC_OK) {
        // Set some minimal dimensions to avoid validation errors
        enc_cfg.g_w = 64;
        enc_cfg.g_h = 64;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        
        // Initialize encoder with version flag
        if (aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, fuzz_int64_t_3, AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
            
            // 4. Try to get preview frame from encoder (may return NULL if no frame available)
            const aom_image_t *preview_frame = aom_codec_get_preview_frame(&encoder_ctx);
            // Just to prevent unused variable warning
            if (preview_frame) { /* Do nothing, just call the function */ }
            
            // Clean up encoder
            aom_codec_destroy(&encoder_ctx);
        }
    }
    
    // 5. Initialize decoder and decode the input data
    aom_codec_ctx_t decoder_ctx;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // Initialize decoder with default configuration
    if (aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, NULL, fuzz_int64_t_4, AOM_DECODER_ABI_VERSION) == AOM_CODEC_OK) {
        // Try to decode the input data
        // We pass the data as user_priv for potential use in callback functions
        aom_codec_decode(&decoder_ctx, data, size, NULL);
        
        // Try to get stream information
        aom_codec_stream_info_t stream_info;
        memset(&stream_info, 0, sizeof(stream_info));
        aom_codec_peek_stream_info(decoder_iface, data, size > 1024 ? 1024 : size, &stream_info);
        
        // Try to get decoded frames
        aom_codec_iter_t iter = NULL;
        aom_image_t *img;
        while ((img = aom_codec_get_frame(&decoder_ctx, &iter)) != NULL) {
            // Do nothing with the image, just iterate through available frames
        }
        
        // Clean up decoder
        aom_codec_destroy(&decoder_ctx);
    }
    
    // 6. Additional test: Create a simple image and manipulate it
    aom_image_t img;
    unsigned char img_data[64 * 64 * 3];  // Simple RGB data for 64x64 image
    
    // Wrap the image data
    aom_image_t *wrapped_img = aom_img_wrap(&img, AOM_IMG_FMT_I420, 64, fuzz_uint32_t_5, fuzz_uint32_t_6, img_data);
    if (wrapped_img) {
        // Try to set a rectangle (using some data from input)
        if (size >= 8) {
            unsigned int x = data[0] % 32;
            unsigned int y = data[1] % 32;
            unsigned int w = 16 + (data[2] % 16);
            unsigned int h = 16 + (data[3] % 16);
            aom_img_set_rect(wrapped_img, x, y, w, h, fuzz_uint32_t_7);
        }
        
        // Free the image (img_data is stack allocated, so no free needed)
        // aom_img_free only frees if img_data_owner is set, which it isn't for aom_img_wrap
    }
    
    // 7. Test ULEB encoding/decoding with input data
    if (size >= 16) {
        uint64_t value = 0;
        size_t length = 0;
        
        // Decode a ULEB value from the beginning of the data
        if (aom_uleb_decode(data, size > 100 ? 100 : size, &value, &length) == 0) {
            // Successfully decoded, now try to encode it back
            uint8_t encoded[10];
            size_t encoded_size = 0;
            
            aom_uleb_encode(fuzz_uint64_t_8, sizeof(encoded), encoded, &encoded_size);
            
            // Also try fixed size encoding
            if (size >= 20) {
                size_t pad_to = data[10] % 8 + 1;
                aom_uleb_encode_fixed_size(fuzz_uint64_t_9, sizeof(encoded), pad_to, encoded, &encoded_size);
            }
        }
    }
    
    return 0;
}