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
//<ID> 125
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 0
//<Quality> {"density":25,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_error_detail","aom_codec_error","aom_codec_peek_stream_info","aom_codec_destroy","aom_codec_decode","aom_codec_destroy","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_img_remove_metadata","aom_img_alloc","aom_img_alloc_with_border","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_error","aom_codec_error_detail","aom_codec_destroy","aom_img_free","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_img_remove_metadata","aom_img_alloc","aom_img_alloc_with_border","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_error","aom_codec_error_detail","aom_codec_destroy","aom_img_free","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // 1. Print version information
    const char *version_str = aom_codec_version_str();
    (void)version_str; // Prevent unused variable warning

    // 2. Initialize decoder
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error_detail(&decoder);
        const char *error_str = aom_codec_error(&decoder);
        (void)error_detail;
        (void)error_str;
        return 0;
    }

    // 3. Peek stream info
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    res = aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }

    // 4. Decode the input data
    res = aom_codec_decode(&decoder, data, size, nullptr);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }

    // 5. Get decoded frame
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_frame = aom_codec_get_frame(&decoder, &iter);
    
    if (decoded_frame) {
        // 6. Use various image APIs
        int plane_width = aom_img_plane_width(decoded_frame, 0);
        int plane_height = aom_img_plane_height(decoded_frame, 0);
        size_t metadata_count = aom_img_num_metadata(decoded_frame);
        (void)plane_width;
        (void)plane_height;
        (void)metadata_count;
        
        // 7. Remove metadata if present
        aom_img_remove_metadata(decoded_frame);
        
        // 8. Allocate new images for encoding
        aom_image_t *img_no_border = aom_img_alloc(nullptr, decoded_frame->fmt, 
                                                   decoded_frame->d_w, 
                                                   decoded_frame->d_h, 16);
        
        aom_image_t *img_with_border = aom_img_alloc_with_border(nullptr, 
                                                                decoded_frame->fmt,
                                                                decoded_frame->d_w,
                                                                decoded_frame->d_h,
                                                                16, 16, 32);
        
        if (img_no_border && img_with_border && decoded_frame->fmt == img_no_border->fmt) {
            // 9. Initialize encoder
            aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
            aom_codec_ctx_t encoder;
            aom_codec_enc_cfg_t enc_cfg;
            
            // Get default encoder configuration
            res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
            if (res == AOM_CODEC_OK) {
                // Set some basic parameters
                enc_cfg.g_w = decoded_frame->d_w;
                enc_cfg.g_h = decoded_frame->d_h;
                enc_cfg.g_threads = 1;
                enc_cfg.rc_target_bitrate = 1000;
                enc_cfg.g_error_resilient = 1;
                
                // Initialize encoder
                res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, 
                                           AOM_ENCODER_ABI_VERSION);
                
                if (res == AOM_CODEC_OK) {
                    // 10. Encode the decoded frame (using no border image as input)
                    // Copy frame data to allocated image (simplified - in reality would need proper copy)
                    res = aom_codec_encode(&encoder, img_no_border, 0, 1, 0);
                    
                    if (res == AOM_CODEC_OK) {
                        // 11. Get encoded data
                        aom_codec_iter_t enc_iter = nullptr;
                        const aom_codec_cx_pkt_t *pkt;
                        
                        while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                            // Process packet if needed
                            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                                // Frame packet received
                                (void)pkt->data.frame.buf; // Mark as used
                            }
                        }
                        
                        // Get encoder error if any
                        const char *enc_error = aom_codec_error(&encoder);
                        const char *enc_error_detail = aom_codec_error_detail(&encoder);
                        (void)enc_error;
                        (void)enc_error_detail;
                    }
                    
                    // 12. Destroy encoder
                    aom_codec_destroy(&encoder);
                }
            }
        }
        
        // 13. Free allocated images
        if (img_no_border) {
            aom_img_free(img_no_border);
        }
        if (img_with_border) {
            aom_img_free(img_with_border);
        }
    }
    
    // 14. Destroy decoder
    aom_codec_destroy(&decoder);
    
    return 0;
}