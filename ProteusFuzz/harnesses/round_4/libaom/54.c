// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret;
    int value;

    if (size < sizeof(int)) return 0;
    memcpy(&value, data, sizeof(int));

    // AV1E_SET_ENABLE_PALETTE
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_PALETTE, value);
    (void)ret;

    if (size >= 2 * sizeof(int)) {
        int mtu;
        memcpy(&mtu, data + sizeof(int), sizeof(int));
        // AV1E_SET_MTU
        ret = aom_codec_control(ctx, AV1E_SET_MTU, mtu);
        (void)ret;
    }

    if (size >= 3 * sizeof(int)) {
        int min_cr;
        memcpy(&min_cr, data + 2 * sizeof(int), sizeof(int));
        // AV1E_SET_MIN_CR
        ret = aom_codec_control(ctx, AV1E_SET_MIN_CR, min_cr);
        (void)ret;
    }

    if (size >= 4 * sizeof(int)) {
        int enable_sb;
        memcpy(&enable_sb, data + 3 * sizeof(int), sizeof(int));
        // AV1E_ENABLE_SB_MULTIPASS_UNIT_TEST
        ret = aom_codec_control(ctx, AV1E_ENABLE_SB_MULTIPASS_UNIT_TEST, enable_sb);
        (void)ret;
    }

    if (size >= 5 * sizeof(int)) {
        int enable_overlay;
        memcpy(&enable_overlay, data + 4 * sizeof(int), sizeof(int));
        // AV1E_SET_ENABLE_OVERLAY
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_OVERLAY, enable_overlay);
        (void)ret;
    }

    if (size >= 6 * sizeof(int)) {
        int qm_min;
        memcpy(&qm_min, data + 5 * sizeof(int), sizeof(int));
        // AV1E_SET_QM_MIN
        ret = aom_codec_control(ctx, AV1E_SET_QM_MIN, qm_min);
        (void)ret;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    aom_codec_err_t err;

    if (Size == 0) return 0;

    // Initialize context
    memset(&ctx, 0, sizeof(ctx));

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize encoder config
    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Set some basic config values
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 0;
    cfg.g_lag_in_frames = 0;

    // Initialize encoder
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Create a dummy image
    memset(&img, 0, sizeof(img));
    img.w = 64;
    img.h = 64;
    img.d_w = 64;
    img.d_h = 64;
    img.stride[0] = 64;
    img.stride[1] = 32;
    img.stride[2] = 32;
    img.planes[0] = malloc(64 * 64);
    img.planes[1] = malloc(32 * 32);
    img.planes[2] = malloc(32 * 32);
    
    if (!img.planes[0] || !img.planes[1] || !img.planes[2]) {
        free(img.planes[0]);
        free(img.planes[1]);
        free(img.planes[2]);
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fill image with data if available
    if (Size > 0) {
        size_t fill_size = Size < 64 * 64 ? Size : 64 * 64;
        memcpy(img.planes[0], Data, fill_size);
        if (Size > 64 * 64) {
            fill_size = Size - 64 * 64;
            size_t uv_size = 32 * 32;
            if (fill_size > uv_size) fill_size = uv_size;
            memcpy(img.planes[1], Data + 64 * 64, fill_size);
            if (Size > 64 * 64 + 32 * 32) {
                fill_size = Size - (64 * 64 + 32 * 32);
                if (fill_size > uv_size) fill_size = uv_size;
                memcpy(img.planes[2], Data + 64 * 64 + 32 * 32, fill_size);
            }
        }
    }

    // Encode one frame (5 arguments: ctx, img, pts, duration, flags)
    aom_codec_encode(&ctx, &img, 0, 1, 0);

    // Fuzz the control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    free(img.planes[0]);
    free(img.planes[1]);
    free(img.planes[2]);
    aom_codec_destroy(&ctx);

    return 0;
}