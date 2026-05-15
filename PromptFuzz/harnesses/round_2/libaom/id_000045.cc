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
//<ID> 123
//<Prompt> []
/*<Combination>: [
*/
//<score> 8.5, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["aom_uleb_encode","aom_uleb_encode_fixed_size","aom_uleb_decode","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_error_detail","aom_codec_error","aom_codec_decode","aom_codec_error","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_alloc_with_border","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_uleb_encode","aom_uleb_encode_fixed_size","aom_uleb_decode","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_alloc_with_border","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_plane_height","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < 64) {
	return 0;
	}
	
    
    // 1. ULEB encoding/decoding operations
    {
        uint64_t decoded_value = 0;
        size_t encoded_size = 0;
        size_t decoded_length = 0;
        std::vector<uint8_t> encoded_buffer(32, 0);
        
        // Use first byte as ULEB value (cap to avoid overflow)
        uint64_t test_value = data[0] & 0x7F;
        
        // Encode ULEB
        aom_uleb_encode(test_value, 32, encoded_buffer.data(), &encoded_size);
        
        // Fixed size encode
        aom_uleb_encode_fixed_size(test_value, 32, 8, encoded_buffer.data(), &encoded_size);
        
        // Decode ULEB
        aom_uleb_decode(data, std::min<size_t>(size, 16), &decoded_value, &decoded_length);
    }
    
    // 2. Decoding path: AV1 bitstream decoding
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
	return 0;
	}
	
    
    // Get decoder capabilities
    aom_codec_caps_t decoder_caps = aom_codec_get_caps(decoder_iface);
    (void)decoder_caps; // Suppress unused warning
    
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 128;
    dec_cfg.h = 128;
    dec_cfg.allow_lowbitdepth = 1;
    
    // Initialize decoder
    aom_codec_err_t ret = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, 
                                                 AOM_DECODER_ABI_VERSION);
    if (ret != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&decoder);
        const char *error_str = aom_codec_error(&decoder);
        (void)error_detail;
        (void)error_str;
        return 0;
    }
    
    // Decode input data
    ret = aom_codec_decode(&decoder, data, std::min<size_t>(size, 4096), nullptr);
    if (ret != AOM_CODEC_OK) {
        const char *error_str = aom_codec_error(&decoder);
        (void)error_str;
    }
    
    // Get stream info
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    aom_codec_get_stream_info(&decoder, &si);
    
    // Iterate through decoded frames
    aom_codec_iter_t iter = nullptr;
    const aom_image_t *decoded_frame = aom_codec_get_frame(&decoder, &iter);
    
    // Allocate image for encoding
    aom_image_t *img = nullptr;
    if (decoded_frame) {
        // Create image with border for potential encoding
        img = aom_img_alloc_with_border(nullptr, decoded_frame->fmt,
                                       decoded_frame->d_w, decoded_frame->d_h,
                                       32, 32, 32);
        
        if (img) {
            // Process metadata
            size_t num_meta = aom_img_num_metadata(decoded_frame);
            for (size_t i = 0; i < num_meta; ++i) {
                const aom_metadata_t *meta = aom_img_get_metadata(decoded_frame, i);
                if (meta) {
                    // Allocate and free metadata
                    aom_metadata_t *new_meta = aom_img_metadata_alloc(
                        meta->type, meta->payload, meta->sz, meta->insert_flag);
                    if (new_meta) {
                        aom_img_metadata_free(new_meta);
                    }
                }
            }
            
            // Get plane height
            int plane_height = aom_img_plane_height(img, 0);
            (void)plane_height;
        }
    }
    
    // 3. Encoding path: Re-encode if we have an image
    if (img) {
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        if (encoder_iface) {
            aom_codec_ctx_t encoder;
            aom_codec_enc_cfg_t enc_cfg;
            
            // Get default encoder config
            ret = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
            if (ret == AOM_CODEC_OK) {
                // Set basic parameters
                enc_cfg.g_w = img->d_w;
                enc_cfg.g_h = img->d_h;
                enc_cfg.rc_target_bitrate = 256;
                
                // Initialize encoder
                ret = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0,
                                            AOM_ENCODER_ABI_VERSION);
                
                if (ret == AOM_CODEC_OK) {
                    // Encode the image
                    static int frame_count = 0;
                    ret = aom_codec_encode(&encoder, img, frame_count++, 1, 0);
                    
                    if (ret == AOM_CODEC_OK) {
                        // Get encoded data
                        aom_codec_iter_t enc_iter = nullptr;
                        const aom_codec_cx_pkt_t *pkt = nullptr;
                        
                        while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                            // Process packet if needed
                            (void)pkt;
                        }
                    }
                    
                    // Destroy encoder
                    aom_codec_destroy(&encoder);
                }
            }
        }
        
        // Free the allocated image
        aom_img_free(img);
    }
    
    // Destroy decoder
    aom_codec_destroy(&decoder);
    
    return 0;
}