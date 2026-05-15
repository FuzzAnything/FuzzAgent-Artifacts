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

namespace harness_002 {
/*
 * Fuzzing harness for libaom AV1 encoder configuration and control operations
 * Targets encoder configuration APIs with high undiscovered branch counts:
 * - aom_codec_enc_config_set (12 undiscovered branches)
 * - aom_codec_get_global_headers (10 undiscovered branches)
 * - aom_codec_get_preview_frame (10 undiscovered branches)
 * - aom_codec_set_cx_data_buf (6 undiscovered branches)
 * Based on coverage guidance: /root/FuzzAgent/output/libaom/coverage_guidance/harness_002_guidance.md
 * Semantic differentiation: harness_000 targets basic decoder/encoder, 
 * harness_001 targets image metadata, this harness focuses on encoder configuration and control.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful fuzzing:
    // - Image dimensions (2*4 bytes) = 8
    // - Config parameters (several ints) = ~20
    // - Buffer size/alignment = 8
    // - Flags/usage = 4
    // - Minimum image data = 100
    // Total minimum: ~140 bytes
    if (size < 140) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Step 0: Consume image parameters for test frame
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(16, 512);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(16, 512);
    
    // Create a test image buffer (I420 format) using proper stride calculation
    // Use aom_img_alloc_with_border which handles allocation internally to avoid buffer size issues
    aom_image_t* img = aom_img_alloc_with_border(nullptr, AOM_IMG_FMT_I420, width, height, 
                                                  1,  // stride alignment
                                                  1,  // size alignment  
                                                  0); // no border
    
    if (img == nullptr) {
        return 0; // Failed to allocate image
    }
    
    // Fill the image with data from fuzzer input
    // Calculate total buffer size needed to fill the image planes
    size_t total_buffer_needed = 0;
    for (int plane = 0; plane < 3; plane++) {
        if (img->stride[plane] > 0) {
            // For I420: plane 0 = Y, plane 1 = U, plane 2 = V
            size_t plane_height = (plane == 0) ? height : ((height + 1) / 2);
            total_buffer_needed += img->stride[plane] * plane_height;
        }
    }
    
    if (fdp.remaining_bytes() < total_buffer_needed + 100) {
        aom_img_free(img);
        return 0; // Not enough data for image + config
    }
    
    // Fill each plane with fuzzer data
    for (int plane = 0; plane < 3; plane++) {
        if (img->stride[plane] > 0 && img->planes[plane] != nullptr) {
            size_t plane_height = (plane == 0) ? height : ((height + 1) / 2);
            size_t plane_size = img->stride[plane] * plane_height;
            
            std::vector<uint8_t> plane_data = fdp.ConsumeBytes<uint8_t>(plane_size);
            if (plane_data.size() < plane_size) {
                // Not enough data, fill with zeros
                memset(img->planes[plane], 0, plane_size);
            } else {
                memcpy(img->planes[plane], plane_data.data(), plane_size);
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
    
    // Get default configuration
    aom_codec_enc_cfg_t cfg;
    unsigned int usage = fdp.ConsumeIntegralInRange<unsigned int>(0, 2); // 0-2 for AOM_USAGE values
    
    aom_codec_err_t res = aom_codec_enc_config_default(iface, &cfg, usage);
    if (res != AOM_CODEC_OK) {
        return 0; // Failed to get default config
    }
    
    // Step 2: Initialize encoder with version check
    res = aom_codec_enc_init_ver(&codec, iface, &cfg, flags, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        return 0; // Encoder initialization failed
    }
    
    // Step 3: Modify encoder configuration using aom_codec_enc_config_set
    // Consume configuration parameters from fuzzer input
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = fdp.ConsumeIntegralInRange<int>(1, 1000);
    cfg.g_timebase.den = fdp.ConsumeIntegralInRange<int>(1, 1000);
    cfg.g_profile = fdp.ConsumeIntegralInRange<unsigned int>(0, 3);
    cfg.g_bit_depth = fdp.ConsumeBool() ? AOM_BITS_8 : AOM_BITS_10;
    cfg.g_input_bit_depth = cfg.g_bit_depth;
    cfg.g_lag_in_frames = fdp.ConsumeIntegralInRange<unsigned int>(0, 25);
    cfg.rc_end_usage = static_cast<aom_rc_mode>(
        fdp.ConsumeIntegralInRange<unsigned int>(0, 2));
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
    cfg.rc_min_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, 63);
    cfg.rc_max_quantizer = fdp.ConsumeIntegralInRange<unsigned int>(0, 63);
    cfg.g_error_resilient = fdp.ConsumeIntegral<uint32_t>();
    cfg.g_threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 8);
    
    // Apply configuration changes
    res = aom_codec_enc_config_set(&codec, &cfg);
    // Note: We don't check result here to allow invalid configurations to be tested
    
    // Step 4: Set compressed data output buffer using aom_codec_set_cx_data_buf
    // Allocate output buffer
    size_t output_buffer_size = fdp.ConsumeIntegralInRange<size_t>(1024, 65536);
    std::vector<uint8_t> output_buffer(output_buffer_size, 0);
    
    aom_fixed_buf_t buf;
    buf.buf = output_buffer.data();
    buf.sz = output_buffer_size;
    
    unsigned int pad_before = fdp.ConsumeIntegralInRange<unsigned int>(0, 256);
    unsigned int pad_after = fdp.ConsumeIntegralInRange<unsigned int>(0, 256);
    
    res = aom_codec_set_cx_data_buf(&codec, &buf, pad_before, pad_after);
    // Note: We don't check result to test various buffer configurations
    
    // Step 5: Get global headers using aom_codec_get_global_headers
    aom_fixed_buf_t* global_headers = aom_codec_get_global_headers(&codec);
    if (global_headers != nullptr) {
        // Global headers available - consume and free them
        free(global_headers->buf);
        free(global_headers);
    }
    
    // Step 6: Encode frames using aom_codec_encode
    // Encode a few frames with different flags
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    
    for (int frame_idx = 0; frame_idx < 3 && fdp.remaining_bytes() > 10; frame_idx++) {
        aom_enc_frame_flags_t frame_flags = static_cast<aom_enc_frame_flags_t>(
            fdp.ConsumeIntegral<uint32_t>());
        
        // Encode the frame
        res = aom_codec_encode(&codec, wrapped_img, pts, duration, frame_flags);
        
        // Step 7: Retrieve encoded data using aom_codec_get_cx_data
        const aom_codec_cx_pkt_t* pkt = nullptr;
        aom_codec_iter_t iter = nullptr;
        
        while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != nullptr) {
            // Process packet if needed
            // We just consume it to exercise the API
        }
        
        // Step 8: Get preview frame during encoding using aom_codec_get_preview_frame
        const aom_image_t* preview_frame = aom_codec_get_preview_frame(&codec);
        if (preview_frame != nullptr) {
            // Preview frame available - we can examine its properties
            // but we must not modify it as per API documentation
        }
        
        pts += duration;
        
        // For the last iteration, flush the encoder with NULL image
        if (frame_idx == 2) {
            res = aom_codec_encode(&codec, nullptr, pts, duration, 0);
            
            // Retrieve any remaining data
            pkt = nullptr;
            iter = nullptr;
            while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != nullptr) {
                // Process remaining packets
            }
        }
    }
    
    // Step 9: Clean up encoder using aom_codec_destroy
    aom_codec_destroy(&codec);
    
    // Clean up allocated image
    aom_img_free(img);
    return 0;
}

} // namespace harness_002
