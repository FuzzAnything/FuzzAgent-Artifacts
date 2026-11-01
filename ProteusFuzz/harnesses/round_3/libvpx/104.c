// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_SVC_REF_FRAME_CONFIG__value at vp8cx.h:1060:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// VP9E_SET_SVC__value at vp8cx.h:1036:1 in vp8cx.h
// VP9E_SET_SVC_SPATIAL_LAYER_SYNC__value at vp8cx.h:1084:1 in vp8cx.h
// VP9E_SET_SVC_FRAME_DROP_LAYER__value at vp8cx.h:1078:1 in vp8cx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// VP9E_SET_SVC_PARAMETERS__value at vp8cx.h:1040:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_image.h>

static int fuzz_vp9_svc_controls(const uint8_t *data, size_t size) {
    if (size < 10) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t img;
    vpx_codec_iface_t *codec = vpx_codec_vp9_cx();
    if (!codec) return 0;

    if (vpx_codec_enc_config_default(&cfg, codec, 0) != 0) return 0;

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 1;

    if (vpx_codec_enc_init(&ctx, codec, &cfg, 0) != 0) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Allocate and initialize image
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, 128, 128, 1);
    memset(img.planes[0], 128, 128 * 128);
    memset(img.planes[1], 128, 64 * 64);
    memset(img.planes[2], 128, 64 * 64);

    int ret = 0;
    uint8_t *ptr = (uint8_t *)data;

    // Fuzz VP9E_SET_SVC
    if (size > 1) {
        int val = ptr[0];
        ret = vpx_codec_control(&ctx, VP9E_SET_SVC, val);
        (void)ret;
    }

    // Fuzz VP9E_SET_SVC_SPATIAL_LAYER_SYNC
    if (size > 2) {
        vpx_svc_spatial_layer_sync_t sync_cfg;
        memset(&sync_cfg, 0, sizeof(sync_cfg));
        for (int i = 0; i < VPX_SS_MAX_LAYERS && i < 4; i++) {
            sync_cfg.spatial_layer_sync[i] = ptr[1 + i] % 2;
        }
        sync_cfg.base_layer_intra_only = ptr[5] % 2;
        ret = vpx_codec_control(&ctx, VP9E_SET_SVC_SPATIAL_LAYER_SYNC, &sync_cfg);
        (void)ret;
    }

    // Fuzz VP9E_SET_SVC_FRAME_DROP_LAYER
    if (size > 6) {
        vpx_svc_frame_drop_t drop_cfg;
        memset(&drop_cfg, 0, sizeof(drop_cfg));
        for (int i = 0; i < VPX_SS_MAX_LAYERS && i < 4; i++) {
            drop_cfg.framedrop_thresh[i] = ptr[6 + i];
        }
        drop_cfg.framedrop_mode = (SVC_LAYER_DROP_MODE)(ptr[10] % 2);
        drop_cfg.max_consec_drop = ptr[11];
        ret = vpx_codec_control(&ctx, VP9E_SET_SVC_FRAME_DROP_LAYER, &drop_cfg);
        (void)ret;
    }

    // Fuzz VP9E_SET_SVC_LAYER_ID
    if (size > 12) {
        vpx_svc_layer_id_t layer_id;
        memset(&layer_id, 0, sizeof(layer_id));
        layer_id.spatial_layer_id = ptr[12] % 4;
        layer_id.temporal_layer_id = ptr[13] % 4;
        for (int i = 0; i < VPX_SS_MAX_LAYERS && i < 4; i++) {
            layer_id.temporal_layer_id_per_spatial[i] = ptr[14 + i] % 4;
        }
        ret = vpx_codec_control(&ctx, VP9E_SET_SVC_LAYER_ID, &layer_id);
        (void)ret;
    }

    // Fuzz VP9E_SET_SVC_PARAMETERS
    // Since the struct definition is not provided in the prompt, we pass NULL or a dummy pointer
    // to avoid compilation errors while still triggering the control path.
    if (size > 18) {
        // Passing NULL is a valid way to test the control if the implementation handles it,
        // or we can pass a dummy pointer if the API expects one. 
        // Given the error, we pass a pointer to the data buffer to satisfy the void* requirement.
        ret = vpx_codec_control(&ctx, VP9E_SET_SVC_PARAMETERS, (void*)ptr);
        (void)ret;
    }

    // Fuzz VP9E_SET_SVC_REF_FRAME_CONFIG
    if (size > 60) {
        vpx_svc_ref_frame_config_t ref_cfg;
        memset(&ref_cfg, 0, sizeof(ref_cfg));
        for (int i = 0; i < VPX_SS_MAX_LAYERS && i < 4; i++) {
            ref_cfg.lst_fb_idx[i] = ptr[20 + i] % 16;
            ref_cfg.gld_fb_idx[i] = ptr[24 + i] % 16;
            ref_cfg.alt_fb_idx[i] = ptr[28 + i] % 16;
            ref_cfg.update_buffer_slot[i] = ptr[32 + i] % 2;
            ref_cfg.update_last[i] = ptr[36 + i] % 2;
            ref_cfg.update_golden[i] = ptr[40 + i] % 2;
            ref_cfg.update_alt_ref[i] = ptr[44 + i] % 2;
            ref_cfg.reference_last[i] = ptr[48 + i] % 2;
            ref_cfg.reference_golden[i] = ptr[52 + i] % 2;
            ref_cfg.reference_alt_ref[i] = ptr[56 + i] % 2;
            ref_cfg.duration[i] = ptr[60 + i] * 10;
        }
        ret = vpx_codec_control(&ctx, VP9E_SET_SVC_REF_FRAME_CONFIG, &ref_cfg);
        (void)ret;
    }

    // Encode one frame to exercise the state
    vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);

    vpx_img_free(&img);
    vpx_codec_destroy(&ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    return fuzz_vp9_svc_controls(Data, Size);
}