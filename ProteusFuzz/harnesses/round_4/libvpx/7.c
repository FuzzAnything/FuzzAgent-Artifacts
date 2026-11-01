// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_SVC_REF_FRAME_CONFIG__value at vp8cx.h:1060:1 in vp8cx.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_DISABLE_LOOPFILTER__value at vp8cx.h:1095:1 in vp8cx.h
// VP9E_SET_SVC_FRAME_DROP_LAYER__value at vp8cx.h:1078:1 in vp8cx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_image.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

static int fuzz_setup_encoder(vpx_codec_ctx_t *ctx, vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (!ctx || !cfg) return -1;

    memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));

    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return -1;

    if (vpx_codec_enc_config_default(iface, cfg, 0) != VPX_CODEC_OK) {
        return -1;
    }

    if (size > 0) {
        cfg->g_w = (size > 0 ? data[0] : 0) % 256 + 16;
        cfg->g_h = (size > 1 ? data[1] : 0) % 256 + 16;
        if (cfg->g_w % 2 != 0) cfg->g_w++;
        if (cfg->g_h % 2 != 0) cfg->g_h++;
        
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = (size > 2 ? data[2] : 30) % 100 + 1;
        cfg->rc_target_bitrate = (size > 3 ? data[3] : 50) % 1000 + 100;
        cfg->g_threads = (size > 4 ? data[4] : 1) % 4 + 1;
        cfg->g_error_resilient = (size > 5 ? data[5] : 0) % 3;
        cfg->g_lag_in_frames = (size > 6 ? data[6] : 0) % 10;
    } else {
        cfg->g_w = 128;
        cfg->g_h = 128;
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 30;
        cfg->rc_target_bitrate = 500;
    }

    if (vpx_codec_enc_init(ctx, iface, cfg, 0) != VPX_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));

    if (fuzz_setup_encoder(&ctx, &cfg, Data, Size) != 0) {
        return 0;
    }

    vpx_svc_frame_drop_t svc_drop;
    memset(&svc_drop, 0, sizeof(svc_drop));
    if (Size > 11) {
        svc_drop.framedrop_mode = (Data[10] % 2);
        svc_drop.max_consec_drop = (Data[11] % 10);
    }

    vpx_svc_layer_id_t svc_layer;
    memset(&svc_layer, 0, sizeof(svc_layer));
    if (Size > 13) {
        svc_layer.spatial_layer_id = (Data[12] % 4);
        svc_layer.temporal_layer_id = (Data[13] % 4);
    }

    vpx_svc_ref_frame_config_t svc_ref;
    memset(&svc_ref, 0, sizeof(svc_ref));
    if (Size > 15) {
        svc_ref.update_buffer_slot[0] = (Data[14] % 2);
        svc_ref.reference_last[0] = (Data[15] % 2);
    }

    // 1. VP9E_SET_DISABLE_LOOPFILTER__value
    int disable_loopfilter = (Size > 0) ? (Data[0] % 2) : 1;
    if (vpx_codec_control(&ctx, VP9E_SET_DISABLE_LOOPFILTER, disable_loopfilter) != VPX_CODEC_OK) {
        // Ignore control errors
    }

    // 2. VP9E_SET_SVC_FRAME_DROP_LAYER__value
    if (vpx_codec_control(&ctx, VP9E_SET_SVC_FRAME_DROP_LAYER, &svc_drop) != VPX_CODEC_OK) {
        // Ignore control errors
    }

    // 3. VP9E_SET_SVC_LAYER_ID__value
    if (vpx_codec_control(&ctx, VP9E_SET_SVC_LAYER_ID, &svc_layer) != VPX_CODEC_OK) {
        // Ignore control errors
    }

    // 4. VP9E_SET_SVC_REF_FRAME_CONFIG__value
    if (vpx_codec_control(&ctx, VP9E_SET_SVC_REF_FRAME_CONFIG, &svc_ref) != VPX_CODEC_OK) {
        // Ignore control errors
    }

    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fill image with fuzz data safely
    for (int i = 0; i < 4; i++) {
        if (img->planes[i]) {
            size_t plane_size = (size_t)img->stride[i] * img->h;
            size_t copy_size = (Size < plane_size) ? Size : plane_size;
            memcpy(img->planes[i], Data, copy_size);
            // Only memset if there is remaining space to avoid overflow
            if (copy_size < plane_size) {
                size_t remaining = plane_size - copy_size;
                // Ensure remaining is not huge to prevent TLE, though ASan handles overflow
                if (remaining > 0 && remaining < plane_size) {
                    memset(img->planes[i] + copy_size, 0, remaining);
                }
            }
        }
    }

    vpx_codec_encode(&ctx, img, 0, 1, 0, 0);

    // 5. vpx_codec_get_cx_data
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            size_t sz = pkt->data.frame.sz;
            (void)sz;
        }
    }

    // 6. vpx_codec_destroy
    vpx_codec_destroy(&ctx);

    // 7. vpx_img_free
    vpx_img_free(img);

    return 0;
}