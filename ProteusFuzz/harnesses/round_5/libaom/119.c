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
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    if (Size < 24) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 1;

    int res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Extract parameters from input data to diversify states
    int32_t param1 = *(const int32_t *)(Data);
    int32_t param2 = *(const int32_t *)(Data + 4);
    int32_t param3 = *(const int32_t *)(Data + 8);
    int32_t param4 = *(const int32_t *)(Data + 12);
    int32_t param5 = *(const int32_t *)(Data + 16);
    int32_t param6 = *(const int32_t *)(Data + 20);

    // Fuzz AV1E_SET_RTC_EXTERNAL_RC
    aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, param1);

    // Fuzz AV1E_SET_ENABLE_PALETTE
    aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, param2);

    // Fuzz AV1E_SET_ENABLE_CHROMA_DELTAQ
    aom_codec_control(&ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, param3);

    // Fuzz AV1E_SET_MIN_PARTITION_SIZE
    aom_codec_control(&ctx, AV1E_SET_MIN_PARTITION_SIZE, param4);

    // Fuzz AV1E_SET_MAX_PARTITION_SIZE
    aom_codec_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, param5);

    // Fuzz AV1E_SET_ENABLE_INTERINTER_WEDGE
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, param6);

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}