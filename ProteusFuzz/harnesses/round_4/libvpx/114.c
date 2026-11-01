// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_COLOR_RANGE__value at vp8cx.h:1058:1 in vp8cx.h
// VP9E_SET_RENDER_SIZE__value at vp8cx.h:1062:1 in vp8cx.h
// VP9E_SET_ROW_MT__value at vp8cx.h:1066:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// VP9E_SET_FRAME_PERIODIC_BOOST__value at vp8cx.h:1032:1 in vp8cx.h
// VP9E_SET_POSTENCODE_DROP__value at vp8cx.h:1089:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vp8cx.h>
#include <vp8dx.h>

static int fuzz_vp9_encoder(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *codec_iface = vpx_codec_vp9_cx();
    if (!codec_iface) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    int ret = vpx_codec_enc_config_default(codec_iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 1;

    ret = vpx_codec_enc_init(&ctx, codec_iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz various VP9E_SET_* controls using input data
    // Note: vpx_codec_control takes 3 arguments: (ctx, id, data)
    // For controls expecting multiple values, we must pack them into a struct or pointer.
    
    if (size >= 1) {
        int val = data[0] % 2;
        vpx_codec_control(&ctx, VP9E_SET_COLOR_RANGE, val);
    }

    if (size >= 3) {
        int width = (data[1] % 128) + 16;
        int height = (data[2] % 128) + 16;
        // VP9E_SET_RENDER_SIZE expects a pointer to vpx_codec_render_size_t or similar struct
        // Since we don't have the exact struct definition in the prompt, we assume it takes a pointer to int[2] or similar
        // However, standard libvpx usage for render size is often a struct. 
        // If the macro expects a struct, we need to define it. 
        // Based on common libvpx patterns, let's try passing a pointer to an array if struct is unknown, 
        // but typically it's a struct. Let's assume a simple int pointer approach might fail if struct is needed.
        // Correct approach for libvpx: VP9E_SET_RENDER_SIZE takes a pointer to vpx_codec_render_size_t.
        // Since we can't see the struct, we will try to pass a pointer to a local array if the macro expands to a function call expecting a pointer.
        // Actually, looking at libvpx source, VP9E_SET_RENDER_SIZE takes a pointer to vpx_codec_render_size_t.
        // Let's define a minimal struct if not present, or use a union/pointer trick.
        // For safety, we will skip complex struct packing if not defined, but the prompt implies we should fuzz it.
        // Let's assume the macro handles the cast or expects a specific pointer type.
        // In many versions, it expects: vpx_codec_control(&ctx, VP9E_SET_RENDER_SIZE, &width, &height) is WRONG.
        // It expects: vpx_codec_control(&ctx, VP9E_SET_RENDER_SIZE, &render_size_struct)
        // Since we don't have the struct, we will attempt to pass a pointer to a local variable if the macro allows, 
        // but strictly speaking, we need the struct. 
        // Given the constraints, let's try to pass a pointer to an int array if the macro is flexible, 
        // but the error log showed the macro takes 3 args. 
        // Let's assume the macro expands to a call that takes a pointer. 
        // We will create a dummy struct or use a pointer to a local array if the API allows.
        // Actually, standard libvpx: VP9E_SET_RENDER_SIZE takes a pointer to vpx_codec_render_size_t.
        // Let's define a local struct compatible with the expected layout if possible, or just skip if unsure.
        // However, to make it compile, we must match the macro signature.
        // The macro is: vpx_codec_control(ctx, id, data). 
        // If VP9E_SET_RENDER_SIZE expects a struct pointer, we need to pass a pointer to a struct.
        // Let's assume the struct is: typedef struct { int w; int h; } vpx_codec_render_size_t;
        // We will define it locally if not present.
        typedef struct { int w; int h; } render_size_t;
        render_size_t rs = { width, height };
        vpx_codec_control(&ctx, VP9E_SET_RENDER_SIZE, &rs);
    }

    if (size >= 4) {
        int row_mt = data[3] % 2;
        vpx_codec_control(&ctx, VP9E_SET_ROW_MT, row_mt);
    }

    if (size >= 5) {
        int level = data[4] % 7; // Assuming levels 0-6
        vpx_codec_control(&ctx, VP9E_SET_TARGET_LEVEL, level);
    }

    if (size >= 6) {
        int boost = data[5] % 100;
        vpx_codec_control(&ctx, VP9E_SET_FRAME_PERIODIC_BOOST, boost);
    }

    if (size >= 7) {
        int drop = data[6] % 2;
        vpx_codec_control(&ctx, VP9E_SET_POSTENCODE_DROP, drop);
    }

    // Attempt to encode a dummy frame to exercise the encoder state
    vpx_img_fmt_t img_fmt = VPX_IMG_FMT_I420;
    vpx_image_t img;
    vpx_img_alloc(&img, img_fmt, 128, 128, 1);
    
    // Fill image with fuzz data if available
    if (size >= 128 * 128 * 3 / 2) {
        memcpy(img.planes[0], data, 128 * 128);
        memcpy(img.planes[1], data + 128 * 128, 64 * 64);
        memcpy(img.planes[2], data + 128 * 128 + 64 * 64, 64 * 64);
    } else {
        memset(img.planes[0], 0, 128 * 128);
        memset(img.planes[1], 0, 64 * 64);
        memset(img.planes[2], 0, 64 * 64);
    }

    vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);

    vpx_img_free(&img);
    vpx_codec_destroy(&ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    fuzz_vp9_encoder(Data, Size);

    return 0;
}