#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <algorithm>
#include <vector>
#include <map>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8dx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_image.h"

namespace harness_007 {
/*
 * Fuzzing harness for libvpx decoder callback execution APIs
 * Targets: vpx_codec_register_put_frame_cb and vpx_codec_register_put_slice_cb
 * Focus: Callback execution patterns with real VP8/VP9 test data, different registration timing,
 *        and comprehensive callback execution testing
 * Distinct from harness_003: This harness focuses on actual callback execution rather than just registration,
 * uses real test data patterns, and tests callback timing variations
 */




// Minimum input size for meaningful testing
const size_t MIN_INPUT_SIZE = 256;

// Maximum test frame size
const size_t MAX_TEST_FRAME_SIZE = 4096;

// Callback statistics structure
typedef struct callback_stats {
    int frame_callback_count;
    int slice_callback_count;
    int last_frame_width;
    int last_frame_height;
    int last_slice_valid_x;
    int last_slice_valid_y;
    int last_slice_update_x;
    int last_slice_update_y;
    bool error_occurred;
    std::vector<int> frame_sizes;
    std::vector<int> slice_counts;
} callback_stats_t;

// Enhanced frame callback function that actually processes image data
static void enhanced_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    if (user_priv && img) {
        callback_stats_t* stats = (callback_stats_t*)user_priv;
        stats->frame_callback_count++;
        
        // Record frame dimensions
        stats->last_frame_width = img->d_w;
        stats->last_frame_height = img->d_h;
        
        // Process some image data to exercise more code paths
        // Check image format and basic properties
        if (img->fmt != VPX_IMG_FMT_I420 && img->fmt != VPX_IMG_FMT_I422 &&
            img->fmt != VPX_IMG_FMT_I444 && img->fmt != VPX_IMG_FMT_I42016) {
            // Unsupported format, but we still count it
        }
        
        // Record that we got a frame
        stats->frame_sizes.push_back(img->d_w * img->d_h);
        
        // Try to access some image data (but be careful with bounds)
        if (img->planes[0] && img->stride[0] > 0 && img->d_h > 0) {
            // Just sample a few bytes to ensure we're reading the image
            volatile uint8_t sample = img->planes[0][0];
            (void)sample; // Prevent unused variable warning
        }
    }
}

// Enhanced slice callback function with more processing
static void enhanced_put_slice_callback(void* user_priv, const vpx_image_t* img,
                                       const vpx_image_rect_t* valid,
                                       const vpx_image_rect_t* update) {
    if (user_priv && img) {
        callback_stats_t* stats = (callback_stats_t*)user_priv;
        stats->slice_callback_count++;
        
        // Record slice information
        if (valid) {
            stats->last_slice_valid_x = valid->x;
            stats->last_slice_valid_y = valid->y;
        }
        if (update) {
            stats->last_slice_update_x = update->x;
            stats->last_slice_update_y = update->y;
        }
        
        // Record slice count for this frame
        if (stats->slice_counts.empty() || stats->frame_callback_count > stats->slice_counts.size()) {
            stats->slice_counts.push_back(1);
        } else {
            stats->slice_counts.back()++;
        }
        
        // Process some slice data
        if (img->planes[0] && valid && img->stride[0] > 0) {
            // Calculate a safe offset within the slice
            int safe_y = std::min(valid->y, img->d_h - 1);
            int safe_x = std::min(valid->x, img->d_w - 1);
            
            if (safe_y >= 0 && safe_x >= 0) {
                size_t offset = safe_y * img->stride[0] + safe_x;
                if (offset < (size_t)(img->stride[0] * img->d_h)) {
                    volatile uint8_t sample = img->planes[0][offset];
                    (void)sample; // Prevent unused variable warning
                }
            }
        }
    }
}

