// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

/*
 * Fuzz driver for libaom control functions related to quantizers.
 * Target functions:
 * - AOMD_GET_LAST_QUANTIZER
 * - AOMD_GET_BASE_Q_IDX
 * - AOME_SET_CQ_LEVEL
 * - AOME_GET_LAST_QUANTIZER_64
 * - AV1E_SET_QUANTIZER_ONE_PASS
 * - AOME_GET_LAST_QUANTIZER
 */

static void init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return;

    /* Use fuzz data to tweak config slightly */
    if (size > 0) cfg.g_w = (data[0] % 256) + 16;
    if (size > 1) cfg.g_h = (data[1] % 256) + 16;
    if (size > 2) cfg.rc_target_bitrate = (data[2] % 100) + 100;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(ctx);
        return;
    }
}

static void init_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    if (size > 0) cfg.threads = (data[0] % 4) + 1;

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(ctx);
        return;
    }
}

int LLVMFuzzerTestOneInput_155(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx_enc = {0};
    aom_codec_ctx_t ctx_dec = {0};
    int ret = 0;

    /* Initialize encoder and decoder */
    init_encoder(&ctx_enc, Data, Size);
    init_decoder(&ctx_dec, Data, Size);

    /*
     * Helper to safely call control functions
     */
    #define TRY_CTRL(ctx, cmd, arg) do { \
        if ((ctx)->iface) { \
            ret = aom_codec_control((ctx), (cmd), (arg)); \
            (void)ret; /* suppress unused warning */ \
        } \
    } while(0)

    /*
     * Fuzz AOMD_GET_LAST_QUANTIZER (decoder)
     */
    TRY_CTRL(&ctx_dec, AOMD_GET_LAST_QUANTIZER, NULL);

    /*
     * Fuzz AOMD_GET_BASE_Q_IDX (decoder)
     */
    TRY_CTRL(&ctx_dec, AOMD_GET_BASE_Q_IDX, NULL);

    /*
     * Fuzz AOME_SET_CQ_LEVEL (encoder)
     * Argument: int level
     */
    if (Size > 3) {
        int cq_level = (int)(Data[3] % 64);
        TRY_CTRL(&ctx_enc, AOME_SET_CQ_LEVEL, &cq_level);
    }

    /*
     * Fuzz AOME_GET_LAST_QUANTIZER_64 (encoder)
     * Argument: int64_t *val
     */
    if (Size > 4) {
        int64_t val64 = (int64_t)(Data[4] << 16);
        TRY_CTRL(&ctx_enc, AOME_GET_LAST_QUANTIZER_64, &val64);
    }

    /*
     * Fuzz AV1E_SET_QUANTIZER_ONE_PASS (encoder)
     * Argument: int q
     */
    if (Size > 5) {
        int q = (int)(Data[5] % 256);
        TRY_CTRL(&ctx_enc, AV1E_SET_QUANTIZER_ONE_PASS, &q);
    }

    /*
     * Fuzz AOME_GET_LAST_QUANTIZER (encoder)
     * Argument: int *val
     */
    if (Size > 6) {
        int val = (int)(Data[6] % 256);
        TRY_CTRL(&ctx_enc, AOME_GET_LAST_QUANTIZER, &val);
    }

    /* Cleanup */
    aom_codec_destroy(&ctx_enc);
    aom_codec_destroy(&ctx_dec);

    return 0;
}