// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static int initialize_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (!initialize_encoder(&ctx, &cfg)) {
        return 0;
    }

    uint8_t val = Data[0];
    uint8_t val2 = Data[1];

    // Fuzz AV1E_SET_ENABLE_FILTER_INTRA
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, val) != AOM_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // Fuzz AV1E_SET_ENABLE_RECT_TX
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_TX, val) != AOM_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_COMP
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, val) != AOM_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // Fuzz AV1E_SET_TRANSFER_CHARACTERISTICS
    if (aom_codec_control(&ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, val) != AOM_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // Fuzz AV1E_SET_ENABLE_PALETTE
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, val) != AOM_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // Fuzz AV1E_SET_ENABLE_GLOBAL_MOTION
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val) != AOM_CODEC_OK) {
        // Ignore error, continue fuzzing
    }

    // Try to encode a dummy frame to trigger internal state changes
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 16, 16, 1);
    memset(img.planes[0], val, 16*16);
    memset(img.planes[1], val2, 8*8);
    memset(img.planes[2], val2, 8*8);

    aom_enc_frame_flags_t flags = 0;
    aom_codec_encode(&ctx, &img, 0, 1, flags);

    aom_codec_iter_t iter = NULL;
    aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&ctx, &iter))) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Do not free the frame here as it is managed by the codec context
            // aom_img_free(&pkt->data.frame_arg); 
        }
    }

    aom_img_free(&img);
    cleanup_encoder(&ctx);

    return 0;
}