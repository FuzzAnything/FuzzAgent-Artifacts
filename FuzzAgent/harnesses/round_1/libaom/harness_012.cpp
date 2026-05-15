#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"

namespace harness_012 {
/*
 * Copyright (c) 2024 Fuzzing Harness Generator
 *
 * Targeted fuzzing harness for libaom global motion estimation pipeline.
 * Focuses on exercising FAST corner detection paths (aom_fast9_detect) via
 * aom_codec_encode to reach blocked branches in global motion estimation.
 * 
 * Primary Target: aom_fast9_detect (96% blocked branches - 1898/1974)
 * Secondary Targets: Global motion estimation pipeline via encoder
 * 
 * APIs to target:
 * - aom_codec_enc_init_ver (Required Helper: Initialize encoder)
 * - aom_img_alloc (Required Helper: Create test images with varying patterns)
 * - aom_codec_encode (Target: Main encoding path triggering global motion estimation)
 * - aom_codec_get_cx_data (Required Helper: Retrieve encoded output)
 * - aom_img_free, aom_codec_destroy (Required Helper: Cleanup)
 * 
 * Semantic differentiation from previous 12 harnesses:
 * - harness_000: Broad decoder/encoder coverage
 * - harness_001: Basic encoder operations  
 * - harness_002: Common utilities and error handling
 * - harness_003: Advanced encoder configuration and buffer management
 * - harness_004: Decoder frame buffer management and metadata operations
 * - harness_005: Encoder metadata retrieval and informational API testing
 * - harness_006: Decoder tile operations, frame information, and decoder control APIs
 * - harness_007: SVC (Scalable Video Coding) multi-layer encoding
 * - harness_008: External rate control library integration with advanced error handling
 * - harness_009: Film grain controls, denoising, chroma subsampling, and advanced encoder feature combinations
 * - harness_010: Bitstream parsing utilities, memory management APIs, and intra prediction search functions
 * - harness_011: Image manipulation APIs - wrapping, flipping, allocation with borders
 * - harness_012: Global motion estimation with FAST corner detection via varying image patterns (NEW)
 */



// Helper function to generate synthetic images with corner-like features
// FAST corner detection looks for patterns where a pixel is significantly brighter/darker
// than its surrounding circle of pixels. We generate patterns that should trigger this.
static void generate_corner_pattern_image(aom_image_t *img, uint8_t pattern_type, 
                                          uint8_t brightness_level, int frame_index) {
    if (!img || !img->planes[0]) return;
    
    // Get image dimensions
    const int width = img->d_w;
    const int height = img->d_h;
    const int stride = img->stride[0];
    uint8_t *y_plane = img->planes[0];
    
    // Base brightness influenced by fuzzed brightness_level parameter
    uint8_t base_brightness = (uint8_t)std::clamp(128 + (int)(brightness_level % 64) - 32, 0, 255); // Range: 96-159
    
    // Different pattern types to trigger various corner detection scenarios
    switch (pattern_type % 8) {
        case 0: // Checkerboard pattern (sharp corners everywhere)
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    uint8_t value = ((x / 8) % 2) ^ ((y / 8) % 2) ? 
                                   (uint8_t)std::clamp((int)base_brightness + 60, 0, 255) : 
                                   (uint8_t)std::clamp((int)base_brightness - 60, 0, 255);
                    y_plane[y * stride + x] = value;
                }
            }
            break;
            
