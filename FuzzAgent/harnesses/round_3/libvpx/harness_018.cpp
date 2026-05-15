#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <algorithm>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8cx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_image.h"

namespace harness_018 {
/*
 * Fuzzing harness for libvpx deep algorithmic coverage - FINAL HARNESS
 * Targets critical algorithmic gaps identified in final analysis:
 * 1) VP9 encoder core algorithms (vp9_encodeframe.c functions like nonrd_pick_partition, 
 *    vp9_mcomp.c motion estimation, vp9_ratectrl.c rate control logic) - currently 19.67% coverage
 * 2) vpx_dsp signal processing functions (DCT/IDCT transforms, convolution filters, 
 *    variance computations) - currently 8.16% coverage
 * Strategy: Test deep algorithmic paths, mathematical edge cases, complex control flow, 
 *           and optimized routines through varied encoder configuration
 * Differentiation: Unlike previous API-focused harnesses, targets internal algorithms 
 *                  and signal processing via encoder parameter manipulation
 * Approach: Configure VP9 encoder with diverse algorithmic parameters to exercise 
 *           internal decision-making and signal processing paths
 */



// Minimum input size for algorithmic testing (needs more data for complex parameters)
const size_t MIN_INPUT_SIZE = 256;

// Helper function to create test image with fuzzed data for algorithmic testing
static vpx_image_t* create_algorithmic_test_image(unsigned int width, unsigned int height, 
                                                 vpx_img_fmt_t fmt, FuzzedDataProvider* fdp) {
    vpx_image_t* img = vpx_img_alloc(nullptr, fmt, width, height, 16);
    if (!img) return nullptr;
    
    // Fill image planes with fuzzed data to affect algorithm decisions
    for (int plane = 0; plane < 3; ++plane) {
        unsigned char* buf = img->planes[plane];
        if (!buf) continue;
        
        int stride = img->stride[plane];
        int w = (plane == 0) ? width : (width + 1) / 2;
        int h = (plane == 0) ? height : (height + 1) / 2;
        
        for (int y = 0; y < h; ++y) {
            if (fdp->remaining_bytes() < (size_t)w) {
                // Not enough data, free image and return
                vpx_img_free(img);
                return nullptr;
            }
            std::vector<uint8_t> row_data = fdp->ConsumeBytes<uint8_t>(w);
            memcpy(buf + y * stride, row_data.data(), w);
        }
    }
    
    return img;
}

// Configure encoder with algorithmic parameters that affect internal decisions
// Configure encoder with algorithmic parameters that affect internal decisions
// Using only public API control IDs that actually exist
static void configure_algorithmic_parameters(vpx_codec_ctx_t* codec, 
                                            vpx_codec_enc_cfg_t* cfg,
                                            FuzzedDataProvider* fdp) {
    if (!codec || !cfg || !fdp) return;
    
    // Consume algorithmic parameters that affect internal decision making
    // Use only control IDs that exist in public API
    
    // Speed/CPU usage - affects motion estimation, partition decisions
    int cpu_used = fdp->ConsumeIntegralInRange<int>(-9, 9);
    vpx_codec_control(codec, VP8E_SET_CPUUSED, cpu_used);
    
    // Sharpness - affects quantization and quality decisions
    uint8_t sharpness = fdp->ConsumeIntegral<uint8_t>() % 8;
    vpx_codec_control(codec, VP8E_SET_SHARPNESS, sharpness);
    
    // Static threshold - affects motion estimation decisions
    uint16_t static_threshold = fdp->ConsumeIntegral<uint16_t>();
    vpx_codec_control(codec, VP8E_SET_STATIC_THRESHOLD, static_threshold);
    
    // Enable auto alt reference - affects temporal layer decisions
    uint8_t enable_auto_alt_ref = fdp->ConsumeIntegral<uint8_t>() % 7; // 0-6 for VP9
    vpx_codec_control(codec, VP8E_SET_ENABLEAUTOALTREF, enable_auto_alt_ref);
    
    // Lossless encoding mode - affects quantization and transform decisions
    uint8_t lossless = fdp->ConsumeBool();
    vpx_codec_control(codec, VP9E_SET_LOSSLESS, lossless);
    
    // Tile configuration - affects parallel processing and partition boundaries
    uint8_t tile_columns = fdp->ConsumeIntegral<uint8_t>() % 4; // Log2 value: 0=1, 1=2, 2=4, 3=8
    uint8_t tile_rows = fdp->ConsumeIntegral<uint8_t>() % 4;
    vpx_codec_control(codec, VP9E_SET_TILE_COLUMNS, tile_columns);
    vpx_codec_control(codec, VP9E_SET_TILE_ROWS, tile_rows);
    
    // Frame parallel decoding - affects frame buffering decisions
    uint8_t frame_parallel_decoding = fdp->ConsumeBool();
    vpx_codec_control(codec, VP9E_SET_FRAME_PARALLEL_DECODING, frame_parallel_decoding);
    
    // Adaptive quantization mode - affects rate control decisions
    uint8_t aq_mode = fdp->ConsumeIntegral<uint8_t>() % 4; // 0=off, 1=variance, 2=complexity, 3=cyclic
    vpx_codec_control(codec, VP9E_SET_AQ_MODE, aq_mode);
    
    // Noise sensitivity - affects motion estimation and filtering
    uint8_t noise_sensitivity = fdp->ConsumeIntegral<uint8_t>() % 5; // 0-4
    vpx_codec_control(codec, VP9E_SET_NOISE_SENSITIVITY, noise_sensitivity);
    
    // Color space - affects color conversion and compression
    uint8_t color_space = fdp->ConsumeIntegral<uint8_t>() % 4; // 0=unknown, 1=bt601, 2=bt709, 3=smpte170
    vpx_codec_control(codec, VP9E_SET_COLOR_SPACE, color_space);
    
    // Color range - affects bit depth decisions
    uint8_t color_range = fdp->ConsumeBool();
    vpx_codec_control(codec, VP9E_SET_COLOR_RANGE, color_range);
    
    // Row-based multi-threading - affects parallel encoding
    uint8_t row_mt = fdp->ConsumeBool();
    vpx_codec_control(codec, VP9E_SET_ROW_MT, row_mt);
    
    // Alt reference adaptive quantization - affects temporal quality
    uint8_t alt_ref_aq = fdp->ConsumeBool();
    vpx_codec_control(codec, VP9E_SET_ALT_REF_AQ, alt_ref_aq);
    
    // Delta Q for UV - affects chroma quantization
    int delta_q_uv = fdp->ConsumeIntegralInRange<int>(-63, 63);
    vpx_codec_control(codec, VP9E_SET_DELTA_Q_UV, delta_q_uv);
    
    // Disable loopfilter - affects post-processing
    uint8_t disable_loopfilter = fdp->ConsumeBool();
    vpx_codec_control(codec, VP9E_SET_DISABLE_LOOPFILTER, disable_loopfilter);
    
    // Max inter bitrate percentage - affects rate control
    uint32_t max_inter_bitrate_pct = fdp->ConsumeIntegralInRange<uint32_t>(0, 1000);
    vpx_codec_control(codec, VP9E_SET_MAX_INTER_BITRATE_PCT, max_inter_bitrate_pct);
    
    // Golden frame CBR boost - affects rate allocation
    uint32_t gf_cbr_boost_pct = fdp->ConsumeIntegralInRange<uint32_t>(0, 1000);
    vpx_codec_control(codec, VP9E_SET_GF_CBR_BOOST_PCT, gf_cbr_boost_pct);
    
    // Min GF interval - affects temporal layer decisions
    uint32_t min_gf_interval = fdp->ConsumeIntegralInRange<uint32_t>(0, 32);
    vpx_codec_control(codec, VP9E_SET_MIN_GF_INTERVAL, min_gf_interval);
    
    // Max GF interval - affects temporal layer decisions  
    uint32_t max_gf_interval = fdp->ConsumeIntegralInRange<uint32_t>(0, 32);
    vpx_codec_control(codec, VP9E_SET_MAX_GF_INTERVAL, max_gf_interval);
    
    // Target level - affects bitrate and complexity decisions
    uint8_t target_level = fdp->ConsumeIntegral<uint8_t>() % 32;
    vpx_codec_control(codec, VP9E_SET_TARGET_LEVEL, target_level);
    
    // TPL (two-pass latency) - affects rate control
    uint8_t tpl = fdp->ConsumeBool();
    vpx_codec_control(codec, VP9E_SET_TPL, tpl);
    
    // Post-encode drop - affects rate control decisions
    uint8_t postencode_drop = fdp->ConsumeBool();
    vpx_codec_control(codec, VP9E_SET_POSTENCODE_DROP, postencode_drop);
    
    // Disable overshoot maxq for CBR - affects rate control
    uint8_t disable_overshoot_maxq_cbr = fdp->ConsumeBool();
    vpx_codec_control(codec, VP9E_SET_DISABLE_OVERSHOOT_MAXQ_CBR, disable_overshoot_maxq_cbr);
    
    // Quantizer one-pass - affects quantization decisions
    int quantizer_one_pass = fdp->ConsumeIntegralInRange<int>(-1, 63);
    vpx_codec_control(codec, VP9E_SET_QUANTIZER_ONE_PASS, quantizer_one_pass);
    
    // Key frame filtering - affects intra frame decisions
    uint8_t key_frame_filtering = fdp->ConsumeBool();
    vpx_codec_control(codec, VP9E_SET_KEY_FRAME_FILTERING, key_frame_filtering);
    
    // Validate input HBD - affects high bit depth processing
    uint8_t validate_input_hbd = fdp->ConsumeBool();
    vpx_codec_control(codec, VP9E_SET_VALIDATE_INPUT_HBD, validate_input_hbd);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters for algorithmic testing
    uint8_t width_idx = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t height_idx = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t bit_depth = (fdp.ConsumeBool()) ? 8 : 10;  // Test 8-bit and 10-bit
    uint8_t speed = fdp.ConsumeIntegral<uint8_t>() % 10;  // Speed 0-9
    uint8_t cq_level = fdp.ConsumeIntegral<uint8_t>() % 64;  // Quantizer 0-63
    uint8_t lossless = fdp.ConsumeBool();
    uint8_t color_space = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t color_range = fdp.ConsumeBool();
    
    // Map width/height indices to dimensions that trigger different algorithmic paths
    // Different sizes trigger different partition decisions and motion estimation ranges
    unsigned int widths[] = {64, 128, 256, 512};
    unsigned int heights[] = {48, 96, 192, 384};
    unsigned int width = widths[width_idx % 4];
    unsigned int height = heights[height_idx % 4];
    
    // Choose VP9 encoder for algorithmic testing
    const vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    if (!encoder_iface) {
        return 0;
    }
    
    // Initialize encoder configuration
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    
    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));
    
    // Get default configuration
    if (vpx_codec_enc_config_default(encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Set basic configuration for algorithmic testing
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;  // 1 Mbps
    cfg.g_error_resilient = 1;     // Error resilient for algorithmic testing
    cfg.g_pass = VPX_RC_ONE_PASS;  // Single pass for algorithmic testing
    cfg.g_lag_in_frames = 0;       // No lag for immediate algorithmic decisions
    
    // Set bit depth for testing high-bit-depth signal processing paths
    if (bit_depth == 10) {
        cfg.g_bit_depth = VPX_BITS_10;
        cfg.g_input_bit_depth = 10;
    }
    
    // Initialize encoder
    if (vpx_codec_enc_init(&codec, encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Configure algorithmic parameters that affect internal decisions
    configure_algorithmic_parameters(&codec, &cfg, &fdp);
    
    // Set additional parameters for algorithmic testing
    vpx_codec_control(&codec, VP8E_SET_CPUUSED, speed);
    vpx_codec_control(&codec, VP8E_SET_CQ_LEVEL, cq_level);
    vpx_codec_control(&codec, VP9E_SET_LOSSLESS, lossless);
    vpx_codec_control(&codec, VP9E_SET_COLOR_SPACE, color_space);
    vpx_codec_control(&codec, VP9E_SET_COLOR_RANGE, color_range);
    
    // Create test image with fuzzed data to affect algorithmic decisions
    vpx_img_fmt_t fmt = (bit_depth == 8) ? VPX_IMG_FMT_I420 : VPX_IMG_FMT_I42016;
    vpx_image_t* img = create_algorithmic_test_image(width, height, fmt, &fdp);
    if (!img) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Encode the image to exercise internal algorithms
    // The actual encoding process triggers:
    // 1. nonrd_pick_partition decisions in vp9_encodeframe.c
    // 2. Motion estimation algorithms in vp9_mcomp.c  
    // 3. Rate control logic in vp9_ratectrl.c
    // 4. Signal processing in vpx_dsp (DCT/IDCT, convolution, variance)
    vpx_codec_encode(&codec, img, 0, 1, 0, VPX_DL_GOOD_QUALITY);
    
    // Get encoded data to complete the encoding pipeline
    const vpx_codec_cx_pkt_t* pkt;
    vpx_codec_iter_t iter = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Frame encoded successfully - algorithms executed
            break;
        }
    }
    
    // Test additional algorithmic paths with a second frame
    // This tests inter-frame algorithms and temporal dependencies
    if (fdp.remaining_bytes() > 128) {
        // Create second image with different data to test motion estimation
        vpx_image_t* img2 = create_algorithmic_test_image(width, height, fmt, &fdp);
        if (img2) {
            // Encode second frame to test inter-frame algorithms
            vpx_codec_encode(&codec, img2, 1, 1, 0, VPX_DL_GOOD_QUALITY);
            vpx_img_free(img2);
        }
    }
    
    // Test flush to exercise finalization algorithms
    vpx_codec_encode(&codec, nullptr, 0, 0, 0, VPX_DL_GOOD_QUALITY);
    
    // Cleanup
    vpx_img_free(img);
    vpx_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_018
