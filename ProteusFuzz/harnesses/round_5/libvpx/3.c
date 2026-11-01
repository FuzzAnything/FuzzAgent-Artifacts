// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_TOKEN_PARTITIONS__value at vp8cx.h:994:1 in vp8cx.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// VP8E_SET_TEMPORAL_LAYER_ID__value at vp8cx.h:1018:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vpx_image.h>
#include <vpx/vp8cx.h>

/* Define VP9E_SET_SVC_LAYER_ID if not present in standard headers for this build */
#ifndef VP9E_SET_SVC_LAYER_ID
#define VP9E_SET_SVC_LAYER_ID 26
#endif

static int create_dummy_image(const uint8_t *data, size_t size, vpx_image_t *img) {
    if (size < 16) return 0;
    unsigned int w = 16;
    unsigned int h = 16;
    if (size > 256) {
        w = 32;
        h = 32;
    }
    if (vpx_img_alloc(img, VPX_IMG_FMT_I420, w, h, 1) == NULL) {
        return 0;
    }
    size_t plane_size = w * h;
    size_t u_plane_size = (w / 2) * (h / 2);
    if (size >= plane_size + 2 * u_plane_size) {
        memcpy(img->planes[VPX_PLANE_Y], data, plane_size);
        memcpy(img->planes[VPX_PLANE_U], data + plane_size, u_plane_size);
        memcpy(img->planes[VPX_PLANE_V], data + plane_size + u_plane_size, u_plane_size);
    } else {
        memset(img->planes[VPX_PLANE_Y], 0, plane_size);
        memset(img->planes[VPX_PLANE_U], 0, u_plane_size);
        memset(img->planes[VPX_PLANE_V], 0, u_plane_size);
    }
    return 1;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();

    if (iface == NULL) return 0;

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) return 0;

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 0;

    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) return 0;

    // 1. VP8E_SET_TOKEN_PARTITIONS__value
    int token_partitions = (Size > 0) ? (Data[0] % 4) : 0;
    vpx_codec_control(&ctx, VP8E_SET_TOKEN_PARTITIONS, token_partitions);

    // 2. VP8E_SET_MAX_INTRA_BITRATE_PCT__value
    int max_intra_pct = (Size > 1) ? (Data[1] % 100 + 1) : 50;
    vpx_codec_control(&ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, max_intra_pct);

    // 3. VP9E_SET_SVC_LAYER_ID__value
    vpx_svc_layer_id_t svc_layer_id = {0};
    svc_layer_id.spatial_layer_id = (Size > 2) ? (Data[2] % 2) : 0;
    svc_layer_id.temporal_layer_id = (Size > 3) ? (Data[3] % 4) : 0;
    vpx_codec_control(&ctx, VP9E_SET_SVC_LAYER_ID, &svc_layer_id);

    // 4. VP8E_SET_TEMPORAL_LAYER_ID__value
    int temporal_layer_id = (Size > 4) ? (Data[4] % 4) : 0;
    vpx_codec_control(&ctx, VP8E_SET_TEMPORAL_LAYER_ID, temporal_layer_id);

    vpx_image_t img;
    memset(&img, 0, sizeof(img));

    if (create_dummy_image(Data, Size, &img)) {
        vpx_codec_pts_t pts = 0;
        unsigned long duration = 1;
        vpx_enc_frame_flags_t flags = 0;
        vpx_enc_deadline_t deadline = 0;

        // 5. vpx_codec_encode
        vpx_codec_err_t res = vpx_codec_encode(&ctx, &img, pts, duration, flags, deadline);
        (void)res;

        // 6. vpx_codec_get_cx_data
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                (void)pkt->data.frame.sz;
                (void)pkt->data.frame.pts;
            }
        }
    } else {
        vpx_codec_encode(&ctx, NULL, 0, 0, 0, 0);
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
            (void)pkt;
        }
    }

    // 7. vpx_codec_destroy
    vpx_codec_destroy(&ctx);

    // 8. vpx_img_free
    vpx_img_free(&img);

    return 0;
}