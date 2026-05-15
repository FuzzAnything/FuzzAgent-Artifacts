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

namespace harness_001 {
/*
 * Fuzzing harness for libvpx video encoder
 * Target APIs: vpx_codec_enc_init, vpx_codec_encode, vpx_codec_get_cx_data,
 *              vpx_codec_enc_config_set, vpx_codec_enc_config_default,
 *              vpx_img_alloc, vpx_codec_destroy
 * Focus: VP8/VP9 encoder operations with complete invocation sequence
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check: need enough for configuration and at least some image data
    const size_t MIN_SIZE = 64;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    // Choose encoder type: VP8 or VP9
    bool use_vp9 = fdp.ConsumeBool();
    
    // Image dimensions (limited to reasonable sizes for fuzzing)
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(16, 512);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(16, 512);
    
    // Encoder flags
    vpx_codec_flags_t flags = 0;
    bool use_psnr = fdp.ConsumeBool();
    bool use_output_partition = fdp.ConsumeBool();
    
    if (use_psnr) {
        flags |= VPX_CODEC_USE_PSNR;
    }
    if (use_output_partition) {
        flags |= VPX_CODEC_USE_OUTPUT_PARTITION;
    }
    
    // Choose encoder interface
    vpx_codec_iface_t* encoder_iface = nullptr;
    if (use_vp9) {
        encoder_iface = vpx_codec_vp9_cx();
    } else {
        encoder_iface = vpx_codec_vp8_cx();
    }
    
    if (!encoder_iface) {
        return 0;
    }
    
    // Get default encoder configuration
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(encoder_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Override some configuration parameters with fuzzed data
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = fdp.ConsumeIntegralInRange<unsigned int>(24, 60);
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
    cfg.g_error_resilient = fdp.ConsumeBool() ? VPX_ERROR_RESILIENT_DEFAULT : 0;
    
    // Initialize encoder context
    vpx_codec_ctx_t codec;
    vpx_codec_err_t err = vpx_codec_enc_init(&codec, encoder_iface, &cfg, flags);
    
    // Handle initialization failures
    if (err == VPX_CODEC_INCAPABLE) {
        // Try without problematic flags
        flags &= ~(VPX_CODEC_USE_PSNR | VPX_CODEC_USE_OUTPUT_PARTITION);
        if (vpx_codec_enc_init(&codec, encoder_iface, &cfg, flags) != VPX_CODEC_OK) {
            return 0;
        }
    } else if (err != VPX_CODEC_OK) {
        return 0;
    }
    
    // Try to set encoder configuration (dynamic reconfiguration)
    bool reconfigure = fdp.ConsumeBool();
    if (reconfigure) {
        // Modify some parameters
        cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(50, 20000);
        cfg.g_pass = fdp.ConsumeBool() ? VPX_RC_ONE_PASS : VPX_RC_FIRST_PASS;
        
        vpx_codec_enc_config_set(&codec, &cfg);
        // Ignore error - configuration may fail for valid reasons
    }
    
    // Create test image for encoding
    vpx_image_t img;
    vpx_img_fmt_t img_fmt = VPX_IMG_FMT_I420;
    
    // Allocate image with fuzzed dimensions
    if (!vpx_img_alloc(&img, img_fmt, width, height, 16)) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Fill image planes with fuzzed data
    // For I420 format: Y plane first, then U, then V
    size_t remaining_bytes = fdp.remaining_bytes();
    if (remaining_bytes < width * height) {
        vpx_img_free(&img);
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Consume data for Y plane (luminance)
    size_t y_size = width * height;
    std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(y_size);
    if (y_data.size() < y_size) {
        vpx_img_free(&img);
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Fill Y plane
    for (unsigned int y = 0; y < height; ++y) {
        uint8_t* row = img.planes[VPX_PLANE_Y] + y * img.stride[VPX_PLANE_Y];
        size_t copy_size = std::min<size_t>(width, y_size - y * width);
        if (copy_size > 0) {
            memcpy(row, y_data.data() + y * width, copy_size);
        }
    }
    
    // Consume data for U and V planes (chrominance) if available
    size_t uv_width = (width + 1) / 2;
    size_t uv_height = (height + 1) / 2;
    size_t uv_size = uv_width * uv_height;
    
    if (fdp.remaining_bytes() >= uv_size * 2) {
        std::vector<uint8_t> u_data = fdp.ConsumeBytes<uint8_t>(uv_size);
        std::vector<uint8_t> v_data = fdp.ConsumeBytes<uint8_t>(uv_size);
        
        // Fill U plane
        for (unsigned int y = 0; y < uv_height; ++y) {
            uint8_t* row = img.planes[VPX_PLANE_U] + y * img.stride[VPX_PLANE_U];
            size_t copy_size = std::min<size_t>(uv_width, uv_size - y * uv_width);
            if (copy_size > 0) {
                memcpy(row, u_data.data() + y * uv_width, copy_size);
            }
        }
        
        // Fill V plane
        for (unsigned int y = 0; y < uv_height; ++y) {
            uint8_t* row = img.planes[VPX_PLANE_V] + y * img.stride[VPX_PLANE_V];
            size_t copy_size = std::min<size_t>(uv_width, uv_size - y * uv_width);
            if (copy_size > 0) {
                memcpy(row, v_data.data() + y * uv_width, copy_size);
            }
        }
    }
    
    // Encode the image
    vpx_codec_encode(&codec, &img, 0, 1, 0, VPX_DL_REALTIME);
    
    // Get encoded data
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t *pkt = nullptr;
    int frame_cnt = 0;
    
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            frame_cnt++;
            
            // Test control API operations based on fuzzer input
            if (frame_cnt == 1 && fdp.remaining_bytes() > 0) {
                // Test some encoder control operations
                bool test_control = fdp.ConsumeBool();
                if (test_control) {
                    if (use_vp9) {
                        // VP9-specific controls - use valid control IDs
                        int cq_level = fdp.ConsumeIntegralInRange<int>(0, 63);
                        vpx_codec_control(&codec, VP9E_SET_QUANTIZER_ONE_PASS, cq_level);
                        
                        bool enable_tile = fdp.ConsumeBool();
                        if (enable_tile) {
                            int tile_columns = fdp.ConsumeIntegralInRange<int>(0, 6);
                            vpx_codec_control(&codec, VP9E_SET_TILE_COLUMNS, tile_columns);
                        }
                    } else {
                        // VP8-specific controls
                        int cq_level = fdp.ConsumeIntegralInRange<int>(0, 63);
                        vpx_codec_control(&codec, VP8E_SET_CQ_LEVEL, cq_level);
                        
                        bool enable_arf = fdp.ConsumeBool();
                        if (enable_arf) {
                            vpx_codec_control(&codec, VP8E_SET_ENABLEAUTOALTREF, 1);
                        }
                    }
                }
            }
        }
    }
    
    // Test multi-frame encoding if we have more data
    if (frame_cnt > 0 && fdp.remaining_bytes() > width * height / 4) {
        // Create a second frame with different data
        std::vector<uint8_t> second_frame_data = fdp.ConsumeBytes<uint8_t>(width * height / 4);
        if (second_frame_data.size() > 0) {
            // Update Y plane with new data
            for (unsigned int y = 0; y < height && y * width < second_frame_data.size(); ++y) {
                uint8_t* row = img.planes[VPX_PLANE_Y] + y * img.stride[VPX_PLANE_Y];
                size_t copy_size = std::min<size_t>(width, second_frame_data.size() - y * width);
                if (copy_size > 0) {
                    memcpy(row, second_frame_data.data() + y * width, copy_size);
                }
            }
            
            // Encode second frame
            vpx_codec_encode(&codec, &img, 1, 1, 0, VPX_DL_REALTIME);
            
            // Get encoded data from second frame
            iter = nullptr;
            while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                    frame_cnt++;
                }
            }
        }
    }
    
    // Test flushing the encoder (NULL image means flush)
    vpx_codec_encode(&codec, nullptr, 0, 0, 0, 0);
    
    iter = nullptr;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        // Just consume the packets
    }
    
    // Clean up
    vpx_img_free(&img);
    vpx_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_001
