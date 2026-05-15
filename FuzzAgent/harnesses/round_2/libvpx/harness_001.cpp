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

namespace harness_001 {
/*
 * Fuzzing harness for libvpx control and utility APIs
 * Targets: vpx_codec_control, vpx_codec_get_frame, vpx_codec_get_cx_data,
 *          vpx_codec_peek_stream_info, vpx_img_alloc, vpx_img_free
 */




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 4) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Determine test type from first byte
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>();
    
    if (test_type & 0x01) {
        // Test decoder control APIs
        if (fdp.remaining_bytes() < 10) {
            return 0;
        }
        
        // Consume decoder configuration
        uint8_t config_byte = fdp.ConsumeIntegral<uint8_t>();
        unsigned int threads = (config_byte & 0x3f) + 1;
        
        vpx_codec_ctx_t codec;
        vpx_codec_dec_cfg_t cfg = { threads, 0, 0 };
        vpx_codec_flags_t flags = 0;
        
        // Try VP8 decoder
        vpx_codec_err_t err = vpx_codec_dec_init(&codec, vpx_codec_vp8_dx(), &cfg, flags);
        if (err != 0) {
            // Try VP9 decoder
            err = vpx_codec_dec_init(&codec, vpx_codec_vp9_dx(), &cfg, flags);
            if (err != 0) {
                return 0;
            }
        }
        
        // Test various control codes based on fuzzed input
        uint8_t control_type = fdp.ConsumeIntegral<uint8_t>();
        
        switch (control_type % 4) {
            case 0:
                // VP8_SET_POSTPROC
                if (fdp.remaining_bytes() >= sizeof(vp8_postproc_cfg_t)) {
                    vp8_postproc_cfg_t pp;
                    pp.post_proc_flag = fdp.ConsumeIntegral<int>();
                    pp.deblocking_level = fdp.ConsumeIntegral<int>();
                    pp.noise_level = fdp.ConsumeIntegral<int>();
                    vpx_codec_control(&codec, VP8_SET_POSTPROC, &pp);
                }
                break;
                
            case 1:
                // VP9D_SET_LOOP_FILTER_OPT
                if (fdp.remaining_bytes() >= 1) {
                    int enable = fdp.ConsumeIntegral<int>() & 0x01;
                    vpx_codec_control(&codec, VP9D_SET_LOOP_FILTER_OPT, enable);
                }
                break;
                
            case 2:
                // Test peek_stream_info with random data
                if (fdp.remaining_bytes() >= 16) {
                    std::vector<uint8_t> stream_data = fdp.ConsumeBytes<uint8_t>(
                        fdp.ConsumeIntegralInRange<size_t>(1, 1024));
                    vpx_codec_stream_info_t stream_info;
                    stream_info.sz = sizeof(stream_info);
                    vpx_codec_peek_stream_info(vpx_codec_vp9_dx(), 
                                              stream_data.data(), 
                                              stream_data.size(), 
                                              &stream_info);
                }
                break;
                
            default:
                // Test decode and get_frame
                if (fdp.remaining_bytes() >= 4) {
                    std::vector<uint8_t> frame_data = fdp.ConsumeBytes<uint8_t>(
                        fdp.ConsumeIntegralInRange<size_t>(1, 4096));
                    
                    err = vpx_codec_decode(&codec, frame_data.data(), frame_data.size(), nullptr, 0);
                    
                    // Try to get decoded frames
                    vpx_codec_iter_t iter = nullptr;
                    vpx_image_t *img = nullptr;
                    int frame_count = 0;
                    while ((img = vpx_codec_get_frame(&codec, &iter)) != nullptr && frame_count < 10) {
                        frame_count++;
                    }
                }
                break;
        }
        
        vpx_codec_destroy(&codec);
        
    } else {
        // Test encoder control APIs and image handling
        if (fdp.remaining_bytes() < 20) {
            return 0;
        }
        
        // Consume encoder configuration
        uint16_t width = fdp.ConsumeIntegralInRange<uint16_t>(16, 512);
        uint16_t height = fdp.ConsumeIntegralInRange<uint16_t>(16, 512);
        
        vpx_codec_ctx_t codec;
        vpx_image_t raw;
        vpx_codec_enc_cfg_t cfg;
        
        // Get default config for VP8
        if (vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, 0)) {
            return 0;
        }
        
        cfg.g_w = width;
        cfg.g_h = height;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 200;
        
        // Try VP8 encoder
        vpx_codec_err_t err = vpx_codec_enc_init(&codec, vpx_codec_vp8_cx(), &cfg, 0);
        if (err != 0) {
            // Try VP9 encoder
            err = vpx_codec_enc_init(&codec, vpx_codec_vp9_cx(), &cfg, 0);
            if (err != 0) {
                return 0;
            }
        }
        
        // Test vpx_img_alloc with different formats
        uint8_t img_test = fdp.ConsumeIntegral<uint8_t>();
        vpx_img_fmt_t format = VPX_IMG_FMT_I420;
        
        switch (img_test % 4) {
            case 0: format = VPX_IMG_FMT_I420; break;
            case 1: format = VPX_IMG_FMT_I422; break;
            case 2: format = VPX_IMG_FMT_I444; break;
            case 3: format = VPX_IMG_FMT_YV12; break;
        }
        
        // Test image allocation
        if (vpx_img_alloc(&raw, format, width, height, 1)) {
            // Fill image with some data if available
            if (fdp.remaining_bytes() > 0) {
                size_t bytes_to_fill = std::min(fdp.remaining_bytes(), 
                                               (size_t)(raw.d_w * raw.d_h));
                std::vector<uint8_t> img_data = fdp.ConsumeBytes<uint8_t>(bytes_to_fill);
                
                // Fill Y plane
                for (size_t i = 0; i < bytes_to_fill && i < (size_t)(raw.d_w * raw.d_h); i++) {
                    raw.planes[0][i] = img_data[i];
                }
            }
            
            // Test encoder control APIs
            uint8_t control_type = fdp.ConsumeIntegral<uint8_t>();
            
            switch (control_type % 3) {
                case 0:
                    // VP8E_SET_CPUUSED
                    if (fdp.remaining_bytes() >= 1) {
                        int cpu_used = fdp.ConsumeIntegralInRange<int>(-16, 16);
                        vpx_codec_control(&codec, VP8E_SET_CPUUSED, cpu_used);
                    }
                    break;
                    
                case 1:
                    // VP8E_SET_ENABLEAUTOALTREF
                    if (fdp.remaining_bytes() >= 1) {
                        int enable = fdp.ConsumeIntegral<int>() & 0x01;
                        vpx_codec_control(&codec, VP8E_SET_ENABLEAUTOALTREF, enable);
                    }
                    break;
                    
                case 2:
                    // VP9E_SET_TILE_COLUMNS
                    if (fdp.remaining_bytes() >= 1) {
                        int tile_columns = fdp.ConsumeIntegralInRange<int>(0, 6);
                        vpx_codec_control(&codec, VP9E_SET_TILE_COLUMNS, tile_columns);
                    }
                    break;
            }
            
            // Encode the image
            vpx_enc_deadline_t quality = VPX_DL_GOOD_QUALITY;
            if (fdp.remaining_bytes() >= 1 && (fdp.ConsumeIntegral<uint8_t>() & 0x01)) {
                quality = VPX_DL_REALTIME;
            }
            
            err = vpx_codec_encode(&codec, &raw, 0, 1, 0, quality);
            
            // Test vpx_codec_get_cx_data
            vpx_codec_iter_t iter = nullptr;
            const vpx_codec_cx_pkt_t *pkt = nullptr;
            int packet_count = 0;
            
            while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr && packet_count < 10) {
                packet_count++;
                
                // Handle different packet types
                switch (pkt->kind) {
                    case VPX_CODEC_CX_FRAME_PKT:
                        // Frame packet
                        break;
                    case VPX_CODEC_STATS_PKT:
                        // Stats packet
                        break;
                    case VPX_CODEC_FPMB_STATS_PKT:
                        // FPMB stats packet
                        break;
                    default:
                        break;
                }
            }
            
            vpx_img_free(&raw);
        }
        
        vpx_codec_destroy(&codec);
    }
    
    return 0;
}

} // namespace harness_001
