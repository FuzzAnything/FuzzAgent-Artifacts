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
//<ID> 180
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 0
//<Quality> {"density":25,"unique_branches":{},"library_calls":["aom_uleb_decode","aom_uleb_encode","aom_uleb_encode_fixed_size","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_alloc_with_border","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_plane_height","aom_img_plane_height","aom_img_plane_width","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_uleb_encode","aom_uleb_encode_fixed_size","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_alloc_with_border","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_metadata_free","aom_img_plane_height","aom_img_plane_height","aom_img_plane_width","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // ULEB encode/decode operations
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    uint8_t encoded_buffer[100] = {0};
    size_t encoded_size = 0;
    
    // 1. Try to decode ULEB from input data
    if (aom_uleb_decode(data, size, &uleb_value, &uleb_length) == 0) {
        // 2. Encode the decoded value
        aom_uleb_encode(uleb_value, sizeof(encoded_buffer), encoded_buffer, &encoded_size);
        
        // 3. Encode with fixed size
        aom_uleb_encode_fixed_size(uleb_value, sizeof(encoded_buffer), 10, 
                                   encoded_buffer, &encoded_size);
    }
    
    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // 4. Get decoder capabilities
    aom_codec_caps_t decoder_caps = aom_codec_get_caps(decoder_iface);
    
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    // 5. Initialize decoder
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION) 
        == AOM_CODEC_OK) {
        
        // 6. Get stream information
        aom_codec_stream_info_t stream_info = {0};
        if (aom_codec_get_stream_info(&decoder, &stream_info) == AOM_CODEC_OK) {
            // Use stream information
            unsigned int width = stream_info.w;
            unsigned int height = stream_info.h;
            
            // 7. Try to decode the input data
            aom_codec_decode(&decoder, data, size, nullptr);
            
            // 8. Get decoded frames
            aom_codec_iter_t iter = nullptr;
            aom_image_t *decoded_img = nullptr;
            while ((decoded_img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
                
                // 9. Allocate image with border for processing
                aom_image_t processed_img;
                if (aom_img_alloc_with_border(&processed_img, decoded_img->fmt, 
                                             decoded_img->d_w, decoded_img->d_h, 
                                             32, 32, 32) != nullptr) {
                    
                    // 10. Get number of metadata
                    size_t num_metadata = aom_img_num_metadata(decoded_img);
                    
                    // 11. Get and process metadata
                    for (size_t i = 0; i < num_metadata; i++) {
                        const aom_metadata_t *metadata = aom_img_get_metadata(decoded_img, i);
                        if (metadata) {
                            // 12. Allocate new metadata
                            aom_metadata_t *new_metadata = aom_img_metadata_alloc(
                                metadata->type, metadata->payload, metadata->sz, AOM_MIF_ANY_FRAME);
                            
                            // 13. Free allocated metadata
                            aom_img_metadata_free(new_metadata);
                        }
                    }
                    
                    // 14. Get plane height
                    int plane_height = aom_img_plane_height(decoded_img, 0);
                    
                    // Copy decoded image to processed image
                    for (int plane = 0; plane < 3; plane++) {
                        int plane_h = aom_img_plane_height(decoded_img, plane);
                        int plane_w = aom_img_plane_width(decoded_img, plane);
                        for (int y = 0; y < plane_h; y++) {
                            memcpy(processed_img.planes[plane] + y * processed_img.stride[plane],
                                   decoded_img->planes[plane] + y * decoded_img->stride[plane],
                                   plane_w);
                        }
                    }
                    
                    // Initialize encoder
                    aom_codec_ctx_t encoder;
                    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
                    
                    aom_codec_enc_cfg_t enc_cfg;
                    
                    // 15. Get default encoder config
                    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
                        enc_cfg.g_w = processed_img.d_w;
                        enc_cfg.g_h = processed_img.d_h;
                        enc_cfg.g_threads = 1;
                        enc_cfg.g_lag_in_frames = 0;
                        
                        // 16. Initialize encoder
                        if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, 
                                                  AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
                            
                            // 17. Encode the processed image
                            if (aom_codec_encode(&encoder, &processed_img, 0, 1, 0) == AOM_CODEC_OK) {
                                
                                // 18. Get encoded data
                                aom_codec_iter_t enc_iter = nullptr;
                                const aom_codec_cx_pkt_t *pkt = nullptr;
                                while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                                    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                                        // Write to output file
                                        fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                                    }
                                }
                            }
                            
                            // 19. Destroy encoder
                            aom_codec_destroy(&encoder);
                        }
                    }
                    
                    // 20. Free processed image
                    aom_img_free(&processed_img);
                }
            }
        }
        
        // 21. Destroy decoder
        aom_codec_destroy(&decoder);
    }
    
    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}