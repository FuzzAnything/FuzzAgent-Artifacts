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

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    if (!ctx || !cfg) return 0;

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg->g_w = 128;
    cfg->g_h = 128;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};

    if (!init_encoder(&ctx, &cfg)) {
        return 0;
    }

    uint32_t value = 0;
    if (Size >= 4) {
        memcpy(&value, Data, 4);
    }

    int8_t flag = (value & 0xFF) ? 1 : 0;
    int32_t int_val = (int32_t)value;

    // Fuzz AV1E_SET_MAX_PARTITION_SIZE
    aom_codec_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, int_val);

    // Fuzz AV1E_SET_ENABLE_INTERINTER_WEDGE
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, flag);

    // Fuzz AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
    aom_codec_control(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, int_val);

    // Fuzz AV1E_SET_MIN_PARTITION_SIZE
    aom_codec_control(&ctx, AV1E_SET_MIN_PARTITION_SIZE, int_val);

    // Fuzz AV1E_SET_ENABLE_AB_PARTITIONS
    aom_codec_control(&ctx, AV1E_SET_ENABLE_AB_PARTITIONS, flag);

    // Fuzz AV1E_SET_RTC_EXTERNAL_RC
    aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, flag);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}