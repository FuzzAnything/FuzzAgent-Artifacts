#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <cstring>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"

namespace harness_015 {
/*
 * Fuzzing harness for libaom AV1 encoder configuration validation
 * Specifically targets 285 blocked branches in validate_config function
 * Based on coverage guidance: /root/FuzzAgent/output/libaom/coverage_guidance/harness_015_guidance.md
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_006: general edge case configuration values
 * - harness_015: targeted exploration of specific validation paths in validate_config:
 *   1. max_gf_interval > 0 validation (lines 738-741)
 *   2. gf_min_pyr_height > gf_max_pyr_height validation (lines 744-748)
 *   3. large_scale_tile = 1 and aq_mode > 0 incompatibility (lines 790-793)
 *   4. Second pass encoding with valid two-pass stats validation (lines 804-824)
 */



// Include the actual FIRSTPASS_STATS definition from libaom
// Note: We can't directly include av1/encoder/firstpass.h due to build dependencies
// We'll use a minimal compatible structure based on the validation requirements
typedef struct {
  double frame;
  double weight;
  double intra_error;
  double coded_error;
  double sr_coded_error;
  double pcnt_inter;
  double pcnt_motion;
  double pcnt_second_ref;
  double pcnt_neutral;
  double intra_skip_pct;
  double inactive_zone_rows;
  double inactive_zone_cols;
  double MVr;
  double MVc;
  double MVrv;
  double MVcv;
  double mv_in_out_count;
  double duration;
  double count;
} FIRSTPASS_STATS;

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful fuzzing:
    // - Configuration parameters: ~100 bytes
    // - Image data for test frames: ~100 bytes
    // - Two-pass stats data: ~152 bytes (2 * sizeof(FIRSTPASS_STATS))
    // Total minimum: ~352 bytes
    if (size < 352) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Step 0: Create a simple test image
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(64, 1920);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(64, 1080);
    
    aom_image_t* img = aom_img_alloc_with_border(nullptr, AOM_IMG_FMT_I420, width, height, 
                                                  1,  // stride alignment
                                                  1,  // size alignment  
                                                  0); // no border
    
    if (img == nullptr) {
        return 0; // Failed to allocate image
    }
    
    // Fill the image with minimal data
    for (int plane = 0; plane < 3; plane++) {
        if (img->stride[plane] > 0 && img->planes[plane] != nullptr) {
            size_t plane_height = (plane == 0) ? height : ((height + 1) / 2);
            size_t plane_size = img->stride[plane] * plane_height;
            
            // Use a small amount of data to fill image
            if (fdp.remaining_bytes() >= plane_size) {
                std::vector<uint8_t> plane_data = fdp.ConsumeBytes<uint8_t>(plane_size);
                memcpy(img->planes[plane], plane_data.data(), plane_size);
            } else {
                memset(img->planes[plane], 0, plane_size);
            }
        }
    }
    
    aom_image_t* wrapped_img = img;
    
    // Step 1: Initialize encoder context
    aom_codec_ctx_t codec;
    aom_codec_iface_t* iface = aom_codec_av1_cx();
    
    // Consume initialization flags
    aom_codec_flags_t flags = static_cast<aom_codec_flags_t>(
        fdp.ConsumeIntegral<uint32_t>());
    
    // Get default configuration
    aom_codec_enc_cfg_t cfg;
    unsigned int usage = fdp.ConsumeIntegralInRange<unsigned int>(0, 2); // 0-2 for AOM_USAGE values
    
    aom_codec_err_t res = aom_codec_enc_config_default(iface, &cfg, usage);
    if (res != AOM_CODEC_OK) {
        aom_img_free(img);
        return 0; // Failed to get default config
    }
    
    // Set basic configuration
    cfg.g_w = width;
    cfg.g_h = height;
    
    // Step 2: Initialize encoder with version check
    res = aom_codec_enc_init_ver(&codec, iface, &cfg, flags, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        aom_img_free(img);
        return 0; // Encoder initialization failed
    }
    
    // Step 3: Determine which validation path to test based on fuzzer input
    uint8_t validation_target = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Declare variables outside switch to avoid scope issues
    unsigned int aq_mode = 0;
    size_t packet_sz = 0;
    size_t stats_buffer_size = 0;
    
