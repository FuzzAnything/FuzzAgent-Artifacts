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
//<ID> 139
//<Prompt> []
/*<Combination>: [
*/
//<score> 360, nr_unique_branch: 44
//<Quality> {"density":40,"unique_branches":{"write_tile_info_max_tile":[[2212,9,2212,48,0,0,4,0]],"rc_pick_q_and_bounds_q_mode":[[2146,7,2146,30,0,0,4,0],[2155,7,2155,19,0,0,4,0],[35,23,35,34,1,0,4,1]],"get_active_cq_level":[[1546,33,1546,54,0,0,4,0],[1547,10,1547,47,0,0,4,1],[1548,10,1548,44,0,0,4,1]],"av1_prune_partitions_before_search":[[1797,7,1797,49,0,0,4,1]],"av1_cdef_compute_sb_list":[[34,23,34,34,3,0,4,0]],"get_intra_q_and_bounds":[[1831,7,1831,29,0,0,4,1],[1837,14,1837,41,0,0,4,1],[1878,9,1878,36,0,0,4,1],[1882,9,1882,47,0,0,4,1],[1888,9,1888,40,0,0,4,0],[1893,9,1893,47,0,0,4,1],[1905,9,1905,35,0,0,4,0],[1906,10,1906,52,0,0,4,1],[1907,10,1907,49,0,0,4,1],[34,23,34,34,1,0,4,1],[34,23,34,34,2,0,4,1]],"prune_partitions_after_none":[[4213,7,4213,59,0,0,4,1]],"get_error_calc_width_in_filt_units":[[383,30,383,62,0,0,4,0],[384,7,384,43,0,0,4,0]],"rc_pick_q_and_bounds":[[2203,7,2203,33,0,0,4,0]],"av1_rc_pick_q_and_bounds":[[2356,8,2356,38,0,0,4,1],[2357,8,2357,53,0,0,4,1]],"fill_borders_for_fbs_on_frame_boundary":[[321,38,321,66,0,0,4,0],[322,7,322,33,0,0,4,0],[322,7,322,33,0,0,4,1],[322,37,322,66,0,0,4,1]],"av1_set_speed_features_qindex_dependent":[[2882,31,2882,76,0,0,4,1],[2975,18,2975,63,0,0,4,0]],"init_encode_rd_sb":[[678,8,678,51,0,0,4,0]],"av1_cdef_mse_calc_block":[[549,7,549,34,0,0,4,0],[551,9,551,28,0,0,4,1],[551,32,551,50,0,0,4,1],[555,9,555,28,0,0,4,1],[555,32,555,50,0,0,4,0],[34,23,34,34,9,0,4,0],[34,23,34,34,10,0,4,0]],"av1_set_size_dependent_vars":[[703,9,703,39,0,0,4,0],[704,9,704,67,0,0,4,1]],"av1_get_second_pass_params":[[4029,7,4029,33,0,0,4,0]],"av1_update_state":[[34,23,34,34,5,0,4,0]]},"library_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_error","aom_codec_destroy","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_uleb_decode","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_error","aom_codec_destroy","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_encode","aom_codec_error","aom_codec_get_cx_data","aom_img_free","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_get_cx_data","aom_codec_error","aom_codec_error","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_get_stream_info","aom_codec_decode","aom_codec_get_frame","aom_uleb_decode","aom_codec_av1_cx","aom_codec_iface_name","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_encode","aom_codec_error","aom_codec_get_cx_data","aom_img_free","aom_codec_get_cx_data","aom_codec_error","aom_codec_error","aom_codec_destroy","aom_codec_destroy"],"visited":1}
/*Looking at the APIs provided, I can see they cover both decoding and encoding functionalities of libaom, along with image manipulation. Let me create a fuzz driver that processes AV1 encoded data, decodes it, manipulates the images, and then re-encodes them.

*/


