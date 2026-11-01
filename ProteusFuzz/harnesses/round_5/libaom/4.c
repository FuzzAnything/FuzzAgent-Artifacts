// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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
 * Targets:
 * - AV1E_SET_ENABLE_INTRABC
 * - AV1E_SET_INTER_DCT_ONLY
 * - AV1E_SET_NOISE_SENSITIVITY
 * - AV1E_SET_S_FRAME_MODE
 * - AV1E_SET_ENABLE_FLIP_IDTX
 * - AV1E_SET_REDUCED_TX_TYPE_SET
 */

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 0;
    cfg->g_lag_in_frames = 0;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_control_int(aom_codec_ctx_t *ctx, int control_id, int32_t value) {
    return aom_codec_control(ctx, control_id, value) == AOM_CODEC_OK;
}

static int fuzz_control_uint(aom_codec_ctx_t *ctx, int control_id, uint32_t value) {
    return aom_codec_control(ctx, control_id, value) == AOM_CODEC_OK;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int ret;
    int idx = 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    /*
     * Use input data to drive diverse control calls.
     * We map ranges of input bytes to specific control IDs and values.
     */

    /* 1. AV1E_SET_ENABLE_INTRABC (int32_t: 0 or 1) */
    if (idx < Size) {
        int32_t val = (Data[idx++] & 1);
        fuzz_control_int(&ctx, AV1E_SET_ENABLE_INTRABC, val);
    }

    /* 2. AV1E_SET_INTER_DCT_ONLY (int32_t: 0 or 1) */
    if (idx < Size) {
        int32_t val = (Data[idx++] & 1);
        fuzz_control_int(&ctx, AV1E_SET_INTER_DCT_ONLY, val);
    }

    /* 3. AV1E_SET_NOISE_SENSITIVITY (int32_t: 0..100) */
    if (idx < Size) {
        int32_t val = Data[idx++] % 101;
        fuzz_control_int(&ctx, AV1E_SET_NOISE_SENSITIVITY, val);
    }

    /* 4. AV1E_SET_S_FRAME_MODE (int32_t: 0..2) */
    if (idx < Size) {
        int32_t val = Data[idx++] % 3;
        fuzz_control_int(&ctx, AV1E_SET_S_FRAME_MODE, val);
    }

    /* 5. AV1E_SET_ENABLE_FLIP_IDTX (int32_t: 0 or 1) */
    if (idx < Size) {
        int32_t val = (Data[idx++] & 1);
        fuzz_control_int(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, val);
    }

    /* 6. AV1E_SET_REDUCED_TX_TYPE_SET (int32_t: 0 or 1) */
    if (idx < Size) {
        int32_t val = (Data[idx++] & 1);
        fuzz_control_int(&ctx, AV1E_SET_REDUCED_TX_TYPE_SET, val);
    }

    /* Additional diverse calls if more data is available */
    while (idx < Size) {
        int control_id;
        int32_t value;
        int mode = Data[idx++] % 6;

        switch (mode) {
            case 0:
                control_id = AV1E_SET_ENABLE_INTRABC;
                value = (Data[idx < Size ? idx : idx] & 1);
                break;
            case 1:
                control_id = AV1E_SET_INTER_DCT_ONLY;
                value = (Data[idx < Size ? idx : idx] & 1);
                break;
            case 2:
                control_id = AV1E_SET_NOISE_SENSITIVITY;
                value = Data[idx < Size ? idx : idx] % 101;
                break;
            case 3:
                control_id = AV1E_SET_S_FRAME_MODE;
                value = Data[idx < Size ? idx : idx] % 3;
                break;
            case 4:
                control_id = AV1E_SET_ENABLE_FLIP_IDTX;
                value = (Data[idx < Size ? idx : idx] & 1);
                break;
            case 5:
                control_id = AV1E_SET_REDUCED_TX_TYPE_SET;
                value = (Data[idx < Size ? idx : idx] & 1);
                break;
            default:
                control_id = AV1E_SET_ENABLE_INTRABC;
                value = 0;
        }

        if (idx < Size) {
            idx++;
        }

        ret = aom_codec_control(&ctx, control_id, value);
        (void)ret;
    }

    cleanup_encoder(&ctx);
    return 0;
}