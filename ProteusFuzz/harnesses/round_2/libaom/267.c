// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static int initialize_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
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

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_267(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (initialize_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    int val = 0;
    if (Size >= 1) {
        val = Data[0];
    }

    // Fuzz AV1E_SET_RTC_EXTERNAL_RC (takes int)
    if (Size >= 2) {
        val = Data[1];
        aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, val);
    }

    // Fuzz AV1E_SET_MIN_PARTITION_SIZE (takes int)
    if (Size >= 3) {
        val = Data[2];
        aom_codec_control(&ctx, AV1E_SET_MIN_PARTITION_SIZE, val);
    }

    // Fuzz AOME_SET_ENABLEAUTOBWDREF (takes int)
    if (Size >= 4) {
        val = Data[3];
        aom_codec_control(&ctx, AOME_SET_ENABLEAUTOBWDREF, val);
    }

    // Fuzz AV1E_SET_ENABLE_INTRA_EDGE_FILTER (takes int)
    if (Size >= 5) {
        val = Data[4];
        aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, val);
    }

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ (takes int)
    if (Size >= 6) {
        val = Data[5];
        aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val);
    }

    // Fuzz AV1E_SET_EXTERNAL_PARTITION (takes aom_ext_part_funcs_t *)
    if (Size >= 7) {
        static aom_ext_part_funcs_t ext_funcs;
        memset(&ext_funcs, 0, sizeof(ext_funcs));
        // Set some dummy values if data is available
        if (Size >= 8) {
            ext_funcs.decision_mode = (aom_ext_part_decision_mode_t)(Data[7] & 1);
        }
        aom_codec_control(&ctx, AV1E_SET_EXTERNAL_PARTITION, &ext_funcs);
    }

    cleanup_encoder(&ctx);
    return 0;
}