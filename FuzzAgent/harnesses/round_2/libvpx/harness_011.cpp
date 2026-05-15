#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <memory>
#include <vector>
#include <cstring>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_image.h"

namespace harness_011 {
/*
 * Fuzzing harness for libvpx targeting deep coverage exploration of vpx_dsp module 
 * focusing on high-bit-depth SIMD-optimized DSP functions
 * 
 * Target: Improve vpx_dsp module coverage from 38.96% line coverage
 * Critical blocked branches to unlock: 214 blocked branches in setup_rtcd_internal
 * 
 * Goal: Exercise high-bit-depth encoding with various frame characteristics
 * to trigger CPU feature detection paths and SIMD-optimized DSP functions
 * 
 * APIs to target:
 * - vpx_codec_enc_init_ver (encoder initialization with high-bit-depth flags)
 * - vpx_codec_enc_config_default (configuration for high-bit-depth encoding)
 * - vpx_codec_encode (execute high-bit-depth DSP functions)
 * - vpx_codec_get_cx_data (retrieve encoded packets)
 * - vpx_codec_destroy (cleanup)
 * 
 * Additional requirements:
 * - Force initialization of RTCD system by triggering vpx_dsp_rtcd()
 * - Test multiple image formats (I420, I422, I444) and bit depths (8, 10, 12)
 * - Use varying frame sizes to hit different SIMD optimizations
 * - Include edge cases: small frames, odd dimensions, high frequency content
 * 
 * Invocation sequence:
 * 1. Initialize VP9 encoder with vpx_codec_enc_init_ver using high-bit-depth flags
 * 2. Get default config with vpx_codec_enc_config_default
 * 3. Configure encoder with high-bit-depth settings (8/10/12 bit) and color spaces
 * 4. Encode multiple times with different frame types (keyframe, inter-frame)
 * 5. Retrieve encoded packets with vpx_codec_get_cx_data
 * 6. Clean up with vpx_codec_destroy
 * 
 * Semantic differentiation:
 * Unlike harness_010 which focuses on VP9 encoder configuration validation,
 * this harness specifically targets high-bit-depth encoding paths that
 * exercise SIMD-optimized DSP functions in vpx_dsp module.
 * Unlike harness_000 which tests basic encoder/decoder operations,
 * this harness systematically tests high-bit-depth encoding with varied
 * image formats, bit depths, and frame characteristics.
 * 
 * Expected coverage gain:
 * - Unlock blocked branches in setup_rtcd_internal
 * - Improve vpx_dsp module coverage from 38.96% toward 50%+
 * - Exercise high-bit-depth SIMD functions (vpx_highbd_fdct32x32_rd_sse2, etc.)
 * - Cover CPU feature detection paths currently untouched
 */




int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for configuration parameters and image data
    const size_t MIN_SIZE = sizeof(uint8_t) * 128 + sizeof(int) * 32 + sizeof(unsigned int) * 64;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters
    uint8_t config_byte = fdp.ConsumeIntegral<uint8_t>();
    
    // Determine bit depth (8, 10, or 12 bits)
    uint8_t bit_depth_selector = fdp.ConsumeIntegral<uint8_t>() % 3;
    int bit_depth;
    vpx_codec_flags_t flags = 0;
    
    switch (bit_depth_selector) {
        case 0:
            bit_depth = 8;
            flags = 0;  // Standard 8-bit
            break;
        case 1:
            bit_depth = 10;
            flags = VPX_CODEC_USE_HIGHBITDEPTH;
            break;
        case 2:
            bit_depth = 12;
            flags = VPX_CODEC_USE_HIGHBITDEPTH;
            break;
    }
    // Determine image format (I420, I422, I444)
    uint8_t format_selector = fdp.ConsumeIntegral<uint8_t>() % 3;
    vpx_img_fmt_t img_fmt;
    
