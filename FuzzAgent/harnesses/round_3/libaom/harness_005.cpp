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
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_image.h"

namespace harness_005 {
/*
 * Fuzzing harness for libaom AV1 codec informational APIs and image operations
 * Targets uncovered codec informational functions and image flipping:
 * - aom_codec_iface_name (2 undiscovered branches)
 * - aom_codec_version_extra_str
 * - aom_codec_version_str
 * - aom_codec_version
 * - aom_codec_build_config
 * - aom_img_flip
 * Based on coverage guidance: /root/FuzzAgent/output/libaom/coverage_guidance/harness_005_guidance.md
 * Semantic differentiation: harness_000 targets basic decoder/encoder,
 * harness_001 targets image metadata, harness_002 targets encoder configuration,
 * harness_003 targets decoder error handling, harness_004 targets decoder stream information.
 * This harness focuses on codec informational APIs and image flipping.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful fuzzing:
    // - Codec type selection (1 byte)
    // - Image dimensions (2*4 bytes) = 8
    // - Image format selection (1 byte)
    // - Alignment value (1 byte)
    // - Buffer for image data = ~100
    // Total minimum: ~111 bytes
    if (size < 112) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // PHASE 1: Codec Informational API Testing
    
    // Step 1: Call standalone informational functions (don't require codec context)
    // These can be called without any initialization
    
    // Get version information
    int version = aom_codec_version();
    const char* version_str = aom_codec_version_str();
    const char* version_extra_str = aom_codec_version_extra_str();
    const char* build_config = aom_codec_build_config();
    
    // Version components can be examined (but we just need to call the APIs)
    int major_version = (version >> 16) & 0xFF;
    int minor_version = (version >> 8) & 0xFF;
    int patch_version = version & 0xFF;
    
    // Step 2: Initialize a codec to test aom_codec_iface_name
    // Choose between encoder and decoder based on fuzzer input
    uint8_t codec_type = fdp.ConsumeIntegral<uint8_t>() % 2;
    aom_codec_ctx_t codec;
    aom_codec_iface_t* iface = nullptr;
    
    if (codec_type == 0) {
        // Initialize encoder
        iface = aom_codec_av1_cx();
    } else {
        // Initialize decoder
        iface = aom_codec_av1_dx();
    }
    
    if (iface == nullptr) {
        return 0;
    }
    
    // Get interface name before initialization
    const char* iface_name = aom_codec_iface_name(iface);
    
    // Initialize codec with default configuration
    aom_codec_err_t init_result;
    if (codec_type == 0) {
        // Encoder initialization
        aom_codec_enc_cfg_t cfg;
        aom_codec_enc_config_default(iface, &cfg, 0);
        
        // Set some basic parameters from fuzzer input
        cfg.g_w = fdp.ConsumeIntegralInRange<unsigned int>(16, 512);
        cfg.g_h = fdp.ConsumeIntegralInRange<unsigned int>(16, 512);
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;
        cfg.g_pass = AOM_RC_ONE_PASS;
        cfg.g_lag_in_frames = 0;
        cfg.rc_end_usage = AOM_Q;
        cfg.rc_min_quantizer = 2;
        cfg.rc_max_quantizer = 58;
        cfg.rc_target_bitrate = 256;
        
        init_result = aom_codec_enc_init_ver(&codec, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    } else {
        // Decoder initialization
        aom_codec_dec_cfg_t cfg;
        cfg.threads = 1;
        cfg.w = 0;
        cfg.h = 0;
        cfg.allow_lowbitdepth = 1;
        
        init_result = aom_codec_dec_init_ver(&codec, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    }
    
    if (init_result != AOM_CODEC_OK) {
        // Initialization failed, but we've already called aom_codec_iface_name
        // which was our primary target
        return 0;
    }
    
    // Get interface name again after initialization (should be the same)
    const char* iface_name_after_init = aom_codec_iface_name(codec.iface);
    
    // PHASE 2: Image Creation and Flipping
    
    // Step 3: Create an image for flipping operation
    // Consume image parameters from fuzzer input
    unsigned int img_width = fdp.ConsumeIntegralInRange<unsigned int>(16, 256);
    unsigned int img_height = fdp.ConsumeIntegralInRange<unsigned int>(16, 256);
    
    // Select image format from valid options
    uint8_t format_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    aom_img_fmt_t img_format;
    switch (format_choice) {
        case 0:
            img_format = AOM_IMG_FMT_I420;
            break;
        case 1:
            img_format = AOM_IMG_FMT_YV12;
            break;
        case 2:
            img_format = AOM_IMG_FMT_I422;
            break;
        case 3:
            img_format = AOM_IMG_FMT_I444;
            break;
        default:
            img_format = AOM_IMG_FMT_I420;
    }
    
    // Alignment (must be power of 2, at least 1)
    unsigned int alignment = 1 << (fdp.ConsumeIntegralInRange<uint8_t>(0, 4));
    
    // Create image using aom_img_alloc
    aom_image_t* image = aom_img_alloc(NULL, img_format, img_width, img_height, alignment);
    if (image == nullptr) {
        // Clean up codec before returning
        aom_codec_destroy(&codec);
        return 0;
    }
    
    // Fill image planes with some data from fuzzer input to make flipping meaningful
    size_t total_pixels = img_width * img_height;
    size_t bytes_needed = total_pixels; // Simplified estimate
    
    if (fdp.remaining_bytes() >= bytes_needed) {
        // Fill Y plane
        std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(total_pixels);
        if (y_data.size() == total_pixels) {
            for (unsigned int y = 0; y < img_height; ++y) {
                memcpy(image->planes[AOM_PLANE_Y] + y * image->stride[AOM_PLANE_Y],
                       y_data.data() + y * img_width,
                       img_width);
            }
        }
        
        // For planar formats, fill U and V planes if we have enough data
        if (img_format == AOM_IMG_FMT_I420 || img_format == AOM_IMG_FMT_YV12) {
            size_t uv_pixels = (img_width / 2) * (img_height / 2);
            if (fdp.remaining_bytes() >= uv_pixels * 2) {
                std::vector<uint8_t> u_data = fdp.ConsumeBytes<uint8_t>(uv_pixels);
                std::vector<uint8_t> v_data = fdp.ConsumeBytes<uint8_t>(uv_pixels);
                
                for (unsigned int y = 0; y < img_height / 2; ++y) {
                    memcpy(image->planes[AOM_PLANE_U] + y * image->stride[AOM_PLANE_U],
                           u_data.data() + y * (img_width / 2),
                           img_width / 2);
                    memcpy(image->planes[AOM_PLANE_V] + y * image->stride[AOM_PLANE_V],
                           v_data.data() + y * (img_width / 2),
                           img_width / 2);
                }
            }
        }
    }
    
    // Step 4: Call aom_img_flip on the created image
    // This is the primary image operation target
    aom_img_flip(image);
    
    // PHASE 3: Cleanup
    
    // Step 5: Clean up resources
    aom_img_free(image);
    aom_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_005
