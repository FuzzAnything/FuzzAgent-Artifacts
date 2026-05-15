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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 33
//<Quality> {"density":12,"unique_branches":{"exhaustive_mesh_search":[[1664,17,1664,35,0,0,4,1]],"av1_rd_pick_palette_intra_sbuv":[[815,22,815,61,0,0,4,0],[864,24,864,43,0,0,4,0]],"av1_rd_pick_palette_intra_sby":[[593,31,593,71,0,0,4,1]],"search_filter_level":[[167,11,167,31,0,0,4,1],[173,11,173,47,0,0,4,0],[175,13,175,40,0,0,4,0],[183,9,183,28,0,0,4,1],[201,24,201,46,0,0,4,0]],"av1_pick_filter_level":[[437,37,437,61,0,0,4,0],[438,9,438,62,0,0,4,1]],"write_modes_b":[[1540,7,1540,45,0,0,4,1]],"av1_rc_update_rate_correction_factors":[[998,7,998,30,0,0,4,0],[1000,12,1000,35,0,0,4,1],[1035,7,1035,31,0,0,4,0],[1040,9,1040,48,0,0,4,1],[1042,14,1042,38,0,0,4,1],[34,23,34,34,3,0,4,1],[34,23,34,34,4,0,4,1]],"av1_fast_palette_color_index_context":[[109,9,109,49,0,0,4,1],[113,14,113,54,0,0,4,0],[116,14,116,54,0,0,4,1],[128,9,128,48,0,0,4,0],[135,9,135,38,0,0,4,1],[136,10,136,40,0,0,4,0],[136,10,136,40,0,0,4,1],[136,44,136,73,0,0,4,0],[136,44,136,73,0,0,4,1],[139,9,139,29,0,0,4,0],[140,11,140,40,0,0,4,1],[143,11,143,40,0,0,4,1],[153,21,153,43,0,0,4,1]],"av1_encode_frame":[[2773,11,2773,59,0,0,4,0]]},"library_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_plane_width","aom_img_num_metadata","aom_img_alloc","aom_img_remove_metadata","aom_img_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_plane_width","aom_img_num_metadata","aom_img_alloc","aom_img_remove_metadata","aom_img_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy"],"visited":1}
/**/


extern "C" {
}

// Fuzzer entry point
extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // 1. Print library version using aom_codec_version_str()
    const char *version_str = aom_codec_version_str();
    (void)version_str; // Prevent unused variable warning
    
    // 2. Initialize decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, 
                                                 AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        // Cleanup and return
        return 0;
    }
    
    // 3. Get stream information using aom_codec_get_stream_info
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // First try to peek stream info
    aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    
    // Then get stream info from decoder context
    aom_codec_get_stream_info(&decoder, &stream_info);
    
    // 4. Decode the input data
    res = aom_codec_decode(&decoder, data, size, NULL);
    
    // 5. Get decoded frames and process them
    aom_codec_iter_t iter = NULL;
    const aom_image_t *img;
    
    while ((img = aom_codec_get_frame(&decoder, &iter)) != NULL) {
        // 6. Get plane width using aom_img_plane_width
        int plane_width = aom_img_plane_width(img, 0);
        (void)plane_width; // Prevent unused variable warning
        
        // 7. Get number of metadata using aom_img_num_metadata
        size_t metadata_count = aom_img_num_metadata(img);
        
        // 8. Remove metadata using aom_img_remove_metadata (need non-const image)
        if (metadata_count > 0) {
            // Create a mutable copy of the image to remove metadata
            aom_image_t *mutable_img = aom_img_alloc(NULL, img->fmt, img->d_w, img->d_h, 16);
            if (mutable_img) {
                // Copy image data (simplified - in real code would copy plane data)
                aom_img_remove_metadata(mutable_img);
                aom_img_free(mutable_img);
            }
        }
    }
    
    // 9. Initialize encoder to use aom_codec_enc_config_default and aom_codec_get_cx_data
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    // Get default encoder configuration using aom_codec_enc_config_default
    res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (res == AOM_CODEC_OK) {
        // Set some minimal configuration
        enc_cfg.g_w = 64;
        enc_cfg.g_h = 64;
        enc_cfg.g_timebase.num = 1;
        enc_cfg.g_timebase.den = 30;
        enc_cfg.rc_target_bitrate = 100;
        enc_cfg.g_threads = 1;
        enc_cfg.g_lag_in_frames = 0;
        
        // Initialize encoder
        res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, 
                                     AOM_ENCODER_ABI_VERSION);
        
        if (res == AOM_CODEC_OK) {
            // Create a simple test image for encoding
            aom_image_t *test_img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 64, 64, 16);
            
            if (test_img) {
                // Fill image with some data from fuzzer input
                size_t data_offset = 0;
                for (int plane = 0; plane < 3; plane++) {
                    int stride = test_img->stride[plane];
                    int height = aom_img_plane_height(test_img, plane);
                    int width = aom_img_plane_width(test_img, plane);
                    
                    for (int y = 0; y < height; y++) {
                        if (data_offset >= size) break;
                        size_t copy_size = (size - data_offset) < (size_t)width ? 
                                          (size - data_offset) : (size_t)width;
                        memcpy(test_img->planes[plane] + y * stride, 
                               data + data_offset, copy_size);
                        data_offset = (data_offset + copy_size) % size;
                    }
                }
                
                // Encode the image
                aom_codec_encode(&encoder, test_img, 0, 1, 0);
                
                // Get encoded data using aom_codec_get_cx_data
                const aom_codec_cx_pkt_t *pkt;
                aom_codec_iter_t enc_iter = NULL;
                
                while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                    // Process packet if needed
                    (void)pkt; // Prevent unused variable warning
                }
                
                // Cleanup test image
                aom_img_free(test_img);
            }
            
            // Destroy encoder
            aom_codec_destroy(&encoder);
        }
    }
    
    // 10. Cleanup decoder
    aom_codec_destroy(&decoder);
    
    return 0;
}