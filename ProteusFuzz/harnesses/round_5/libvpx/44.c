// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int setup_encoder(vpx_codec_ctx_t *ctx, vpx_codec_iface_t *iface) {
    vpx_codec_enc_cfg_t cfg;
    int res;

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return -1;
    }

    cfg.g_w = 160;
    cfg.g_h = 160;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return -1;
    }

    return 0;
}

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    vpx_codec_destroy(ctx);
}

static int fuzz_svc_layer_id(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp9_dx();

    if (!iface) return 0;

    if (setup_encoder(&ctx, iface) != 0) {
        return 0;
    }

    vpx_svc_layer_id_t layer_id;
    memset(&layer_id, 0, sizeof(layer_id));

    if (size >= 4) {
        memcpy(&layer_id.spatial_layer_id, data, 4);
        data += 4; size -= 4;
    }
    if (size >= 4) {
        memcpy(&layer_id.temporal_layer_id, data, 4);
        data += 4; size -= 4;
    }
    
    size_t remaining = size;
    size_t arr_size = sizeof(layer_id.temporal_layer_id_per_spatial);
    size_t copy_len = (remaining < arr_size) ? remaining : arr_size;
    memcpy(layer_id.temporal_layer_id_per_spatial, data, copy_len);

#ifdef VP9E_SET_SVC_LAYER_ID
    vpx_codec_control(&ctx, VP9E_SET_SVC_LAYER_ID, &layer_id);
#endif

    cleanup_encoder(&ctx);
    return 0;
}

static int fuzz_svc_frame_drop(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp9_dx();

    if (!iface) return 0;

    if (setup_encoder(&ctx, iface) != 0) {
        return 0;
    }

    vpx_svc_frame_drop_t frame_drop;
    memset(&frame_drop, 0, sizeof(frame_drop));

    size_t offset = 0;
    size_t thresh_size = sizeof(frame_drop.framedrop_thresh);
    size_t copy_len = (size < thresh_size) ? size : thresh_size;
    
    if (copy_len > 0) {
        memcpy(frame_drop.framedrop_thresh, data, copy_len);
        offset += copy_len;
        size -= copy_len;
    }

    if (size >= 4) {
        memcpy(&frame_drop.framedrop_mode, data + offset, 4);
        offset += 4;
        size -= 4;
    }

    if (size >= 4) {
        memcpy(&frame_drop.max_consec_drop, data + offset, 4);
        offset += 4;
        size -= 4;
    }

#ifdef VP9E_SET_SVC_FRAME_DROP_LAYER
    vpx_codec_control(&ctx, VP9E_SET_SVC_FRAME_DROP_LAYER, &frame_drop);
#endif

    cleanup_encoder(&ctx);
    return 0;
}

static int fuzz_other_svc_controls(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp9_dx();

    if (!iface) return 0;

    if (setup_encoder(&ctx, iface) != 0) {
        return 0;
    }

    int val = 0;
    if (size >= 4) {
        memcpy(&val, data, 4);
    }

#ifdef VP9E_SET_SVC
    vpx_codec_control(&ctx, VP9E_SET_SVC, &val);
#endif

#ifdef VP9E_SET_SVC_GF_TEMPORAL_REF
    vpx_codec_control(&ctx, VP9E_SET_SVC_GF_TEMPORAL_REF, &val);
#endif

#ifdef VP9E_SET_SVC_INTER_LAYER_PRED
    vpx_codec_control(&ctx, VP9E_SET_SVC_INTER_LAYER_PRED, &val);
#endif

    cleanup_encoder(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_svc_layer_id(Data, Size);
    fuzz_svc_frame_drop(Data, Size);
    fuzz_other_svc_controls(Data, Size);

    return 0;
}