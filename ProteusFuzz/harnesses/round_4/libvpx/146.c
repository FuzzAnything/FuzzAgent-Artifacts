// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_TUNING__value at vp8cx.h:1006:1 in vp8cx.h
// VP8D_SET_DECRYPTOR__value at vp8dx.h:198:1 in vp8dx.h
// VPXD_SET_DECRYPTOR__value at vp8dx.h:196:1 in vp8dx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static vpx_codec_ctx_t* init_codec_ctx(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t*)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));
    
    // Try VP8 encoder first
    const vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }
    
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_enc_config_default(iface, &cfg, 0);
    
    // Use fuzzing data to set some configuration parameters safely
    if (Size >= 4) {
        uint32_t val = *(const uint32_t*)Data;
        cfg.g_w = (val % 2048) + 16;
        cfg.g_h = ((val >> 16) % 2048) + 16;
        cfg.g_timebase.num = (val % 100) + 1;
        cfg.g_timebase.den = ((val >> 8) % 100) + 1;
    } else {
        cfg.g_w = 128;
        cfg.g_h = 128;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
    }
    
    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    
    return ctx;
}

int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    vpx_codec_ctx_t *ctx = init_codec_ctx(Data, Size);
    if (!ctx) return 0;
    
    // Extract arguments from fuzzing data
    int arg1 = 0, arg2 = 0;
    if (Size >= 2) arg1 = Data[1] % 100;
    if (Size >= 3) arg2 = Data[2] % 100;
    
    // Fuzz known control IDs
    // VP8E_SET_CPUUSED: 0-8
    if (arg1 <= 8) {
        vpx_codec_control(ctx, VP8E_SET_CPUUSED, arg1);
    }
    
    // VP8E_SET_TUNING: 0-1 (usually)
    if (arg1 <= 1) {
        vpx_codec_control(ctx, VP8E_SET_TUNING, arg1);
    }
    
    // VP8D_SET_DECRYPTOR: Requires struct, skip direct integer fuzzing to avoid crash
    // We can try to pass a dummy pointer if we construct a struct, but for safety
    // we skip this in the simple driver or use a valid struct.
    // Since we don't have a valid decrypt_cb, we skip this specific control for now
    // or pass NULL which might trigger an error path we want to test.
    vpx_codec_control(ctx, VP8D_SET_DECRYPTOR, NULL);
    
    // VPXD_SET_DECRYPTOR: Similar to VP8D_SET_DECRYPTOR
    vpx_codec_control(ctx, VPXD_SET_DECRYPTOR, NULL);
    
    // Note: VP9E_SET_TUNE_CONTENT is not available in vp8cx.h, so we skip it
    // unless we include vp9cx.h which was missing in the environment.
    
    // Cleanup
    vpx_codec_destroy(ctx);
    free(ctx);
    
    return 0;
}