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

namespace harness_006 {
/*
 * Fuzzing harness for libaom AV1 encoder configuration validation edge cases
 * Targets validate_config function with 266 blocked branches through edge case configuration values
 * Based on coverage guidance: /root/FuzzAgent/output/libaom/coverage_guidance/harness_006_guidance.md
 * Semantic differentiation: harness_002 targeted basic encoder configuration; 
 * harness_006 focuses specifically on edge case configuration values to exercise validation branches.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful fuzzing:
    // - Multiple configuration parameters: ~100 bytes
    // - Edge case values for various config fields: ~50 bytes  
    // - Image data for test frames: ~100 bytes
    // Total minimum: ~250 bytes
    if (size < 250) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Step 0: Consume image parameters for test frame
    // Use edge case dimensions that may trigger validation branches
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(1, 65536);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(1, 65536);
    
    // Create a test image buffer (I420 format)
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
    
    // Consume initialization flags - use edge case values
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
    
    // Step 2: Initialize encoder with version check
    res = aom_codec_enc_init_ver(&codec, iface, &cfg, flags, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        aom_img_free(img);
        return 0; // Encoder initialization failed
    }
    
    // Step 3: Modify configuration parameters to edge case values
    // Target specific edge cases from validate_config function
    
    // 1. Frame dimension edge cases
    cfg.g_w = width;
    cfg.g_h = height;
    
    // Edge case: g_forced_max_frame_width/height values
    cfg.g_forced_max_frame_width = fdp.ConsumeBool() ? 
        fdp.ConsumeIntegralInRange<unsigned int>(0, 65536) : 0;
    cfg.g_forced_max_frame_height = fdp.ConsumeBool() ? 
        fdp.ConsumeIntegralInRange<unsigned int>(0, 65536) : 0;
    
    // Edge case: max_frame_area > 2^30 (from validate_config line 703)
    // Force large dimensions to trigger area validation
    if (fdp.ConsumeBool()) {
        cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(32768, 65536);
        cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(32768, 65536);
    }
    
    // 2. Timebase edge cases (validate_config lines 706-707)
    cfg.g_timebase.num = fdp.ConsumeIntegralInRange<int>(0, 1000000001);
    cfg.g_timebase.den = fdp.ConsumeIntegralInRange<int>(0, 1000000001);
    
    // 3. Profile edge cases (validate_config line 708)
    // MAX_PROFILES is 3 (profiles 0,1,2), so use range 0-4 for edge cases
    cfg.g_profile = fdp.ConsumeIntegralInRange<unsigned int>(0, 4);
    
    // 4. Bitrate and quantization edge cases (lines 710-712)
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(0, 2000001);
    cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, 64);
    cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, 64);
    
    // Ensure min <= max for validation
    if (cfg.rc_min_quantizer > cfg.rc_max_quantizer) {
        std::swap(cfg.rc_min_quantizer, cfg.rc_max_quantizer);
    }
    
    // 5. Usage and thread edge cases (lines 719-723)
    // AOM_USAGE_ALL_INTRA is 2, so use 0-3 for edge cases
    cfg.g_usage = fdp.ConsumeIntegralInRange<unsigned int>(0, 3);
    // Use reasonable thread count range with edge case
    cfg.g_threads = fdp.ConsumeIntegralInRange<unsigned int>(0, 65);
    
    // 6. Rate control edge cases (lines 724-727)
    cfg.rc_end_usage = static_cast<aom_rc_mode>(
        fdp.ConsumeIntegralInRange<unsigned int>(0, 3)); // 3 is out of range
    cfg.rc_undershoot_pct = fdp.ConsumeIntegralInRange<unsigned int>(0, 101);
    cfg.rc_overshoot_pct = fdp.ConsumeIntegralInRange<unsigned int>(0, 101);
    cfg.rc_2pass_vbr_bias_pct = fdp.ConsumeIntegralInRange<unsigned int>(0, 101);
    
    // 7. Keyframe edge cases (lines 728-735)
    cfg.kf_mode = static_cast<aom_kf_mode>(
        fdp.ConsumeIntegralInRange<unsigned int>(0, 3)); // AOM_KF_AUTO is 2, 3 is out of range
    cfg.rc_dropframe_thresh = fdp.ConsumeIntegralInRange<unsigned int>(0, 101);
    cfg.g_pass = static_cast<aom_enc_pass>(
        fdp.ConsumeIntegralInRange<unsigned int>(0, 4)); // AOM_RC_THIRD_PASS is 3, 4 is out of range
    // MAX_LAG_BUFFERS appears to be 25 based on harness_002, use 0-26 for edge
    cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(0, 26);
    cfg.kf_max_dist = fdp.ConsumeIntegralInRange<unsigned int>(0, 1000);
    
    // 8. Bit depth edge cases (lines 800-801, 839-845)
    cfg.g_bit_depth = static_cast<aom_bit_depth_t>(
        fdp.ConsumeIntegralInRange<unsigned int>(AOM_BITS_8 - 1, AOM_BITS_12 + 1));
    cfg.g_input_bit_depth = fdp.ConsumeIntegralInRange<unsigned int>(7, 13);
    
    // 9. Monochrome and profile compatibility (lines 834-836)
    cfg.monochrome = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
    
    // 10. Fixed QP offsets with wrong end usage (lines 847-853)
    cfg.use_fixed_qp_offsets = fdp.ConsumeIntegralInRange<unsigned int>(0, 3);
    
    // 11. Superres edge cases (lines 755-761)
    cfg.rc_superres_mode = static_cast<aom_superres_mode>(
        fdp.ConsumeIntegralInRange<unsigned int>(0, 2)); // AOM_SUPERRES_AUTO is 1, 2 is out of range
    // SCALE_NUMERATOR appears to be 8 based on validate_config references, use 7-17 for edge
    cfg.rc_superres_denominator = fdp.ConsumeIntegralInRange<unsigned int>(7, 17);
    cfg.rc_superres_kf_denominator = fdp.ConsumeIntegralInRange<unsigned int>(7, 17);
    cfg.rc_superres_qthresh = fdp.ConsumeIntegralInRange<unsigned int>(0, 64);
    cfg.rc_superres_kf_qthresh = fdp.ConsumeIntegralInRange<unsigned int>(0, 64);
    
    // Step 4: Call aom_codec_enc_config_set with modified configuration to trigger validation branches
    res = aom_codec_enc_config_set(&codec, &cfg);
    // Note: We don't check result here to allow invalid configurations to trigger validation
    
    // Step 5: Optionally call aom_codec_control with various control IDs
    // Test encoder controls that may have validation logic
    
    // AV1E_SET_MODE_COST_UPD_FREQ (validate_config line 926)
    int mode_cost_upd_freq = fdp.ConsumeIntegralInRange<int>(0, 4);
    aom_codec_control(&codec, AV1E_SET_MODE_COST_UPD_FREQ, mode_cost_upd_freq);
    
    // AV1E_SET_COEFF_COST_UPD_FREQ (line 925)
    int coeff_cost_upd_freq = fdp.ConsumeIntegralInRange<int>(0, 4);
    aom_codec_control(&codec, AV1E_SET_COEFF_COST_UPD_FREQ, coeff_cost_upd_freq);
    
    // AV1E_SET_MV_COST_UPD_FREQ (line 927)
    int mv_cost_upd_freq = fdp.ConsumeIntegralInRange<int>(0, 4);
    aom_codec_control(&codec, AV1E_SET_MV_COST_UPD_FREQ, mv_cost_upd_freq);
    
    // Test other control IDs with edge values
    if (fdp.ConsumeBool()) {
        int cdf_update_mode = fdp.ConsumeIntegralInRange<int>(0, 3);
        aom_codec_control(&codec, AV1E_SET_CDF_UPDATE_MODE, cdf_update_mode);
    }
    
    if (fdp.ConsumeBool()) {
        // AQ_MODE_COUNT likely small, use 0-4
        int aq_mode = fdp.ConsumeIntegralInRange<int>(0, 4);
        aom_codec_control(&codec, AV1E_SET_AQ_MODE, aq_mode);
    }
    
    if (fdp.ConsumeBool()) {
        // DELTA_Q_MODE_COUNT likely small, use 0-4
        int deltaq_mode = fdp.ConsumeIntegralInRange<int>(0, 4);
        aom_codec_control(&codec, AV1E_SET_DELTAQ_MODE, deltaq_mode);
    }
    
    // Step 6: Call aom_codec_encode with test frames to validate configurations work
    // Try to encode a few frames even if configuration may be invalid
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
    
    // Step 7: Clean up
    aom_codec_destroy(&codec);
    aom_img_free(img);
    
    return 0;
}

} // namespace harness_006
