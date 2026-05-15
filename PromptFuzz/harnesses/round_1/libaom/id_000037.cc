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
//<ID> 82
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 2
//<Quality> {"density":10,"unique_branches":{"aom_img_set_rect":[[246,28,246,43,0,0,4,1],[247,7,247,22,0,0,4,1]]},"library_calls":["aom_uleb_decode","aom_img_alloc","aom_img_set_rect","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_get_caps","aom_codec_enc_config_default","aom_img_free","aom_codec_enc_init_ver","aom_img_free","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_destroy","aom_img_free","aom_codec_decode","aom_codec_get_stream_info","aom_codec_destroy","aom_codec_destroy","aom_img_free"],"critical_calls":["aom_uleb_decode","aom_img_alloc","aom_img_set_rect","aom_img_add_metadata","aom_img_num_metadata","aom_img_get_metadata","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_destroy","aom_codec_destroy","aom_img_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 16) {
        return 0;  // Not enough data for meaningful fuzzing
    }

    // 1. Use aom_uleb_decode to decode a value from the input
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    int uleb_result = aom_uleb_decode(data, size, &uleb_value, &uleb_length);
    // Note: uleb_result indicates success/failure, but we continue regardless

    // 2. Create an image to work with
    const unsigned int width = 64;
    const unsigned int height = 64;
    const unsigned int align = 16;
    const aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    
    aom_image_t img;
    if (aom_img_alloc(&img, fmt, width, height, align) == nullptr) {
        return 0;  // Image allocation failed
    }

    // 3. Use aom_img_set_rect with values derived from input
    // Use modulo to ensure values stay within bounds
    unsigned int rect_x = data[0] % (width - 8);
    unsigned int rect_y = data[1] % (height - 8);
    unsigned int rect_w = 8 + (data[2] % 8);  // 8-16 pixels
    unsigned int rect_h = 8 + (data[3] % 8);  // 8-16 pixels
    unsigned int border = data[4] % 4;
    
    aom_img_set_rect(&img, rect_x, rect_y, rect_w, rect_h, border);

    // 4. Add some metadata to the image
    // Use a portion of input data as metadata payload
    size_t metadata_sz = size > 100 ? 20 : size / 5;
    if (metadata_sz > 0 && metadata_sz < size) {
        uint8_t *metadata_data = (uint8_t *)malloc(metadata_sz);
        if (metadata_data) {
            memcpy(metadata_data, data + (size - metadata_sz), metadata_sz);
            aom_img_add_metadata(&img, 1, metadata_data, metadata_sz, AOM_MIF_ANY_FRAME);
            free(metadata_data);
        }
    }

    // 5. Get and remove metadata
    size_t num_metadata = aom_img_num_metadata(&img);
    if (num_metadata > 0) {
        const aom_metadata_t *metadata = aom_img_get_metadata(&img, 0);
        // We could use metadata here if needed
    }
    aom_img_remove_metadata(&img);

    // 6. Get codec capabilities
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_caps_t caps = aom_codec_get_caps(encoder_iface);
    // caps could be checked for specific features

    // 7. Initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t cfg;
    
    // Get default configuration
    if (aom_codec_enc_config_default(encoder_iface, &cfg, 0) != AOM_CODEC_OK) {
        aom_img_free(&img);
        return 0;
    }
    
    // Set some basic parameters
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    
    // Initialize encoder with version check
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        aom_img_free(&img);
        return 0;
    }

    // 8. Initialize decoder and get stream info
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, nullptr, 0, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        aom_codec_destroy(&encoder);
        aom_img_free(&img);
        return 0;
    }

    // Create stream info structure
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // Try to get stream info from the input data
    // Note: aom_codec_get_stream_info requires a valid stream and initialized decoder context
    // We'll attempt to decode first to populate the context
    if (size > 0) {
        // Try to decode a frame to populate decoder context
        aom_codec_decode(&decoder, data, size, nullptr);
        
        // Now try to get stream info
        aom_codec_get_stream_info(&decoder, &stream_info);
    }

    // 9. Cleanup
    aom_codec_destroy(&decoder);
    aom_codec_destroy(&encoder);
    aom_img_free(&img);

    return 0;
}