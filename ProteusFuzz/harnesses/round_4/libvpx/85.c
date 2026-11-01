// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_DISABLE_LOOPFILTER__value at vp8cx.h:1095:1 in vp8cx.h
// VP9E_SET_FRAME_PARALLEL_DECODING__value at vp8cx.h:1028:1 in vp8cx.h
// VP9E_SET_NOISE_SENSITIVITY__value at vp8cx.h:1034:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_SHARPNESS__value at vp8cx.h:990:1 in vp8cx.h
// VP8E_SET_ARNR_STRENGTH__value at vp8cx.h:1002:1 in vp8cx.h
// VP8E_SET_SCREEN_CONTENT_MODE__value at vp8cx.h:1020:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int fuzz_vp8_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface;
    vpx_codec_enc_cfg_t ecfg;
    vpx_image_t img;
    int ret;
    uint8_t *img_buffer = NULL;
    size_t required_size;

    iface = vpx_codec_vp8_cx();
    if (!iface) return 0;

    ret = vpx_codec_enc_config_default(iface, &ecfg, 0);
    if (ret != VPX_CODEC_OK) return 0;

    ecfg.g_w = 16;
    ecfg.g_h = 16;
    ecfg.g_timebase.num = 1;
    ecfg.g_timebase.den = 30;
    ecfg.rc_target_bitrate = 1000;

    ret = vpx_codec_enc_init(&ctx, iface, &ecfg, 0);
    if (ret != VPX_CODEC_OK) return 0;

    // Calculate required buffer size for I420 16x16
    // Y plane: 16*16, U plane: 8*8, V plane: 8*8
    required_size = (16 * 16) + (8 * 8) + (8 * 8);
    
    if (size < required_size) {
        // Allocate and initialize a valid image buffer if input is too small
        img_buffer = (uint8_t *)malloc(required_size);
        if (!img_buffer) {
            vpx_codec_destroy(&ctx);
            return 0;
        }
        memset(img_buffer, 128, required_size); // Initialize to gray
        
        vpx_img_wrap(&img, VPX_IMG_FMT_I420, 16, 16, 1, img_buffer);
    } else {
        // Use input data, but ensure we have enough for the planes
        // We copy the first 'required_size' bytes to a safe buffer to avoid 
        // passing raw fuzzer data that might be misaligned or insufficient for stride
        img_buffer = (uint8_t *)malloc(required_size);
        if (!img_buffer) {
            vpx_codec_destroy(&ctx);
            return 0;
        }
        memcpy(img_buffer, data, required_size);
        
        vpx_img_wrap(&img, VPX_IMG_FMT_I420, 16, 16, 1, img_buffer);
    }

    ret = vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);
    if (ret != VPX_CODEC_OK) {
        free(img_buffer);
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fuzz VP8E_SET_SHARPNESS__value
    int sharpness_val = 0;
    if (size > 0) sharpness_val = data[0] % 7; 
    ret = vpx_codec_control(&ctx, VP8E_SET_SHARPNESS, sharpness_val);
    (void)ret;

    // Fuzz VP8E_SET_ARNR_STRENGTH__value
    int arnr_strength = 0;
    if (size > 1) arnr_strength = data[1] % 5; 
    ret = vpx_codec_control(&ctx, VP8E_SET_ARNR_STRENGTH, arnr_strength);
    (void)ret;

    // Fuzz VP8E_SET_SCREEN_CONTENT_MODE__value
    int screen_mode = 0;
    if (size > 2) screen_mode = data[2] % 2; 
    ret = vpx_codec_control(&ctx, VP8E_SET_SCREEN_CONTENT_MODE, screen_mode);
    (void)ret;

    free(img_buffer);
    vpx_codec_destroy(&ctx);
    return 0;
}

static int fuzz_vp9_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface;
    vpx_codec_enc_cfg_t ecfg;
    vpx_image_t img;
    int ret;
    uint8_t *img_buffer = NULL;
    size_t required_size;

    iface = vpx_codec_vp9_cx();
    if (!iface) return 0;

    ret = vpx_codec_enc_config_default(iface, &ecfg, 0);
    if (ret != VPX_CODEC_OK) return 0;

    ecfg.g_w = 16;
    ecfg.g_h = 16;
    ecfg.g_timebase.num = 1;
    ecfg.g_timebase.den = 30;
    ecfg.rc_target_bitrate = 1000;

    ret = vpx_codec_enc_init(&ctx, iface, &ecfg, 0);
    if (ret != VPX_CODEC_OK) return 0;

    required_size = (16 * 16) + (8 * 8) + (8 * 8);
    
    if (size < required_size) {
        img_buffer = (uint8_t *)malloc(required_size);
        if (!img_buffer) {
            vpx_codec_destroy(&ctx);
            return 0;
        }
        memset(img_buffer, 128, required_size);
        
        vpx_img_wrap(&img, VPX_IMG_FMT_I420, 16, 16, 1, img_buffer);
    } else {
        img_buffer = (uint8_t *)malloc(required_size);
        if (!img_buffer) {
            vpx_codec_destroy(&ctx);
            return 0;
        }
        memcpy(img_buffer, data, required_size);
        
        vpx_img_wrap(&img, VPX_IMG_FMT_I420, 16, 16, 1, img_buffer);
    }

    ret = vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);
    if (ret != VPX_CODEC_OK) {
        free(img_buffer);
        vpx_codec_destroy(&ctx);
        return 0;
    }

    // Fuzz VP9E_SET_DISABLE_LOOPFILTER__value
    int loopfilter_val = 0;
    if (size > 0) loopfilter_val = data[0] % 2;
    ret = vpx_codec_control(&ctx, VP9E_SET_DISABLE_LOOPFILTER, loopfilter_val);
    (void)ret;

    // Fuzz VP9E_SET_FRAME_PARALLEL_DECODING__value
    int parallel_val = 0;
    if (size > 1) parallel_val = data[1] % 2;
    ret = vpx_codec_control(&ctx, VP9E_SET_FRAME_PARALLEL_DECODING, parallel_val);
    (void)ret;

    // Fuzz VP9E_SET_NOISE_SENSITIVITY__value
    int noise_val = 0;
    if (size > 2) noise_val = data[2] % 256;
    ret = vpx_codec_control(&ctx, VP9E_SET_NOISE_SENSITIVITY, noise_val);
    (void)ret;

    free(img_buffer);
    vpx_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    if (Data[0] % 2 == 0) {
        fuzz_vp8_encoder(Data, Size);
    } else {
        fuzz_vp9_encoder(Data, Size);
    }

    return 0;
}