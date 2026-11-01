// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// VP9E_GET_LEVEL__value at vp8cx.h:1068:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_codec.h>

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

static vpx_codec_ctx_t* create_encoder(const vpx_codec_iface_t *codec) {
    vpx_codec_ctx_t *ctx = calloc(1, sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(codec, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    if (vpx_codec_enc_init(ctx, codec, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    const vpx_codec_iface_t *codec = vpx_codec_vp9_cx();
    if (!codec) return 0;

    vpx_codec_ctx_t *ctx = create_encoder(codec);
    if (!ctx) return 0;

    // Fuzz VP9E_SET_SVC_INTER_LAYER_PRED
    int inter_layer_pred = Data[0] % 2;
    vpx_codec_control(ctx, VP9E_SET_SVC_INTER_LAYER_PRED, inter_layer_pred);

    // Fuzz VP9E_SET_SVC
    int svc_mode = Data[Size > 1 ? 1 : 0] % 2;
    vpx_codec_control(ctx, VP9E_SET_SVC, svc_mode);

    // Fuzz VP9E_SET_SVC_LAYER_ID
    vpx_svc_layer_id_t layer_id;
    if (Size > 4) {
        layer_id.spatial_layer_id = Data[2] % 4;
        layer_id.temporal_layer_id = Data[3] % 8;
        for (int i = 0; i < VPX_SS_MAX_LAYERS && Size > 4 + i; i++) {
            layer_id.temporal_layer_id_per_spatial[i] = Data[4 + i] % 8;
        }
    } else {
        layer_id.spatial_layer_id = 0;
        layer_id.temporal_layer_id = 0;
        memset(layer_id.temporal_layer_id_per_spatial, 0, sizeof(layer_id.temporal_layer_id_per_spatial));
    }
    vpx_codec_control(ctx, VP9E_SET_SVC_LAYER_ID, &layer_id);

    // Fuzz VP9E_SET_TARGET_LEVEL
    int target_level = Data[Size > 5 ? 5 : 0] % 10;
    vpx_codec_control(ctx, VP9E_SET_TARGET_LEVEL, target_level);

    // Fuzz VP9E_SET_SVC_PARAMETERS
    // The previous crash occurred because VP9E_SET_SVC_PARAMETERS expects a vpx_svc_parameters_t structure,
    // but we were passing a vpx_svc_layer_id_t structure. Since we don't have the full definition of
    // vpx_svc_parameters_t in the provided context and it requires complex initialization,
    // we will skip this specific control to avoid undefined behavior or memory corruption.
    // If the structure were available, we would initialize it carefully here.
    // vpx_codec_control(ctx, VP9E_SET_SVC_PARAMETERS, &svc_params); // Skipped for safety

    // Fuzz VP9E_GET_LEVEL
    int level;
    vpx_codec_control(ctx, VP9E_GET_LEVEL, &level);

    cleanup_encoder(ctx);
    return 0;
}