    switch (format_selector) {
        case 0:
            img_fmt = VPX_IMG_FMT_I420;
            break;
        case 1:
            img_fmt = VPX_IMG_FMT_I422;
            break;
        case 2:
            img_fmt = VPX_IMG_FMT_I444;
            break;
    }
    
    // Set high-bit-depth flag if needed
    if (bit_depth > 8) {
        img_fmt = static_cast<vpx_img_fmt_t>(img_fmt | VPX_IMG_FMT_HIGHBITDEPTH);
    }
    
    // Determine frame dimensions with edge cases
    uint16_t width, height;
    
    // Small frames for edge cases (odd dimensions, small sizes)
    if ((config_byte & 0x01) != 0) {
        width = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
        height = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
        
        // Ensure odd dimensions for some tests
        if ((config_byte & 0x02) != 0) {
            width |= 1;  // Make odd
            height |= 1; // Make odd
        }
    } else {
        // Normal frames for SIMD optimization testing
        width = fdp.ConsumeIntegralInRange<uint16_t>(64, 1024);
        height = fdp.ConsumeIntegralInRange<uint16_t>(64, 1024);
        
        // Align to SIMD boundaries for some tests
        if ((config_byte & 0x04) != 0) {
            width = (width / 16) * 16;  // Multiple of 16
            height = (height / 16) * 16; // Multiple of 16
        }
    }
    
    // Determine stride alignment (must be power of 2)
    unsigned int stride_align = 32;  // Common alignment for SIMD
    
    // Initialize encoder context
    vpx_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err;
    
