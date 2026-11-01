// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

// Helper to safely initialize a decoder context
static aom_codec_ctx_t* init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Fixed: Removed invalid fields 'allow_highbitdepth' and 'allow_intrabc'
    // cfg.allow_highbitdepth = 1; 
    // cfg.allow_intrabc = 1;
    // Using valid field if needed, though defaults are usually fine for fuzzing
    cfg.allow_lowbitdepth = 1; 

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

// Helper to safely initialize an encoder context
static aom_codec_ctx_t* init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Try Decoder Path
    aom_codec_ctx_t *dec_ctx = init_decoder(Data, Size);
    if (dec_ctx) {
        int result;
        int32_t out_val = 0;
        
        // 1. AOMD_GET_FRAME_FLAGS
        result = aom_codec_control(dec_ctx, AOMD_GET_FRAME_FLAGS, &out_val);
        (void)result;
        
        // 2. AOMD_GET_SHOW_EXISTING_FRAME_FLAG
        result = aom_codec_control(dec_ctx, AOMD_GET_SHOW_EXISTING_FRAME_FLAG, &out_val);
        (void)result;
        
        // 3. AOMD_GET_SHOW_FRAME_FLAG
        result = aom_codec_control(dec_ctx, AOMD_GET_SHOW_FRAME_FLAG, &out_val);
        (void)result;
        
        // 4. AOMD_GET_ALTREF_PRESENT
        result = aom_codec_control(dec_ctx, AOMD_GET_ALTREF_PRESENT, &out_val);
        (void)result;
        
        // 5. AOMD_GET_LAST_REF_UPDATES
        result = aom_codec_control(dec_ctx, AOMD_GET_LAST_REF_UPDATES, &out_val);
        (void)result;
        
        // 6. AOMD_GET_FRAME_CORRUPTED
        result = aom_codec_control(dec_ctx, AOMD_GET_FRAME_CORRUPTED, &out_val);
        (void)result;

        // Cleanup decoder
        aom_codec_destroy(dec_ctx);
        free(dec_ctx);
    }

    // Try Encoder Path
    aom_codec_ctx_t *enc_ctx = init_encoder(Data, Size);
    if (enc_ctx) {
        int result;
        int32_t out_val = 0;
        
        // Attempting decoder commands on encoder context to test error handling
        // or if any are shared.
        
        result = aom_codec_control(enc_ctx, AOMD_GET_FRAME_FLAGS, &out_val);
        (void)result;
        
        result = aom_codec_control(enc_ctx, AOMD_GET_SHOW_EXISTING_FRAME_FLAG, &out_val);
        (void)result;
        
        result = aom_codec_control(enc_ctx, AOMD_GET_SHOW_FRAME_FLAG, &out_val);
        (void)result;
        
        result = aom_codec_control(enc_ctx, AOMD_GET_ALTREF_PRESENT, &out_val);
        (void)result;
        
        result = aom_codec_control(enc_ctx, AOMD_GET_LAST_REF_UPDATES, &out_val);
        (void)result;
        
        result = aom_codec_control(enc_ctx, AOMD_GET_FRAME_CORRUPTED, &out_val);
        (void)result;

        // Cleanup encoder
        aom_codec_destroy(enc_ctx);
        free(enc_ctx);
    }

    return 0;
}