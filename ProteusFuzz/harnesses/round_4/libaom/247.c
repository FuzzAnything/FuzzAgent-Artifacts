// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control_typechecked_AV1E_SET_DELTALF_MODE at aomcx.h:2165:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_LOOPFILTER_CONTROL at aomcx.h:2297:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_COLOR_RANGE at aomcx.h:1999:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_CHROMA_SAMPLE_POSITION at aomcx.h:1987:1 in aomcx.h
// aom_codec_control_typechecked_AV1_SET_SKIP_LOOP_FILTER at aomdx.h:513:1 in aomdx.h
// aom_codec_control_typechecked_AOME_GET_LOOPFILTER_LEVEL at aomcx.h:2300:1 in aomcx.h
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

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_247(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int ret;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Fuzz AV1E_SET_DELTALF_MODE
    if (Size >= 1) {
        unsigned int val = Data[0];
        ret = aom_codec_control_typechecked_AV1E_SET_DELTALF_MODE(&ctx, AV1E_SET_DELTALF_MODE, val);
        (void)ret;
    }

    // Fuzz AV1E_SET_LOOPFILTER_CONTROL
    if (Size >= 2) {
        int val = (Data[1] << 8) | Data[0];
        ret = aom_codec_control_typechecked_AV1E_SET_LOOPFILTER_CONTROL(&ctx, AV1E_SET_LOOPFILTER_CONTROL, val);
        (void)ret;
    }

    // Fuzz AV1E_SET_COLOR_RANGE
    if (Size >= 3) {
        int val = Data[2] & 0x1;
        ret = aom_codec_control_typechecked_AV1E_SET_COLOR_RANGE(&ctx, AV1E_SET_COLOR_RANGE, val);
        (void)ret;
    }

    // Fuzz AV1E_SET_CHROMA_SAMPLE_POSITION
    if (Size >= 4) {
        int val = Data[3] & 0x3;
        ret = aom_codec_control_typechecked_AV1E_SET_CHROMA_SAMPLE_POSITION(&ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, val);
        (void)ret;
    }

    // Fuzz AV1_SET_SKIP_LOOP_FILTER
    if (Size >= 5) {
        int val = Data[4] & 0x1;
        ret = aom_codec_control_typechecked_AV1_SET_SKIP_LOOP_FILTER(&ctx, AV1_SET_SKIP_LOOP_FILTER, val);
        (void)ret;
    }

    // Fuzz AOME_GET_LOOPFILTER_LEVEL
    if (Size >= 6) {
        int dummy_val = 0;
        int *val_ptr = &dummy_val;
        ret = aom_codec_control_typechecked_AOME_GET_LOOPFILTER_LEVEL(&ctx, AOME_GET_LOOPFILTER_LEVEL, val_ptr);
        (void)ret;
    }

    cleanup_codec(&ctx);
    return 0;
}