// Include libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure minimum size for basic operations
    if (size < 16) {
        return 0;
    }

    // 1. Get codec version
    const char *version_str = aom_codec_version_str();
    // Can use version_str for logging if needed

    // 2. Initialize decoder interface
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    const char *decoder_name = aom_codec_iface_name(decoder_iface);
    
    // 3. Peek stream info from input data
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    aom_codec_err_t peek_result = aom_codec_peek_stream_info(
        decoder_iface, data, size, &stream_info);
    
    // 4. Initialize decoder context
    aom_codec_ctx_t decoder_ctx;
    memset(&decoder_ctx, 0, sizeof(decoder_ctx));
    
    // Use default decoder configuration
    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    
    aom_codec_err_t dec_init_result = aom_codec_dec_init_ver(
        &decoder_ctx, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (dec_init_result != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error(&decoder_ctx);
        // Clean up and return
        aom_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    // 5. Get stream info from decoder context
    aom_codec_stream_info_t decoder_stream_info;
    memset(&decoder_stream_info, 0, sizeof(decoder_stream_info));
    aom_codec_err_t stream_info_result = aom_codec_get_stream_info(
        &decoder_ctx, &decoder_stream_info);
    
    // 6. Decode the input data
    aom_codec_err_t decode_result = aom_codec_decode(
        &decoder_ctx, data, size, nullptr);
    
    // 7. Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_frame = aom_codec_get_frame(&decoder_ctx, &iter);
    
    // 8. Process ULEB encoded values from data
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    if (size > 0) {
        // Use a portion of the input data for ULEB decoding
        size_t uleb_data_size = size < 16 ? size : 16;
        aom_uleb_decode(data, uleb_data_size, &uleb_value, &uleb_length);
    }
    
    // 9. Initialize encoder interface
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    const char *encoder_name = aom_codec_iface_name(encoder_iface);
    
    // 10. Initialize encoder context
    aom_codec_ctx_t encoder_ctx;
    memset(&encoder_ctx, 0, sizeof(encoder_ctx));
    
    aom_codec_enc_cfg_t enc_cfg;
    memset(&enc_cfg, 0, sizeof(enc_cfg));
    
    // Set basic encoder configuration
    unsigned int usage = 0; // Good quality
    aom_codec_enc_config_default(encoder_iface, &enc_cfg, usage);
    
    // Set dimensions from decoded frame or defaults
    if (decoded_frame) {
        enc_cfg.g_w = decoded_frame->d_w;
        enc_cfg.g_h = decoded_frame->d_h;
    } else {
        enc_cfg.g_w = 320;
        enc_cfg.g_h = 240;
    }
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.g_threads = 1;
    enc_cfg.g_lag_in_frames = 0;
    enc_cfg.rc_end_usage = AOM_Q;
    enc_cfg.rc_min_quantizer = 20;
    enc_cfg.rc_max_quantizer = 40;
    
    aom_codec_err_t enc_init_result = aom_codec_enc_init_ver(
        &encoder_ctx, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
    
    if (enc_init_result != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error(&encoder_ctx);
        aom_codec_destroy(&decoder_ctx);
        return 0;
    }
    
    // 11. Create image for encoding
    aom_image_t *img_to_encode = nullptr;
    
    if (decoded_frame) {
        // Allocate image with same format as decoded frame
        img_to_encode = aom_img_alloc(
            nullptr,
            decoded_frame->fmt,
            decoded_frame->d_w,
            decoded_frame->d_h,
            1);
        
        if (img_to_encode) {
            // Copy data from decoded frame (simplified - just copy plane pointers)
            // In real usage, you'd need to copy actual pixel data
            for (int plane = 0; plane < 3; plane++) {
                if (decoded_frame->planes[plane]) {
                    // Get plane dimensions
                    int plane_width = aom_img_plane_width(img_to_encode, plane);
                    int plane_height = aom_img_plane_height(img_to_encode, plane);
                    
                    // Here you would normally copy pixel data
                    // For fuzzing, we'll just note the dimensions
                    (void)plane_width;
                    (void)plane_height;
                }
            }
            
            // 12. Add metadata to image
            uint32_t metadata_type = 1; // Example metadata type
            uint8_t metadata_data[] = {0x01, 0x02, 0x03, 0x04};
            size_t metadata_size = sizeof(metadata_data);
            
            aom_img_add_metadata(
                img_to_encode,
                metadata_type,
                metadata_data,
                metadata_size,
                AOM_MIF_ANY_FRAME);
            
            // 13. Get number of metadata entries
            size_t num_metadata = aom_img_num_metadata(img_to_encode);
            
            // 14. Remove metadata
            aom_img_remove_metadata(img_to_encode);
            
            // 15. Encode the image
            aom_codec_pts_t pts = 0;
            unsigned long duration = 1;
            aom_enc_frame_flags_t flags = 0;
            
            aom_codec_err_t encode_result = aom_codec_encode(
                &encoder_ctx,
                img_to_encode,
                pts,
                duration,
                flags);
            
            // Check for encoding errors
            if (encode_result != AOM_CODEC_OK) {
                const char *error_detail = aom_codec_error(&encoder_ctx);
            }
            
            // 16. Get encoded data
            aom_codec_iter_t enc_iter = nullptr;
            const aom_codec_cx_pkt_t *pkt = nullptr;
            
            while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
                // Process packet if needed
                // For fuzzing, we just consume it
            }
            
            // Free the allocated image
            aom_img_free(img_to_encode);
        }
    } else {
        // If no decoded frame, create a test image
        img_to_encode = aom_img_alloc(
            nullptr,
            AOM_IMG_FMT_I420,
            320,
            240,
            1);
        
        if (img_to_encode) {
            // Get plane dimensions
            for (int plane = 0; plane < 3; plane++) {
                int plane_width = aom_img_plane_width(img_to_encode, plane);
                int plane_height = aom_img_plane_height(img_to_encode, plane);
                (void)plane_width;
                (void)plane_height;
            }
            
            // Add metadata
            uint32_t metadata_type = 2;
            uint8_t metadata_data[] = {0xAA, 0xBB, 0xCC};
            aom_img_add_metadata(
                img_to_encode,
                metadata_type,
                metadata_data,
                sizeof(metadata_data),
                AOM_MIF_KEY_FRAME);
            
            size_t num_metadata = aom_img_num_metadata(img_to_encode);
            aom_img_remove_metadata(img_to_encode);
            
            // Encode
            aom_codec_err_t encode_result = aom_codec_encode(
                &encoder_ctx,
                img_to_encode,
                0,
                1,
                0);
            
            // Get encoded data
            aom_codec_iter_t enc_iter = nullptr;
            const aom_codec_cx_pkt_t *pkt = nullptr;
            while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
                // Consume packet
            }
            
            aom_img_free(img_to_encode);
        }
    }
    
    // Get any remaining encoded data
    aom_codec_iter_t final_iter = nullptr;
    const aom_codec_cx_pkt_t *final_pkt = nullptr;
    while ((final_pkt = aom_codec_get_cx_data(&encoder_ctx, &final_iter)) != nullptr) {
        // Consume packet
    }
    
    // 17. Check for any errors
    const char *decoder_error = aom_codec_error(&decoder_ctx);
    const char *encoder_error = aom_codec_error(&encoder_ctx);
    
    // 18. Clean up
    aom_codec_destroy(&decoder_ctx);
    aom_codec_destroy(&encoder_ctx);
    
    return 0;
}