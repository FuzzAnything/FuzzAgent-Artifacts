// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static vpx_codec_ctx_t *create_codec_context(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t *ctx = NULL;
    vpx_codec_iface_t *iface = NULL;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;

    // Try to initialize a VP9 encoder context first, fallback to VP8
    iface = vpx_codec_vp9_cx();
    if (!iface) {
        iface = vpx_codec_vp8_cx();
        if (!iface) return NULL;
    }

    ctx = (vpx_codec_ctx_t *)calloc(1, sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;

    // Initialize configuration
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzzing data to tweak configuration slightly if possible
    cfg.g_w = (size > 0 ? data[0] % 128 : 0) + 16;
    cfg.g_h = (size > 1 ? data[1] % 128 : 0) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (size > 2 ? data[2] % 60 : 0) + 30;
    cfg.g_error_resilient = 1;

    res = vpx_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_codec_context(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t *ctx = create_codec_context(Data, Size);
    if (!ctx) return 0;

    // Extract control ID from data
    // We use the raw integer value from the fuzzer input as the ctrl_id
    int ctrl_id = Data[0];
    
    // The vpx_codec_control macro expects a defined constant (e.g., VP9E_SET_*),
    // not a variable, because it concatenates the ID into the function name.
    // Since we cannot predict which specific control ID the fuzzer wants to test
    // without a mapping, and the macro requires a literal, we must call the
    // underlying function vpx_codec_control_ directly for dynamic fuzzing of IDs.
    
    // Call the underlying function directly with the variable ctrl_id
    // This allows us to test arbitrary control IDs provided by the fuzzer.
    vpx_codec_err_t res = vpx_codec_control_(ctx, ctrl_id, 0);
    
    // Try with a value derived from data if the control expects one
    if (Size > 1) {
        int value = Data[1];
        res = vpx_codec_control_(ctx, ctrl_id, value);
    }

    // Additional diversity: try different values for the same ID
    if (Size > 2) {
        int val1 = Data[1];
        int val2 = Data[2];
        // Note: vpx_codec_control_ is variadic, but the specific number of args
        // depends on the control ID. Passing extra args might be ignored or cause issues
        // depending on the implementation, but for fuzzing we try to trigger states.
        // However, to be safe and match common patterns, we usually pass 1 or 2 args.
        // Let's try passing two values for controls that might expect a pair.
        res = vpx_codec_control_(ctx, ctrl_id, val1, val2);
    }

    cleanup_codec_context(ctx);
    return 0;
}