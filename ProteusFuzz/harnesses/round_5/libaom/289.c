// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static aom_codec_ctx_t* create_encoder_context(const uint8_t *data, size_t size) {
    if (size < 4) return NULL;
    
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    
    memset(ctx, 0, sizeof(aom_codec_ctx_t));
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }
    
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    
    // Use fuzzing data to configure width/height safely
    cfg.g_w = (data[0] % 256) + 16;
    cfg.g_h = (data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (data[2] % 255) + 1;
    
    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    
    return ctx;
}

static aom_codec_ctx_t* create_decoder_context(const uint8_t *data, size_t size) {
    if (size < 4) return NULL;
    
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    
    memset(ctx, 0, sizeof(aom_codec_ctx_t));
    
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }
    
    aom_codec_dec_cfg_t cfg;
    // Initialize config manually with valid fields only
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 0;
    cfg.allow_lowbitdepth = 0;
    
    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    
    return ctx;
}

int LLVMFuzzerTestOneInput_289(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    aom_codec_ctx_t *enc_ctx = NULL;
    aom_codec_ctx_t *dec_ctx = NULL;
    
    // Create encoder context
    enc_ctx = create_encoder_context(Data, Size);
    if (enc_ctx) {
        int val = (Data[0] % 2);
        
        // Test AV1E_SET_ENABLE_INTRABC
        aom_codec_control(enc_ctx, AV1E_SET_ENABLE_INTRABC, val);
        
        // Test AV1E_SET_SKIP_POSTPROC_FILTERING
        aom_codec_control(enc_ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, val);
        
        // Test AV1E_SET_TILE_ROWS
        int tile_rows = Data[1] % 8;
        aom_codec_control(enc_ctx, AV1E_SET_TILE_ROWS, tile_rows);
        
        // Test AV1E_SET_ENABLE_RECT_TX
        int rect_tx = (Data[2] % 2);
        aom_codec_control(enc_ctx, AV1E_SET_ENABLE_RECT_TX, rect_tx);
        
        cleanup_codec(enc_ctx);
    }
    
    // Create decoder context
    dec_ctx = create_decoder_context(Data, Size);
    if (dec_ctx) {
        int val = (Data[0] % 2);
        
        // Test AV1_SET_SKIP_LOOP_FILTER
        aom_codec_control(dec_ctx, AV1_SET_SKIP_LOOP_FILTER, val);
        
        // Test AOME_GET_LOOPFILTER_LEVEL
        int loopfilter_level;
        aom_codec_control(dec_ctx, AOME_GET_LOOPFILTER_LEVEL, &loopfilter_level);
        
        cleanup_codec(dec_ctx);
    }
    
    return 0;
}