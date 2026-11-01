// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

/*
 * Fuzz driver for libaom decoder control functions.
 * Targets:
 * - AOMD_GET_ALTREF_PRESENT
 * - AOMD_GET_SHOW_EXISTING_FRAME_FLAG
 * - AOMD_GET_SHOW_FRAME_FLAG
 * - AOMD_GET_FRAME_FLAGS
 * - AOMD_GET_LAST_REF_UPDATES
 * - AOMD_GET_FRAME_CORRUPTED
 */

static int run_decoder_control_fuzz(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) {
        return 0;
    }

    aom_codec_err_t ret;
    int val;

    // Feed input data to the decoder
    ret = aom_codec_decode(ctx, data, size, NULL);
    // AOM_CODEC_NEED_MORE is not a standard return code in all versions, 
    // so we only check for OK and ERROR to be safe.
    if (ret != AOM_CODEC_OK && ret != AOM_CODEC_ERROR) {
        return 0;
    }

    // Get output packets to ensure the decoder has processed the frame
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;

    while ((pkt = aom_codec_get_frame(ctx, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Try AOMD_GET_ALTREF_PRESENT
            ret = aom_codec_control(ctx, AOMD_GET_ALTREF_PRESENT, &val);
            (void)ret; (void)val;

            // Try AOMD_GET_SHOW_EXISTING_FRAME_FLAG
            ret = aom_codec_control(ctx, AOMD_GET_SHOW_EXISTING_FRAME_FLAG, &val);
            (void)ret; (void)val;

            // Try AOMD_GET_SHOW_FRAME_FLAG
            ret = aom_codec_control(ctx, AOMD_GET_SHOW_FRAME_FLAG, &val);
            (void)ret; (void)val;

            // Try AOMD_GET_FRAME_FLAGS
            ret = aom_codec_control(ctx, AOMD_GET_FRAME_FLAGS, &val);
            (void)ret; (void)val;

            // Try AOMD_GET_LAST_REF_UPDATES
            int ref_updates[32]; // AV1 has up to 32 reference frames
            memset(ref_updates, 0, sizeof(ref_updates));
            ret = aom_codec_control(ctx, AOMD_GET_LAST_REF_UPDATES, ref_updates);
            (void)ret;

            // Try AOMD_GET_FRAME_CORRUPTED
            ret = aom_codec_control(ctx, AOMD_GET_FRAME_CORRUPTED, &val);
            (void)ret; (void)val;
        }
    }

    return 0;
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize decoder context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        return 0;
    }

    aom_codec_dec_cfg_t cfg;
    // Initialize config with zeros first
    memset(&cfg, 0, sizeof(cfg));
    
    // Set valid configuration options
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;
    // Note: allow_highbitdepth, allow_multiref, allow_intrabc are not valid members
    // of aom_codec_dec_cfg_t in this version of libaom.

    aom_codec_err_t ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Run fuzz logic
    run_decoder_control_fuzz(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}