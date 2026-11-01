// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    if (iface == NULL) {
        return 0;
    }

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    /* Use fuzzing data to configure encoder parameters safely */
    if (size > 0) {
        cfg.g_w = (data[0] % 128) + 16;
        if (size > 1) cfg.g_h = (data[1] % 128) + 16;
        if (size > 2) cfg.g_timebase.num = (data[2] % 100) + 1;
        if (size > 3) cfg.g_timebase.den = (data[3] % 100) + 1;
        if (size > 4) cfg.rc_target_bitrate = (data[4] % 1000) + 100;
        if (size > 5) cfg.g_threads = (data[5] % 8) + 1;
        if (size > 6) cfg.g_pass = (data[6] % 2);
        if (size > 7) cfg.rc_end_usage = (data[7] % 2) ? AOM_CBR : AOM_VBR;
    } else {
        cfg.g_w = 64;
        cfg.g_h = 64;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 500;
        cfg.g_threads = 1;
        cfg.g_pass = 0;
        cfg.rc_end_usage = AOM_VBR;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (!init_encoder(&ctx, Data, Size)) {
        return 0;
    }

    /* Extract values from fuzzing data */
    int val1 = (Size > 0) ? (int)Data[0] : 0;
    int val2 = (Size > 1) ? (int)Data[1] : 0;
    int val3 = (Size > 2) ? (int)Data[2] : 0;
    int val4 = (Size > 3) ? (int)Data[3] : 0;

    /* 
     * Invoke control functions using the correct enum constants 
     * as indicated by the compiler errors.
     */

    /* 1. AV1E_SET_FRAME_PERIODIC_BOOST */
    if (aom_codec_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, val1) == AOM_CODEC_OK) {
        /* Success */
    }

    /* 2. AOME_USE_REFERENCE */
    if (aom_codec_control(&ctx, AOME_USE_REFERENCE, val2) == AOM_CODEC_OK) {
        /* Success */
    }

    /* 3. AV1E_SET_ENABLE_ONESIDED_COMP */
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_ONESIDED_COMP, val3) == AOM_CODEC_OK) {
        /* Success */
    }

    /* 4. AV1E_SET_ENABLE_REF_FRAME_MVS */
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val4) == AOM_CODEC_OK) {
        /* Success */
    }

    /* 5. AV1E_SET_ENABLE_FILTER_INTRA */
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, val1) == AOM_CODEC_OK) {
        /* Success */
    }

    /* 6. AV1E_SET_ENABLE_SMOOTH_INTRA */
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, val2) == AOM_CODEC_OK) {
        /* Success */
    }

    /* Cleanup */
    cleanup_encoder(&ctx);

    return 0;
}