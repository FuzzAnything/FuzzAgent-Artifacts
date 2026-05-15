#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_image.h"

namespace harness_002 {
/*
 * Fuzzing harness for libvpx multi-resolution video encoder
 * Target APIs: vpx_codec_enc_init_multi_ver, vpx_codec_enc_init_multi, 
 *              vpx_codec_encode, vpx_codec_get_cx_data, vpx_codec_destroy,
 *              vpx_codec_enc_config_default, vpx_img_alloc
 * Focus: Multi-encoder operations, error handling, and frame buffer callbacks
 *        Specifically targets vpx_codec_enc_init_multi_ver with 54 undiscovered branches
 */



// Maximum number of encoders for multi-resolution encoding (matches VP8 limitation)
#define MAX_ENCODERS 16

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check: need enough for basic configuration
    const size_t MIN_SIZE = 128;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Determine number of encoders (1-16, but typically 1-3 for multi-resolution)
    int num_encoders = fdp.ConsumeIntegralInRange<int>(1, MAX_ENCODERS);
    
    // Choose encoder interface - multi-encoder only supports VP8 according to docs
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx();
    if (!encoder_iface) {
        return 0;
    }
    
    // Arrays for encoder contexts, configurations, and down-sampling factors
    vpx_codec_ctx_t codec[MAX_ENCODERS];
    vpx_codec_enc_cfg_t cfg[MAX_ENCODERS];
    vpx_rational_t dsf[MAX_ENCODERS];
    
    // Initialize configurations for each encoder
    bool config_ok = true;
    for (int i = 0; i < num_encoders; i++) {
        // Get default configuration
        if (vpx_codec_enc_config_default(encoder_iface, &cfg[i], 0) != VPX_CODEC_OK) {
            config_ok = false;
            break;
        }
        
        // Override configuration with fuzzed data
        // Basic image dimensions (keep reasonable for fuzzing)
        unsigned int base_width = fdp.ConsumeIntegralInRange<unsigned int>(16, 512);
        unsigned int base_height = fdp.ConsumeIntegralInRange<unsigned int>(16, 512);
        
        // Apply down-sampling factor for multi-resolution
        // Each successive encoder gets smaller dimensions
        unsigned int downscale_factor = fdp.ConsumeIntegralInRange<unsigned int>(1, 4);
        unsigned int width = base_width / (i + 1);
        unsigned int height = base_height / (i + 1);
        
        // Ensure minimum dimensions
        if (width < 16) width = 16;
        if (height < 16) height = 16;
        
        cfg[i].g_w = width;
        cfg[i].g_h = height;
        cfg[i].g_timebase.num = 1;
        cfg[i].g_timebase.den = fdp.ConsumeIntegralInRange<unsigned int>(24, 60);
        cfg[i].rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
        cfg[i].g_error_resilient = fdp.ConsumeBool() ? VPX_ERROR_RESILIENT_DEFAULT : 0;
        
        // Set down-sampling factor for this encoder
        dsf[i].num = 1;
        dsf[i].den = downscale_factor;
    }
    
    if (!config_ok) {
        return 0;
    }
    
    // Encoder flags - test different combinations
    vpx_codec_flags_t flags = 0;
    bool use_psnr = fdp.ConsumeBool();
    bool use_output_partition = fdp.ConsumeBool();
    
    if (use_psnr) {
        flags |= VPX_CODEC_USE_PSNR;
    }
    if (use_output_partition) {
        flags |= VPX_CODEC_USE_OUTPUT_PARTITION;
    }
    
    // Initialize multi-encoder
    // Use vpx_codec_enc_init_multi_ver directly to target specific uncovered branches
    vpx_codec_err_t err = vpx_codec_enc_init_multi_ver(
        &codec[0], encoder_iface, &cfg[0], num_encoders, flags, &dsf[0], 
        VPX_ENCODER_ABI_VERSION);
    
