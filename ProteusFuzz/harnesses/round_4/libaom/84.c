// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

/* Helper to create a valid encoder context for testing */
static aom_codec_ctx_t *create_encoder_ctx(void) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

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

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/* Helper to create a valid decoder context for testing */
static aom_codec_ctx_t *create_decoder_ctx(void) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_dec_cfg_t cfg;
    cfg.threads = 0;
    cfg.allow_lowbitdepth = 1;

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Use first byte to decide encoder vs decoder context */
    int use_encoder = (Data[0] % 2 == 0);
    aom_codec_ctx_t *ctx = NULL;

    if (use_encoder) {
        ctx = create_encoder_ctx();
    } else {
        ctx = create_decoder_ctx();
    }

    if (!ctx) return 0;

    /* Use second byte to select which API to fuzz */
    int cmd = (Size > 1) ? (Data[1] % 6) : 0;

    int ret = AOM_CODEC_OK;
    int val = 0;
    aom_active_map_t active_map;
    memset(&active_map, 0, sizeof(active_map));

    /* Prepare active_map if needed */
    if (Size > 10) {
        unsigned int rows = (Data[2] % 16) + 1;
        unsigned int cols = (Data[3] % 16) + 1;
        size_t map_size = rows * cols;
        if (map_size > 0 && map_size < 1024) {
            active_map.active_map = (unsigned char *)malloc(map_size);
            if (active_map.active_map) {
                active_map.rows = rows;
                active_map.cols = cols;
                memcpy(active_map.active_map, Data + 4, map_size);
            }
        }
    }

    switch (cmd) {
        case 0:
            /* AV1E_SET_ENABLE_INTERINTRA_COMP */
            val = (Size > 4) ? (Data[4] % 2) : 0;
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, val);
            break;
        case 1:
            /* AOME_SET_ACTIVEMAP */
            ret = aom_codec_control(ctx, AOME_SET_ACTIVEMAP, &active_map);
            break;
        case 2:
            /* AV1E_SET_ENABLE_GLOBAL_MOTION */
            val = (Size > 5) ? (Data[5] % 2) : 0;
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val);
            break;
        case 3:
            /* AV1E_GET_ACTIVEMAP */
            ret = aom_codec_control(ctx, AV1E_GET_ACTIVEMAP, &active_map);
            break;
        case 4:
            /* AV1E_SET_ENABLE_TX_SIZE_SEARCH */
            val = (Size > 6) ? (Data[6] % 2) : 0;
            ret = aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);
            break;
        case 5:
            /* AV1E_SET_TRANSFER_CHARACTERISTICS */
            /* Values for transfer characteristics are typically enums, clamp to safe range */
            val = (Size > 7) ? (Data[7] % 10) : 0;
            ret = aom_codec_control(ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, val);
            break;
        default:
            break;
    }

    /* Cleanup active_map if allocated */
    if (active_map.active_map) {
        free(active_map.active_map);
    }

    /* Cleanup codec context */
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }

    return 0;
}