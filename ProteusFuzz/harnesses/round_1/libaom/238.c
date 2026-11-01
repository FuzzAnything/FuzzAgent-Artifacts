// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

/* Helper to initialize encoder context */
static aom_codec_ctx_t* init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/* Helper to initialize decoder context */
static aom_codec_ctx_t* init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/* Helper to cleanup encoder */
static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

/* Helper to cleanup decoder */
static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

/* Helper to extract int from data */
static int get_int_from_data(const uint8_t *data, size_t size, int default_val) {
    if (size >= 4) {
        return *(int*)(data);
    }
    return default_val;
}

int LLVMFuzzerTestOneInput_238(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    int val = get_int_from_data(Data, Size, 1);

    /* Test Encoder Controls */
    aom_codec_ctx_t *enc_ctx = init_encoder(Data, Size);
    if (enc_ctx) {
        /* AV1E_SET_ROW_MT */
        aom_codec_control(enc_ctx, AV1E_SET_ROW_MT, val);
        
        /* AV1E_SET_ENABLE_INTERINTER_WEDGE */
        aom_codec_control(enc_ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val);
        
        /* AV1E_SET_COEFF_COST_UPD_FREQ */
        aom_codec_control(enc_ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val);
        
        /* AV1E_SET_FP_MT */
        aom_codec_control(enc_ctx, AV1E_SET_FP_MT, val);
        
        /* AV1E_SET_ENABLE_INTRA_EDGE_FILTER */
        aom_codec_control(enc_ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, val);
        
        cleanup_encoder(enc_ctx);
    }

    /* Test Decoder Controls */
    aom_codec_ctx_t *dec_ctx = init_decoder(Data, Size);
    if (dec_ctx) {
        /* AV1D_SET_ROW_MT */
        aom_codec_control(dec_ctx, AV1D_SET_ROW_MT, val);
        
        cleanup_decoder(dec_ctx);
    }

    return 0;
}