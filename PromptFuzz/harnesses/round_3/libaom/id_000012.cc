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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 157.5, nr_unique_branch: 44
//<Quality> {"density":15,"unique_branches":{"write_wiener_filter":[[1851,7,1851,31,0,0,4,0],[1851,7,1851,31,0,0,4,1],[1870,7,1870,31,0,0,4,0],[1870,7,1870,31,0,0,4,1]],"av1_rc_pick_q_and_bounds":[[2356,8,2356,38,0,0,4,1],[2357,8,2357,53,0,0,4,1]],"av1_set_size_dependent_vars":[[703,9,703,39,0,0,4,0],[704,9,704,67,0,0,4,1]],"prune_partitions_after_none":[[4213,7,4213,59,0,0,4,1]],"get_intra_q_and_bounds":[[1831,7,1831,29,0,0,4,1],[1878,9,1878,36,0,0,4,0],[1905,9,1905,35,0,0,4,0],[1906,10,1906,52,0,0,4,1],[1907,10,1907,49,0,0,4,1]],"get_active_cq_level":[[1546,33,1546,54,0,0,4,0],[1547,10,1547,47,0,0,4,1],[1548,10,1548,44,0,0,4,1]],"av1_get_second_pass_params":[[4029,7,4029,33,0,0,4,0]],"encode_restoration_mode":[[1792,7,1792,26,0,0,4,0]],"rc_pick_q_and_bounds":[[2203,7,2203,33,0,0,4,0]],"av1_set_speed_features_qindex_dependent":[[2882,31,2882,76,0,0,4,1],[2975,18,2975,63,0,0,4,0]],"init_encode_rd_sb":[[677,8,677,46,0,0,4,1],[678,8,678,51,0,0,4,0]],"loop_restoration_write_sb_coeffs":[[1966,14,1966,43,0,0,4,0],[1972,9,1972,35,0,0,4,0]],"set_encoder_config":[[1292,7,1292,30,0,0,4,0]],"search_wiener":[[1726,7,1726,32,0,0,4,0],[1745,7,1745,32,0,0,4,0],[1746,7,1746,30,0,0,4,0]],"validate_config":[[847,7,847,33,0,0,4,0],[866,41,866,69,0,0,4,1],[869,12,869,53,0,0,4,1],[673,9,673,29,82,0,4,1]],"av1_prune_partitions_before_search":[[1797,7,1797,49,0,0,4,1]],"av1_full_pixel_search":[[1898,5,1898,15,0,0,4,1],[1899,5,1899,19,0,0,4,0],[1910,8,1910,32,0,0,4,1],[1910,36,1910,64,0,0,4,0]],"copy_unit_info":[[1856,7,1856,46,0,0,4,0]],"rc_pick_q_and_bounds_q_mode":[[2146,7,2146,30,0,0,4,0],[2155,7,2155,19,0,0,4,0],[35,23,35,34,1,0,4,1]],"av1_change_config":[[931,34,931,57,0,0,4,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_img_alloc","aom_codec_destroy","aom_codec_get_global_headers","aom_codec_encode","aom_codec_get_preview_frame","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_flip","aom_img_free","aom_img_metadata_alloc","aom_img_remove_metadata","aom_img_metadata_free","aom_uleb_encode","aom_img_free","aom_codec_destroy","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_peek_stream_info","aom_codec_destroy"],"critical_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_img_alloc","aom_codec_get_global_headers","aom_codec_encode","aom_codec_get_preview_frame","aom_img_alloc","aom_img_plane_height","aom_img_plane_width","aom_img_flip","aom_img_free","aom_img_metadata_alloc","aom_img_remove_metadata","aom_img_metadata_free","aom_uleb_encode","aom_img_free","aom_codec_destroy","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_peek_stream_info","aom_codec_destroy"],"visited":1}
/**/


extern "C" {
}

