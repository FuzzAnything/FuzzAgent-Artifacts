// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8dx.h>

/* Static helper to initialize a decoder context for fuzzing */
static vpx_codec_ctx_t *init_decoder_context(void) {
    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(vpx_codec_ctx_t));

    /* Initialize the decoder context with VP8 */
    vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 32;
    cfg.h = 32;

    if (vpx_codec_dec_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/* Callback function for put_frame */
static void put_frame_cb(void *user_priv, const vpx_image_t *frame) {
    (void)user_priv;
    (void)frame;
    /* Do nothing, just a placeholder */
}

/* Callback function for put_slice with correct signature */
static void put_slice_cb(void *user_priv, const vpx_image_t *frame,
                         const vpx_image_rect_t *rect, const vpx_image_rect_t *sub_rect) {
    (void)user_priv;
    (void)frame;
    (void)rect;
    (void)sub_rect;
    /* Do nothing, just a placeholder */
}

int LLVMFuzzerTestOneInput_161(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    vpx_codec_ctx_t *ctx = init_decoder_context();
    if (!ctx) return 0;

    /* 1. Test vpx_codec_err_to_string with various error codes derived from input */
    vpx_codec_err_t err_code = (vpx_codec_err_t)(Data[0] & 0xFF);
    const char *err_str = vpx_codec_err_to_string(err_code);
    if (err_str) {
        /* Consume the string to prevent unused variable warnings */
        size_t len = strlen(err_str);
        (void)len;
    }

    /* 2. Test vpx_codec_register_put_frame_cb */
    /* Try with valid callback and user data derived from input */
    /* Ensure we have at least 2 bytes before accessing Data[1] */
    void *user_priv = NULL;
    if (Size > 1) {
        user_priv = (void *)(uintptr_t)(Data[1] & 0xFF);
    }
    
    vpx_codec_err_t ret1 = vpx_codec_register_put_frame_cb(ctx, put_frame_cb, user_priv);
    (void)ret1;

    /* 3. Test vpx_codec_register_put_slice_cb */
    vpx_codec_err_t ret2 = vpx_codec_register_put_slice_cb(ctx, put_slice_cb, user_priv);
    (void)ret2;

    /* 4. Trigger an error to test vpx_codec_error_detail */
    /* We can try to decode invalid data to trigger an error */
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *frame = NULL;
    vpx_codec_err_t decode_ret = vpx_codec_decode(ctx, NULL, 0, &frame, 0);
    (void)decode_ret;

    /* Get error detail after a potential failure */
    const char *detail = vpx_codec_error_detail(ctx);
    if (detail) {
        size_t detail_len = strlen(detail);
        (void)detail_len;
    }

    /* 5. Test with NULL context to ensure robustness (if API allows) */
    /* Note: Passing NULL to these functions usually returns error immediately */
    const char *null_detail = vpx_codec_error_detail(NULL);
    (void)null_detail;

    /* 6. Cleanup */
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }

    return 0;
}