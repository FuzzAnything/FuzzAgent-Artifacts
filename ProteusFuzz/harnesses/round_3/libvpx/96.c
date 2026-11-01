// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// VP9E_SET_SVC_GF_TEMPORAL_REF__value at vp8cx.h:1082:1 in vp8cx.h
// VP9E_SET_MAX_GF_INTERVAL__value at vp8cx.h:1054:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// VP9E_SET_SVC_INTER_LAYER_PRED__value at vp8cx.h:1076:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static void cleanup_codec(vpx_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        vpx_codec_destroy(ctx);
    }
    if (ctx) {
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)calloc(1, sizeof(vpx_codec_ctx_t));
    if (!ctx) {
        return 0;
    }

    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) {
        cleanup_codec(ctx);
        return 0;
    }

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Use input data to diversify configuration safely
    cfg.g_w = Data[0] ? Data[0] : 16;
    cfg.g_h = Data[1] ? Data[1] : 16;
    cfg.g_timebase.num = Data[2] ? Data[2] : 1;
    cfg.g_timebase.den = Data[3] ? Data[3] : 1;

    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Fuzz VP9E_SET_ROW_MT__value (assumed to take an int)
    int row_mt_val = Data[4] & 0xFF;
    vpx_codec_control(ctx, VP9E_SET_ROW_MT, row_mt_val);

    // Fuzz VP9E_SET_SVC_GF_TEMPORAL_REF__value (assumed to take an int)
    int svc_ref_val = Data[5] & 0xFF;
    vpx_codec_control(ctx, VP9E_SET_SVC_GF_TEMPORAL_REF, svc_ref_val);

    // Fuzz VP9E_SET_MAX_GF_INTERVAL__value (assumed to take an int)
    int max_gf_val = Data[6] & 0xFF;
    vpx_codec_control(ctx, VP9E_SET_MAX_GF_INTERVAL, max_gf_val);

    // Fuzz VP9E_SET_POSTENCODE_DROP__value (assumed to take an int)
    int postencode_drop_val = Data[7] & 0xFF;
    vpx_codec_control(ctx, VP9E_SET_POSTENCODE_DROP, postencode_drop_val);

    // Fuzz VP9E_SET_TARGET_LEVEL__value (assumed to take an int)
    int target_level_val = Data[8] & 0xFF;
    vpx_codec_control(ctx, VP9E_SET_TARGET_LEVEL, target_level_val);

    // Fuzz VP9E_SET_SVC_INTER_LAYER_PRED__value (assumed to take an int)
    int inter_layer_pred_val = Data[9] & 0xFF;
    vpx_codec_control(ctx, VP9E_SET_SVC_INTER_LAYER_PRED, inter_layer_pred_val);

    // Cleanup
    cleanup_codec(ctx);

    return 0;
}