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
//<ID> 149
//<Prompt> []
/*<Combination>: [
*/
//<score> 63, nr_unique_branch: 2
//<Quality> {"density":21,"unique_branches":{"encoder_init":[[34,31,34,32,5,0,4,1]],"is_fpmt_config":[[1144,7,1144,35,0,0,4,0]]},"library_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_destroy","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_uleb_decode","aom_codec_get_caps","aom_codec_av1_cx","aom_codec_get_caps","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_uleb_decode","aom_codec_get_caps","aom_codec_av1_cx","aom_codec_get_caps","aom_codec_destroy"],"visited":0}
/**/


// Include necessary headers (these would be provided by libaom)
// Assuming standard libaom headers are available
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // 1. Get codec version
    const char *version_str = aom_codec_version_str();
    (void)version_str; // Use it to avoid unused variable warning
    
    // 2. Setup decoder
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_ctx_t decoder_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.allow_lowbitdepth = 1;
    
    // Initialize decoder
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, 0, 
                                                 AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        // If decoder init fails, try to cleanup and return
        aom_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    // 3. Decode the input data
    err = aom_codec_decode(&decoder_ctx, data, size, nullptr);
    
    // 4. Get stream info even if decode failed
    aom_codec_stream_info_t stream_info = {0};
    aom_codec_get_stream_info(&decoder_ctx, &stream_info);
    
    // 5. Try to get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_image = nullptr;
    
    // Get first frame if available
    decoded_image = aom_codec_get_frame(&decoder_ctx, &iter);
    
    // 6. Setup encoder if we got a valid frame
    aom_codec_ctx_t encoder_ctx;
    aom_codec_enc_cfg_t enc_cfg;
    
    if (decoded_image) {
        // Get encoder interface
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        
        // Get default encoder config
        unsigned int usage = 0; // Good quality mode
        err = aom_codec_enc_config_default(encoder_iface, &enc_cfg, usage);
        
        if (err == AOM_CODEC_OK) {
            // Set encoder dimensions from decoded image
            enc_cfg.g_w = decoded_image->d_w;
            enc_cfg.g_h = decoded_image->d_h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.g_threads = 1;
            enc_cfg.g_lag_in_frames = 0;
            enc_cfg.rc_end_usage = AOM_CBR;
            enc_cfg.rc_target_bitrate = 1000;
            enc_cfg.kf_mode = AOM_KF_AUTO;
            enc_cfg.kf_max_dist = 30;
            
            // Initialize encoder
            err = aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, 0,
                                         AOM_ENCODER_ABI_VERSION);
            
            if (err == AOM_CODEC_OK) {
                // Set an encoder option
                aom_codec_set_option(&encoder_ctx, "cpu-used", "8");
                
                // 7. Create input image for encoder
                aom_image_t input_image;
                aom_img_fmt_t fmt = decoded_image->fmt;
                
                // Allocate image buffer
                aom_image_t *img = aom_img_alloc(&input_image, fmt, 
                                                decoded_image->d_w, 
                                                decoded_image->d_h, 
                                                1);
                
                if (img) {
                    // Copy decoded image data
                    for (int plane = 0; plane < 3; plane++) {
                        int width = aom_img_plane_width(img, plane);
                        int height = aom_img_plane_height(img, plane);
                        
                        if (plane < 3 && decoded_image->planes[plane]) {
                            size_t plane_size = width * height;
                            if (plane_size > 0) {
                                memcpy(img->planes[plane], decoded_image->planes[plane],
                                      plane_size);
                            }
                        }
                    }
                    
                    // Get metadata count (if any)
                    size_t metadata_count = aom_img_num_metadata(decoded_image);
                    (void)metadata_count;
                    
                    // 8. Encode the image
                    aom_codec_encode(&encoder_ctx, img, 0, 1, 0);
                    
                    // 9. Get encoded data
                    const aom_codec_cx_pkt_t *pkt = nullptr;
                    aom_codec_iter_t enc_iter = nullptr;
                    
                    while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
                        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                            // Got encoded frame packet
                            break;
                        }
                    }
                    
                    // Cleanup allocated image
                    aom_img_free(img);
                }
                
                // Destroy encoder
                aom_codec_destroy(&encoder_ctx);
            }
        }
    }
    
    // 10. Use aom_uleb_decode on input data
    if (size > 0) {
        uint64_t uleb_value = 0;
        size_t uleb_length = 0;
        
        // Try to decode first ULEB value from input
        aom_uleb_decode(data, size, &uleb_value, &uleb_length);
    }
    
    // 11. Get codec capabilities
    aom_codec_caps_t dec_caps = aom_codec_get_caps(decoder_iface);
    aom_codec_iface_t *encoder_iface_test = aom_codec_av1_cx();
    aom_codec_caps_t enc_caps = aom_codec_get_caps(encoder_iface_test);
    (void)dec_caps;
    (void)enc_caps;
    
    // 12. Cleanup decoder
    aom_codec_destroy(&decoder_ctx);
    
    return 0;
}