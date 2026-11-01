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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_276(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    aom_codec_ctx_t codec_ctx;
    memset(&codec_ctx, 0, sizeof(codec_ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings safely
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = Data[2] + 1;
    cfg.g_timebase.den = Data[3] + 1;
    cfg.rc_target_bitrate = (Data[4] % 1000) + 100;
    cfg.g_threads = (Data[5] % 4) + 1;

    if (aom_codec_enc_init(&codec_ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz various control functions with derived values from input
    int val;

    val = Data[10] % 100;
    aom_codec_control(&codec_ctx, AV1E_SET_NOISE_SENSITIVITY, val);

    val = Data[11] % 10;
    aom_codec_control(&codec_ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, val);

    val = Data[12] % 1000;
    aom_codec_control(&codec_ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, val);

    val = Data[13] % 2;
    aom_codec_control(&codec_ctx, AV1E_SET_ENABLE_INTRABC, val);

    val = Data[14] % 100;
    aom_codec_control(&codec_ctx, AV1E_SET_GF_CBR_BOOST_PCT, val);

    val = Data[15] % 3; // 0: PSNR, 1: SSIM, 2: unspecified
    aom_codec_control(&codec_ctx, AOME_SET_TUNING, val);

    // Cleanup
    cleanup_codec(&codec_ctx);

    return 0;
}