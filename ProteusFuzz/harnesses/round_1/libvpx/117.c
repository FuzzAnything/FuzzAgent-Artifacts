// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// VP9E_SET_FRAME_PARALLEL_DECODING__value at vp8cx.h:1028:1 in vp8cx.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// VP9E_SET_DISABLE_LOOPFILTER__value at vp8cx.h:1095:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// VP9E_SET_SVC_REF_FRAME_CONFIG__value at vp8cx.h:1060:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_image.h>

static vpx_codec_ctx_t *create_encoder(int codec_id) {
    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));

    const vpx_codec_iface_t *iface;
    if (codec_id == 0) {
        iface = vpx_codec_vp8_cx();
    } else {
        iface = vpx_codec_vp9_cx();
    }

    if (!iface) {
        free(ctx);
        return NULL;
    }

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != 0) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_error_resilient = 1;

    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != 0) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    int codec_id = Data[0] % 2;
    vpx_codec_ctx_t *ctx = create_encoder(codec_id);
    if (!ctx) return 0;

    // Prepare dummy frame data
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    vpx_image_t img;
    if (vpx_img_alloc(&img, fmt, 64, 64, 1) != 0) {
        cleanup_encoder(ctx);
        return 0;
    }

    // Fill image with fuzz data
    if (Size > 2) {
        size_t img_size = img.stride[0] * img.h;
        size_t copy_size = (Size - 2) < img_size ? (Size - 2) : img_size;
        // Fixed: Use 'planes' instead of 'plane'
        memcpy(img.planes[0], Data + 1, copy_size);
    }

    // Fuzz VP9E_SET_SVC_REF_FRAME_CONFIG__value (using the struct provided)
    // Note: VPX_SS_MAX_LAYERS is typically defined in vpx_encoder.h or vp9_encoder.h
    #ifndef VPX_SS_MAX_LAYERS
    #define VPX_SS_MAX_LAYERS 4
    #endif

    if (codec_id == 1) {
        struct vpx_svc_ref_frame_config svc_cfg;
        memset(&svc_cfg, 0, sizeof(svc_cfg));
        
        // Initialize with fuzz data where possible
        for (int i = 0; i < VPX_SS_MAX_LAYERS && i < (int)(Size / sizeof(int)); i++) {
            svc_cfg.lst_fb_idx[i] = (int)Data[1 + i * sizeof(int)];
            svc_cfg.gld_fb_idx[i] = (int)Data[1 + i * sizeof(int) + 1];
            svc_cfg.alt_fb_idx[i] = (int)Data[1 + i * sizeof(int) + 2];
            svc_cfg.update_buffer_slot[i] = (int)Data[1 + i * sizeof(int) + 3];
        }

        vpx_codec_control(ctx, VP9E_SET_SVC_REF_FRAME_CONFIG, &svc_cfg);
    }

    // Fuzz VP8E_SET_SCREEN_CONTENT_MODE__value (VP8 only)
    if (codec_id == 0) {
        int screen_mode = Data[1] % 2;
        vpx_codec_control(ctx, VP8E_SET_SCREEN_CONTENT_MODE, screen_mode);
    }

    // Fuzz VP9E_SET_FRAME_PARALLEL_DECODING__value (VP9 only)
    if (codec_id == 1) {
        int parallel_dec = (Data[1] >> 1) % 2;
        vpx_codec_control(ctx, VP9E_SET_FRAME_PARALLEL_DECODING, parallel_dec);
    }

    // Fuzz VP9E_SET_NOISE_SENSITIVITY__value (VP9 only)
    if (codec_id == 1) {
        int noise_sens = Data[1] % 100;
        vpx_codec_control(ctx, VP9E_SET_NOISE_SENSITIVITY, noise_sens);
    }

    // Fuzz VP9E_SET_DISABLE_LOOPFILTER__value (VP9 only)
    if (codec_id == 1) {
        int disable_loopfilter = (Data[1] >> 2) % 2;
        vpx_codec_control(ctx, VP9E_SET_DISABLE_LOOPFILTER, disable_loopfilter);
    }

    // Attempt to encode a frame to exercise the state
    vpx_codec_encode(ctx, &img, 0, 1, 0, 0);

    // Cleanup
    vpx_img_free(&img);
    cleanup_encoder(ctx);

    return 0;
}