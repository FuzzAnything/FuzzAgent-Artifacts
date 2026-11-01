// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    struct aom_codec_enc_cfg cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return -1;

    if (size >= 2) {
        cfg.g_w = (data[0] % 255) + 16;
        cfg.g_h = (data[1] % 255) + 16;
    } else {
        cfg.g_w = 16;
        cfg.g_h = 16;
    }

    cfg.g_w = (cfg.g_w + 15) & ~15;
    cfg.g_h = (cfg.g_h + 15) & ~15;

    cfg.g_threads = 1;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    return 0;
}

int LLVMFuzzerTestOneInput_258(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_err_t ret;
    int value;
    int control_id;

    if (Size < 1) return 0;

    memset(&ctx, 0, sizeof(ctx));

    if (init_encoder(&ctx, Data, Size) != 0) {
        return 0;
    }

    int selector = Data[0] % 6;

    switch (selector) {
        case 0:
            control_id = AV1E_SET_INTER_DCT_ONLY;
            break;
        case 1:
            control_id = AV1E_SET_ENABLE_OBMC;
            break;
        case 2:
            control_id = AV1E_SET_COEFF_COST_UPD_FREQ;
            break;
        case 3:
            control_id = AV1E_SET_MIN_CR;
            break;
        case 4:
            control_id = AV1E_SET_ENABLE_SMOOTH_INTRA;
            break;
        case 5:
            /* AV1E_SET_ENABLE_DIST_8X8 is not a standard public macro in recent libaom versions.
             * Using AV1E_SET_ENABLE_RECT_TX as a fallback for testing encoder controls if the specific one is missing.
             * If the target environment has a custom header defining AV1E_SET_ENABLE_DIST_8X8, 
             * this might need adjustment, but for standard compilation, we use a valid control.
             */
            control_id = AV1E_SET_ENABLE_RECT_TX; 
            break;
        default:
            control_id = AV1E_SET_INTER_DCT_ONLY;
            break;
    }

    if (Size >= 2) {
        value = (int)(Data[1] & 0xFF);
        if (Size >= 3) {
            value |= (int)(Data[2] & 0xFF) << 8;
        }
    } else {
        value = 0;
    }

    ret = aom_codec_control(&ctx, control_id, value);

    aom_codec_destroy(&ctx);

    return 0;
}