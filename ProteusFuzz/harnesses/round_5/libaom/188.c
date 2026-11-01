// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_188(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to safely access indices up to 10
    if (Size < 12) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings safely
    cfg.g_w = (Data[0] % 128) + 16;
    cfg.g_h = (Data[1] % 128) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 100) + 1;
    cfg.g_threads = (Data[3] % 8) + 1;
    cfg.rc_target_bitrate = (Data[4] % 1000) + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_QM_U
    int qm_u = (Data[5] % 64);
    aom_codec_control(&ctx, AV1E_SET_QM_U, &qm_u);

    // Fuzz AV1E_SET_VBR_CORPUS_COMPLEXITY_LAP
    int vbr_corpus = (Data[6] % 100);
    aom_codec_control(&ctx, AV1E_SET_VBR_CORPUS_COMPLEXITY_LAP, &vbr_corpus);

    // Fuzz AV1E_SET_TILE_ROWS (Using TILE_ROWS as TILE_MODE is not standard)
    int tile_rows = (Data[7] % 8) + 1;
    aom_codec_control(&ctx, AV1E_SET_TILE_ROWS, &tile_rows);

    // Fuzz AV1E_SET_QM_MIN
    int qm_min = (Data[8] % 64);
    aom_codec_control(&ctx, AV1E_SET_QM_MIN, &qm_min);

    // Fuzz AV1E_SET_TILE_COLUMNS
    int tile_cols = (Data[9] % 8) + 1;
    aom_codec_control(&ctx, AV1E_SET_TILE_COLUMNS, &tile_cols);

    // Fuzz AOME_SET_SHARPNESS
    int sharpness = (Data[10] % 7);
    aom_codec_control(&ctx, AOME_SET_SHARPNESS, &sharpness);

    // Create a dummy frame to encode
    aom_image_t frame;
    if (aom_img_alloc(&frame, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1) != AOM_CODEC_OK) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Fill frame with fuzz data
    size_t y_size = cfg.g_w * cfg.g_h;
    size_t uv_size = (cfg.g_w * cfg.g_h) / 4;
    
    // Ensure we don't write out of bounds if Size is small, though we checked Size >= 12
    uint8_t fill_byte = Data[11];
    
    for (size_t i = 0; i < y_size; i++) {
        frame.planes[0][i] = fill_byte;
    }
    for (size_t i = 0; i < uv_size; i++) {
        frame.planes[1][i] = fill_byte;
        frame.planes[2][i] = fill_byte;
    }

    // Encode the frame
    aom_codec_encode(&ctx, &frame, 0, 1, 0);

    aom_img_free(&frame);
    cleanup_codec(&ctx);
    return 0;
}