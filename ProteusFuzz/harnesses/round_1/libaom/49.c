// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

// Helper function to initialize decoder context
static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Fixed: Removed invalid members allow_highbitdepth and allow_intrabc
    // cfg.allow_highbitdepth = 1; 
    // cfg.allow_intrabc = 1;

    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_err_t ret = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Attempt to decode the input data to get a valid frame context
    aom_codec_iter_t iter = NULL;
    aom_codec_cx_pkt_t *pkt = aom_codec_get_frame(ctx, &iter);
    
    // If no frame was decoded, we still proceed to test control functions
    // as they may handle the "no frame" state or return specific errors.
    
    return ctx;
}

// Helper function to cleanup decoder context
static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_decoder(Data, Size);
    if (!ctx) return 0;

    // Ensure we have a frame to query, otherwise the control functions might fail
    // or return default values. We try to decode at least one frame.
    aom_codec_iter_t iter = NULL;
    aom_codec_cx_pkt_t *pkt = aom_codec_get_frame(ctx, &iter);
    
    // 1. AOMD_GET_STILL_PICTURE
    aom_still_picture_info still_info = {0};
    aom_codec_control(ctx, AOMD_GET_STILL_PICTURE, &still_info);

    // 2. AOMD_GET_ORDER_HINT
    int order_hint = 0;
    aom_codec_control(ctx, AOMD_GET_ORDER_HINT, &order_hint);

    // 3. AOMD_GET_LAST_REF_USED
    int last_ref_used = 0;
    aom_codec_control(ctx, AOMD_GET_LAST_REF_USED, &last_ref_used);

    // 4. AOMD_GET_FWD_KF_PRESENT
    int fwd_kf_present = 0;
    aom_codec_control(ctx, AOMD_GET_FWD_KF_PRESENT, &fwd_kf_present);

    // 5. AOMD_GET_S_FRAME_INFO
    aom_s_frame_info s_frame_info = {0};
    aom_codec_control(ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);

    // 6. AOMD_GET_ALTREF_PRESENT
    int altref_present = 0;
    aom_codec_control(ctx, AOMD_GET_ALTREF_PRESENT, &altref_present);

    // Cleanup
    cleanup_decoder(ctx);

    return 0;
}