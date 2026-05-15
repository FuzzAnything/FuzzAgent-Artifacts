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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 84.5, nr_unique_branch: 75
//<Quality> {"density":13,"unique_branches":{"set_encoder_config":[[1375,8,1375,41,0,0,4,0],[1375,45,1375,79,0,0,4,0],[1375,45,1375,79,0,0,4,1],[34,23,34,34,10,0,4,0],[34,23,34,34,10,0,4,1]],"switch_tile_and_get_next_job":[[34,23,34,34,1,0,4,1]],"firstpass_intra_prediction":[[528,7,528,34,0,0,4,0]],"compute_num_enc_row_mt_workers":[[34,23,34,34,2,0,4,0]],"fp_enc_row_mt_worker_hook":[[482,9,482,27,0,0,4,1],[495,9,495,26,0,0,4,0]],"av1_select_sb_size":[[1018,29,1018,46,0,0,4,1],[1024,30,1024,51,0,0,4,0],[1024,55,1024,71,0,0,4,0],[1024,55,1024,71,0,0,4,1]],"compute_num_ai_workers":[[34,23,34,34,1,0,4,0]],"av1_set_frame_size":[[2694,7,2694,30,0,0,4,1]],"get_sync_range":[[37,12,37,25,0,0,4,0]],"set_good_speed_feature_framesize_dependent":[[773,9,773,26,0,0,4,1],[779,9,779,26,0,0,4,0],[781,16,781,33,0,0,4,0],[787,9,787,26,0,0,4,0],[808,9,808,26,0,0,4,0],[814,9,814,26,0,0,4,0],[823,9,823,26,0,0,4,0],[831,9,831,26,0,0,4,0],[837,9,837,26,0,0,4,0],[843,9,843,26,0,0,4,1],[845,9,845,26,0,0,4,0],[846,45,846,56,0,0,4,1],[853,9,853,26,0,0,4,0],[855,11,855,18,0,0,4,1],[862,9,862,27,0,0,4,1],[866,37,866,44,0,0,4,0],[870,9,870,27,0,0,4,1],[890,9,890,26,0,0,4,0],[891,23,891,48,0,0,4,0],[891,23,891,48,0,0,4,1],[900,9,900,26,0,0,4,0],[903,52,903,63,0,0,4,1],[912,9,912,26,0,0,4,0],[917,9,917,26,0,0,4,0],[918,47,918,54,0,0,4,0],[923,9,923,26,0,0,4,0],[925,47,925,54,0,0,4,0],[938,9,938,26,0,0,4,0],[945,9,945,26,0,0,4,0],[954,9,954,26,0,0,4,1],[956,9,956,26,0,0,4,0],[958,16,958,33,0,0,4,0],[959,44,959,68,0,0,4,0],[962,9,962,26,0,0,4,0],[973,9,973,26,0,0,4,0],[975,16,975,33,0,0,4,0],[978,9,978,26,0,0,4,0],[984,9,984,27,0,0,4,1],[991,9,991,27,0,0,4,1],[995,9,995,26,0,0,4,1],[1001,9,1001,26,0,0,4,0],[1003,12,1003,38,0,0,4,1],[1003,42,1003,65,0,0,4,0],[1007,9,1007,26,0,0,4,0],[1020,9,1020,26,0,0,4,0],[1020,30,1020,48,0,0,4,1],[1024,9,1024,26,0,0,4,0],[1028,9,1028,27,0,0,4,0],[1032,9,1032,26,0,0,4,0],[1036,9,1036,27,0,0,4,1],[1041,9,1041,26,0,0,4,0],[1049,9,1049,26,0,0,4,0],[1055,9,1055,26,0,0,4,0],[1061,9,1061,27,0,0,4,1]],"av1_get_max_num_workers":[[34,23,34,34,2,0,4,0]],"av1_set_speed_features_framesize_independent":[[2718,9,2718,19,0,0,4,0],[2732,10,2732,43,0,0,4,1],[2732,47,2732,77,0,0,4,1]]},"library_calls":["aom_codec_av1_cx","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc_with_border","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_wrap","aom_codec_encode","aom_codec_get_preview_frame","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_get_caps","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc_with_border","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_wrap","aom_codec_encode","aom_codec_get_preview_frame","aom_img_free","aom_codec_destroy"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing
    
    // Create a FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize encoder interface
    aom_codec_iface_t* codec_iface = aom_codec_av1_cx();
    if (!codec_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Check codec capabilities
    aom_codec_caps_t caps = aom_codec_get_caps(codec_iface);
    
    // Create encoder context
    aom_codec_ctx_t encoder;
    memset(&encoder, 0, sizeof(encoder));
    
    // Setup encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(codec_iface, &cfg, 0);
    
    // Set some basic configuration from input data
    cfg.g_w = (data[0] % 128 + 1) * 16;  // Width: 16-2048
    cfg.g_h = (data[1] % 128 + 1) * 16;  // Height: 16-2048
    cfg.g_threads = (data[2] % 8) + 1;   // Threads: 1-8
    cfg.rc_target_bitrate = 1000;        // Fixed bitrate
    
    // Initialize encoder
    aom_codec_err_t err = aom_codec_enc_init_ver(&encoder, codec_iface, &cfg, 0, 
                                                 AOM_ENCODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set encoder options from input data
    char option_name[32];
    char option_value[32];
    
    // Set quality option
    snprintf(option_name, sizeof(option_name), "cpu-used");
    snprintf(option_value, sizeof(option_value), "%d", data[3] % 9); // 0-8
    aom_codec_set_option(&encoder, option_name, option_value);
    
    // Create an image with border using input data
    const unsigned int width = cfg.g_w;
    const unsigned int height = cfg.g_h;
    const unsigned int border = 32;
    
    aom_image_t* img_with_border = aom_img_alloc_with_border(
        nullptr, AOM_IMG_FMT_I420, width, height, 1, 1, border);
    
    if (img_with_border) {
        // Fill image planes with data from input
        size_t data_offset = 4;
        for (int plane = 0; plane < 3; plane++) {
            int plane_width = aom_img_plane_width(img_with_border, plane);
            int plane_height = aom_img_plane_height(img_with_border, plane);
            size_t plane_size = plane_width * plane_height;
            
            if (data_offset + plane_size > size) {
                plane_size = size - data_offset;
            }
            
            if (plane_size > 0) {
                uint8_t* plane_data = img_with_border->planes[plane];
                fseek(in_file, data_offset, SEEK_SET);
                fread(plane_data, 1, plane_size, in_file);
                data_offset += plane_size;
            }
        }
        
        // Add metadata to the image
        if (data_offset + 20 < size) {
            uint32_t metadata_type = *reinterpret_cast<const uint32_t*>(data + data_offset);
            data_offset += 4;
            
            size_t metadata_size = (size - data_offset) % 1024; // Limit metadata size
            if (metadata_size > 0) {
                aom_img_add_metadata(img_with_border, metadata_type, 
                                    data + data_offset, metadata_size,
                                    AOM_MIF_ANY_FRAME);
            }
        }
        
        // Create another image by wrapping the same data
        aom_image_t wrapped_img;
        aom_img_wrap(&wrapped_img, AOM_IMG_FMT_I420, width, height, 
                    1, img_with_border->img_data);
        
        // Encode the image
        aom_codec_encode(&encoder, &wrapped_img, 0, 1, 0);
        
        // Try to get preview frame
        const aom_image_t* preview = aom_codec_get_preview_frame(&encoder);
        
        // Clean up the allocated image
        aom_img_free(img_with_border);
    }
    
    // Clean up
    aom_codec_destroy(&encoder);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}