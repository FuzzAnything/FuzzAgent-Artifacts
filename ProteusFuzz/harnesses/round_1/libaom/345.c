// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER at aomcx.h:1902:1 in aomcx.h
// aom_codec_control_typechecked_AOMD_GET_LAST_QUANTIZER at aomdx.h:519:1 in aomdx.h
// aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER_64 at aomcx.h:1905:1 in aomcx.h
// aom_codec_control_typechecked_AOMD_GET_SB_SIZE at aomdx.h:580:1 in aomdx.h
// aom_codec_control_typechecked_AOME_SET_CQ_LEVEL at aomcx.h:1917:1 in aomcx.h
// aom_codec_control_typechecked_AOMD_GET_BASE_Q_IDX at aomdx.h:592:1 in aomdx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

// Helper to initialize encoder context
static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
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

    // Fuzz configuration slightly based on input data if size > 0
    if (size > 0) {
        cfg.g_w = (data[0] % 512) + 16;
        cfg.g_h = (data[1] % 512) + 16;
        cfg.g_timebase.num = (data[2] % 100) + 1;
        cfg.g_timebase.den = (data[3] % 1000) + 1;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

// Helper to initialize decoder context
static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    if (size > 0) {
        cfg.threads = (data[0] % 8) + 1;
        // Fixed field name from allow_highbitdepth to allow_lowbitdepth
        cfg.allow_lowbitdepth = (data[1] % 2);
    }

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

// Cleanup helper
static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_345(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Determine which codec to use based on input
    int use_encoder = (Data[0] % 2);
    int use_decoder = !use_encoder;

    aom_codec_ctx_t *ctx = NULL;

    if (use_encoder) {
        ctx = init_encoder(Data, Size);
    } else {
        ctx = init_decoder(Data, Size);
    }

    if (!ctx) return 0;

    // Prepare variables for return values
    int int_val = 0;
    aom_superblock_size_t sb_size = AOM_SUPERBLOCK_SIZE_64X64;
    unsigned int cq_level = 0;

    // Fuzz control parameters
    if (Size > 1) {
        cq_level = (unsigned int)(Data[1] % 64); 
    }

    // Invoke target functions
    // The typechecked macros expand to functions requiring 3 arguments:
    // (ctx, control_id, data_ptr_or_val)
    // The control_id is the first argument of the macro definition (e.g., AOME_GET_LAST_QUANTIZER)

    // 1. AOME_GET_LAST_QUANTIZER (Encoder only)
    if (use_encoder) {
        aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER(ctx, AOME_GET_LAST_QUANTIZER, &int_val);
    }

    // 2. AOMD_GET_LAST_QUANTIZER (Decoder only)
    if (use_decoder) {
        aom_codec_control_typechecked_AOMD_GET_LAST_QUANTIZER(ctx, AOMD_GET_LAST_QUANTIZER, &int_val);
    }

    // 3. AOME_GET_LAST_QUANTIZER_64 (Encoder only)
    if (use_encoder) {
        aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER_64(ctx, AOME_GET_LAST_QUANTIZER_64, &int_val);
    }

    // 4. AOMD_GET_SB_SIZE (Decoder only)
    if (use_decoder) {
        aom_codec_control_typechecked_AOMD_GET_SB_SIZE(ctx, AOMD_GET_SB_SIZE, &sb_size);
    }

    // 5. AOME_SET_CQ_LEVEL (Encoder only)
    if (use_encoder) {
        aom_codec_control_typechecked_AOME_SET_CQ_LEVEL(ctx, AOME_SET_CQ_LEVEL, cq_level);
    }

    // 6. AOMD_GET_BASE_Q_IDX (Decoder only)
    if (use_decoder) {
        aom_codec_control_typechecked_AOMD_GET_BASE_Q_IDX(ctx, AOMD_GET_BASE_Q_IDX, &int_val);
    }

    // Cleanup
    cleanup_codec(ctx);

    return 0;
}