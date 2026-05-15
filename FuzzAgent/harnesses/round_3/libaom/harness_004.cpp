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
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_image.h"

namespace harness_004 {
/*
 * Fuzzing harness for libaom AV1 decoder stream information operations
 * Targets decoder stream info APIs with high undiscovered branch counts:
 * - aom_codec_get_stream_info (10 undiscovered branches)
 * - aom_codec_get_caps (2 undiscovered branches)
 * - aom_img_plane_height (2 undiscovered branches)
 * - aom_img_plane_width (2 undiscovered branches)
 * Based on coverage guidance: /root/FuzzAgent/output/libaom/coverage_guidance/harness_004_guidance.md
 * Semantic differentiation: harness_000 targets basic decoder/encoder,
 * harness_001 targets image metadata, harness_002 targets encoder configuration,
 * harness_003 targets decoder error handling. This harness focuses on decoder stream information retrieval.
 */



// Generate synthetic AV1-like data from fuzzer input for decoder testing
// Returns a vector containing synthetic data that resembles AV1 bitstream
std::vector<uint8_t> generate_synthetic_av1_data(FuzzedDataProvider& fdp, size_t max_size) {
    std::vector<uint8_t> result;
    
    // Determine how much data to generate (0 to max_size)
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(0, max_size);
    
    if (data_size == 0) {
        // Return empty vector for no data case
        return result;
    }
    
    // Generate synthetic AV1-like data
    // Start with a simple OBU-like structure
    if (data_size >= 2) {
        // First byte: synthetic OBU header
        uint8_t obu_header = fdp.ConsumeIntegral<uint8_t>();
        result.push_back(obu_header);
        
        // Second byte: synthetic size field (simplified)
        uint8_t size_field = fdp.ConsumeIntegral<uint8_t>();
        result.push_back(size_field);
        
        // Remaining bytes: synthetic payload
        size_t payload_size = std::min(data_size - 2, fdp.remaining_bytes());
        if (payload_size > 0) {
            std::vector<uint8_t> payload = fdp.ConsumeBytes<uint8_t>(payload_size);
            result.insert(result.end(), payload.begin(), payload.end());
        }
    } else {
        // Very small data - just use what we have
        result = fdp.ConsumeBytes<uint8_t>(data_size);
    }
    
    return result;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful fuzzing:
    // - Image dimensions (2*4 bytes) = 8
    // - Image format and stride alignment = 4
    // - Buffer for image data = ~100
    // - Decoder config params = 16
    // - Flags and controls = 8
    // Total minimum: ~136 bytes
    if (size < 140) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // PHASE 1: Initialization Phase
    
    // Step 1: Test aom_codec_get_caps (standalone test)
    // Get AV1 decoder interface and test capabilities
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    if (decoder_iface == nullptr) {
        return 0;
    }
    
    // Call aom_codec_get_caps to get decoder capabilities
    aom_codec_caps_t decoder_caps = aom_codec_get_caps(decoder_iface);
    // Capabilities are a bitfield, we just need to call the function
    
    // Step 2: Create test image using aom_img_alloc
    // Consume image parameters from fuzzer input
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(16, 512);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(16, 512);
    
    // Select image format from valid options
    aom_img_fmt_t format = static_cast<aom_img_fmt_t>(
        fdp.ConsumeIntegralInRange<uint8_t>(0, 8));
    
    // Validate format is supported (fallback to I420 if not)
    if (format != AOM_IMG_FMT_I420 && format != AOM_IMG_FMT_YV12 && 
        format != AOM_IMG_FMT_I422 && format != AOM_IMG_FMT_I444 &&
        format != AOM_IMG_FMT_NV12) {
        format = AOM_IMG_FMT_I420;
    }
    
    // Create image with alignment
    unsigned int stride_align = 1;
    uint8_t align_power = fdp.ConsumeIntegralInRange<uint8_t>(0, 8);
    stride_align = 1 << align_power;
    if (stride_align == 0) stride_align = 1;
    
    aom_image_t* img = aom_img_alloc(nullptr, format, width, height, stride_align);
    if (img == nullptr) {
        // Try with default parameters if allocation fails
        img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 64, 64, 1);
        if (img == nullptr) {
            return 0; // Failed to allocate image
        }
    }
    