// Generate test VP8/VP9 frame data based on fuzzer input
static std::vector<uint8_t> generate_test_frame(FuzzedDataProvider* fdp, bool is_vp9) {
    std::vector<uint8_t> frame_data;
    
    // Consume frame size from input
    size_t frame_size = fdp->ConsumeIntegralInRange<size_t>(32, MAX_TEST_FRAME_SIZE);
    
    // Generate frame data with some structure to look like VP8/VP9 data
    frame_data.reserve(frame_size);
    
    // Add frame header pattern
    if (is_vp9) {
        // VP9 frame marker (0x85, 0x01)
        frame_data.push_back(0x85);
        frame_data.push_back(0x01);
    } else {
        // VP8 frame marker
        frame_data.push_back(0x9d);
        frame_data.push_back(0x01);
        frame_data.push_back(0x2a);
    }
    
    // Fill remaining bytes with data from fuzzer input
    size_t remaining = frame_size - frame_data.size();
    if (remaining > 0) {
        std::vector<uint8_t> additional_data = fdp->ConsumeBytes<uint8_t>(remaining);
        frame_data.insert(frame_data.end(), additional_data.begin(), additional_data.end());
    }
    
    // Ensure we have at least minimum size
    if (frame_data.size() < 16) {
        // Pad with zeros if needed
        frame_data.resize(16, 0);
    }
    
    return frame_data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    uint8_t codec_choice = fdp.ConsumeIntegral<uint8_t>() % 2;  // 0=VP8, 1=VP9
    uint8_t registration_timing = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0=before init, 1=after init, 2=mid-decode
    uint8_t callback_types = fdp.ConsumeIntegral<uint8_t>() % 4;  // Which callbacks to use
    uint8_t decode_iterations = fdp.ConsumeIntegralInRange<uint8_t>(1, 5);
    uint8_t use_reinitialization = fdp.ConsumeBool();
    uint8_t test_callback_removal = fdp.ConsumeBool();
    
    // Choose codec interface based on fuzzer input
    const vpx_codec_iface_t* decoder_iface = nullptr;
    bool is_vp9 = false;
    
    if (codec_choice == 0) {
        // VP8 decoder
        decoder_iface = vpx_codec_vp8_dx();
        is_vp9 = false;
    } else {
        // VP9 decoder (supports more features)
        decoder_iface = vpx_codec_vp9_dx();
        is_vp9 = true;
    }
    
    if (!decoder_iface) {
        return 0;
    }
    
    // Check codec capabilities
    vpx_codec_caps_t codec_caps = vpx_codec_get_caps(decoder_iface);
    bool supports_put_frame = (codec_caps & VPX_CODEC_CAP_PUT_FRAME) != 0;
    bool supports_put_slice = (codec_caps & VPX_CODEC_CAP_PUT_SLICE) != 0;
    
    // Adjust callback configuration based on codec capabilities
    if ((callback_types & 0x01) && !supports_put_frame) {
        callback_types &= ~0x01;  // Clear frame callback bit
    }
    if ((callback_types & 0x02) && !supports_put_slice) {
        callback_types &= ~0x02;  // Clear slice callback bit
    }
    
    // Initialize decoder configuration
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t cfg = {1, 0, 0};  // Single thread by default
    
    // Setup callback statistics
    callback_stats_t stats = {0, 0, 0, 0, 0, 0, 0, 0, false};
    callback_stats_t* stats_ptr = &stats;
    
    // Test different callback registration timing strategies
    vpx_codec_err_t err = VPX_CODEC_OK;
    
    if (registration_timing == 0) {
        // Strategy 1: Try to register callbacks BEFORE decoder initialization
        // This should fail, but we want to test the error path
        if (callback_types & 0x01) {
            err = vpx_codec_register_put_frame_cb(&decoder, enhanced_put_frame_callback, stats_ptr);
            // Should return VPX_CODEC_ERROR (decoder not initialized)
        }
        
        if (callback_types & 0x02) {
            err = vpx_codec_register_put_slice_cb(&decoder, enhanced_put_slice_callback, stats_ptr);
            // Should return VPX_CODEC_ERROR (decoder not initialized)
        }
    }
    
    // Initialize decoder (regardless of previous registration attempts)
    err = vpx_codec_dec_init(&decoder, decoder_iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        // Try with different configuration
        cfg.threads = 1;
        err = vpx_codec_dec_init(&decoder, decoder_iface, &cfg, 0);
        if (err != VPX_CODEC_OK) {
            return 0;
        }
    }
    
    if (registration_timing == 1) {
        // Strategy 2: Register callbacks AFTER decoder initialization (normal case)
        if (callback_types & 0x01) {
            err = vpx_codec_register_put_frame_cb(&decoder, enhanced_put_frame_callback, stats_ptr);
            // Record if registration succeeded
            if (err != VPX_CODEC_OK && err != VPX_CODEC_INCAPABLE) {
                stats.error_occurred = true;
            }
        }
        
        if (callback_types & 0x02) {
            err = vpx_codec_register_put_slice_cb(&decoder, enhanced_put_slice_callback, stats_ptr);
            // Record if registration succeeded
            if (err != VPX_CODEC_OK && err != VPX_CODEC_INCAPABLE) {
                stats.error_occurred = true;
            }
        }
    }
    
    // Generate test frame data for decoding
    std::vector<std::vector<uint8_t>> test_frames;
    for (int i = 0; i < decode_iterations && fdp.remaining_bytes() > 32; i++) {
        test_frames.push_back(generate_test_frame(&fdp, is_vp9));
    }
    
    if (test_frames.empty()) {
        vpx_codec_destroy(&decoder);
        return 0;
    }
    
    // Perform decoding iterations
    for (size_t frame_idx = 0; frame_idx < test_frames.size(); frame_idx++) {
        if (registration_timing == 2 && frame_idx == test_frames.size() / 2) {
            // Strategy 3: Register callbacks MID-DECODE (after some frames)
            if (callback_types & 0x01) {
                err = vpx_codec_register_put_frame_cb(&decoder, enhanced_put_frame_callback, stats_ptr);
            }
            
            if (callback_types & 0x02) {
                err = vpx_codec_register_put_slice_cb(&decoder, enhanced_put_slice_callback, stats_ptr);
            }
        }
        
        // Decode the test frame
        const std::vector<uint8_t>& frame_data = test_frames[frame_idx];
        err = vpx_codec_decode(&decoder, frame_data.data(), frame_data.size(), nullptr, 0);
        
        // Try to get decoded frames (will trigger callbacks if registered)
        vpx_codec_iter_t iter = nullptr;
        vpx_image_t* img = nullptr;
        
        int frames_from_iteration = 0;
        while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
            frames_from_iteration++;
            // Callbacks should have been triggered already if registered
            
            // Try some decoder controls that might affect callback behavior
            if (is_vp9 && frames_from_iteration == 1) {
                // Test some VP9-specific controls
                int enable = fdp.ConsumeBool() ? 1 : 0;
                vpx_codec_control(&decoder, VP9D_SET_ROW_MT, enable);
                
                if (fdp.ConsumeBool()) {
                    vpx_codec_control(&decoder, VP9_INVERT_TILE_DECODE_ORDER, fdp.ConsumeIntegralInRange<int>(0, 4));
                }
            }
        }
        vpx_codec_stream_info_t stream_info;
        stream_info.sz = sizeof(stream_info);
        vpx_codec_peek_stream_info(decoder_iface, frame_data.data(), frame_data.size(), &stream_info);
        
        // Try get stream info from decoder context
        vpx_codec_get_stream_info(&decoder, &stream_info);
    }
    
    // Test callback removal/re-registration
    if (test_callback_removal && (callback_types & 0x01)) {
        // Try to register a different callback (simulating removal/replacement)
        callback_stats_t alternate_stats = {0, 0, 0, 0, 0, 0, 0, 0, false};
        err = vpx_codec_register_put_frame_cb(&decoder, enhanced_put_frame_callback, &alternate_stats);
        
        // Decode one more frame to see if new callback is used
        if (fdp.remaining_bytes() > 32) {
            std::vector<uint8_t> final_frame = generate_test_frame(&fdp, is_vp9);
            vpx_codec_decode(&decoder, final_frame.data(), final_frame.size(), nullptr, 0);
            
            vpx_codec_iter_t iter = nullptr;
            while (vpx_codec_get_frame(&decoder, &iter) != nullptr) {
                // Callbacks should trigger with alternate_stats
            }
        }
    }
    
    // Test reinitialization with callbacks
    if (use_reinitialization) {
        // Destroy and reinitialize decoder
        vpx_codec_destroy(&decoder);
        
        // Reset stats for new decoder instance
        callback_stats_t new_stats = {0, 0, 0, 0, 0, 0, 0, 0, false};
        
        // Reinitialize with possibly different configuration
        cfg.threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 4);
        err = vpx_codec_dec_init(&decoder, decoder_iface, &cfg, 0);
        
        if (err == VPX_CODEC_OK) {
            // Register callbacks again
            if (callback_types & 0x01) {
                vpx_codec_register_put_frame_cb(&decoder, enhanced_put_frame_callback, &new_stats);
            }
            
            if (callback_types & 0x02) {
                vpx_codec_register_put_slice_cb(&decoder, enhanced_put_slice_callback, &new_stats);
            }
            
            // Decode one more frame
            if (fdp.remaining_bytes() > 32) {
                std::vector<uint8_t> reinit_frame = generate_test_frame(&fdp, is_vp9);
                vpx_codec_decode(&decoder, reinit_frame.data(), reinit_frame.size(), nullptr, 0);
                
                vpx_codec_iter_t iter = nullptr;
                while (vpx_codec_get_frame(&decoder, &iter) != nullptr) {
                    // New callbacks should trigger
                }
            }
        }
    } else {
        // Normal cleanup
        vpx_codec_destroy(&decoder);
    }
    
    // Use the callback statistics to prevent optimization
    // This ensures the callbacks actually run and their effects aren't optimized away
    volatile int total_callbacks = stats.frame_callback_count + stats.slice_callback_count;
    volatile bool had_errors = stats.error_occurred;
    (void)total_callbacks;
    (void)had_errors;
    
    return 0;
}

} // namespace harness_007
