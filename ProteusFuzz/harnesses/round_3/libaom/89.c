// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

/*
 * Helper function to initialize a decoder context.
 * Returns 0 on success, -1 on failure.
 */
static int init_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t ret;

    if (!ctx || !data || size == 0) {
        return -1;
    }

    iface = aom_codec_av1_dx();
    if (!iface) {
        return -1;
    }

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    ret = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }

    /* Attempt to decode the input data to trigger internal state changes */
    /* aom_codec_decode handles the data packet internally, no need for a manual packet struct */
    ret = aom_codec_decode(ctx, data, size, NULL);
    if (ret != AOM_CODEC_OK) {
        /* Decoding might fail on fuzzed data, which is expected.
         * We still want to try the control functions if the context is valid. */
    }

    return 0;
}

/*
 * Helper function to cleanup the decoder context.
 */
static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (init_decoder(&ctx, Data, Size) != 0) {
        /* If initialization fails, we cannot safely test the control functions. */
        return 0;
    }

    /*
     * The target functions are typically macros wrapping aom_codec_control.
     * We invoke them using the standard aom_codec_control API with the corresponding enums.
     */

    /* 1. AOMD_GET_ALTREF_PRESENT */
    int altref_present = 0;
    aom_codec_err_t ret = aom_codec_control(&ctx, AOMD_GET_ALTREF_PRESENT, &altref_present);
    (void)ret;

    /* 2. AOMD_GET_S_FRAME_INFO */
    aom_s_frame_info s_frame_info;
    memset(&s_frame_info, 0, sizeof(s_frame_info));
    ret = aom_codec_control(&ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);
    (void)ret;

    /* 3. AOMD_GET_STILL_PICTURE */
    aom_still_picture_info still_pic_info;
    memset(&still_pic_info, 0, sizeof(still_pic_info));
    ret = aom_codec_control(&ctx, AOMD_GET_STILL_PICTURE, &still_pic_info);
    (void)ret;

    /* 4. AOMD_GET_FWD_KF_PRESENT */
    int fwd_kf_present = 0;
    ret = aom_codec_control(&ctx, AOMD_GET_FWD_KF_PRESENT, &fwd_kf_present);
    (void)ret;

    /* 5. AOMD_GET_ORDER_HINT */
    int order_hint = 0;
    ret = aom_codec_control(&ctx, AOMD_GET_ORDER_HINT, &order_hint);
    (void)ret;

    /* 6. AOMD_GET_LAST_REF_USED */
    int last_ref_used = 0;
    ret = aom_codec_control(&ctx, AOMD_GET_LAST_REF_USED, &last_ref_used);
    (void)ret;

    /* Cleanup */
    cleanup_decoder(&ctx);

    return 0;
}