// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_ENABLE_DNL_DENOISING
 * - AV1E_SET_ENABLE_OVERLAY
 * - AV1E_SET_ENABLE_PALETTE
 * - AV1E_SET_RENDER_SIZE
 * - AV1E_GET_SEQ_LEVEL_IDX
 * - AV1E_SET_ENABLE_CHROMA_DELTAQ
 */

static int fuzz_encoder_controls(const uint8_t *data, size_t size) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_err_t ret;
    aom_codec_enc_cfg_t cfg;

    // Initialize encoder config with defaults
    ret = aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Parse input data to configure encoder settings
    if (size >= 4) {
        cfg.g_w = (uint16_t)(data[0] | (data[1] << 8));
        cfg.g_h = (uint16_t)(data[2] | (data[3] << 8));
        if (cfg.g_w == 0) cfg.g_w = 64;
        if (cfg.g_h == 0) cfg.g_h = 64;
    } else {
        cfg.g_w = 64;
        cfg.g_h = 64;
    }

    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 1;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    // Initialize codec context
    ret = aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Create a dummy input frame
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    
    // Fill image with fuzz data if available
    if (size > 0 && img.planes[0]) {
        // img.planes[i] is uint8_t*, stride is in img.stride[i]
        size_t total_size = (size_t)img.stride[0] * img.h; // Y plane size
        size_t copy_size = (size < total_size) ? size : total_size;
        memcpy(img.planes[0], data, copy_size);
    }

    // Encode the frame
    // Signature: aom_codec_encode(ctx, img, pts, duration, flags)
    ret = aom_codec_encode(&ctx, &img, 0, 1, 0);
    (void)ret;

    // Test Control Functions
    int val;
    aom_codec_err_t ctrl_ret;

    // 1. AV1E_SET_ENABLE_DNL_DENOISING
    val = (size > 4) ? (data[4] & 0x01) : (data[0] & 0x01);
    ctrl_ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, val);
    (void)ctrl_ret;

    // 2. AV1E_SET_ENABLE_OVERLAY
    val = (size > 5) ? (data[5] & 0x01) : (data[0] & 0x01);
    ctrl_ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_OVERLAY, val);
    (void)ctrl_ret;

    // 3. AV1E_SET_ENABLE_PALETTE
    val = (size > 6) ? (data[6] & 0x01) : (data[0] & 0x01);
    ctrl_ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, val);
    (void)ctrl_ret;

    // 4. AV1E_SET_RENDER_SIZE
    uint32_t rw = (size > 7) ? (data[7] | (size > 8 ? data[8] : 0) << 8) : 64;
    uint32_t rh = (size > 9) ? (data[9] | (size > 10 ? data[10] : 0) << 8) : 64;
    if (rw == 0) rw = 64;
    if (rh == 0) rh = 64;
    
    // AV1E_SET_RENDER_SIZE expects a pointer to a struct or two ints.
    // Based on libaom, it takes a pointer to a struct containing width/height.
    // We'll pass a pointer to an array of two uint32_t as a fallback.
    uint32_t render_args[2] = {rw, rh};
    ctrl_ret = aom_codec_control(&ctx, AV1E_SET_RENDER_SIZE, render_args);
    (void)ctrl_ret;

    // 5. AV1E_GET_SEQ_LEVEL_IDX
    int seq_level_idx;
    ctrl_ret = aom_codec_control(&ctx, AV1E_GET_SEQ_LEVEL_IDX, &seq_level_idx);
    (void)ctrl_ret;
    (void)seq_level_idx;

    // 6. AV1E_SET_ENABLE_CHROMA_DELTAQ
    val = (size > 11) ? (data[11] & 0x01) : (data[0] & 0x01);
    ctrl_ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, val);
    (void)ctrl_ret;

    // Finalize encoding
    // Signature: aom_codec_encode(ctx, NULL, pts, duration, flags)
    aom_codec_encode(&ctx, NULL, 0, 0, 0);

    // Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_285(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Ensure we have enough data for basic operations
    if (Size < 4) {
        // Pad with zeros if too small
        static uint8_t padded[4];
        memcpy(padded, Data, Size);
        memset(padded + Size, 0, 4 - Size);
        return fuzz_encoder_controls(padded, 4);
    }

    return fuzz_encoder_controls(Data, Size);
}