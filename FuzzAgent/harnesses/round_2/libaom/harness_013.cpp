#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"

namespace harness_013 {
/*
 * Fuzzing harness for libaom AV1 flow estimation module (0% coverage area)
 * Targets completely uncovered flow estimation algorithms in aom_dsp/flow_estimation/
 * 
 * Coverage guidance focus: Flow estimation module has 0% coverage across multiple files:
 * - flow_estimation.c, corner_detect.c, corner_match.c, disflow.c, ransac.c
 * - x86 SIMD implementations: corner_match_avx2.c, disflow_sse4.c, etc.
 * 
 * Strategy to trigger flow estimation:
 * 1. Enable global motion estimation: AV1E_SET_ENABLE_GLOBAL_MOTION = 1
 * 2. Configure encoder for motion search: AOM_USAGE_REALTIME/GOOD_QUALITY with AOM_CQ/AOM_Q
 * 3. Create test frames with motion patterns: translation, rotation, scaling
 * 4. Include feature-rich scenes for corner detection
 * 5. Test different transformation types: IDENTITY, TRANSLATION, ROTZOOM, AFFINE
 * 
 * Target API call chain: aom_codec_enc_init_ver -> aom_codec_encode -> encoder_encode ->
 * global motion estimation -> flow_estimation module functions
 * 
 * Different from existing harnesses:
 * 1. Explicit focus on flow estimation (0% coverage area)
 * 2. Global motion enablement with AV1E_SET_ENABLE_GLOBAL_MOTION
 * 3. Motion pattern generation between frames
 * 4. Targeting computer vision algorithms (corner detection, feature matching, RANSAC)
 * 5. SIMD optimization path coverage (AVX2, SSE4 implementations)
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: encoder config + motion parameters + minimal frame data
    const size_t MIN_INPUT_SIZE = sizeof(uint8_t) * 20 + 512; // Parameters + small image
    if (size < MIN_INPUT_SIZE) {
        return 0;  // Not enough data for meaningful flow estimation testing
    }

    FuzzedDataProvider fdp(data, size);

    // ========== Phase 1: Encoder Configuration for Flow Estimation ==========
    // Consume fixed-size parameters first
    uint8_t usage_type = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0: REALTIME, 1: GOOD_QUALITY, 2: ALL_INTRA
    uint16_t width = fdp.ConsumeIntegralInRange<uint16_t>(64, 256);  // Optimal for motion estimation
    uint16_t height = fdp.ConsumeIntegralInRange<uint16_t>(64, 256);
    uint8_t threads = fdp.ConsumeIntegralInRange<uint8_t>(1, 4);
    uint8_t profile = fdp.ConsumeIntegral<uint8_t>() % 4;
    bool use_highbitdepth = fdp.ConsumeBool();
    
    // Flow estimation specific parameters
    bool enable_global_motion = true;  // Critical for flow estimation
    uint8_t motion_pattern = fdp.ConsumeIntegral<uint8_t>() % 5;  // 0-4 for different motion types
    uint8_t texture_type = fdp.ConsumeIntegral<uint8_t>() % 4;    // 0: corners, 1: edges, 2: noise, 3: smooth
    bool enable_warped_motion = fdp.ConsumeBool();
    bool test_different_ref_frames = fdp.ConsumeBool();
    
    // Rate control for motion estimation
    uint8_t rc_mode = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0: CQ, 1: Q, 2: VBR
    uint8_t cq_level = fdp.ConsumeIntegralInRange<uint8_t>(20, 40);  // Moderate quality
    
    // Number of frames to encode with motion between them
    uint8_t num_frames = fdp.ConsumeIntegralInRange<uint8_t>(2, 4);  // Need at least 2 for motion
    
    // Map usage type
    unsigned int usage;
    switch (usage_type) {
        case 0: usage = AOM_USAGE_REALTIME; break;
        case 1: usage = AOM_USAGE_GOOD_QUALITY; break;
        case 2: usage = AOM_USAGE_ALL_INTRA; break;
        default: usage = AOM_USAGE_REALTIME; break;
    }
    
    // Map bit depth
    aom_bit_depth_t bit_depth = use_highbitdepth ? AOM_BITS_10 : AOM_BITS_8;

    // ========== Phase 2: Encoder Initialization with Global Motion ==========
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) {
        return 0;
    }

    // Get default encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(encoder_iface, &cfg, usage) != AOM_CODEC_OK) {
        return 0;
    }

    // Configure encoder for flow estimation
    cfg.g_usage = usage;
    cfg.g_threads = threads;
    cfg.g_profile = profile;
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;  // 30 fps
    
    // Set rate control for motion estimation
    switch (rc_mode) {
        case 0:
            cfg.rc_end_usage = AOM_CQ;
            break;
        case 1:
            cfg.rc_end_usage = AOM_Q;
            break;
        case 2:
            cfg.rc_end_usage = AOM_VBR;
            cfg.rc_target_bitrate = 1000;  // Reasonable bitrate
            break;
    }
    
    // Increase motion search range for better flow estimation
    cfg.g_lag_in_frames = 2;  // Small GOP for motion testing
    cfg.rc_min_quantizer = 20;
    cfg.rc_max_quantizer = 40;

    // Initialize encoder
    aom_codec_ctx_t encoder;
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }

    // Set CQ level if using CQ or Q mode
    if (rc_mode == 0 || rc_mode == 1) {
        aom_codec_control(&encoder, AOME_SET_CQ_LEVEL, cq_level);
    }

    // ========== Phase 3: Enable Global Motion for Flow Estimation ==========
    aom_codec_control(&encoder, AV1E_SET_ENABLE_GLOBAL_MOTION, 1);
    
    // Enable warped motion if specified
    if (enable_warped_motion) {
        aom_codec_control(&encoder, AV1E_SET_ENABLE_WARPED_MOTION, 1);
        aom_codec_control(&encoder, AV1E_SET_ALLOW_WARPED_MOTION, 1);
    }
    
    // Set motion estimation settings
    aom_codec_control(&encoder, AV1E_SET_ENABLE_OBMC, 1);
    aom_codec_control(&encoder, AV1E_SET_ENABLE_PALETTE, 1);
    aom_codec_control(&encoder, AV1E_SET_ENABLE_INTRABC, 1);

    // ========== Phase 4: Create Test Frames with Motion Patterns ==========
    std::vector<aom_image_t*> frames;
    
    for (uint8_t frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        // Allocate image buffer
        aom_image_t* img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, width, height, 32);
        if (!img) {
            // Cleanup already allocated frames
            for (auto* frame : frames) {
                aom_img_free(frame);
            }
            aom_codec_destroy(&encoder);
            return 0;
        }
        
        // Fill image with pattern based on motion type and texture
        uint8_t* y_plane = img->planes[0];
        uint8_t* u_plane = img->planes[1];
        uint8_t* v_plane = img->planes[2];
        
        size_t y_size = width * height;
        size_t uv_size = (width / 2) * (height / 2);
        
        // Generate pattern based on texture type and frame index (for motion)
        for (size_t i = 0; i < y_size; i++) {
            uint8_t pattern_value = 0;
            
            // Calculate position for motion patterns
            size_t y = i / width;
            size_t x = i % width;
            
            switch (texture_type) {
                case 0:  // Corners/features (good for corner detection)
                    pattern_value = ((x % 32 == 0) || (y % 32 == 0)) ? 255 : 128;
                    // Add motion based on frame index
                    if (motion_pattern == 0) {  // Horizontal translation
                        int offset_x = frame_idx * 4;
                        pattern_value = (((x + offset_x) % 32 == 0) || (y % 32 == 0)) ? 255 : 128;
                    } else if (motion_pattern == 1) {  // Vertical translation
                        int offset_y = frame_idx * 4;
                        pattern_value = ((x % 32 == 0) || ((y + offset_y) % 32 == 0)) ? 255 : 128;
                    }
                    break;
                    
                case 1:  // Edges (good for flow estimation)
                    pattern_value = (x < width/2 || y < height/2) ? 200 : 60;
                    // Add rotation-like pattern
                    if (motion_pattern == 2) {  // Rotation-like
                        double angle = frame_idx * 0.1;
                        double rotated_x = x * std::cos(angle) - y * std::sin(angle);
                        pattern_value = (std::fabs(std::fmod(rotated_x, 32.0)) < 4.0) ? 255 : 128;
                    }
                    break;
                    
                case 2:  // Noise (challenging for flow estimation)
                    pattern_value = fdp.ConsumeIntegralInRange<uint8_t>(0, 255);
                    break;
                    
                case 3:  // Smooth with gradient
                default:
                    pattern_value = (x * 255 / width + y * 255 / height) / 2;
                    // Add scaling-like pattern
                    if (motion_pattern == 3) {  // Scaling-like
                        double scale = 1.0 + frame_idx * 0.05;
                        double scaled_x = x * scale;
                        double scaled_y = y * scale;
                        pattern_value = (std::fmod(scaled_x + scaled_y, 32.0) < 16.0) ? 200 : 80;
                    }
                    break;
            }
            
            y_plane[i] = pattern_value;
        }
        
        // Fill UV planes with simpler patterns
        for (size_t i = 0; i < uv_size; i++) {
            u_plane[i] = 128;
            v_plane[i] = 128;
        }
        
        frames.push_back(img);
    }

    // ========== Phase 5: Encode Frames with Motion Estimation ==========
    for (size_t i = 0; i < frames.size(); i++) {
        aom_image_t* img = frames[i];
        
        // Set frame flags based on position
        aom_enc_frame_flags_t flags = 0;
        if (i == 0) {
            flags = AOM_EFLAG_FORCE_KF;  // First frame as keyframe
        } else if (test_different_ref_frames) {
            // Vary reference frame usage to exercise different motion paths
            uint8_t ref_flags = fdp.ConsumeIntegral<uint8_t>() % 4;
            if (ref_flags == 0) flags |= AOM_EFLAG_NO_REF_LAST;
            if (ref_flags == 1) flags |= AOM_EFLAG_NO_REF_LAST2;
            if (ref_flags == 2) flags |= AOM_EFLAG_NO_REF_LAST3;
        }
        
        // Encode the frame (triggers motion/flow estimation)
        if (aom_codec_encode(&encoder, img, i, 1, flags) != AOM_CODEC_OK) {
            // Continue with next frame even if one fails
        }
        
        // Try to get encoded data
        const aom_codec_cx_pkt_t* pkt = nullptr;
        aom_codec_iter_t iter = nullptr;
        while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
            // Packet available, flow estimation would have been triggered
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Frame encoded successfully
            }
        }
    }

    // ========== Phase 6: Cleanup ==========
    for (auto* img : frames) {
        aom_img_free(img);
    }
    
    aom_codec_destroy(&encoder);
    
    return 0;
}

} // namespace harness_013