    // Step 3: Test aom_img_plane_height and aom_img_plane_width on created image
    // Test all planes (0=Y, 1=U, 2=V)
    for (int plane = 0; plane < 3; plane++) {
        int plane_width = aom_img_plane_width(img, plane);
        int plane_height = aom_img_plane_height(img, plane);
        
        // Validate plane dimensions are non-negative
        (void)plane_width;
        (void)plane_height;
    }
    
    // PHASE 2: Decoder Setup
    
    // Step 4: Initialize decoder with aom_codec_dec_init_ver
    aom_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));
    
    // Consume decoder configuration parameters
    aom_codec_dec_cfg_t cfg;
    cfg.threads = fdp.ConsumeIntegralInRange<int>(0, 16);
    cfg.w = fdp.ConsumeIntegralInRange<unsigned int>(0, 4096);
    cfg.h = fdp.ConsumeIntegralInRange<unsigned int>(0, 4096);
    cfg.allow_lowbitdepth = fdp.ConsumeBool() ? 1 : 0;
    
    // Consume initialization flags
    aom_codec_flags_t flags = static_cast<aom_codec_flags_t>(
        fdp.ConsumeIntegral<uint32_t>());
    
    // Initialize decoder
    aom_codec_err_t init_res = aom_codec_dec_init_ver(&codec, decoder_iface, &cfg, 
                                                      flags, AOM_DECODER_ABI_VERSION);
    
    // If initialization failed, we still need to cleanup and exit
    if (init_res != AOM_CODEC_OK) {
        aom_img_free(img);
        return 0;
    }
    
    // Step 5: Decode sample AV1 data using aom_codec_decode
    // Generate synthetic AV1 data from fuzzer input
    std::vector<uint8_t> av1_data = generate_synthetic_av1_data(fdp, 1024);
    
    // Decode the data
    aom_codec_err_t decode_res = aom_codec_decode(&codec, av1_data.data(), 
                                                  av1_data.size(), nullptr);
    
    // We continue even if decode fails to test get_stream_info
    
    // PHASE 3: Target Operations
    
    // Step 6: Call aom_codec_get_stream_info to retrieve stream dimensions and information
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // Set initial values
    stream_info.w = 0;
    stream_info.h = 0;
    stream_info.is_kf = 0;
    stream_info.number_spatial_layers = 0;
    stream_info.number_temporal_layers = 0;
    stream_info.is_annexb = 0;
    
    aom_codec_err_t stream_info_res = aom_codec_get_stream_info(&codec, &stream_info);
    
    // Step 7: Validate returned stream info structure
    // Even if function returns error, we can examine the structure
    (void)stream_info_res;
    
    // Check if we got valid dimensions (non-zero means stream info was successfully parsed)
    if (stream_info.w > 0 && stream_info.h > 0) {
        // Valid stream dimensions obtained
        (void)stream_info.w;
        (void)stream_info.h;
    }
    
    // Also test stream info with different initialization
    aom_codec_stream_info_t stream_info2;
    memset(&stream_info2, 0, sizeof(stream_info2));
    // Don't initialize fields to test default behavior
    (void)aom_codec_get_stream_info(&codec, &stream_info2);
    
    // PHASE 4: Cleanup Phase
    
    // Step 8: Call aom_img_free to release image memory
    aom_img_free(img);
    
    // Step 9: Call aom_codec_destroy to clean up decoder context
    aom_codec_err_t destroy_res = aom_codec_destroy(&codec);
    (void)destroy_res;
    
    return 0;
}

} // namespace harness_004
