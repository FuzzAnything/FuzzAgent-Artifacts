// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>

static vpx_codec_ctx_t g_ctx;
static vpx_codec_enc_cfg_t g_cfg;
static vpx_image_t g_img;
static uint8_t *g_custom_buf = NULL;
static vpx_fixed_buf_t g_custom_buf_struct;

static void cleanup(void) {
    if (g_ctx.iface) {
        vpx_codec_destroy(&g_ctx);
    }
    if (g_custom_buf) {
        free(g_custom_buf);
        g_custom_buf = NULL;
    }
    if (g_img.planes[0]) {
        vpx_img_free(&g_img);
        memset(&g_img, 0, sizeof(g_img));
    }
    memset(&g_ctx, 0, sizeof(g_ctx));
}

static int init_encoder(const uint8_t *data, size_t size) {
    const vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        return 0;
    }

    if (vpx_codec_enc_config_default(iface, &g_cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    if (size > 0) g_cfg.g_w = (data[0] % 1024) + 16;
    if (size > 1) g_cfg.g_h = (data[1] % 1024) + 16;
    if (size > 2) {
        int bd = data[2] % 3;
        if (bd == 0) g_cfg.g_bit_depth = VPX_BITS_8;
        else if (bd == 1) g_cfg.g_bit_depth = VPX_BITS_10;
        else g_cfg.g_bit_depth = VPX_BITS_12;
    }
    if (size > 3) g_cfg.g_input_bit_depth = g_cfg.g_bit_depth;
    if (size > 4) g_cfg.rc_target_bitrate = (data[4] % 100) + 10;
    if (size > 5) g_cfg.g_threads = (data[5] % 8) + 1;
    if (size > 6) g_cfg.rc_end_usage = (data[6] % 4);
    if (size > 7) g_cfg.kf_mode = (data[7] % 3);

    g_cfg.g_timebase.num = 1;
    g_cfg.g_timebase.den = (size > 8) ? ((data[8] % 60) + 1) : 30;

    if (vpx_codec_enc_init(&g_ctx, iface, &g_cfg, 0) != VPX_CODEC_OK) {
        cleanup();
        return 0;
    }

    if (vpx_img_alloc(&g_img, VPX_IMG_FMT_I420, g_cfg.g_w, g_cfg.g_h, 1) < 0) {
        cleanup();
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cleanup();

    if (!init_encoder(Data, Size)) {
        return 0;
    }

    size_t plane_size = g_img.stride[0] * g_img.h;
    if (g_img.planes[0]) {
        memcpy(g_img.planes[0], Data, Size < plane_size ? Size : plane_size);
    }
    if (g_img.planes[1] && g_img.planes[2]) {
        size_t uv_size = g_img.stride[1] * (g_img.h >> g_img.y_chroma_shift);
        size_t copy_size = (Size > plane_size) ? (Size - plane_size) : 0;
        if (copy_size > 0) {
            memcpy(g_img.planes[1], Data + plane_size, copy_size < uv_size ? copy_size : uv_size);
            if (g_img.planes[2]) {
                memcpy(g_img.planes[2], Data + plane_size, copy_size < uv_size ? copy_size : uv_size);
            }
        }
    }

    if (Size > 10) {
        g_cfg.g_threads = Data[10] % 16;
        g_cfg.rc_target_bitrate = (Data[11] % 500) + 1;
        vpx_codec_enc_config_set(&g_ctx, &g_cfg);
    }

    if (Size > 12) {
        size_t buf_size = (Data[12] % 10) * 1024 + 1024;
        if (!g_custom_buf) {
            g_custom_buf = malloc(buf_size);
            if (g_custom_buf) {
                g_custom_buf_struct.buf = g_custom_buf;
                g_custom_buf_struct.sz = buf_size;
                unsigned int pad_before = Data[13] % 100;
                unsigned int pad_after = Data[14] % 100;
                vpx_codec_set_cx_data_buf(&g_ctx, &g_custom_buf_struct, pad_before, pad_after);
            }
        } else {
            vpx_codec_set_cx_data_buf(&g_ctx, NULL, 0, 0);
        }
    }

    vpx_codec_pts_t pts = (Size > 15) ? ((int64_t)Data[15] << 24 | Data[16] << 16 | Data[17] << 8 | Data[18]) : 0;
    unsigned long duration = (Size > 19) ? (Data[19] % 100) + 1 : 1;
    vpx_enc_frame_flags_t flags = (Size > 20) ? (Data[20] % 4) : 0;
    vpx_enc_deadline_t deadline = (Size > 21) ? (Data[21] * 1000) : 0;

    if (vpx_codec_encode(&g_ctx, &g_img, pts, duration, flags, deadline) != VPX_CODEC_OK) {
    }

    vpx_codec_encode(&g_ctx, NULL, pts + 1, duration, 0, 0);

    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&g_ctx, NULL)) != NULL) {
        (void)pkt;
    }

    cleanup();
    return 0;
}