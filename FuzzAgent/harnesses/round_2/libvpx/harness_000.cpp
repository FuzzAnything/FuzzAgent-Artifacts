#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <memory>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8dx.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_image.h"

namespace harness_000 {
/*
 * Fuzzing harness for libvpx decoder and encoder APIs
 * Targets: vpx_codec_dec_init, vpx_codec_decode, vpx_codec_enc_init, vpx_codec_encode
 */




// Simple implementation of mem_get_le32
static inline uint32_t mem_get_le32(const uint8_t* mem) {
    return (uint32_t)mem[0] | ((uint32_t)mem[1] << 8) | 
           ((uint32_t)mem[2] << 16) | ((uint32_t)mem[3] << 24);
}

#define IVF_FRAME_HDR_SZ (4 + 8) /* 4 byte size + 8 byte timestamp */
#define IVF_FILE_HDR_SZ 32

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 4) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Determine test type from first byte
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>();
    
    if (test_type & 0x01) {
        // Test decoder
        if (fdp.remaining_bytes() < IVF_FILE_HDR_SZ + 4) {
            return 0;
        }
        
        // Consume decoder configuration
        uint8_t config_byte = fdp.ConsumeIntegral<uint8_t>();
        unsigned int threads = (config_byte & 0x3f) + 1;
        
        vpx_codec_ctx_t codec;
        vpx_codec_dec_cfg_t cfg = { threads, 0, 0 };
        vpx_codec_flags_t flags = 0;
        
        if ((config_byte & 0x40) != 0) {
            flags |= VPX_CODEC_USE_POSTPROC;
        }
        
        // Try VP8 decoder
        vpx_codec_err_t err = vpx_codec_dec_init(&codec, vpx_codec_vp8_dx(), &cfg, flags);
        if (err != 0) {
            // Try VP9 decoder
            err = vpx_codec_dec_init(&codec, vpx_codec_vp9_dx(), &cfg, flags);
            if (err != 0) {
                return 0;
            }
        }
        
        // Consume remaining data as "compressed frame"
        std::vector<uint8_t> frame_data = fdp.ConsumeRemainingBytes<uint8_t>();
        if (frame_data.size() > 0) {
            // Try to decode the frame
            err = vpx_codec_decode(&codec, frame_data.data(), frame_data.size(), nullptr, 0);
            
            // Try to get decoded frames
            vpx_codec_iter_t iter = nullptr;
            vpx_image_t *img = nullptr;
            while ((img = vpx_codec_get_frame(&codec, &iter)) != nullptr) {
                // Frame decoded successfully
            }
        }
        
        vpx_codec_destroy(&codec);
        
    } else {
        // Test encoder
        if (fdp.remaining_bytes() < 10) {
            return 0;
        }
        
        // Consume encoder configuration
        uint8_t config_byte = fdp.ConsumeIntegral<uint8_t>();
        uint16_t width = fdp.ConsumeIntegralInRange<uint16_t>(1, 1024);
        uint16_t height = fdp.ConsumeIntegralInRange<uint16_t>(1, 1024);
        
        vpx_codec_ctx_t codec;
        vpx_image_t raw;
        vpx_codec_enc_cfg_t cfg;
        
        // Get default config for VP8
        if (vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, 0)) {
            return 0;
        }
        
        // Set configuration from fuzzed data
        cfg.g_w = width;
        cfg.g_h = height;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 200;
        cfg.g_error_resilient = 1;
        
        // Try VP8 encoder
        vpx_codec_err_t err = vpx_codec_enc_init(&codec, vpx_codec_vp8_cx(), &cfg, 0);
        if (err != 0) {
            // Try VP9 encoder
            err = vpx_codec_enc_init(&codec, vpx_codec_vp9_cx(), &cfg, 0);
            if (err != 0) {
                return 0;
            }
        }
        
        // Allocate image buffer
        if (!vpx_img_alloc(&raw, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1)) {
            vpx_codec_destroy(&codec);
            return 0;
        }
        
        // Fill image with fuzzed data if available
        size_t image_size = raw.d_w * raw.d_h * 3 / 2; // I420 format
        if (fdp.remaining_bytes() >= image_size) {
            std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_size);
            // Simple fill of Y plane
            if (image_data.size() >= raw.d_w * raw.d_h) {
                for (unsigned int i = 0; i < raw.d_w * raw.d_h; i++) {
                    raw.planes[0][i] = image_data[i % image_data.size()];
                }
            }
        }
        
        // Encode a single frame
        vpx_enc_deadline_t quality = VPX_DL_GOOD_QUALITY;
        if ((config_byte & 0x40) != 0) {
            quality = VPX_DL_REALTIME;
        } else if ((config_byte & 0x20) != 0) {
            quality = VPX_DL_BEST_QUALITY;
        }
        
        err = vpx_codec_encode(&codec, &raw, 0, 1, 0, quality);
        
        // Get encoded data
        vpx_codec_iter_t iter = nullptr;
        const vpx_codec_cx_pkt_t *pkt = nullptr;
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Encoded frame available
            }
        }
        
        vpx_img_free(&raw);
        vpx_codec_destroy(&codec);
    }
    
    return 0;
}

} // namespace harness_000
