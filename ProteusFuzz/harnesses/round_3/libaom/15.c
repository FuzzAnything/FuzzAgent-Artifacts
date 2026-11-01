// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_INTRA_DCT_ONLY
 * - AV1E_SET_ENABLE_WARPED_MOTION
 * - AV1E_SET_ENABLE_RECT_PARTITIONS
 * - AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
 * - AV1E_SET_LOOPFILTER_CONTROL
 * - AV1E_SET_MAX_PARTITION_SIZE
 */

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg,
                        const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    if (!ctx || !cfg) return -1;

    /* Use AV1 encoder */
    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    /* Initialize config with defaults */
    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    /* Set some basic parameters from fuzz input if available */
    if (size >= 4) {
        uint32_t width = *(const uint32_t *)data;
        uint32_t height = *(const uint32_t *)(data + 4);
        /* Clamp to reasonable values to avoid allocation issues */
        width = width ? (width > 4096 ? 4096 : width) : 64;
        height = height ? (height > 4096 ? 4096 : height) : 64;
        cfg->g_w = width;
        cfg->g_h = height;
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
        cfg->g_threads = 1;
        cfg->g_lag_in_frames = 0;
        cfg->rc_target_bitrate = 1000;
    } else {
        cfg->g_w = 64;
        cfg->g_h = 64;
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
        cfg->g_threads = 1;
        cfg->g_lag_in_frames = 0;
        cfg->rc_target_bitrate = 1000;
    }

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int ret;
    int val;
    int loopfilter_val;
    int partition_size;

    if (Size < 1) return 0;

    /* Initialize encoder */
    if (init_encoder(&ctx, &cfg, Data, Size) != 0) {
        return 0;
    }

    /* Extract values from input for diverse testing */
    val = (Size > 0) ? Data[0] : 0;
    loopfilter_val = (Size > 1) ? Data[1] : 0;
    partition_size = (Size > 2) ? (int)Data[2] : 0;

    /* 1. AV1E_SET_INTRA_DCT_ONLY: expects int (0 or 1) */
    ret = aom_codec_control(&ctx, AV1E_SET_INTRA_DCT_ONLY, val & 1);
    (void)ret;

    /* 2. AV1E_SET_ENABLE_WARPED_MOTION: expects int (0 or 1) */
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_WARPED_MOTION, (val >> 1) & 1);
    (void)ret;

    /* 3. AV1E_SET_ENABLE_RECT_PARTITIONS: expects int (0 or 1) */
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, (val >> 2) & 1);
    (void)ret;

    /* 4. AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR: expects int (milliseconds) */
    int drop_ms = (Size > 3) ? (int)Data[3] : 0;
    drop_ms = (drop_ms < 0) ? 0 : drop_ms;
    ret = aom_codec_control(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, drop_ms);
    (void)ret;

    /* 5. AV1E_SET_LOOPFILTER_CONTROL: expects int (loopfilter strength or mode) */
    /* Use loopfilter_val directly, may need to be within valid range */
    ret = aom_codec_control(&ctx, AV1E_SET_LOOPFILTER_CONTROL, loopfilter_val);
    (void)ret;

    /* 6. AV1E_SET_MAX_PARTITION_SIZE: expects int (partition size) */
    /* Ensure partition_size is a valid power of 2 or within allowed range */
    if (partition_size > 0 && partition_size <= 128) {
        ret = aom_codec_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, partition_size);
    } else {
        /* Try with a default valid value if input is out of range */
        ret = aom_codec_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, 64);
    }
    (void)ret;

    /* Cleanup */
    cleanup_encoder(&ctx);

    return 0;
}