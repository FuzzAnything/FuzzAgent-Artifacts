// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int setup_encoder(vpx_codec_ctx_t *ctx, vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    vpx_codec_iface_t *iface;
    vpx_codec_err_t err;

    if (size < 4) return 0;

    iface = vpx_codec_vp8_dx();
    if (!iface) return 0;

    err = vpx_codec_enc_config_default(iface, cfg, 0);
    if (err != VPX_CODEC_OK) return 0;

    // Fuzz configuration parameters
    cfg->g_w = (data[0] % 512) + 16;
    cfg->g_h = (data[1] % 512) + 16;
    cfg->g_timebase.num = (data[2] % 100) + 1;
    cfg->g_timebase.den = (data[3] % 1000) + 1;
    cfg->rc_target_bitrate = (data[4] % 1000) + 100;
    cfg->rc_min_quantizer = (data[5] % 63);
    cfg->rc_max_quantizer = (data[6] % 63) + 1;
    cfg->kf_min_dist = (data[7] % 256);
    cfg->kf_max_dist = (data[8] % 256) + cfg->kf_min_dist;

    err = vpx_codec_enc_init(ctx, iface, cfg, 0);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(ctx);
        return 0;
    }

    return 1;
}

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;

    // Flush and consume all remaining packets
    vpx_codec_encode(ctx, NULL, 0, 0, 0, VPX_DL_REALTIME);
    while ((pkt = vpx_codec_get_cx_data(ctx, &iter)) != NULL) {
        // Consume packet to avoid memory leaks in internal buffers
    }

    vpx_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_163(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t img;
    vpx_fixed_buf_t custom_buf;
    unsigned char *buf_mem;
    vpx_codec_err_t err;
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    unsigned int pad_before, pad_after;

    if (Size < 10) return 0;

    // Initialize context
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));
    memset(&img, 0, sizeof(img));

    // Setup encoder
    if (!setup_encoder(&ctx, &cfg, Data, Size)) return 0;

    // Prepare custom buffer for vpx_codec_set_cx_data_buf
    buf_mem = malloc(Size > 4096 ? 4096 : Size);
    if (!buf_mem) {
        cleanup_encoder(&ctx);
        return 0;
    }
    custom_buf.buf = buf_mem;
    custom_buf.sz = Size > 4096 ? 4096 : Size;

    // Fuzz vpx_codec_set_cx_data_buf
    pad_before = (Data[10] % 100);
    pad_after = (Data[11] % 100);
    err = vpx_codec_set_cx_data_buf(&ctx, &custom_buf, pad_before, pad_after);
    (void)err;

    // Fuzz vpx_codec_enc_config_set with modified config
    cfg.g_threads = (Data[12] % 8) + 1;
    cfg.kf_mode = (Data[13] % 3); // 0, 1, or 2
    err = vpx_codec_enc_config_set(&ctx, &cfg);
    (void)err;

    // Create a dummy image for encoding
    img.fmt = VPX_IMG_FMT_I420;
    img.w = cfg.g_w;
    img.h = cfg.g_h;
    img.d_w = cfg.g_w;
    img.d_h = cfg.g_h;
    img.bit_depth = 8;
    img.stride[0] = img.w;
    img.stride[1] = img.w / 2;
    img.stride[2] = img.w / 2;
    
    // Allocate image planes
    size_t y_size = img.w * img.h;
    size_t uv_size = (img.w / 2) * (img.h / 2);
    img.planes[0] = malloc(y_size);
    img.planes[1] = malloc(uv_size);
    img.planes[2] = malloc(uv_size);
    img.planes[3] = NULL;

    if (!img.planes[0] || !img.planes[1] || !img.planes[2]) {
        free(img.planes[0]);
        free(img.planes[1]);
        free(img.planes[2]);
        free(buf_mem);
        cleanup_encoder(&ctx);
        return 0;
    }

    // Fill image with fuzz data
    memset(img.planes[0], Data[14] % 256, y_size);
    memset(img.planes[1], Data[15] % 256, uv_size);
    memset(img.planes[2], Data[16] % 256, uv_size);

    // Fuzz vpx_codec_encode
    vpx_codec_pts_t pts = (vpx_codec_pts_t)(Data[17] % 1000);
    unsigned long duration = (unsigned long)(Data[18] % 100) + 1;
    vpx_enc_frame_flags_t flags = (vpx_enc_frame_flags_t)(Data[19] & 0xFF);
    vpx_enc_deadline_t deadline = (vpx_enc_deadline_t)(Data[20] % 1000000);

    err = vpx_codec_encode(&ctx, &img, pts, duration, flags, deadline);
    (void)err;

    // Fuzz vpx_codec_get_cx_data
    iter = NULL;
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Just consume the packet
            (void)pkt->data.frame.buf;
            (void)pkt->data.frame.sz;
        }
    }

    // Flush encoder
    err = vpx_codec_encode(&ctx, NULL, 0, 0, 0, VPX_DL_REALTIME);
    (void)err;

    // Consume remaining packets
    iter = NULL;
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        // Consume
    }

    // Cleanup
    free(img.planes[0]);
    free(img.planes[1]);
    free(img.planes[2]);
    free(buf_mem);
    cleanup_encoder(&ctx);

    return 0;
}