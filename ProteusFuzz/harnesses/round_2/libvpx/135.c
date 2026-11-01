// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// VP8_SET_POSTPROC__value at vp8.h:124:1 in vp8.h
// VP9E_SET_FRAME_PARALLEL_DECODING__value at vp8cx.h:1028:1 in vp8cx.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_image.h>

static int fuzz_vp8_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface;
    vpx_codec_err_t err;
    int width = 16, height = 16;
    
    if (size >= 8) {
        width = *(const int32_t *)(data);
        height = *(const int32_t *)(data + 4);
        if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
            width = 16; height = 16;
        }
    }

    iface = vpx_codec_vp8_cx();
    if (!iface) return 0;

    vpx_codec_enc_cfg_t cfg;
    vpx_codec_enc_config_default(iface, &cfg, 0);
    
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;

    err = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    /* Fuzz VP8E_SET_MAX_INTRA_BITRATE_PCT */
    if (size > 0) {
        int val = (int)data[0] % 200; 
        vpx_codec_control(&ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, val);
    }

    /* Fuzz VP8E_SET_NOISE_SENSITIVITY */
    if (size > 1) {
        int val = (int)data[1] % 100;
        vpx_codec_control(&ctx, VP8E_SET_NOISE_SENSITIVITY, val);
    }

    /* Fuzz VP8E_SET_SCREEN_CONTENT_MODE */
    if (size > 2) {
        int val = (int)data[2] % 2; 
        vpx_codec_control(&ctx, VP8E_SET_SCREEN_CONTENT_MODE, val);
    }

    /* Fuzz VP8_SET_POSTPROC */
    /* Requires at least 6 bytes: post_proc_flag, deblocking_level, noise_level */
    if (size > 5) {
        vp8_postproc_cfg_t cfg_pp;
        cfg_pp.post_proc_flag = (int)data[3] & 0xFF;
        cfg_pp.deblocking_level = (int)data[4] % 17; 
        cfg_pp.noise_level = (int)data[5] % 17; 
        vpx_codec_control(&ctx, VP8_SET_POSTPROC, &cfg_pp);
    }

    /* Fuzz VP9E_SET_FRAME_PARALLEL_DECODING */
    if (size > 6) {
        int val = (int)data[6] % 2;
        vpx_codec_control(&ctx, VP9E_SET_FRAME_PARALLEL_DECODING, val);
    }

    /* Fuzz VP9E_SET_NOISE_SENSITIVITY */
    if (size > 7) {
        int val = (int)data[7] % 100;
        vpx_codec_control(&ctx, VP9E_SET_NOISE_SENSITIVITY, val);
    }

    /* Prepare dummy image for encoding */
    vpx_image_t img;
    memset(&img, 0, sizeof(img));
    
    size_t plane_size = width * height;
    unsigned char *buf = malloc(plane_size * 3 / 2);
    if (!buf) {
        vpx_codec_destroy(&ctx);
        return 0;
    }
    memset(buf, 128, plane_size * 3 / 2);

    vpx_img_wrap(&img, VPX_IMG_FMT_I420, width, height, 1, buf);

    vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);
    free(buf);

    vpx_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    fuzz_vp8_encoder(Data, Size);
    return 0;
}