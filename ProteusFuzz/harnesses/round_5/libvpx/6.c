// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_FRAME_PERIODIC_BOOST__value at vp8cx.h:1032:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
// VP9E_SET_TUNE_CONTENT__value at vp8cx.h:1044:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_TILE_COLUMNS__value at vp8cx.h:1024:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_image.h>

static int fuzz_vp9_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface;
    vpx_codec_enc_cfg_t cfg;
    int ret;

    // Use VP9 encoder interface
    iface = vpx_codec_vp9_cx();
    if (!iface) {
        return 0;
    }

    ret = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_error_resilient = 1;

    ret = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) {
        return 0;
    }

    // Invoke VP9E_SET_FRAME_PERIODIC_BOOST__value
    ret = vpx_codec_control(&ctx, VP9E_SET_FRAME_PERIODIC_BOOST, 10);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Invoke VP9E_SET_NOISE_SENSITIVITY__value
    ret = vpx_codec_control(&ctx, VP9E_SET_NOISE_SENSITIVITY, 5);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Invoke VP8E_SET_STATIC_THRESHOLD__value
    // Note: This is a VP8 specific control. On a VP9 context, it may fail or be ignored.
    ret = vpx_codec_control(&ctx, VP8E_SET_STATIC_THRESHOLD, 100);
    // We do not check for VPX_CODEC_OK here strictly, as it's expected to fail on VP9 context

    // Invoke VP9E_SET_TUNE_CONTENT__value
    // Use VP9E_TUNE_FILM (value 1) or VP9E_TUNE_DEFAULT (value 0) if enum is missing in some headers
    // Standard libvpx defines VP9E_TUNE_FILM. If compilation fails due to missing enum, use 1.
    // To ensure compilation without external enum dependency, we use the integer value directly if needed,
    // but assuming standard headers, we try the enum. If the error persists, we fallback to 1.
    // Based on the error, VP9E_TUNE_FILM is missing. We will use the integer value 1 which corresponds to FILM.
    ret = vpx_codec_control(&ctx, VP9E_SET_TUNE_CONTENT, 1); 
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Invoke VP9E_SET_TILE_COLUMNS__value
    ret = vpx_codec_control(&ctx, VP9E_SET_TILE_COLUMNS, 2);
    if (ret != VPX_CODEC_OK) {
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Perform a dummy encode to exercise the state
    if (size > 0) {
        vpx_image_t img;
        vpx_codec_cx_pkt_t pkt;
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t *pkt_ptr;

        // Allocate image
        vpx_image_t *img_ptr = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 64, 64, 16);
        if (!img_ptr) {
            vpx_codec_destroy(&ctx);
            return 0;
        }
        img = *img_ptr;

        // Copy fuzz data into the image plane if size permits
        size_t plane_size = 64 * 64;
        if (size >= plane_size) {
            memcpy(img.planes[0], data, plane_size);
        } else {
            memcpy(img.planes[0], data, size);
        }
        // Fill rest with 0 to avoid uninitialized memory issues if size is small
        if (size < plane_size) {
            memset(img.planes[0] + size, 0, plane_size - size);
        }
        
        // For simplicity in fuzzing, we leave chroma planes as 0 (black) or copy if data is large enough
        // This is a rough approximation for fuzzing purposes.

        ret = vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);
        if (ret != VPX_CODEC_OK) {
            vpx_img_free(&img);
            vpx_codec_destroy(&ctx);
            return 0;
        }

        while ((pkt_ptr = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
            if (pkt_ptr->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Consume frame data
                (void)pkt_ptr->data.frame.sz;
                (void)pkt_ptr->data.frame.buf;
            }
        }
        
        vpx_img_free(&img);
    }

    vpx_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    fuzz_vp9_encoder(Data, Size);
    
    return 0;
}