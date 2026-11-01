// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// VP8E_SET_TEMPORAL_LAYER_ID__value at vp8cx.h:1018:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_image.h>

static int fuzz_encode(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img = NULL;
    int ret = 0;

    // Get the VP8 encoder interface
    const vpx_codec_iface_t *codec_iface = vpx_codec_vp8_cx();
    if (!codec_iface) {
        return 0;
    }

    if (vpx_codec_enc_config_default(codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters based on input data
    if (size > 0) {
        cfg.g_w = (data[0] % 1024) + 16;
        cfg.g_h = (data[1] % 1024) + 16;
        cfg.g_timebase.num = (data[2] % 100) + 1;
        cfg.g_timebase.den = (data[3] % 100) + 1;
        cfg.rc_target_bitrate = (data[4] % 10000) + 100;
    }

    if (vpx_codec_enc_init(&ctx, codec_iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Allocate image
    img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (!img) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fill image planes with fuzz data
    size_t plane_size = img->planes[0] ? img->stride[0] * img->h : 0;
    size_t copy_size = (size > plane_size) ? plane_size : size;
    if (img->planes[0]) {
        memcpy(img->planes[0], data, copy_size);
    }
    
    // Fill U and V planes with derived data if they exist
    if (img->planes[1]) {
        for (int i = 0; i < img->stride[1] * (img->h / 2); i++) {
            img->planes[1][i] = (data[i % size] >> 4) & 0x0F;
        }
    }
    if (img->planes[2]) {
        for (int i = 0; i < img->stride[2] * (img->h / 2); i++) {
            img->planes[2][i] = (data[i % size] << 4) & 0xF0;
        }
    }

    // Invoke VP8E_SET_MAX_INTRA_BITRATE_PCT__value
    int intra_pct = (size > 5) ? (data[5] % 101) : 50;
    vpx_codec_control(&ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, intra_pct);

    // Invoke VP8E_SET_FRAME_FLAGS__value
    int frame_flags = (size > 6) ? (data[6] % 10) : 0;
    vpx_codec_control(&ctx, VP8E_SET_FRAME_FLAGS, frame_flags);

    // Invoke VP8E_SET_TEMPORAL_LAYER_ID__value
    int layer_id = (size > 7) ? (data[7] % 4) : 0;
    vpx_codec_control(&ctx, VP8E_SET_TEMPORAL_LAYER_ID, layer_id);

    // Encode frame
    vpx_codec_pts_t pts = 0;
    unsigned long duration = (size > 8) ? (data[8] % 100) + 1 : 30;
    vpx_enc_frame_flags_t flags = (size > 9) ? (data[9] % 2) : 0;
    vpx_enc_deadline_t deadline = VPX_DL_REALTIME;

    ret = vpx_codec_encode(&ctx, img, pts, duration, flags, deadline);
    if (ret != VPX_CODEC_OK) {
        // Continue to cleanup even on error
    }

    // Get encoded data
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Process frame data if needed
            (void)pkt->data.frame.buf;
            (void)pkt->data.frame.sz;
        }
    }

    // Flush encoder
    vpx_codec_encode(&ctx, NULL, pts, duration, flags, deadline);
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        // Consume remaining packets
    }

    // Cleanup
    vpx_codec_destroy(&ctx);
    vpx_img_free(img);

    return 0;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Basic sanity check
    if (Size < 5) {
        return 0;
    }

    return fuzz_encode(Data, Size);
}