// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"

static int fuzz_decode_and_query(const uint8_t *data, size_t size) {
    if (size < 4) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    aom_codec_iter_t iter = NULL;
    aom_codec_cx_pkt_t *pkt;

    // Initialize config
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 64;
    cfg.h = 64;

    // Get decoder interface
    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Decode input data
    ret = aom_codec_decode(&ctx, data, size, NULL);
    // AOM_CODEC_NO_OUTPUT is not a standard error code in aom_codec_err_t enum.
    // Valid states are AOM_CODEC_OK, AOM_CODEC_ERROR, AOM_CODEC_INVALID_PARAM, etc.
    // We proceed to get frames if the call didn't return a hard error.
    if (ret != AOM_CODEC_OK) {
        // If it's not OK, it might be an error or just no output. 
        // We try to get frames anyway as sometimes data is valid but no frame is ready immediately.
        // If it's a real error, the get_frame loop will likely yield nothing or fail gracefully.
    }

    // Query controls if decoding produced frames
    while ((pkt = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // AOMD_GET_ALTREF_PRESENT
            int altref_present;
            aom_codec_control(&ctx, AOMD_GET_ALTREF_PRESENT, &altref_present);

            // AOMD_GET_S_FRAME_INFO
            aom_s_frame_info s_frame_info;
            aom_codec_control(&ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);

            // AOMD_GET_STILL_PICTURE
            aom_still_picture_info still_info;
            aom_codec_control(&ctx, AOMD_GET_STILL_PICTURE, &still_info);

            // AOMD_GET_FWD_KF_PRESENT
            int fwd_kf_present;
            aom_codec_control(&ctx, AOMD_GET_FWD_KF_PRESENT, &fwd_kf_present);

            // AOMD_GET_ORDER_HINT
            int order_hint;
            aom_codec_control(&ctx, AOMD_GET_ORDER_HINT, &order_hint);

            // AOMD_GET_LAST_REF_USED
            int last_ref_used;
            aom_codec_control(&ctx, AOMD_GET_LAST_REF_USED, &last_ref_used);
        }
    }

    aom_codec_destroy(&ctx);
    return 1;
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Try decoding
    fuzz_decode_and_query(Data, Size);

    return 0;
}