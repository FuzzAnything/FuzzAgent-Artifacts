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
//<ID> 85
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 23
//<Quality> {"density":22,"unique_branches":{"write_sequence_header":[[2655,9,2655,50,0,0,4,0]],"av1_write_metadata_obu":[[4199,7,4200,8,0,0,4,1],[4203,7,4203,56,0,0,4,1]],"write_uncompressed_header_obu":[[2980,9,2980,50,0,0,4,0]],"av1_write_metadata_array":[[4217,7,4217,11,0,0,4,1],[4222,22,4222,33,0,0,4,0],[4222,22,4222,33,0,0,4,1],[4224,9,4224,25,0,0,4,0],[4224,29,4224,54,0,0,4,0],[4227,12,4227,53,0,0,4,0],[4228,12,4228,57,0,0,4,0],[4233,13,4233,25,0,0,4,1],[4247,13,4247,34,0,0,4,1],[4253,13,4253,35,0,0,4,1],[4257,13,4258,71,0,0,4,0]],"encode_frame_to_data_rate":[[4345,7,4345,48,0,0,4,0],[4348,9,4348,35,0,0,4,0],[4351,11,4351,54,0,0,4,1],[4366,11,4366,40,0,0,4,1],[4435,7,4435,48,0,0,4,0],[4436,21,4436,35,0,0,4,0],[4436,21,4436,35,0,0,4,1],[4437,11,4437,56,0,0,4,0]]},"library_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_get_stream_info","aom_codec_decode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_destroy","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_img_remove_metadata","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_img_add_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_img_remove_metadata","aom_img_free","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_get_stream_info","aom_codec_decode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_img_remove_metadata","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_img_add_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_img_remove_metadata","aom_img_free","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Print version string (API 1)
    const char *version = aom_codec_version_str();
    (void)version; // Avoid unused warning

    // Step 1: Decode input bitstream
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *dec_iface = aom_codec_av1_dx(); // API 2
    
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    // API 3: Initialize decoder
    if (aom_codec_dec_init_ver(&decoder, dec_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }
    
    // API 4: Peek stream info
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    aom_codec_peek_stream_info(dec_iface, data, size, &stream_info);
    
    // API 5: Get stream info from decoder
    aom_codec_get_stream_info(&decoder, &stream_info);
    
    // Decode the input data (API 22)
    aom_codec_decode(&decoder, data, size, NULL);
    
    // Step 2: Encode decoded frames
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *enc_iface = aom_codec_av1_cx(); // API 7
    
    aom_codec_enc_cfg_t enc_cfg;
    // API 6: Get default encoder config
    if (aom_codec_enc_config_default(enc_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Set basic encoder parameters
    enc_cfg.g_w = stream_info.w > 0 ? stream_info.w : 128;
    enc_cfg.g_h = stream_info.h > 0 ? stream_info.h : 128;
    enc_cfg.g_threads = 1;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_error_resilient = AOM_ERROR_RESILIENT_DEFAULT;
    enc_cfg.g_lag_in_frames = 0;
    
    // API 8: Initialize encoder
    if (aom_codec_enc_init_ver(&encoder, enc_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        return 0;
    }
    
    // Get decoded frames and re-encode them
    aom_codec_iter_t iter = NULL;
    aom_image_t *img;
    int frame_count = 0;
    
    while ((img = aom_codec_get_frame(&decoder, &iter)) != NULL) { // API 23
        frame_count++;
        
        // API 10, 11: Get plane dimensions
        int plane_width = aom_img_plane_width(img, 0);
        int plane_height = aom_img_plane_height(img, 0);
        (void)plane_width;
        (void)plane_height;
        
        // API 12: Add metadata to image
        uint8_t metadata_data[] = {0x01, 0x02, 0x03};
        aom_img_add_metadata(img, 1, metadata_data, sizeof(metadata_data), 
                           AOM_MIF_ANY_FRAME);
        
        // API 13: Get number of metadata
        size_t num_metadata = aom_img_num_metadata(img);
        (void)num_metadata;
        
        // API 15: Encode the frame
        aom_codec_encode(&encoder, img, frame_count, 1, 0);
        
        // Get encoded data (API 16)
        aom_codec_iter_t enc_iter = NULL;
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
            // Process encoded packet if needed
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Encoded frame available
            }
        }
        
        // API 14: Remove metadata
        aom_img_remove_metadata(img);
        
        // Only process first few frames to limit execution time
        if (frame_count >= 2) break;
    }
    
    // If no frames were decoded, create a test image and encode it
    if (frame_count == 0) {
        // API 9: Allocate test image
        aom_image_t test_img;
        aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
        unsigned int width = 128;
        unsigned int height = 128;
        unsigned int align = 1;
        
        if (aom_img_alloc(&test_img, fmt, width, height, align) != NULL) {
            // Fill with dummy data
            for (int plane = 0; plane < 3; plane++) {
                uint8_t *plane_buf = test_img.planes[plane];
                int stride = test_img.stride[plane];
                int plane_h = aom_img_plane_height(&test_img, plane);
                for (int y = 0; y < plane_h; y++) {
                    memset(plane_buf + y * stride, y % 256, 
                          aom_img_plane_width(&test_img, plane));
                }
            }
            
            // Add metadata (API 12)
            uint8_t test_metadata[] = {0x04, 0x05, 0x06};
            aom_img_add_metadata(&test_img, 2, test_metadata, 
                               sizeof(test_metadata), AOM_MIF_KEY_FRAME);
            
            // Encode test frame (API 15)
            aom_codec_encode(&encoder, &test_img, 0, 1, 0);
            
            // Get encoded data (API 16)
            aom_codec_iter_t enc_iter = NULL;
            const aom_codec_cx_pkt_t *pkt;
            while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    // Encoded frame available
                }
            }
            
            // Remove metadata (API 14)
            aom_img_remove_metadata(&test_img);
            
            // API 17: Free image
            aom_img_free(&test_img);
        }
    }
    
    // Flush encoder
    aom_codec_encode(&encoder, NULL, frame_count, 1, 0);
    
    // Get remaining encoded data
    aom_codec_iter_t enc_iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Final encoded frames
        }
    }
    
    // API 18, 19: Destroy codec contexts
    aom_codec_destroy(&decoder);
    aom_codec_destroy(&encoder);
    
    return 0;
}