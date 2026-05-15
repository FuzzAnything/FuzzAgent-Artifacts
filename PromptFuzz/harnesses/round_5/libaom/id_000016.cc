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
//<ID> 23
//<Prompt> []
/*<Combination>: [
*/
//<score> 64, nr_unique_branch: 7
//<Quality> {"density":16,"unique_branches":{"set_bitstream_level_tier":[[481,14,482,33,0,0,4,1],[484,14,485,33,0,0,4,1],[487,14,488,33,0,0,4,1],[490,14,491,33,0,0,4,0]],"set_tile_info":[[388,12,388,61,0,0,4,0],[35,23,35,34,3,0,4,0]],"does_level_match":[[460,10,460,43,0,0,4,1]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_destroy","aom_codec_decode","aom_codec_error","aom_codec_destroy","aom_codec_get_stream_info","aom_codec_destroy","aom_codec_get_frame","aom_img_plane_height","aom_img_num_metadata","aom_img_get_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_destroy","aom_codec_enc_config_set","aom_codec_error","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_decode","aom_codec_error","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_height","aom_img_num_metadata","aom_img_get_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_error","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 1;
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    // Use aom_codec_err_to_string to convert error code
    const char *err_str = aom_codec_err_to_string(err);
    (void)err_str; // Prevent unused variable warning
    
    if (err != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read and decode data
    uint8_t *buffer = (uint8_t *)malloc(size);
    if (!buffer) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    fread(buffer, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Decode the frame
    err = aom_codec_decode(&decoder, buffer, size, nullptr);
    
    // Use aom_codec_error to get error string
    const char *decoder_error = aom_codec_error(&decoder);
    (void)decoder_error; // Prevent unused variable warning
    
    free(buffer);
    
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get stream information
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    err = aom_codec_get_stream_info(&decoder, &stream_info);
    
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img = nullptr;
    
    while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Use aom_img_plane_height for each plane
        for (int plane = 0; plane < 3; plane++) {
            int plane_height = aom_img_plane_height(img, plane);
            (void)plane_height; // Prevent unused variable warning
        }
        
        // Get metadata if available
        size_t num_metadata = aom_img_num_metadata(img);
        for (size_t i = 0; i < num_metadata; i++) {
            const aom_metadata_t *metadata = aom_img_get_metadata(img, i);
            (void)metadata; // Prevent unused variable warning
        }
    }

    // Initialize encoder to use aom_codec_enc_config_set
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    err = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set encoder configuration based on stream info
    if (stream_info.w > 0 && stream_info.h > 0) {
        enc_cfg.g_w = stream_info.w;
        enc_cfg.g_h = stream_info.h;
    } else {
        enc_cfg.g_w = 640;
        enc_cfg.g_h = 480;
    }
    enc_cfg.rc_target_bitrate = 1000;
    
    err = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Use aom_codec_enc_config_set to update configuration
    enc_cfg.rc_target_bitrate = 2000;
    err = aom_codec_enc_config_set(&encoder, &enc_cfg);
    
    // Get error string for encoder
    const char *encoder_error = aom_codec_error(&encoder);
    (void)encoder_error; // Prevent unused variable warning

    // Cleanup
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);
	return 0;
}