// Fuzzer entry point
extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small to be meaningful
    if (size < 64) {
	return 0;
	}
	
    
    // Create a temporary buffer for operations
    std::vector<uint8_t> buffer(data, data + size);
    
    // 1. Create and initialize encoder context
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) {
	return 0;
	}
	
    
    // Get default encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err = aom_codec_enc_config_default(encoder_iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
	return 0;
	}
	
    
    // Modify config with some fuzzed parameters (safe values)
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_threads = 1;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    cfg.rc_end_usage = AOM_Q;
    cfg.rc_min_quantizer = 20;
    cfg.rc_max_quantizer = 40;
    
    // Initialize encoder
    err = aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
	return 0;
	}
	
    
    // 2. Set encoder configuration (first API call)
    err = aom_codec_enc_config_set(&encoder, &cfg);
    // Note: This might fail if config is invalid, but we continue anyway
    
    // 3. Create a test image
    aom_image_t img;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    unsigned int width = 64;
    unsigned int height = 64;
    unsigned int align = 32;
    
    if (aom_img_alloc(&img, fmt, width, height, align) == NULL) {
        aom_codec_destroy(&encoder);
        return 0;
    }
    
    // Fill image planes with some fuzzed data
    size_t plane_size = (width * height) >> (img.x_chroma_shift + img.y_chroma_shift);
    for (int plane = 0; plane < 3; ++plane) {
        size_t copy_size = std::min(plane_size, size);
        if (copy_size > 0) {
            memcpy(img.planes[plane], data, copy_size);
        }
    }
    
    // 4. Get global headers (second API call)
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&encoder);
    if (global_headers) {
        // We don't need to do anything with them, just ensure they're freed
        free(global_headers->buf);
        free(global_headers);
    }
    
    // 5. Encode a frame to trigger preview frame generation
    aom_codec_encode(&encoder, &img, 0, 1, 0);
    
    // 6. Get preview frame (third API call)
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&encoder);
    
    // 7. If we got a preview frame, flip it (fourth API call)
    if (preview_frame) {
        // We need a mutable copy since aom_img_flip modifies the image
        aom_image_t mutable_img;
        if (aom_img_alloc(&mutable_img, preview_frame->fmt, 
                         preview_frame->d_w, preview_frame->d_h, 
                         align) != NULL) {
            // Copy the preview frame
            for (int plane = 0; plane < 3; ++plane) {
                size_t plane_bytes = aom_img_plane_width(&mutable_img, plane) *
                                   aom_img_plane_height(&mutable_img, plane);
                memcpy(mutable_img.planes[plane], preview_frame->planes[plane], plane_bytes);
            }
            
            // Flip the image
            aom_img_flip(&mutable_img);
            
            aom_img_free(&mutable_img);
        }
    }
    
    // 8. Allocate metadata (fifth API call)
    // Use some fuzzed data for metadata
    size_t metadata_size = size > 100 ? 50 : size / 2;
    if (metadata_size > 0) {
        uint8_t metadata_data[50];
        memcpy(metadata_data, data, metadata_size < 50 ? metadata_size : 50);
        
        aom_metadata_t *metadata = aom_img_metadata_alloc(
            1,  // type
            metadata_data,
            metadata_size,
            AOM_MIF_ANY_FRAME
        );
        
        if (metadata) {
            // 9. Remove metadata from image (sixth API call)
            // Note: The image doesn't actually have metadata attached, but
            // we still call the function to exercise it
            aom_img_remove_metadata(&img);
            
            // Free the allocated metadata
            aom_img_metadata_free(metadata);
        }
    }
    
    // 10. Encode a ULEB value (seventh API call)
    // Use a value derived from the input
    uint64_t uleb_value = size;
    uint8_t coded_value[10];
    size_t coded_size = 0;
    
    aom_uleb_encode(uleb_value, sizeof(coded_value), coded_value, &coded_size);
    
    // 11. Clean up
    aom_img_free(&img);
    aom_codec_destroy(&encoder);
    
    // 12. Also test with decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    if (decoder_iface) {
        // Initialize decoder
        err = aom_codec_dec_init_ver(&decoder, decoder_iface, NULL, 0, AOM_DECODER_ABI_VERSION);
        if (err == AOM_CODEC_OK) {
            // Try to decode the input data
            aom_codec_decode(&decoder, data, size, NULL);
            
            // Try to get stream info
            aom_codec_stream_info_t si;
            memset(&si, 0, sizeof(si));
            aom_codec_peek_stream_info(decoder_iface, data, size, &si);
            
            aom_codec_destroy(&decoder);
        }
    }
    
    return 0;
}