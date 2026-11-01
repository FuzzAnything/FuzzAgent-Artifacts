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

#include "aomcx.h"
#include "aomdx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_external_partition.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"
#include "aom_encoder.h"
#include "aom_integer.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return -1;
    }

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Data == NULL || Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Fuzz control commands with input data
    int32_t value;
    if (Size >= sizeof(int32_t)) {
        memcpy(&value, Data, sizeof(int32_t));
    } else {
        value = (int32_t)Size;
    }

    // Try AV1E_SET_REDUCED_REFERENCE_SET
    aom_codec_control(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, value);

    // Try AV1E_SET_ENABLE_ONESIDED_COMP
    aom_codec_control(&ctx, AV1E_SET_ENABLE_ONESIDED_COMP, value);

    // Try AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, value);

    // Try AV1E_SET_ENABLE_REF_FRAME_MVS
    aom_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, value);

    // Try AV1E_SET_ALLOW_WARPED_MOTION
    aom_codec_control(&ctx, AV1E_SET_ALLOW_WARPED_MOTION, value);

    // Try AV1E_SET_ENABLE_FILTER_INTRA
    aom_codec_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, value);

    cleanup_codec(&ctx);
    return 0;
}