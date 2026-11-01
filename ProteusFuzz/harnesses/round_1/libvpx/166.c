#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8dx.h>

static void safe_destroy(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_166(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 128;
    cfg.h = 128;

    vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    if (!iface) return 0;

    // Scenario 1: Trigger Line 30 (ver != VPX_DECODER_ABI_VERSION)
    vpx_codec_err_t res;
    res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, 0); 
    (void)res;
    safe_destroy(&ctx);
    memset(&ctx, 0, sizeof(ctx));

    // Scenario 2: Trigger Line 32 (!ctx || !iface)
    res = vpx_codec_dec_init_ver(NULL, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    (void)res;
    
    res = vpx_codec_dec_init_ver(&ctx, NULL, &cfg, 0, VPX_DECODER_ABI_VERSION);
    (void)res;
    memset(&ctx, 0, sizeof(ctx));

    // Scenario 3: Trigger Lines 36-37, 39-40, 42-43 (Unsupported flags)
    res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, VPX_CODEC_USE_POSTPROC, VPX_DECODER_ABI_VERSION);
    (void)res;
    safe_destroy(&ctx);
    memset(&ctx, 0, sizeof(ctx));

    res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, VPX_CODEC_USE_ERROR_CONCEALMENT, VPX_DECODER_ABI_VERSION);
    (void)res;
    safe_destroy(&ctx);
    memset(&ctx, 0, sizeof(ctx));

    res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, VPX_CODEC_USE_INPUT_FRAGMENTS, VPX_DECODER_ABI_VERSION);
    (void)res;
    safe_destroy(&ctx);
    memset(&ctx, 0, sizeof(ctx));

    // Scenario 4: Trigger Line 55/56/57 (Internal init failure or success)
    res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        vpx_codec_error_detail(&ctx);
    } else {
        safe_destroy(&ctx);
    }

    // Scenario 5: Try to trigger internal init failure with bad config
    memset(&ctx, 0, sizeof(ctx));
    vpx_codec_dec_cfg_t bad_cfg;
    memset(&bad_cfg, 0, sizeof(bad_cfg));
    bad_cfg.threads = 0;
    bad_cfg.w = 0;
    bad_cfg.h = 0;
    
    res = vpx_codec_dec_init_ver(&ctx, iface, &bad_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        vpx_codec_error_detail(&ctx);
    } else {
        safe_destroy(&ctx);
    }

    return 0;
}