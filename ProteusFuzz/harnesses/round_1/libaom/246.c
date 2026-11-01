// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AOMD_GET_TILE_INFO at aomdx.h:570:1 in aomdx.h
// aom_codec_control_typechecked_AV1E_SET_SINGLE_TILE_DECODING at aomcx.h:2168:1 in aomcx.h
// aom_codec_control_typechecked_AV1_INVERT_TILE_DECODE_ORDER at aomdx.h:510:1 in aomdx.h
// aom_codec_control_typechecked_AV1E_SET_TILE_COLUMNS at aomcx.h:1942:1 in aomcx.h
// aom_codec_control_typechecked_AV1_SET_TILE_MODE at aomdx.h:528:1 in aomdx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_RECT_PARTITIONS at aomcx.h:2060:1 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom_decoder.h"
#include "aomdx.h"
#include "aom_external_partition.h"
#include "aom_integer.h"
#include "aom_image.h"
#include "aom_encoder.h"
#include "aomcx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_frame_buffer.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int try_init_decoder(aom_codec_ctx_t *ctx, aom_codec_iface_t *iface, const struct aom_codec_dec_cfg *cfg, aom_codec_flags_t flags) {
    return aom_codec_dec_init(ctx, iface, cfg, flags);
}

static int try_init_encoder(aom_codec_ctx_t *ctx, aom_codec_iface_t *iface, const struct aom_codec_enc_cfg *cfg, aom_codec_flags_t flags) {
    return aom_codec_enc_init(ctx, iface, cfg, flags);
}

int LLVMFuzzerTestOneInput_246(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Use the first byte to decide whether to test decoder or encoder
    int is_decoder = (Data[0] % 2 == 0);
    
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t err;
    aom_codec_flags_t flags = 0;
    
    // Initialize configuration structures
    struct aom_codec_dec_cfg dec_cfg;
    struct aom_codec_enc_cfg enc_cfg;
    
    // Default initialization for decoder config
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.w = 32;
    dec_cfg.h = 32;
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 0;

    // Initialize encoder config using the standard API
    iface = is_decoder ? aom_codec_av1_dx() : aom_codec_av1_cx();
    if (!iface) return 0;
    
    if (!is_decoder) {
        err = aom_codec_enc_config_default(iface, &enc_cfg, 0);
        if (err != AOM_CODEC_OK) {
            cleanup_codec(&ctx);
            return 0;
        }
    }

    // Fuzz configuration parameters based on input data
    if (Size > 1) dec_cfg.w = Data[1];
    if (Size > 2) dec_cfg.h = Data[2];
    if (Size > 3) dec_cfg.threads = Data[3] % 16;
    if (Size > 4) dec_cfg.allow_lowbitdepth = Data[4] % 2;
    
    if (Size > 5) enc_cfg.g_w = Data[5];
    if (Size > 6) enc_cfg.g_h = Data[6];
    if (Size > 7) enc_cfg.g_threads = Data[7] % 16;
    if (Size > 8) enc_cfg.g_lag_in_frames = Data[8] % 25;
    if (Size > 9) enc_cfg.rc_target_bitrate = Data[9] * 1000;
    if (Size > 10) enc_cfg.g_error_resilient = Data[10] % 2;

    // Initialize the codec context
    if (is_decoder) {
        iface = aom_codec_av1_dx();
        if (!iface) return 0;
        err = try_init_decoder(&ctx, iface, &dec_cfg, flags);
        if (err != AOM_CODEC_OK) {
            cleanup_codec(&ctx);
            return 0;
        }
    } else {
        iface = aom_codec_av1_cx();
        if (!iface) return 0;
        err = try_init_encoder(&ctx, iface, &enc_cfg, flags);
        if (err != AOM_CODEC_OK) {
            cleanup_codec(&ctx);
            return 0;
        }
    }

    // Prepare tile info structure for GET_TILE_INFO
    aom_tile_info tile_info;
    memset(&tile_info, 0, sizeof(tile_info));

    // Extract values from input data for control parameters
    int val1 = (Size > 11) ? (Data[11] % 2) : 0;
    int val2 = (Size > 12) ? (Data[12] % 2) : 0;
    int val3 = (Size > 13) ? (Data[13] % 16) : 0; // Tile columns usually limited
    int val4 = (Size > 14) ? (Data[14] % 2) : 0;
    int val5 = (Size > 15) ? (Data[15] % 2) : 0;

    // Call target API functions
    // The typechecked functions require: (ctx, ctrl_id, data)
    
    // 1. AOMD_GET_TILE_INFO (Decoder specific)
    if (is_decoder) {
        aom_codec_control_typechecked_AOMD_GET_TILE_INFO(&ctx, AOMD_GET_TILE_INFO, &tile_info);
    }

    // 2. AV1E_SET_SINGLE_TILE_DECODING (Encoder specific)
    if (!is_decoder) {
        aom_codec_control_typechecked_AV1E_SET_SINGLE_TILE_DECODING(&ctx, AV1E_SET_SINGLE_TILE_DECODING, (unsigned int)val1);
    }

    // 3. AV1_INVERT_TILE_DECODE_ORDER (Decoder specific)
    if (is_decoder) {
        aom_codec_control_typechecked_AV1_INVERT_TILE_DECODE_ORDER(&ctx, AV1_INVERT_TILE_DECODE_ORDER, val2);
    }

    // 4. AV1E_SET_TILE_COLUMNS (Encoder specific)
    if (!is_decoder) {
        aom_codec_control_typechecked_AV1E_SET_TILE_COLUMNS(&ctx, AV1E_SET_TILE_COLUMNS, (unsigned int)val3);
    }

    // 5. AV1_SET_TILE_MODE (Decoder specific)
    if (is_decoder) {
        aom_codec_control_typechecked_AV1_SET_TILE_MODE(&ctx, AV1_SET_TILE_MODE, (unsigned int)val4);
    }

    // 6. AV1E_SET_ENABLE_RECT_PARTITIONS (Encoder specific)
    if (!is_decoder) {
        aom_codec_control_typechecked_AV1E_SET_ENABLE_RECT_PARTITIONS(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, val5);
    }

    // Cleanup
    cleanup_codec(&ctx);
    
    return 0;
}