// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Target APIs exercised via aom_codec_control with AV1E_* constants.
 */

static int is_valid_data(const uint8_t *data, size_t size) {
    if (size < 4) return 0;
    return 1;
}

static void setup_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    // Use the correct function name aom_codec_av1_cx
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return;

    // Use input data to seed configuration values if possible, otherwise use defaults
    if (size >= 4) {
        uint32_t seed = *(const uint32_t *)data;
        cfg.g_w = (seed & 0x3FF) | 16; // Width 16-1023
        cfg.g_h = ((seed >> 10) & 0x3FF) | 16; // Height 16-1023
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = (seed >> 20) & 0xFF;
        if (cfg.g_timebase.den == 0) cfg.g_timebase.den = 1;
        cfg.rc_target_bitrate = (seed & 0x7FFFFFFF) | 1;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(ctx);
        memset(ctx, 0, sizeof(aom_codec_ctx_t));
        return;
    }
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (!is_valid_data(Data, Size)) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(aom_codec_ctx_t));

    setup_encoder(&ctx, Data, Size);

    if (ctx.iface == NULL) {
        return 0;
    }

    // Extract parameters from input data to fuzz control values
    int32_t val1 = 0;
    int32_t val2 = 0;
    int32_t val3 = 0;
    int32_t val4 = 0;
    int32_t val5 = 0;
    int32_t val6 = 0;

    if (Size >= 8) {
        val1 = *(const int32_t *)(Data + 0);
        val2 = *(const int32_t *)(Data + 4);
    }
    if (Size >= 16) {
        val3 = *(const int32_t *)(Data + 8);
        val4 = *(const int32_t *)(Data + 12);
    }
    if (Size >= 24) {
        val5 = *(const int32_t *)(Data + 16);
        val6 = *(const int32_t *)(Data + 20);
    }

    // Fuzz AV1E_SET_INTRA_DCT_ONLY
    aom_codec_control(&ctx, AV1E_SET_INTRA_DCT_ONLY, val1);

    // Fuzz AV1E_SET_FORCE_VIDEO_MODE
    aom_codec_control(&ctx, AV1E_SET_FORCE_VIDEO_MODE, val2);

    // Fuzz AV1E_SET_ENABLE_KEYFRAME_FILTERING
    aom_codec_control(&ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, val3);

    // Fuzz AV1E_SET_ENABLE_CDEF
    aom_codec_control(&ctx, AV1E_SET_ENABLE_CDEF, val4);

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
    aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val5);

    // Fuzz AV1E_SET_ENABLE_QM
    aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, val6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}