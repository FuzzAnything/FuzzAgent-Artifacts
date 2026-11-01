// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_enc_cfg_t cfg;
    // Use aom_codec_av1_cx() to get the AV1 encoder interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    if (!iface) {
        return 0;
    }

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters based on input data
    if (size > 0) {
        cfg.g_w = (data[0] % 512) + 16; // Ensure width is at least 16
        cfg.g_h = (data[1 % size] % 512) + 16; // Ensure height is at least 16
        if (size > 2) cfg.g_timebase.num = (data[2 % size] % 100) + 1;
        if (size > 3) cfg.g_timebase.den = (data[3 % size] % 1000) + 1;
        if (size > 4) cfg.g_threads = (data[4 % size] % 8) + 1;
        if (size > 5) cfg.rc_target_bitrate = (data[5 % size] % 10000) + 100;
    } else {
        cfg.g_w = 128;
        cfg.g_h = 128;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;
        cfg.rc_target_bitrate = 1000;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int ret;
    int val;

    if (Size < 2) {
        return 0;
    }

    memset(&ctx, 0, sizeof(ctx));

    if (!init_encoder(&ctx, Data, Size)) {
        return 0;
    }

    // Extract a value from the input to use as the control parameter
    // Ensure the value is within a reasonable range (0-1 for booleans, or 0-255 for others)
    val = Data[0] % 256;

    // 1. AV1E_SET_ENABLE_PALETTE
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, val);
    (void)ret;

    // 2. AV1E_SET_ENABLE_INTERINTRA_COMP
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, val);
    (void)ret;

    // 3. AV1E_SET_ENABLE_MASKED_COMP
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_MASKED_COMP, val);
    (void)ret;

    // 4. AV1E_SET_ENABLE_RECT_PARTITIONS
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, val);
    (void)ret;

    // 5. AV1E_SET_QM_MIN
    ret = aom_codec_control(&ctx, AV1E_SET_QM_MIN, val);
    (void)ret;

    // 6. AV1E_SET_ENABLE_GLOBAL_MOTION
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val);
    (void)ret;

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}