    switch (validation_target) {
        case 0: // Test max_gf_interval > 0 validation (lines 738-741)
            {
                // Set min_gf_interval and max_gf_interval to trigger validation
                // max_gf_interval > 0 and >= min_gf_interval (with min >= 2)
                unsigned int min_gf_interval = fdp.ConsumeIntegralInRange<unsigned int>(2, 10);
                unsigned int max_gf_interval = fdp.ConsumeIntegralInRange<unsigned int>(min_gf_interval, 20);
                
                aom_codec_control(&codec, AV1E_SET_MIN_GF_INTERVAL, min_gf_interval);
                aom_codec_control(&codec, AV1E_SET_MAX_GF_INTERVAL, max_gf_interval);
            }
            break;
            
        case 1: // Test gf_min_pyr_height > gf_max_pyr_height validation (lines 744-748)
            {
                // Set pyramid heights in wrong order to trigger validation error
                unsigned int gf_min_pyr_height = fdp.ConsumeIntegralInRange<unsigned int>(3, 5);
                unsigned int gf_max_pyr_height = fdp.ConsumeIntegralInRange<unsigned int>(1, gf_min_pyr_height - 1);
                
                aom_codec_control(&codec, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, gf_min_pyr_height);
                aom_codec_control(&codec, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, gf_max_pyr_height);
            }
            break;
            
        case 2: // Test large_scale_tile = 1 and aq_mode > 0 incompatibility (lines 790-793)
            {
                // Enable large scale tile coding
                cfg.large_scale_tile = 1;
                
                // Set adaptive quantization mode (non-zero to trigger error)
                aq_mode = fdp.ConsumeIntegralInRange<unsigned int>(1, 3);
                aom_codec_control(&codec, AV1E_SET_AQ_MODE, aq_mode);
                
                // Re-set configuration with large_scale_tile enabled
                res = aom_codec_enc_config_set(&codec, &cfg);
                // This should trigger validation error in validate_config
            }
            break;
            
        case 3: // Test second pass encoding with two-pass stats validation (lines 804-824)
            {
                // Set to second pass encoding
                cfg.g_pass = AOM_RC_SECOND_PASS;
                
                // Create valid two-pass stats buffer
                // Need at least 2 packets as per validation (line 816)
                packet_sz = sizeof(FIRSTPASS_STATS);
                stats_buffer_size = 2 * packet_sz;
                
                if (fdp.remaining_bytes() >= stats_buffer_size) {
                    std::vector<uint8_t> stats_buffer = fdp.ConsumeBytes<uint8_t>(stats_buffer_size);
                    
                    // Set up the stats structure
                    cfg.rc_twopass_stats_in.buf = stats_buffer.data();
                    cfg.rc_twopass_stats_in.sz = stats_buffer_size;
                    
                    // Try to re-initialize with second pass config
                    // This will trigger validation in validate_config
                    aom_codec_destroy(&codec);
                    res = aom_codec_enc_init_ver(&codec, iface, &cfg, flags, AOM_ENCODER_ABI_VERSION);
                }
            }
            break;
    }
    
    // Step 4: Try to encode frames with potentially invalid configuration
    // This will exercise the validation logic in the encoder
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    
    for (int frame_idx = 0; frame_idx < 2 && fdp.remaining_bytes() > 10; frame_idx++) {
        aom_enc_frame_flags_t frame_flags = static_cast<aom_enc_frame_flags_t>(
            fdp.ConsumeIntegral<uint32_t>());
        
        // Encode the frame - may fail due to invalid configuration
        res = aom_codec_encode(&codec, wrapped_img, pts, duration, frame_flags);
        
        // Retrieve encoded data if available
        const aom_codec_cx_pkt_t* pkt = nullptr;
        aom_codec_iter_t iter = nullptr;
        
        while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != nullptr) {
            // Process packet if needed
        }
        
        pts += duration;
    }
    
    // Step 5: Clean up
    aom_codec_destroy(&codec);
    aom_img_free(img);
    
    return 0;
}

} // namespace harness_015
