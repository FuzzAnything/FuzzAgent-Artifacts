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

namespace harness_013 {
/*
 * Fuzzing harness for AV1 encoder targeting av1_sort_fi32_16 sorting network
 * with 465 blocked branches (71.5% blocked) through direct transform type
 * selection path (prune_tx_2D).
 * 
 * Target: av1_sort_fi32_16 function in sorting_network.h called from prune_tx_2D
 * in tx_search.c during transform type selection. Each of the 61 SWAP operations
 * has conditional branches (k[i] >= k[j]) that need diverse input orderings.
 * 
 * Call chain: aom_codec_encode → encoder_encode → search_tx_type → 
 * get_tx_mask → prune_tx_2D → av1_sort_fi32_16
 * 
 * Strategy: Generate focused image patterns that directly affect the 16-element
 * scores array in prune_tx_2D. Small block sizes (4x4 to 32x32) with specific
 * texture patterns to produce diverse coefficient orderings that exercise both
 * true and false comparison branches in the sorting network.
 * 
 * Key insight: The scores array contains transform type probabilities derived
 * from energy distribution features. By creating images with specific directional
 * patterns (horizontal, vertical, diagonal edges, textures), we influence the
 * energy distribution and thus the transform type scores ordering.
 * 
 * Performance optimization: Use minimal frame sizes (64x64 max) and encode only
 * 1-2 frames per input to maximize fuzzing throughput while hitting the target
 * sorting network branches.
 */




// Generate image with specific pattern to affect transform type scores
void generate_patterned_image(aom_image_t* img, FuzzedDataProvider& fdp, 
                             int pattern_type, int width, int height) {
    // Y plane (luma) has the most impact on transform selection
    uint8_t* y_plane = img->planes[0];
    int y_stride = img->stride[0];
    
    switch (pattern_type % 6) {
        case 0: {
            // Strong horizontal patterns - favors horizontal transforms
            for (int y = 0; y < height; y++) {
                uint8_t row_value = 128 + 127 * sin(2.0f * M_PI * y / 16.0f);
                for (int x = 0; x < width; x++) {
                    y_plane[y * y_stride + x] = row_value;
                }
            }
            break;
        }
        
        case 1: {
            // Strong vertical patterns - favors vertical transforms  
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    uint8_t col_value = 128 + 127 * sin(2.0f * M_PI * x / 16.0f);
                    y_plane[y * y_stride + x] = col_value;
                }
            }
            break;
        }
        
        case 2: {
            // Diagonal patterns - tests mixed directional transforms
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    uint8_t diag_value = 128 + 127 * sin(2.0f * M_PI * (x + y) / 32.0f);
                    y_plane[y * y_stride + x] = diag_value;
                }
            }
            break;
        }
        
        case 3: {
            // Checkerboard pattern - creates high frequency content
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    y_plane[y * y_stride + x] = ((x / 4 + y / 4) % 2) * 255;
                }
            }
            break;
        }
        
        case 4: {
            // Gradient pattern - smooth transitions
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    float norm_x = (float)x / width;
                    float norm_y = (float)y / height;
                    y_plane[y * y_stride + x] = 255 * norm_x * norm_y;
                }
            }
            break;
        }
        
        case 5: {
            // Random pattern from fuzzer input - maximum diversity
            std::vector<uint8_t> random_data = fdp.ConsumeBytes<uint8_t>(width * height);
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int idx = y * width + x;
                    if (idx < (int)random_data.size()) {
                        y_plane[y * y_stride + x] = random_data[idx];
                    } else {
                        y_plane[y * y_stride + x] = 128;
                    }
                }
            }
            break;
        }
    }
    
    // Fill UV planes with simple data (less critical for transform selection)
    if (img->planes[1] && img->planes[2]) {
        int uv_width = width / 2;
        int uv_height = height / 2;
        int u_stride = img->stride[1];
        int v_stride = img->stride[2];
        
        for (int y = 0; y < uv_height; y++) {
            for (int x = 0; x < uv_width; x++) {
                img->planes[1][y * u_stride + x] = 128;
                img->planes[2][y * v_stride + x] = 128;
            }
        }
    }
}

