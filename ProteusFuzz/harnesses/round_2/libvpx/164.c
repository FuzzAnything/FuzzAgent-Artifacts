// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8dx.h>
#include <vpx/vp8cx.h>

/* Dummy callback implementations required for registration */
/* Correct signature for vpx_codec_put_frame_cb_fn_t: void (*)(void *user_priv, const vpx_image_t *img) */
static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

/* Correct signature for vpx_codec_put_slice_cb_fn_t: void (*)(void *user_priv, const vpx_image_t *img, const struct vpx_image_rect *rect, const struct vpx_image_rect *slice_rect) */
static void dummy_put_slice_cb(void *user_priv, const vpx_image_t *img, const struct vpx_image_rect *rect, const struct vpx_image_rect *slice_rect) {
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)slice_rect;
}

/* Helper to safely initialize a decoder context if possible */
static vpx_codec_ctx_t *init_decoder_context(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));

    /* Attempt to get decoder interface */
    const vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    if (!iface) {
        iface = vpx_codec_vp9_dx();
    }
    if (!iface) {
        free(ctx);
        return NULL;
    }

    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 64;
    cfg.h = 64;

    if (vpx_codec_dec_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Feed some data to ensure state is active */
    if (data && size > 0) {
        vpx_codec_decode(ctx, data, size, NULL, -1);
    }

    return ctx;
}

/* Helper to cleanup context */
static void cleanup_decoder_context(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_164(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* 1. Prepare environment: Initialize decoder context */
    vpx_codec_ctx_t *ctx = init_decoder_context(Data, Size);

    if (ctx) {
        /* 2. Fuzz vpx_codec_register_put_frame_cb */
        /* Try with valid callback */
        vpx_codec_register_put_frame_cb(ctx, dummy_put_frame_cb, (void *)Data);
        /* Try with NULL callback to trigger error path */
        vpx_codec_register_put_frame_cb(ctx, NULL, (void *)Data);

        /* 3. Fuzz vpx_codec_register_put_slice_cb */
        vpx_codec_register_put_slice_cb(ctx, dummy_put_slice_cb, (void *)Data);
        vpx_codec_register_put_slice_cb(ctx, NULL, (void *)Data);

        /* 4. Fuzz vpx_codec_error_detail */
        /* Force an error to have something to query */
        vpx_codec_decode(ctx, NULL, 0, NULL, -1);
        const char *detail = vpx_codec_error_detail(ctx);
        if (detail) {
            /* Consume the string to prevent unused variable warning and ensure path coverage */
            (void)strlen(detail);
        }

        /* 5. Fuzz vpx_codec_err_to_string */
        /* Test with known error codes */
        const char *str1 = vpx_codec_err_to_string(VPX_CODEC_OK);
        const char *str2 = vpx_codec_err_to_string(VPX_CODEC_ERROR);
        const char *str3 = vpx_codec_err_to_string(VPX_CODEC_MEM_ERROR);
        const char *str4 = vpx_codec_err_to_string(VPX_CODEC_INCAPABLE);
        const char *str5 = vpx_codec_err_to_string(VPX_CODEC_INVALID_PARAM);
        const char *str6 = vpx_codec_err_to_string((vpx_codec_err_t)9999); /* Unknown */

        /* Ensure pointers are valid (non-NULL) to avoid crashes in fuzzer harness if they were NULL */
        if (str1) (void)strlen(str1);
        if (str2) (void)strlen(str2);
        if (str3) (void)strlen(str3);
        if (str4) (void)strlen(str4);
        if (str5) (void)strlen(str5);
        if (str6) (void)strlen(str6);

        /* Cleanup */
        cleanup_decoder_context(ctx);
    } else {
        /* If init failed, we can still fuzz error string functions with dummy error codes */
        const char *str = vpx_codec_err_to_string(VPX_CODEC_ERROR);
        if (str) (void)strlen(str);
    }

    return 0;
}