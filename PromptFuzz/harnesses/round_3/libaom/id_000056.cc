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
//<ID> 163
//<Prompt> []
/*<Combination>: [
*/
//<score> 31, nr_unique_branch: 0
//<Quality> {"density":31,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_dec_init_ver","aom_codec_destroy","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_destroy","aom_codec_get_frame","aom_img_alloc_with_border","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_flip","aom_uleb_encode","aom_uleb_decode","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_remove_metadata","aom_img_metadata_free","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_enc_config_set","aom_codec_get_global_headers","aom_codec_encode","aom_codec_get_preview_frame","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_av1_cx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_alloc_with_border","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_flip","aom_uleb_encode","aom_uleb_decode","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_remove_metadata","aom_img_metadata_free","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_enc_config_set","aom_codec_get_global_headers","aom_codec_encode","aom_codec_get_preview_frame","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Initialize variables
    aom_codec_ctx_t decoder_ctx = {};
    aom_codec_ctx_t encoder_ctx = {};
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    
    // 1. Decoder initialization
    aom_codec_dec_cfg_t dec_cfg = {};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, 0, 
                                                  AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder_ctx);
        return 0;
    }

    // 2. Peek stream info
    aom_codec_stream_info_t stream_info = {};
    aom_codec_peek_stream_info(decoder_iface, data, size < 100 ? size : 100, &stream_info);

    // 3. Decode the input data
    res = aom_codec_decode(&decoder_ctx, data, size, nullptr);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder_ctx);
        return 0;
    }

    // 4. Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t* decoded_img = aom_codec_get_frame(&decoder_ctx, &iter);
    
    if (decoded_img) {
        // 5. Process the decoded image
        
        // Create a new image with border using decoded image dimensions
        aom_image_t* bordered_img = aom_img_alloc_with_border(nullptr, AOM_IMG_FMT_I420,
                                                              decoded_img->d_w, decoded_img->d_h,
                                                              16, 1, 32);
        
        // Create another image without border
        aom_image_t* regular_img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420,
                                                 decoded_img->d_w, decoded_img->d_h, 16);
        
        if (bordered_img && regular_img) {
            // 6. Get plane dimensions
            int plane_width = aom_img_plane_width(decoded_img, 0);
            int plane_height = aom_img_plane_height(decoded_img, 0);
            
            // 7. Flip the image
            aom_img_flip(decoded_img);
            
            // 8. Test ULEB encode/decode
            uint64_t test_value = size;
            uint8_t coded_buffer[10];
            size_t coded_size = 0;
            aom_uleb_encode(test_value, sizeof(coded_buffer), coded_buffer, &coded_size);
            
            uint64_t decoded_value = 0;
            size_t decoded_length = 0;
            aom_uleb_decode(coded_buffer, coded_size, &decoded_value, &decoded_length);
            
            // 9. Test metadata operations
            uint8_t metadata_data[] = {0x01, 0x02, 0x03};
            aom_metadata_t* metadata = aom_img_metadata_alloc(1, metadata_data, 3, 
                                                              AOM_MIF_KEY_FRAME);
            
            if (metadata) {
                // Add metadata to image
                aom_img_add_metadata(bordered_img, metadata->type, metadata->payload, 
                                     metadata->sz, metadata->insert_flag);
                
                // Remove metadata
                aom_img_remove_metadata(bordered_img);
                
                // Free metadata
                aom_img_metadata_free(metadata);
            }
            
            // 10. Encoder initialization
            aom_codec_enc_cfg_t enc_cfg = {};
            res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
            
            if (res == AOM_CODEC_OK) {
                enc_cfg.g_w = decoded_img->d_w;
                enc_cfg.g_h = decoded_img->d_h;
                enc_cfg.g_timebase.num = 1;
                enc_cfg.g_timebase.den = 30;
                enc_cfg.rc_target_bitrate = 1000;
                enc_cfg.g_pass = AOM_RC_ONE_PASS;
                enc_cfg.g_lag_in_frames = 0;
                
                res = aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, 0,
                                            AOM_ENCODER_ABI_VERSION);
                
                if (res == AOM_CODEC_OK) {
                    // 11. Set encoder option
                    aom_codec_set_option(&encoder_ctx, "cpu-used", "1");
                    
                    // 12. Set encoder config (redundant but for API coverage)
                    aom_codec_enc_config_set(&encoder_ctx, &enc_cfg);
                    
                    // 13. Get global headers
                    aom_fixed_buf_t* global_headers = aom_codec_get_global_headers(&encoder_ctx);
                    if (global_headers) {
                        free(global_headers->buf);
                        free(global_headers);
                    }
                    
                    // 14. Encode the frame
                    res = aom_codec_encode(&encoder_ctx, decoded_img, 0, 1, 0);
                    
                    if (res == AOM_CODEC_OK) {
                        // 15. Get preview frame
                        const aom_image_t* preview = aom_codec_get_preview_frame(&encoder_ctx);
                        
                        // 16. Get encoded data
                        aom_codec_iter_t enc_iter = nullptr;
                        const aom_codec_cx_pkt_t* pkt = nullptr;
                        
                        while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
                            // Process the packet if needed
                        }
                    }
                    
                    // 17. Destroy encoder
                    aom_codec_destroy(&encoder_ctx);
                }
            }
            
            // 18. Free allocated images
            aom_img_free(bordered_img);
            aom_img_free(regular_img);
        }
    }
    
    // 19. Destroy decoder
    aom_codec_destroy(&decoder_ctx);
    
    return 0;
}