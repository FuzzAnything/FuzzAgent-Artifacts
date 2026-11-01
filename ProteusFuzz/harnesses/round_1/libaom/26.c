// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

/*
 * Helper function to initialize a decoder context.
 * Returns 0 on success, non-zero on failure.
 */
static int init_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t ret;

    if (ctx == NULL) return 1;

    iface = aom_codec_av1_dx();
    if (iface == NULL) return 1;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    ret = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 1;

    /* Feed some data to the decoder to ensure it is in a valid state */
    if (size > 0 && data != NULL) {
        aom_codec_decode(ctx, data, size, NULL);
    }

    return 0;
}

/*
 * Helper function to cleanup decoder context.
 */
static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    int ret;
    int val;
    av1_ext_ref_frame_t ext_ref;
    aom_image_t img;

    if (Size == 0) return 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&ext_ref, 0, sizeof(ext_ref));
    memset(&img, 0, sizeof(img));

    /* Initialize decoder */
    if (init_decoder(&ctx, Data, Size) != 0) {
        return 0;
    }

    /*
     * 1. aom_codec_control_typechecked_AOMD_GET_LAST_QUANTIZER
     *    Expects: int *arg
     */
    ret = aom_codec_control(&ctx, AOMD_GET_LAST_QUANTIZER, &val);
    (void)ret;

    /*
     * 2. aom_codec_control_typechecked_AV1D_SET_EXT_REF_PTR
     *    Expects: av1_ext_ref_frame_t *arg
     *    Prepare a dummy image to attach to the ref frame.
     */
    img.planes[0] = (unsigned char *)Data;
    img.stride[0] = (Size > 0) ? (int)(Size / 10) : 0;
    if (img.stride[0] == 0) img.stride[0] = 1;
    img.w = (img.stride[0] > 0) ? img.stride[0] : 1;
    img.h = 1;
    img.fmt = AOM_IMG_FMT_I420;

    ext_ref.img = &img;
    ext_ref.num = 1;

    ret = aom_codec_control(&ctx, AV1D_SET_EXT_REF_PTR, &ext_ref);
    (void)ret;

    /*
     * 3. aom_codec_control_typechecked_AOMD_GET_FRAME_CORRUPTED
     *    Expects: int *arg
     */
    ret = aom_codec_control(&ctx, AOMD_GET_FRAME_CORRUPTED, &val);
    (void)ret;

    /*
     * 4. aom_codec_control_typechecked_AOMD_GET_LAST_REF_USED
     *    Expects: int *arg (array of ref frames)
     */
    int ref_used[8] = {0};
    ret = aom_codec_control(&ctx, AOMD_GET_LAST_REF_USED, ref_used);
    (void)ret;

    /*
     * 5. aom_codec_control_typechecked_AOMD_GET_LAST_REF_UPDATES
     *    Expects: int *arg
     */
    ret = aom_codec_control(&ctx, AOMD_GET_LAST_REF_UPDATES, &val);
    (void)ret;

    /*
     * 6. aom_codec_control_typechecked_AOMD_GET_ALTREF_PRESENT
     *    Expects: int *arg
     */
    ret = aom_codec_control(&ctx, AOMD_GET_ALTREF_PRESENT, &val);
    (void)ret;

    /* Cleanup */
    cleanup_decoder(&ctx);

    return 0;
}