// Test encoding with focus on transform type selection paths
void test_transform_selection_focused(FuzzedDataProvider& fdp) {
    // Need minimum input for configuration
    if (fdp.remaining_bytes() < 128) {
        return;
    }
    
    // Get AV1 encoder interface
    aom_codec_iface_t* iface = aom_codec_av1_cx();
    if (!iface) {
        return;
    }
    
    // Create encoder context
    aom_codec_ctx_t codec;
    
    // Get default configuration
    aom_codec_enc_cfg_t cfg;
    unsigned int usage = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
    
    if (aom_codec_enc_config_default(iface, &cfg, usage) != AOM_CODEC_OK) {
        return;
    }
    
    // Critical: Use small frame sizes to focus on transform type selection
    // Transform selection happens at block level (4x4 to 32x32)
    // Using small overall frames increases density of transform decisions
    int size_choice = fdp.ConsumeIntegralInRange<int>(0, 3);
    switch (size_choice) {
        case 0:
            cfg.g_w = 32;
            cfg.g_h = 32;
            break;
        case 1:
            cfg.g_w = 64;
            cfg.g_h = 64;
            break;
        case 2:
            cfg.g_w = 96;
            cfg.g_h = 96;
            break;
        case 3:
        default:
            cfg.g_w = 128;
            cfg.g_h = 128;
            break;
    }
    
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 1000);
    
    // Settings that affect transform type selection
    cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(0, 1); // 0-1 for intra focus
    cfg.g_error_resilient = 0; // Standard encoding
    
    // Quality parameters that affect transform pruning
    cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(20, 50);
    cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, cfg.rc_max_quantizer / 2);
    
    // Initialize encoder
    if (aom_codec_enc_init(&codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        return;
    }
    
    // Set CPU usage - lower values enable more thorough transform search
    int cpu_used = fdp.ConsumeIntegralInRange<int>(0, 6); // 0-6 for better quality
    aom_codec_control(&codec, AOME_SET_CPUUSED, cpu_used);
    
    // Create image
    aom_image_t img;
    if (!aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1)) {
        aom_codec_destroy(&codec);
        return;
    }
    
    // Generate image with pattern that affects transform type selection
    int pattern_type = fdp.ConsumeIntegralInRange<int>(0, 5);
    generate_patterned_image(&img, fdp, pattern_type, cfg.g_w, cfg.g_h);
    
    // Encode 1-2 frames to exercise transform selection
    int frames_to_encode = fdp.ConsumeIntegralInRange<int>(1, 2);
    
    for (int frame_idx = 0; frame_idx < frames_to_encode; frame_idx++) {
        aom_enc_frame_flags_t flags = 0;
        
        // Force keyframe occasionally to reset encoder state
        if (frame_idx == 0 && fdp.ConsumeBool()) {
            flags |= AOM_EFLAG_FORCE_KF;
        }
        
        // Set presentation timestamp
        aom_codec_pts_t pts = frame_idx * 1000;
        unsigned long duration = 1;
        
        // Critical: This encode call will trigger transform type selection
        // which calls prune_tx_2D → av1_sort_fi32_16
        aom_codec_encode(&codec, &img, pts, duration, flags);
        
        // Drain encoder output
        const aom_codec_cx_pkt_t* pkt = NULL;
        aom_codec_iter_t iter = NULL;
        while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Just consume packets
        }
    }
    
    // Clean up
    aom_img_free(&img);
    aom_codec_destroy(&codec);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) {
        return 0; // Too small for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Run focused transform selection test
    test_transform_selection_focused(fdp);
    
    return 0;
}

} // namespace harness_013
