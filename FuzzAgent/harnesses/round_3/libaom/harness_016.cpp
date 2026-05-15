#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <cstring>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"

namespace harness_016 {
/*
 * Fuzzing harness for libaom AV1 encoder restoration filter testing
 * Specifically targets compute_stats_win7_avx2 function with 198 blocked branches
 * Based on coverage guidance: /root/FuzzAgent/output/libaom/coverage_guidance/harness_016_guidance.md
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: general decoder/encoder testing with IVF format
 * - harness_015: encoder configuration validation testing
 * - harness_016: targeted restoration filter path with speed feature variations
 *   Specifically targets use_downsampled_wiener_stats flag (set when speed >= 5)
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful fuzzing:
    // - Speed level (1 byte)
    // - Image dimensions (2 * 4 bytes)
    // - Image data for at least one frame (~100 bytes)
    // Total minimum: ~109 bytes
    if (size < 109) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Step 0: Create test image with varied characteristics to trigger restoration filter
    // Restoration filter is more likely to be used with certain content characteristics
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(64, 1920);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(64, 1080);
    
    aom_image_t* img = aom_img_alloc_with_border(nullptr, AOM_IMG_FMT_I420, width, height, 
                                                  1,  // stride alignment
                                                  1,  // size alignment  
                                                  0); // no border
    
    if (img == nullptr) {
        return 0; // Failed to allocate image
    }
    
    // Fill the image with fuzzer data to create varied content
    for (int plane = 0; plane < 3; plane++) {
        if (img->stride[plane] > 0 && img->planes[plane] != nullptr) {
            size_t plane_height = (plane == 0) ? height : ((height + 1) / 2);
            size_t plane_size = img->stride[plane] * plane_height;
            
            // Use fuzzer data to fill image planes
            if (fdp.remaining_bytes() >= plane_size) {
                std::vector<uint8_t> plane_data = fdp.ConsumeBytes<uint8_t>(plane_size);
                memcpy(img->planes[plane], plane_data.data(), plane_size);
            } else {
                // Fill with pattern to ensure some texture for restoration filter
                memset(img->planes[plane], plane * 64, plane_size);
            }
        }
    }
    
    aom_image_t* wrapped_img = img;
    
    // Step 1: Initialize encoder context
    aom_codec_ctx_t codec;
    aom_codec_iface_t* iface = aom_codec_av1_cx();
    
    // Consume initialization flags
    aom_codec_flags_t flags = static_cast<aom_codec_flags_t>(
        fdp.ConsumeIntegral<uint32_t>());
    
    // Step 2: Get default encoder configuration
    aom_codec_enc_cfg_t cfg;
    unsigned int usage = fdp.ConsumeIntegralInRange<unsigned int>(0, 2); // 0-2 for AOM_USAGE values
    
    aom_codec_err_t res = aom_codec_enc_config_default(iface, &cfg, usage);
    if (res != AOM_CODEC_OK) {
        aom_img_free(img);
        return 0; // Failed to get default config
    }
    
    // Set basic configuration
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000; // Reasonable bitrate for testing
    cfg.g_threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 4);
    
    // Enable features that might trigger restoration filter usage
    cfg.g_error_resilient = fdp.ConsumeBool() ? 1 : 0;
    
    // Step 3: Set speed feature via aom_codec_control
    // According to speed_features.c, use_downsampled_wiener_stats is set when speed >= 5
    // We need to test both paths: speed < 5 (full resolution) and speed >= 5 (downsampled)
    int speed_level = fdp.ConsumeIntegralInRange<int>(0, 12);
    
    // Step 4: Initialize encoder with configuration
    res = aom_codec_enc_init_ver(&codec, iface, &cfg, flags, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        aom_img_free(img);
        return 0; // Encoder initialization failed
    }
    
    // Set speed level using AOME_SET_CPUUSED (controls internal speed settings)
    aom_codec_control(&codec, AOME_SET_CPUUSED, speed_level);
    
    // Set additional encoder controls that might affect restoration filter
    // These are derived from fuzzer input to create varied scenarios
    if (fdp.ConsumeBool()) {
        int sharpness = fdp.ConsumeIntegralInRange<int>(0, 7);
        aom_codec_control(&codec, AOME_SET_SHARPNESS, sharpness);
    }
    
    if (fdp.ConsumeBool()) {
        int aq_mode = fdp.ConsumeIntegralInRange<int>(0, 3);
        aom_codec_control(&codec, AV1E_SET_AQ_MODE, aq_mode);
    }
    
    // Set loop restoration filter parameters
    // These directly control whether restoration filter is enabled and how it operates
    if (fdp.ConsumeBool()) {
        int enable_restoration = fdp.ConsumeBool() ? 1 : 0;
        aom_codec_control(&codec, AV1E_SET_ENABLE_RESTORATION, enable_restoration);
    }
    
    // Set loop filter control which might affect restoration filter behavior
    if (fdp.ConsumeBool()) {
        int loopfilter_control = fdp.ConsumeIntegralInRange<int>(0, 2);
        aom_codec_control(&codec, AV1E_SET_LOOPFILTER_CONTROL, loopfilter_control);
    }
    // Step 5: Encode test frames to trigger restoration filter computation
    // Encode multiple frames to increase chance of triggering restoration filter
    int num_frames = fdp.ConsumeIntegralInRange<int>(1, 3);
    
    for (int frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        // Use a frame flags based on fuzzer input
        aom_enc_frame_flags_t frame_flags = 0;
        
        // Set frame type based on fuzzer input
        if (frame_idx == 0 || fdp.ConsumeBool()) {
            frame_flags |= AOM_EFLAG_FORCE_KF;
        }
        
        // Encode the frame
        res = aom_codec_encode(&codec, wrapped_img, frame_idx, 1, frame_flags);
        
        if (res != AOM_CODEC_OK) {
            // Encoding failed, but continue to cleanup
            break;
        }
        
        // Step 6: Retrieve encoded output
        const aom_codec_cx_pkt_t* pkt = nullptr;
        aom_codec_iter_t iter = nullptr;
        
        while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != nullptr) {
            // Process the packet if needed
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Frame packet received - restoration filter would have been applied
                // during encoding if enabled and conditions met
            }
        }
        
        // If we're out of fuzzer data, break early
        if (fdp.remaining_bytes() < 10) {
            break;
        }
    }
    
    // Flush any remaining frames
    res = aom_codec_encode(&codec, nullptr, -1, 1, 0);
    if (res == AOM_CODEC_OK) {
        const aom_codec_cx_pkt_t* pkt = nullptr;
        aom_codec_iter_t iter = nullptr;
        while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != nullptr) {
            // Process final packets
        }
    }
    
    // Step 7: Clean up
    aom_codec_destroy(&codec);
    aom_img_free(img);
    
    return 0;
}

} // namespace harness_016
