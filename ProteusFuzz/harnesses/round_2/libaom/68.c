// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

// Helper function to safely initialize the encoder context
static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 0;
    cfg->rc_target_bitrate = 100;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

// Helper function to cleanup encoder context
static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    size_t offset = 0;
    int val;

    // Fuzz AV1E_SET_ENABLE_INTRABC
    if (offset < Size) {
        val = Data[offset++];
        aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val);
    }

    // Fuzz AV1E_SET_ENABLE_DIAGONAL_INTRA
    if (offset < Size) {
        val = Data[offset++];
        aom_codec_control(&ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, val);
    }

    // Fuzz AV1E_SET_TUNE_CONTENT
    if (offset < Size) {
        val = Data[offset++];
        aom_codec_control(&ctx, AV1E_SET_TUNE_CONTENT, val);
    }

    // Fuzz AV1E_SET_ENABLE_AB_PARTITIONS
    if (offset < Size) {
        val = Data[offset++];
        aom_codec_control(&ctx, AV1E_SET_ENABLE_AB_PARTITIONS, val);
    }

    // Fuzz AV1E_SET_ENABLE_CFL_INTRA
    if (offset < Size) {
        val = Data[offset++];
        aom_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, val);
    }

    // Fuzz AV1E_SET_ENABLE_PAETH_INTRA
    if (offset < Size) {
        val = Data[offset++];
        aom_codec_control(&ctx, AV1E_SET_ENABLE_PAETH_INTRA, val);
    }

    // Additional random control calls to explore state space
    for (size_t i = 0; i < Size && offset < Size; i++) {
        int cmd = Data[offset++];
        int arg = (offset < Size) ? Data[offset++] : 0;
        
        int control_id;
        switch (cmd % 6) {
            case 0: control_id = AV1E_SET_ENABLE_INTRABC; break;
            case 1: control_id = AV1E_SET_ENABLE_DIAGONAL_INTRA; break;
            case 2: control_id = AV1E_SET_TUNE_CONTENT; break;
            case 3: control_id = AV1E_SET_ENABLE_AB_PARTITIONS; break;
            case 4: control_id = AV1E_SET_ENABLE_CFL_INTRA; break;
            case 5: control_id = AV1E_SET_ENABLE_PAETH_INTRA; break;
            default: control_id = AV1E_SET_ENABLE_INTRABC;
        }
        aom_codec_control(&ctx, control_id, arg);
    }

    cleanup_encoder(&ctx);
    return 0;
}