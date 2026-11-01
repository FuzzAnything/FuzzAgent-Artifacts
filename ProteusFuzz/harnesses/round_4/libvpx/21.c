// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// VP8E_SET_TEMPORAL_LAYER_ID__value at vp8cx.h:1018:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_codec.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_image.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static vpx_codec_ctx_t *init_encoder(vpx_codec_enc_cfg_t *cfg) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface;
    vpx_codec_err_t res;

    iface = vpx_codec_vp8_cx();
    if (!iface) return NULL;

    memset(&ctx, 0, sizeof(ctx));

    res = vpx_codec_enc_config_default(iface, cfg, 0);
    if (res != VPX_CODEC_OK) {
        return NULL;
    }

    res = vpx_codec_enc_init(&ctx, iface, cfg, 0);
    if (res != VPX_CODEC_OK) {
        return NULL;
    }

    // Allocate memory for the context to return, copying the initialized stack data
    vpx_codec_ctx_t *ctx_ptr = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx_ptr) return NULL;
    memcpy(ctx_ptr, &ctx, sizeof(vpx_codec_ctx_t));

    return ctx_ptr;
}

static vpx_image_t *create_image(vpx_img_fmt_t fmt, unsigned int w, unsigned int h) {
    return vpx_img_alloc(NULL, fmt, w, h, 1);
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    vpx_codec_ctx_t *ctx = NULL;
    vpx_image_t *img = NULL;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    vpx_enc_deadline_t deadline = VPX_DL_REALTIME;

    // Initialize config
    vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, 0);
    
    // Ensure dimensions are valid and within reasonable bounds
    unsigned int w = (Data[0] % 256) + 16;
    unsigned int h = (Data[1] % 256) + 16;
    
    if (w > 1920) w = 1920;
    if (h > 1080) h = 1080;

    cfg.g_w = w;
    cfg.g_h = h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.rc_min_quantizer = 2;
    cfg.rc_max_quantizer = 63;

    ctx = init_encoder(&cfg);
    if (!ctx) return 0;

    // Apply control settings based on input data
    // 1. VP8E_SET_MAX_INTRA_BITRATE_PCT__value
    int max_intra_pct = (Data[2] % 100) + 1;
    vpx_codec_control(ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, max_intra_pct);

    // 2. VP8E_SET_FRAME_FLAGS__value
    int frame_flags_val = Data[3];
    vpx_codec_control(ctx, VP8E_SET_FRAME_FLAGS, frame_flags_val);

    // 3. VP8E_SET_TEMPORAL_LAYER_ID__value
    int layer_id = Data[4] % 4;
    vpx_codec_control(ctx, VP8E_SET_TEMPORAL_LAYER_ID, layer_id);

    // Create an image to encode
    img = create_image(VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h);
    if (!img) {
        vpx_codec_destroy(ctx);
        free(ctx);
        return 0;
    }

    // Encode a frame
    res = vpx_codec_encode(ctx, img, pts, duration, flags, deadline);
    
    // Get output data
    if (res == VPX_CODEC_OK) {
        while ((pkt = vpx_codec_get_cx_data(ctx, &iter)) != NULL) {
            // Consume packet data
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                (void)pkt->data.frame.buf;
            }
        }
    }

    // Flush the encoder
    res = vpx_codec_encode(ctx, NULL, pts + duration, duration, flags, deadline);
    if (res == VPX_CODEC_OK) {
        while ((pkt = vpx_codec_get_cx_data(ctx, &iter)) != NULL) {
            (void)pkt;
        }
    }

    // Cleanup
    vpx_codec_destroy(ctx);
    vpx_img_free(img);
    free(ctx);

    return 0;
}