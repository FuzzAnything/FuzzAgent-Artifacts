#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"

namespace harness_000 {
/*
 * Fuzzing harness for libaom AV1 encoder API
 * Targets core encoder initialization, configuration, and encoding functions
 */




int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for basic configuration
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume configuration parameters from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(16, 1024);
    int height = fdp.ConsumeIntegralInRange<int>(16, 1024);
    int usage = fdp.ConsumeBool() ? 0 : 1;  // 0 for good quality, 1 for realtime
    int cpu_used = fdp.ConsumeIntegralInRange<int>(0, 9);
    int target_bitrate = fdp.ConsumeIntegralInRange<int>(100, 10000);
    int g_error_resilient = fdp.ConsumeIntegral<uint32_t>();
    int frames_to_encode = fdp.ConsumeIntegralInRange<int>(1, 10);
    
    // Get encoder interface
    aom_codec_iface_t *encoder = aom_codec_av1_cx();
    if (!encoder) {
        return 0;
    }

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res = aom_codec_enc_config_default(encoder, &cfg, usage);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Apply fuzzed configuration
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;  // 30 fps
    cfg.rc_target_bitrate = target_bitrate;
    cfg.g_error_resilient = static_cast<aom_codec_er_flags_t>(g_error_resilient);
    
    // Initialize encoder
    aom_codec_ctx_t codec;
    if (aom_codec_enc_init(&codec, encoder, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set CPU usage
    if (aom_codec_control(&codec, AOME_SET_CPUUSED, cpu_used) != AOM_CODEC_OK) {
        aom_codec_destroy(&codec);
        return 0;
    }

    // Create a test image
    aom_image_t img;
    if (!aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1)) {
        aom_codec_destroy(&codec);
        return 0;
    }

    // Fill image with random data from fuzzer input
    size_t y_plane_size = width * height;
    size_t uv_plane_size = (width / 2) * (height / 2);
    
    if (fdp.remaining_bytes() < y_plane_size + 2 * uv_plane_size) {
        aom_img_free(&img);
        aom_codec_destroy(&codec);
        return 0;
    }

    // Fill Y plane
    std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(y_plane_size);
    memcpy(img.planes[0], y_data.data(), y_plane_size);
    
    // Fill U plane
    std::vector<uint8_t> u_data = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
    memcpy(img.planes[1], u_data.data(), uv_plane_size);
    
    // Fill V plane
    std::vector<uint8_t> v_data = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
    memcpy(img.planes[2], v_data.data(), uv_plane_size);

    // Encode frames
    for (int frame_idx = 0; frame_idx < frames_to_encode; frame_idx++) {
        int flags = 0;
        
        // Randomly force keyframe
        if (fdp.ConsumeBool()) {
            flags |= AOM_EFLAG_FORCE_KF;
        }
        
        // Randomly set reference frame flags
        if (fdp.ConsumeBool()) {
            flags |= AOM_EFLAG_NO_REF_LAST;
        }
        if (fdp.ConsumeBool()) {
            flags |= AOM_EFLAG_NO_REF_LAST2;
        }
        if (fdp.ConsumeBool()) {
            flags |= AOM_EFLAG_NO_REF_LAST3;
        }
        if (fdp.ConsumeBool()) {
            flags |= AOM_EFLAG_NO_REF_GF;
        }
        if (fdp.ConsumeBool()) {
            flags |= AOM_EFLAG_NO_REF_ARF;
        }

        // Encode the frame
        res = aom_codec_encode(&codec, &img, frame_idx, 1, flags);
        if (res != AOM_CODEC_OK) {
            // Continue even if encoding fails
            continue;
        }

        // Get encoded data
        aom_codec_iter_t iter = nullptr;
        const aom_codec_cx_pkt_t *pkt = nullptr;
        
        while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != nullptr) {
            // Process packet if needed (just consume it for fuzzing)
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Packet data is available in pkt->data.frame.buf
                // Size is pkt->data.frame.sz
                // We don't need to do anything with it for fuzzing
            }
        }
        
        // Check if we have enough data for next iteration
        if (fdp.remaining_bytes() < 100) {
            break;
        }
    }

    // Flush encoder
    aom_codec_encode(&codec, nullptr, -1, 1, 0);
    
    // Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&codec);

    return 0;
}

} // namespace harness_000
