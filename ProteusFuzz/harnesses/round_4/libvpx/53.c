// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// VP8E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1072:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// VP8E_SET_ENABLEAUTOALTREF__value at vp8cx.h:986:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static vpx_codec_ctx_t codec_ctx;
static vpx_image_t img;
static vpx_codec_enc_cfg_t cfg;
static vpx_codec_iface_t *iface;
static int codec_initialized = 0;

static void cleanup(void) {
    if (codec_initialized) {
        vpx_codec_destroy(&codec_ctx);
        codec_initialized = 0;
    }
    if (img.planes[0]) {
        vpx_img_free(&img);
        memset(&img, 0, sizeof(img));
    }
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cleanup();

    iface = vpx_codec_vp8_cx();
    if (!iface) {
        return 0;
    }

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    if (vpx_codec_enc_init(&codec_ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }
    codec_initialized = 1;

    if (vpx_img_alloc(&img, VPX_IMG_FMT_I420, 16, 16, 1) != 0) {
        cleanup();
        return 0;
    }

    memset(img.planes[0], 0, 16 * 16);
    memset(img.planes[1], 128, 8 * 8);
    memset(img.planes[2], 128, 8 * 8);

    int ctrl_val = Data[0];
    int ctrl_val2 = (Size > 1) ? Data[1] : 0;
    int ctrl_val3 = (Size > 2) ? Data[2] : 0;
    int ctrl_val4 = (Size > 3) ? Data[3] : 0;
    int ctrl_val5 = (Size > 4) ? Data[4] : 0;
    int ctrl_val6 = (Size > 5) ? Data[5] : 0;

    if (vpx_codec_control(&codec_ctx, VP8E_SET_GF_CBR_BOOST_PCT, ctrl_val) != 0) {
    }
    if (vpx_codec_control(&codec_ctx, VP8E_SET_NOISE_SENSITIVITY, ctrl_val2) != 0) {
    }
    if (vpx_codec_control(&codec_ctx, VP8E_SET_MAX_INTRA_BITRATE_PCT, ctrl_val3) != 0) {
    }
    if (vpx_codec_control(&codec_ctx, VP8E_SET_CPUUSED, ctrl_val4 & 0x07) != 0) {
    }
    if (vpx_codec_control(&codec_ctx, VP8E_SET_FRAME_FLAGS, ctrl_val5) != 0) {
    }
    if (vpx_codec_control(&codec_ctx, VP8E_SET_ENABLEAUTOALTREF, ctrl_val6 & 0x01) != 0) {
    }

    vpx_codec_frame_flags_t flags = 0;
    vpx_codec_encode(&codec_ctx, &img, 0, 1, flags, 0);

    cleanup();
    return 0;
}