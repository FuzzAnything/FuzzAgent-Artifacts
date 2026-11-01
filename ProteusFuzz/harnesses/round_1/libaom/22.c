// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return 0;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return 0;
    }

    // Limit dimensions to prevent integer overflow during allocation and memset
    // Max reasonable dimension for fuzzing to avoid OOM and overflow
    #define MAX_DIM 4096
    
    cfg.g_w = (Size > 0) ? Data[0] : 64;
    cfg.g_h = (Size > 1) ? Data[1] : 64;

    // Validate dimensions
    if (cfg.g_w == 0 || cfg.g_w > MAX_DIM) cfg.g_w = 64;
    if (cfg.g_h == 0 || cfg.g_h > MAX_DIM) cfg.g_h = 64;

    cfg.g_timebase.num = (Size > 2) ? Data[2] : 1;
    cfg.g_timebase.den = (Size > 3) ? Data[3] : 30;
    if (cfg.g_timebase.num == 0) cfg.g_timebase.num = 1;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return 0;
    }

    aom_image_t img;
    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1) != 0) {
        cleanup_codec(ctx);
        return 0;
    }

    memset(img.planes[0], 0, img.stride[0] * img.d_h);
    memset(img.planes[1], 0, img.stride[1] * img.d_h);
    memset(img.planes[2], 0, img.stride[2] * img.d_h);

    if (Size > 0) {
        int32_t val = (int32_t)(Data[0]);
        aom_codec_control(ctx, AV1E_SET_QM_MIN, val);
    }
    if (Size > 1) {
        int32_t val = (int32_t)(Data[1]);
        aom_codec_control(ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val);
    }
    if (Size > 2) {
        int32_t val = (int32_t)(Data[2]);
        aom_codec_control(ctx, AV1E_SET_VBR_CORPUS_COMPLEXITY_LAP, val);
    }
    if (Size > 3) {
        int32_t val = (int32_t)(Data[3]);
        aom_codec_control(ctx, AV1E_SET_TILE_COLUMNS, val);
    }
    if (Size > 4) {
        int32_t val = (int32_t)(Data[4]);
        aom_codec_control(ctx, AOME_SET_SHARPNESS, val);
    }
    if (Size > 5) {
        int32_t val = (int32_t)(Data[5]);
        aom_codec_control(ctx, AV1E_SET_MODE_COST_UPD_FREQ, val);
    }

    aom_codec_frame_flags_t flags = 0;
    aom_codec_encode(ctx, &img, 0, 1, flags);

    aom_codec_iter_t iter = NULL;
    aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(ctx, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Process encoded frame data if needed
        }
    }

    aom_img_free(&img);
    cleanup_codec(ctx);

    return 0;
}