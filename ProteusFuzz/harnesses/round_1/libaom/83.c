// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int fuzz_decoder_control(aom_codec_ctx_t *ctx) {
    if (!ctx) return 0;

    aom_codec_err_t res;
    aom_s_frame_info s_frame_info;
    int show_frame_flag;
    int order_hint;
    int frame_flags;
    int last_ref_used;

    // AOMD_GET_SHOW_FRAME_FLAG
    res = aom_codec_control(ctx, AOMD_GET_SHOW_FRAME_FLAG, &show_frame_flag);
    (void)res;

    // AOMD_GET_LAST_REF_USED
    res = aom_codec_control(ctx, AOMD_GET_LAST_REF_USED, &last_ref_used);
    (void)res;

    // AOMD_GET_ORDER_HINT
    res = aom_codec_control(ctx, AOMD_GET_ORDER_HINT, &order_hint);
    (void)res;

    // AOMD_GET_FRAME_FLAGS
    res = aom_codec_control(ctx, AOMD_GET_FRAME_FLAGS, &frame_flags);
    (void)res;

    // AOMD_GET_S_FRAME_INFO
    res = aom_codec_control(ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);
    (void)res;

    return 0;
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize decoder context
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) return 0;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1; 

    aom_codec_err_t err = aom_codec_dec_init(&ctx, decoder_iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Feed data to the decoder to trigger state changes
    aom_codec_err_t decode_err = aom_codec_decode(&ctx, Data, Size, NULL);
    
    // Iterate over output packets to ensure state is updated
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt_out;
    
    while ((pkt_out = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        // Check for valid frame types. 
        // Use AOM_CODEC_CX_FRAME_PKT as the correct enum value.
        if (pkt_out->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Now the context has a valid frame state to query
            fuzz_decoder_control(&ctx);
        }
    }

    // Even if no frames were decoded, try to call control functions to test error paths/empty state
    fuzz_decoder_control(&ctx);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}