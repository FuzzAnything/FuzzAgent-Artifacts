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
//<ID> 24
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_decode","aom_codec_error","aom_codec_destroy","aom_codec_get_frame","aom_codec_destroy","aom_img_plane_height","aom_img_num_metadata","aom_img_get_metadata","aom_codec_get_stream_info","aom_codec_error","aom_codec_destroy","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_err_to_string","aom_codec_destroy","aom_codec_enc_config_set","aom_codec_error","aom_codec_destroy","aom_codec_destroy","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_plane_height","aom_img_num_metadata","aom_img_get_metadata","aom_codec_get_stream_info","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_error","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if data is too small for meaningful processing
    if (size < 10) {
        return 0;
    }

    // Use the provided APIs in a coherent sequence:
    // 1. Decode the input bitstream to get an image
    // 2. Extract metadata from the decoded image
    // 3. Get stream information
    // 4. Configure encoder with the stream info
    // 5. Handle errors throughout

    aom_codec_ctx_t decoder;
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_stream_info_t stream_info;
    aom_image_t *img = nullptr;
    aom_codec_err_t res;
    const aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    const aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();

    // Initialize decoder with default configuration
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 1;
    res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    // Use aom_codec_err_to_string to convert error code
    if (res != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(res);
        // Error string is available but we don't need to use it further in fuzzing
        (void)err_str;
        return 0;
    }

    // Decode the input data
    res = aom_codec_decode(&decoder, data, size, nullptr);
    
    // Check for errors using aom_codec_error
    if (res != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error(&decoder);
        (void)error_detail;
        aom_codec_destroy(&decoder);
        return 0;
    }

    // Get the decoded frame
    aom_codec_iter_t iter = nullptr;
    img = aom_codec_get_frame(&decoder, &iter);
    
    if (!img) {
        aom_codec_destroy(&decoder);
        return 0;
    }

    // Use aom_img_plane_height to get plane heights
    for (int plane = 0; plane < 3; ++plane) {
        int plane_h = aom_img_plane_height(img, plane);
        (void)plane_h; // Use the result to prevent optimization
    }

    // Use aom_img_get_metadata to retrieve metadata if present
    size_t metadata_count = aom_img_num_metadata(img);
    for (size_t i = 0; i < metadata_count; ++i) {
        const aom_metadata_t *metadata = aom_img_get_metadata(img, i);
        (void)metadata;
    }

    // Get stream information from the decoder
    memset(&stream_info, 0, sizeof(stream_info));
    res = aom_codec_get_stream_info(&decoder, &stream_info);
    
    if (res != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error(&decoder);
        (void)error_detail;
        aom_codec_destroy(&decoder);
        return 0;
    }

    // Initialize encoder with default configuration based on stream info
    unsigned int usage = 0; // Good quality
    res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, usage);
    
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }

    // Adjust encoder configuration based on decoded stream info
    enc_cfg.g_w = stream_info.w;
    enc_cfg.g_h = stream_info.h;
    enc_cfg.rc_target_bitrate = 1000; // Arbitrary bitrate for fuzzing

    // Initialize encoder
    res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
    
    if (res != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(res);
        (void)err_str;
        aom_codec_destroy(&decoder);
        return 0;
    }

    // Use aom_codec_enc_config_set to apply configuration (redundant but demonstrates API)
    res = aom_codec_enc_config_set(&encoder, &enc_cfg);
    
    // Check error using aom_codec_error
    if (res != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error(&encoder);
        (void)error_detail;
        aom_codec_destroy(&decoder);
        aom_codec_destroy(&encoder);
        return 0;
    }

    // Clean up
    aom_codec_destroy(&decoder);
    aom_codec_destroy(&encoder);
    
    return 0;
}