    // Test various error handling paths
    if (err != VPX_CODEC_OK) {
        // Test error recovery paths by trying different configurations
        if (err == VPX_CODEC_INCAPABLE) {
            // Try without problematic flags
            flags &= ~(VPX_CODEC_USE_PSNR | VPX_CODEC_USE_OUTPUT_PARTITION);
            err = vpx_codec_enc_init_multi(&codec[0], encoder_iface, &cfg[0], 
                                          num_encoders, flags, &dsf[0]);
        }
        
        // If still not OK, test with fewer encoders
        if (err != VPX_CODEC_OK && num_encoders > 1) {
            // Try with just one encoder
            err = vpx_codec_enc_init(&codec[0], encoder_iface, &cfg[0], flags);
            if (err == VPX_CODEC_OK) {
                // Single encoder initialized successfully
                num_encoders = 1;
            } else {
                // Couldn't initialize any encoder
                return 0;
            }
        } else if (err != VPX_CODEC_OK) {
            return 0;
        }
    }
    
    // Allocate test images for each encoder
    vpx_image_t img[MAX_ENCODERS];
    bool images_allocated = true;
    
    for (int i = 0; i < num_encoders; i++) {
        if (!vpx_img_alloc(&img[i], VPX_IMG_FMT_I420, cfg[i].g_w, cfg[i].g_h, 32)) {
            images_allocated = false;
            break;
        }
    }
    
    if (!images_allocated) {
        // Clean up any already allocated images
        for (int i = 0; i < num_encoders; i++) {
            if (img[i].fmt != VPX_IMG_FMT_NONE) {
                vpx_img_free(&img[i]);
            }
        }
        
        // Destroy encoders
        for (int i = 0; i < num_encoders; i++) {
            vpx_codec_destroy(&codec[i]);
        }
        return 0;
    }
    
    // Fill image planes with fuzzed data
    for (int i = 0; i < num_encoders; i++) {
        unsigned int width = cfg[i].g_w;
        unsigned int height = cfg[i].g_h;
        
        // Calculate plane sizes
        size_t y_size = width * height;
        size_t uv_width = (width + 1) / 2;
        size_t uv_height = (height + 1) / 2;
        size_t uv_size = uv_width * uv_height;
        
        // Check if we have enough data for this image
        if (fdp.remaining_bytes() < y_size + uv_size * 2) {
            // Not enough data for all images, break early
            break;
        }
        
        // Fill Y plane (luminance)
        std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(y_size);
        if (y_data.size() < y_size) {
            break;
        }
        
        for (unsigned int y = 0; y < height; ++y) {
            uint8_t* row = img[i].planes[VPX_PLANE_Y] + y * img[i].stride[VPX_PLANE_Y];
            size_t copy_size = std::min<size_t>(width, y_size - y * width);
            if (copy_size > 0) {
                memcpy(row, y_data.data() + y * width, copy_size);
            }
        }
        
        // Fill U and V planes (chrominance) if available
        if (fdp.remaining_bytes() >= uv_size * 2) {
            std::vector<uint8_t> u_data = fdp.ConsumeBytes<uint8_t>(uv_size);
            std::vector<uint8_t> v_data = fdp.ConsumeBytes<uint8_t>(uv_size);
            
            // Fill U plane
            for (unsigned int y = 0; y < uv_height; ++y) {
                uint8_t* row = img[i].planes[VPX_PLANE_U] + y * img[i].stride[VPX_PLANE_U];
                size_t copy_size = std::min<size_t>(uv_width, uv_size - y * uv_width);
                if (copy_size > 0) {
                    memcpy(row, u_data.data() + y * uv_width, copy_size);
                }
            }
            
            // Fill V plane
            for (unsigned int y = 0; y < uv_height; ++y) {
                uint8_t* row = img[i].planes[VPX_PLANE_V] + y * img[i].stride[VPX_PLANE_V];
                size_t copy_size = std::min<size_t>(uv_width, uv_size - y * uv_width);
                if (copy_size > 0) {
                    memcpy(row, v_data.data() + y * uv_width, copy_size);
                }
            }
        }
    }
    
