// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>

static int try_decoder_fuzz(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL) return 0;

    // AV1_INVERT_TILE_DECODE_ORDER
    int invert_order = 1;
    if (size > 0) invert_order = data[0] & 0xFF;
    aom_codec_control(ctx, AV1_INVERT_TILE_DECODE_ORDER, invert_order);

    // AV1_SET_DECODE_TILE_COL
    int tile_col = 0;
    if (size > 1) tile_col = data[1] & 0xFF;
    if (tile_col > 0) tile_col = (tile_col % 64) + 1;
    aom_codec_control(ctx, AV1_SET_DECODE_TILE_COL, tile_col);

    // AV1_SET_DECODE_TILE_ROW
    int tile_row = 0;
    if (size > 2) tile_row = data[2] & 0xFF;
    if (tile_row > 0) tile_row = (tile_row % 64) + 1;
    aom_codec_control(ctx, AV1_SET_DECODE_TILE_ROW, tile_row);

    // AOMD_GET_TILE_INFO
    aom_tile_info info;
    memset(&info, 0, sizeof(info));
    aom_codec_control(ctx, AOMD_GET_TILE_INFO, &info);

    // AV1_SET_TILE_MODE
    int tile_mode = 0;
    if (size > 3) tile_mode = data[3] & 0xFF;
    aom_codec_control(ctx, AV1_SET_TILE_MODE, tile_mode);

    return 0;
}

static int try_encoder_fuzz(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL) return 0;

    // AV1E_SET_SINGLE_TILE_DECODING
    int single_tile = 0;
    if (size > 0) single_tile = data[0] & 0xFF;
    aom_codec_control(ctx, AV1E_SET_SINGLE_TILE_DECODING, single_tile);

    // AV1_INVERT_TILE_DECODE_ORDER (also valid for encoder context in some configurations)
    int invert_order = 1;
    if (size > 1) invert_order = data[1] & 0xFF;
    aom_codec_control(ctx, AV1_INVERT_TILE_DECODE_ORDER, invert_order);

    return 0;
}

int LLVMFuzzerTestOneInput_233(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize decoder
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) return 0;

    aom_codec_ctx_t dec_ctx;
    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    // Removed invalid fields: allow_highbitdepth, allow_intrabc, allow_superres, allow_frame_parallel
    // These fields do not exist in the standard aom_codec_dec_cfg_t structure in this version.
    
    if (aom_codec_dec_init(&dec_ctx, decoder_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize encoder
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    if (!encoder_iface) {
        aom_codec_destroy(&dec_ctx);
        return 0;
    }

    aom_codec_ctx_t enc_ctx;
    aom_codec_enc_cfg_t enc_cfg;
    memset(&enc_cfg, 0, sizeof(enc_cfg));
    enc_cfg.g_w = 128;
    enc_cfg.g_h = 128;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_error_resilient = 1;

    if (aom_codec_enc_init(&enc_ctx, encoder_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(&dec_ctx);
        return 0;
    }

    // Fuzz decoder controls
    try_decoder_fuzz(&dec_ctx, Data, Size);

    // Fuzz encoder controls
    try_encoder_fuzz(&enc_ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&dec_ctx);
    aom_codec_destroy(&enc_ctx);

    return 0;
}