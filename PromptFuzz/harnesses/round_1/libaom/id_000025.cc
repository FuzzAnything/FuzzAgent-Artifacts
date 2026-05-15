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
//<ID> 57
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"parse_decode_block":[[1179,48,1179,70,0,0,4,0]]},"library_calls":["aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_decode","aom_codec_destroy","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_codec_get_preview_frame","aom_uleb_encode","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_destroy","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_codec_get_preview_frame","aom_uleb_encode","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


// Include libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Too small for meaningful processing
    
    // 1. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 2;
    dec_cfg.w = 320;
    dec_cfg.h = 240;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, aom_codec_av1_dx(), &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        // Try without config
        res = aom_codec_dec_init_ver(&decoder, aom_codec_av1_dx(), nullptr, 0, AOM_DECODER_ABI_VERSION);
        if (res != AOM_CODEC_OK) {
            return 0;
        }
    }
    
    // 2. Decode the input data
    res = aom_codec_decode(&decoder, data, size, nullptr);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // 3. Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_img = nullptr;
    int frame_count = 0;
    
    while ((decoded_img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        frame_count++;
        
        // 4. Get metadata from decoded image
        for (size_t i = 0; i < aom_img_num_metadata(decoded_img); i++) {
            const aom_metadata_t *metadata = aom_img_get_metadata(decoded_img, i);
            // Just access metadata to satisfy API usage
            if (metadata) {
                // Do nothing, just ensure the API is called
            }
        }
        
        // 5. Try to get preview frame
        const aom_image_t *preview = aom_codec_get_preview_frame(&decoder);
        if (preview) {
            // Preview frame exists, just ensure API is called
        }
        
        // 6. Use ULEB encode on some value derived from input
        uint64_t uleb_value = static_cast<uint64_t>(size);
        uint8_t uleb_buffer[10];
        size_t coded_size = 0;
        if (size > 0) {
            uleb_value = data[0];
        }
        aom_uleb_encode(uleb_value, sizeof(uleb_buffer), uleb_buffer, &coded_size);
        
        // 7. Initialize encoder
        aom_codec_ctx_t encoder;
        aom_codec_enc_cfg_t enc_cfg;
        aom_codec_enc_config_default(aom_codec_av1_cx(), &enc_cfg, 0);
        
        // Set encoder configuration based on decoded image
        if (decoded_img) {
            enc_cfg.g_w = decoded_img->d_w;
            enc_cfg.g_h = decoded_img->d_h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.rc_target_bitrate = 1000;
            enc_cfg.g_threads = 2;
            enc_cfg.g_profile = 0;
            enc_cfg.g_bit_depth = (aom_bit_depth_t)decoded_img->bit_depth;
            enc_cfg.g_input_bit_depth = decoded_img->bit_depth;
        }
        
        res = aom_codec_enc_init_ver(&encoder, aom_codec_av1_cx(), &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
        if (res != AOM_CODEC_OK) {
            aom_codec_destroy(&decoder);
            return 0;
        }
        
        // 8. Set encoder configuration
        res = aom_codec_enc_config_set(&encoder, &enc_cfg);
        // We continue even if this fails
        
        // 9. Prepare output buffer for encoded data
        aom_fixed_buf_t output_buf;
        output_buf.buf = malloc(4096);
        output_buf.sz = 4096;
        
        if (output_buf.buf) {
            // 10. Set output buffer for encoded data
            res = aom_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);
            // We continue even if this fails
            
            // 11. Encode the decoded image
            aom_codec_encode(&encoder, decoded_img, 0, 1, 0);
            
            free(output_buf.buf);
        }
        
        // Destroy encoder for this frame
        aom_codec_destroy(&encoder);
        
        // Break after processing first frame to limit resource usage
        break;
    }
    
    // 12. Destroy decoder
    aom_codec_destroy(&decoder);
    
    return 0;
}