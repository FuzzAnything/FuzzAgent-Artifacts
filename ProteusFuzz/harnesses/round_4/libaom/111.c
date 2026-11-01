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
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (!ctx || !cfg) return 0;

    // Use the correct function name aom_codec_av1_cx()
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to set some config parameters safely
    if (size > 0) {
        cfg->g_w = (data[0] % 256) * 16 + 16;
        if (size > 1) cfg->g_h = (data[1] % 256) * 16 + 16;
        if (size > 2) cfg->g_timebase.num = (data[2] % 255) + 1;
        if (size > 3) cfg->g_timebase.den = (data[3] % 255) + 1;
        if (size > 4) cfg->g_threads = (data[4] % 16) + 1;
        if (size > 5) cfg->g_error_resilient = data[5] % 2;
        if (size > 6) cfg->rc_target_bitrate = (data[6] % 1000) + 100;
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    aom_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    if (!init_encoder(&ctx, &cfg, Data, Size)) {
        return 0;
    }

    // Fuzz the control functions with diverse values from input
    int val;
    if (Size > 0) val = Data[0] % 2;
    else val = 0;
    aom_codec_control(&ctx, AOME_SET_ENABLEAUTOBWDREF, val);

    if (Size > 1) val = Data[1] % 2;
    else val = 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_ONESIDED_COMP, val);

    if (Size > 2) val = Data[2] % 2;
    else val = 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val);

    if (Size > 3) val = Data[3] % 2;
    else val = 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val);

    if (Size > 4) val = Data[4] % 4; // Assuming S_FRAME_MODE has more states
    else val = 0;
    aom_codec_control(&ctx, AV1E_SET_S_FRAME_MODE, val);

    if (Size > 5) val = Data[5] % 3; // Assuming TIMING_INFO_TYPE has multiple types
    else val = 0;
    aom_codec_control(&ctx, AV1E_SET_TIMING_INFO_TYPE, val);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}