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

namespace harness_009 {
/*
 * Fuzzing harness for AV1 encoder targeting transform type selection to exercise
 * sorting network branches av1_sort_fi32_16 and av1_sort_fi32_8.
 * 
 * Target: av1_sort_fi32_16 with 461 blocked branches (71% blocked) and 
 * av1_sort_fi32_8 with 171 blocked branches (90% blocked) in SWAP macro comparisons.
 * These functions are called during transform type selection (search_tx_type) 
 * via prune_tx_2D -> get_tx_mask path.
 * 
 * Strategy: Generate small frames with varied pixel patterns (edges, textures, 
 * flat areas) to influence transform type selection and create diverse coefficient
 * orderings that exercise both true and false branches in sorting network comparisons.
 * 
 * Focus on performance: Use small frame sizes (64x64, 128x128) and varied patterns
 * to keep fuzzing performance reasonable while maximizing coverage of sorting network branches.
 * 
 * API sequence:
 * 1. aom_codec_enc_init_ver - initialize encoder
 * 2. aom_codec_enc_config_default - get default config  
 * 3. Configure encoder for transform type search sensitivity
 * 4. aom_img_alloc - create input frames with diverse patterns
 * 5. aom_codec_encode repeatedly with varied image patterns
 * 6. aom_codec_destroy - cleanup
 * 7. aom_img_free - cleanup frames
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need enough input for image generation and configuration
    if (size < 256) {
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
    // Use small frame sizes to focus on transform type selection while maintaining performance
    int frame_size_choice = fdp.ConsumeIntegralInRange<int>(0, 2);
    switch (frame_size_choice) {
        case 0:
            cfg.g_w = 64;
            cfg.g_h = 64;
            break;
        case 1:
            cfg.g_w = 128;
            cfg.g_h = 128;
            break;
        case 2:
        default:
            cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(32, 128);
            cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(32, 128);
            break;
    }
    
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
    int num_frames = fdp.ConsumeIntegralInRange<int>(1, 8);
    
    for (int frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        if (fdp.remaining_bytes() < 128) {
            break; // Not enough input for another frame
        }

        // Calculate plane sizes
        size_t y_plane_size = cfg.g_w * cfg.g_h;
        size_t uv_plane_size = (cfg.g_w / 2) * (cfg.g_h / 2);
        
        // Check if we have enough input for this pattern
        if (fdp.remaining_bytes() < y_plane_size + 2 * uv_plane_size) {
            break;
        }

        // Generate Y plane (luma) with different patterns focused on transform type selection
        uint8_t* y_plane = img.planes[0];
        uint8_t* u_plane = img.planes[1];
        uint8_t* v_plane = img.planes[2];
        
        // Choose pattern type specifically designed to affect transform type selection
        int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 5);
        
        switch (pattern_type) {
            case 0: {
                // Pattern 0: High-frequency texture patterns
                // Creates many non-zero DCT coefficients affecting transform type decisions
                float freq_x = fdp.ConsumeFloatingPointInRange<float>(1.0f, 8.0f);
                float freq_y = fdp.ConsumeFloatingPointInRange<float>(1.0f, 8.0f);
                float phase = fdp.ConsumeFloatingPointInRange<float>(0.0f, 2.0f * M_PI);
                
                for (size_t i = 0; i < y_plane_size; i++) {
                    int x = i % cfg.g_w;
                    int y = i / cfg.g_w;
                    
                    // Create high-frequency texture
                    float value = 128.0f + 100.0f * 
                        sin(2.0f * M_PI * freq_x * x / cfg.g_w + phase) *
                        cos(2.0f * M_PI * freq_y * y / cfg.g_h);
                    
                    y_plane[i] = static_cast<uint8_t>(fmax(0, fmin(255, value)));
                }
                break;
            }
            
            case 1: {
                // Pattern 1: Strong directional edges at various angles
                // Produces directional patterns that favor specific transform types
                float angle = fdp.ConsumeFloatingPointInRange<float>(0.0f, 2.0f * M_PI);
                float edge_strength = fdp.ConsumeFloatingPointInRange<float>(0.5f, 2.0f);
                int edge_count = fdp.ConsumeIntegralInRange<int>(1, 5);
                
                for (size_t i = 0; i < y_plane_size; i++) {
                    int x = i % cfg.g_w;
                    int y = i / cfg.g_w;
                    
                    float value = 128.0f;
                    for (int e = 0; e < edge_count; e++) {
                        float offset = e * cfg.g_h / (edge_count + 1);
                        float dist = (x * cos(angle) + (y - offset) * sin(angle));
                        value += 60.0f * edge_strength * tanh(dist / 5.0f);
                    }
                    
                    y_plane[i] = static_cast<uint8_t>(fmax(0, fmin(255, value)));
                }
                break;
            }
            
            case 2: {
                // Pattern 2: Checkerboard pattern with varying block sizes
                // Creates alternating patterns that affect transform type decisions
                int block_size = fdp.ConsumeIntegralInRange<int>(2, 16);
                float contrast = fdp.ConsumeFloatingPointInRange<float>(0.1f, 1.0f);
                
                for (size_t i = 0; i < y_plane_size; i++) {
                    int x = i % cfg.g_w;
                    int y = i / cfg.g_w;
                    
                    int block_x = x / block_size;
                    int block_y = y / block_size;
                    bool is_dark = ((block_x + block_y) % 2) == 0;
                    
                    float base = is_dark ? 64.0f : 192.0f;
                    float noise = (fdp.ConsumeIntegral<uint8_t>() / 255.0f - 0.5f) * 50.0f * contrast;
                    float value = base + noise;
                    
                    y_plane[i] = static_cast<uint8_t>(fmax(0, fmin(255, value)));
                }
                break;
            }
            
            case 3: {
                // Pattern 3: Gradient with varying directions and curvature
                // Produces smoothly varying coefficients for different transform type selections
                int grad_type = fdp.ConsumeIntegralInRange<int>(0, 3);
                float grad_start = fdp.ConsumeFloatingPointInRange<float>(32.0f, 224.0f);
                float grad_end = fdp.ConsumeFloatingPointInRange<float>(32.0f, 224.0f);
                float curvature = fdp.ConsumeFloatingPointInRange<float>(-1.0f, 1.0f);
                
                for (size_t i = 0; i < y_plane_size; i++) {
                    int x = i % cfg.g_w;
                    int y = i / cfg.g_w;
                    float t;
                    
                    switch (grad_type) {
                        case 0: // horizontal linear
                            t = static_cast<float>(x) / cfg.g_w;
                            break;
                        case 1: // vertical linear
                            t = static_cast<float>(y) / cfg.g_h;
                            break;
                        case 2: // diagonal
                            t = static_cast<float>(x + y) / (cfg.g_w + cfg.g_h);
                            break;
                        case 3: // radial with curvature
                        default:
                            float dx = x - cfg.g_w / 2.0f;
                            float dy = y - cfg.g_h / 2.0f;
                            float dist = sqrt(dx*dx + dy*dy);
                            float max_dist = sqrt(cfg.g_w*cfg.g_w/4.0f + cfg.g_h*cfg.g_h/4.0f);
                            t = dist / max_dist;
                            t = t + curvature * t * (1.0f - t); // apply curvature
                            break;
                    }
                    
                    // Apply easing function for more varied distributions
                    float eased_t = t < 0.5f ? 2.0f * t * t : 1.0f - pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
                    float value = grad_start + eased_t * (grad_end - grad_start);
                    
                    y_plane[i] = static_cast<uint8_t>(fmax(0, fmin(255, value)));
                }
                break;
            }
            
            case 4: {
                // Pattern 4: Sparse pattern with isolated features
                // Creates few non-zero coefficients affecting transform type selection
                int feature_count = fdp.ConsumeIntegralInRange<int>(1, 10);
                float feature_intensity = fdp.ConsumeFloatingPointInRange<float>(50.0f, 200.0f);
                
                // Start with flat background
                memset(y_plane, 128, y_plane_size);
                
                for (int f = 0; f < feature_count; f++) {
                    int center_x = fdp.ConsumeIntegralInRange<int>(0, cfg.g_w - 1);
                    int center_y = fdp.ConsumeIntegralInRange<int>(0, cfg.g_h - 1);
                    float radius = fdp.ConsumeFloatingPointInRange<float>(1.0f, 10.0f);
                    
                    for (size_t i = 0; i < y_plane_size; i++) {
                        int x = i % cfg.g_w;
                        int y = i / cfg.g_w;
                        
                        float dx = x - center_x;
                        float dy = y - center_y;
                        float dist = sqrt(dx*dx + dy*dy);
                        
                        if (dist < radius) {
                            float gaussian = exp(-dist*dist / (2.0f * radius * radius));
                            float delta = feature_intensity * gaussian;
                            float current = y_plane[i];
                            y_plane[i] = static_cast<uint8_t>(fmax(0, fmin(255, current + delta)));
                        }
                    }
                }
                break;
            }
            
            case 5: {
                // Pattern 5: Mixed frequency content
                // Combines low and high frequencies to create complex coefficient distributions
                float low_freq = fdp.ConsumeFloatingPointInRange<float>(0.5f, 2.0f);
                float high_freq = fdp.ConsumeFloatingPointInRange<float>(4.0f, 16.0f);
                float mix_ratio = fdp.ConsumeFloatingPointInRange<float>(0.0f, 1.0f);
                
                for (size_t i = 0; i < y_plane_size; i++) {
                    int x = i % cfg.g_w;
                    int y = i / cfg.g_w;
                    
                    float low_freq_val = 128.0f + 80.0f * 
                        sin(2.0f * M_PI * low_freq * x / cfg.g_w) *
                        cos(2.0f * M_PI * low_freq * y / cfg.g_h);
                    
                    float high_freq_val = 128.0f + 40.0f * 
                        sin(2.0f * M_PI * high_freq * x / cfg.g_w) *
                        cos(2.0f * M_PI * high_freq * y / cfg.g_h);
                    
                    float value = mix_ratio * high_freq_val + (1.0f - mix_ratio) * low_freq_val;
                    
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
        
        // Encode the frame with varying parameters to affect transform type search
        aom_codec_flags_t flags = 0;
        
        // Vary frame type to trigger different transform search behaviors
        if (frame_idx == 0) {
            flags |= AOM_EFLAG_FORCE_KF; // First frame as keyframe
        } else if (fdp.ConsumeBool()) {
            flags |= AOM_EFLAG_NO_UPD_LAST | AOM_EFLAG_NO_UPD_GF | AOM_EFLAG_NO_UPD_ARF;
        }
        
        // Encode with varying duration
        long duration = fdp.ConsumeIntegralInRange<long>(1, 4);
        aom_codec_err_t encode_result = aom_codec_encode(&codec, &img, frame_idx * duration, duration, flags);
        
        // Don't check encode_result - we want to continue even if encoding fails
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

} // namespace harness_009
