// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AV1_SET_TILE_MODE at aomdx.h:528:1 in aomdx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control_typechecked_AV1_SET_DECODE_TILE_COL at aomdx.h:525:1 in aomdx.h
// aom_codec_control_typechecked_AV1E_SET_SINGLE_TILE_DECODING at aomcx.h:2168:1 in aomcx.h
// aom_codec_control_typechecked_AV1_INVERT_TILE_DECODE_ORDER at aomdx.h:510:1 in aomdx.h
// aom_codec_control_typechecked_AV1D_GET_TILE_COUNT at aomdx.h:507:1 in aomdx.h
// aom_codec_control_typechecked_AV1_SET_DECODE_TILE_ROW at aomdx.h:522:1 in aomdx.h
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

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize decoder context
    aom_codec_ctx_t dec_ctx;
    aom_codec_dec_cfg_t dec_cfg;
    aom_codec_iface_t *dec_iface = aom_codec_av1_dx();
    
    if (!dec_iface) return 0;

    // Initialize decoder config with valid fields
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    if (aom_codec_dec_init(&dec_ctx, dec_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize encoder context
    aom_codec_ctx_t enc_ctx;
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    
    if (!enc_iface) {
        cleanup_decoder(&dec_ctx);
        return 0;
    }

    if (aom_codec_enc_config_default(enc_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
        cleanup_decoder(&dec_ctx);
        return 0;
    }

    if (aom_codec_enc_init(&enc_ctx, enc_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
        cleanup_decoder(&dec_ctx);
        return 0;
    }

    // Fuzz AV1_SET_DECODE_TILE_COL (Decoder)
    if (Size > 0) {
        int tile_col = Data[0] % 64; 
        aom_codec_control_typechecked_AV1_SET_DECODE_TILE_COL(&dec_ctx, AV1_SET_DECODE_TILE_COL, tile_col);
    }

    // Fuzz AV1E_SET_SINGLE_TILE_DECODING (Encoder)
    if (Size > 1) {
        unsigned int single_tile = Data[1] % 2;
        aom_codec_control_typechecked_AV1E_SET_SINGLE_TILE_DECODING(&enc_ctx, AV1E_SET_SINGLE_TILE_DECODING, single_tile);
    }

    // Fuzz AV1_INVERT_TILE_DECODE_ORDER (Decoder)
    if (Size > 2) {
        int invert = Data[2] % 2;
        aom_codec_control_typechecked_AV1_INVERT_TILE_DECODE_ORDER(&dec_ctx, AV1_INVERT_TILE_DECODE_ORDER, invert);
    }

    // Fuzz AV1D_GET_TILE_COUNT (Decoder)
    if (Size > 3) {
        int tile_count = 0;
        aom_codec_control_typechecked_AV1D_GET_TILE_COUNT(&dec_ctx, AV1D_GET_TILE_COUNT, &tile_count);
        (void)tile_count;
    }

    // Fuzz AV1_SET_DECODE_TILE_ROW (Decoder)
    if (Size > 4) {
        int tile_row = Data[4] % 64; 
        aom_codec_control_typechecked_AV1_SET_DECODE_TILE_ROW(&dec_ctx, AV1_SET_DECODE_TILE_ROW, tile_row);
    }

    // Fuzz AV1_SET_TILE_MODE (Decoder)
    if (Size > 5) {
        int tile_mode = Data[5] % 4; 
        aom_codec_control_typechecked_AV1_SET_TILE_MODE(&dec_ctx, AV1_SET_TILE_MODE, tile_mode);
    }

    // Cleanup
    cleanup_decoder(&dec_ctx);
    cleanup_encoder(&enc_ctx);

    return 0;
}