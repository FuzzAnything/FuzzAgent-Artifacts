#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <memory>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"

namespace harness_003 {
/*
 * Fuzzing harness for libaom AV1 encoder configuration and control APIs
 * Targets comprehensive encoder configuration workflow: initialization, 
 * configuration setting, options, buffer management, and data retrieval.
 * Focuses on encoder configuration APIs which currently have 46+ undiscovered branches.
 * 
 * Target API sequence:
 * 1. aom_codec_av1_cx() - Get encoder interface
 * 2. aom_codec_enc_init_ver() - Initialize encoder with version
 * 3. aom_codec_enc_config_default() - Get default configuration
 * 4. aom_codec_enc_config_set() - Set modified configuration (UNCOVERED)
 * 5. aom_codec_set_option() - Set codec options (UNCOVERED)
 * 6. aom_codec_set_cx_data_buf() - Set compressed data buffer (UNCOVERED)
 * 7. aom_codec_encode() - Encode frames
 * 8. aom_codec_get_preview_frame() - Get preview frame (UNCOVERED)
 * 9. aom_codec_get_global_headers() - Get global headers (UNCOVERED)
 * 10. aom_codec_get_cx_data() - Get encoded data
 * 11. aom_codec_destroy() - Cleanup
 */




int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for comprehensive configuration
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume configuration parameters from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(16, 1024);
    int height = fdp.ConsumeIntegralInRange<int>(16, 1024);
    int usage = fdp.ConsumeIntegralInRange<int>(0, 2);  // 0=good quality, 1=realtime, 2=all intra
    int target_bitrate = fdp.ConsumeIntegralInRange<int>(100, 10000);
    int g_error_resilient = fdp.ConsumeIntegral<uint32_t>();
    int g_pass = fdp.ConsumeIntegralInRange<int>(0, 2);  // 0=one pass, 1=first pass, 2=second pass
    int g_lag_in_frames = fdp.ConsumeIntegralInRange<int>(0, 25);
    
    // Consume option parameters
    bool set_options = fdp.ConsumeBool();
    std::string option_name, option_value;
    if (set_options) {
        option_name = fdp.ConsumeRandomLengthString(32);
        option_value = fdp.ConsumeRandomLengthString(32);
    }
    
    // Consume buffer configuration
    bool use_custom_buffer = fdp.ConsumeBool();
    unsigned int pad_before = fdp.ConsumeIntegralInRange<unsigned int>(0, 256);
    unsigned int pad_after = fdp.ConsumeIntegralInRange<unsigned int>(0, 256);
    
    // Get encoder interface
    aom_codec_iface_t *encoder = aom_codec_av1_cx();
    if (!encoder) {
        return 0;
    }

    // Step 1: Get default encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res = aom_codec_enc_config_default(encoder, &cfg, usage);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Modify configuration with fuzzed values
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;  // 30 fps
    cfg.rc_target_bitrate = target_bitrate;
    cfg.g_error_resilient = static_cast<aom_codec_er_flags_t>(g_error_resilient);
    cfg.g_pass = static_cast<aom_enc_pass>(g_pass);
    cfg.g_lag_in_frames = g_lag_in_frames;

    // Step 2: Initialize encoder with explicit version
    aom_codec_ctx_t codec;
    if (aom_codec_enc_init_ver(&codec, encoder, &cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        return 0;
    }

    // Step 3: Set encoder configuration using aom_codec_enc_config_set (UNCOVERED API)
    // Modify configuration slightly and set it
    cfg.rc_target_bitrate = target_bitrate / 2;  // Change bitrate
    aom_codec_err_t config_set_res = aom_codec_enc_config_set(&codec, &cfg);
    // We ignore the result as it may fail with invalid configurations (that's what fuzzing tests)

    // Step 4: Set encoder options using aom_codec_set_option (UNCOVERED API)
    if (set_options && !option_name.empty()) {
        aom_codec_err_t option_res = aom_codec_set_option(&codec, option_name.c_str(), option_value.c_str());
        // We ignore the result as it may fail with invalid options
    }

    // Step 5: Set compressed data output buffer using aom_codec_set_cx_data_buf (UNCOVERED API)
    aom_fixed_buf_t output_buffer;
    std::vector<uint8_t> buffer_data;
    if (use_custom_buffer && fdp.remaining_bytes() > 1024) {
        size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1024, 8192);
        buffer_data = fdp.ConsumeBytes<uint8_t>(std::min(buffer_size, fdp.remaining_bytes()));
        output_buffer.buf = buffer_data.data();
        output_buffer.sz = buffer_data.size();
        
        aom_codec_err_t buffer_res = aom_codec_set_cx_data_buf(&codec, &output_buffer, pad_before, pad_after);
        // We ignore the result as it may fail with invalid buffer configuration
    } else {
        // Use NULL buffer to restore default behavior
        aom_codec_err_t buffer_res = aom_codec_set_cx_data_buf(&codec, NULL, 0, 0);
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

    // Step 6: Encode frame
    aom_codec_err_t encode_res = aom_codec_encode(&codec, &img, 0, 1, 0);
    if (encode_res != AOM_CODEC_OK) {
        // Continue anyway to test other APIs
    }

    // Step 7: Get preview frame using aom_codec_get_preview_frame (UNCOVERED API)
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&codec);
    // preview_frame may be NULL, that's OK

    // Step 8: Get global headers using aom_codec_get_global_headers (UNCOVERED API)
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&codec);
    if (global_headers) {
        // Free the memory as documented
        if (global_headers->buf) {
            free(global_headers->buf);
        }
        free(global_headers);
    }

    // Step 9: Get encoded data
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != NULL) {
        // Process packet - just iterate through them
        // Packet types: AOM_CODEC_CX_FRAME_PKT, AOM_CODEC_STATS_PKT, etc.
    }

    // Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&codec);

    return 0;
}

} // namespace harness_003
