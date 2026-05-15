#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <memory>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_image.h"

namespace harness_007 {
/*
 * Fuzzing harness for libvpx encoder output control APIs
 * Target APIs: vpx_codec_get_preview_frame, vpx_codec_get_global_headers, 
 *              vpx_codec_set_cx_data_buf, vpx_codec_enc_init, vpx_codec_encode,
 *              vpx_codec_get_cx_data, vpx_codec_destroy, vpx_img_alloc
 * Focus: Encoder output data retrieval, preview frame access, global header retrieval,
 *        and compressed data buffer management
 * Specifically targets encoder output control APIs that have not been covered:
 * - vpx_codec_get_preview_frame: Gets preview image from encoder (0 hits, undiscovered branches)
 * - vpx_codec_get_global_headers: Gets global headers for stream (0 hits, undiscovered branches)
 * - vpx_codec_set_cx_data_buf: Sets compressed data output buffer (0 hits, undiscovered branches)
 * Semantic differentiation: This harness uniquely focuses on encoder output management
 * rather than encoder configuration, SVC controls, or error handling.
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check: need enough for configuration, buffer setup, and image data
    const size_t MIN_SIZE = 256;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    
    // Image dimensions (limited to reasonable sizes for fuzzing)
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(64, 512);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(64, 512);
    
    // Choose encoder type: VP8 or VP9 (both support preview/global headers)
    bool use_vp9 = fdp.ConsumeBool();
    
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
    
    // Override configuration with fuzzed data
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = fdp.ConsumeIntegralInRange<unsigned int>(24, 60);
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(500, 5000);
    
    // Encoder flags - minimal flags for output control testing
    vpx_codec_flags_t flags = 0;
    bool use_psnr = fdp.ConsumeBool();
    if (use_psnr) {
        flags |= VPX_CODEC_USE_PSNR;
    }
    
    // Initialize encoder context
    vpx_codec_ctx_t codec;
    vpx_codec_err_t init_err = vpx_codec_enc_init(&codec, encoder_iface, &cfg, flags);
    
    if (init_err != VPX_CODEC_OK) {
        // Try initialization without PSNR if initial fails
        flags &= ~VPX_CODEC_USE_PSNR;
        if (vpx_codec_enc_init(&codec, encoder_iface, &cfg, flags) != VPX_CODEC_OK) {
            return 0;
        }
    }
    
    // Test primary target: vpx_codec_get_global_headers
    // Get global headers for the encoder stream (e.g., codec configuration data)
    vpx_fixed_buf_t* global_headers = vpx_codec_get_global_headers(&codec);
    
    // If global headers are available, we can examine them
    if (global_headers) {
        // Global headers contain codec-specific configuration data
        // We don't need to parse them, just verify the API works
        (void)global_headers->buf;
        (void)global_headers->sz;
    }
    
    // Prepare a test image for encoding
    vpx_image_t* test_image = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, width, height, 16);
    if (!test_image) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Fill image with synthetic data for encoding
    // Use fuzzed data to populate image planes
    size_t y_plane_size = width * height;
    size_t uv_plane_size = (width / 2) * (height / 2);
    size_t total_image_size = y_plane_size + 2 * uv_plane_size;
    
    if (fdp.remaining_bytes() < total_image_size) {
        vpx_img_free(test_image);
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Fill Y plane
    std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(y_plane_size);
    for (size_t i = 0; i < y_plane_size && i < y_data.size(); ++i) {
        test_image->planes[0][i] = y_data[i];
    }
    
    // Fill U plane
    std::vector<uint8_t> u_data = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
    for (size_t i = 0; i < uv_plane_size && i < u_data.size(); ++i) {
        test_image->planes[1][i] = u_data[i];
    }
    
    // Fill V plane
    std::vector<uint8_t> v_data = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
    for (size_t i = 0; i < uv_plane_size && i < v_data.size(); ++i) {
        test_image->planes[2][i] = v_data[i];
    }
    
    // Test secondary target: vpx_codec_set_cx_data_buf
    // Set up an external buffer for compressed data output
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1024, 65536);
    if (fdp.remaining_bytes() < buffer_size) {
        vpx_img_free(test_image);
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    std::vector<uint8_t> compressed_buffer_data = fdp.ConsumeBytes<uint8_t>(buffer_size);
    
    vpx_fixed_buf_t output_buffer;
    output_buffer.buf = compressed_buffer_data.data();
    output_buffer.sz = buffer_size;
    
    // Set padding values from fuzzed data
    unsigned int pad_before = fdp.ConsumeIntegralInRange<unsigned int>(0, 256);
    unsigned int pad_after = fdp.ConsumeIntegralInRange<unsigned int>(0, 256);
    
    // Call vpx_codec_set_cx_data_buf to configure external output buffer
    vpx_codec_err_t set_buf_err = vpx_codec_set_cx_data_buf(&codec, &output_buffer, pad_before, pad_after);
    (void)set_buf_err; // Ignore error - may fail if buffer setup not supported
    
    // Encode a frame
    vpx_codec_err_t encode_err = vpx_codec_encode(&codec, test_image, 0, 1, 0, VPX_DL_REALTIME);
    
    if (encode_err == VPX_CODEC_OK) {
        // Retrieve compressed data using iterator pattern
        vpx_codec_iter_t iter = nullptr;
        const vpx_codec_cx_pkt_t *pkt;
        
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
            // Process the packet - just verify API works
            (void)pkt->kind;
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                (void)pkt->data.frame.buf;
                (void)pkt->data.frame.sz;
            }
        }
        
        // Test tertiary target: vpx_codec_get_preview_frame
        // Get preview frame from encoder (if supported)
        const vpx_image_t* preview_frame = vpx_codec_get_preview_frame(&codec);
        
        if (preview_frame) {
            // Preview frame is available - verify basic structure
            (void)preview_frame->fmt;
            (void)preview_frame->d_w;
            (void)preview_frame->d_h;
            // Note: According to documentation, we MUST NOT write to this buffer
        }
        
        // Encode additional frames with different parameters to exercise more paths
        bool encode_more_frames = fdp.ConsumeBool();
        if (encode_more_frames && fdp.remaining_bytes() > 1000) {
            // Change buffer configuration for second frame
            if (fdp.ConsumeBool()) {
                // Test with NULL buffer to restore default behavior
                vpx_codec_set_cx_data_buf(&codec, nullptr, 0, 0);
            } else if (fdp.remaining_bytes() > 5000) {
                // Set up a different buffer
                size_t second_buffer_size = fdp.ConsumeIntegralInRange<size_t>(1024, 16384);
                if (fdp.remaining_bytes() >= second_buffer_size) {
                    std::vector<uint8_t> second_buffer = fdp.ConsumeBytes<uint8_t>(second_buffer_size);
                    vpx_fixed_buf_t second_output_buffer;
                    second_output_buffer.buf = second_buffer.data();
                    second_output_buffer.sz = second_buffer_size;
                    
                    unsigned int second_pad_before = fdp.ConsumeIntegralInRange<unsigned int>(0, 128);
                    unsigned int second_pad_after = fdp.ConsumeIntegralInRange<unsigned int>(0, 128);
                    
                    vpx_codec_set_cx_data_buf(&codec, &second_output_buffer, second_pad_before, second_pad_after);
                }
            }
            
            // Encode second frame (possibly with flush)
            int force_keyframe = fdp.ConsumeBool() ? VPX_EFLAG_FORCE_KF : 0;
            vpx_codec_encode(&codec, nullptr, 0, 1, force_keyframe, VPX_DL_REALTIME);
            
            // Get preview frame again after second encode
            preview_frame = vpx_codec_get_preview_frame(&codec);
            (void)preview_frame; // Just call the API
        }
    }
    
    // Test edge cases for the target APIs
    
    // Test vpx_codec_get_global_headers with potentially invalid state
    // (though we should avoid calling after destroy)
    
    // Test vpx_codec_set_cx_data_buf with various buffer configurations
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 100) {
        // Test with small buffer (might trigger internal buffer fallback)
        size_t small_buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 100);
        if (fdp.remaining_bytes() >= small_buffer_size) {
            std::vector<uint8_t> small_buffer = fdp.ConsumeBytes<uint8_t>(small_buffer_size);
            vpx_fixed_buf_t small_output_buffer;
            small_output_buffer.buf = small_buffer.data();
            small_output_buffer.sz = small_buffer_size;
            
            vpx_codec_set_cx_data_buf(&codec, &small_output_buffer, 0, 0);
        }
    }
    
    // Cleanup
    vpx_img_free(test_image);
    vpx_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_007
