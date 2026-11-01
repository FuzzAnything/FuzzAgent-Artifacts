// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_TOKEN_PARTITIONS__value at vp8cx.h:994:1 in vp8cx.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP9E_SET_SVC_LAYER_ID__value at vp8cx.h:1042:1 in vp8cx.h
// VP8E_SET_TEMPORAL_LAYER_ID__value at vp8cx.h:1018:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_image.h>

static int fuzz_encode(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t *img = NULL;
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    vpx_codec_err_t res;
    const vpx_codec_iface_t *iface;
    int ret = 0;

    if (Size < 2) return 0;

    // Get the VP8 encoder interface
    iface = vpx_codec_vp8_cx();
    if (!iface) {
        return 0;
    }

    // Initialize configuration
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to configure encoder parameters safely
    cfg.g_w = (Data[0] % 256) + 16; // Minimum 16
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    // Initialize encoder
    res = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Allocate image
    img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fill image with fuzz data
    size_t plane_size = img->planes[0][0] * img->stride[0];
    if (Size > plane_size) Size = plane_size;
    memcpy(img->planes[0], Data, Size);
    memset(img->planes[1], 0, img->stride[1] * (img->h / 2));
    memset(img->planes[2], 0, img->stride[2] * (img->h / 2));

    // Invoke target API functions in order
    // 1. VP8E_SET_TOKEN_PARTITIONS__value
    vpx_codec_control(&ctx, VP8E_SET_TOKEN_PARTITIONS, 0);

    // 2. VP8E_SET_MAX_INTRA_BITRATE_PCT__value
    vpx_codec_control(&ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, 100);

    // 3. VP9E_SET_SVC_LAYER_ID__value
    vpx_svc_layer_id_t svc_id = {0, 0, {0}};
    vpx_codec_control(&ctx, VP9E_SET_SVC_LAYER_ID, &svc_id);

    // 4. VP8E_SET_TEMPORAL_LAYER_ID__value
    vpx_codec_control(&ctx, VP8E_SET_TEMPORAL_LAYER_ID, 0);

    // 5. vpx_codec_encode
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    vpx_enc_deadline_t deadline = VPX_DL_REALTIME;

    res = vpx_codec_encode(&ctx, img, pts, duration, flags, deadline);
    if (res != VPX_CODEC_OK) {
        // Continue to cleanup even if encode fails
    }

    // Flush the encoder
    res = vpx_codec_encode(&ctx, NULL, pts + duration, duration, flags, deadline);

    // 6. vpx_codec_get_cx_data
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        (void)pkt;
    }

    // 7. vpx_codec_destroy
    vpx_codec_destroy(&ctx);

    // 8. vpx_img_free
    vpx_img_free(img);

    return ret;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    fuzz_encode(Data, Size);
    return 0;
}