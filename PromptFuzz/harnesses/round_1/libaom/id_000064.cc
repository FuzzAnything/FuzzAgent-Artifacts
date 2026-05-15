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
//<ID> 138
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 1
//<Quality> {"density":24,"unique_branches":{"validate_img":[[990,35,990,59,0,0,4,0]]},"library_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_iface_name","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_alloc","aom_uleb_decode","aom_img_add_metadata","aom_img_num_metadata","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_error","aom_img_remove_metadata","aom_codec_destroy","aom_img_free","aom_codec_destroy","aom_uleb_decode"],"critical_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_iface_name","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_alloc","aom_uleb_decode","aom_img_add_metadata","aom_img_num_metadata","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_error","aom_img_remove_metadata","aom_codec_destroy","aom_img_free","aom_codec_destroy","aom_uleb_decode"],"visited":0}
/**/


// Include libaom headers

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get version string
    const char *version = aom_codec_version_str();
    if (version) {
        // Just reference it to avoid unused variable warning
        (void)version;
    }
    
    // 2. Get decoder and encoder interfaces
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    
    // 3. Get interface names
    const char *decoder_name = aom_codec_iface_name(decoder_iface);
    const char *encoder_name = aom_codec_iface_name(encoder_iface);
    (void)decoder_name;
    (void)encoder_name;
    
    // 4. Peek stream info from input data
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    
    // 5. Initialize decoder context
    aom_codec_ctx_t decoder_ctx;
    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    
    // Try to initialize decoder with default version
    aom_codec_err_t dec_init_result = aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, 
                                                           &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (dec_init_result == AOM_CODEC_OK) {
        // 6. Get stream info from decoder context
        aom_codec_stream_info_t decoder_stream_info;
        memset(&decoder_stream_info, 0, sizeof(decoder_stream_info));
        aom_codec_get_stream_info(&decoder_ctx, &decoder_stream_info);
        
        // 7. Decode the input data
        aom_codec_decode(&decoder_ctx, data, size, NULL);
        
        // 8. Get decoded frames
        aom_codec_iter_t iter = NULL;
        aom_image_t *decoded_img = NULL;
        while ((decoded_img = aom_codec_get_frame(&decoder_ctx, &iter)) != NULL) {
            // 9. Get plane dimensions
            for (int plane = 0; plane < 3; plane++) {
                int plane_w = aom_img_plane_width(decoded_img, plane);
                int plane_h = aom_img_plane_height(decoded_img, plane);
                (void)plane_w;
                (void)plane_h;
            }
            
            // 10. Allocate a new image for encoding
            aom_image_t *encode_img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 
                                                   decoded_img->d_w, decoded_img->d_h, 1);
            
            if (encode_img) {
                // 11. Try to decode a ULEB value from the input
                uint64_t uleb_value = 0;
                size_t uleb_length = 0;
                size_t uleb_available = size > 10 ? 10 : size;
                int uleb_result = aom_uleb_decode(data, uleb_available, 
                                                &uleb_value, &uleb_length);
                (void)uleb_result;
                (void)uleb_value;
                
                // 12. Add metadata to the image
                uint8_t metadata_data[] = {0x01, 0x02, 0x03};
                aom_img_add_metadata(encode_img, 1, metadata_data, 
                                   sizeof(metadata_data), AOM_MIF_ANY_FRAME);
                
                // 13. Get number of metadata entries
                size_t metadata_count = aom_img_num_metadata(encode_img);
                (void)metadata_count;
                
                // 14. Initialize encoder context
                aom_codec_ctx_t encoder_ctx;
                aom_codec_enc_cfg_t enc_cfg;
                
                // Get default encoder configuration
                aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
                
                // Set dimensions
                enc_cfg.g_w = encode_img->d_w;
                enc_cfg.g_h = encode_img->h;
                enc_cfg.g_bit_depth = AOM_BITS_8;
                enc_cfg.g_input_bit_depth = 8;
                
                // Initialize encoder
                aom_codec_err_t enc_init_result = aom_codec_enc_init_ver(&encoder_ctx, 
                                                                       encoder_iface, 
                                                                       &enc_cfg, 
                                                                       0, 
                                                                       AOM_ENCODER_ABI_VERSION);
                
                if (enc_init_result == AOM_CODEC_OK) {
                    // 15. Encode the image
                    aom_codec_encode(&encoder_ctx, encode_img, 0, 1, 0);
                    
                    // 16. Get encoded data
                    aom_codec_iter_t enc_iter = NULL;
                    const aom_codec_cx_pkt_t *pkt = NULL;
                    while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter)) != NULL) {
                        // Process the packet if needed
                        (void)pkt;
                    }
                    
                    // 17. Get error message (should be NULL if no error)
                    const char *error_msg = aom_codec_error(&encoder_ctx);
                    (void)error_msg;
                    
                    // 18. Remove metadata from image
                    aom_img_remove_metadata(encode_img);
                    
                    // Destroy encoder
                    aom_codec_destroy(&encoder_ctx);
                }
                
                // 19. Free the allocated image
                aom_img_free(encode_img);
            }
        }
        
        // 20. Destroy decoder
        aom_codec_destroy(&decoder_ctx);
    }
    
    // 21. Also test ULEB decode separately with the entire input
    if (size > 0) {
        uint64_t uleb_value2 = 0;
        size_t uleb_length2 = 0;
        aom_uleb_decode(data, size, &uleb_value2, &uleb_length2);
    }
    
    return 0;
}