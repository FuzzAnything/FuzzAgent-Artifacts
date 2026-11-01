// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (ctx == NULL) {
        return 0;
    }

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        cleanup_codec(ctx);
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.rc_target_bitrate = 100;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    uint32_t seed = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
    int val = seed % 2;

    aom_codec_err_t res;

    res = aom_codec_control(ctx, AV1E_SET_ENABLE_QM, val);
    if (res != AOM_CODEC_OK) {
        // Continue to test other controls even if one fails
    }

    res = aom_codec_control(ctx, AV1E_SET_ENABLE_CDEF, val);
    if (res != AOM_CODEC_OK) {
        // Continue
    }

    res = aom_codec_control(ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, val);
    if (res != AOM_CODEC_OK) {
        // Continue
    }

    // AV1E_SET_CHROMA_SUBSAMPLING_Y expects a value (0 or 1 typically)
    res = aom_codec_control(ctx, AV1E_SET_CHROMA_SUBSAMPLING_Y, val);
    if (res != AOM_CODEC_OK) {
        // Continue
    }

    res = aom_codec_control(ctx, AV1E_SET_LOSSLESS, val);
    if (res != AOM_CODEC_OK) {
        // Continue
    }

    res = aom_codec_control(ctx, AV1E_SET_FORCE_VIDEO_MODE, val);
    if (res != AOM_CODEC_OK) {
        // Continue
    }

    cleanup_codec(ctx);
    return 0;
}