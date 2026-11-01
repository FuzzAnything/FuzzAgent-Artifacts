// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

static aom_codec_ctx_t *create_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Safely use fuzzing data to vary configuration slightly
    if (size > 0) {
        cfg.g_w = (data[0] % 1024) + 16;
    }
    if (size > 1) {
        cfg.g_h = (data[1] % 1024) + 16;
    }
    if (size > 2) {
        cfg.g_timebase.num = (data[2] % 100) + 1;
    }
    if (size > 3) {
        cfg.g_timebase.den = (data[3] % 100) + 1;
    }
    if (size > 4) {
        cfg.rc_target_bitrate = (data[4] % 10000) + 100;
    }
    if (size > 5) {
        cfg.g_threads = (data[5] % 8) + 1;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = create_encoder_context(Data, Size);
    if (!ctx) return 0;

    int ret;
    int val;

    // Fuzz AV1E_SET_MAX_PARTITION_SIZE
    val = (Data[0] % 64) + 1;
    ret = aom_codec_control(ctx, AV1E_SET_MAX_PARTITION_SIZE, val);
    (void)ret;

    // Fuzz AV1E_SET_FP_MT_UNIT_TEST (if supported)
    if (Size > 1) {
        val = (Data[1] % 2);
        ret = aom_codec_control(ctx, AV1E_SET_FP_MT_UNIT_TEST, val);
        (void)ret;
    }

    // Fuzz AV1E_SET_ENABLE_PALETTE
    if (Size > 2) {
        val = (Data[2] % 2);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_PALETTE, val);
        (void)ret;
    }

    // Fuzz AV1E_SET_INTRA_DEFAULT_TX_ONLY
    if (Size > 3) {
        val = (Data[3] % 2);
        ret = aom_codec_control(ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, val);
        (void)ret;
    }

    // Fuzz AV1E_SET_POSTENCODE_DROP_RTC
    if (Size > 4) {
        val = (Data[4] % 2);
        ret = aom_codec_control(ctx, AV1E_SET_POSTENCODE_DROP_RTC, val);
        (void)ret;
    }

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    if (Size > 5) {
        val = (Data[5] % 2);
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);
        (void)ret;
    }

    // Cleanup
    aom_codec_destroy(ctx);
    free(ctx);

    return 0;
}