#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"

namespace harness_010 {
/*
 * Fuzzing harness for AV1 encoder targeting transform type selection to exercise
 * sorting network branches av1_sort_fi32_16 with 185 undiscovered branches.
 * 
 * Target: av1_sort_fi32_16 function in sorting_network.h used in prune_tx_2D
 * during transform type selection. The SWAP macro comparisons (k[i] >= k[j])
 * need diverse input orderings to trigger all branch outcomes.
 * 
 * Call chain: aom_codec_encode → encoder_encode → ... → search_tx_type → 
 * get_tx_mask → prune_tx_2D → av1_sort_fi32_16
 * 
 * Strategy: Generate small frames (4x4 to 32x32) with focused patterns that
 * create diverse coefficient distributions affecting transform type selection.
 * The goal is to produce different orderings in the 16-element scores array
 * passed to av1_sort_fi32_16, triggering different comparison outcomes in the
 * 61 SWAP operations (122 comparison branches).
 * 
 * Performance optimization: Use very small frame sizes, limit to 1-2 frames per
 * input, and use simpler pattern generation to reduce computational overhead
 * while maintaining coverage potential for sorting network branches.
 * 
 * API sequence:
 * 1. aom_codec_enc_config_default - get default encoder configuration
 * 2. aom_codec_enc_init_ver - initialize encoder
 * 3. aom_img_alloc - create input frames with diverse patterns
 * 4. aom_codec_encode - encode frames to trigger transform type selection
 * 5. aom_codec_destroy - cleanup
 * 6. aom_img_free - cleanup frames
 */




int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need enough input for configuration and at least basic image data
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Get AV1 encoder interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Create encoder context
    aom_codec_ctx_t codec;

    // Get default configuration
    aom_codec_enc_cfg_t cfg;
    unsigned int usage = fdp.ConsumeIntegralInRange<unsigned int>(0, 2); // GOOD_QUALITY=0, REALTIME=1, ALL_INTRA=2
    
    aom_codec_err_t cfg_result = aom_codec_enc_config_default(iface, &cfg, usage);
    if (cfg_result != AOM_CODEC_OK) {
        return 0;
    }

