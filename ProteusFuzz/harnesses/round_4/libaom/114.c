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
#include <stdlib.h>
#include <string.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) {
        return 0;
    }
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        cleanup_codec(ctx);
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Use fuzz data to configure some basic settings
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    aom_codec_err_t ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Fuzz various control commands based on input data
    int32_t val;
    int cmd;

    // Determine which command to run based on input byte
    switch (Data[0] % 6) {
        case 0:
            // AV1E_SET_BITRATE_ONE_PASS_CBR
            val = (int32_t)(Data[1] * 256 + Data[2]);
            ret = aom_codec_control(ctx, AV1E_SET_BITRATE_ONE_PASS_CBR, val);
            break;
        case 1:
            // AV1E_SET_MODE_COST_UPD_FREQ
            val = (int32_t)(Data[1] * 256 + Data[2]);
            ret = aom_codec_control(ctx, AV1E_SET_MODE_COST_UPD_FREQ, val);
            break;
        case 2:
            // AOME_SET_CQ_LEVEL
            val = (int32_t)(Data[1] * 256 + Data[2]);
            ret = aom_codec_control(ctx, AOME_SET_CQ_LEVEL, val);
            break;
        case 3:
            // AV1E_SET_ENABLE_TX_SIZE_SEARCH
            val = (int32_t)(Data[1] % 2);
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);
            break;
        case 4:
            // AOME_SET_MAX_INTRA_BITRATE_PCT
            val = (int32_t)(Data[1] * 256 + Data[2]);
            ret = aom_codec_control(ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, val);
            break;
        case 5:
            // AV1E_SET_DELTAQ_STRENGTH
            val = (int32_t)(Data[1] * 256 + Data[2]);
            ret = aom_codec_control(ctx, AV1E_SET_DELTAQ_STRENGTH, val);
            break;
        default:
            break;
    }

    // Cleanup
    cleanup_codec(ctx);
    return 0;
}