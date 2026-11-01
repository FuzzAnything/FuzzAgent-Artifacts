// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_184(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    // Use first byte to decide between encoder and decoder paths
    int mode = Data[0] % 2;
    int control_type = Data[1] % 6; // 0-5 for the 6 target controls

    if (mode == 0) {
        // Encoder path
        aom_codec_ctx_t ctx = {0};
        aom_codec_iface_t *iface = aom_codec_av1_cx();
        if (!iface) return 0;

        aom_codec_enc_cfg_t cfg;
        if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
            return 0;
        }

        // Use fuzzed data to set some config values safely
        cfg.g_w = (Data[2] % 256) + 16;
        cfg.g_h = (Data[3] % 256) + 16;
        cfg.g_timebase.num = (Data[4] % 256) + 1;
        cfg.g_timebase.den = (Data[5] % 256) + 1;
        cfg.rc_target_bitrate = (Data[6] % 1000) + 100;

        if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
            return 0;
        }

        int32_t value = 0;
        aom_codec_err_t res = AOM_CODEC_OK;

        // Map control_type to specific AV1 encoder controls
        switch (control_type) {
            case 0: // AV1E_SET_COLOR_PRIMARIES
                value = (Data[7] % 9); // Valid range 0-8
                res = aom_codec_control(&ctx, AV1E_SET_COLOR_PRIMARIES, value);
                break;
            case 1: // AV1E_SET_ENABLE_INTERINTRA_WEDGE
                value = (Data[7] % 2); // 0 or 1
                res = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, value);
                break;
            case 2: // AV1E_SET_ALLOW_WARPED_MOTION
                value = (Data[7] % 2); // 0 or 1
                res = aom_codec_control(&ctx, AV1E_SET_ALLOW_WARPED_MOTION, value);
                break;
            case 3: // AV1E_SET_ENABLE_RECT_TX
                value = (Data[7] % 2); // 0 or 1
                res = aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_TX, value);
                break;
            case 5: // AV1E_SET_ENABLE_AB_PARTITIONS
                value = (Data[7] % 2); // 0 or 1
                res = aom_codec_control(&ctx, AV1E_SET_ENABLE_AB_PARTITIONS, value);
                break;
            default:
                break;
        }

        (void)res; // Suppress unused variable warning
        cleanup_encoder(&ctx);
    } else {
        // Decoder path
        aom_codec_ctx_t ctx = {0};
        aom_codec_iface_t *iface = aom_codec_av1_dx();
        if (!iface) return 0;

        aom_codec_dec_cfg_t cfg = {0};
        cfg.threads = (Data[2] % 8) + 1;
        cfg.allow_lowbitdepth = (Data[3] % 2);

        if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
            return 0;
        }

        int32_t value = 0;
        aom_codec_err_t res = AOM_CODEC_OK;

        // Only control_type 4 is relevant for decoder (AOMD_GET_ORDER_HINT)
        if (control_type == 4) {
            res = aom_codec_control(&ctx, AOMD_GET_ORDER_HINT, &value);
        }

        (void)res; // Suppress unused variable warning
        cleanup_decoder(&ctx);
    }

    return 0;
}