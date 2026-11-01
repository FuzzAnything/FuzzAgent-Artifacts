// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>

/*
 * Fuzz driver for libaom API functions.
 * Focuses on aom_codec_get_frame and related control functions.
 */

static int try_codec_control(aom_codec_ctx_t *ctx, int ctrl, void *arg) {
    if (ctx == NULL) return 0;
    return aom_codec_control(ctx, ctrl, arg);
}

int LLVMFuzzerTestOneInput_305(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t err;
    aom_codec_iter_t iter = NULL;
    aom_image_t *img = NULL;

    /* Initialize decoder config with safe defaults */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;
    /* Removed cfg.cpu_used as it is not present in all versions of aom_codec_dec_cfg_t */
    
    /* Initialize decoder context */
    err = aom_codec_dec_init(&ctx, aom_codec_av1_dx(), &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    /* Decode the input data */
    err = aom_codec_decode(&ctx, (const uint8_t *)Data, Size, NULL);
    if (err != AOM_CODEC_OK) {
        /* Even if decode fails, we can still try to iterate */
    }

    /* Iterate over frames */
    while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        /* Access frame data to exercise memory paths */
        if (img->planes[0] != NULL && img->stride[0] > 0) {
            volatile uint8_t *p = img->planes[0];
            for (int i = 0; i < 10 && i < img->h; i++) {
                (void)p[i * img->stride[0]];
            }
        }
        /* libaom manages image lifetime until next decode call, do not free here */
    }

    /* Try control functions if macros are defined */
    /* 1. aom_codec_control_typechecked_AV1D_GET_FRAME_HEADER_INFO */
#ifdef AOM_CODEC_AV1D_GET_FRAME_HEADER_INFO
    try_codec_control(&ctx, AOM_CODEC_AV1D_GET_FRAME_HEADER_INFO, NULL);
#endif

    /* 2. aom_codec_control_typechecked_AV1_COPY_NEW_FRAME_IMAGE */
#ifdef AOM_CODEC_AV1_COPY_NEW_FRAME_IMAGE
    try_codec_control(&ctx, AOM_CODEC_AV1_COPY_NEW_FRAME_IMAGE, NULL);
#endif

    /* 3. aom_codec_control_typechecked_AOMD_GET_SHOW_FRAME_FLAG */
#ifdef AOM_CODEC_AOMD_GET_SHOW_FRAME_FLAG
    try_codec_control(&ctx, AOM_CODEC_AOMD_GET_SHOW_FRAME_FLAG, NULL);
#endif

    /* 4. aom_codec_control_typechecked_AV1_GET_NEW_FRAME_IMAGE */
#ifdef AOM_CODEC_AV1_GET_NEW_FRAME_IMAGE
    try_codec_control(&ctx, AOM_CODEC_AV1_GET_NEW_FRAME_IMAGE, NULL);
#endif

    /* 6. aom_codec_control_typechecked_AV1D_SET_EXT_REF_PTR */
#ifdef AOM_CODEC_AV1D_SET_EXT_REF_PTR
    av1_ext_ref_frame_t ext_ref;
    memset(&ext_ref, 0, sizeof(ext_ref));
    ext_ref.num = 0;
    ext_ref.img = NULL;
    try_codec_control(&ctx, AOM_CODEC_AV1D_SET_EXT_REF_PTR, &ext_ref);
#endif

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return 0;
}