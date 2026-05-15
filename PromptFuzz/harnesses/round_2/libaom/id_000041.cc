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
//<ID> 116
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["aom_img_alloc_with_border","aom_img_free","aom_codec_av1_dx","aom_img_free","aom_codec_dec_init_ver","aom_codec_error_detail","aom_codec_err_to_string","aom_img_free","aom_codec_destroy","aom_codec_decode","aom_codec_get_frame","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_num_metadata","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_encode","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_img_alloc_with_border","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error_detail","aom_codec_err_to_string","aom_codec_decode","aom_codec_get_frame","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_num_metadata","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_encode","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Minimum size for meaningful fuzzing

    // Event: Decode AV1 video from input data, process frames with metadata,
    // and set up encoding output buffer

    aom_codec_ctx_t decoder;
    aom_codec_ctx_t encoder;
    aom_codec_iter_t iter = nullptr;
    aom_image_t *frame = nullptr;
    aom_fixed_buf_t output_buf;
    int ret = 0;
    
    // 1. Create image with border for potential processing
    aom_image_t img_border;
    // Use a small fixed size since we don't know image dimensions from fuzzed data
    aom_image_t *border_img = aom_img_alloc_with_border(&img_border, 
                                                        AOM_IMG_FMT_I420,
                                                        128, 96,  // d_w, d_h
                                                        32,       // align
                                                        64,       // size_align
                                                        8);       // border
    
    if (!border_img) {
        // Clean up partial allocations
        aom_img_free(border_img);
        return 0;
    }

    // 2. Initialize decoder
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        aom_img_free(border_img);
        return 0;
    }

    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    aom_codec_err_t decode_init_result = aom_codec_dec_init_ver(&decoder, 
                                                                 decoder_iface, 
                                                                 &dec_cfg, 
                                                                 0, 
                                                                 AOM_DECODER_ABI_VERSION);
    
    // Use error APIs
    const char *error_detail = aom_codec_error_detail(&decoder);
    const char *error_string = aom_codec_err_to_string(decode_init_result);
    
    if (decode_init_result != AOM_CODEC_OK) {
        aom_img_free(border_img);
        aom_codec_destroy(&decoder);
        return 0;
    }

    // 3. Try to decode the input data
    aom_codec_decode(&decoder, data, size, nullptr);
    
    // 4. Get decoded frames and process them
    while ((frame = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        // Create metadata for the frame
        // Use first 4 bytes of data as metadata type if available
        uint32_t metadata_type = (size >= 4) ? *(reinterpret_cast<const uint32_t*>(data)) : 1234;
        
        // Use a portion of input data as metadata payload
        size_t metadata_size = (size > 50) ? 50 : size/2;
        const uint8_t *metadata_data = (metadata_size > 0) ? data : nullptr;
        
        // Allocate metadata structure
        aom_metadata_t *metadata = aom_img_metadata_alloc(metadata_type,
                                                          metadata_data,
                                                          metadata_size,
                                                          AOM_MIF_ANY_FRAME);
        
        if (metadata) {
            // Add metadata to the frame
            int add_meta_result = aom_img_add_metadata(frame,
                                                       metadata_type,
                                                       metadata_data,
                                                       metadata_size,
                                                       AOM_MIF_ANY_FRAME);
            
            // Get number of metadata items in the image
            size_t num_metadata = aom_img_num_metadata(frame);
            
            // Free the metadata structure (was copied by aom_img_add_metadata)
            aom_img_metadata_free(metadata);
        }
        
        // Only process one frame to avoid infinite loops
        break;
    }

    // 5. Initialize encoder to use set_cx_data_buf
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    if (encoder_iface) {
        aom_codec_enc_cfg_t enc_cfg;
        aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        
        // Set minimal configuration for fuzzing
        enc_cfg.g_w = 128;
        enc_cfg.g_h = 96;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 1000;
        enc_cfg.g_threads = 1;
        enc_cfg.g_pass = AOM_RC_ONE_PASS;
        enc_cfg.g_lag_in_frames = 0;
        
        aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
        
        // 6. Set up output buffer for encoded data
        output_buf.buf = malloc(4096); // Small buffer for fuzzing
        output_buf.sz = 4096;
        
        if (output_buf.buf) {
            // Set the output buffer with padding
            aom_codec_set_cx_data_buf(&encoder, &output_buf, 16, 16);
            
            // Try to encode the border image
            if (border_img) {
                aom_codec_encode(&encoder, border_img, 0, 1, 0);
            }
        }
    }

    // 7. Cleanup
    aom_img_free(border_img);
    aom_codec_destroy(&decoder);
    aom_codec_destroy(&encoder);
    
    if (output_buf.buf) {
        free(output_buf.buf);
    }

    return 0;
}