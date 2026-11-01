// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

/* Helper to initialize encoder */
static aom_codec_ctx_t *init_encoder(aom_codec_iface_t *iface, const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Use fuzzing data to set some config values */
    if (size > 0) cfg.g_w = (size % 4096) + 16;
    if (size > 1) cfg.g_h = (size % 4096) + 16;
    if (size > 2) cfg.g_timebase.num = (data[2] % 100) + 1;
    if (size > 3) cfg.g_timebase.den = (data[3] % 100) + 1;
    if (size > 4) cfg.rc_target_bitrate = (data[4] % 10000) + 1000;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

/* Helper to initialize decoder */
static aom_codec_ctx_t *init_decoder(aom_codec_iface_t *iface) {
    aom_codec_ctx_t *ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    return ctx;
}

int LLVMFuzzerTestOneInput_300(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Get AV1 encoder and decoder interfaces */
    aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    aom_codec_iface_t *dec_iface = aom_codec_av1_dx();

    if (!enc_iface || !dec_iface) return 0;

    /* Initialize Encoder */
    aom_codec_ctx_t *enc_ctx = init_encoder(enc_iface, Data, Size);
    if (!enc_ctx) return 0;

    /* Initialize Decoder */
    aom_codec_ctx_t *dec_ctx = init_decoder(dec_iface);
    if (!dec_ctx) {
        aom_codec_destroy(enc_ctx);
        free(enc_ctx);
        return 0;
    }

    /*
     * Attempt to control the encoder with various AV1 specific controls.
     * Using the correct enum values found in aomcx.h (e.g., AV1E_SET_...).
     */

    /* AV1E_SET_TILE_ROWS */
    if (Size > 0) {
        int tile_rows = Data[0] % 8; /* Max tile rows is limited */
        aom_codec_control(enc_ctx, AV1E_SET_TILE_ROWS, tile_rows);
    }

    /* AV1E_SET_SUPERBLOCK_SIZE */
    if (Size > 1) {
        int sb_size = Data[1] % 3; /* 0: 64x64, 1: 128x128, 2: 256x256 */
        aom_codec_control(enc_ctx, AV1E_SET_SUPERBLOCK_SIZE, sb_size);
    }

    /* AV1E_SET_ENABLE_INTRABC */
    if (Size > 2) {
        int enable = Data[2] % 2;
        aom_codec_control(enc_ctx, AV1E_SET_ENABLE_INTRABC, enable);
    }

    /* AV1D_SET_IS_ANNEXB */
    if (Size > 3) {
        int is_annexb = Data[3] % 2;
        aom_codec_control(dec_ctx, AV1D_SET_IS_ANNEXB, is_annexb);
    }

    /*
     * For AV1E_SET_QM_U and AV1E_SET_CHROMA_SUBSAMPLING_Y, we attempt to use
     * the enum names directly if they exist in the headers.
     * If the specific "typechecked" functions were meant to be called directly,
     * we cannot do so without their signatures. We assume they map to these
     * control IDs.
     */

    /* Attempt QM_U */
    #ifdef AV1E_SET_QM_U
    if (Size > 4) {
        int qm_u = Data[4] % 256;
        aom_codec_control(enc_ctx, AV1E_SET_QM_U, qm_u);
    }
    #endif

    /* Attempt CHROMA_SUBSAMPLING_Y */
    #ifdef AV1E_SET_CHROMA_SUBSAMPLING_Y
    if (Size > 5) {
        int chroma_y = Data[5] % 4;
        aom_codec_control(enc_ctx, AV1E_SET_CHROMA_SUBSAMPLING_Y, chroma_y);
    }
    #endif

    /* Cleanup */
    aom_codec_destroy(enc_ctx);
    aom_codec_destroy(dec_ctx);
    free(enc_ctx);
    free(dec_ctx);

    return 0;
}