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
//<ID> 61
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"aom_obu_type_to_string":[[196,5,196,21,0,0,4,0]]},"library_calls":["aom_codec_version_str","aom_obu_type_to_string","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_alloc","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_img_remove_metadata","aom_img_free"],"critical_calls":["aom_codec_version_str","aom_obu_type_to_string","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_alloc","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_img_free","aom_codec_destroy","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_img_remove_metadata","aom_img_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // 1. Get codec version string
    const char *version_str = aom_codec_version_str();
    
    // 2. Try to extract OBU type from first byte
    if (size > 0) {
        // Extract OBU type from the first byte (bits 3-7 in AV1 OBU header)
        uint8_t first_byte = data[0];
        int obu_type = (first_byte >> 3) & 0x1F;
        const char *obu_type_str = aom_obu_type_to_string(static_cast<OBU_TYPE>(obu_type));
    }
    
    // 3. Create decoder interface
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    const char *decoder_name = aom_codec_iface_name(decoder_iface);
    
    // 4. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, 
                                                 AOM_DECODER_ABI_VERSION);
    
    // 5. Peek stream info from input data
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    
    // 6. Try to decode the input data
    if (err == AOM_CODEC_OK) {
        aom_codec_decode(&decoder, data, size, nullptr);
        
        // 7. Get stream info after decoding
        aom_codec_get_stream_info(&decoder, &stream_info);
        
        // 8. Get decoded frames
        aom_codec_iter_t iter = nullptr;
        aom_image_t *decoded_frame = nullptr;
        
        while ((decoded_frame = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
            // 9. Get plane dimensions
            for (int plane = 0; plane < 3; plane++) {
                int plane_w = aom_img_plane_width(decoded_frame, plane);
                int plane_h = aom_img_plane_height(decoded_frame, plane);
            }
            
            // 10. Allocate a new image for testing
            aom_image_t *test_image = nullptr;
            if (decoded_frame->d_w > 0 && decoded_frame->d_h > 0) {
                test_image = aom_img_alloc(nullptr, decoded_frame->fmt, 
                                          decoded_frame->d_w, decoded_frame->d_h, 16);
            }
            
            // 11. Remove metadata if image exists
            if (test_image) {
                aom_img_remove_metadata(test_image);
                
                // 12. Create encoder interface
                aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
                const char *encoder_name = aom_codec_iface_name(encoder_iface);
                
                // 13. Setup encoder configuration
                aom_codec_enc_cfg_t enc_cfg;
                unsigned int usage = 0; // Good quality
                aom_codec_enc_config_default(encoder_iface, &enc_cfg, usage);
                
                // Set basic parameters
                enc_cfg.g_w = test_image->d_w;
                enc_cfg.g_h = test_image->d_h;
                enc_cfg.g_timebase.num = 1;
                enc_cfg.g_timebase.den = 30;
                enc_cfg.g_threads = 1;
                enc_cfg.g_pass = AOM_RC_ONE_PASS;
                enc_cfg.g_lag_in_frames = 0;
                enc_cfg.rc_end_usage = AOM_Q;
                enc_cfg.rc_min_quantizer = 2;
                enc_cfg.rc_max_quantizer = 58;
                
                // 14. Initialize encoder
                aom_codec_ctx_t encoder;
                aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, 
                                      AOM_ENCODER_ABI_VERSION);
                
                // 15. Set encoder configuration
                aom_codec_enc_config_set(&encoder, &enc_cfg);
                
                // 16. Encode the test image
                aom_codec_encode(&encoder, test_image, 0, 1, 0);
                
                // 17. Get encoded data
                aom_codec_iter_t enc_iter = nullptr;
                const aom_codec_cx_pkt_t *pkt = nullptr;
                
                while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                    // Process packet if needed
                }
                
                // 18. Destroy encoder
                aom_codec_destroy(&encoder);
                
                // 19. Free test image
                aom_img_free(test_image);
            }
        }
        
        // 20. Destroy decoder
        aom_codec_destroy(&decoder);
    }
    
    // Additional test: Try to create a small image and encode it
    if (stream_info.w > 0 && stream_info.h > 0) {
        const unsigned int test_w = 64;
        const unsigned int test_h = 64;
        
        aom_image_t *img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, test_w, test_h, 16);
        if (img) {
            // Initialize image with some data
            for (int plane = 0; plane < 3; plane++) {
                unsigned char *plane_buf = img->planes[plane];
                int stride = img->stride[plane];
                int plane_h = aom_img_plane_height(img, plane);
                int plane_w = aom_img_plane_width(img, plane);
                
                if (plane_buf) {
                    for (int y = 0; y < plane_h; y++) {
                        for (int x = 0; x < plane_w; x++) {
                            plane_buf[y * stride + x] = (x + y) % 256;
                        }
                    }
                }
            }
            
            aom_img_remove_metadata(img);
            aom_img_free(img);
        }
    }
    
    return 0;
}