#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <memory>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_image.h"

namespace harness_010 {
/*
 * Fuzzing harness for libvpx VP8/VP9 video codec library
 * Target: Encoder deep coverage focusing on motion estimation and rate control paths
 * 
 * This harness specifically targets deep encoder logic paths:
 * 1. vpx_codec_enc_init_ver - Encoder initialization with varied configurations
 * 2. vpx_codec_enc_config_default - Default configuration retrieval
 * 3. vpx_codec_encode - Frame encoding with varied parameters
 * 4. vpx_codec_get_cx_data - Encoded packet retrieval
 * 5. vpx_codec_control_ - Encoder control parameter setting
 * 6. vpx_codec_destroy - Resource cleanup
 * 
 * Primary Objective: Reach blocked branches in encoder motion estimation and rate control:
 * - vp8_find_best_sub_pixel_step_iteratively (263/282 blocked)
 * - encode_with_recode_loop (completely uncovered)
 * - Other deep encoder logic paths
 * 
 * Testing Strategy:
 * 1. Use FuzzedDataProvider to generate varied encoder configurations
 * 2. Test both VP8 and VP9 codecs with different parameter combinations
 * 3. Vary encoder settings: speed, quantization, multi-pass mode, bitrate
 * 4. Create diverse frame content: flat, textured, motion patterns
 * 5. Test different frame types: keyframe, inter-frame with various flags
 * 6. Exercise vpx_codec_control_ with multiple control IDs
 * 7. Process encoded packets to test rate control paths
 * 
 * Differentiated from existing harnesses:
 * - harness_000.cpp: Basic encode/decode operations
 * - harness_001.cpp: Multi-frame encoding scenarios
 * - harness_003.cpp: VP9 encoding with external buffers
 * - harness_005.cpp: VP8 encoding with image manipulation
 * - harness_007.cpp: Encoder deep coverage (found library bug, archived)
 * - This harness focuses on deep encoder coverage: motion estimation, rate control,
 *   with fixes for high bit depth alignment issue found in harness_007
 */




// Helper functions for image plane dimensions
static int vpx_img_plane_width(const vpx_image_t *img, int plane) {
    if (plane > 0 && img->x_chroma_shift > 0)
        return (img->d_w + 1) >> img->x_chroma_shift;
    else
        return img->d_w;
}

static int vpx_img_plane_height(const vpx_image_t *img, int plane) {
    if (plane > 0 && img->y_chroma_shift > 0)
        return (img->d_h + 1) >> img->y_chroma_shift;
    else
        return img->d_h;
}

// Create test image with varied content patterns for motion estimation
static vpx_image_t* create_test_image_with_pattern(FuzzedDataProvider& fdp,
                                                   unsigned int width,
                                                   unsigned int height,
                                                   vpx_img_fmt_t fmt,
                                                   uint8_t pattern_type) {
    vpx_image_t* img = vpx_img_alloc(nullptr, fmt, width, height, 1);
    if (!img) {
        return nullptr;
    }
    
    // Fill image planes based on pattern type
    for (int plane = 0; plane < 3; ++plane) {
        int plane_width = vpx_img_plane_width(img, plane);
        int plane_height = vpx_img_plane_height(img, plane);
        size_t plane_size = plane_width * plane_height;
        
        if (plane_size == 0) continue;
        
        // Ensure we have enough data for this plane
        if (fdp.remaining_bytes() < plane_size) {
            // Not enough data, fill with a simple pattern
            uint8_t fill_value = 128;
            if (pattern_type == 1) {
                // Flat pattern
                fill_value = fdp.ConsumeIntegralInRange<uint8_t>(64, 192);
            } else if (pattern_type == 2) {
                // Gradient pattern for motion estimation
                fill_value = 64 + (plane * 64);
            }
            memset(img->planes[plane], fill_value, plane_size);
        } else {
            // Use fuzzed data for textured patterns
            auto plane_data = fdp.ConsumeBytes<uint8_t>(plane_size);
            if (pattern_type == 0) {
                // Random texture (good for motion estimation)
                memcpy(img->planes[plane], plane_data.data(), plane_size);
            } else if (pattern_type == 3) {
                // Checkerboard pattern for motion estimation
                for (size_t i = 0; i < plane_size; i++) {
                    uint8_t val = ((i / plane_width) ^ (i % plane_width)) & 1 ? 64 : 192;
                    img->planes[plane][i] = val;
                }
            } else {
                // Default: use fuzzed data
                memcpy(img->planes[plane], plane_data.data(), plane_data.size());
            }
        }
    }
    
    return img;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for encoder configuration and frame data
    if (size < 256) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume encoder configuration parameters
    uint8_t codec_type = fdp.ConsumeIntegral<uint8_t>() % 2;  // 0=VP8, 1=VP9
    uint8_t pass_mode = fdp.ConsumeIntegral<uint8_t>() % 3;   // 0=one-pass, 1=first-pass, 2=last-pass
    uint8_t speed_setting = fdp.ConsumeIntegral<uint8_t>() % 17; // 0-16 for VP8, will be adjusted
    
    // Image dimensions (capped to reasonable sizes for fuzzing)
    unsigned int width = (fdp.ConsumeIntegral<uint16_t>() % 320) + 64;
    unsigned int height = (fdp.ConsumeIntegral<uint16_t>() % 240) + 48;
    
    // Bitrate and quantization parameters
    unsigned int bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 1000000);
    unsigned int min_q = fdp.ConsumeIntegralInRange<unsigned int>(0, 63);
    unsigned int max_q = fdp.ConsumeIntegralInRange<unsigned int>(min_q, 63);
    unsigned int cq_level = fdp.ConsumeIntegralInRange<unsigned int>(0, 63);
    
