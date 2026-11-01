// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_DISABLE_LOOPFILTER__value at vp8cx.h:1095:1 in vp8cx.h
// VP9E_SET_SVC_FRAME_DROP_LAYER__value at vp8cx.h:1078:1 in vp8cx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// VP9E_SET_SVC_REF_FRAME_CONFIG__value at vp8cx.h:1060:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vpx_image.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8dx.h>

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 50) {
        return 0;
    }

    // Initialize codec context
    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Get encoder interface
    const vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) {
        return 0;
    }

    // Initialize encoder configuration
    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Set some basic config from fuzz data to ensure diversity
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = Data[2] % 100 + 1;
    cfg.g_timebase.den = Data[3] % 100 + 1;

    // Initialize encoder
    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Create a dummy image to potentially use or just to test vpx_img_free later
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 
                                     cfg.g_w, cfg.g_h, 1);
    if (!img) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // 1. VP9E_SET_DISABLE_LOOPFILTER__value
    int disable_loopfilter = Data[4] % 2;
    if (vpx_codec_control(&ctx, VP9E_SET_DISABLE_LOOPFILTER, disable_loopfilter) != VPX_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // 2. VP9E_SET_SVC_FRAME_DROP_LAYER__value
    vpx_svc_frame_drop_t svc_drop;
    memset(&svc_drop, 0, sizeof(svc_drop));
    for (int i = 0; i < VPX_SS_MAX_LAYERS && i < 4; i++) {
        svc_drop.framedrop_thresh[i] = Data[5 + i] % 100;
    }
    svc_drop.framedrop_mode = Data[9] % 2;
    svc_drop.max_consec_drop = Data[10] % 10;

    if (vpx_codec_control(&ctx, VP9E_SET_SVC_FRAME_DROP_LAYER, &svc_drop) != VPX_CODEC_OK) {
        // Ignore error
    }

    // 3. VP9E_SET_SVC_LAYER_ID__value
    vpx_svc_layer_id_t svc_layer_id;
    memset(&svc_layer_id, 0, sizeof(svc_layer_id));
    svc_layer_id.spatial_layer_id = Data[11] % 4;
    svc_layer_id.temporal_layer_id = Data[12] % 8;
    for (int i = 0; i < VPX_SS_MAX_LAYERS && i < 4; i++) {
        svc_layer_id.temporal_layer_id_per_spatial[i] = Data[13 + i] % 8;
    }

    if (vpx_codec_control(&ctx, VP9E_SET_SVC_LAYER_ID, &svc_layer_id) != VPX_CODEC_OK) {
        // Ignore error
    }

    // 4. VP9E_SET_SVC_REF_FRAME_CONFIG__value
    vpx_svc_ref_frame_config_t svc_ref_config;
    memset(&svc_ref_config, 0, sizeof(svc_ref_config));
    for (int i = 0; i < VPX_SS_MAX_LAYERS && i < 4; i++) {
        svc_ref_config.lst_fb_idx[i] = Data[17 + i] % 8;
        svc_ref_config.gld_fb_idx[i] = Data[21 + i] % 8;
        svc_ref_config.alt_fb_idx[i] = Data[25 + i] % 8;
        svc_ref_config.update_buffer_slot[i] = Data[29 + i] % 2;
        svc_ref_config.reference_last[i] = Data[33 + i] % 2;
        svc_ref_config.reference_golden[i] = Data[37 + i] % 2;
        svc_ref_config.reference_alt_ref[i] = Data[41 + i] % 2;
        svc_ref_config.duration[i] = Data[45 + i] * 10;
    }

    if (vpx_codec_control(&ctx, VP9E_SET_SVC_REF_FRAME_CONFIG, &svc_ref_config) != VPX_CODEC_OK) {
        // Ignore error
    }

    // Encode a frame to generate some output data for iteration
    // Use 0 for flags as VPX_EFLAG_NONE is not defined in some versions
    vpx_codec_encode(&ctx, img, 0, 1, 0, 0);

    // 5. vpx_codec_get_cx_data
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            if (pkt->data.frame.buf && pkt->data.frame.sz > 0) {
                // Access frame data safely
            }
        }
    }

    // 6. vpx_codec_destroy
    vpx_codec_destroy(&ctx);

    // 7. vpx_img_free
    vpx_img_free(img);

    return 0;
}