// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// VP8E_SET_ARNR_STRENGTH__value at vp8cx.h:1002:1 in vp8cx.h
// VP8E_SET_MAX_INTRA_BITRATE_PCT__value at vp8cx.h:1010:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static vpx_codec_ctx_t codec;
static vpx_image_t img;
static vpx_codec_enc_cfg_t cfg;

static vpx_codec_err_t init_codec(vpx_codec_iface_t *iface) {
    vpx_codec_err_t res;
    memset(&cfg, 0, sizeof(cfg));
    res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) return res;
    
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;
    
    res = vpx_codec_enc_init(&codec, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) return res;
    
    // vpx_img_alloc returns an int (0 on success), not vpx_codec_err_t
    if (vpx_img_alloc(&img, VPX_IMG_FMT_I420, 128, 128, 1) != 0) {
        vpx_codec_destroy(&codec);
        return VPX_CODEC_ERROR;
    }
    
    // Initialize image data with a pattern
    memset(img.planes[0], 128, img.stride[0] * img.d_h);
    memset(img.planes[1], 128, img.stride[1] * (img.d_h >> 1));
    memset(img.planes[2], 128, img.stride[2] * (img.d_h >> 1));
    
    return VPX_CODEC_OK;
}

static void cleanup_codec(void) {
    vpx_img_free(&img);
    vpx_codec_destroy(&codec);
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    int mode = Data[0] % 2; // 0 for VP8, 1 for VP9 (VP9 skipped due to header constraints)
    vpx_codec_iface_t *iface = NULL;
    
    if (mode == 0) {
        iface = vpx_codec_vp8_cx();
    } else {
        // VP9 encoder interface requires vp9cx.h which is not included to avoid compile errors.
        // Skipping VP9 mode to ensure compilation success.
        return 0; 
    }
    
    if (!iface) return 0;
    
    if (init_codec(iface) != VPX_CODEC_OK) {
        cleanup_codec();
        return 0;
    }
    
    int8_t val8 = (int8_t)(Data[1] - 128);
    int val32 = 0;
    if (Size >= 6) {
        val32 = Data[2] | (Data[3] << 8) | (Data[4] << 16) | (Data[5] << 24);
    }
    
    // Fuzz VP8E_SET_SCREEN_CONTENT_MODE
    if (mode == 0) {
        vpx_codec_control(&codec, VP8E_SET_SCREEN_CONTENT_MODE, val8);
        vpx_codec_control(&codec, VP8E_SET_ARNR_STRENGTH, val8);
        vpx_codec_control(&codec, VP8E_SET_MAX_INTRA_BITRATE_PCT, val32);
        vpx_codec_control(&codec, VP8E_SET_NOISE_SENSITIVITY, val8);
    }
    
    // Encode a frame to exercise the state
    vpx_codec_encode(&codec, &img, 0, 1, 0, 0);
    
    cleanup_codec();
    return 0;
}