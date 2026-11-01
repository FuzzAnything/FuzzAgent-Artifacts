// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>

// Helper to initialize encoder context
static aom_codec_ctx_t* init_encoder(void) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    
    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return NULL;
    
    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));
    
    // Initialize configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    
    // Set some basic config
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    
    // Initialize the encoder
    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    
    return ctx;
}

// Helper to initialize decoder context
static aom_codec_ctx_t* init_decoder(void) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t err;
    
    // Get the AV1 decoder interface
    iface = aom_codec_av1_dx();
    if (!iface) return NULL;
    
    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));
    
    // Initialize configuration
    memset(&cfg, 0, sizeof(cfg));
    
    // Initialize the decoder
    err = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    
    return ctx;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // 1. Initialize Encoder
    aom_codec_ctx_t *enc_ctx = init_encoder();
    if (enc_ctx) {
        // Control 1: AV1E_SET_ENABLE_ANGLE_DELTA (int)
        int val1 = Data[0] & 1;
        aom_codec_control(enc_ctx, AV1E_SET_ENABLE_ANGLE_DELTA, val1);
        
        // Control 2: AV1E_SET_MATRIX_COEFFICIENTS (int)
        int val2 = Data[1] % 5;
        aom_codec_control(enc_ctx, AV1E_SET_MATRIX_COEFFICIENTS, val2);
        
        // Control 3: AV1E_SET_ENABLE_SMOOTH_INTRA (int)
        int val3 = Data[2] & 1;
        aom_codec_control(enc_ctx, AV1E_SET_ENABLE_SMOOTH_INTRA, val3);
        
        // Control 4: AV1E_SET_FILM_GRAIN_TEST_VECTOR (void*)
        // Passing fuzz data as a pointer to test vector handling
        aom_codec_control(enc_ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, (void*)Data);
        
        // Control 5: AV1E_SET_INTER_DCT_ONLY (int)
        int val5 = Data[3] & 1;
        aom_codec_control(enc_ctx, AV1E_SET_INTER_DCT_ONLY, val5);
        
        // Try some invalid controls to test error handling
        aom_codec_control(enc_ctx, 0, 0); // Invalid ID
        aom_codec_control(enc_ctx, -1, 0); // Invalid ID
        
        // Cleanup encoder
        aom_codec_destroy(enc_ctx);
        free(enc_ctx);
    }
    
    // 2. Initialize Decoder
    aom_codec_ctx_t *dec_ctx = init_decoder();
    if (dec_ctx) {
        // Try some generic decoder controls if available
        // Using a known valid decoder control ID if possible, or generic testing
        // Since specific decoder controls weren't listed, we test error paths
        
        // Try invalid controls
        aom_codec_control(dec_ctx, 0, 0);
        aom_codec_control(dec_ctx, -1, 0);
        
        // Cleanup decoder
        aom_codec_destroy(dec_ctx);
        free(dec_ctx);
    }
    
    return 0;
}