    err = vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }
    
    // Configure encoder for high-bit-depth
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = static_cast<vpx_bit_depth_t>(bit_depth);
    cfg.g_input_bit_depth = static_cast<unsigned int>(bit_depth);
    
    // Vary other configuration parameters to hit different code paths
    cfg.rc_target_bitrate = fdp.ConsumeIntegralInRange<unsigned int>(100, 10000);
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = fdp.ConsumeIntegralInRange<unsigned int>(24, 60);
    cfg.g_threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 8);
    
    // Set profile based on bit depth and format
    if (bit_depth == 8) {
        cfg.g_profile = fdp.ConsumeIntegralInRange<unsigned int>(0, 2);
    } else {
        // High bit-depth profiles
        cfg.g_profile = fdp.ConsumeIntegralInRange<unsigned int>(2, 3);
    }
    
    // Initialize encoder with high-bit-depth flags
    err = vpx_codec_enc_init_ver(&codec, vpx_codec_vp9_cx(), &cfg, flags, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return 0;
    }
    
    // Create synthetic image data
    vpx_image_t* img = nullptr;
    size_t img_size = 0;
    
    // Allocate image buffer
    img = vpx_img_alloc(nullptr, img_fmt, width, height, stride_align);
    if (!img) {
        vpx_codec_destroy(&codec);
        return 0;
    }
    
    // Use actual strides from allocated image structure
    // The stride array contains stride in bytes for each plane
    int y_stride_actual = img->stride[VPX_PLANE_Y];
    int u_stride_actual = img->stride[VPX_PLANE_U];
    int v_stride_actual = img->stride[VPX_PLANE_V];
    
    // Calculate actual plane sizes based on image dimensions and strides
    // For planar formats, we need to consider actual height for each plane
    // which may be different due to chroma subsampling
    unsigned int y_height = height;
    unsigned int u_height = height;
    unsigned int v_height = height;
    
    // Adjust heights for chroma subsampling
    if (img_fmt == VPX_IMG_FMT_I420 || img_fmt == (VPX_IMG_FMT_I420 | VPX_IMG_FMT_HIGHBITDEPTH)) {
        u_height = (height + 1) / 2;
        v_height = (height + 1) / 2;
    } else if (img_fmt == VPX_IMG_FMT_I422 || img_fmt == (VPX_IMG_FMT_I422 | VPX_IMG_FMT_HIGHBITDEPTH)) {
        u_height = height;
        v_height = height;
    }
    // For I444, all planes have same height
    
    // Calculate actual plane sizes in bytes
    size_t y_plane_size = y_stride_actual * y_height;
    size_t u_plane_size = u_stride_actual * u_height;
    size_t v_plane_size = v_stride_actual * v_height;
    
    // Check if we have enough fuzzer data for image content
    size_t required_img_data = y_plane_size + u_plane_size + v_plane_size;
    if (fdp.remaining_bytes() < required_img_data) {
        vpx_img_free(img);
        vpx_codec_destroy(&codec);
        return 0;
    }
    // Fill Y plane with fuzzer data - use actual stride
    std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(y_plane_size);
    if (y_data.size() == y_plane_size) {
        // Copy row by row to ensure we don't overflow
        uint8_t* y_dst = img->planes[VPX_PLANE_Y];
        const uint8_t* y_src = y_data.data();
        for (unsigned int row = 0; row < y_height; row++) {
            memcpy(y_dst, y_src, y_stride_actual);
            y_dst += y_stride_actual;
            y_src += y_stride_actual;
        }
    }
    
    // Fill U plane with fuzzer data - use actual stride
    std::vector<uint8_t> u_data = fdp.ConsumeBytes<uint8_t>(u_plane_size);
    if (u_data.size() == u_plane_size) {
        // Copy row by row using actual stride
        uint8_t* u_dst = img->planes[VPX_PLANE_U];
        const uint8_t* u_src = u_data.data();
        for (unsigned int row = 0; row < u_height; row++) {
            memcpy(u_dst, u_src, u_stride_actual);
            u_dst += u_stride_actual;
            u_src += u_stride_actual;
        }
    }
    
    // Fill V plane with fuzzer data - use actual stride
    std::vector<uint8_t> v_data = fdp.ConsumeBytes<uint8_t>(v_plane_size);
    if (v_data.size() == v_plane_size) {
        // Copy row by row using actual stride
        uint8_t* v_dst = img->planes[VPX_PLANE_V];
        const uint8_t* v_src = v_data.data();
        for (unsigned int row = 0; row < v_height; row++) {
            memcpy(v_dst, v_src, v_stride_actual);
            v_dst += v_stride_actual;
            v_src += v_stride_actual;
        }
    }
    
    // Encode multiple frames with different types
    int num_frames = fdp.ConsumeIntegralInRange<int>(1, 5);
    for (int i = 0; i < num_frames && fdp.remaining_bytes() > 0; i++) {
        vpx_codec_flags_t encode_flags = 0;
        
        // Set keyframe flag for first frame or based on fuzzer data
        if (i == 0 || (fdp.ConsumeIntegral<uint8_t>() & 0x01) != 0) {
            encode_flags |= VPX_EFLAG_FORCE_KF;
        }
        
        // Determine frame duration
        long duration = fdp.ConsumeIntegralInRange<long>(1, 1000);
        
        // Encode the frame
        err = vpx_codec_encode(&codec, img, i * duration, duration, encode_flags, VPX_DL_REALTIME);
        
        // Retrieve encoded packets
        const vpx_codec_cx_pkt_t* pkt = nullptr;
        vpx_codec_iter_t iter = nullptr;
        
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
            // Process packet if needed
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Frame encoded successfully
            }
        }
        
        // Break if we're out of fuzzer data
        if (fdp.remaining_bytes() < 10) {
            break;
        }
    }
    
    // Flush encoder
    err = vpx_codec_encode(&codec, nullptr, -1, 1, 0, VPX_DL_REALTIME);
    
    // Retrieve any remaining packets
    const vpx_codec_cx_pkt_t* pkt = nullptr;
    vpx_codec_iter_t iter = nullptr;
    
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
        // Process final packets
    }
    
    // Cleanup
    vpx_img_free(img);
    vpx_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_011
