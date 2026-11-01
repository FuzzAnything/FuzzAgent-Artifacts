// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
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

    // Use fuzz input to set some configuration parameters safely
    if (size > 0) cfg.g_w = (uint32_t)(data[0] & 0x3F) + 16; // Min 16
    if (size > 1) cfg.g_h = (uint32_t)(data[1] & 0x3F) + 16; // Min 16
    if (size > 2) cfg.g_timebase.num = (uint32_t)(data[2] & 0xFF) + 1;
    if (size > 3) cfg.g_timebase.den = (uint32_t)(data[3] & 0xFF) + 1;
    if (size > 4) cfg.g_threads = (uint32_t)(data[4] & 0xFF) + 1;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    // Fuzz AV1E_SET_ENABLE_INTRABC
    if (Size > 0) {
        int enable_intrabc = (Data[0] & 1);
        aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);
    }

    // Fuzz AV1E_SET_REDUCED_REFERENCE_SET
    if (Size > 1) {
        int reduced_ref = (Data[1] & 1);
        aom_codec_control(ctx, AV1E_SET_REDUCED_REFERENCE_SET, reduced_ref);
    }

    // Fuzz AV1E_SET_CHROMA_SAMPLE_POSITION
    if (Size > 2) {
        int chroma_pos = Data[2] % 3; // Valid range 0-2
        aom_codec_control(ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, chroma_pos);
    }

    // Fuzz AV1E_SET_TILE_ROWS
    if (Size > 3) {
        int tile_rows = (Data[3] % 8) + 1; // Reasonable range
        aom_codec_control(ctx, AV1E_SET_TILE_ROWS, tile_rows);
    }

    // Fuzz AV1E_SET_ENABLE_CFL_INTRA
    if (Size > 4) {
        int enable_cfl = (Data[4] & 1);
        aom_codec_control(ctx, AV1E_SET_ENABLE_CFL_INTRA, enable_cfl);
    }

    // Fuzz AV1E_SET_ENABLE_RECT_PARTITIONS
    if (Size > 5) {
        int enable_rect = (Data[5] & 1);
        aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, enable_rect);
    }

    // Encode a dummy frame
    aom_image_t img;
    // Ensure dimensions are valid based on config
    uint32_t w = ctx->config.enc->g_w;
    uint32_t h = ctx->config.enc->g_h;
    
    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, w, h, 1) != 0) {
        cleanup_encoder(ctx);
        return 0;
    }
    
    memset(img.planes[0], 128, img.d_w * img.d_h);
    memset(img.planes[1], 128, img.d_w * img.d_h / 4);
    memset(img.planes[2], 128, img.d_w * img.d_h / 4);

    // Corrected function call: aom_codec_encode(ctx, img, pts, duration, flags)
    aom_codec_encode(ctx, &img, 0, 1, 0);

    aom_img_free(&img);

    cleanup_encoder(ctx);

    return 0;
}