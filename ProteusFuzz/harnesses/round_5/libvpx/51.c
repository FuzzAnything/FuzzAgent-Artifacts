// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// VP9E_SET_SVC_FRAME_DROP_LAYER__value at vp8cx.h:1078:1 in vp8cx.h
// VP9E_SET_SVC_REF_FRAME_CONFIG__value at vp8cx.h:1060:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static vpx_codec_ctx_t codec_ctx;
static vpx_codec_enc_cfg_t cfg;
static int codec_initialized = 0;

static void cleanup_codec(void) {
    if (codec_initialized) {
        vpx_codec_destroy(&codec_ctx);
        memset(&codec_ctx, 0, sizeof(codec_ctx));
        codec_initialized = 0;
    }
}

static int init_codec(void) {
    if (codec_initialized) {
        cleanup_codec();
    }

    memset(&cfg, 0, sizeof(cfg));

    if (vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &cfg, 0) != VPX_CODEC_OK) {
        return -1;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;

    if (vpx_codec_enc_init(&codec_ctx, vpx_codec_vp9_cx(), &cfg, 0) != VPX_CODEC_OK) {
        return -1;
    }

    codec_initialized = 1;
    return 0;
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    if (init_codec() != 0) {
        return 0;
    }

    uint8_t mode = Data[0] % 2;

    if (mode == 0) {
        vpx_svc_layer_id_t layer_id;
        memset(&layer_id, 0, sizeof(layer_id));

        if (Size > 1) layer_id.spatial_layer_id = Data[1] % 4;
        if (Size > 2) layer_id.temporal_layer_id = Data[2] % 4;
        if (Size > 3) layer_id.temporal_layer_id_per_spatial[0] = Data[3] % 4;

        vpx_codec_control(&codec_ctx, VP9E_SET_SVC_LAYER_ID, &layer_id);
    } else {
        vpx_svc_frame_drop_t frame_drop;
        memset(&frame_drop, 0, sizeof(frame_drop));

        if (Size > 1) frame_drop.framedrop_thresh[0] = Data[1] % 100;
        if (Size > 2) frame_drop.framedrop_mode = (SVC_LAYER_DROP_MODE)(Data[2] % 2);
        if (Size > 3) frame_drop.max_consec_drop = Data[3] % 10;

        vpx_codec_control(&codec_ctx, VP9E_SET_SVC_FRAME_DROP_LAYER, &frame_drop);
    }

    if (Size > 4) {
        vpx_svc_ref_frame_config_t ref_config;
        memset(&ref_config, 0, sizeof(ref_config));

        for (int i = 0; i < VPX_SS_MAX_LAYERS && (size_t)(4 + i) < Size; i++) {
            ref_config.lst_fb_idx[i] = Data[4 + i] % 8;
        }
        for (int i = 0; i < VPX_SS_MAX_LAYERS && (size_t)(4 + VPX_SS_MAX_LAYERS + i) < Size; i++) {
            ref_config.gld_fb_idx[i] = Data[4 + VPX_SS_MAX_LAYERS + i] % 8;
        }

        vpx_codec_control(&codec_ctx, VP9E_SET_SVC_REF_FRAME_CONFIG, &ref_config);
    }

    cleanup_codec();
    return 0;
}