    // Encode frames with each encoder
    for (int i = 0; i < num_encoders; i++) {
        // Determine frame duration and flags based on fuzzer input
        long duration = fdp.ConsumeIntegralInRange<long>(1, 100);
        vpx_enc_frame_flags_t frame_flags = 0;
        
        // Set frame flags based on input
        if (fdp.ConsumeBool()) {
            frame_flags |= VPX_EFLAG_FORCE_KF;
        }
        
        // Encode the image
        vpx_codec_encode(&codec[i], &img[i], 0, duration, frame_flags, VPX_DL_REALTIME);
        
        // Get encoded data
        vpx_codec_iter_t iter = nullptr;
        const vpx_codec_cx_pkt_t *pkt = nullptr;
        int frame_cnt = 0;
        
        while ((pkt = vpx_codec_get_cx_data(&codec[i], &iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                frame_cnt++;
                
                // Test control API operations based on remaining fuzzer input
                if (frame_cnt == 1 && fdp.remaining_bytes() > 0) {
                    // Test VP8-specific controls for multi-encoder
                    bool test_control = fdp.ConsumeBool();
                    if (test_control) {
                        // VP8-specific controls for multi-resolution encoding
                        int cq_level = fdp.ConsumeIntegralInRange<int>(0, 63);
                        vpx_codec_control(&codec[i], VP8E_SET_CQ_LEVEL, cq_level);
                        
                        // Test multi-resolution specific controls
                        bool enable_auto_alt_ref = fdp.ConsumeBool();
                        if (enable_auto_alt_ref) {
                            vpx_codec_control(&codec[i], VP8E_SET_ENABLEAUTOALTREF, 1);
                        }
                        
                        // Test static threshold
                        int static_threshold = fdp.ConsumeIntegralInRange<int>(0, 1000);
                        vpx_codec_control(&codec[i], VP8E_SET_STATIC_THRESHOLD, static_threshold);
                    }
                }
            }
        }
    }
    
    // Test multi-frame encoding with remaining data
    if (fdp.remaining_bytes() > 0) {
        // Encode additional frames with different parameters
        for (int i = 0; i < num_encoders && fdp.remaining_bytes() > 0; i++) {
            // Test flush encoding (NULL image)
            vpx_codec_encode(&codec[i], nullptr, 0, 0, 0, 0);
            
            // Consume any remaining packets
            vpx_codec_iter_t iter = nullptr;
            const vpx_codec_cx_pkt_t *pkt = nullptr;
            while ((pkt = vpx_codec_get_cx_data(&codec[i], &iter)) != nullptr) {
                // Just consume the packets
            }
        }
    }
    
    // Test error conditions by attempting invalid operations
    if (fdp.ConsumeBool() && num_encoders > 0) {
        // Test passing invalid parameters to multi-encoder init
        vpx_codec_ctx_t invalid_ctx;
        vpx_codec_err_t invalid_err;
        
        // Test with invalid number of encoders
        invalid_err = vpx_codec_enc_init_multi_ver(
            &invalid_ctx, encoder_iface, &cfg[0], 0, flags, &dsf[0], 
            VPX_ENCODER_ABI_VERSION);
        // Error expected, ignore result
        
        // Test with invalid ABI version
        invalid_err = vpx_codec_enc_init_multi_ver(
            &invalid_ctx, encoder_iface, &cfg[0], 1, flags, &dsf[0], 
            VPX_ENCODER_ABI_VERSION - 1);
        // Error expected, ignore result
    }
    
    // Clean up
    for (int i = 0; i < num_encoders; i++) {
        vpx_img_free(&img[i]);
        vpx_codec_destroy(&codec[i]);
    }
    
    return 0;
}

} // namespace harness_002
