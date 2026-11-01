// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_profile = 0;
    cfg->g_pass = 0;
    cfg->rc_target_bitrate = 1000;
    
    if (aom_codec_enc_init(ctx, aom_codec_av1_cx(), cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = NULL;
    aom_codec_enc_cfg_t cfg;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return 0;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    if (init_encoder(ctx, &cfg) != 0) {
        cleanup_codec(ctx);
        return 0;
    }

    // Fuzz AOME_SET_CQ_LEVEL
    if (Size >= 1) {
        int cq_level = Data[0] % 64; // Valid range for CQ level
        aom_codec_control(ctx, AOME_SET_CQ_LEVEL, cq_level);
    }

    // Fuzz AOME_SET_ARNR_STRENGTH
    if (Size >= 2) {
        int arnr_strength = Data[1] % 10; // Typical range 0-9
        aom_codec_control(ctx, AOME_SET_ARNR_STRENGTH, arnr_strength);
    }

    // Fuzz AOME_SET_ARNR_MAXFRAMES
    if (Size >= 3) {
        int arnr_maxframes = Data[2] % 16; // Typical range 0-15
        aom_codec_control(ctx, AOME_SET_ARNR_MAXFRAMES, arnr_maxframes);
    }

    // Fuzz AV1E_SET_AQ_MODE
    if (Size >= 4) {
        int aq_mode = Data[3] % 5; // Typical range 0-4
        aom_codec_control(ctx, AV1E_SET_AQ_MODE, aq_mode);
    }

    // Fuzz AV1E_SET_DELTAQ_STRENGTH
    if (Size >= 5) {
        int deltaq_strength = Data[4] % 64; // Typical range 0-63
        aom_codec_control(ctx, AV1E_SET_DELTAQ_STRENGTH, deltaq_strength);
    }

    // Fuzz AV1E_SET_MODE_COST_UPD_FREQ
    if (Size >= 6) {
        int mode_cost_upd_freq = Data[5] % 100; // Typical range 0-99
        aom_codec_control(ctx, AV1E_SET_MODE_COST_UPD_FREQ, mode_cost_upd_freq);
    }

    // Cleanup
    cleanup_codec(ctx);
    return 0;
}