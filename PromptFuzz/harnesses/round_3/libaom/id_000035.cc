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
//<ID> 83
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":24,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_error","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_error","aom_codec_get_frame","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_error","aom_codec_encode","aom_codec_error","aom_codec_get_cx_data","aom_img_metadata_free","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_error","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_error","aom_codec_get_frame","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_error","aom_codec_encode","aom_codec_error","aom_codec_get_cx_data","aom_img_metadata_free","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":1}
/*I'll create a fuzz driver that uses libaom to decode and re-encode video data, incorporating all the specified APIs. Here's the step-by-step implementation:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Open input as a FILE* using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Open output file for encoded data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    aom_codec_ctx_t decoder = {0};
    aom_codec_ctx_t encoder = {0};
    aom_codec_stream_info_t stream_info = {0};
    aom_image_t *img = nullptr;
    aom_metadata_t *metadata = nullptr;
    int ret_val = 0;
    
    do {
        // 3. Get AV1 decoder interface and check its name
        aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
        const char *iface_name = aom_codec_iface_name(decoder_iface);
        (void)iface_name; // Use variable to avoid unused warning
        
        // 4. Get codec capabilities
        aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
        (void)caps; // Use variable to avoid unused warning
        
        // 5. Initialize decoder
        aom_codec_err_t decode_err = aom_codec_dec_init_ver(
            &decoder, decoder_iface, nullptr, 0, AOM_DECODER_ABI_VERSION);
        
        if (decode_err != AOM_CODEC_OK) {
            const char *error_str = aom_codec_error(&decoder);
            (void)error_str; // Use variable to avoid unused warning
            ret_val = -1;
            break;
        }
        
        // 6. Try to peek stream info from input data
        fseek(in_file, 0, SEEK_SET);
        uint8_t *buffer = (uint8_t *)malloc(size);
        if (!buffer) {
            ret_val = -1;
            break;
        }
        
        size_t bytes_read = fread(buffer, 1, size, in_file);
        if (bytes_read == 0) {
            free(buffer);
            ret_val = -1;
            break;
        }
        
        aom_codec_err_t peek_err = aom_codec_peek_stream_info(
            decoder_iface, buffer, bytes_read, &stream_info);
        
        if (peek_err != AOM_CODEC_OK) {
            free(buffer);
            ret_val = -1;
            break;
        }
        
        // 7. Decode the input data
        aom_codec_err_t decode_result = aom_codec_decode(
            &decoder, buffer, bytes_read, nullptr);
        
        free(buffer);
        
        if (decode_result != AOM_CODEC_OK) {
            const char *error_str = aom_codec_error(&decoder);
            (void)error_str; // Use variable to avoid unused warning
            ret_val = -1;
            break;
        }
        
        // 8. Get decoded frame
        aom_codec_iter_t iter = nullptr;
        img = aom_codec_get_frame(&decoder, &iter);
        
        if (!img) {
            ret_val = -1;
            break;
        }
        
        // 9. Allocate metadata using aom_img_metadata_alloc
        // Use part of input data for metadata (first 64 bytes or less)
        size_t metadata_size = size > 64 ? 64 : size;
        uint32_t metadata_type = 1; // Example metadata type
        metadata = aom_img_metadata_alloc(
            metadata_type, 
            data, 
            metadata_size, 
            AOM_MIF_ANY_FRAME);
        
        if (metadata) {
            // Add metadata to image (using aom_img_add_metadata which is in the provided list)
            aom_img_add_metadata(
                img, 
                metadata->type, 
                metadata->payload, 
                metadata->sz, 
                metadata->insert_flag);
            
            // Free the allocated metadata
            aom_img_metadata_free(metadata);
            metadata = nullptr;
        }
        
        // 10. Initialize encoder
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        
        // Get encoder capabilities
        aom_codec_caps_t encoder_caps = aom_codec_get_caps(encoder_iface);
        (void)encoder_caps; // Use variable to avoid unused warning
        
        // Setup encoder configuration
        aom_codec_enc_cfg_t cfg;
        aom_codec_err_t enc_cfg_err = aom_codec_enc_config_default(
            encoder_iface, &cfg, 0); // 0 = good quality
        if (enc_cfg_err != AOM_CODEC_OK) {
            ret_val = -1;
            break;
        }
        
        // Set basic encoder parameters
        cfg.g_w = img->d_w;
        cfg.g_h = img->d_h;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30; // 30 fps
        cfg.rc_target_bitrate = 1000; // 1000 kbps
        cfg.g_pass = AOM_RC_ONE_PASS;
        cfg.g_lag_in_frames = 0;
        cfg.g_error_resilient = 0;
        
        // 11. Initialize encoder
        aom_codec_err_t encode_err = aom_codec_enc_init_ver(
            &encoder, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
        
        if (encode_err != AOM_CODEC_OK) {
            const char *error_str = aom_codec_error(&encoder);
            (void)error_str; // Use variable to avoid unused warning
            ret_val = -1;
            break;
        }
        
        // 12. Encode the decoded frame using aom_codec_encode
        aom_codec_err_t encode_result = aom_codec_encode(
            &encoder, 
            img, 
            0,  // pts (presentation timestamp)
            1,  // duration (1 frame)
            0); // flags (no special flags)
        
        if (encode_result != AOM_CODEC_OK) {
            const char *error_str = aom_codec_error(&encoder);
            (void)error_str; // Use variable to avoid unused warning
            ret_val = -1;
            break;
        }
        
        // 13. Get encoded data and write to output file
        aom_codec_iter_t enc_iter = nullptr;
        const aom_codec_cx_pkt_t *pkt;
        
        while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
        
        ret_val = 0;
        
    } while (0);
    
    // 14. Cleanup
    if (metadata) {
        aom_img_metadata_free(metadata);
    }
    
    if (img) {
        aom_img_free(img);
    }
    
    if (encoder.iface) {
        aom_codec_err_t destroy_err = aom_codec_destroy(&encoder);
        (void)destroy_err;
    }
    
    if (decoder.iface) {
        aom_codec_err_t destroy_err = aom_codec_destroy(&decoder);
        (void)destroy_err;
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return ret_val;
}