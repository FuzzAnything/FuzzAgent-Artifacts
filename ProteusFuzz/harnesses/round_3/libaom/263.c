// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>

// Helper to safely initialize the decoder context
static int init_decoder(aom_codec_ctx_t *ctx, const aom_codec_iface_t *iface) {
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Removed invalid fields: allow_highbitdepth, allow_intrabc, allow_superres, allow_frame_parallel, show_frame
    // These fields do not exist in the standard aom_codec_dec_cfg_t structure for the target version.
    
    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

// Helper to safely finalize the decoder context
static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

// Helper to get the AV1 decoder interface
static const aom_codec_iface_t *get_av1_decoder_iface(void) {
    return aom_codec_av1_dx();
}

int LLVMFuzzerTestOneInput_263(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = get_av1_decoder_iface();
    if (!iface) {
        return 0;
    }

    if (!init_decoder(&ctx, iface)) {
        return 0;
    }

    // Fuzz aom_codec_decode
    // We pass the fuzzed data directly as the coded data buffer.
    // The size is Size. user_priv is set to NULL.
    aom_codec_decode(&ctx, Data, Size, NULL);

    // Attempt to query decoder state via control functions.
    // Using standard aom_codec_control API with known control IDs.
    
    // AOMD_GET_ALTREF_PRESENT
    int altref_present = 0;
    aom_codec_control(&ctx, AOMD_GET_ALTREF_PRESENT, &altref_present);

    // AOMD_GET_LAST_QUANTIZER
    int last_quantizer = 0;
    aom_codec_control(&ctx, AOMD_GET_LAST_QUANTIZER, &last_quantizer);

    // AOMD_GET_FRAME_CORRUPTED
    int frame_corrupted = 0;
    aom_codec_control(&ctx, AOMD_GET_FRAME_CORRUPTED, &frame_corrupted);

    // AOMD_GET_LAST_REF_UPDATES
    int last_ref_updates = 0;
    aom_codec_control(&ctx, AOMD_GET_LAST_REF_UPDATES, &last_ref_updates);

    // AV1D_SET_EXT_REF_PTR
    // Create a dummy structure to test the control path.
    av1_ext_ref_frame_t ext_ref;
    memset(&ext_ref, 0, sizeof(ext_ref));
    ext_ref.num = 0;
    ext_ref.img = NULL;
    aom_codec_control(&ctx, AV1D_SET_EXT_REF_PTR, &ext_ref);

    // Drain the decoder to ensure all frames are processed and resources are freed
    aom_codec_cx_pkt_t pkt;
    while (aom_codec_get_frame(&ctx, &pkt) == AOM_CODEC_OK) {
        // Discard frames
    }

    cleanup_decoder(&ctx);
    return 0;
}