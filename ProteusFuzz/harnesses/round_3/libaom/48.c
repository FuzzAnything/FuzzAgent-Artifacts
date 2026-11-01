// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 1) return 0;

    int val = 0;
    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
    }

    // AV1E_SET_ENABLE_TX_SIZE_SEARCH
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val) != AOM_CODEC_OK) {
        // Ignore errors, just continue
    }

    // AV1E_SET_ENABLE_GLOBAL_MOTION
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // AV1E_SET_CDF_UPDATE_MODE
    if (aom_codec_control(ctx, AV1E_SET_CDF_UPDATE_MODE, val) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // AV1E_SET_MODE_COST_UPD_FREQ
    if (aom_codec_control(ctx, AV1E_SET_MODE_COST_UPD_FREQ, val) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // AOME_SET_CQ_LEVEL
    if (aom_codec_control(ctx, AOME_SET_CQ_LEVEL, val) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // AV1E_SET_QM_U
    if (aom_codec_control(ctx, AV1E_SET_QM_U, val) != AOM_CODEC_OK) {
        // Ignore errors
    }

    return 1;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    memset(&ctx, 0, sizeof(ctx));

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 100;

    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Perform fuzzing on encoder controls
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}