// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_TILE_ROWS__value at vp8cx.h:1026:1 in vp8cx.h
// VP9E_SET_TILE_COLUMNS__value at vp8cx.h:1024:1 in vp8cx.h
// VP8E_GET_LAST_QUANTIZER__value at vp8cx.h:996:1 in vp8cx.h
// VP8E_GET_LAST_QUANTIZER_64__value at vp8cx.h:998:1 in vp8cx.h
// VPXD_GET_LAST_QUANTIZER__value at vp8dx.h:214:1 in vp8dx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface;
    vpx_codec_err_t res;
    int tile_rows, tile_cols, quantizer;
    int64_t q64;

    if (Size < 3) {
        return 0;
    }

    // Initialize context structure manually to avoid dependency on init functions
    memset(&ctx, 0, sizeof(ctx));
    ctx.name = "fuzz_dummy";
    ctx.iface = NULL;
    ctx.err = VPX_CODEC_OK;
    ctx.err_detail = NULL;
    ctx.init_flags = 0;
    ctx.config.raw = NULL;
    ctx.priv = NULL;

    // Initialize config
    memset(&cfg, 0, sizeof(cfg));
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 0;
    cfg.g_pass = 0;
    cfg.g_profile = 0;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_dropframe_thresh = 0;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;
    cfg.rc_end_usage = VPX_CBR;

    // Extract parameters from fuzz data
    tile_rows = Data[0] % 16;
    tile_cols = Data[1] % 16;
    quantizer = Data[2] % 64;

    // Try VP8
    iface = vpx_codec_vp8_cx();
    if (iface) {
        res = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
        if (res == VPX_CODEC_OK) {
            // Test VP9E controls on VP8 context (expected to return INCAPABLE)
            // Note: VP9E_* constants are defined in vpx/vpx_codec.h for generic usage
            vpx_codec_control(&ctx, VP9E_SET_TILE_ROWS, tile_rows);
            vpx_codec_control(&ctx, VP9E_SET_TILE_COLUMNS, tile_cols);
            
            // Test VP8E_GET_LAST_QUANTIZER
            vpx_codec_control(&ctx, VP8E_GET_LAST_QUANTIZER, &quantizer);
            
            // Test VP8E_GET_LAST_QUANTIZER_64
            q64 = 0;
            vpx_codec_control(&ctx, VP8E_GET_LAST_QUANTIZER_64, &q64);
            
            // Test VPXD_GET_LAST_QUANTIZER (Decoder control on Encoder context)
            vpx_codec_control(&ctx, VPXD_GET_LAST_QUANTIZER, &quantizer);

            vpx_codec_destroy(&ctx);
            memset(&ctx, 0, sizeof(ctx));
        }
    }

    // Try VP9 (using vpx_codec_vp9_cx if available, otherwise skip)
    // Note: vp9cx.h is not included due to missing file error, 
    // but vpx_codec_vp9_cx is usually declared in vpx_codec.h or vp9cx.h.
    // If vp9cx.h is missing, we rely on vpx_codec.h having the declaration or skip VP9 specific init.
    // Assuming standard libvpx installation where vpx_codec_vp9_cx is available via vpx_codec.h or similar.
    // If compilation fails here due to missing declaration, the user needs to ensure vp9cx.h is present 
    // or use a different method to get the VP9 interface. 
    // For this fix, we attempt to call it assuming it's declared in vpx_codec.h in some versions, 
    // or we rely on the fact that the error was specifically about the header file.
    // Since we cannot include vp9cx.h, we will skip VP9 specific init if the symbol is not found.
    // However, to keep the driver robust, we will just try to get the interface if the symbol exists.
    // In standard libvpx, vpx_codec_vp9_cx is often in vp9cx.h. 
    // If that header is missing, we might not be able to init VP9. 
    // We will proceed with VP8 only if VP9 init fails or is unavailable.
    
    // Attempt to get VP9 interface (declaration might be missing without vp9cx.h)
    // We cast to void* to avoid compilation error if the function is not declared, 
    // but this is risky. Better to assume the environment has the necessary declarations 
    // or rely on vpx_codec.h. 
    // Let's assume vpx_codec_vp9_cx is declared in vpx_codec.h for this specific build environment 
    // or we simply skip VP9 testing if the header is truly missing.
    // Given the error, we remove the VP9 specific block to ensure compilation.
    
    // Re-adding VP9 block assuming vpx_codec_vp9_cx is available via vpx_codec.h in some setups
    // or by using a function pointer trick if strictly necessary, but standard practice is to include headers.
    // Since the user said vp9cx.h is not found, we will omit VP9 initialization to fix the compile error.
    // The fuzzing will still cover VP8 and the control logic.

    // Test direct call to vpx_codec_control_ with invalid context (null iface)
    // This tests the error handling path for missing interface
    memset(&ctx, 0, sizeof(ctx));
    ctx.iface = NULL;
    ctx.priv = NULL;
    vpx_codec_control_(&ctx, VP9E_SET_TILE_ROWS, tile_rows);
    vpx_codec_control_(&ctx, VP8E_GET_LAST_QUANTIZER, &quantizer);
    vpx_codec_control_(&ctx, VPXD_GET_LAST_QUANTIZER, &quantizer);

    return 0;
}