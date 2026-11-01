// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_307(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Use the first byte to decide between encoder and decoder paths
    int is_encoder_path = Data[0] & 1;

    if (is_encoder_path) {
        aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
        if (!ctx) return 0;

        aom_codec_iface_t *iface = aom_codec_av1_cx();
        if (!iface) {
            free(ctx);
            return 0;
        }

        aom_codec_enc_cfg_t cfg;
        if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
            free(ctx);
            return 0;
        }

        // Use fuzzing data to set some configuration values safely
        cfg.g_w = (Size > 1) ? Data[1] : 64;
        cfg.g_h = (Size > 2) ? Data[2] : 64;
        if (cfg.g_w == 0) cfg.g_w = 64;
        if (cfg.g_h == 0) cfg.g_h = 64;

        cfg.g_timebase.num = (Size > 3) ? Data[3] : 1;
        cfg.g_timebase.den = (Size > 4) ? Data[4] : 30;
        if (cfg.g_timebase.num == 0) cfg.g_timebase.num = 1;
        if (cfg.g_timebase.den == 0) cfg.g_timebase.den = 30;

        cfg.rc_target_bitrate = (Size > 5) ? Data[5] : 1000;
        if (cfg.rc_target_bitrate == 0) cfg.rc_target_bitrate = 1000;

        cfg.g_threads = (Size > 6) ? Data[6] : 1;
        if (cfg.g_threads == 0) cfg.g_threads = 1;

        if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
            free(ctx);
            return 0;
        }

        // Fuzz control functions for encoder
        int32_t sharpness = (Size > 7) ? (int32_t)Data[7] : 0;
        aom_codec_control(ctx, AOME_SET_SHARPNESS, sharpness);

        int32_t static_threshold = (Size > 8) ? (int32_t)Data[8] : 0;
        aom_codec_control(ctx, AOME_SET_STATIC_THRESHOLD, static_threshold);

        int32_t mode_cost_upd_freq = (Size > 9) ? (int32_t)Data[9] : 0;
        aom_codec_control(ctx, AV1E_SET_MODE_COST_UPD_FREQ, mode_cost_upd_freq);

        int32_t aq_mode = (Size > 10) ? (int32_t)Data[10] : 0;
        aom_codec_control(ctx, AV1E_SET_AQ_MODE, aq_mode);

        int32_t max_intra_bitrate_pct = (Size > 11) ? (int32_t)Data[11] : 0;
        aom_codec_control(ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, max_intra_bitrate_pct);

        cleanup_encoder(ctx);
    } else {
        aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
        if (!ctx) return 0;

        aom_codec_iface_t *iface = aom_codec_av1_dx();
        if (!iface) {
            free(ctx);
            return 0;
        }

        aom_codec_dec_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = (Size > 1) ? Data[1] : 1;
        if (cfg.threads == 0) cfg.threads = 1;

        if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
            free(ctx);
            return 0;
        }

        // Fuzz control function for decoder
        int32_t is_annexb = (Size > 2) ? (int32_t)Data[2] : 0;
        aom_codec_control(ctx, AV1D_SET_IS_ANNEXB, is_annexb);

        cleanup_decoder(ctx);
    }

    return 0;
}