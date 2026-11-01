// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static vpx_codec_ctx_t *g_ctx = NULL;
static vpx_image_t *g_img = NULL;
static uint8_t *g_custom_buf = NULL;
static size_t g_custom_buf_size = 0;

static void cleanup_fuzz_resources(void) {
    if (g_ctx) {
        vpx_codec_destroy(g_ctx);
        g_ctx = NULL;
    }
    if (g_img) {
        vpx_img_free(g_img);
        g_img = NULL;
    }
    if (g_custom_buf) {
        free(g_custom_buf);
        g_custom_buf = NULL;
        g_custom_buf_size = 0;
    }
}

static int init_encoder(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    if (!iface) return 0;

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters safely
    if (size > 0) {
        cfg.g_w = (data[0] % 128) + 16;
        if (cfg.g_w > 1920) cfg.g_w = 1920;
        cfg.g_h = (data[1] % 128) + 16;
        if (cfg.g_h > 1080) cfg.g_h = 1080;
        cfg.g_threads = (data[2] % 8) + 1;
        cfg.rc_target_bitrate = (data[3] % 5000) + 100;
        cfg.rc_min_quantizer = (data[4] % 64);
        cfg.rc_max_quantizer = (data[5] % 63) + cfg.rc_min_quantizer + 1;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = (data[6] % 60) + 1;
        cfg.g_pass = (data[7] % 3); // 0, 1, or 2
    } else {
        cfg.g_w = 160;
        cfg.g_h = 120;
        cfg.g_threads = 1;
        cfg.rc_target_bitrate = 1000;
        cfg.rc_min_quantizer = 10;
        cfg.rc_max_quantizer = 50;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_pass = VPX_RC_ONE_PASS;
    }

    if (vpx_codec_enc_init(&g_ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Allocate image
    g_img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!g_img) {
        vpx_codec_destroy(g_ctx);
        g_ctx = NULL;
        return 0;
    }

    // Allocate custom buffer for vpx_codec_set_cx_data_buf
    g_custom_buf_size = 64 * 1024;
    g_custom_buf = (uint8_t *)malloc(g_custom_buf_size);
    if (!g_custom_buf) {
        vpx_img_free(g_img);
        g_img = NULL;
        vpx_codec_destroy(g_ctx);
        g_ctx = NULL;
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    // Cleanup previous run
    cleanup_fuzz_resources();

    // Initialize encoder if we have enough data or default
    if (!init_encoder(Data, Size)) {
        return 0;
    }

    // Fill image planes with fuzz data
    if (g_img) {
        size_t plane_size = g_img->planes[0][0] * g_img->stride[0];
        size_t total_size = plane_size * 3; // Y, U, V
        if (Size > total_size) Size = total_size;
        memcpy(g_img->planes[0], Data, Size);
        // Fill rest with zeros if needed
        if (Size < total_size) {
            memset(g_img->planes[0] + Size, 0, total_size - Size);
        }
    }

    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;

    // 1. Test vpx_codec_enc_config_set
    vpx_codec_enc_cfg_t new_cfg;
    vpx_codec_enc_config_default(vpx_codec_vp8_dx(), &new_cfg, 0);
    if (Size > 10) {
        new_cfg.g_w = (Data[10] % 128) + 16;
        new_cfg.g_h = (Data[11] % 128) + 16;
        new_cfg.rc_target_bitrate = (Data[12] % 5000) + 100;
    }
    vpx_codec_enc_config_set(g_ctx, &new_cfg);

    // 2. Test vpx_codec_set_cx_data_buf
    if (g_custom_buf) {
        vpx_fixed_buf_t buf;
        buf.buf = g_custom_buf;
        buf.sz = g_custom_buf_size;
        unsigned int pad_before = (Size > 0) ? (Data[0] % 100) : 0;
        unsigned int pad_after = (Size > 1) ? (Data[1] % 100) : 0;
        vpx_codec_set_cx_data_buf(g_ctx, &buf, pad_before, pad_after);
    } else {
        // Test with NULL to restore default
        vpx_codec_set_cx_data_buf(g_ctx, NULL, 0, 0);
    }

    // 3. Test vpx_codec_encode
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    vpx_enc_deadline_t deadline = VPX_DL_REALTIME;

    if (Size > 20) {
        pts = (Data[20] << 8) | Data[21];
        duration = (Data[22] << 8) | Data[23];
        flags = (Data[24] << 8) | Data[25];
        deadline = (Data[26] << 8) | Data[27];
        if (deadline == 0) deadline = VPX_DL_REALTIME;
    }

    // Encode a frame
    vpx_codec_encode(g_ctx, g_img, pts, duration, flags, deadline);

    // Retrieve output packets
    while ((pkt = vpx_codec_get_cx_data(g_ctx, &iter)) != NULL) {
        // Consume packet to avoid leaks or state issues
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Just read the size to ensure validity
            (void)pkt->data.frame.sz;
        }
    }

    // Flush encoder
    vpx_codec_encode(g_ctx, NULL, 0, 0, 0, VPX_DL_REALTIME);
    while ((pkt = vpx_codec_get_cx_data(g_ctx, &iter)) != NULL) {
        (void)pkt;
    }

    // Cleanup
    cleanup_fuzz_resources();

    return 0;
}