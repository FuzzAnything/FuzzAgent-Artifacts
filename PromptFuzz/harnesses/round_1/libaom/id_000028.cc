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
//<ID> 63
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 9
//<Quality> {"density":12,"unique_branches":{"sad32x32":[[23,15,23,21,0,0,4,0],[23,15,23,21,0,0,4,1]],"av1_ml_prune_4_partition":[[1514,13,1514,19,0,0,4,0],[1515,13,1515,19,0,0,4,1]],"finer_search_wiener":[[1540,13,1540,55,0,0,4,1]],"av1_cdef_search":[[965,11,965,26,0,0,4,1]],"av1_prune_ab_partitions":[[1916,34,1916,74,0,0,4,1],[1919,34,1919,74,0,0,4,1],[1997,7,1997,47,0,0,4,1]]},"library_calls":["aom_codec_version_str","aom_obu_type_to_string","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_alloc","aom_img_remove_metadata","aom_img_free","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_obu_type_to_string","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_alloc","aom_img_remove_metadata","aom_img_free","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":1}
/**/


// Include necessary libaom headers

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small for basic operations
    if (size < 1) {
        return 0;
    }

    // API 1: Get libaom version string
    const char* version_str = aom_codec_version_str();
    (void)version_str; // Prevent unused variable warning

    // API 2: Get OBU type string (using first byte as OBU type)
    OBU_TYPE obu_type = static_cast<OBU_TYPE>(data[0] % 10); // OBU_TYPE enum typically 0-9
    const char* obu_str = aom_obu_type_to_string(obu_type);
    (void)obu_str;

    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx(); // API 3
    
    // API 4: Get decoder interface name
    const char* iface_name = aom_codec_iface_name(decoder_iface);
    (void)iface_name;
    
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    // API 5: Initialize decoder
    aom_codec_err_t dec_init_res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (dec_init_res != AOM_CODEC_OK) {
        // If decoder initialization fails, try to continue with other APIs
        // but skip decoder-specific operations
    } else {
        // API 6: Peek stream info
        aom_codec_stream_info_t stream_info = {0};
        aom_codec_peek_stream_info(decoder_iface, data, std::min(size, (size_t)1024), &stream_info);
        
        // API 7: Decode the input
        aom_codec_decode(&decoder, data, size, nullptr);
        
        // API 8: Get stream info from decoder context
        aom_codec_get_stream_info(&decoder, &stream_info);
        
        // API 9: Get decoded frames
        aom_codec_iter_t iter = nullptr;
        const aom_image_t* decoded_frame;
        std::vector<const aom_image_t*> frames;
        
        while ((decoded_frame = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
            frames.push_back(decoded_frame);
            
            // API 10: Get plane width
            int plane_width = aom_img_plane_width(decoded_frame, 0);
            (void)plane_width;
            
            // API 11: Get plane height
            int plane_height = aom_img_plane_height(decoded_frame, 0);
            (void)plane_height;
            
            // API 12: Remove metadata from image (create non-const copy)
            aom_image_t* img_copy = aom_img_alloc(nullptr, decoded_frame->fmt, 
                                                  decoded_frame->d_w, decoded_frame->d_h, 16);
            if (img_copy) {
                // Copy metadata flags for demonstration
                if (decoded_frame->metadata) {
                    // API 13: Remove metadata
                    aom_img_remove_metadata(img_copy);
                }
                
                // API 14: Free allocated image
                aom_img_free(img_copy);
            }
        }
    }
    
    // Initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx(); // API 15
    
    // Get encoder interface name
    const char* encoder_iface_name = aom_codec_iface_name(encoder_iface);
    (void)encoder_iface_name;
    
    aom_codec_enc_cfg_t enc_cfg;
    
    // API 16: Get default encoder configuration
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
        // Configure encoder based on input data
        enc_cfg.g_w = 128;
        enc_cfg.g_h = 128;
        enc_cfg.g_threads = 1;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 100;
        enc_cfg.g_pass = AOM_RC_ONE_PASS;
        enc_cfg.g_lag_in_frames = 0;
        
        // API 17: Initialize encoder
        if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
            // API 18: Set encoder configuration (change bitrate based on input)
            enc_cfg.rc_target_bitrate = 100 + (data[0] % 900); // 100-1000 kbps
            aom_codec_enc_config_set(&encoder, &enc_cfg);
            
            // Create a test image for encoding
            aom_image_t* test_image = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 128, 128, 16);
            if (test_image) {
                // Fill image with data (using input to influence content)
                for (int plane = 0; plane < 3; ++plane) {
                    uint8_t* plane_buf = test_image->planes[plane];
                    int stride = test_image->stride[plane];
                    int plane_width = aom_img_plane_width(test_image, plane);
                    int plane_height = aom_img_plane_height(test_image, plane);
                    
                    for (int y = 0; y < plane_height; ++y) {
                        size_t data_offset = (y * plane_width) % size;
                        uint8_t fill_value = data[data_offset];
                        memset(plane_buf + y * stride, fill_value, plane_width);
                    }
                }
                
                // API 19: Encode the image
                aom_codec_encode(&encoder, test_image, 0, 1, 0);
                
                // API 20: Get encoded data
                aom_codec_iter_t enc_iter = nullptr;
                const aom_codec_cx_pkt_t* pkt;
                while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                    // Process packet if needed
                    (void)pkt;
                }
                
                aom_img_free(test_image);
            }
            
            // API 21: Destroy encoder
            aom_codec_destroy(&encoder);
        }
    }
    
    // Destroy decoder if it was initialized
    if (dec_init_res == AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
    }
    
    return 0;
}