    // Use very small frame sizes for performance while still exercising transform selection
    // Small blocks (4x4 to 32x32) are where transform type selection is most active
    int size_choice = fdp.ConsumeIntegralInRange<int>(0, 3);
    switch (size_choice) {
        case 0:
            cfg.g_w = 4;
            cfg.g_h = 4;
            break;
        case 1:
            cfg.g_w = 8;
            cfg.g_h = 8;
            break;
        case 2:
            cfg.g_w = 16;
            cfg.g_h = 16;
            break;
        case 3:
        default:
            cfg.g_w = 32;
            cfg.g_h = 32;
            break;
    }
    
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 1000);
    
    // Enable transform type search
    cfg.g_usage = usage;
    
    // Set quality parameters that affect transform selection
    cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(20, 63);
    cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, cfg.rc_max_quantizer);
    
    // Initialize encoder
    aom_codec_err_t init_result = aom_codec_enc_init_ver(&codec, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (init_result != AOM_CODEC_OK) {
        return 0;
    }

    // Set CPU usage (affects transform search quality/speed tradeoff)
    int cpu_used = fdp.ConsumeIntegralInRange<int>(0, 9);
    aom_codec_control(&codec, AOME_SET_CPUUSED, cpu_used);

    // Create image for encoding
    aom_image_t img;
    if (!aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1)) {
        aom_codec_destroy(&codec);
        return 0;
    }

    // Generate 1-2 frames max for performance
    int num_frames = fdp.ConsumeIntegralInRange<int>(1, 2);
    
    for (int frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        if (fdp.remaining_bytes() < 64) {
            break; // Not enough input for another frame
        }

        // Calculate plane sizes
        size_t y_plane_size = cfg.g_w * cfg.g_h;
        size_t uv_plane_size = (cfg.g_w / 2) * (cfg.g_h / 2);
        
        // Check if we have enough input for this pattern
        if (fdp.remaining_bytes() < y_plane_size) {
            break;
        }

        // Generate Y plane (luma) with patterns designed to affect transform type selection
        // We focus on patterns that create different energy distributions to influence
        // the scores array passed to av1_sort_fi32_16
        uint8_t* y_plane = img.planes[0];
        uint8_t* u_plane = img.planes[1];
        uint8_t* v_plane = img.planes[2];
        
        // Choose pattern type - each creates different coefficient distributions
        int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 4);
        
        switch (pattern_type) {
            case 0: {
                // Pattern 0: Horizontal gradient - favors horizontal transforms
                float start_val = fdp.ConsumeFloatingPointInRange<float>(32.0f, 224.0f);
                float end_val = fdp.ConsumeFloatingPointInRange<float>(32.0f, 224.0f);
                
                for (size_t i = 0; i < y_plane_size; i++) {
                    int x = i % cfg.g_w;
                    float t = static_cast<float>(x) / cfg.g_w;
                    float value = start_val + t * (end_val - start_val);
                    y_plane[i] = static_cast<uint8_t>(fmax(0, fmin(255, value)));
                }
                break;
            }
            
            case 1: {
                // Pattern 1: Vertical gradient - favors vertical transforms
                float start_val = fdp.ConsumeFloatingPointInRange<float>(32.0f, 224.0f);
                float end_val = fdp.ConsumeFloatingPointInRange<float>(32.0f, 224.0f);
                
                for (size_t i = 0; i < y_plane_size; i++) {
                    int y = i / cfg.g_w;
                    float t = static_cast<float>(y) / cfg.g_h;
                    float value = start_val + t * (end_val - start_val);
                    y_plane[i] = static_cast<uint8_t>(fmax(0, fmin(255, value)));
                }
                break;
            }
            
            case 2: {
                // Pattern 2: Diagonal pattern - creates mixed horizontal/vertical energy
                float base_val = fdp.ConsumeFloatingPointInRange<float>(64.0f, 192.0f);
                float amp = fdp.ConsumeFloatingPointInRange<float>(10.0f, 100.0f);
                float freq = fdp.ConsumeFloatingPointInRange<float>(1.0f, 4.0f);
                
                for (size_t i = 0; i < y_plane_size; i++) {
                    int x = i % cfg.g_w;
                    int y = i / cfg.g_w;
                    float value = base_val + amp * sin(2.0f * M_PI * freq * (x + y) / (cfg.g_w + cfg.g_h));
                    y_plane[i] = static_cast<uint8_t>(fmax(0, fmin(255, value)));
                }
                break;
            }
            
            case 3: {
                // Pattern 3: Checkerboard - creates high-frequency content
                int block_size = fdp.ConsumeIntegralInRange<int>(1, 4);
                float dark_val = fdp.ConsumeFloatingPointInRange<float>(32.0f, 96.0f);
                float light_val = fdp.ConsumeFloatingPointInRange<float>(160.0f, 224.0f);
                
                for (size_t i = 0; i < y_plane_size; i++) {
                    int x = i % cfg.g_w;
                    int y = i / cfg.g_w;
                    int block_x = x / block_size;
                    int block_y = y / block_size;
                    bool is_dark = ((block_x + block_y) % 2) == 0;
                    float value = is_dark ? dark_val : light_val;
                    y_plane[i] = static_cast<uint8_t>(fmax(0, fmin(255, value)));
                }
                break;
            }
            
            case 4: {
                // Pattern 4: Random noise from fuzzer input - creates unpredictable distributions
                // This should create diverse coefficient orderings for the sorting network
                if (fdp.remaining_bytes() >= y_plane_size) {
                    std::vector<uint8_t> noise_data = fdp.ConsumeBytes<uint8_t>(y_plane_size);
                    memcpy(y_plane, noise_data.data(), y_plane_size);
                } else {
                    // Fallback to simple pattern if not enough input
                    for (size_t i = 0; i < y_plane_size; i++) {
                        y_plane[i] = fdp.ConsumeIntegral<uint8_t>();
                    }
                }
                break;
            }
        }
        
        // Fill UV planes with constant color (simplified for performance)
        memset(u_plane, 128, uv_plane_size);
        memset(v_plane, 128, uv_plane_size);
        
        // Encode the frame
        aom_codec_flags_t flags = 0;
        
        // First frame as keyframe
        if (frame_idx == 0) {
            flags |= AOM_EFLAG_FORCE_KF;
        }
        
        // Encode with short duration
        long duration = 1;
        aom_codec_encode(&codec, &img, frame_idx * duration, duration, flags);
        
        // Don't check encode_result - we want to exercise the transform selection
        // path even if encoding fails due to unusual patterns
    }
    
    // Flush encoder
    aom_codec_encode(&codec, NULL, -1, 1, 0);
    
    // Clean up
    aom_img_free(&img);
    aom_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_010
