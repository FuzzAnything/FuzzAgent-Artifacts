// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
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

static vpx_codec_ctx_t *init_codec(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;

    if (!data || size == 0) {
        // Default fallback if no data
        iface = vpx_codec_vp9_cx();
    } else {
        iface = vpx_codec_vp9_cx();
    }

    if (!iface) return NULL;

    memset(&ctx, 0, sizeof(ctx));
    ctx.name = "fuzz_ctx";
    ctx.iface = iface;
    ctx.err = VPX_CODEC_OK;
    ctx.err_detail = NULL;
    ctx.init_flags = 0;
    ctx.priv = NULL;

    res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) return NULL;

    cfg.g_w = (size > 0 ? (data[0] % 2048) + 16 : 128);
    cfg.g_h = (size > 1 ? (data[1] % 2048) + 16 : 128);
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;

    res = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) return NULL;

    ctx.config.enc = &cfg;
    
    // Return a pointer to a static or allocated buffer? 
    // vpx_codec_enc_init modifies the ctx passed in. 
    // We need to return a valid pointer. Since we can't return a pointer to a local stack variable safely 
    // if the function returns, we must allocate on heap or use a static. 
    // However, vpx_codec_enc_init expects a valid ctx. 
    // Let's allocate the ctx on the heap to return safely.
    vpx_codec_ctx_t *heap_ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!heap_ctx) return NULL;
    memcpy(heap_ctx, &ctx, sizeof(vpx_codec_ctx_t));
    // Note: The internal 'priv' pointer inside ctx was allocated by vpx_codec_enc_init.
    // It points to heap memory managed by libvpx. 
    // The 'ctx' struct itself was on stack. We copied it to heap_ctx.
    // This is generally unsafe if libvpx expects the original pointer, but for fuzzing 
    // where we just want to call control functions, it might work if the iface handles it.
    // Actually, vpx_codec_enc_init stores the passed ctx pointer in internal structures.
    // So we MUST pass the heap allocated ctx to vpx_codec_enc_init directly.
    
    // Re-doing init properly with heap allocation
    free(heap_ctx); // Free the bad copy
    
    vpx_codec_ctx_t *final_ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!final_ctx) return NULL;
    memset(final_ctx, 0, sizeof(vpx_codec_ctx_t));
    final_ctx->name = "fuzz_ctx";
    final_ctx->iface = iface;
    
    res = vpx_codec_enc_init(final_ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        free(final_ctx);
        return NULL;
    }
    final_ctx->config.enc = &cfg;
    
    return final_ctx;
}

static void cleanup_codec(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        // vpx_codec_destroy frees the context structure itself in libvpx
        // So we should NOT free(ctx) here.
    }
}

// Helper functions defined outside LLVMFuzzerTestOneInput
static int call_control_int(vpx_codec_ctx_t *c, int id, int val) {
    return vpx_codec_control_(c, id, val);
}

static int call_control_ptr(vpx_codec_ctx_t *c, int id, void *val) {
    return vpx_codec_control_(c, id, val);
}

int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    vpx_codec_ctx_t *ctx = init_codec(Data, Size);
    if (!ctx) return 0;

    int loopfilter_val = 0;
    
    // 1. VP9E_GET_LOOPFILTER_LEVEL
    #ifdef VP9E_GET_LOOPFILTER_LEVEL
    vpx_codec_control_(ctx, VP9E_GET_LOOPFILTER_LEVEL, &loopfilter_val);
    #endif

    // 2. VP9E_SET_DISABLE_LOOPFILTER
    #ifdef VP9E_SET_DISABLE_LOOPFILTER
    call_control_int(ctx, VP9E_SET_DISABLE_LOOPFILTER, Data[0] % 2);
    #endif

    // 3. VP9E_SET_RENDER_SIZE
    #ifdef VP9E_SET_RENDER_SIZE
    int w = (Size > 0 ? (Data[0] % 2048) + 16 : 128);
    int h = (Size > 1 ? (Data[1] % 2048) + 16 : 128);
    vpx_codec_control_(ctx, VP9E_SET_RENDER_SIZE, w, h);
    #endif

    // 4. VP9_SET_SKIP_LOOP_FILTER
    #ifdef VP9_SET_SKIP_LOOP_FILTER
    call_control_int(ctx, VP9_SET_SKIP_LOOP_FILTER, Data[0] % 2);
    #endif

    // 5. VP9E_SET_SVC
    #ifdef VP9E_SET_SVC
    call_control_int(ctx, VP9E_SET_SVC, Data[0] % 2);
    #endif

    cleanup_codec(ctx);

    return 0;
}