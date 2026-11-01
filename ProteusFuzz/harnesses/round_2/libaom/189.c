// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_189(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    // Initialize encoder config
    ret = aom_codec_enc_config_default(NULL, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic config values to ensure valid initialization
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    // Initialize encoder context
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return 0;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Extract data from fuzz input to use as parameters
    int param_int = 0;
    if (Size >= sizeof(int)) {
        memcpy(&param_int, Data, sizeof(int));
        // Clamp to reasonable range for boolean-like or small integer settings
        if (param_int < 0) param_int = 0;
        if (param_int > 2) param_int = 2;
    }

    // Fuzz AV1E_SET_CHROMA_SAMPLE_POSITION
    ret = aom_codec_control(ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, param_int);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_COMP
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, param_int);
    (void)ret;

    // Fuzz AV1E_GET_NUM_OPERATING_POINTS
    int num_ops = 0;
    ret = aom_codec_control(ctx, AV1E_GET_NUM_OPERATING_POINTS, &num_ops);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_RECT_PARTITIONS
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, param_int);
    (void)ret;

    // Fuzz AV1E_SET_INTRA_DCT_ONLY
    ret = aom_codec_control(ctx, AV1E_SET_INTRA_DCT_ONLY, param_int);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_WARPED_MOTION
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_WARPED_MOTION, param_int);
    (void)ret;

    // Cleanup
    cleanup_codec(ctx);

    return 0;
}