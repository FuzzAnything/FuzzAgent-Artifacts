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

namespace harness_008 {
/*
 * Fuzzing harness for AV1 encoder targeting av1_sort_fi32_16 sorting network branch coverage
 * 
 * Target: av1_sort_fi32_16 with 461 blocked branches (71% blocked) in SWAP macro comparisons
 * This function is called during transform type selection (search_tx_type) and sorts 16 float keys.
 * Each of the 60+ comparisons in the sorting network has a branch on (k[i] >= k[j]).
 * 
 * Strategy: Generate varied image patterns that produce different coefficient orderings
 * to trigger both true and false branches in SWAP macro comparisons.
 * 
 * API sequence:
 * 1. aom_codec_enc_init_ver - initialize encoder
 * 2. aom_codec_enc_config_default - get default config
 * 3. Configure encoder for transform type search sensitivity
 * 4. aom_codec_encode repeatedly with varied image patterns
 * 5. aom_codec_destroy - cleanup
 */




int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need enough input for image generation and configuration
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

    // Configure encoder for transform type search sensitivity
    // Use small frame sizes to focus on transform type selection
    cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(32, 128);
    cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(32, 128);
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
    
    // Enable transform type search by setting appropriate parameters
    cfg.g_usage = usage;
    
    // Set quality/compression parameters that affect transform selection
    cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(20, 63);
    cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, cfg.rc_max_quantizer);
    
    // Initialize encoder
    aom_codec_err_t init_result = aom_codec_enc_init_ver(&codec, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (init_result != AOM_CODEC_OK) {
        return 0;
    }

    // Set CPU usage (affects transform search speed/quality tradeoff)
    int cpu_used = fdp.ConsumeIntegralInRange<int>(0, 9);
    aom_codec_control(&codec, AOME_SET_CPUUSED, cpu_used);

    // Create image for encoding
    aom_image_t img;
    if (!aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1)) {
        aom_codec_destroy(&codec);
        return 0;
    }

    // Generate multiple frames with different patterns to exercise various coefficient distributions
    int num_frames = fdp.ConsumeIntegralInRange<int>(1, 5);
    
    for (int frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        if (fdp.remaining_bytes() < 64) {
            break; // Not enough input for another frame
        }

        // Choose pattern type for this frame
        int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 3);
        
        // Calculate plane sizes
        size_t y_plane_size = cfg.g_w * cfg.g_h;
        size_t uv_plane_size = (cfg.g_w / 2) * (cfg.g_h / 2);
        
        // Check if we have enough input for this pattern
        if (fdp.remaining_bytes() < y_plane_size + 2 * uv_plane_size) {
            break;
        }

        // Generate Y plane (luma) with different patterns
        uint8_t* y_plane = img.planes[0];
        uint8_t* u_plane = img.planes[1];
        uint8_t* v_plane = img.planes[2];
        
        switch (pattern_type) {
            case 0: {
                // Pattern 0: Synthetic images with alternating high/low frequency content
                // Creates coefficients with varying magnitudes for sorting
                float freq_mult = fdp.ConsumeFloatingPointInRange<float>(0.1f, 10.0f);
                float amp_mult = fdp.ConsumeFloatingPointInRange<float>(0.1f, 2.0f);
                
                for (size_t i = 0; i < y_plane_size; i++) {
                    int x = i % cfg.g_w;
                    int y = i / cfg.g_w;
                    
                    // Create alternating frequency pattern
                    float value = 128.0f + 127.0f * amp_mult * 
                        sin(2.0f * M_PI * freq_mult * x / cfg.g_w) *
                        cos(2.0f * M_PI * freq_mult * y / cfg.g_h);
                    
                    y_plane[i] = static_cast<uint8_t>(fmax(0, fmin(255, value)));
                }
                break;
            }
            
            case 1: {
                // Pattern 1: Edge patterns at different orientations
                // Produces directional coefficients that sort differently
                float angle = fdp.ConsumeFloatingPointInRange<float>(0.0f, 2.0f * M_PI);
                float edge_width = fdp.ConsumeFloatingPointInRange<float>(1.0f, 20.0f);
                int edge_x = fdp.ConsumeIntegralInRange<int>(0, cfg.g_w);
                int edge_y = fdp.ConsumeIntegralInRange<int>(0, cfg.g_h);
                
                float cos_angle = cos(angle);
                float sin_angle = sin(angle);
                
                for (size_t i = 0; i < y_plane_size; i++) {
                    int x = i % cfg.g_w;
                    int y = i / cfg.g_w;
                    
                    // Calculate distance to edge line
                    float dx = x - edge_x;
                    float dy = y - edge_y;
                    float dist = fabs(dx * cos_angle + dy * sin_angle);
                    
                    // Create smooth edge transition
                    float value = 128.0f + 127.0f * tanh(dist / edge_width);
                    y_plane[i] = static_cast<uint8_t>(fmax(0, fmin(255, value)));
                }
                break;
            }
            
            case 2: {
                // Pattern 2: Random noise with varying statistical properties
                // Creates coefficients with random ordering for sorting network
                float mean = fdp.ConsumeFloatingPointInRange<float>(64.0f, 192.0f);
                float stddev = fdp.ConsumeFloatingPointInRange<float>(1.0f, 64.0f);
                
                // Consume random bytes for noise
                std::vector<uint8_t> noise_data = fdp.ConsumeBytes<uint8_t>(y_plane_size);
                
                for (size_t i = 0; i < y_plane_size; i++) {
                    // Add Gaussian-like noise to mean
                    float noise = (noise_data[i] / 255.0f - 0.5f) * 2.0f;
                    float value = mean + stddev * noise;
                    y_plane[i] = static_cast<uint8_t>(fmax(0, fmin(255, value)));
                }
                break;
            }
            
            case 3: {
                // Pattern 3: Gradients to produce ordered coefficient distributions
                // Creates smoothly varying coefficients for predictable sorting
                int grad_dir = fdp.ConsumeIntegralInRange<int>(0, 3); // 0: horizontal, 1: vertical, 2: diagonal, 3: radial
                float grad_start = fdp.ConsumeFloatingPointInRange<float>(0.0f, 255.0f);
                float grad_end = fdp.ConsumeFloatingPointInRange<float>(0.0f, 255.0f);
                
                for (size_t i = 0; i < y_plane_size; i++) {
                    int x = i % cfg.g_w;
                    int y = i / cfg.g_w;
                    float t;
                    
                    switch (grad_dir) {
                        case 0: // horizontal
                            t = static_cast<float>(x) / cfg.g_w;
                            break;
                        case 1: // vertical
                            t = static_cast<float>(y) / cfg.g_h;
                            break;
                        case 2: // diagonal
                            t = static_cast<float>(x + y) / (cfg.g_w + cfg.g_h);
                            break;
                        case 3: // radial
                        default:
                            float dx = x - cfg.g_w / 2.0f;
                            float dy = y - cfg.g_h / 2.0f;
                            float dist = sqrt(dx*dx + dy*dy);
                            t = dist / sqrt(cfg.g_w*cfg.g_w/4.0f + cfg.g_h*cfg.g_h/4.0f);
                            break;
                    }
                    
                    float value = grad_start + t * (grad_end - grad_start);
                    y_plane[i] = static_cast<uint8_t>(fmax(0, fmin(255, value)));
                }
                break;
            }
        }
        
        // Fill UV planes (chroma) with simpler patterns
        // Use remaining fuzzer input for UV planes
        if (fdp.remaining_bytes() >= 2 * uv_plane_size) {
            std::vector<uint8_t> u_data = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
            std::vector<uint8_t> v_data = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
            
            memcpy(u_plane, u_data.data(), uv_plane_size);
            memcpy(v_plane, v_data.data(), uv_plane_size);
        } else {
            // Fill with constant color if not enough input
            memset(u_plane, 128, uv_plane_size);
            memset(v_plane, 128, uv_plane_size);
        }
        
        // Encode the frame
        // Set frame flags: vary between keyframes and inter-frames
        aom_codec_flags_t flags = 0;
        if (frame_idx == 0 || fdp.ConsumeBool()) {
            flags |= AOM_EFLAG_FORCE_KF; // Force keyframe
        }
        
        // Encode with varying duration and deadline
        aom_codec_err_t encode_result = aom_codec_encode(&codec, &img, frame_idx, 1, flags);
        
        // We don't check encode_result because we want to continue even if encoding fails
        // Some patterns may produce invalid coefficients that cause encoding errors
        // This is acceptable as we're testing the transform type selection path
    }
    
    // Flush encoder (encode NULL frame)
    aom_codec_encode(&codec, NULL, -1, 1, 0);
    
    // Clean up
    aom_img_free(&img);
    aom_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_008