        case 1: // Diagonal lines (edges at 45 degrees)
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int diag = (x + y) % 16;
                    uint8_t value = diag < 8 ? 
                                   (uint8_t)std::clamp((int)base_brightness + 40, 0, 255) : 
                                   (uint8_t)std::clamp((int)base_brightness - 40, 0, 255);
                    y_plane[y * stride + x] = value;
                }
            }
            break;
            
        case 2: // Concentric circles (curved edges)
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int dx = x - width / 2;
                    int dy = y - height / 2;
                    int dist = sqrt(dx*dx + dy*dy) / 8;
                    uint8_t value = (dist % 2) ? 
                                   (uint8_t)std::clamp((int)base_brightness + 50, 0, 255) : 
                                   (uint8_t)std::clamp((int)base_brightness - 50, 0, 255);
                    y_plane[y * stride + x] = value;
                }
            }
            break;
            
        case 3: // Random dot pattern (isolated corners)
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Pseudo-random based on position and frame index
                    uint32_t hash = (x * 7919 + y * 65537 + frame_index * 104729) % 100;
                    uint8_t value = hash < 5 ? 
                                   (uint8_t)std::clamp((int)base_brightness + 70, 0, 255) : 
                                   hash < 10 ? 
                                   (uint8_t)std::clamp((int)base_brightness - 70, 0, 255) : 
                                   base_brightness;
                    y_plane[y * stride + x] = value;
                }
            }
            break;
            
        case 4: // Horizontal and vertical bars (strong orthogonal edges)
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    bool is_bar = (x % 32 < 8) || (y % 32 < 8);
                    uint8_t value = is_bar ? 
                                   (uint8_t)std::clamp((int)base_brightness + 55, 0, 255) : 
                                   (uint8_t)std::clamp((int)base_brightness - 55, 0, 255);
                    y_plane[y * stride + x] = value;
                }
            }
            break;
            
        case 5: // Gradient with sudden changes (edge detection challenge)
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int grad = (x * 256 / width + y * 256 / height) / 2;
                    // Add sudden jumps at regular intervals
                    if (x % 24 == 0 || y % 24 == 0) {
                        grad = 255 - grad;
                    }
                    int pixel_value = base_brightness + (grad - 128) / 2;
                    uint8_t value = (uint8_t)std::clamp(pixel_value, 0, 255);
                    y_plane[y * stride + x] = value;
                }
            }
            break;
            
        case 6: // Simulated camera motion blur with corner features
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Motion direction varies with frame index to simulate camera pan
                    int motion_offset = (frame_index * 3) % 16;
                    int pattern_x = (x + motion_offset) % width;
                    int pattern_y = (y + motion_offset) % height;
                    
                    // Corner pattern with motion
                    bool is_corner = (pattern_x % 20 < 5) && (pattern_y % 20 < 5);
                    uint8_t value = is_corner ? 
                                   (uint8_t)std::clamp((int)base_brightness + 65, 0, 255) : 
                                   (uint8_t)std::clamp((int)base_brightness - 20, 0, 255);
                    y_plane[y * stride + x] = value;
                }
            }
            break;
            
        case 7: // Texture with varying frequency components
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    // Multiple frequency components
                    float freq1 = sin(x * 0.2f) * cos(y * 0.2f);
                    float freq2 = sin(x * 0.5f) * cos(y * 0.5f);
                    float freq3 = sin(x * 1.0f) * cos(y * 1.0f);
                    float combined = (freq1 + freq2 + freq3) / 3.0f;
                    
                    float pixel_float = base_brightness + combined * 40.0f;
                    uint8_t value = (uint8_t)std::clamp(pixel_float, 0.0f, 255.0f);
                    // Add occasional sharp corners
                    if ((x % 15 == 0) && (y % 15 == 0)) {
                        value = (uint8_t)std::clamp((float)base_brightness + 80.0f, 0.0f, 255.0f);
                    }
                    y_plane[y * stride + x] = value;
                }
            }
            break;
    }
    
    // Fill UV planes with mid-gray for simplicity (4:2:0 format)
    if (img->fmt == AOM_IMG_FMT_I420) {
        const int uv_width = width / 2;
        const int uv_height = height / 2;
        const int uv_stride = img->stride[1];
        
        for (int y = 0; y < uv_height; y++) {
            memset(img->planes[1] + y * uv_stride, 128, uv_width);
            memset(img->planes[2] + y * uv_stride, 128, uv_width);
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum input size: need enough for configuration and multiple frames
    const size_t MIN_INPUT_SIZE = sizeof(int) * 15 + 512;
    if (size < MIN_INPUT_SIZE) {
        return 0;  // Insufficient input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Get encoder interface
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) {
        return 0;
    }

    // Step 2: Get default encoder configuration
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_err_t cfg_err = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
    if (cfg_err != AOM_CODEC_OK) {
        return 0;
    }

    // Consume configuration parameters from fuzzed input
    // Fixed-size parameters first
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(64, 512);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(64, 512);
    unsigned int threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 8);
    unsigned int bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
    unsigned int fps_num = 30;
    unsigned int fps_den = 1;
    
    // Global motion estimation related parameters
    uint8_t enable_global_motion = fdp.ConsumeIntegral<uint8_t>() % 2;
    uint8_t num_frames = fdp.ConsumeIntegralInRange<uint8_t>(2, 10); // Multiple frames for motion
    uint8_t base_pattern_type = fdp.ConsumeIntegral<uint8_t>();
    uint8_t brightness_variation = fdp.ConsumeIntegral<uint8_t>();

    // Update encoder configuration
    enc_cfg.g_w = width;
    enc_cfg.g_h = height;
    enc_cfg.g_timebase.num = fps_num;
    enc_cfg.g_timebase.den = fps_den;
    enc_cfg.rc_target_bitrate = bitrate;
    enc_cfg.g_threads = threads;
    
    // Configure for global motion estimation
    // Use aom_codec_control to set relevant parameters
    enc_cfg.g_error_resilient = 0;
    
    // Set low latency mode to encourage simpler motion estimation
    enc_cfg.g_lag_in_frames = 0;

    // Step 3: Initialize encoder
    aom_codec_ctx_t encoder;
    aom_codec_err_t init_err = aom_codec_enc_init(&encoder, encoder_iface, &enc_cfg, 0);
    if (init_err != AOM_CODEC_OK) {
        return 0;
    }

    // Set encoder controls for global motion estimation
    // Enable features that might trigger FAST corner detection
    aom_codec_control(&encoder, AOME_SET_CPUUSED, 6);  // Balanced speed/quality
    aom_codec_control(&encoder, AV1E_SET_ENABLE_GLOBAL_MOTION, enable_global_motion);
    
    // Set adaptive motion search which uses feature matching
    aom_codec_control(&encoder, AV1E_SET_ENABLE_OBMC, 1);
    
    // Enable transform type search which can use corner features
    aom_codec_control(&encoder, AV1E_SET_ENABLE_TX64, 1);

    // Step 4: Create and encode multiple frames with varying patterns
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, 1);
    if (!img) {
        aom_codec_destroy(&encoder);
        return 0;
    }

    // Encode sequence of frames with camera motion simulation
    for (int frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        // Generate different pattern for each frame to simulate scene changes
        uint8_t pattern_type = (base_pattern_type + frame_idx) % 8;
        uint8_t brightness = (brightness_variation * frame_idx) % 64;
        
        // Create image with corner-like features
        generate_corner_pattern_image(img, pattern_type, brightness, frame_idx);
        
        // Encoding flags - vary based on frame index
        aom_enc_frame_flags_t flags = 0;
        if (frame_idx == 0) {
            flags |= AOM_EFLAG_FORCE_KF;  // Force keyframe on first frame
        }
        
        // Encode the frame
        aom_codec_err_t encode_err = aom_codec_encode(&encoder, img, frame_idx, 1, flags);
        
        if (encode_err == AOM_CODEC_OK) {
            // Retrieve encoded data to exercise the full pipeline
            const aom_codec_cx_pkt_t *pkt = nullptr;
            aom_codec_iter_t iter = nullptr;
            
            while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    // Encoded frame data available
                    // For fuzzing, we don't need to do anything with the data
                }
            }
        }
        
        // Simulate camera motion by slightly modifying parameters for next frame
        // This encourages global motion estimation between frames
        if (frame_idx < num_frames - 1) {
            // Small parameter changes to simulate camera movement
            width += (fdp.ConsumeIntegral<uint8_t>() % 5) - 2;
            height += (fdp.ConsumeIntegral<uint8_t>() % 5) - 2;
            
            // Clamp to reasonable values
            if (width < 64) width = 64;
            if (width > 512) width = 512;
            if (height < 64) height = 64;
            if (height > 512) height = 512;
        }
    }

    // Step 5: Flush encoder (encode remaining frames)
    bool has_more_data = true;
    while (has_more_data) {
        aom_codec_err_t encode_err = aom_codec_encode(&encoder, nullptr, -1, 1, 0);
        if (encode_err != AOM_CODEC_OK) {
            break;
        }
        
        const aom_codec_cx_pkt_t *pkt = nullptr;
        aom_codec_iter_t iter = nullptr;
        has_more_data = false;
        
        while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
            has_more_data = true;
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Process flushed frame
            }
        }
    }

    // Step 6: Cleanup
    aom_img_free(img);
    aom_codec_destroy(&encoder);

    return 0;
}

} // namespace harness_012
