// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_peek_stream_info at vpx_decoder.c:65:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

/* Helper to get the VP8 or VP9 decoder interface based on input data */
static vpx_codec_iface_t *get_decoder_iface(const uint8_t *data, size_t size) {
    (void)data;
    (void)size;
    
    /* Try VP8 first */
    vpx_codec_iface_t *iface_vp8 = vpx_codec_vp8_dx();
    if (iface_vp8) return iface_vp8;
    
    /* Try VP9 */
    vpx_codec_iface_t *iface_vp9 = vpx_codec_vp9_dx();
    if (iface_vp9) return iface_vp9;
    
    return NULL;
}

/* Callback for decoded frames (optional, but good for completeness) */
static vpx_image_t *put_frame_cb(void *ctx, const vpx_image_t *img) {
    (void)ctx;
    (void)img;
    return NULL;
}

/* Callback for decoded slices (optional) */
static vpx_image_t *put_slice_cb(void *ctx, const vpx_image_t *img) {
    (void)ctx;
    (void)img;
    return NULL;
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_iter_t iter = NULL;
    vpx_codec_stream_info_t si;
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_iface_t *iface = NULL;
    vpx_image_t *img = NULL;
    vpx_codec_err_t res;

    /* Initialize config */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;

    /* Try to get a valid interface */
    iface = get_decoder_iface(Data, Size);
    if (!iface) {
        return 0;
    }

    /* Initialize the decoder context */
    memset(&ctx, 0, sizeof(ctx));
    res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        /* If init fails, we can still test error handling functions */
        const char *detail = vpx_codec_error_detail(&ctx);
        (void)detail;
        return 0;
    }

    /* Register callbacks if needed (optional for fuzzing, but safe) */
    /* Note: vpx_codec_set_frame_proc and vpx_codec_set_slice_proc are not standard API in all versions.
       We rely on the default behavior or internal callbacks if not set. 
       Since the API might be missing or different, we skip setting them explicitly to avoid compilation errors.
       The decoder will use default callbacks if none are set.
    */

    /* 1. Test vpx_codec_peek_stream_info */
    si.sz = sizeof(si);
    res = vpx_codec_peek_stream_info(iface, Data, Size, &si);
    /* res can be OK or error, we don't fail here, just continue */

    /* 2. Test vpx_codec_decode */
    /* Decode the input data */
    res = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
    
    /* Handle decode result */
    if (res != VPX_CODEC_OK && res != VPX_CODEC_ERROR) {
        /* Some errors are expected in fuzzing */
    }

    /* 3. Test vpx_codec_get_stream_info */
    si.sz = sizeof(si);
    res = vpx_codec_get_stream_info(&ctx, &si);
    (void)res;

    /* 4. Test vpx_codec_get_frame iteration */
    iter = NULL;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        /* Process frame if needed, here we just iterate to avoid leaks */
        /* The image is valid until next decode or free */
        vpx_img_free(img);
    }

    /* 5. Test vpx_codec_error_detail */
    const char *err_detail = vpx_codec_error_detail(&ctx);
    (void)err_detail;

    /* Cleanup: Flush the decoder */
    vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    
    /* Iterate again to free any remaining frames */
    iter = NULL;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        vpx_img_free(img);
    }

    /* Destroy the context */
    vpx_codec_destroy(&ctx);

    return 0;
}