    // Frame count and pattern type
    uint8_t frame_count = fdp.ConsumeIntegralInRange<uint8_t>(1, 10);
    uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 4; // 0=random, 1=flat, 2=gradient, 3=checkerboard
    
    // Select codec interface
    const vpx_codec_iface_t* codec_iface = nullptr;
    if (codec_type == 0) {
        codec_iface = vpx_codec_vp8_cx();
    } else {
        codec_iface = vpx_codec_vp9_cx();
    }
    
    if (!codec_iface) {
        return 0;
    }
    
    // Get default encoder configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res = vpx_codec_enc_config_default(codec_iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }
    
    // Modify configuration based on fuzzed parameters
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = bitrate;
    cfg.g_pass = (pass_mode == 0) ? VPX_RC_ONE_PASS : 
                 (pass_mode == 1) ? VPX_RC_FIRST_PASS : VPX_RC_LAST_PASS;
    
    // Set quantization parameters
    cfg.rc_min_quantizer = min_q;
    cfg.rc_max_quantizer = max_q;
    
    // Set VP9-specific parameters if using VP9
    if (codec_type == 1) {
        // VP9 allows up to 4 threads
        cfg.g_threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 4);
    } else {
        // VP8 supports more threads
        cfg.g_threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 8);
    }
    
    // Initialize encoder
    vpx_codec_ctx_t codec;
    vpx_codec_flags_t flags = 0;
    
    // NOTE: Removed VPX_CODEC_USE_HIGHBITDEPTH flag to avoid alignment crash
    // found in harness_007. The library has a bug when this flag is enabled
    // with 8-bit image data (CONVERT_TO_SHORTPTR macro misalignment).
    // If high bit depth testing is needed, should create actual high bit depth
    // images (VPX_IMG_FMT_I42016, etc.) instead of using 8-bit data with flag.
    
    res = vpx_codec_enc_init_ver(&codec, codec_iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        return 0;
    }
    
    // Apply encoder controls for motion estimation and rate control
    // CPU used (speed setting) - crucial for motion estimation paths
    if (codec_type == 0) {
        // VP8: -16 to 16
        int cpu_used = (int)speed_setting - 8; // Convert to -8 to 8 range
        vpx_codec_control(&codec, VP8E_SET_CPUUSED, cpu_used);
    } else {
        // VP9: -9 to 9 (positive values for speed, negative for quality)
        int cpu_used = (int)speed_setting; // 0-16, will be clamped
        vpx_codec_control(&codec, VP8E_SET_CPUUSED, cpu_used);
    }
    
    // Set noise sensitivity (affects motion estimation)
    if (codec_type == 0 && fdp.ConsumeBool()) {
        unsigned int noise_sensitivity = fdp.ConsumeIntegral<uint8_t>() % 5;
        vpx_codec_control(&codec, VP8E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    }
    
    // Set sharpness
    if (fdp.ConsumeBool()) {
        unsigned int sharpness = fdp.ConsumeIntegral<uint8_t>() % 8;
        vpx_codec_control(&codec, VP8E_SET_SHARPNESS, sharpness);
    }
    
    // Set static threshold
    if (fdp.ConsumeBool()) {
        unsigned int static_thresh = fdp.ConsumeIntegralInRange<unsigned int>(0, 1000);
        vpx_codec_control(&codec, VP8E_SET_STATIC_THRESHOLD, static_thresh);
    }
    
    // Set token partitions (VP8 only)
    if (codec_type == 0 && fdp.ConsumeBool()) {
        int token_partitions = fdp.ConsumeIntegral<uint8_t>() % 4;
        vpx_codec_control(&codec, VP8E_SET_TOKEN_PARTITIONS, token_partitions);
    }
    
    // Set auto altref (important for rate control)
    if (fdp.ConsumeBool()) {
        unsigned int enable_auto_altref = fdp.ConsumeBool() ? 1 : 0;
        if (codec_type == 0) {
            vpx_codec_control(&codec, VP8E_SET_ENABLEAUTOALTREF, enable_auto_altref);
        }
    }
    
    // Set ARNR parameters (affects motion estimation quality)
    if (fdp.ConsumeBool()) {
        unsigned int arnr_maxframes = fdp.ConsumeIntegralInRange<unsigned int>(0, 15);
        unsigned int arnr_strength = fdp.ConsumeIntegralInRange<unsigned int>(0, 6);
        vpx_codec_control(&codec, VP8E_SET_ARNR_MAXFRAMES, arnr_maxframes);
        vpx_codec_control(&codec, VP8E_SET_ARNR_STRENGTH, arnr_strength);
    }
    
    // Set CQ level (constant quality)
    if (fdp.ConsumeBool()) {
        vpx_codec_control(&codec, VP8E_SET_CQ_LEVEL, cq_level);
    }
    
    // Set max intra bitrate percentage
    if (fdp.ConsumeBool()) {
        unsigned int max_intra_bitrate_pct = fdp.ConsumeIntegralInRange<unsigned int>(0, 300);
        vpx_codec_control(&codec, VP8E_SET_MAX_INTRA_BITRATE_PCT, max_intra_bitrate_pct);
    }
    
    // Encode frames with varied content and flags
    vpx_image_t* img = nullptr;
    vpx_codec_pts_t frame_pts = 0;
    unsigned long duration = 1; // 1 tick per frame
    
    for (uint8_t i = 0; i < frame_count; i++) {
        // Create image with different pattern for each frame to create motion
        if (img) {
            vpx_img_free(img);
        }
        
        img = create_test_image_with_pattern(fdp, width, height, VPX_IMG_FMT_I420, pattern_type);
        if (!img) {
            break;
        }
        
        // Determine frame flags
        vpx_enc_frame_flags_t frame_flags = 0;
        
        if (i == 0) {
            // First frame should be a keyframe - use 0 frame flags for keyframe
            // The encoder will automatically make the first frame a keyframe
            frame_flags = 0;
        } else {
            // Randomly set frame flags to test different encoding paths
            if (fdp.ConsumeBool()) frame_flags |= VP8_EFLAG_NO_REF_LAST;
            if (fdp.ConsumeBool()) frame_flags |= VP8_EFLAG_NO_REF_GF;
            if (fdp.ConsumeBool()) frame_flags |= VP8_EFLAG_NO_REF_ARF;
            if (fdp.ConsumeBool()) frame_flags |= VP8_EFLAG_NO_UPD_LAST;
            if (fdp.ConsumeBool()) frame_flags |= VP8_EFLAG_NO_UPD_GF;
            if (fdp.ConsumeBool()) frame_flags |= VP8_EFLAG_NO_UPD_ARF;
            if (fdp.ConsumeBool()) frame_flags |= VP8_EFLAG_FORCE_GF;
            if (fdp.ConsumeBool()) frame_flags |= VP8_EFLAG_FORCE_ARF;
            if (fdp.ConsumeBool()) frame_flags |= VP8_EFLAG_NO_UPD_ENTROPY;
        }
        
        // Set frame flags via control
        vpx_codec_control(&codec, VP8E_SET_FRAME_FLAGS, (int)frame_flags);
        // Encode the frame
        res = vpx_codec_encode(&codec, img, frame_pts, duration, frame_flags, VPX_DL_REALTIME);
        
        if (res != VPX_CODEC_OK) {
            // Encoding error, but continue to test error paths
            continue;
        }
        
        // Process encoded packets to exercise rate control and packetization paths
        const vpx_codec_cx_pkt_t *pkt = nullptr;
        vpx_codec_iter_t iter = nullptr;
        
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
            // Process different packet types
            switch (pkt->kind) {
                case VPX_CODEC_CX_FRAME_PKT:
                    // Frame packet - exercise packet processing
                    (void)pkt->data.frame.pts;
                    (void)pkt->data.frame.duration;
                    (void)pkt->data.frame.flags;
                    break;
                    
                case VPX_CODEC_STATS_PKT:
                    // Stats packet - important for multi-pass encoding
                    (void)pkt->data.twopass_stats;
                    break;
                    
                case VPX_CODEC_PSNR_PKT:
                    // PSNR packet
                    (void)pkt->data.psnr;
                    break;
                    
                default:
                    // Other packet types
                    break;
            }
        }
        
        // Increment presentation timestamp
        frame_pts += duration;
        
        // Change pattern for next frame to simulate motion
        pattern_type = (pattern_type + 1) % 4;
    }
    
    // Flush the encoder to get any remaining frames
    res = vpx_codec_encode(&codec, nullptr, -1, 0, 0, VPX_DL_REALTIME);
    
    // Process any remaining packets from flush
    if (res == VPX_CODEC_OK) {
        const vpx_codec_cx_pkt_t *pkt = nullptr;
        vpx_codec_iter_t iter = nullptr;
        
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
            // Process final packets
            (void)pkt;
        }
    }
    
    // Cleanup
    if (img) {
        vpx_img_free(img);
    }
    
    vpx_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_010
