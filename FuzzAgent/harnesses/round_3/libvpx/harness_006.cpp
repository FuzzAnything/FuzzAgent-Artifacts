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
#include "vpx/vp8dx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_image.h"

namespace harness_006 {
/*
 * Fuzzing harness for libvpx advanced encoder configuration APIs
 * Targets uncovered encoder configuration APIs identified by coverage analysis:
 * 1. vpx_codec_enc_config_set - dynamic encoder configuration changes
 * 2. vpx_codec_get_preview_frame - preview frame retrieval  
 * 3. vpx_codec_set_cx_data_buf - external buffer management for encoded data
 */




// Minimum input size for meaningful testing
const size_t MIN_INPUT_SIZE = 128;

// Helper function to create a simple test image
static vpx_image_t* create_test_image(unsigned int width, unsigned int height, 
                                     vpx_img_fmt_t fmt, FuzzedDataProvider* fdp) {
    vpx_image_t* img = vpx_img_alloc(nullptr, fmt, width, height, 16);
    if (!img) return nullptr;
    
    // Fill image with simple pattern
    for (int plane = 0; plane < 3; ++plane) {
        unsigned char* buf = img->planes[plane];
        if (!buf) continue;
        
        int stride = img->stride[plane];
        int w = (plane == 0) ? width : (width + 1) / 2;
        int h = (plane == 0) ? height : (height + 1) / 2;
        
        for (int y = 0; y < h; ++y) {
            uint8_t pattern = (y % 256);
            memset(buf + y * stride, pattern, w);
        }
    }
    
    return img;
}

// Test vpx_codec_enc_config_set with dynamic configuration changes
static void test_dynamic_config_set(vpx_codec_ctx_t* enc, 
                                   vpx_codec_enc_cfg_t* cfg,
                                   FuzzedDataProvider* fdp) {
    if (!enc || !cfg || !fdp) return;
    
    // Consume various configuration changes from fuzzer input
    uint8_t config_change_type = fdp->ConsumeIntegral<uint8_t>() % 6;
    
    switch (config_change_type) {
        case 0: {
            // Change resolution dynamically
            unsigned int new_width = fdp->ConsumeIntegralInRange<unsigned int>(64, 1024);
            unsigned int new_height = fdp->ConsumeIntegralInRange<unsigned int>(48, 768);
            cfg->g_w = new_width;
            cfg->g_h = new_height;
            break;
        }
        
        case 1: {
            // Change bitrate settings
            unsigned int bitrate = fdp->ConsumeIntegralInRange<unsigned int>(100, 1000000);
            cfg->rc_target_bitrate = bitrate;
            cfg->rc_max_quantizer = fdp->ConsumeIntegralInRange<unsigned int>(4, 63);
            cfg->rc_min_quantizer = fdp->ConsumeIntegralInRange<unsigned int>(0, 
                std::min(62u, cfg->rc_max_quantizer - 1));
            break;
        }
        
        case 2: {
            // Change frame rate settings
            cfg->g_timebase.num = fdp->ConsumeIntegralInRange<int>(1, 100);
            cfg->g_timebase.den = fdp->ConsumeIntegralInRange<int>(1, 1000);
            break;
        }
        
        case 3: {
            // Change thread count
            cfg->g_threads = fdp->ConsumeIntegralInRange<unsigned int>(1, 8);
            break;
        }
        
        case 4: {
            // Change error resilience
            cfg->g_error_resilient = fdp->ConsumeIntegral<vpx_codec_er_flags_t>();
            break;
        }
        
        case 5: {
            // Change lag in frames
            cfg->g_lag_in_frames = fdp->ConsumeIntegralInRange<unsigned int>(0, 25);
            break;
        }
    }
    
    // Apply the configuration change
    vpx_codec_err_t result = vpx_codec_enc_config_set(enc, cfg);
    // Result may be OK or error depending on validness of change
    (void)result;
}

// Test vpx_codec_set_cx_data_buf with external buffer management
// Returns the allocated buffer in output parameters (if any)
static void test_external_buffer_management(vpx_codec_ctx_t* enc,
                                           FuzzedDataProvider* fdp,
                                           uint8_t** allocated_buffer,
                                           size_t* buffer_size) {
    if (!enc || !fdp) return;
    
    // Initialize output parameters
    if (allocated_buffer) *allocated_buffer = nullptr;
    if (buffer_size) *buffer_size = 0;
    
    uint8_t buffer_test_type = fdp->ConsumeIntegral<uint8_t>() % 3;
    
    switch (buffer_test_type) {
        case 0: {
            // Test with allocated buffer
            size_t alloc_size = fdp->ConsumeIntegralInRange<size_t>(1024, 65536);
            uint8_t* buffer_data = (uint8_t*)malloc(alloc_size);
            if (buffer_data) {
                vpx_fixed_buf_t buf;
                buf.buf = buffer_data;
                buf.sz = alloc_size;
                
                unsigned int pad_before = fdp->ConsumeIntegralInRange<unsigned int>(0, 128);
                unsigned int pad_after = fdp->ConsumeIntegralInRange<unsigned int>(0, 128);
                
                vpx_codec_err_t result = vpx_codec_set_cx_data_buf(enc, &buf, pad_before, pad_after);
                (void)result;
                
                // Return allocated buffer to caller (don't free here)
                if (allocated_buffer) *allocated_buffer = buffer_data;
                if (buffer_size) *buffer_size = alloc_size;
            }
            break;
        }
        
        case 1: {
            // Test with NULL buffer (restore default behavior)
            vpx_codec_err_t result = vpx_codec_set_cx_data_buf(enc, nullptr, 0, 0);
            (void)result;
            break;
        }
        
        case 2: {
            // Test with various padding combinations
            vpx_fixed_buf_t buf;
            size_t small_buffer_size = fdp->ConsumeIntegralInRange<size_t>(256, 4096);
            uint8_t* small_buffer = (uint8_t*)malloc(small_buffer_size);
            if (small_buffer) {
                buf.buf = small_buffer;
                buf.sz = small_buffer_size;
                
                // Test edge cases for padding
                unsigned int large_pad_before = fdp->ConsumeIntegralInRange<unsigned int>(0, 1024);
                unsigned int large_pad_after = fdp->ConsumeIntegralInRange<unsigned int>(0, 1024);
                
                vpx_codec_err_t result = vpx_codec_set_cx_data_buf(enc, &buf, 
                                                                  large_pad_before, 
                                                                  large_pad_after);
                (void)result;
                
                // Return allocated buffer to caller (don't free here)
                if (allocated_buffer) *allocated_buffer = small_buffer;
                if (buffer_size) *buffer_size = small_buffer_size;
            }
            break;
        }
    }
}

// Test vpx_codec_get_preview_frame functionality
static void test_preview_frame_retrieval(vpx_codec_ctx_t* enc, 
                                        FuzzedDataProvider* fdp) {
    if (!enc || !fdp) return;
    
    // Try to get preview frame at different stages
    uint8_t retrieval_timing = fdp->ConsumeIntegral<uint8_t>() % 3;
    
    switch (retrieval_timing) {
        case 0: {
            // Get preview frame immediately after init
            const vpx_image_t* preview = vpx_codec_get_preview_frame(enc);
            (void)preview; // May be NULL at this stage
            break;
        }
        
        case 1: {
            // Get preview frame multiple times
            for (int i = 0; i < 3; i++) {
                const vpx_image_t* preview = vpx_codec_get_preview_frame(enc);
                (void)preview;
            }
            break;
        }
        
        case 2: {
            // Test with different encoder states
            // First encode a frame to potentially generate preview
            // Then try to get preview
            vpx_image_t* test_img = create_test_image(64, 48, VPX_IMG_FMT_I420, fdp);
            if (test_img) {
                vpx_codec_encode(enc, test_img, 0, 1, 0, VPX_DL_REALTIME);
                vpx_img_free(test_img);
                
                // Now try to get preview
                const vpx_image_t* preview = vpx_codec_get_preview_frame(enc);
                (void)preview;
            }
            break;
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Variables to hold externally allocated buffers
    uint8_t* external_buffer = nullptr;
    size_t external_buffer_size = 0;
    
    // Consume configuration parameters from fuzzer input
    uint8_t codec_choice = fdp.ConsumeIntegral<uint8_t>() % 2;  // 0=VP8, 1=VP9
    uint8_t width_idx = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t height_idx = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Map width/height indices to actual dimensions
    unsigned int widths[] = {64, 128, 256, 512};
    unsigned int heights[] = {48, 96, 192, 384};
    unsigned int width = widths[width_idx % 4];
    unsigned int height = heights[height_idx % 4];
    
    // Choose codec interface based on fuzzer input
    const vpx_codec_iface_t* encoder_iface = nullptr;
    
    if (codec_choice == 0) {
        // VP8
        encoder_iface = vpx_codec_vp8_cx();
    } else {
        // VP9
        encoder_iface = vpx_codec_vp9_cx();
    }
    
    if (!encoder_iface) {
        return 0;
    }
    
    // Initialize encoder configuration
    vpx_codec_ctx_t enc;
    vpx_codec_enc_cfg_t cfg;
    
    memset(&enc, 0, sizeof(enc));
    memset(&cfg, 0, sizeof(cfg));
    
    // Get default configuration
    if (vpx_codec_enc_config_default(encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Set basic configuration
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000000;
    cfg.g_error_resilient = VPX_ERROR_RESILIENT_DEFAULT;
    cfg.g_lag_in_frames = 0;
    
    // Initialize encoder
    if (vpx_codec_enc_init(&enc, encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Set CPU usage for reasonable speed
    vpx_codec_control_(&enc, VP8E_SET_CPUUSED, 6);
    
    // Execute test scenario based on fuzzer input
    switch (test_scenario) {
        case 0: {
            // Scenario 1: Dynamic configuration changes during encoding
            test_dynamic_config_set(&enc, &cfg, &fdp);
            
            // Encode a test frame with new configuration
            vpx_image_t* test_img = create_test_image(width, height, VPX_IMG_FMT_I420, &fdp);
            if (test_img) {
                vpx_codec_encode(&enc, test_img, 0, 1, 0, VPX_DL_REALTIME);
                vpx_img_free(test_img);
            }
            break;
        }
        
        case 1: {
            // Scenario 2: External buffer management with encoding
            test_external_buffer_management(&enc, &fdp, &external_buffer, &external_buffer_size);
            
            // Encode frames with external buffer setup
            vpx_image_t* test_img = create_test_image(width, height, VPX_IMG_FMT_I420, &fdp);
            if (test_img) {
                vpx_codec_encode(&enc, test_img, 0, 1, 0, VPX_DL_REALTIME);
                
                // Iterate through encoded data
                vpx_codec_iter_t iter = nullptr;
                const vpx_codec_cx_pkt_t* pkt;
                while ((pkt = vpx_codec_get_cx_data(&enc, &iter)) != nullptr) {
                    // Process packet if needed
                    (void)pkt;
                }
                
                vpx_img_free(test_img);
            }
            break;
        }
        
        case 2: {
            // Scenario 3: Preview frame retrieval during encoding
            // Encode a frame first
            vpx_image_t* test_img = create_test_image(width, height, VPX_IMG_FMT_I420, &fdp);
            if (test_img) {
                vpx_codec_encode(&enc, test_img, 0, 1, 0, VPX_DL_REALTIME);
                
                // Test preview frame retrieval
                test_preview_frame_retrieval(&enc, &fdp);
                
                vpx_img_free(test_img);
            }
            break;
        }
        
        case 3: {
            // Scenario 4: Combined test - config change + buffer management + preview
            test_dynamic_config_set(&enc, &cfg, &fdp);
            test_external_buffer_management(&enc, &fdp, &external_buffer, &external_buffer_size);
            
            // Encode frame
            vpx_image_t* test_img = create_test_image(width, height, VPX_IMG_FMT_I420, &fdp);
            if (test_img) {
                vpx_codec_encode(&enc, test_img, 0, 1, 0, VPX_DL_REALTIME);
                
                // Get preview frame
                test_preview_frame_retrieval(&enc, &fdp);
                
                // Get encoded data
                vpx_codec_iter_t iter = nullptr;
                const vpx_codec_cx_pkt_t* pkt;
                while ((pkt = vpx_codec_get_cx_data(&enc, &iter)) != nullptr) {
                    // Process packet
                    (void)pkt;
                }
                
                vpx_img_free(test_img);
            }
            break;
        }
    }
    
    // Flush encoder
    vpx_codec_encode(&enc, nullptr, 0, 0, 0, VPX_DL_REALTIME);
    
    // Clean up
    vpx_codec_destroy(&enc);
    
    // Free any externally allocated buffer after encoder is destroyed
    if (external_buffer) {
        free(external_buffer);
        external_buffer = nullptr;
        external_buffer_size = 0;
    }
    
    return 0;
}

} // namespace harness_006
