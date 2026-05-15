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
//<ID> 248
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_destroy","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_wrap","aom_img_add_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_codec_control","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_wrap","aom_img_add_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_control","aom_codec_control","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Too small for meaningful processing
    
    // Step 1: Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Step 2: Initialize decoder to get metadata from encoded stream
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (aom_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read some data for decoding
    uint8_t *decode_buffer = (uint8_t *)malloc(size);
    if (!decode_buffer) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(decode_buffer, 1, size, in_file);
    rewind(in_file);
    
    // Step 3: Decode to get image
    if (aom_codec_decode(&decoder, decode_buffer, bytes_read, NULL) == AOM_CODEC_OK) {
        aom_codec_iter_t iter = NULL;
        aom_image_t *decoded_img = aom_codec_get_frame(&decoder, &iter);
        
        if (decoded_img) {
            // Step 4: Get metadata from decoded image
            size_t num_metadata = aom_img_num_metadata(decoded_img);
            for (size_t i = 0; i < num_metadata; i++) {
                const aom_metadata_t *metadata = aom_img_get_metadata(decoded_img, i);
                // Metadata retrieved, could be used further
            }
            
            // Step 5: Create wrapped image for re-encoding
            // Use a portion of input data as image data
            size_t img_data_size = decoded_img->w * decoded_img->h * 3 / 2;
            uint8_t *img_buffer = (uint8_t *)malloc(img_data_size);
            if (img_buffer && size >= img_data_size) {
                memcpy(img_buffer, data, std::min(size, img_data_size));
                
                aom_image_t wrapped_img;
                aom_img_wrap(&wrapped_img, AOM_IMG_FMT_I420, 
                            decoded_img->w, decoded_img->h, 
                            1, img_buffer);
                
                // Step 6: Add metadata to wrapped image
                uint32_t metadata_type = 1; // Example type
                const uint8_t metadata_data[] = {0x01, 0x02, 0x03};
                aom_img_add_metadata(&wrapped_img, metadata_type, 
                                    metadata_data, sizeof(metadata_data),
                                    AOM_MIF_KEY_FRAME);
                
                // Step 7: Initialize encoder with default config
                aom_codec_ctx_t encoder;
                aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
                
                aom_codec_enc_cfg_t enc_cfg;
                if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
                    enc_cfg.g_w = decoded_img->w;
                    enc_cfg.g_h = decoded_img->h;
                    enc_cfg.rc_target_bitrate = 1000;
                    
                    if (aom_codec_enc_init(&encoder, encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
                        // Step 8: Use encoder control API
                        // Set quality control
                        aom_codec_control(&encoder, AOME_SET_CPUUSED, 8);
                        aom_codec_control(&encoder, AOME_SET_CQ_LEVEL, 32);
                        
                        // Step 9: Set output buffer for encoded data
                        aom_fixed_buf_t output_buf;
                        size_t output_size = 1024 * 1024; // 1MB buffer
                        output_buf.buf = malloc(output_size);
                        output_buf.sz = output_size;
                        
                        if (output_buf.buf) {
                            aom_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);
                            
                            // Step 10: Encode the wrapped image
                            aom_codec_encode(&encoder, &wrapped_img, 0, 1, 0);
                            
                            // Iterate through encoded packets
                            aom_codec_iter_t enc_iter = NULL;
                            const aom_codec_cx_pkt_t *pkt;
                            while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                                // Process encoded packets if needed
                            }
                            
                            free(output_buf.buf);
                        }
                        
                        aom_codec_destroy(&encoder);
                    }
                }
                
                // Clean wrapped image resources
                aom_img_free(&wrapped_img);
                free(img_buffer);
            }
        }
    }
    
    // Step 11: Cleanup
    free(decode_buffer);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}