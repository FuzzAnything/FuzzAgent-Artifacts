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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 2
//<Quality> {"density":15,"unique_branches":{"aom_codec_err_to_string":[[49,5,49,33,0,0,4,0]],"aom_codec_enc_config_set":[[302,15,302,26,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_error","aom_codec_destroy","aom_codec_peek_stream_info","aom_codec_get_stream_info","aom_codec_error_detail","aom_codec_decode","aom_codec_error","aom_codec_get_frame","aom_img_plane_height","aom_img_num_metadata","aom_img_get_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_config_set","aom_codec_error","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_err_to_string","aom_codec_error","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_peek_stream_info","aom_codec_get_stream_info","aom_codec_error_detail","aom_codec_decode","aom_codec_error","aom_codec_get_frame","aom_img_plane_height","aom_img_num_metadata","aom_img_get_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_config_set","aom_codec_error","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_err_to_string","aom_codec_error","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libaom APIs to decode AV1 video data from the input byte stream and extract information:

*/



extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Create a decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // Initialize decoder with default configuration
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    // Use error APIs
    const char *err_str = aom_codec_err_to_string(err);
    if (err != AOM_CODEC_OK) {
        const char *decoder_error = aom_codec_error(&decoder);
        // Clean up partially initialized decoder
        aom_codec_destroy(&decoder);
        return 0;
    }

    // Get stream information
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // First try to peek stream info
    if (aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info) == AOM_CODEC_OK) {
        // Then get stream info from decoder context
        err = aom_codec_get_stream_info(&decoder, &stream_info);
        if (err != AOM_CODEC_OK) {
            const char *error_detail = aom_codec_error_detail(&decoder);
        }
    }

    // Decode the input data
    err = aom_codec_decode(&decoder, data, size, nullptr);
    if (err != AOM_CODEC_OK) {
        const char *decoder_error = aom_codec_error(&decoder);
    }

    // Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img = nullptr;
    
    while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Use image plane height API
        for (int plane = 0; plane < 3; plane++) {
            int plane_height = aom_img_plane_height(img, plane);
            // Just check the value is reasonable
            if (plane_height < 0 || plane_height > 10000) {
                // Handle potential issue
            }
        }
        
        // Get metadata if available
        size_t num_metadata = aom_img_num_metadata(img);
        for (size_t i = 0; i < num_metadata; i++) {
            const aom_metadata_t *metadata = aom_img_get_metadata(img, i);
            if (metadata) {
                // Access metadata fields
                uint32_t type = metadata->type;
                size_t sz = metadata->sz;
            }
        }
    }

    // Create an encoder context to use aom_codec_enc_config_set
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    // Get default encoder configuration
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
        // Set encoder configuration
        err = aom_codec_enc_config_set(&encoder, &enc_cfg);
        if (err != AOM_CODEC_OK) {
            const char *encoder_error = aom_codec_error(&encoder);
        }
    }
    
    // Initialize encoder (even if we don't use it for encoding)
    err = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (err == AOM_CODEC_OK) {
        // Set encoder configuration again (this is the API we need to use)
        err = aom_codec_enc_config_set(&encoder, &enc_cfg);
        if (err != AOM_CODEC_OK) {
            const char *error_str = aom_codec_err_to_string(err);
            const char *encoder_error = aom_codec_error(&encoder);
        }
        
        // Clean up encoder
        aom_codec_destroy(&encoder);
    }

    // Clean up decoder
    aom_codec_destroy(&decoder);

    return 0;
}