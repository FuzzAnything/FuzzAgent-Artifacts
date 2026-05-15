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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 10.666667, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"set_encoder_config":[[1347,7,1347,34,0,0,4,1]]},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_error_detail","aom_codec_decode","aom_codec_error_detail","aom_codec_get_frame","aom_img_plane_height","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_num_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_error_detail","aom_codec_get_frame","aom_img_plane_height","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_num_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":2}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if no data
    if (!data || size == 0) {
        return 0;
    }
    
    // Create a FILE pointer from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get decoder interface
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Check decoder capabilities
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    (void)caps; // Use caps to avoid unused variable warning
    
    // Initialize decoder configuration
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    // Initialize decoder context
    aom_codec_ctx_t decoder_ctx;
    aom_codec_err_t dec_init_result = aom_codec_dec_init_ver(
        &decoder_ctx, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (dec_init_result != AOM_CODEC_OK) {
        // Use error detail API
        const char *error_detail = aom_codec_error_detail(&decoder_ctx);
        (void)error_detail; // Use error_detail to avoid unused variable warning
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read and decode frames
    uint8_t buffer[4096];
    size_t bytes_read;
    bool decoding_done = false;
    
    while (!decoding_done) {
        bytes_read = fread(buffer, 1, sizeof(buffer), in_file);
        if (bytes_read == 0) {
            break;
        }
        
        aom_codec_err_t decode_result = aom_codec_decode(
            &decoder_ctx, buffer, bytes_read, NULL);
        
        if (decode_result != AOM_CODEC_OK) {
            // Try to get error details
            const char *error_detail = aom_codec_error_detail(&decoder_ctx);
            (void)error_detail; // Use error_detail to avoid unused variable warning
            break;
        }
        
        // Get decoded frame
        aom_codec_iter_t iter = NULL;
        aom_image_t *img = aom_codec_get_frame(&decoder_ctx, &iter);
        
        if (img) {
            // Use plane height API
            int plane_height = aom_img_plane_height(img, 0);
            (void)plane_height; // Use plane_height to avoid unused variable warning
            
            // Create metadata for the image
            uint32_t metadata_type = 1; // Example type
            uint8_t metadata_data[] = {0x01, 0x02, 0x03};
            size_t metadata_size = sizeof(metadata_data);
            
            aom_metadata_t *metadata = aom_img_metadata_alloc(
                metadata_type, metadata_data, metadata_size, AOM_MIF_ANY_FRAME);
            
            if (metadata) {
                // Note: In real usage, metadata would be attached to image
                // For fuzzing, we just allocate and free it
                aom_img_metadata_free(metadata);
            }
            
            // Get number of existing metadata (should be 0 since we didn't attach)
            size_t num_metadata = aom_img_num_metadata(img);
            (void)num_metadata; // Use num_metadata to avoid unused variable warning
            
            // Create encoder context for re-encoding
            aom_codec_ctx_t encoder_ctx;
            aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
            
            if (encoder_iface) {
                aom_codec_enc_cfg_t enc_cfg;
                aom_codec_err_t enc_cfg_result = aom_codec_enc_config_default(
                    encoder_iface, &enc_cfg, 0);
                
                if (enc_cfg_result == AOM_CODEC_OK) {
                    enc_cfg.g_w = img->d_w;
                    enc_cfg.g_h = img->d_h;
                    enc_cfg.g_timebase.num = 1;
                    enc_cfg.g_timebase.den = 30;
                    enc_cfg.rc_target_bitrate = 1000;
                    enc_cfg.g_pass = AOM_RC_ONE_PASS;
                    enc_cfg.g_lag_in_frames = 0;
                    enc_cfg.kf_mode = AOM_KF_DISABLED;
                    
                    aom_codec_err_t enc_init_result = aom_codec_enc_init_ver(
                        &encoder_ctx, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
                    
                    if (enc_init_result == AOM_CODEC_OK) {
                        // Encode the decoded frame
                        aom_codec_err_t encode_result = aom_codec_encode(
                            &encoder_ctx, img, 0, 1, 0);
                        
                        (void)encode_result; // Use encode_result to avoid unused variable warning
                        
                        // Get encoded data (optional for fuzzing)
                        aom_codec_iter_t enc_iter = NULL;
                        const aom_codec_cx_pkt_t *pkt;
                        while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter)) != NULL) {
                            // Packet type could be AOM_CODEC_CX_FRAME_PKT, etc.
                            // For fuzzing, we just consume the data
                            (void)pkt;
                        }
                        
                        // Destroy encoder
                        aom_codec_destroy(&encoder_ctx);
                    }
                }
            }
        }
        
        if (feof(in_file)) {
            decoding_done = true;
        }
    }
    
    // Cleanup
    aom_codec_destroy(&decoder_ctx);
    assert_file_closed(&in_file);;
    
    // Note: No output file is created since this is a fuzzer
    // The purpose is to exercise the APIs, not to produce valid output
    
    assert_file_closed(&in_file);
	return 0;
}