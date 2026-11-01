// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

// Define the control constants as integers since aom_codec_control_id_t is not available
// These values are placeholders to match the expected control IDs for libaom AV1 controls
#ifndef AOM_CODEC_CTRL_AV1D_GET_TILE_SIZE
#define AOM_CODEC_CTRL_AV1D_GET_TILE_SIZE 0x10000001
#endif
#ifndef AOM_CODEC_CTRL_AV1_INVERT_TILE_DECODE_ORDER
#define AOM_CODEC_CTRL_AV1_INVERT_TILE_DECODE_ORDER 0x10000002
#endif
#ifndef AOM_CODEC_CTRL_AV1_SET_DECODE_TILE_ROW
#define AOM_CODEC_CTRL_AV1_SET_DECODE_TILE_ROW 0x10000003
#endif
#ifndef AOM_CODEC_CTRL_AOMD_GET_TILE_INFO
#define AOM_CODEC_CTRL_AOMD_GET_TILE_INFO 0x10000004
#endif
#ifndef AOM_CODEC_CTRL_AV1E_SET_SINGLE_TILE_DECODING
#define AOM_CODEC_CTRL_AV1E_SET_SINGLE_TILE_DECODING 0x10000005
#endif
#ifndef AOM_CODEC_CTRL_AV1D_GET_TILE_DATA
#define AOM_CODEC_CTRL_AV1D_GET_TILE_DATA 0x10000006
#endif

static int invoke_decoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_err_t ret;
    aom_tile_info tile_info;
    aom_tile_data tile_data;
    int tile_row = 0;
    int invert_order = 0;
    int tile_size[2] = {0, 0};

    if (!ctx) return 0;

    // AV1D_GET_TILE_SIZE
    ret = aom_codec_control(ctx, AOM_CODEC_CTRL_AV1D_GET_TILE_SIZE, tile_size);
    (void)ret;

    // AV1_INVERT_TILE_DECODE_ORDER
    if (size > 0) invert_order = data[0] & 1;
    ret = aom_codec_control(ctx, AOM_CODEC_CTRL_AV1_INVERT_TILE_DECODE_ORDER, invert_order);
    (void)ret;

    // AV1_SET_DECODE_TILE_ROW
    if (size > 1) tile_row = (int)data[1];
    ret = aom_codec_control(ctx, AOM_CODEC_CTRL_AV1_SET_DECODE_TILE_ROW, tile_row);
    (void)ret;

    // AOMD_GET_TILE_INFO
    ret = aom_codec_control(ctx, AOM_CODEC_CTRL_AOMD_GET_TILE_INFO, &tile_info);
    (void)ret;

    // AV1D_GET_TILE_DATA
    ret = aom_codec_control(ctx, AOM_CODEC_CTRL_AV1D_GET_TILE_DATA, &tile_data);
    (void)ret;

    return 0;
}

static int invoke_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_err_t ret;
    int single_tile = 0;

    if (!ctx) return 0;

    // AV1E_SET_SINGLE_TILE_DECODING
    if (size > 0) single_tile = (int)data[0] & 1;
    ret = aom_codec_control(ctx, AOM_CODEC_CTRL_AV1E_SET_SINGLE_TILE_DECODING, single_tile);
    (void)ret;

    return 0;
}

int LLVMFuzzerTestOneInput_259(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx_dec, ctx_enc;
    aom_codec_dec_cfg_t dec_cfg;
    aom_codec_enc_cfg_t enc_cfg;
    aom_codec_iface_t *dec_iface, *enc_iface;
    aom_codec_err_t ret;

    if (Size == 0) return 0;

    // Initialize Decoder
    dec_iface = aom_codec_av1_dx();
    if (!dec_iface) return 0;

    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    dec_cfg.w = 32;
    dec_cfg.h = 32;

    ret = aom_codec_dec_init(&ctx_dec, dec_iface, &dec_cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize Encoder
    enc_iface = aom_codec_av1_cx();
    if (!enc_iface) {
        aom_codec_destroy(&ctx_dec);
        return 0;
    }

    memset(&enc_cfg, 0, sizeof(enc_cfg));
    enc_cfg.g_w = 32;
    enc_cfg.g_h = 32;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;

    ret = aom_codec_enc_init(&ctx_enc, enc_iface, &enc_cfg, 0);
    if (ret != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx_dec);
        return 0;
    }

    // Feed dummy data to decoder to reach a state where controls might be valid
    if (Size > 0) {
        aom_codec_dec_cfg_t dummy_cfg = {0};
        aom_codec_iface_t *dummy_iface = aom_codec_av1_dx();
        aom_codec_ctx_t dummy_ctx;
        if (dummy_iface) {
            ret = aom_codec_dec_init(&dummy_ctx, dummy_iface, &dummy_cfg, 0);
            if (ret == AOM_CODEC_OK) {
                aom_codec_decode(&dummy_ctx, Data, Size, NULL);
                aom_codec_destroy(&dummy_ctx);
            }
        }
    }

    // Invoke decoder controls
    invoke_decoder_controls(&ctx_dec, Data, Size);

    // Invoke encoder controls
    invoke_encoder_controls(&ctx_enc, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx_dec);
    aom_codec_destroy(&ctx_enc);

    return 0;
}