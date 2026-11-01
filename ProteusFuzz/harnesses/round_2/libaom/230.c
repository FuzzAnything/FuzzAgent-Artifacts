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
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

/*
 * Helper function to initialize a codec context for fuzzing.
 * Returns NULL if initialization fails.
 */
static aom_codec_ctx_t *init_fuzz_codec(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    aom_image_t img;
    int width, height;

    // Extract width and height from input if possible, otherwise use defaults
    if (size >= 4) {
        width = (data[0] << 8) | data[1];
        height = (data[2] << 8) | data[3];
        if (width == 0 || height == 0) {
            width = 16;
            height = 16;
        }
        // Cap dimensions to reasonable fuzzing limits
        if (width > 1920) width = 1920;
        if (height > 1080) height = 1080;
    } else {
        width = 16;
        height = 16;
    }

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_lag_in_frames = 0;
    cfg.g_pass = AOM_RC_ONE_PASS;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Prepare a dummy image for encoding
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);
    memset(&img.planes[0][0], 0, width * height); // Y plane
    memset(&img.planes[1][0], 128, (width * height) / 4); // U plane
    memset(&img.planes[2][0], 128, (width * height) / 4); // V plane

    // Encode one frame to ensure the encoder is in a valid state
    // Signature: aom_codec_encode(ctx, img, pts, duration, flags)
    res = aom_codec_encode(ctx, &img, 0, 1, 0);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(ctx);
        aom_img_free(&img);
        free(ctx);
        return NULL;
    }

    aom_img_free(&img);
    return ctx;
}

static void cleanup_fuzz_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_230(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_fuzz_codec(Data, Size);
    if (!ctx) return 0;

    int ret = 0;
    int num_op;
    int max_drop;
    aom_svc_params_t svc_params;
    int num_spatial;
    int seq_level;
    int drop_mode;

    // Initialize SVC params with safe defaults
    memset(&svc_params, 0, sizeof(svc_params));
    svc_params.number_spatial_layers = 1;
    svc_params.number_temporal_layers = 1;
    for (int i = 0; i < AOM_MAX_LAYERS; i++) {
        svc_params.max_quantizers[i] = 63;
        svc_params.min_quantizers[i] = 0;
    }
    for (int i = 0; i < AOM_MAX_SS_LAYERS; i++) {
        svc_params.scaling_factor_num[i] = 1;
        svc_params.scaling_factor_den[i] = 1;
    }
    for (int i = 0; i < AOM_MAX_LAYERS; i++) {
        svc_params.layer_target_bitrate[i] = 1000;
    }
    for (int i = 0; i < AOM_MAX_TS_LAYERS; i++) {
        svc_params.framerate_factor[i] = 1;
    }

    // Fuzz AV1E_GET_NUM_OPERATING_POINTS
    ret = aom_codec_control(ctx, AV1E_GET_NUM_OPERATING_POINTS, &num_op);
    (void)ret;

    // Fuzz AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR
    if (Size >= 2) {
        max_drop = (Data[0] << 8) | Data[1];
        if (max_drop > 100) max_drop = 100; // Cap for safety
        ret = aom_codec_control(ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, &max_drop);
        (void)ret;
    }

    // Fuzz AV1E_SET_SVC_PARAMS
    if (Size >= 4) {
        svc_params.number_spatial_layers = (Data[2] % 3) + 1; // 1 to 3
        svc_params.number_temporal_layers = (Data[3] % 3) + 1; // 1 to 3
        ret = aom_codec_control(ctx, AV1E_SET_SVC_PARAMS, &svc_params);
        (void)ret;
    }

    // Fuzz AOME_SET_NUMBER_SPATIAL_LAYERS
    if (Size >= 5) {
        num_spatial = Data[4] % 4; // 0 to 3
        if (num_spatial == 0) num_spatial = 1;
        ret = aom_codec_control(ctx, AOME_SET_NUMBER_SPATIAL_LAYERS, &num_spatial);
        (void)ret;
    }

    // Fuzz AV1E_SET_TARGET_SEQ_LEVEL_IDX
    if (Size >= 6) {
        seq_level = Data[5] % 10; // 0 to 9
        ret = aom_codec_control(ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, &seq_level);
        (void)ret;
    }

    // Fuzz AV1E_SET_SVC_FRAME_DROP_MODE
    if (Size >= 7) {
        drop_mode = Data[6] % 2; // 0 or 1
        ret = aom_codec_control(ctx, AV1E_SET_SVC_FRAME_DROP_MODE, &drop_mode);
        (void)ret;
    }

    cleanup_fuzz_codec(ctx);
    return 0;
}