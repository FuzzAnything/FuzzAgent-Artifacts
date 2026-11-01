// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;

    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t res;
    aom_codec_enc_cfg_t cfg;
    
    // Extract parameters from fuzz input
    int idx = 0;
    int enable_val = (idx < Size) ? Data[idx++] : 0;
    int inter_dct_val = (idx < Size) ? Data[idx++] : 0;
    int loopfilter_val = (idx < Size) ? Data[idx++] : 0;
    int cdf_update_val = (idx < Size) ? Data[idx++] : 0;
    int flip_idtx_val = (idx < Size) ? Data[idx++] : 0;
    int diff_wtd_comp_val = (idx < Size) ? Data[idx++] : 0;

    // Initialize encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return 0;

    // Initialize encoder config
    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 0;

    // Initialize codec context
    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Fuzz various control functions using the correct enum constants
    // 1. AV1E_SET_ENABLE_WARPED_MOTION
    res = aom_codec_control(ctx, AV1E_SET_ENABLE_WARPED_MOTION, enable_val);
    (void)res;

    // 2. AV1E_SET_INTER_DCT_ONLY
    res = aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, inter_dct_val);
    (void)res;

    // 3. AV1E_SET_LOOPFILTER_CONTROL
    res = aom_codec_control(ctx, AV1E_SET_LOOPFILTER_CONTROL, loopfilter_val);
    (void)res;

    // 4. AV1E_SET_CDF_UPDATE_MODE
    res = aom_codec_control(ctx, AV1E_SET_CDF_UPDATE_MODE, cdf_update_val);
    (void)res;

    // 5. AV1E_SET_ENABLE_FLIP_IDTX
    res = aom_codec_control(ctx, AV1E_SET_ENABLE_FLIP_IDTX, flip_idtx_val);
    (void)res;

    // 6. AV1E_SET_ENABLE_DIFF_WTD_COMP
    res = aom_codec_control(ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, diff_wtd_comp_val);
    (void)res;

    // Cleanup
    cleanup_codec(ctx);

    return 0;
}