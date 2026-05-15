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
//<ID> 133
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_set_rect","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_get_global_headers","aom_codec_set_cx_data_buf","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_set_rect","aom_img_metadata_alloc","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_get_global_headers","aom_codec_set_cx_data_buf","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations

    // Create a FILE* to read the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Initialize codec interface for AV1 decoder
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 1;
    
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Try to decode the input data
    aom_codec_decode(&decoder, data, size, NULL);

    // Get frame from decoder
    aom_codec_iter_t iter = NULL;
    aom_image_t* decoded_image = aom_codec_get_frame(&decoder, &iter);

    if (decoded_image) {
        // Use aom_img_set_rect to set a rectangular region
        unsigned int rect_x = data[0] % (decoded_image->d_w / 2);
        unsigned int rect_y = data[1] % (decoded_image->d_h / 2);
        unsigned int rect_w = 8 + (data[2] % 16);
        unsigned int rect_h = 8 + (data[3] % 16);
        unsigned int border = data[4] % 4;
        
        if (rect_x + rect_w <= decoded_image->d_w && rect_y + rect_h <= decoded_image->d_h) {
            aom_img_set_rect(decoded_image, rect_x, rect_y, rect_w, rect_h, border);
        }

        // Create metadata
        uint32_t metadata_type = data[5];
        size_t metadata_size = data[6] % 64;
        std::vector<uint8_t> metadata_data(metadata_size);
        
        if (metadata_size > 0 && size > 50) {
            size_t copy_size = std::min(metadata_size, size - 50);
            memcpy(metadata_data.data(), data + 50, copy_size);
        }
        
        aom_metadata_insert_flags_t insert_flag = static_cast<aom_metadata_insert_flags_t>(data[7] % 6);
        aom_metadata_t* metadata = aom_img_metadata_alloc(metadata_type, 
                                                        metadata_data.data(), 
                                                        metadata_data.size(), 
                                                        insert_flag);
        
        if (metadata) {
            // Note: Metadata is allocated but not attached to image since 
            // aom_img_add_metadata is not in our required API list
            aom_img_metadata_free(metadata);
        }
    }

    // Initialize encoder
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    aom_codec_ctx_t encoder;
    aom_codec_enc_cfg_t enc_cfg;
    
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
        enc_cfg.g_w = 64;
        enc_cfg.g_h = 64;
        enc_cfg.g_threads = 1;
        enc_cfg.rc_end_usage = AOM_Q;
        
        if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
            
            // Set encoder option
            const char* option_name = "cpu-used";
            char option_value[32];
            snprintf(option_value, sizeof(option_value), "%d", data[8] % 9);
            aom_codec_set_option(&encoder, option_name, option_value);
            
            // Get global headers
            aom_fixed_buf_t* global_headers = aom_codec_get_global_headers(&encoder);
            if (global_headers) {
                free(global_headers->buf);
                free(global_headers);
            }
            
            // Prepare output buffer for encoded data
            aom_fixed_buf_t output_buf;
            const size_t buf_size = 1024 * 1024; // 1MB buffer
            output_buf.buf = malloc(buf_size);
            output_buf.sz = buf_size;
            
            if (output_buf.buf) {
                // Set output buffer for encoder
                unsigned int pad_before = data[9] % 16;
                unsigned int pad_after = data[10] % 16;
                aom_codec_set_cx_data_buf(&encoder, &output_buf, pad_before, pad_after);
                
                free(output_buf.buf);
            }
            
            // Clean up encoder
            aom_codec_destroy(&encoder);
        }
    }

    // Clean up
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}