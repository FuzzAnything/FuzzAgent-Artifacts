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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 0
//<Quality> {"density":28,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_destroy","aom_codec_get_frame","aom_codec_destroy","aom_img_plane_width","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_destroy","aom_codec_set_option","aom_codec_control","aom_codec_get_global_headers","aom_img_alloc","aom_codec_destroy","aom_codec_destroy","aom_img_plane_height","aom_codec_encode","aom_img_free","aom_codec_destroy","aom_codec_destroy","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_plane_width","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_control","aom_codec_get_global_headers","aom_img_alloc","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small
    if (size < 100) {
	return 0;
	}
	
    
    // Initialize decoder to parse input stream
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // Decode input stream to get frames
    if (aom_codec_decode(&decoder, data, size, nullptr) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Get decoded frame
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_img = aom_codec_get_frame(&decoder, &iter);
    
    if (!decoded_img) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Use aom_img_plane_width to get plane width
    int plane_width = aom_img_plane_width(decoded_img, 0);
    
    // Remove metadata from decoded image if present
    aom_img_remove_metadata(decoded_img);
    
    // Set up encoder configuration
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    // Get default encoder configuration
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Set encoder parameters based on decoded image
    enc_cfg.g_w = decoded_img->d_w;
    enc_cfg.g_h = decoded_img->d_h;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.g_threads = 1;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_profile = 0;
    enc_cfg.g_input_bit_depth = (unsigned int)decoded_img->bit_depth;
    
    // Initialize encoder using aom_codec_enc_init_ver
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Use aom_codec_set_option to set encoder option
    aom_codec_set_option(&encoder, "cpu-used", "5");
    
    // Use aom_codec_control to set encoder control
    aom_codec_control(&encoder, AOME_SET_CPUUSED, 5);
    
    // Get global headers from encoder
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&encoder);
    
    // Create output image for encoding (copy from decoded image)
    aom_image_t *input_img = aom_img_alloc(nullptr, decoded_img->fmt, 
                                          decoded_img->d_w, decoded_img->d_h, 1);
    if (!input_img) {
        if (global_headers) free(global_headers->buf);
        aom_codec_destroy(&encoder);
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Copy image data
    for (int plane = 0; plane < 3; plane++) {
        int stride = decoded_img->stride[plane];
        int height = aom_img_plane_height(decoded_img, plane);
        for (int y = 0; y < height; y++) {
            memcpy(input_img->planes[plane] + y * stride,
                   decoded_img->planes[plane] + y * stride,
                   stride);
        }
    }
    
    // Encode the image
    if (aom_codec_encode(&encoder, input_img, 0, 1, 0) != AOM_CODEC_OK) {
        aom_img_free(input_img);
        if (global_headers) free(global_headers->buf);
        aom_codec_destroy(&encoder);
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Get encoded data
    aom_codec_iter_t enc_iter = nullptr;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        // Process encoded packets (do nothing in fuzzing)
    }
    
    // Cleanup
    aom_img_free(input_img);
    if (global_headers) free(global_headers->buf);
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);
    
    return 0;
}